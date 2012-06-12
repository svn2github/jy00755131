#ifndef __OPTION_WINDOW_IMPL_H__
#define __OPTION_WINDOW_IMPL_H__

BOOL init_options_dialog(HWND, HWND, LPARAM);
BOOL command_options_message(HWND, int, HWND, UINT);
BOOL destroy_options_dialog(HWND);
BOOL close_options_dialog(HWND);
INT_PTR CALLBACK dialog_options_proc(HWND, UINT, WPARAM, LPARAM);
void save_optons_data(APPLICATION_ENVIRONMENT_PTR env_ptr, HWND);

#endif //__OPTION_WINDOW_IMPL_H__

