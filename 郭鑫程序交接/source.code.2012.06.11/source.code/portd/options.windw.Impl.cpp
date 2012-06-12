// pcid.cpp : Defines the entry point for the application.
//

#include "../common/config.h"
#include "../common/log.h"
#include "pcid.h"
#include "input.windw.Impl.h"
#include "options.windw.Impl.h"


// Global Variables:
extern APPLICATION_ENVIRONMENT __application_environment;
char __input_text[MAX_STRING_SIZE] = {0};

BOOL init_options_dialog(HWND dialog_handle, HWND default_window_handle, LPARAM param)
{
	UNREFERENCED_PARAMETER(param);
	UNREFERENCED_PARAMETER(default_window_handle);
	UNREFERENCED_PARAMETER(dialog_handle);

	errno_t err = NULL;
	char value[MAX_STRING_SIZE] = {0};
	unsigned long* tmp_long = NULL;

	tmp_long = __application_environment.ip_filter_;

	while( tmp_long && *tmp_long){

		SendMessage(GET_WINDOW_HANDLE2(dialog_handle, IDC_IP_LIST), LB_ADDSTRING, 
					NULL, (LPARAM)inet_ntoa(*(IN_ADDR*)tmp_long));
		++tmp_long;
	}

	tmp_long = __application_environment.port_filter_;
	while(tmp_long && *tmp_long){

		err = _itoa_s(*tmp_long, value, sizeof(value), 10);
		if(err == 0)	
			SendMessage(GET_WINDOW_HANDLE2(dialog_handle, IDC_PORT_LIST), LB_ADDSTRING, NULL, (LPARAM)value);

		++tmp_long;
	}

	return TRUE;
}

void operate_ip(APPLICATION_ENVIRONMENT_PTR env_ptr, HWND dialog_handle, UINT id)
{
	unsigned long ip = 0x0;
	LONG index = 0;
	INT_PTR result = 0x0;
	memset(__input_text, 0x0, sizeof(__input_text));

	switch(id){
		case IDC_BUTTON_MODIFY_IP:

			index = ListBox_GetCurSel(GET_WINDOW_HANDLE2(dialog_handle, IDC_IP_LIST));
			if(index == LB_ERR){

				show_message_box(dialog_handle, IDS_STRING156, MB_ICONWARNING);
				break;
			}

			ListBox_GetText(GET_WINDOW_HANDLE2(dialog_handle, IDC_IP_LIST), index, __input_text);

			result = DialogBoxParam(env_ptr->instance_, 
									MAKEINTRESOURCE(IDD_INPUT_DIALOG),
									dialog_handle, 
									dialog_input_proc, 
									(LPARAM)__input_text);
			if(result == TRUE){

				ip = inet_addr(__input_text);
				if(ip != INADDR_NONE){

					SendMessage(GET_WINDOW_HANDLE2(dialog_handle, IDC_IP_LIST), LB_DELETESTRING, (WPARAM)index, NULL);
					SendMessage(GET_WINDOW_HANDLE2(dialog_handle, IDC_IP_LIST),	
								LB_INSERTSTRING, (WPARAM)index, (LPARAM)__input_text);
				}
				else{

					show_message_box(dialog_handle, IDS_STRING157, MB_ICONSTOP);
				}
			}
			break;
		case IDC_BUTTON_ADD_IP:
			result = DialogBoxParam(env_ptr->instance_, 
									MAKEINTRESOURCE(IDD_INPUT_DIALOG),
									dialog_handle, 
									dialog_input_proc, 
									(LPARAM)__input_text);
			if(result == TRUE){

				ip = inet_addr(__input_text);
				if(ip != INADDR_NONE){

					index = SendMessage(GET_WINDOW_HANDLE2(dialog_handle, IDC_IP_LIST),	LB_ADDSTRING, NULL, (LPARAM)__input_text);
					ListBox_SetCurSel(GET_WINDOW_HANDLE2(dialog_handle, IDC_IP_LIST), index);
				}
				else
					show_message_box(dialog_handle, IDS_STRING157, MB_ICONSTOP);
			}

			break;
		case IDC_BUTTON_DELETE_IP:
			index = ListBox_GetCurSel(GET_WINDOW_HANDLE2(dialog_handle, IDC_IP_LIST));
			if(index == LB_ERR)
				show_message_box(dialog_handle, IDS_STRING156, MB_ICONWARNING);
			else
				SendMessage(GET_WINDOW_HANDLE2(dialog_handle, IDC_IP_LIST), LB_DELETESTRING, (WPARAM)index, NULL);
			
			break;
	}
}

