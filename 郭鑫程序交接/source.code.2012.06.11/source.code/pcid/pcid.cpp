
#include "../common/config.h"
#include "../common/log.h"
#include "pcid.h"
#include "registry.optr.h"
#include "filter.optr.h"
#include "options.windw.Impl.h"

// Global Variables:
APPLICATION_ENVIRONMENT __application_environment;
static const TCHAR __application_version[] = __TEXT("Rev: 1.2.33.202.2011.08.12.20MHZ.FIFO.SERIAL.NO");

inline void OUTPUT_DEBUG_STRING(const char* info) { __application_environment.log_->output_log(info); };

void OUTPUT_DEBUG_STRING(unsigned int id, ...)
{
	int size = 0;
	char buf[MAX_STRING_SIZE * 20] = {0};
	char format[MAX_STRING_SIZE] = {0};
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

	// Initialize global strings
	memset(&__application_environment, 0x0, sizeof(__application_environment));

	__application_environment.instance_ = hInstance;
	__application_environment.log_ = new segd::log;
	__application_environment.log_->init(NULL, segd::log::CONSOLE_TYPE);

	__application_environment.main_window_ = CreateDialogParam(__application_environment.instance_, 
												MAKEINTRESOURCE(IDD_PCID_DIALOG), NULL, main_Wnd_proc, NULL);

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
		HANDLE_DIALOG_MESSAGE(window_handle, WM_TIMER, on_timer);
	}

	return FALSE;
}

BOOL init_dialog(HWND window_handle, HWND default_window_handle, LPARAM param)
{
	UNREFERENCED_PARAMETER(param);
	UNREFERENCED_PARAMETER(default_window_handle);

	__application_environment.main_window_ = window_handle;
	__application_environment.be_started_ = PCI_WORK_STOPPED;

	SetWindowText(__application_environment.main_window_, __application_version);
	read_registry(&__application_environment);
	set_window_control(false);

	return TRUE;
}

BOOL destroy_dialog(HWND window_handle)
{
	UNREFERENCED_PARAMETER(window_handle);
	PostQuitMessage(0);
	return 0;
}

BOOL close_dialog(HWND window_handle)
{
	if(is_running(&__application_environment))
		SendMessage(window_handle, WM_COMMAND, IDC_BTN_DI, NULL);

	//release log
	__application_environment.log_->release();
	delete __application_environment.log_;
	__application_environment.log_ = NULL;

	if(__application_environment.ip_filter_){

		free(__application_environment.ip_filter_);
		__application_environment.ip_filter_ = NULL;
	}

	if(__application_environment.port_filter_){

		free(__application_environment.port_filter_);
		__application_environment.port_filter_ = NULL;
	}

	return DestroyWindow(window_handle);
}

BOOL command_message(HWND window_handle, int resource_id, HWND window_control_handle, UINT notification_code)
{
	BOOL is_ok = TRUE;
	is_ok = do_control(window_handle, resource_id, window_control_handle, notification_code);
	return is_ok;
}

BOOL do_control(HWND window_handle, int resource_id, HWND window_control_handle, UINT notification_code)
{
	UNREFERENCED_PARAMETER(window_handle);
	UNREFERENCED_PARAMETER(window_control_handle);
	UNREFERENCED_PARAMETER(notification_code);

	BOOL is_ok = TRUE;

	switch(resource_id){
		case IDC_BTN_DI:
			start_end_button_down();
			break;
		case IDC_KEY_BUTTON:
			key_button_down();
			break;
		case IDC_BTN_SAVEDATA:
			PostMessage(window_handle, WM_CLOSE, NULL, NULL);
			break;
		case IDC_BTN_SEND_ONE_FRAME:
			OUTPUT_DEBUG_STRING("SND");
			break;
		case IDC_ASSIGN_IP_PORT_BUTTON:
			assign_ip_button_down();
		case IDC_CRC_CHECK:
			;
			break;
	}

	return is_ok;
}

BOOL assign_ip_button_down()
{
	INT_PTR result = 0x0;

	read_filter(&__application_environment);
	result = DialogBoxParam(__application_environment.instance_,
							MAKEINTRESOURCE(IDD_OPTIONS_DIALOG),
							__application_environment.main_window_,
							dialog_options_proc,	
							NULL);	

	if(result){

#ifdef __DEBUG_IT__
		test_filter_driver(&__application_environment);
#endif
		write_filter(&__application_environment);
	}	


	return TRUE;
}


