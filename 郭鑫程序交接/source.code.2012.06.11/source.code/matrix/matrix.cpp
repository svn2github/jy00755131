#include "..\common\config.h"
#include "..\common\typedef.h"
#include "..\common\sockimpl.h"
#include "heartbeat.proc.h"
#include "the.first.package.proc.h"
#include "ip.setting.resp.h"
#include "the.first.package.check.h"
#include "the.end.package.proc.h"
#include "the.end.package.time.delay.proc.h"
#include "the.end.package.time.delay.setting.proc.h"
#include "common.functions.h"
#include "equipment.comm.order.h"
#include "ad.operation.resp.h"
#include "ad.operation.proc.h"
#include "ad.collection.resp.h"
#include "matrix.h"

#pragma warning(disable: 4100)

// Global Variables:
EQUIPMENT_ITEM __meta_items[MAX_EQUIPMENT_SIZE];
EQUIPMENT_ITEM_PTR __free_items; //没有使用设备
EQUIPMENT_ITEM_PTR __work_items; //已经使用设备
int __work_items_size;	//使用设备个数

LING_ARRAY_ITEM __line_array[MAX_BIG_LINE_SIZE]; //大线数组，标志每条大线上设备

unsigned long volatile __the_first_package_idled_counter; //没有收到数据，如果收到+1, 否则等于zero

//bool volatile __be_the_cross_line_of_the_end_package_proc_completed;//是否已经完成交叉线上的尾包处理,这可能包括两个方向A,B
THE_END_PACKAGE_PROC_RUNNING __the_end_package_proc_running; //某条线路(交叉线A,B; 大线A,B)上尾包处理正在进行中...
unsigned long volatile __line_no;//标记当前分配的大线数量

//hash ip & serial.no & route 
stdext_hash_map __serial_no_hash_map;
stdext_hash_map __ip_hash_map;
stdext_hash_map __route_hash_map;

THE_HEART_BEAT_PACKAGE_INFO __heartbeat_data;

APPLICATION_ENVIRONMENT __application_environment;
static const TCHAR __application_version[] = __TEXT("Rev: 1.0.12.55.2011.8.30.EIGHT.PORTS.1K");
bool __be_ad_collection_stopping; //是否停止AD采集

#include "ad.collection.proc.inl"
#include "ad.collection.stop.proc.inl"
#include "ad.offset.proc .inl"
#include "ad.setting.proc .inl"

void output_log(unsigned int id, ...)
{
	int size = 0;
	char buf[MAX_STRING_SIZE * 10] = {0};
	char format[MAX_STRING_SIZE * 10] = {0};
	va_list ap;

	size = get_string(id, format, sizeof(format));
	format[size] = 0x0;

	va_start(ap, id);
	_vstprintf_s(buf, format, ap);
	va_end(ap);

	__application_environment.log_->output_log(buf);
}