void operate_port(APPLICATION_ENVIRONMENT_PTR env_ptr, HWND dialog_handle, UINT id)
{
	unsigned short port = 0x0;
	LONG index = 0;
	INT_PTR result = 0x0;
	char* end_ptr = NULL;
	memset(__input_text, 0x0, sizeof(__input_text));

	switch(id){
		case IDC_BUTTON_MODIFY_PORT:

			index = ListBox_GetCurSel(GET_WINDOW_HANDLE2(dialog_handle, IDC_PORT_LIST));
			if(index == LB_ERR){

				show_message_box(dialog_handle, IDS_STRING156, MB_ICONWARNING);
				break;
			}

			ListBox_GetText(GET_WINDOW_HANDLE2(dialog_handle, IDC_PORT_LIST), index, __input_text);

			result = DialogBoxParam(env_ptr->instance_, 
									MAKEINTRESOURCE(IDD_INPUT_DIALOG),
									dialog_handle, 
									dialog_input_proc, 
									(LPARAM)__input_text);
			if(result == TRUE){

				port = static_cast<unsigned short>(strtoul(__input_text, &end_ptr, 10));
				if(port < USHRT_MAX && port > 0){

					_ultoa_s(port, __input_text, MAX_STRING_SIZE, 10);
					SendMessage(GET_WINDOW_HANDLE2(dialog_handle, IDC_PORT_LIST), LB_DELETESTRING, (WPARAM)index, NULL);
					SendMessage(GET_WINDOW_HANDLE2(dialog_handle, IDC_PORT_LIST),	
								LB_INSERTSTRING, (WPARAM)index, (LPARAM)__input_text);
				}
				else{

					show_message_box(dialog_handle, IDS_STRING158, MB_ICONSTOP);
				}
			}
			break;
		case IDC_BUTTON_ADD_PORT:
			result = DialogBoxParam(env_ptr->instance_, 
									MAKEINTRESOURCE(IDD_INPUT_DIALOG),
									dialog_handle, 
									dialog_input_proc, 
									(LPARAM)__input_text);
			if(result == TRUE){

				port = static_cast<unsigned short>(strtoul(__input_text, &end_ptr, 10));
				if(port < USHRT_MAX && port > 0){

					_ultoa_s(port, __input_text, MAX_STRING_SIZE, 10);
					index = SendMessage(GET_WINDOW_HANDLE2(dialog_handle, IDC_PORT_LIST), LB_ADDSTRING, NULL, (LPARAM)__input_text);
					ListBox_SetCurSel(GET_WINDOW_HANDLE2(dialog_handle, IDC_PORT_LIST), index);
				}
				else
					show_message_box(dialog_handle, IDS_STRING158, MB_ICONSTOP);
			}

			break;
		case IDC_BUTTON_DELETE_PORT:
			index = ListBox_GetCurSel(GET_WINDOW_HANDLE2(dialog_handle, IDC_PORT_LIST));
			if(index == LB_ERR)
				show_message_box(dialog_handle, IDS_STRING156, MB_ICONWARNING);
			else
				SendMessage(GET_WINDOW_HANDLE2(dialog_handle, IDC_PORT_LIST), LB_DELETESTRING, (WPARAM)index, NULL);

			break;
	}

}

