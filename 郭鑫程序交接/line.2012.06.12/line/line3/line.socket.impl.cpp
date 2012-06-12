/**
@brief	该文件为测线socket操作类的实现文件
@details	
@file	line.socket.impl2.cpp
@note	继承：line.socket
@see	
@version	
@author	kuoxin	
@bug
*/

#include "StdAfx.h"
#include "line.socket.impl.h"
#include "line.h"

extern CLineApp theApp;

unsigned short line_socket_impl::current_cmd_ = line_socket_impl::CURRENT_CMD_NULL;
bool line_socket_impl::is_timeout_ = false	;
UINT_PTR line_socket_impl::time_id_ = 0x0;
unsigned int line_socket_impl::timer_event_id_ = 0x0;

VOID CALLBACK line_socket_impl::timer_proc(HWND hwnd,	  UINT uMsg,	 UINT_PTR idEvent, DWORD dwTime)
{
	line_socket_impl::current_cmd_ = line_socket_impl::CURRENT_CMD_NULL; 
	line_socket_impl::is_timeout_ = true;

	switch(line_socket_impl::timer_event_id_){
		case CLIENT_CONNECT_RESP_TIMER:// Client Connect timeout
			theApp.show_dialog(IDS_STRING143, SW_HIDE, MAX_TIMEOUT_SIZE);
			theApp.show_message(IDS_STRING143);
			theApp.set_application_status(application_free_status);
			break;
		case FIELD_ON_RESP_TIMER://Field On timeout
			theApp.show_dialog(IDS_STRING112, SW_HIDE, MAX_TIMEOUT_SIZE);
			theApp.show_message(IDS_STRING112);
			theApp.set_application_status(application_login_status);
		break;
		case FIELD_ON_WAIT_TIME_TIMER: //等待接受服务器FieldOn等待时间通知超时
			theApp.show_message(IDS_STRING115);
			theApp.show_dialog(IDS_STRING115, SW_HIDE, MAX_TIMEOUT_SIZE);
			theApp.set_application_status(application_login_status);
			break;
		case FIELD_OFF_RESP_TIMER: //field off timeout
			theApp.show_message(IDS_STRING123);
			theApp.show_dialog(IDS_STRING123, SW_HIDE, MAX_TIMEOUT_SIZE);
			theApp.set_application_status(application_field_on_after_status);
			break;
		case QUERY_INSTRUMENT_INFO_RESP_TIMER://CmdQueryInstrumentInfo response
			theApp.show_message(IDS_STRING130);
			theApp.show_dialog(IDS_STRING130, SW_HIDE, MAX_TIMEOUT_SIZE);
			break;
		case QUERY_INSTRUMENT_INFO_ALL_RESP_TIMER: // CmdQueryInstrumentInfoAll response
			theApp.show_message(IDS_STRING131);
			theApp.show_dialog(IDS_STRING131, SW_HIDE, MAX_TIMEOUT_SIZE);
			break;
		case QUERY_INSTRUMENT_INFO_TIMER:// wait for server's CmdQueryInstrumentInfo
			theApp.show_message(IDS_STRING134);
			theApp.show_dialog(IDS_STRING134, SW_HIDE, MAX_TIMEOUT_SIZE);
			break;
		case QUERY_INSTRUMENT_INFO_ALL_TIMER:// wait for server's CmdQueryInstrumentInfoAll 
			theApp.show_message(IDS_STRING135);
			theApp.show_dialog(IDS_STRING135, SW_HIDE, MAX_TIMEOUT_SIZE);
			break;
		case QUERY_RECV_SECTION_RESULT_TIMER://wait for server's CmdQueryRecvSection
			theApp.show_message(IDS_STRING141);
			theApp.show_dialog(IDS_STRING141, SW_HIDE, MAX_TIMEOUT_SIZE);
			break;
		case QUERY_INSTRUMENT_TEST_BASE_LIMIT_RESULT_TIMER:
			theApp.show_message(IDS_STRING147);
			theApp.show_dialog(IDS_STRING147, SW_HIDE, MAX_TIMEOUT_SIZE);
			break;
	}

	KillTimer(NULL, line_socket_impl::time_id_);
	line_socket_impl::time_id_ = 0x0;
	line_socket_impl::timer_event_id_ = 0x0;
}

line_socket_impl::line_socket_impl(CString ip, unsigned short port, unsigned int recv_buff_size):
															hit_test_thread_(NULL), recv_pack_buff_(NULL), recv_pack_buff_size_(0)

{
	ip_ = ip;
	port_ = port;
	recv_buff_size_ = recv_buff_size;
	hit_test_event_ = new CEvent(FALSE, TRUE);
	is_timeout_ = false;
	time_id_ = 0x0;
	timer_event_id_ = 0x0;
}

line_socket_impl::~line_socket_impl(void)
{
	if(time_id_){		KillTimer(NULL, time_id_); time_id_ = 0x0;}

	if(hit_test_event_){

		hit_test_event_->SetEvent();

		if(hit_test_thread_){
			end_heartbeat_thread();
			hit_test_thread_ = NULL;
		}

		delete hit_test_event_;
		hit_test_event_ = NULL;
	}

	if(recv_pack_buff_size_ > 0 && recv_pack_buff_ != NULL){
		delete []recv_pack_buff_;
		recv_pack_buff_ = NULL;
	}
}

