#pragma once

#include "resource.h"
#include "..\common\typedef.h"
#include "..\common\log.h"

#define GET_WINDOW_HANDLE(crtl_id)	GetDlgItem(__application_environment.main_window_, crtl_id)

typedef struct __tagAPPLICATION_ENVIRONMENT 
{
	HINSTANCE instance_;// current instance
	HWND main_window_;
	HWND option_window_;

	bool is_running_;
	//sock
	unsigned long ip_;

	log* log_;

}APPLICATION_ENVIRONMENT, *APPLICATION_ENVIRONMENT_PTR;

int get_string(UINT, LPTSTR, unsigned);
BOOL WINAPI main_Wnd_proc(HWND, UINT, WPARAM, LPARAM);
BOOL init_dialog(HWND, HWND, LPARAM);
BOOL destroy_dialog(HWND);
BOOL close_dialog(HWND);
BOOL command_message(HWND, int, HWND, UINT);
BOOL do_control(HWND, int, HWND window_control_handle, UINT);
BOOL set_window_control(bool);
bool is_running(APPLICATION_ENVIRONMENT_PTR);
bool is_stopping(APPLICATION_ENVIRONMENT_PTR);
BOOL start_button_down();

bool start_matrix_proc(APPLICATION_ENVIRONMENT_PTR);
bool stop_matrix_proc();
void init_matrix_proc();
void release_matrix_proc();

void output_log(unsigned int id, ...);

VOID CALLBACK the_heartbeat_timer_proc(HWND, UINT, UINT_PTR, DWORD);
VOID CALLBACK check_the_first_package_timer(HWND, UINT, UINT_PTR, DWORD);
VOID CALLBACK check_ip_setting_resp_timer_proc(HWND, UINT, UINT_PTR, DWORD);
VOID CALLBACK proc_time_delay_setting_resp(HWND, UINT, UINT_PTR, DWORD);


