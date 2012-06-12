// pcid.cpp : Defines the entry point for the application.
//

#include "../common/config.h"
#include "../common/log.h"
#include <commctrl.h>
#include "pcid.h"
#include "sn.ip.list.window.impl.h"

extern APPLICATION_ENVIRONMENT __application_environment;

BOOL init_sn_ip_list_dialog(HWND dialog_handle, HWND default_window_handle, LPARAM param)
{
	UNREFERENCED_PARAMETER(default_window_handle);
	UNREFERENCED_PARAMETER(dialog_handle);

	HWND list_view = NULL;
	int i = 0;
	LVCOLUMN lvc; 
	LVITEM lvi;
	char szText[MAX_PATH] = {0};

	list_view = GET_WINDOW_HANDLE2(dialog_handle, IDC_SN_IP_LIST);
	LoadString((HINSTANCE)GetModuleHandle(NULL), IDS_SN_STRING, szText, sizeof(szText)/sizeof(szText[0]));

	memset(&lvc, 0x0, sizeof(lvc));
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.iSubItem = 0;
	lvc.pszText = szText;
	lvc.cx = 200;
	lvc.fmt = LVCFMT_LEFT;
	ListView_InsertColumn(list_view, 0, &lvc);

	LoadString((HINSTANCE)GetModuleHandle(NULL), IDS_IP_ADDRESS, szText, sizeof(szText)/sizeof(szText[0]));
	memset(&lvc, 0x0, sizeof(lvc));
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.iSubItem = 1;
	lvc.pszText = szText;
	lvc.cx = 200;
	lvc.fmt = LVCFMT_LEFT;
	ListView_InsertColumn(list_view, 1, &lvc);

	memset(&lvi, 0x0, sizeof(lvi));
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
	lvi.state = 0;
	lvi.stateMask = 0;

	for(APPLICATION_ENVIRONMENT::IP_TABLE_MAP_ITER itr = __application_environment.virtual_ip_table_map_->begin();
		itr != __application_environment.virtual_ip_table_map_->end(); ++itr, ++i){

		lvi.iItem = i;
		lvi.iSubItem = 0;

		sprintf(szText, "%X", itr->first);

		lvi.pszText = szText;
		ListView_InsertItem(list_view, &lvi);

		lvi.iSubItem = 1;
		sprintf(szText, "%s", inet_ntoa(*(PIN_ADDR)&(itr->second)));
		lvi.pszText = szText;
		ListView_InsertItem(list_view, &lvi);
	}


	return TRUE;
}

BOOL command_sn_ip_list_message(HWND dialog_handle, int resource_id, HWND window_control_handle, UINT notification_code)
{
	UNREFERENCED_PARAMETER(notification_code);
	UNREFERENCED_PARAMETER(window_control_handle);
	BOOL is_ok = TRUE;
	switch(resource_id){
		case IDOK:
			EndDialog(dialog_handle, TRUE);
			break;
	}
	
	return is_ok;
}

BOOL close_sn_ip_list_dialog(HWND dialog_handle)
{
	return EndDialog(dialog_handle, FALSE);
}

INT_PTR CALLBACK dialog_sn_ip_list_proc(HWND dialog_handle, UINT message, WPARAM wParam,	LPARAM lParam)
{
	switch (message){

		HANDLE_DIALOG_MESSAGE(dialog_handle, WM_INITDIALOG, init_sn_ip_list_dialog);
		HANDLE_DIALOG_MESSAGE(dialog_handle, WM_COMMAND, command_sn_ip_list_message);
		HANDLE_DIALOG_MESSAGE(dialog_handle, WM_CLOSE, close_sn_ip_list_dialog);
	}

	return FALSE;
}