/**
@fn UINT __cdecl hit_test_thread_proc(LPVOID pParam)
@detail 发送心跳操作给远程服务器,保持连接
@param[in]	pParam	传入当前类指针
@return 返回结束信息
@note	
*/
UINT __cdecl line_socket_impl::hit_test_thread_proc(LPVOID pParam)
{
	line_socket_impl* ls = (line_socket_impl*)pParam;
	SOCKET_PACKAGE_SENT_PTR pack_ptr = NULL;

	while(true){

		pack_ptr = ls->GetFreePackSent();
		ASSERT(pack_ptr != NULL);

		ZeroMemory(pack_ptr, sizeof(SOCKET_PACKAGE_SENT));
		ls->SetPackageCmd(*pack_ptr, CmdSetHeartBeat, 1);
		ls->SetPackageNum(*pack_ptr, 1, 1);
		ls->SetPackage(*pack_ptr, NULL, 0);
		pack_ptr->client_timestep_ = ls->get_client_timestamp();
		pack_ptr->serial_id_ = ls->get_client_pack_id();
		ls->Send(pack_ptr, 1);
		if(::WaitForSingleObject(ls->hit_test_event_->m_hObject, ls->HIT_TEST_TIME) == WAIT_OBJECT_0) break;
	}	

	AfxEndThread(0);
	return 0;
}

/**
@fn bool start_heartbeat_thread()
@detail 开始心跳线程维护服务器和客户端连接
@param[in] void
@return 如果成功将返回true, 否则返回false
@note	
*/
bool line_socket_impl::start_heartbeat_thread()
{
	bool is_ok = true;
	hit_test_thread_ = AfxBeginThread(hit_test_thread_proc, this);
	if(hit_test_thread_ == NULL) is_ok = false;
	return is_ok;
}

/**
@fn bool end_heartbeat_thread()
@detail 结束心跳连接
@param[in]	void
@return 如果成功将返回true, 否则返回false
@note	
*/
bool line_socket_impl::end_heartbeat_thread()
{
	HANDLE handle = NULL;

	if(hit_test_thread_){

		handle = hit_test_thread_->m_hThread;
		if(hit_test_event_){

			hit_test_event_->SetEvent();
			if(::WaitForSingleObject(handle, HIT_TEST_TIME * 2) != WAIT_OBJECT_0){ return false;}
		}
	}
	else{
		return false;
	}
	
	return true;
}

/**
@fn bool login_server()
@detail 链接服务器,向其发送
@param	void	
@return 如果发送成功将返回true, 否则将返回false
@note	
*/
bool line_socket_impl::connect_server()
{
	bool is_ok = true;
	CString info;
	CString::PXSTR str_ptr = NULL;
	SOCKET_PACKAGE_SENT_PTR pack_ptr = NULL;
	int i = 0;

	//显示等待对话框
	CLineApp::show_message(IDS_STRING144);
	CLineApp::show_dialog(IDS_STRING144);

	//链接服务器
	if(!Create()){
		CLineApp::show_message(IDS_STRING102);
		CLineApp::show_dialog(IDS_STRING102);
		is_ok = false;
		goto GOTO;
	}

	//连接服务器
	Connect(ip_, port_);

	//	i = 1;
	//unsigned long arpg = 0x0;
	//WSAAsyncSelect(m_hSocket, AfxGetModuleThreadState()->m_hSocketWindow, NULL, NULL);
	//ioctlsocket(m_hSocket, FIONBIO,  &arpg);
	//while(true){
	//	if(!Connect(ip_, port_)){
	//		if(GetLastError() == WSAEWOULDBLOCK)	break;
	//		if(i > MAX_REPEAT_COUNT) {
	//			CLineApp::show_message(IDS_STRING107);
	//			CLineApp::show_dialog(IDS_STRING107);
	//			//PostMessage(theApp.m_pMainWnd->m_hWnd, WM_QUIT, NULL, NULL);//exit this app
	//			//is_ok = false;
	//			goto GOTO;
	//		}
	//		else{
	//			info.Format(IDS_STRING103, ip_, port_, i+1);
	//			str_ptr = info.LockBuffer();
	//			CLineApp::show_message(str_ptr);
	//			CLineApp::show_dialog(str_ptr);
	//			info.UnlockBuffer();
	//			++i;
	//		}
	//	}
	//	else{ 		break;		}
	//}

	//arpg = 1;
	//ioctlsocket(m_hSocket, FIONBIO,  &arpg);
	//AsyncSelect(FD_READ | FD_WRITE | FD_CLOSE );

GOTO:
	return is_ok;
}

/**
@fn bool login_server()
@detail 链接服务器,向其发送
@param	void	
@return 如果发送成功将返回true, 否则将返回false
@note	
*/
bool line_socket_impl::login_server()
{
	bool is_ok = true;
	CString info = __TEXT("");
	SOCKET_PACKAGE_SENT_PTR pack_ptr = NULL;
	int i = 0;

	if(current_cmd_ != CURRENT_CMD_NULL){
		info.Format(IDS_STRING111,  __TEXT("CmdClientConnect"), current_cmd_);
		CLineApp::show_message(info);
		return is_ok = false;
	}

	//显示等待对话框
	CLineApp::show_message(IDS_STRING106);
	CLineApp::show_dialog(IDS_STRING106);

	//检查socket句柄是否有效
	if(m_hSocket == INVALID_SOCKET) {
		CLineApp::show_message(IDS_STRING105);
		CLineApp::show_dialog(IDS_STRING105);
		CLineApp::post_message(WM_QUIT, NULL, NULL);//exit this app
		return is_ok = false;
	}

	//当前连接状态 == application_login_status
	if( theApp.compare_application_status(application_free_status) != 0){
		info.Format(IDS_STRING110, theApp.get_application_status(), __TEXT("CmdClientConnect"));
		CLineApp::show_message(info);
		CLineApp::show_dialog(info, SW_HIDE, MAX_TIMEOUT_SIZE);
		return is_ok = false;
	}
	
	//登陆服务器
	pack_ptr = GetFreePackSent();
	ASSERT(pack_ptr != NULL);

	ZeroMemory(pack_ptr, sizeof(SOCKET_PACKAGE_SENT));
	SetPackageCmd(*pack_ptr, CmdClientConnect, 1);
	SetPackageNum(*pack_ptr, 1, 1);
	SetPackage(*pack_ptr, CommCheck, strlen(CommCheck) );
	pack_ptr->client_timestep_ = get_client_timestamp();
	pack_ptr->serial_id_ = get_client_pack_id();
	Send(pack_ptr, 1);
	is_timeout_ = false;
	current_cmd_ = CmdClientConnect;
//	timer_event_id_ = CLIENT_CONNECT_RESP_TIMER;
//	time_id_ = SetTimer(NULL, 0, MAX_TIMEOUT_SIZE, line_socket_impl::timer_proc );	
	return is_ok;
}

