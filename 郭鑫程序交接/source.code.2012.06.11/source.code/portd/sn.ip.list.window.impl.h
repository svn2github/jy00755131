#ifndef __SN_IP_LIST_WINDOW_IMPL_H__
#define __SN_IP_LIST_WINDOW_IMPL_H__

BOOL init_sn_ip_list_dialog(HWND, HWND, LPARAM);
BOOL command_sn_ip_list_message(HWND, int, HWND, UINT);
BOOL close_sn_ip_list_dialog(HWND);
INT_PTR CALLBACK dialog_sn_ip_list_proc(HWND, UINT, WPARAM, LPARAM);

#endif //__SN_IP_LIST_WINDOW_IMPL_H__

