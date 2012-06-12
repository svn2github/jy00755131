#ifndef __SEGD_TREE_VIEW_H__
#define __SEGD_TREE_VIEW_H__

#include <CommCtrl.h>


bool	treeview_create(HINSTANCE instance, RECT* rect_ptr, 
						HWND parent_window_handle = NULL, LPCTSTR title = NULL);

bool	treeview_add_root(LPTSTR root_name, void* param = NULL);

bool	treeview_add_item(LPTSTR item_name, HTREEITEM parent_tree_item = NULL, 
						   HTREEITEM tree_item_inserted_after = NULL, void* param = NULL);

HTREEITEM	treeview_get_root_item();
HTREEITEM	treeview_get_the_last_item_inserted();
HTREEITEM	treeview_get_parent_item();
HTREEITEM	treeview_get_last_item_inserted(); //TVI_LAST
HTREEITEM	treeview_get_first_item_inserted(); // TVI_FIRST
HTREEITEM	treeview_get_sort_item_inserted(); // TVI_SORT

void	treeview_clear();










#endif //__SEGD_TREE_VIEW_H__