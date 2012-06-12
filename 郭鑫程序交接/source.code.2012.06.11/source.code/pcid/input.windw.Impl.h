#ifndef __INPUT_WINDOW_IMPL_H__
#define __INPUT_WINDOW_IMPL_H__

BOOL init_input_dialog(HWND, HWND, LPARAM);
BOOL command_input_message(HWND, int, HWND, UINT);
BOOL close_input_dialog(HWND);
INT_PTR CALLBACK dialog_input_proc(HWND, UINT, WPARAM, LPARAM);

#endif //__INPUT_WINDOW_IMPL_H__

