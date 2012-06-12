// pcid.cpp : Defines the entry point for the application.
//

#include "../common/config.h"
#include "../common/log.h"
#include "pcid.h"
#include "input.windw.Impl.h"

// Global Variables:
static char* __input_text = NULL;

BOOL init_input_dialog(HWND dialog_handle, HWND default_window_handle, LPARAM param)
{
	UNREFERENCED_PARAMETER(default_window_handle);
	UNREFERENCED_PARAMETER(dialog_handle);

	int len = 0;
	if(param){

		__input_text = (char*)param;
		len = strlen(__input_text);
		if(len > 0)
			SetWindowText(GET_WINDOW_HANDLE2(dialog_handle, IDC_INPUT_EDIT), __input_text);	
	}

	return TRUE;
}

BOOL command_input_message(HWND dialog_handle, int resource_id, HWND window_control_handle, UINT notification_code)
{
	UNREFERENCED_PARAMETER(notification_code);
	UNREFERENCED_PARAMETER(window_control_handle);
	BOOL is_ok = TRUE;
	switch(resource_id){
		case IDOK:
			Edit_GetText(GET_WINDOW_HANDLE2(dialog_handle, IDC_INPUT_EDIT), __input_text, MAX_STRING_SIZE);
			EndDialog(dialog_handle, TRUE);
			break;
		case IDCANCEL:
			EndDialog(dialog_handle, FALSE);
			break;
	}
	
	return is_ok;
}

BOOL close_input_dialog(HWND dialog_handle)
{
	return EndDialog(dialog_handle, FALSE);
}

INT_PTR CALLBACK dialog_input_proc(HWND dialog_handle, UINT message, WPARAM wParam,	LPARAM lParam)
{
	switch (message){

		HANDLE_DIALOG_MESSAGE(dialog_handle, WM_INITDIALOG, init_input_dialog);
		HANDLE_DIALOG_MESSAGE(dialog_handle, WM_COMMAND, command_input_message);	
		HANDLE_DIALOG_MESSAGE(dialog_handle, WM_CLOSE, close_input_dialog);
	}

	return FALSE;
}
