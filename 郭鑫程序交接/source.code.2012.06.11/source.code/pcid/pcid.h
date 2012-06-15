#ifndef __PCI_DIALOG_H__
#define __PCI_DIALOG_H__

#pragma once

#include "resource.h"
#include "../common/log.h"

#define GET_WINDOW_HANDLE(crtl_id)	GetDlgItem(__application_environment.main_window_, crtl_id)
#define GET_WINDOW_HANDLE2(window_handle, crtl_id)	GetDlgItem(window_handle, crtl_id)

enum __enuPCI_WORK_STATUS
{
	PCI_WORK_STARTED = 0,
	PCI_WORK_STOPPED = 1,
};


typedef struct __tagAPPLICATION_ENVIRONMENT 
{
	HINSTANCE instance_;// current instance
	HWND main_window_;
	HWND option_window_;

	LONG volatile be_started_;

	//sock
	unsigned listen_port_;
	char remote_ip_[MAX_STRING_SIZE];
	unsigned remote_port_;
	unsigned recv_buff_size_;
	unsigned send_buff_size_;
	unsigned recv_thread_waiting_time_;

	//pci 
	unsigned parallel_port_input_waiting_time_;
	unsigned parallel_port_data_input_buffer_size_;
	unsigned parallel_port_data_input_buffer_num_;
	unsigned parallel_port_output_buffer_size_;

	//log socket
	unsigned log_port_;
	char log_ip_[MAX_STRING_SIZE];

	//UDP & PCI counter
	LONG volatile upd_send_num_;
	LONG volatile upd_recv_num_;
	LONG volatile pci_input_num_;
	LONG volatile pci_output_num_;

	//filter & crc
	unsigned long* ip_filter_;
	unsigned long* port_filter_;
	unsigned int enable_to_check_crc_;
	unsigned int enable_to_transfer_data_;

	//全部输出日志
	unsigned int ignored_port_; //在输出全部接受发送信息时,忽略的端口
	unsigned int be_all_info_outputted_;//是否输出全部接受发送信息

	//log
	segd::log* log_;

	LONG volatile serial_no_;//support cross-station debug 
	LONG volatile udp_send_serial_no_; //support cross-station debug 

#if defined(__DEBUG_PCI_WRITE_PORT_PERFORMANCE__)
	segd::log* pci_writing_performance_log_;
#endif //defined(__DEBUG_PCI_WRITE_PORT_PERFORMANCE__)

}APPLICATION_ENVIRONMENT, *APPLICATION_ENVIRONMENT_PTR;

/*	output debug	*/
inline void OUTPUT_DEBUG_STRING(const char* info);
void OUTPUT_DEBUG_STRING(unsigned int id, ...);

/*	app status	*/
inline bool is_running(APPLICATION_ENVIRONMENT_PTR env_ptr);
inline bool is_stopping(APPLICATION_ENVIRONMENT_PTR env_ptr);
void exchange_status(APPLICATION_ENVIRONMENT_PTR env_ptr, bool is_running = true);

/*	UI	*/
BOOL WINAPI main_Wnd_proc(HWND, UINT, WPARAM, LPARAM);
BOOL init_dialog(HWND, HWND, LPARAM);
BOOL close_dialog(HWND);
BOOL destroy_dialog(HWND);

BOOL command_message(HWND, int, HWND, UINT);

BOOL do_menu(HWND, WORD);
BOOL do_Accelerator(HWND, WORD);
BOOL do_control(HWND, int, HWND, UINT);

BOOL start_end_button_down();
BOOL key_button_down();

BOOL assign_ip_button_down();

BOOL set_window_control(bool);
BOOL on_timer(HWND, UINT);

int show_message_box(HWND, UINT,UINT);

/*	load data from resource table*/
int get_string(UINT, LPTSTR, unsigned);
bool get_uint_value(UINT, unsigned*);
bool get_ushort_value(UINT, unsigned short*);

/*	worker	*/
BOOL start_simple_proc(APPLICATION_ENVIRONMENT_PTR app_env);
BOOL stop_simple_proc();

#endif //__PCI_DIALOG_H__