int get_string(UINT id, LPTSTR buf, unsigned size)
{
	return LoadString(__application_environment.instance_, id, buf, size);
}

void exchange_status(APPLICATION_ENVIRONMENT_PTR env_ptr, bool is_running)
{
	if(is_running)
		_InterlockedCompareExchange(&env_ptr->be_started_, PCI_WORK_STARTED, PCI_WORK_STOPPED);
	else
		_InterlockedCompareExchange(&env_ptr->be_started_, PCI_WORK_STOPPED, PCI_WORK_STARTED);
}

bool is_running(APPLICATION_ENVIRONMENT_PTR env_ptr)
{
	bool is_ok = false;
	return is_ok = (_InterlockedCompareExchange(&env_ptr->be_started_, PCI_WORK_STARTED, 
												PCI_WORK_STARTED) == PCI_WORK_STARTED);
};

bool is_stopping(APPLICATION_ENVIRONMENT_PTR env_ptr)
{	
	bool is_ok = false;
	return is_ok = (_InterlockedCompareExchange(&env_ptr->be_started_, PCI_WORK_STOPPED, 
												PCI_WORK_STOPPED) == PCI_WORK_STOPPED);
}

BOOL start_end_button_down()
{
	int len = 0;
	TCHAR buf[MAX_STRING_SIZE] = {0};

	if(is_stopping(&__application_environment)){//start

		//reset the counter
		InterlockedExchange(&__application_environment.pci_input_num_, 0);
		InterlockedExchange(&__application_environment.pci_output_num_, 0);
		InterlockedExchange(&__application_environment.upd_recv_num_, 0);
		InterlockedExchange(&__application_environment.upd_send_num_, 0);
		InterlockedExchange(&__application_environment.udp_send_serial_no_, 0);
		exchange_status(&__application_environment, true);

		//get options
		set_window_control(true);

		//write all setting into registry
		write_registry(&__application_environment);

		//set button text
		len = get_string(IDS_END, buf, sizeof(buf));
		buf[len] = 0x0;
		SendMessage(GET_WINDOW_HANDLE(IDC_BTN_DI), WM_SETTEXT, NULL, (LPARAM)buf);


#if defined(__DEBUG_PCI_WRITE_PORT_PERFORMANCE__)
		__application_environment.pci_writing_performance_log_ = new log;
		__application_environment.pci_writing_performance_log_->init("e:\\writing.perf.log", log::LOG_FILE_TYPE);
#endif //defined(__DEBUG_PCI_WRITE_PORT_PERFORMANCE__)

		//set timer
		SetTimer(__application_environment.main_window_, IDT_TIMER, 100, NULL);

		//init filter info
		init_filter(&__application_environment);

		//go!
		start_simple_proc(&__application_environment);
	}
	else{ //stop

		//stop
		stop_simple_proc();

		//stop timer
		KillTimer(__application_environment.main_window_, IDT_TIMER);

		//set button text
		len = get_string(IDS_START, buf, sizeof(buf));
		buf[len] = 0x0;
		SendMessage(GET_WINDOW_HANDLE(IDC_BTN_DI), WM_SETTEXT, NULL, (LPARAM)buf);

		//set stop status
		exchange_status(&__application_environment, false);


#if defined(__DEBUG_PCI_WRITE_PORT_PERFORMANCE__)
		if(__application_environment.pci_writing_performance_log_){

			__application_environment.pci_writing_performance_log_->release();
			delete __application_environment.pci_writing_performance_log_;
			__application_environment.pci_writing_performance_log_ = NULL;
		}
#endif //defined(__DEBUG_PCI_WRITE_PORT_PERFORMANCE__)

	}

	return TRUE;
}

BOOL key_button_down()
{
	static bool is_be_locked = false;
	BOOL is_enable = FALSE;

	if(is_be_locked)	is_be_locked = false;
	else				is_be_locked = true;

	is_enable = is_be_locked ? FALSE : TRUE;

	for(int i = IDC_BTN_DI; i <= IDC_EDIT_SOCK_POLL_TIME; ++i)
		EnableWindow(GET_WINDOW_HANDLE(i), is_enable);
	
	return TRUE;
}

