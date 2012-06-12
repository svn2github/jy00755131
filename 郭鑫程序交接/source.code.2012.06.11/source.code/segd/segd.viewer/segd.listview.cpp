#include "../../common/config.h"
#include "segd.viewer.h"
#include "segd.listview.h"
#include <commctrl.h>

//global variable
extern Application __theApp;
HWND __listview_handle = NULL;

bool listview_create(HINSTANCE instance, RECT* rect_ptr, HWND parent_window_handle, LPCTSTR title)
{
	bool is_ok = false;

	int i = 0;
	TCHAR buf[MAX_STRING_SIZE / 8] = {0};
	LVCOLUMN lvc = {0};

	int x = (rect_ptr->right - rect_ptr->left)/4 + 1, 
		y = 0, 
		wd = 3 * (rect_ptr->right - rect_ptr->left)/4 - 1, 
		hi = 2 * (rect_ptr->bottom - rect_ptr->top) / 3;

	__listview_handle = CreateWindowEx(0, WC_LISTVIEW, title, 
		WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_EDITLABELS | WS_BORDER | LVS_SINGLESEL ,
		x, y, wd, hi, parent_window_handle, (HMENU)IDC_SEGDVIEWER, instance, NULL);

	if(IsWindow(__listview_handle)) is_ok = true;

	//add some item
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	for(i = IDS_COLUMONS_BEG; i <= IDS_COLUMONS_END; ++i){

		lvc.pszText = buf;
		lvc.cx = sizeof(buf);
		lvc.iSubItem = i - IDS_COLUMONS_BEG;
		lvc.fmt = LVCFMT_CENTER;
		LoadString(__theApp.instance_, i, buf, sizeof(buf));
		ListView_InsertColumn(__listview_handle, i - IDS_COLUMONS_BEG, &lvc);
	}

	return is_ok;
}

bool listview_add_item(int index, TCHAR** data)
{
	LVITEM listview_item;
	
	listview_item.mask = LVIF_TEXT;
	listview_item.iItem = index;
	listview_item.iSubItem = 0;
	listview_item.pszText = *data;
	listview_item.cchTextMax = _tcslen(*data) + 1;
	index = ListView_InsertItem(__listview_handle, &listview_item);
	ListView_SetItemText(__listview_handle, index, 1, *(data + 1));
	ListView_SetItemText(__listview_handle, index, 2, *(data + 2));
	ListView_SetItemText(__listview_handle, index, 3, *(data + 3));

	return index == -1 ? false : true;
}

void listview_clear()
{
	ListView_DeleteAllItems(__listview_handle);
}

