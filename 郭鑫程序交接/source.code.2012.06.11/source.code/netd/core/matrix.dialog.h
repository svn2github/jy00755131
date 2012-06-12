/**
@brief	定义matrix_dialog类
@details	该类主要实现对话框机制
@file	matrix.dialog.h
@note	
@see	
@version	1.0.0.1.2012.02.10
@author	kuoxin	
@bug
*/
#ifndef __MATRIX_DIALOG_H__
#define __MATRIX_DIALOG_H__

class matrix_dialog: public matrix_object
{
public:
	matrix_dialog(unsigned int dialog_id, matrix_dialog* parent_object_ptr = NULL);
	virtual ~matrix_dialog();
public:
	void show();
protected:
	INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

};

#endif /*__MATRIX_DIALOG_H__*/