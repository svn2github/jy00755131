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

/**
@fn bool query_instrument_test_limit()
@detail 查询 InstrumentTestLimit XML文件信息
@param[in]	void
@return 成功返回true, 否则返回false
@note	
*/
bool line_socket_impl::query_instrument_test_limit()
{
	bool is_ok = true;
	SOCKET_PACKAGE_SENT_PTR pack_ptr = NULL;
	CString info = __TEXT("");

	if(current_cmd_ != CURRENT_CMD_NULL){
		info.Format(IDS_STRING111,  __TEXT("CmdQueryInstrumentTestLimitXMLInfo"), current_cmd_);
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
	if( theApp.compare_application_status(application_login_status) < 0){
		info.Format(IDS_STRING137, theApp.get_application_status(), __TEXT("CmdQueryInstrumentTestLimitXMLInfo"));
		CLineApp::show_message(info);
		is_ok = false;
		goto GOTO;
	}

	//显示等待对话框
	info.Format(IDS_STRING148, __TEXT("查询 InstrumentTestLimit XML文件信息"));
	CLineApp::show_message(info);
	CLineApp::show_dialog(info);

	//向服务器发出查询测区信息
	pack_ptr = GetFreePackSent();
	ASSERT(pack_ptr != NULL);

	ZeroMemory(pack_ptr, sizeof(SOCKET_PACKAGE_SENT));
	SetPackageCmd(*pack_ptr, CmdQueryInstrumentTestLimitXMLInfo, 1);
	SetPackageNum(*pack_ptr, 1, 1);
	SetPackage(*pack_ptr, NULL, 0);
	pack_ptr->client_timestep_ = get_client_timestamp();
	pack_ptr->serial_id_ = get_client_pack_id();
	Send(pack_ptr, 1);
	is_timeout_ = false;
	current_cmd_ = CmdQueryInstrumentTestLimitXMLInfo;
	//设置CmdQueryRevSection超时
	//	time_id_ = SetTimer(NULL,  QUERY_RECV_SECTION_RESP_TIMER, MAX_TIMEOUT_SIZE, line_socket_impl::timer_proc );

GOTO:
	return is_ok;
}

/**
@fn void query_instrument_test_limit_resp(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
@detail 服务器应答客户端的查询请求
@param[in]	recv_pack_ptr 接受数据信息
@return void
@note	对于服务器响应的消息处理
*/
void line_socket_impl::query_instrument_test_limit_resp(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
{
	recv_pack_ptr = recv_pack_ptr;
	is_timeout_ = false;
	current_cmd_ = CmdQueryInstrumentTestLimitXMLInfo;
	timer_event_id_ = QUERY_INSTRUMENT_TEST_BASE_LIMIT_RESULT_TIMER;
	time_id_ = SetTimer(NULL,  0, MAX_TIMEOUT_SIZE, line_socket_impl::timer_proc );
}

/**
@fn void query_instrument_test_limit_result(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
@detail 服务器应答客户端的查询InstrumentTestLimit XML文件信息
@param[in]	recv_pack_ptr 接受数据信息
@return void
@note	服务器发送查询消息结果
*/
void line_socket_impl::query_instrument_test_limit_result(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr)
{
	char* buff_ptr = NULL;
	CString info;

	//判断是否超时
	if(!is_timeout_)	{	KillTimer(NULL, time_id_);		timer_event_id_ = time_id_ = 0x0;}	
	else						{	return;};

	//第一次收到信息分配内存
	if(recv_pack_ptr->index_ == 1){
		int size = recv_pack_ptr->num_ * FrameSizeLimit;
		buff_ptr = alloc_recv_pack_buff(size);
		if(buff_ptr == NULL){
			info.Format(IDS_STRING125, __TEXT("接收服务器推送的更新信息"));
			CLineApp::show_message(info);
			CLineApp::show_dialog(info, SW_HIDE, MAX_TIMEOUT_SIZE);
			current_cmd_ = CURRENT_CMD_NULL;
			return;
		}
	}
	else{
		buff_ptr = get_recv_pack_buff();
	}

	//显示提示信息
	CLineApp::show_message(IDS_STRING149);
	CLineApp::show_dialog(IDS_STRING149);

	memcpy(buff_ptr + current_recv_pack_buff_index_,  recv_pack_ptr->pack_, recv_pack_ptr->size_ );
	current_recv_pack_buff_index_ += recv_pack_ptr->size_ ;

	//判断是否可以解析数据包, 最后一个数据包开始解析
	if(recv_pack_ptr->index_ == recv_pack_ptr->num_){
		CLineApp::post_message(WM_QUERY_INSTRUMENT_TEST_BASE_LIMIT, (WPARAM)(buff_ptr),  static_cast<LPARAM>(current_recv_pack_buff_index_));

		//等待服务器发送查询结果回来
		CLineApp::show_message(IDS_STRING142);
		CLineApp::show_dialog(IDS_STRING142, SW_HIDE, MAX_TIMEOUT_SIZE);
		current_cmd_ = CURRENT_CMD_NULL;
		current_recv_pack_buff_index_ = 0x0;
	}
}