int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPTSTR    lpCmdLine,
					 int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	char host_name[MAX_STRING_SIZE]={0};
	struct hostent* hosttent_ptr = NULL;

	// Initialize global strings
	memset(&__application_environment, 0x0, sizeof(__application_environment));

	//log system
	__application_environment.log_ = new log;
	__application_environment.log_->init("e:\\matrix.log", log::LOG_FILE_TYPE);//NULL, log::CONSOLE_TYPE);

	//socket
	init_socket();

	//get local ip address
	gethostname(host_name, sizeof(host_name));
	hosttent_ptr = gethostbyname(host_name);
	__application_environment.ip_ =  *(u_long *) hosttent_ptr->h_addr_list[0];

	__application_environment.instance_ = hInstance;
	__application_environment.main_window_ = CreateDialogParam(__application_environment.instance_, 
		MAKEINTRESOURCE(IDD_MATRIX_DIALOG), NULL, main_Wnd_proc, NULL);

	if(!__application_environment.main_window_)	return FALSE;

	ShowWindow(__application_environment.main_window_, nCmdShow);
	UpdateWindow(__application_environment.main_window_);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)){

		if(!IsDialogMessage(__application_environment.main_window_, &msg)){

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

BOOL WINAPI main_Wnd_proc(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	switch (message){

		HANDLE_DIALOG_MESSAGE(window_handle, WM_INITDIALOG, init_dialog);
		HANDLE_DIALOG_MESSAGE(window_handle, WM_CLOSE, close_dialog);
		HANDLE_DIALOG_MESSAGE(window_handle, WM_COMMAND, command_message);
		HANDLE_DIALOG_MESSAGE(window_handle, WM_DESTROY, destroy_dialog);
		case WM_THE_END_PACKAGE_RECV_MESSAGE: 
			return on_receive_the_end_package(wParam, lParam);
		case WM_THE_END_PACKAGE_TIME_DELAY_RECV_MESSAGE:
			return on_receive_the_end_time_delay_package(wParam, lParam);
		case WM_HEARTBEAT_PACKAGE_MESSAGE:
			return on_receive_heartbeat_package(wParam, lParam);
	}

	return FALSE;
}

BOOL init_dialog(HWND window_handle, HWND default_window_handle, LPARAM param)
{
	UNREFERENCED_PARAMETER(param);
	UNREFERENCED_PARAMETER(default_window_handle);

	__application_environment.main_window_ = window_handle;

	SetWindowText(__application_environment.main_window_, __application_version);
	//read option info
	set_window_control(false);

	return TRUE;
}

BOOL destroy_dialog(HWND window_handle)
{
	UNREFERENCED_PARAMETER(window_handle);
	
	//socket
	release_socket();

	PostQuitMessage(0);
	return 0;
}

BOOL close_dialog(HWND window_handle)
{
	if(is_running(&__application_environment))
		SendMessage(window_handle, WM_COMMAND, ID_START_BUTTON, NULL);

	//release log

	return DestroyWindow(window_handle);
}

BOOL command_message(HWND window_handle, int resource_id, HWND window_control_handle, UINT notification_code)
{
	BOOL is_ok = TRUE;
	is_ok = do_control(window_handle, resource_id, window_control_handle, notification_code);
	return is_ok;
}

VOID CALLBACK check_ad_collection_recv_proc(HWND, UINT, UINT_PTR, DWORD)
{
	KillTimer(__application_environment.main_window_, AD_COLLECTION_RECV_EVENT_ID);

	proc_ad_collection_resp();
	SetTimer(__application_environment.main_window_, AD_COLLECTION_RECV_EVENT_ID, 
			AD_COLLECTION_RECV_TIMEOUT_VALUE, check_ad_collection_recv_proc);
}

VOID CALLBACK check_ad_setting_timer_proc(HWND, UINT, UINT_PTR, DWORD)
{
	KillTimer(__application_environment.main_window_, AD_SETTING_PROC_EVENT_ID);

	if(!proc_ad_setting()){
		SetTimer(__application_environment.main_window_, AD_SETTING_PROC_EVENT_ID, 
			AD_SETTING_PROC_TIMEOUT_VALUE, check_ad_setting_timer_proc);
	}
	else{
		release_ad_setting_proc();
		SendMessage(GET_WINDOW_HANDLE(IDC_AD_SETTING_BUTTON), WM_SETTEXT, NULL, (LPARAM)"OK");
	}
}

VOID CALLBACK check_ad_end_timer_proc(HWND, UINT, UINT_PTR, DWORD)
{
	KillTimer(__application_environment.main_window_, AD_STOP_COLLECTION_PROC_EVENT_ID);

	if(!proc_ad_end()){
		SetTimer(__application_environment.main_window_, AD_STOP_COLLECTION_PROC_EVENT_ID, 
			AD_SETTING_PROC_TIMEOUT_VALUE, check_ad_end_timer_proc);
	}
	else{

		//停止AD采集定时器
		KillTimer(__application_environment.main_window_, AD_COLLECTION_RECV_EVENT_ID);

		release_ad_collecion_proc();
		release_ad_end_proc();
		SendMessage(GET_WINDOW_HANDLE(IDC_STOP_COLLECTION_BUTTON), WM_SETTEXT, NULL, (LPARAM)"OK");
	}
}

VOID CALLBACK check_ad_collection_timer_proc(HWND, UINT, UINT_PTR, DWORD)
{
	KillTimer(__application_environment.main_window_, AD_SETTING_PROC_EVENT_ID);

	if(!proc_ad_collection()){	
		SetTimer(__application_environment.main_window_, AD_SETTING_PROC_EVENT_ID, 
			AD_SETTING_PROC_TIMEOUT_VALUE, check_ad_collection_timer_proc);
	}
	else{
		release_ad_collecion_proc();
		SendMessage(GET_WINDOW_HANDLE(IDC_AD_COLLECTION_BUTTON), WM_SETTEXT, NULL, (LPARAM)"OK");

		if(!init_ad_collection_resp(8, AD_PORT_LOWER))	output_log(IDS_STRING161, GetLastError());	
		else{
			output_log(IDS_STRING162, AD_COLLECTION_RECV_TIMEOUT_VALUE);
			SetTimer(__application_environment.main_window_, AD_COLLECTION_RECV_EVENT_ID,
				AD_COLLECTION_RECV_TIMEOUT_VALUE, check_ad_collection_recv_proc);
		}
	}
}

VOID CALLBACK check_ad_offset_timer_proc(HWND, UINT, UINT_PTR, DWORD)
{
	KillTimer(__application_environment.main_window_, AD_SETTING_PROC_EVENT_ID);

	if(!proc_ad_offset()){	

		SetTimer(__application_environment.main_window_, AD_SETTING_PROC_EVENT_ID, 
			AD_SETTING_PROC_TIMEOUT_VALUE, check_ad_offset_timer_proc);
	}
	else{
		release_ad_offset_proc();
		SendMessage(GET_WINDOW_HANDLE(IDC_ZERO_OFFSET_BUTTON), WM_SETTEXT, NULL, (LPARAM)"OK");
	}
}

void ad_setting_button_down()
{
	init_ad_setting_proc(); 
	SetTimer(__application_environment.main_window_, AD_SETTING_PROC_EVENT_ID, 
		AD_SETTING_PROC_TIMEOUT_VALUE, check_ad_setting_timer_proc);

}

void ad_collection_button_down()
{
	init_ad_collection_proc(); 
	SetTimer(__application_environment.main_window_, AD_SETTING_PROC_EVENT_ID, 
		AD_SETTING_PROC_TIMEOUT_VALUE, check_ad_collection_timer_proc);
}

void ad_zero_offset_button_down()
{
	init_ad_offset_proc();
	SetTimer(__application_environment.main_window_, AD_SETTING_PROC_EVENT_ID, 
		AD_SETTING_PROC_TIMEOUT_VALUE, check_ad_offset_timer_proc);
}

void ad_zero_end_button_down()
{
	init_ad_end_proc();
	SetTimer(__application_environment.main_window_, AD_STOP_COLLECTION_PROC_EVENT_ID, 
		AD_SETTING_PROC_TIMEOUT_VALUE, check_ad_end_timer_proc);
}


BOOL do_control(HWND window_handle, int resource_id, HWND window_control_handle, UINT notification_code)
{
	UNREFERENCED_PARAMETER(window_handle);
	UNREFERENCED_PARAMETER(window_control_handle);
	UNREFERENCED_PARAMETER(notification_code);

	BOOL is_ok = TRUE;

	switch(resource_id){
		case ID_START_BUTTON:
			start_button_down();
			break;
		case IDC_AD_SETTING_BUTTON:
			ad_setting_button_down();
			break;
		case IDC_AD_COLLECTION_BUTTON:
			ad_collection_button_down();
			break;
		case IDC_ZERO_OFFSET_BUTTON:
			ad_zero_offset_button_down();
			break;
		case IDC_STOP_COLLECTION_BUTTON:
			ad_zero_end_button_down();
			break;
		case IDCANCEL:
			PostMessage(__application_environment.main_window_, WM_CLOSE, NULL, NULL);
		default: break;
	}

	return is_ok;
}

int get_string(UINT id, LPTSTR buf, unsigned size)
{
	return LoadString(__application_environment.instance_, id, buf, size);
}

void exchange_status(APPLICATION_ENVIRONMENT_PTR env_ptr, bool is_running)
{
	env_ptr->is_running_ = is_running;
}

bool is_running(APPLICATION_ENVIRONMENT_PTR env_ptr)
{
	return env_ptr->is_running_;
}

bool is_stopping(APPLICATION_ENVIRONMENT_PTR env_ptr)
{
	return env_ptr->is_running_ ? false : true;
}

BOOL start_button_down()
{
	int len = 0;
	TCHAR buf[MAX_STRING_SIZE] = {0};

	if(is_stopping(&__application_environment)){//start

		//reset the counter
		exchange_status(&__application_environment, true);

		//get options
		set_window_control(true);

		//set button text
		len = get_string(IDS_END, buf, sizeof(buf));
		buf[len] = 0x0;
		SendMessage(GET_WINDOW_HANDLE(ID_START_BUTTON), WM_SETTEXT, NULL, (LPARAM)buf);
		
		//go!
		start_matrix_proc(&__application_environment);
	}
	else{ //stop

		//stop
		stop_matrix_proc();

		//set button text
		len = get_string(IDS_START, buf, sizeof(buf));
		buf[len] = 0x0;
		SendMessage(GET_WINDOW_HANDLE(ID_START_BUTTON), WM_SETTEXT, NULL, (LPARAM)buf);		
		//set stop status
		exchange_status(&__application_environment, false);
	}

	return TRUE;
}

BOOL set_window_control(bool is_save)
{

	if(is_save){

	}
	else{

	}

	return TRUE;
}

bool get_uint_value(UINT id, unsigned* result)
{	
	bool is_ok = true;
	int len = 0;
	TCHAR buf[MAX_STRING_SIZE] = {0};
	TCHAR* stop_buf_ptr = NULL;

	len = LoadString(__application_environment.instance_, id, buf, sizeof(buf));
	if(len == 0){
		*result = 0x0;
		return is_ok = false;
	}
	else {
		buf[len] = 0x0;
		*result = _tcstoul(buf, &stop_buf_ptr, 10);
	}

	return is_ok;
}

bool get_ushort_value(UINT id, unsigned short* result)
{
	unsigned tmp_result = 0x0;
	bool is_ok = get_uint_value(id, &tmp_result);

	if(is_ok) *result = static_cast<unsigned short>(tmp_result);

	return is_ok;
}

int show_message_box(HWND window_handle, UINT text_id,UINT type)
{
	TCHAR buf[MAX_STRING_SIZE] = {0};

	get_string(text_id, buf, sizeof(buf));

	if(window_handle == NULL) window_handle = __application_environment.main_window_;

	return MessageBox(window_handle, buf, __TEXT(""), type);
}

void init_matrix_proc()
{
	int i = 0;
	int meta_size = sizeof(__meta_items) / sizeof(__meta_items[0]);
	EQUIPMENT_ITEM_PTR equipment_item_ptr = NULL;

	SecureZeroMemory(__meta_items, sizeof(__meta_items));
	equipment_item_ptr = __free_items = __meta_items;	

	for(i = 1; i < meta_size; ++i){

		equipment_item_ptr->right_ = __meta_items + i;
		equipment_item_ptr = equipment_item_ptr->right_;
	}

	__work_items_size = 0x0;
	__line_no = 0x0;

	SecureZeroMemory(__line_array, sizeof(__line_array));
	__serial_no_hash_map.clear();
	__ip_hash_map.clear();
	__route_hash_map.clear();	
}

void release_matrix_proc()
{
	__serial_no_hash_map.clear();
	__ip_hash_map.clear();
	__route_hash_map.clear();	

	SecureZeroMemory(__line_array, sizeof(__line_array));

	__work_items_size = 0x0;
	__free_items = NULL;
	__work_items = NULL;

	SecureZeroMemory(__meta_items, sizeof(__meta_items));	 
}

bool start_matrix_proc(APPLICATION_ENVIRONMENT_PTR app_env_ptr)
{
	init_matrix_proc();

	//heartbeat
	init_heartbeat_proc(SOCKET_BOARDCAST_ADDRESS);
	init_the_first_package_proc(LCI_IP_ADDRESS);
	init_ip_setting_resp();

	init_the_end_package_proc(SOCKET_BOARDCAST_ADDRESS);
	init_the_end_package_time_delay_proc(NULL);
	init_the_end_package_time_delay_setting_proc();

	//set timer for heartbeat & the first package
	SetTimer(__application_environment.main_window_, HEARTBEAT_EVENT_ID, 
		HEARTBEAT_TIMEOUT_VALUE, the_heartbeat_timer_proc);

	SetTimer(__application_environment.main_window_, IP_SETTING_RESP_EVENT_ID, 
		IP_SETTING_RESP_TIMEOUT_VALUE, check_ip_setting_resp_timer_proc);

	SetTimer(__application_environment.main_window_, CHECK_THE_FIRST_PACKAGE_EVENT_ID,
		CHECK_THE_FIRST_PACKAGE_TIMEOUT_VALUE, check_the_first_package_timer);


	return true;
}

bool stop_matrix_proc()
{
	//kill heartbeat & first package timer
	KillTimer(__application_environment.main_window_, HEARTBEAT_EVENT_ID);
	KillTimer(__application_environment.main_window_, IP_SETTING_RESP_EVENT_ID);
	KillTimer(__application_environment.main_window_, CHECK_THE_FIRST_PACKAGE_EVENT_ID);

	release_the_first_package_proc();

	//heartbeat
	release_heartbeat_proc();
	release_ip_setting_resp();

	//tail pack
	release_the_end_package_proc();
	release_the_end_package_time_delay_proc();
	release_the_end_package_time_delay_setting_proc();

	release_matrix_proc();

	return true;
}

VOID CALLBACK the_heartbeat_timer_proc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	KillTimer(__application_environment.main_window_, HEARTBEAT_EVENT_ID);
	
	//heartbeat proceduce
	heartbeat_proc();
	the_first_package_proc();

	//start again
	SetTimer(__application_environment.main_window_, HEARTBEAT_EVENT_ID, 
		HEARTBEAT_TIMEOUT_VALUE, the_heartbeat_timer_proc);

}

VOID CALLBACK check_the_first_package_timer(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	KillTimer(__application_environment.main_window_, CHECK_THE_FIRST_PACKAGE_EVENT_ID);
	check_the_first_package();
	SetTimer(__application_environment.main_window_, CHECK_THE_FIRST_PACKAGE_EVENT_ID,
		CHECK_THE_FIRST_PACKAGE_TIMEOUT_VALUE, check_the_first_package_timer);
}

VOID CALLBACK check_ip_setting_resp_timer_proc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	KillTimer(__application_environment.main_window_, IP_SETTING_RESP_EVENT_ID);

	proc_ip_setting_resp();

	SetTimer(__application_environment.main_window_, IP_SETTING_RESP_EVENT_ID, 
		IP_SETTING_RESP_TIMEOUT_VALUE, check_ip_setting_resp_timer_proc);
}

VOID CALLBACK proc_time_delay_setting_resp(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	KillTimer(__application_environment.main_window_, TIME_DELAY_SETTING_RESP_EVENT_ID);

	if(!proc_the_tail_package_time_delay_setting())
		SetTimer(__application_environment.main_window_, TIME_DELAY_SETTING_RESP_EVENT_ID,
			TIME_DELAY_SETTING_RESP_TIMEOUT_VALUE, proc_time_delay_setting_resp);
}