BOOL on_timer(HWND window_handle, UINT id)
{
	UNREFERENCED_PARAMETER(id);

	char buf[MAX_STRING_SIZE] = {0};
	LONG tmp[5]={0};
	tmp[0] = _InterlockedCompareExchange(&__application_environment.pci_input_num_, 0, 0);
	tmp[1] = _InterlockedCompareExchange(&__application_environment.pci_output_num_, 0, 0);
	tmp[2] = _InterlockedCompareExchange(&__application_environment.upd_recv_num_, 0, 0);
	tmp[3] = _InterlockedCompareExchange(&__application_environment.upd_send_num_, 0, 0);
	tmp[4] = _InterlockedCompareExchange(&__application_environment.serial_no_, 0, 0);

	for(int i = 0; i < 4; ++i){
		
		if(i == 0)	sprintf_s(buf, sizeof(buf), "%d (Á÷Ë®ºÅ£º%d)", tmp[0], tmp[4]);
		else	sprintf_s(buf, sizeof(buf), "%d", tmp[i]);
		SendMessage(GetDlgItem(window_handle, IDC_STATIC_PCI_INPUT_NUM + i), WM_SETTEXT, NULL, (LPARAM)buf);
	}

	return TRUE;
}

BOOL set_window_control(bool is_save)
{
	char buf[MAX_STRING_SIZE] = {0};
	LONG result = 0x0;

	if(is_save){
		
		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_LISTEN_PORT), WM_GETTEXT, (WPARAM)sizeof(buf), (LPARAM)buf);
		__application_environment.listen_port_ = atoi(buf);

		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_REMOTE_IP), WM_GETTEXT, 
			sizeof(__application_environment.remote_ip_), (LPARAM)__application_environment.remote_ip_);

		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_REMOTE_PORT), WM_GETTEXT, (WPARAM)sizeof(buf), (LPARAM)buf);
		__application_environment.remote_port_ = atoi(buf);

		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_LOG_PORT), WM_GETTEXT, (WPARAM)sizeof(buf), (LPARAM)buf);
		__application_environment.log_port_ = atoi(buf);

		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_LOG_IP), WM_GETTEXT, 
			(WPARAM)sizeof(__application_environment.log_ip_), (LPARAM)__application_environment.log_ip_);

		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_PCI_BUFFER_NUM), WM_GETTEXT, (WPARAM)sizeof(buf), (LPARAM)buf);
		__application_environment.parallel_port_data_input_buffer_num_ = atoi(buf);

		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_PCI_BUFFER_SIZE), WM_GETTEXT, (WPARAM)sizeof(buf), (LPARAM)buf);
		__application_environment.parallel_port_data_input_buffer_size_ = atoi(buf);

		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_PCI_POLL_TIME), WM_GETTEXT, (WPARAM)sizeof(buf), (LPARAM)buf);
		__application_environment.parallel_port_input_waiting_time_ = atoi(buf);

		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_PCI_WRITE_BUFFER_SIZE), WM_GETTEXT, (WPARAM)sizeof(buf), (LPARAM)buf);
		__application_environment.parallel_port_output_buffer_size_ = atoi(buf);

		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_SOCK_RECV_NUM), WM_GETTEXT, (WPARAM)sizeof(buf), (LPARAM)buf);
		__application_environment.recv_buff_size_ = atoi(buf);

		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_SOCK_SEND_NUM), WM_GETTEXT, (WPARAM)sizeof(buf), (LPARAM)buf);
		__application_environment.send_buff_size_ = atoi(buf);

		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_SOCK_POLL_TIME), WM_GETTEXT, (WPARAM)sizeof(buf), (LPARAM)buf);
		__application_environment.recv_thread_waiting_time_ = atoi(buf);

		SendMessage(GET_WINDOW_HANDLE(IDC__IGNORE_LOG_PORT), WM_GETTEXT, (WPARAM)sizeof(buf), (LPARAM)buf);
		__application_environment.ignored_port_ = atoi(buf);

		result = SendMessage(GET_WINDOW_HANDLE(IDC_CRC_CHECK), BM_GETCHECK, NULL, NULL);
		if(result == BST_CHECKED)	__application_environment.enable_to_check_crc_ = TRUE;
		else						__application_environment.enable_to_check_crc_ = FALSE;

		result = SendMessage(GET_WINDOW_HANDLE(IDC_TRANSFER_DATA_CHECK), BM_GETCHECK, NULL, NULL);
		if(result == BST_CHECKED)	__application_environment.enable_to_transfer_data_ = TRUE;
		else						__application_environment.enable_to_transfer_data_ = FALSE;

		result = SendMessage(GET_WINDOW_HANDLE(IDC_ALL_LOG_OUTPUT_CHECK), BM_GETCHECK, NULL, NULL);
		if(result == BST_CHECKED)	__application_environment.be_all_info_outputted_ = TRUE;
		else						__application_environment.be_all_info_outputted_ = FALSE;

	}
	else{

		sprintf_s(buf, sizeof(buf), "%d", __application_environment.listen_port_);
		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_LISTEN_PORT), WM_SETTEXT, NULL, (LPARAM)buf);

		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_REMOTE_IP), WM_SETTEXT, NULL, (LPARAM)__application_environment.remote_ip_);

		sprintf_s(buf, sizeof(buf), "%d", __application_environment.remote_port_);
		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_REMOTE_PORT), WM_SETTEXT, NULL, (LPARAM)buf);

		sprintf_s(buf, sizeof(buf), "%d", __application_environment.log_port_);
		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_LOG_PORT), WM_SETTEXT, NULL, (LPARAM)buf);

		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_LOG_IP), WM_SETTEXT, NULL, (LPARAM)__application_environment.log_ip_);

		sprintf_s(buf, sizeof(buf), "%d", __application_environment.parallel_port_data_input_buffer_num_);
		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_PCI_BUFFER_NUM), WM_SETTEXT, NULL, (LPARAM)buf);

		sprintf_s(buf, sizeof(buf), "%d", __application_environment.parallel_port_data_input_buffer_size_);
		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_PCI_BUFFER_SIZE), WM_SETTEXT, NULL, (LPARAM)buf);

		sprintf_s(buf, sizeof(buf), "%d", __application_environment.parallel_port_input_waiting_time_);
		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_PCI_POLL_TIME), WM_SETTEXT, NULL, (LPARAM)buf);

		sprintf_s(buf, sizeof(buf), "%d", __application_environment.parallel_port_output_buffer_size_);
		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_PCI_WRITE_BUFFER_SIZE), WM_SETTEXT, NULL, (LPARAM)buf);

		sprintf_s(buf, sizeof(buf), "%d", __application_environment.recv_buff_size_);
		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_SOCK_RECV_NUM), WM_SETTEXT, NULL, (LPARAM)buf);

		sprintf_s(buf, sizeof(buf), "%d", __application_environment.send_buff_size_);
		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_SOCK_SEND_NUM), WM_SETTEXT, NULL, (LPARAM)buf);

		sprintf_s(buf, sizeof(buf), "%d", __application_environment.recv_thread_waiting_time_);
		SendMessage(GET_WINDOW_HANDLE(IDC_EDIT_SOCK_POLL_TIME), WM_SETTEXT, NULL, (LPARAM)buf);
		
		sprintf_s(buf, sizeof(buf), "%d", __application_environment.ignored_port_);
		SendMessage(GET_WINDOW_HANDLE(IDC__IGNORE_LOG_PORT), WM_SETTEXT, NULL, (LPARAM)buf);

		if(__application_environment.enable_to_check_crc_ == TRUE)	result = BST_CHECKED;
		else														result = BST_UNCHECKED;

		SendMessage(GET_WINDOW_HANDLE(IDC_CRC_CHECK), BM_SETCHECK, (WPARAM)result, NULL);

		if(__application_environment.enable_to_transfer_data_ == TRUE)	result = BST_CHECKED;
		else															result = BST_UNCHECKED;

		SendMessage(GET_WINDOW_HANDLE(IDC_TRANSFER_DATA_CHECK), BM_SETCHECK, (WPARAM)result, NULL);

		if(__application_environment.be_all_info_outputted_ == TRUE)	result = BST_CHECKED;
		else															result = BST_UNCHECKED;

		SendMessage(GET_WINDOW_HANDLE(IDC_ALL_LOG_OUTPUT_CHECK), BM_SETCHECK, (WPARAM)result, NULL);
	}

	return TRUE;
}

bool get_uint_value(UINT id, unsigned* result)
{	
	bool is_ok = true;
	int len = 0;
	char buf[MAX_STRING_SIZE] = {0};
	char* stop_buf_ptr = NULL;

	len = LoadString(__application_environment.instance_, id, buf, sizeof(buf));
	if(len == 0){
		*result = 0x0;
		return is_ok = false;
	}
	else {
		buf[len] = 0x0;
		*result = strtoul(buf, &stop_buf_ptr, 10);
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
	char buf[MAX_STRING_SIZE] = {0};

	get_string(text_id, buf, sizeof(buf));

	if(window_handle == NULL) window_handle = __application_environment.main_window_;

	return MessageBox(window_handle, buf, "", type);
}