/**
@fn bool login_server_resp()
@detail 链接服务器,向其发送
@param	void	
@return 如果发送成功将返回true, 否则将返回false
@note	
*/
void line_socket_impl::login_server_resp(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
{
	CLineApp::show_message(IDS_STRING108);
	CLineApp::show_dialog(IDS_STRING108);
	current_cmd_ = CURRENT_CMD_NULL;
	theApp.set_application_status(application_login_status);
	theApp.post_message(WM_QUERY_RECV_SECTION);
}

/**
@fn bool field_on()
@detail filed on 操作
@param	void
@return 成功将返回true, 否则返回false
@note	
*/
bool line_socket_impl::field_on()
{
	bool is_ok = true;
	SOCKET_PACKAGE_SENT_PTR pack_ptr = NULL;
	enum APPLICATION_STATUS status;
	CString info = __TEXT("");

	if(current_cmd_ != CURRENT_CMD_NULL){
		info.Format(IDS_STRING111,  __TEXT("FieldOn"), current_cmd_);
		CLineApp::show_message(info);
		is_ok = false;
		goto GOTO;
	}

	//显示等待对话框
	CLineApp::show_message(IDS_STRING113);
//	CLineApp::show_dialog(IDS_STRING113);

	//检查socket句柄是否有效
	if(m_hSocket == INVALID_SOCKET) {
		CLineApp::show_message(IDS_STRING105);
//		CLineApp::show_dialog(IDS_STRING105);
		 CLineApp::post_message(WM_QUIT, NULL, NULL);//exit this app
		is_ok = false;
		goto GOTO;
	}

	//当前连接状态 == application_login_status
	if(theApp.compare_application_status(application_login_status) != 0){
		info.Format(IDS_STRING110, theApp.get_application_status(), __TEXT("CmdSetFiledOn"));
		CLineApp::show_message(info);
//		CLineApp::show_dialog(info, SW_HIDE, MAX_TIMEOUT_SIZE);
		is_ok = false;
		goto GOTO;
	}

	//服务器上电
	pack_ptr = GetFreePackSent();
	ASSERT(pack_ptr != NULL);

	ZeroMemory(pack_ptr, sizeof(SOCKET_PACKAGE_SENT));
	SetPackageCmd(*pack_ptr, CmdSetFieldOn, 1);
	SetPackageNum(*pack_ptr, 1, 1);
	SetPackage(*pack_ptr, NULL, 0);
	pack_ptr->client_timestep_ = get_client_timestamp();
	pack_ptr->serial_id_ = get_client_pack_id();
	Send(pack_ptr, 1);
	is_timeout_ = false;
	current_cmd_ = CmdSetFieldOn;
	//设置FieldOn超时
//	time_id_ = SetTimer(NULL, FIELD_ON_RESP_TIMER, MAX_TIMEOUT_SIZE, line_socket_impl::timer_proc );

GOTO:
	return is_ok;
}

/**
@fn void field_on_resp(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
@detail 服务器响应客户端
@param	recv_pack_ptr	接收数据包内容
@return void
@note	服务器响应的消息处理
*/
void line_socket_impl::field_on_resp(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
{
	CString info = __TEXT("");

	//关闭FieldOn超时定时器
//	if(!is_timeout_){		KillTimer(NULL, time_id_); time_id_ = 0x0;}
//	else{
//		info.Format(IDS_STRING119,  current_cmd_, __TEXT("CmdFieldOnWaitTime"));
//		CLineApp::show_message(info);
////		CLineApp::show_dialog(info, SW_HIDE, MAX_TIMEOUT_SIZE);
//		return ;
//	}

	//显示信息在对话框上
	CLineApp::show_message(IDS_STRING116);

	//显示等待服务器CmdFieldOnWaitTime信息
	CLineApp::show_message(IDS_STRING117);
//	CLineApp::show_dialog(IDS_STRING117);
	is_timeout_ = false;
	current_cmd_ = CmdFieldOnWaitTime;
	//设置FieldOnWaitTime超时
	timer_event_id_ = FIELD_ON_WAIT_TIME_TIMER;
	time_id_ = SetTimer(NULL,  0, MAX_TIMEOUT_SIZE, line_socket_impl::timer_proc);
}

/**
@fn void field_on_wait_time(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
@detail 客户端等待服务器发送CmdFieldOnWaitTime通知
@param	recv_pack_ptr	接收数据包内容
@return void
@note	 由服务器发出的消息处理
*/
void line_socket_impl::field_on_wait_time(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
{
	int field_on_wait_time = 0x0;
	CString info = __TEXT("");
	//停止超时操作
	if(!is_timeout_){		KillTimer(NULL, time_id_); time_id_ = timer_event_id_ = 0x0;}
	else{
		info.Format(IDS_STRING119,  current_cmd_, __TEXT("CmdFieldOnWaitTime"));
		CLineApp::show_message(info);
//		CLineApp::show_dialog(info, SW_HIDE, MAX_TIMEOUT_SIZE);
		goto GOTO;
	}

	//判断当前状态是不是CmdSetFieldOn
	if(current_cmd_ != CmdFieldOnWaitTime){
		info.Format(IDS_STRING118,  current_cmd_, __TEXT("CmdFieldOnWaitTime"));
		CLineApp::show_message(info);
//		CLineApp::show_dialog(info, SW_HIDE, MAX_TIMEOUT_SIZE);
		goto GOTO;
	}
	//应答服务器
	//pack_ptr = GetFreePackSent();
	//ASSERT(pack_ptr != NULL);

	//ZeroMemory(pack_ptr, sizeof(SOCKET_PACKAGE_SENT));
	//SetPackageCmd(*pack_ptr, CmdSetFieldOn, 2);
	//SetPackageNum(*pack_ptr, 1, 1);
	//SetPackage(*pack_ptr, NULL, 0);
	//pack_ptr->client_timestep_ = get_client_timestamp();
	//pack_ptr->serial_id_ = get_client_pack_id();
	//pack_ptr->server_timestep_ = recv_pack_ptr->server_timestep_;
	//Send(pack_ptr, 1);

	//分析收到数据包
	current_cmd_ = CURRENT_CMD_NULL;
	timer_event_id_ = 0x0;
	time_id_ = 0x0;
	memcpy(&field_on_wait_time ,  recv_pack_ptr->pack_, sizeof(field_on_wait_time));
	if(field_on_wait_time == 0){
		CLineApp::show_message(IDS_STRING120);
		theApp.set_application_status(application_field_on_after_status);
		theApp.post_message(WM_FIELD_ON_COMPLETE);
		//开始心跳
		start_heartbeat_thread();
	}
	else{
		info.Format(IDS_STRING121, field_on_wait_time);
		CLineApp::show_message(info);
		theApp.set_application_status(application_login_status);
		theApp.post_message(WM_FIELD_ON_COMPLETE, field_on_wait_time);
	}

GOTO:
	return;
}

/**
@fn bool field_off()
@detail field off 操作
@param	void
@return 成功将返回true, 否则返回false
@note	
*/
bool line_socket_impl::field_off()
{
	bool is_ok = true;
	SOCKET_PACKAGE_SENT_PTR pack_ptr = NULL;

	//检查socket句柄是否有效
	if(m_hSocket == INVALID_SOCKET) {
		CLineApp::show_message(IDS_STRING105);
		CLineApp::post_message(WM_QUIT, NULL, NULL);//exit this app
		is_ok = false;
		goto GOTO;
	}

	//当前连接状态
	if(theApp.compare_application_status(application_field_on_after_status)<0){
		CLineApp::show_message(IDS_STRING122);
		is_ok = false;
		goto GOTO;
	}

	//显示等待对话框
	CLineApp::show_message(IDS_STRING114);
//	CLineApp::show_dialog(IDS_STRING114);

	//服务器下电
	pack_ptr = GetFreePackSent();
	ASSERT(pack_ptr != NULL);

	ZeroMemory(pack_ptr, sizeof(SOCKET_PACKAGE_SENT));
	SetPackageCmd(*pack_ptr, CmdSetFieldOff, 1);
	SetPackageNum(*pack_ptr, 1, 1);
	SetPackage(*pack_ptr, NULL, 0);
	pack_ptr->client_timestep_ = get_client_timestamp();
	pack_ptr->serial_id_ = get_client_pack_id();
	Send(pack_ptr, 1);

	//结束心跳
	end_heartbeat_thread();

	//current_cmd_ = CmdSetFieldOff;
	//is_timeout_ = false;

	////设置Field Off定时器
	//time_id_ = SetTimer(NULL, FIELD_OFF_RESP_TIMER, MAX_TIMEOUT_SIZE, timer_proc);

GOTO:
	return is_ok;
}

/**
@fn void field_off_resp(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
@detail 服务器响应客户端
@param	recv_pack_ptr	接收数据包内容
@return void
@note	对于服务器响应的消息处理
*/
void line_socket_impl::field_off_resp(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
{
	//判断是否超时
	//if(is_timeout_)	{	goto GOTO;}
	//else						{	KillTimer(NULL, time_id_); time_id_ = 0x0;}

	//显示信息在对话框上
	CLineApp::show_message(IDS_STRING124);
//	CLineApp::show_dialog(IDS_STRING124, SW_HIDE, MAX_TIMEOUT_SIZE);

	//设置当前客户端状态
	theApp.set_application_status(application_login_status);

GOTO:
	return;
}

/**
@fn void notify_update_table(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
@detail 服务器发送更新信息
@param	recv_pack_ptr	接收数据包内容
@return void
@note	服务器推送消息处理
*/
void line_socket_impl::notify_update_table(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
{
	char* buff_ptr = NULL;
	CString info;

	if(recv_pack_ptr->index_ == 1){
		int size = recv_pack_ptr->num_ * FrameSizeLimit;
		buff_ptr = alloc_recv_pack_buff(size);
		if(buff_ptr == NULL){
			info.Format(IDS_STRING125, __TEXT("接收服务器推送的更新信息"));
			theApp.show_message(info);
			current_cmd_ = CURRENT_CMD_NULL;
			goto GOTO;
		}
	}
	else{
		buff_ptr = get_recv_pack_buff();
	}

	//显示提示信息
//	CLineApp::show_message(IDS_STRING152);

	memcpy(buff_ptr + current_recv_pack_buff_index_,  recv_pack_ptr->pack_, recv_pack_ptr->size_ );
	current_recv_pack_buff_index_ += recv_pack_ptr->size_ ;

	//判断是否可以解析数据包, 最后一个数据包开始解析
	if(recv_pack_ptr->index_ == recv_pack_ptr->num_){
		CLineApp::post_message(WM_UPDATE_DEVICE_TABLE, (WPARAM)(buff_ptr),  static_cast<LPARAM>(current_recv_pack_buff_index_));

		//等待服务器发送查询结果回来
//		CLineApp::show_message(IDS_STRING151);
		current_cmd_ = CURRENT_CMD_NULL;
		current_recv_pack_buff_index_ = 0x0;
		is_timeout_ = false;
	}

GOTO:
	return;
}

/**
@fn char* alloc_recv_pack_buff(int size)
@detail 分配内存为接受多个分包情况
@param[in]	内存大小(byte为单位)
@return 返回分配内存的指针,如果不能分配的话将返回NULL
@note	
*/
char* line_socket_impl::alloc_recv_pack_buff(int size)
{
	if(size == 0) return NULL;

	if(size <= recv_pack_buff_size_){
		ZeroMemory(recv_pack_buff_, recv_pack_buff_size_);
		goto GOTO;
	}
	else{
		if(recv_pack_buff_ != NULL)			delete []recv_pack_buff_;

		recv_pack_buff_size_ = size;
		recv_pack_buff_ = new char[recv_pack_buff_size_];
		ZeroMemory(recv_pack_buff_,  recv_pack_buff_size_);
	}

	current_recv_pack_buff_index_ = 0x0;

GOTO:
	return recv_pack_buff_;
}

/**
@fn bool query_instrument_info(int line_id, int zone_id)
@detail 得到制定行号和区域号上的设备信息
@param[in] line_id 行号
@param[in] zone_id 区域编号
@return 成功发送请求返回true,否则返回false
@note	
*/
bool line_socket_impl::query_instrument_info(int line_id, int zone_id)
{	
	bool is_ok = true;
	SOCKET_PACKAGE_SENT_PTR pack_ptr = NULL;
	enum APPLICATION_STATUS status = application_free_status;
	CString info = __TEXT("");
	int data[2] = {line_id, zone_id};

	if(current_cmd_ != CURRENT_CMD_NULL){
		info.Format(IDS_STRING111,  __TEXT("CmdQueryInstrumentInfo"), current_cmd_);
		CLineApp::show_message(info);
		is_ok = false;
		goto GOTO;
	}

	//检查socket句柄是否有效
	if(m_hSocket == INVALID_SOCKET) {
		CLineApp::show_message(IDS_STRING105);
		CLineApp::post_message(WM_QUIT, NULL, NULL);//exit this app
		is_ok = false;
		goto GOTO;
	}

	//当前连接状态 == application_field_on_after_status
	status = theApp.get_application_status();
	if( theApp.compare_application_status(application_field_on_after_status) != 0){
		info.Format(IDS_STRING137, theApp.get_application_status(), __TEXT("CmdQueryInstrumentInfo"));
		CLineApp::show_message(info);
		is_ok = false;
		goto GOTO;
	}

	//显示等待对话框
	CLineApp::show_message(IDS_STRING127);
	CLineApp::show_dialog(IDS_STRING127);

	//向服务器发出查询请求
	pack_ptr = GetFreePackSent();
	ASSERT(pack_ptr != NULL);

	ZeroMemory(pack_ptr, sizeof(SOCKET_PACKAGE_SENT));
	SetPackageCmd(*pack_ptr, CmdSetFieldOn, 1);
	SetPackageNum(*pack_ptr, 1, 1);
	SetPackage(*pack_ptr, data, sizeof(data));
	pack_ptr->client_timestep_ = get_client_timestamp();
	pack_ptr->serial_id_ = get_client_pack_id();
	Send(pack_ptr, 1);
	is_timeout_ = false;
	current_cmd_ = CmdQueryInstrumentInfo;
	//设置CmdQueryInstrumentInfo超时
	timer_event_id_ = QUERY_INSTRUMENT_INFO_RESP_TIMER;
	time_id_ = SetTimer(NULL, 0 , MAX_TIMEOUT_SIZE, line_socket_impl::timer_proc );

GOTO:
	return is_ok;
}

/**
@fn void query_instrument_info_resp(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
@detail 处理服务器的应答信息
@param[in] recv_pack_ptr 应答数据包
@return void
@note	
*/
void line_socket_impl::query_instrument_info_resp(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
{
	//判断是否超时
	if(is_timeout_)	goto GOTO;
	else						KillTimer(NULL, time_id_);

	//显示信息在对话框上
	CLineApp::show_message(IDS_STRING132);
	CLineApp::show_dialog(IDS_STRING132, SW_HIDE, MAX_TIMEOUT_SIZE);

	//等待服务器发送查询结果回来
	CLineApp::show_message(IDS_STRING135);
	CLineApp::show_dialog(IDS_STRING135);

	current_cmd_ = CmdQueryInstrumentInfo;
	is_timeout_ = false;

	//设置定时器
	timer_event_id_ = QUERY_INSTRUMENT_INFO_TIMER;
	time_id_ = SetTimer(NULL, 0, MAX_TIMEOUT_SIZE, timer_proc);

GOTO:
	return;
}

/**
@fn void recv_querying_instrument_info(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
@detail 接受服务器发送过来的设备信息
@param	recv_pack_ptr	接收数据包内容
@return void
@note	对于服务器响应的消息处理
*/
void line_socket_impl::recv_querying_instrument_info(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
{
	char *buff_ptr = NULL;
	CString info = __TEXT("");
	//向服务器发出查询请求
	SOCKET_PACKAGE_SENT_PTR pack_ptr = NULL;

	//判断是否超时
	if(is_timeout_)	goto GOTO;
	else						KillTimer(NULL, time_id_);

	//应答服务器
	pack_ptr = GetFreePackSent();
	ASSERT(pack_ptr != NULL);

	ZeroMemory(pack_ptr, sizeof(SOCKET_PACKAGE_SENT));
	SetPackageCmd(*pack_ptr, recv_pack_ptr->cmd_, 3);
	SetPackageNum(*pack_ptr, recv_pack_ptr->num_,  recv_pack_ptr->index_ + 1);
	SetPackage(*pack_ptr, NULL, 0);
	pack_ptr->client_timestep_ = get_client_timestamp();
	pack_ptr->serial_id_ = recv_pack_ptr->serial_id_;
	pack_ptr->server_timestep_ = recv_pack_ptr->server_timestep_;
	Send(pack_ptr, 1);

	//得到查询结果数据,有可能存在返回多个数据应答包
	if(recv_pack_ptr->index_ == 0){
		int size = recv_pack_ptr->num_ * FrameSizeLimit;
		buff_ptr = alloc_recv_pack_buff(size);
		if(buff_ptr == NULL){
			info.Format(IDS_STRING125, __TEXT("接收服务器返回查询结果信息"));
			theApp.show_message(info);
			goto GOTO;
		}
	}
	else{
		buff_ptr = get_recv_pack_buff();
	}

	//拷贝到预分配内存
	memcpy(buff_ptr + current_recv_pack_buff_index_,  recv_pack_ptr->pack_, recv_pack_ptr->size_ );
	current_recv_pack_buff_index_ += recv_pack_ptr->size_ ;

	//判断是否可以解析数据包, 最后一个数据包开始解析
	if(recv_pack_ptr->index_ + 1 == recv_pack_ptr->num_){	
		theApp.query_instrument_info(buff_ptr, current_recv_pack_buff_index_); 
	}
	else{
		is_timeout_ = false;
		current_cmd_ = CmdQueryInstrumentInfo;
		timer_event_id_ = QUERY_INSTRUMENT_INFO_TIMER;
		time_id_ = SetTimer(NULL, 0,  MAX_TIMEOUT_SIZE, timer_proc);
	}

GOTO:
	return;
}

/**
@fn bool query_instrument_info_all()
@detail 得到所有设备信息
@param[in] void
@return 成功发送请求返回true,否则返回false
@note	
*/
bool line_socket_impl::query_instrument_info_all()
{	
	bool is_ok = true;
	SOCKET_PACKAGE_SENT_PTR pack_ptr = NULL;
	CString info = __TEXT("");

	//检查socket句柄是否有效
	if(m_hSocket == INVALID_SOCKET) {
		CLineApp::show_message(IDS_STRING105);
		CLineApp::show_dialog(IDS_STRING105);
		CLineApp::post_message(WM_QUIT, NULL, NULL);//exit this app
		is_ok = false;
		goto GOTO;
	}

	//当前连接状态 == application_login_status
	if( theApp.compare_application_status(application_field_on_after_status) != 0){
		info.Format(IDS_STRING137, theApp.get_application_status(), __TEXT("CmdQueryInstrumentInfoAll"));
		CLineApp::show_message(info);
		is_ok = false;
		goto GOTO;
	}

	//显示等待对话框
	CLineApp::show_message(IDS_STRING136);
	CLineApp::show_dialog(IDS_STRING136);

	//向服务器发出查询请求
	pack_ptr = GetFreePackSent();
	ASSERT(pack_ptr != NULL);

	ZeroMemory(pack_ptr, sizeof(SOCKET_PACKAGE_SENT));
	SetPackageCmd(*pack_ptr, CmdQueryInstrumentInfoAll, 1);
	SetPackageNum(*pack_ptr, 1, 1);
	SetPackage(*pack_ptr, NULL, 0);
	pack_ptr->client_timestep_ = get_client_timestamp();
	pack_ptr->serial_id_ = get_client_pack_id();
	Send(pack_ptr, 1);

	//设置CmdQueryInstrumentInfo超时
	is_timeout_ = false;
	current_cmd_ = CmdQueryInstrumentInfoAll;
	timer_event_id_ = QUERY_INSTRUMENT_INFO_ALL_RESP_TIMER;
	time_id_ = SetTimer(NULL,  0, MAX_TIMEOUT_SIZE, line_socket_impl::timer_proc );

GOTO:
	return is_ok;
}

/**
@fn void send_socket_message(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
@detail 重载父类的函数
@param	recv_pack_ptr	接收数据包内容
@return void
@note	对于服务器响应的消息处理
*/
void line_socket_impl::query_instrument_info_all_resp(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
{
	//判断是否超时
	if(is_timeout_)	goto GOTO;
	else						KillTimer(NULL, time_id_);

	//等待服务器发送查询结果回来
	CLineApp::show_message(IDS_STRING138);
	CLineApp::show_dialog(IDS_STRING138);

	//设置定时器
	current_cmd_ = CmdQueryInstrumentInfoAll;
	is_timeout_ = false;
	timer_event_id_ = QUERY_INSTRUMENT_INFO_ALL_TIMER ;
	time_id_ = SetTimer(NULL, 0, MAX_TIMEOUT_SIZE, timer_proc);

GOTO:
	return;
}

/**
@fn void send_socket_message(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
@detail 重载父类的函数
@param	recv_pack_ptr	接收数据包内容
@return void
@note	对于服务器响应的消息处理
*/
void  line_socket_impl::recv_querying_instrument_info_all(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
{
	char *buff_ptr = NULL;
	CString info = __TEXT("");
	SOCKET_PACKAGE_SENT_PTR pack_ptr = NULL;

	//判断是否超时
	if(is_timeout_)	goto GOTO;
	else						KillTimer(NULL, time_id_);

	//应答服务器
	pack_ptr = GetFreePackSent();
	ASSERT(pack_ptr != NULL);

	ZeroMemory(pack_ptr, sizeof(SOCKET_PACKAGE_SENT));
	SetPackageCmd(*pack_ptr, recv_pack_ptr->cmd_, 3);
	SetPackageNum(*pack_ptr, recv_pack_ptr->num_,  recv_pack_ptr->index_ + 1);
	SetPackage(*pack_ptr, NULL, 0);
	pack_ptr->client_timestep_ = get_client_timestamp();
	pack_ptr->serial_id_ = recv_pack_ptr->serial_id_;
	pack_ptr->server_timestep_ = recv_pack_ptr->server_timestep_;
	Send(pack_ptr, 1);

	//得到查询结果数据,有可能存在返回多个数据应答包
	if(recv_pack_ptr->index_ == 0){
		int size = recv_pack_ptr->num_ * FrameSizeLimit;
		buff_ptr = alloc_recv_pack_buff(size);
		if(buff_ptr == NULL){
			info.Format(IDS_STRING125, __TEXT("接收服务器返回查询结果信息"));
			theApp.show_message(info);
			goto GOTO;
		}
	}
	else{
		buff_ptr = get_recv_pack_buff();
	}

	//拷贝到预分配内存
	memcpy(buff_ptr + current_recv_pack_buff_index_,  recv_pack_ptr->pack_, recv_pack_ptr->size_ );
	current_recv_pack_buff_index_ += recv_pack_ptr->size_ ;

	//判断是否可以解析数据包, 最后一个数据包开始解析
	if(recv_pack_ptr->index_ + 1 == recv_pack_ptr->num_){	
		theApp.query_instrument_info_all(buff_ptr, current_recv_pack_buff_index_); 
	}
	else{
		is_timeout_ = false;
		current_cmd_ = CmdQueryInstrumentInfoAll;
		timer_event_id_ = QUERY_INSTRUMENT_INFO_ALL_TIMER;
		time_id_ = SetTimer(NULL, 0,  MAX_TIMEOUT_SIZE, timer_proc);
	}

GOTO:
	return;
}

/**
@fn void send_socket_message(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
@detail 重载父类的函数
@param	recv_pack_ptr	接收数据包内容
@return void
@note	对于服务器响应的消息处理
*/
void line_socket_impl::send_socket_message(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
{
	switch(recv_pack_ptr->cmd_){
	case CmdClientConnect:
		//是否为响应消息,并且是否等于当前最后一个命令请求
		if(current_cmd_ != CmdClientConnect || recv_pack_ptr->cmd_type_ != 3)		return;
		login_server_resp(recv_pack_ptr);
		break;
	case CmdSetFieldOff:
		if(current_cmd_ != CmdSetFieldOff || recv_pack_ptr->cmd_type_ != 3)		return;
		field_off_resp(recv_pack_ptr);
		break;
	case CmdSetFieldOn:
		if(current_cmd_ != CmdSetFieldOn || recv_pack_ptr->cmd_type_ != 3)		return;
		field_on_resp(recv_pack_ptr);
		break;
	case CmdFieldOnWaitTime:
		if(current_cmd_ != CmdFieldOnWaitTime )		return;
		field_on_wait_time(recv_pack_ptr);
		break;
	case CmdQueryInstrumentInfo:
		if(recv_pack_ptr->cmd_type_ == 3){
			if(current_cmd_ == CmdQueryInstrumentInfo)	query_instrument_info_resp(recv_pack_ptr);
		}
		else{
			recv_querying_instrument_info(recv_pack_ptr);
		}
		break;
	case CmdQueryInstrumentInfoAll:
		if(recv_pack_ptr->cmd_type_ == 3){
			if(current_cmd_ == CmdQueryInstrumentInfoAll)	query_instrument_info_all_resp(recv_pack_ptr);
		}
		else{
			recv_querying_instrument_info_all(recv_pack_ptr);
		}
		break;
	case CmdQueryUpdateTable:
		notify_update_table(recv_pack_ptr);
		break;
	case CmdQueryRevSection:
		if(recv_pack_ptr->cmd_type_ == 3){
			if(current_cmd_ == CmdQueryRevSection)	query_recv_section_resp(recv_pack_ptr);
		}
		else{	query_recv_section_result(recv_pack_ptr);}
		break;
	case CmdQueryInstrumentTestLimitXMLInfo:
		if(recv_pack_ptr->cmd_type_ == 3){
			if(current_cmd_ == CmdQueryInstrumentTestLimitXMLInfo)	query_instrument_test_limit_resp(recv_pack_ptr);
		}
		else{	query_instrument_test_limit_result(recv_pack_ptr);}
		break;
	default:
		break;
	}
}

/**
@fn void OnConnect(int nErrorCode)
@detail 重载父类的函数
@param	nErrorCode 返回错误信息
@return void
@note	对于服务器响应的消息处理
*/
void line_socket_impl::OnConnect(int nErrorCode)
{
	LPTSTR str_ptr = NULL;
	CString info = __TEXT("");
	static int i = 1;

	if(nErrorCode == 0){
		CLineApp::post_message(WM_LOGIN_SERVER, NULL, NULL);
	}
	else{
			if(i >= MAX_REPEAT_COUNT) {
				CLineApp::show_message(IDS_STRING107);
				CLineApp::show_dialog(IDS_STRING107);
				PostMessage(theApp.m_pMainWnd->m_hWnd, WM_QUIT, NULL, NULL);//exit this app
			}
			else{
				info.Format(IDS_STRING103, ip_, port_, i+1);
				str_ptr = info.LockBuffer();
				CLineApp::show_message(str_ptr);
				CLineApp::show_dialog(str_ptr);
				info.UnlockBuffer();
				++i;
				Connect(ip_, port_);
			}
	}
	line_socket::OnConnect(nErrorCode);
}

/**
@fn bool query_recv_section()
@detail 查询测区信息
@param	void
@return true 返回正确,否则返回错误
@note	
*/
bool line_socket_impl::query_recv_section()
{
	bool is_ok = true;
	SOCKET_PACKAGE_SENT_PTR pack_ptr = NULL;
	enum APPLICATION_STATUS status = application_free_status;
	CString info = __TEXT("");

	if(current_cmd_ != CURRENT_CMD_NULL){
		info.Format(IDS_STRING111,  __TEXT("CmdQueryRecvSection"), current_cmd_);
		CLineApp::show_message(info);
		is_ok = false;
		goto GOTO;
	}

	//检查socket句柄是否有效
	if(m_hSocket == INVALID_SOCKET) {
		CLineApp::show_message(IDS_STRING105);
		CLineApp::post_message(WM_QUIT, NULL, NULL);//exit this app
		is_ok = false;
		goto GOTO;
	}

	//当前连接状态 == application_field_on_after_status
	if( theApp.compare_application_status(application_login_status) != 0){
		info.Format(IDS_STRING137, theApp.get_application_status(), __TEXT("CmdQueryRecvSection"));
		CLineApp::show_message(info);
		is_ok = false;
		goto GOTO;
	}

	//显示等待对话框
	CLineApp::show_message(IDS_STRING140);
	CLineApp::show_dialog(IDS_STRING140);

	//向服务器发出查询测区信息
	pack_ptr = GetFreePackSent();
	ASSERT(pack_ptr != NULL);

	ZeroMemory(pack_ptr, sizeof(SOCKET_PACKAGE_SENT));
	SetPackageCmd(*pack_ptr, CmdQueryRevSection, 1);
	SetPackageNum(*pack_ptr, 1, 1);
	SetPackage(*pack_ptr, NULL, 0);
	pack_ptr->client_timestep_ = get_client_timestamp();
	pack_ptr->serial_id_ = get_client_pack_id();
	Send(pack_ptr, 1);
	is_timeout_ = false;
	current_cmd_ = CmdQueryRevSection;
	//设置CmdQueryRevSection超时
	//	time_id_ = SetTimer(NULL,  QUERY_RECV_SECTION_RESP_TIMER, MAX_TIMEOUT_SIZE, line_socket_impl::timer_proc );

GOTO:
	return is_ok;
}

/**
@fn void query_recv_section_result(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
@detail 服务器应答客户端的查询测区信息
@param[in]	recv_pack_ptr 接受数据信息
@return void
@note	对于服务器返回查询消息处理
*/
void line_socket_impl::query_recv_section_result(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
{
	int data[2] = {0};

	//判断是否超时
	if(!is_timeout_)	{	KillTimer(NULL, time_id_);		timer_event_id_ = time_id_ = 0x0;}	
	else						{	return;};

	//等待服务器发送查询结果回来
	CLineApp::show_message(IDS_STRING142);
	CLineApp::show_dialog(IDS_STRING142, SW_HIDE, 1000);
	memcpy(data, recv_pack_ptr->pack_, sizeof(data));
	CLineApp::post_message(WM_QUERY_RECV_SECTION_RESULT, data[0], data[1]);
	current_cmd_ = CURRENT_CMD_NULL;
	timer_event_id_ = 0x0;
	time_id_ = 0x0;

	//向服务器发出查询测区信息
	//SOCKET_PACKAGE_SENT_PTR pack_ptr = GetFreePackSent();
	//ASSERT(pack_ptr != NULL);

	//ZeroMemory(pack_ptr, sizeof(SOCKET_PACKAGE_SENT));
	//SetPackageCmd(*pack_ptr, CmdQueryRevSection, 3);
	//SetPackageNum(*pack_ptr, 1, 1);
	//SetPackage(*pack_ptr, NULL, 0);
	//pack_ptr->client_timestep_ = get_client_timestamp();
	//pack_ptr->serial_id_ = recv_pack_ptr->serial_id_;
	//pack_ptr->server_timestep_ = recv_pack_ptr->server_timestep_;
	//Send(pack_ptr, 1);
}

/**
@fn void query_recv_section_resp(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
@detail 服务器应答客户端的查询测区信息
@param[in]	recv_pack_ptr 接受数据信息
@return void
@note	对于服务器响应的消息处理
*/
void line_socket_impl::query_recv_section_resp(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
{
	is_timeout_ = false;
	current_cmd_ = CmdQueryRevSection;
	timer_event_id_ = QUERY_RECV_SECTION_RESULT_TIMER;
	time_id_ = SetTimer(NULL,  0, MAX_TIMEOUT_SIZE, line_socket_impl::timer_proc );
}

void line_socket_impl::OnClose(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
	end_heartbeat_thread();
	line_socket::OnClose(nErrorCode);
}

