// STATUS_DIALOG.cpp : implementation file
//

#include "stdafx.h"
#include "line.h"
#include "STATUS_DIALOG.h"


// CSTATUS_DIALOG dialog

IMPLEMENT_DYNAMIC(CSTATUS_DIALOG, CDialog)

CSTATUS_DIALOG::CSTATUS_DIALOG(CWnd* pParent /*=NULL*/)
	: CDialog(CSTATUS_DIALOG::IDD, pParent), time_id_(NULL)
{

}

CSTATUS_DIALOG::~CSTATUS_DIALOG()
{
	DestroyWindow();
}

void CSTATUS_DIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MESSAGE, status_message_);
}


BEGIN_MESSAGE_MAP(CSTATUS_DIALOG, CDialog)
	ON_MESSAGE(WM_HIDE_DIALOG, OnHideDialog)	 
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSTATUS_DIALOG message handlers
BOOL CSTATUS_DIALOG::OnInitDialog()
{
	CDialog::OnInitDialog();

	status_message_.SetWindowText("");	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

bool CSTATUS_DIALOG::create_dialog()
{
	//Load DLGTEMPLATE
	DLGTEMPLATE* pTemplate;

	HINSTANCE hInst= AfxFindResourceHandle(
		MAKEINTRESOURCE(IDD_DIALOG_STATUS),RT_DIALOG);

	if (hInst == NULL)
	{ 
		TRACE("Cound not find resource in resource chain");
		ASSERT(FALSE);
		return true;
	}

	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(IDD_DIALOG_STATUS),
		RT_DIALOG);
	ASSERT(hRsrc != NULL);

	HGLOBAL hTemplate = ::LoadResource(hInst, hRsrc);
	ASSERT(hTemplate != NULL);

	pTemplate = (DLGTEMPLATE*)::LockResource(hTemplate);

	//Load coresponding DLGINIT resource
	void* lpDlgInit = NULL;
	HGLOBAL hDlgInit = NULL;

	HRSRC hsDlgInit = ::FindResource(hInst, MAKEINTRESOURCE(IDD_DIALOG_STATUS),
		RT_DLGINIT);
	if (hsDlgInit != NULL)
	{
		// load it
		hDlgInit = ::LoadResource(hInst, hsDlgInit);
		ASSERT(hDlgInit != NULL);

		// lock it
		lpDlgInit = ::LockResource(hDlgInit);
		ASSERT(lpDlgInit != NULL);
	}

	//ToDo: Modify DLGTEMPLATE in memory if desired

	CreateIndirect(hTemplate, NULL, hInst);

	::UnlockResource(hTemplate);
	::FreeResource(hTemplate);
	if (hDlgInit) 
	{
		::UnlockResource(hDlgInit);   
		::FreeResource(hDlgInit);
	}	
	
	return true;
}

void CSTATUS_DIALOG::show_message(LPTSTR msg)
{
	status_message_.SetWindowText(msg);
}

void CSTATUS_DIALOG::show_message(UINT msg_id)
{
	CString msg;
	msg.LoadString(msg_id);
	show_message(msg.LockBuffer());
	msg.UnlockBuffer();
}

LRESULT CSTATUS_DIALOG::OnHideDialog( WPARAM wParam, LPARAM lParam )
{
	unsigned int count = static_cast<unsigned int>(wParam);
	if(count > 0)
		time_id_ = SetTimer(HIDE_WINDOW_TIMER, count, NULL);
	return 0L;
}

void CSTATUS_DIALOG::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent){
	case HIDE_WINDOW_TIMER:
		ShowWindow(SW_HIDE);
		KillTimer(time_id_);
		break;
	}

	CDialog::OnTimer(nIDEvent);
}