BOOL command_options_message(HWND dialog_handle, int resource_id, HWND window_control_handle, UINT notification_code)
{
	UNREFERENCED_PARAMETER(notification_code);
	UNREFERENCED_PARAMETER(window_control_handle);

	BOOL is_ok = TRUE;
	switch(resource_id){
		case IDC_OPTIONS_BUTTON_OK:
			save_optons_data(&__application_environment, dialog_handle);
			EndDialog(dialog_handle, TRUE);
			break;
		case IDC_BUTTON_ADD_IP:
		case IDC_BUTTON_MODIFY_IP:
		case IDC_BUTTON_DELETE_IP:
			operate_ip(&__application_environment, dialog_handle, resource_id);
			break;
		case IDC_BUTTON_ADD_PORT:
		case IDC_BUTTON_MODIFY_PORT:
		case IDC_BUTTON_DELETE_PORT:
			operate_port(&__application_environment, dialog_handle, resource_id);	
			break;
		case IDC_OPTIONS_BUTTON_CANCEL:
			EndDialog(dialog_handle, FALSE);
			break;
	}
	
	return is_ok;
}

BOOL close_options_dialog(HWND dialog_handle)
{
	return EndDialog(dialog_handle, FALSE);
}

INT_PTR CALLBACK dialog_options_proc(HWND dialog_handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){

		HANDLE_DIALOG_MESSAGE(dialog_handle, WM_INITDIALOG, init_options_dialog);
		HANDLE_DIALOG_MESSAGE(dialog_handle, WM_COMMAND, command_options_message);	
		HANDLE_DIALOG_MESSAGE(dialog_handle, WM_CLOSE, close_options_dialog);
	}

	return FALSE;
}

void save_optons_data(APPLICATION_ENVIRONMENT_PTR env_ptr, HWND dialog_handle)
{
	LONG result = 0x0;
	LONG list_count = 0x0;
	int index = 0;
	char buf[MAX_STRING_SIZE] = {0};
	char* c_ptr = NULL;

	list_count = SendMessage(GET_WINDOW_HANDLE2(dialog_handle, IDC_IP_LIST), LB_GETCOUNT, NULL, NULL);
	if(list_count != LB_ERR){
		
		if(env_ptr->ip_filter_){

			free(env_ptr->ip_filter_);
			env_ptr->ip_filter_ = NULL;
		}

		if(list_count > 0){
			
			index = (list_count + 2) * sizeof(unsigned long);
			env_ptr->ip_filter_ = (unsigned long*)malloc(index);
			memset(env_ptr->ip_filter_, 0x0, index);
			
			for(index = 0; index < list_count; ++index){
				
				memset(buf, 0x0, sizeof(buf));
				result = SendMessage(GET_WINDOW_HANDLE2(dialog_handle, IDC_IP_LIST), 
									LB_GETTEXT, (WPARAM)index, (LPARAM) buf);  
				env_ptr->ip_filter_[index] = inet_addr(buf);
			}
		}
	}
	else{
		OUTPUT_DEBUG_STRING(IDS_STRING151);
	}

	list_count = SendMessage(GET_WINDOW_HANDLE2(dialog_handle, IDC_PORT_LIST), LB_GETCOUNT, NULL, NULL);
	if(list_count != LB_ERR){

		if(env_ptr->port_filter_){

			free(env_ptr->port_filter_);
			env_ptr->port_filter_ = NULL;
		}

		if(list_count > 0){

			index = (list_count + 2) * sizeof(unsigned long);
			env_ptr->port_filter_ = (unsigned long*)malloc(index);
			memset(env_ptr->port_filter_, 0x0, index);

			for(index = 0; index < list_count; ++index){

				memset(buf, 0x0, sizeof(buf));
				result = SendMessage(GET_WINDOW_HANDLE2(dialog_handle, IDC_PORT_LIST), 
									LB_GETTEXT, (WPARAM)index, (LPARAM) buf);  
				env_ptr->port_filter_[index] = strtoul(buf, &c_ptr, 10);
			}
		}
	}
	else{
		OUTPUT_DEBUG_STRING(IDS_STRING152);
	}
}