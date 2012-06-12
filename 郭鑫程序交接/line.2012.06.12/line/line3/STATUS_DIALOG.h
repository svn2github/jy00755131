#pragma once
#include "afxwin.h"
#include "resource.h"

// CSTATUS_DIALOG dialog
#define WM_HIDE_DIALOG WM_USER + 1000

class CSTATUS_DIALOG : public CDialog
{
	DECLARE_DYNAMIC(CSTATUS_DIALOG)
public:
	CSTATUS_DIALOG(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSTATUS_DIALOG();
// Dialog Data
	enum { IDD = IDD_DIALOG_STATUS };
	enum CONST_DATA{	HIDE_WINDOW_TIMER };
public:
	bool create_dialog();
	void show_message(LPTSTR msg);
	void show_message(UINT msg_id);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	LRESULT OnHideDialog( WPARAM wParam, LPARAM lParam ); 
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CStatic status_message_;
	UINT_PTR time_id_;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
