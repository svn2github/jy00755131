// TestOScopeDlg.cpp : implementation file
//

#include "stdafx.h"
#include <stdlib.h>
#include "TestOScope.h"
#include "TestOScopeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestOScopeDlg dialog

CTestOScopeDlg::CTestOScopeDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CTestOScopeDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CTestOScopeDlg)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
  // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  m_bStartStop = FALSE ;
  srand( (unsigned)time( NULL ) );
  
}

void CTestOScopeDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CTestOScopeDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestOScopeDlg, CDialog)
  //{{AFX_MSG_MAP(CTestOScopeDlg)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_WM_TIMER()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestOScopeDlg message handlers

BOOL CTestOScopeDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Add "About..." menu item to system menu.

  // IDM_ABOUTBOX must be in the system command range.
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT(IDM_ABOUTBOX < 0xF000);

  CMenu* pSysMenu = GetSystemMenu(FALSE);
  if (pSysMenu != NULL)
  {
    CString strAboutMenu;
    strAboutMenu.LoadString(IDS_ABOUTBOX);
    if (!strAboutMenu.IsEmpty())
    {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
    }
  }

  // Set the icon for this dialog.  The framework does this automatically
  //  when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);      // Set big icon
  SetIcon(m_hIcon, FALSE);    // Set small icon
  
  // TODO: Add extra initialization here
  // determine the rectangle for the control
  CRect rect;
  GetDlgItem(IDC_OSCOPE)->GetWindowRect(rect) ;
  ScreenToClient(rect) ;

  // create the control
  m_OScopeCtrl.Create(WS_VISIBLE | WS_CHILD, rect, this) ; 

  // customize the control
  m_OScopeCtrl.SetRange(-10.0, 10.0, 1) ;
  m_OScopeCtrl.SetYUnits("Volts") ;
  m_OScopeCtrl.SetXUnits("Samples (Windows Timer: 100 msec)") ;
  m_OScopeCtrl.SetBackgroundColor(RGB(0, 0, 64)) ;
  m_OScopeCtrl.SetGridColor(RGB(192, 192, 255)) ;
  m_OScopeCtrl.SetPlotColor(RGB(255, 255, 255)) ;

  return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestOScopeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
  if ((nID & 0xFFF0) == IDM_ABOUTBOX)
  {
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
  }
  else
  {
    CDialog::OnSysCommand(nID, lParam);
  }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestOScopeDlg::OnPaint() 
{
  if (IsIconic())
  {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  }
  else
  {
    CDialog::OnPaint();
  }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestOScopeDlg::OnQueryDragIcon()
{
  return (HCURSOR) m_hIcon;
}

void CTestOScopeDlg::OnRunstop() 
{
  // TODO: Add your control notification handler code here
  m_bStartStop ^= TRUE;

  if (m_bStartStop)
    SetTimer(1,100,NULL);
  else
    KillTimer(1);
  
}

void CTestOScopeDlg::OnTimer(UINT nIDEvent) 
{
  double nRandom;

  // generate a random number between -5 and 5
  nRandom = -5.0 + 10.0*rand()/(double)RAND_MAX;

  // append the new value to the plot
  m_OScopeCtrl.AppendPoint(nRandom);

  
  CDialog::OnTimer(nIDEvent);
}

void CTestOScopeDlg::OnOK() 
{
  m_bStartStop ^= TRUE;

  if (m_bStartStop)
    SetTimer(1,100,NULL);
  else
    KillTimer(1);

}

void CTestOScopeDlg::OnCancel() 
{
  if (!m_bStartStop)
    KillTimer(1) ;

  CDialog::OnCancel();
}
