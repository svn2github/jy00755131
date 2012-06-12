#include "../../common/config.h"
#include "segd.viewer.h"
#include "segd.treeview.h"

#pragma comment(lib, "comctl32")

//global variable
extern Application __theApp;
HWND	__treeview_handle = NULL;
HINSTANCE	__instance = NULL;
HWND	__parent_window_handle = NULL;
HTREEITEM	__root_item = NULL;
HTREEITEM	__the_last_item_inserted = TVI_FIRST;
HTREEITEM	__parent_item = TVI_FIRST;

bool treeview_create(HINSTANCE instance, RECT* rect_ptr, HWND parent_window_handle, LPCTSTR title)
{
	bool is_ok = false;

	int x = 0, 
		y = 0, 
		wd = (rect_ptr->right - rect_ptr->left)/4, 
		hi = rect_ptr->bottom - rect_ptr->top;

	__treeview_handle = CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, WC_TREEVIEW, title, 
										WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT,
										x, y, wd, hi, parent_window_handle, (HMENU)IDC_SEGDVIEWER, instance, NULL);

	if(IsWindow(__treeview_handle)) is_ok = true;

	return is_ok;
}

bool treeview_add_root(LPTSTR root_name, void* param)
{
	TVINSERTSTRUCT insert_struct;

	SecureZeroMemory(&insert_struct, sizeof(insert_struct));

	insert_struct.hParent = TVI_ROOT;
	insert_struct.hInsertAfter = NULL;
	
	insert_struct.itemex.mask = TVIF_PARAM | TVIF_TEXT;
	insert_struct.itemex.pszText = root_name;
	insert_struct.itemex.cchTextMax = _tcslen(root_name) + 1;
	insert_struct.itemex.lParam = (LPARAM)param;
	__root_item = reinterpret_cast<HTREEITEM>(SendMessage(__treeview_handle, 
															TVM_INSERTITEM, 0, (LPARAM)&insert_struct));
	if(__root_item == NULL)	return false;
	__parent_item = __root_item;
	return true;
}

HTREEITEM treeview_get_root_item(){ return __root_item;}
HTREEITEM treeview_get_the_last_item_inserted(){ return __the_last_item_inserted;}
HTREEITEM treeview_get_first_item_inserted(){ return TVI_FIRST;}
HTREEITEM treeview_get_last_item_inserted(){ return TVI_LAST;}
HTREEITEM treeview_get_sort_item_inserted(){ return TVI_SORT;}
HTREEITEM treeview_get_parent_item(){ return __parent_item;}

bool treeview_add_item(LPTSTR item_name, HTREEITEM parent_tree_item, HTREEITEM tree_item_inserted_after, void* param)
{
	TVINSERTSTRUCT insert_struct;

	SecureZeroMemory(&insert_struct, sizeof(insert_struct));

	insert_struct.hParent = parent_tree_item == NULL ? treeview_get_parent_item() : parent_tree_item;

	insert_struct.hInsertAfter = tree_item_inserted_after == NULL ? 
									treeview_get_the_last_item_inserted() : tree_item_inserted_after;

	insert_struct.itemex.mask = TVIF_PARAM | TVIF_TEXT;
	insert_struct.itemex.pszText = item_name;
	insert_struct.itemex.cchTextMax = _tcslen(item_name) + 1;
	insert_struct.itemex.lParam = (LPARAM)param;

	__the_last_item_inserted = reinterpret_cast<HTREEITEM>(SendMessage(__treeview_handle, TVM_INSERTITEM, 0, (LPARAM)&insert_struct));
	if(__the_last_item_inserted == NULL)	return false;
	if(parent_tree_item != __parent_item) __parent_item = parent_tree_item;

	return true;
}

void treeview_clear()
{
	SendMessage(__treeview_handle, TVM_DELETEITEM, NULL, NULL);
}

