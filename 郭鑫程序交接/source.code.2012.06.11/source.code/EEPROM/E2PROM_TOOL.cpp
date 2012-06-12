// E2PROM_TOOL.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "E2PROM_TOOL.h"

#include "MainFrm.h"
#include "E2PROM_TOOLDoc.h"
#include "E2PROM_TOOLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


unsigned short str_to_hex2(char* buf, int buf_size)
{
	unsigned short value = 0,
		size = 0,
		tmp_val = 0;
	float base_value = 16;

	for(int i = 0; i < buf_size; ++i) {

		if(buf[i] == ' ') continue;

		if((buf[i] >= '0') && (buf[i] <= '9')) 
			tmp_val = (buf[i] - '0');
		else if((buf[i] >= 'A') && (buf[i] <= 'F')) 
			tmp_val = (buf[i] - 'A' + 10);
		else if((buf[i] >= 'a') && (buf[i] <= 'f')) 
			tmp_val = (buf[i] - 'a' + 10);

		switch(i){

			case 0:
				value += 16 * tmp_val;
				break;
			case 1:
				value += tmp_val;
				break;
			default:
				break;
		}
	}
	return value;
};

/////////////////////////////////////////////////////////////////////////////
// CE2PROM_TOOLApp

BEGIN_MESSAGE_MAP(CE2PROM_TOOLApp, CWinApp)
	//{{AFX_MSG_MAP(CE2PROM_TOOLApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	//ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CE2PROM_TOOLApp construction

CE2PROM_TOOLApp::CE2PROM_TOOLApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CE2PROM_TOOLApp object

CE2PROM_TOOLApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CE2PROM_TOOLApp initialization

BOOL CE2PROM_TOOLApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	//add console 
#ifdef _DEBUG
	AllocConsole();
#endif
	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CE2PROM_TOOLDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CE2PROM_TOOLView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CE2PROM_TOOLApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CE2PROM_TOOLApp message handlers


int CE2PROM_TOOLApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
#ifdef _DEBUG
	FreeConsole();
#endif

	return CWinApp::ExitInstance();
}
