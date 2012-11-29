// IC_TESTDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IC_TEST.h"
#include "IC_TESTDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CIC_TESTDlg dialog




CIC_TESTDlg::CIC_TESTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIC_TESTDlg::IDD, pParent)
	, m_pProData(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIC_TESTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MSG, m_ctrlListMsg);
	DDX_Control(pDX, IDC_EDIT_HEX, m_ctrlEditHex);
	DDX_Control(pDX, IDC_SPIN_SRC, m_ctrlSpinSRC);
	DDX_Control(pDX, IDC_SPIN_IRC4M, m_ctrlSpinIRC4M);
	DDX_Control(pDX, IDC_SPIN_IRC1K, m_ctrlSpinIRC1K);
}

BEGIN_MESSAGE_MAP(CIC_TESTDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_RESET, &CIC_TESTDlg::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_RESET_MSG, &CIC_TESTDlg::OnBnClickedBtnResetMsg)
	ON_BN_CLICKED(IDC_BTN_OPENFILE, &CIC_TESTDlg::OnBnClickedBtnOpenfile)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CIC_TESTDlg message handlers

BOOL CIC_TESTDlg::OnInitDialog()
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
	SetIcon(m_hIcon, TRUE);         // Set big icon
	SetIcon(m_hIcon, FALSE);        // Set small icon
	// TODO: Add extra initialization here
	m_ctrlSpinSRC.SetBuddy(GetDlgItem(IDC_EDIT_SRC));
	m_ctrlSpinSRC.SetRange(0, 255);
	m_ctrlSpinIRC1K.SetBuddy(GetDlgItem(IDC_EDIT_IRC1K));
	m_ctrlSpinIRC1K.SetRange(0, 255);
	m_ctrlSpinIRC4M.SetBuddy(GetDlgItem(IDC_EDIT_IRC4M));
	m_ctrlSpinIRC4M.SetRange(0, 255);
	RefreshControls();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIC_TESTDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CIC_TESTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIC_TESTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// 重置控件
void CIC_TESTDlg::RefreshControls(void)
{
	if (m_pProData != NULL)
	{
		delete[] m_pProData;
		m_pProData = NULL;
	}
	m_bySCR = 0x00;
	m_byIRC1K = 0x00;
	m_byIRC4M = 0x00;
	SetDlgItemInt(IDC_EDIT_SRC, m_bySCR);
	SetDlgItemInt(IDC_EDIT_IRC1K, m_byIRC1K);
	SetDlgItemInt(IDC_EDIT_IRC4M, m_byIRC4M);
	SetDlgItemText(IDC_COMBO_SRC, _T(""));
	SetDlgItemText(IDC_COMBO_IRC1K, _T(""));
	SetDlgItemText(IDC_COMBO_IRC4M, _T(""));
	SetDlgItemText(IDC_COMBO_PROGRAM_MODE, _T(""));
	m_uiAddrMax = 0;
	m_uiAddrMin = DefaultBufSize;
	m_ctrlEditHex.SetBPR(16);
	m_ctrlEditHex.SetOptions(TRUE, TRUE, FALSE, TRUE);
	m_ctrlEditHex.Clear();
	while(m_ctrlListMsg.GetCount() != 0)
	{
		m_ctrlListMsg.DeleteString(0);
	}
}


void CIC_TESTDlg::OnBnClickedBtnReset()
{
	// TODO: 在此添加控件通知处理程序代码
	RefreshControls();
}


void CIC_TESTDlg::OnBnClickedBtnResetMsg()
{
	// TODO: 在此添加控件通知处理程序代码
	while(m_ctrlListMsg.GetCount() != 0)
	{
		m_ctrlListMsg.DeleteString(0);
	}
}


void CIC_TESTDlg::OnBnClickedBtnOpenfile()
{
	// TODO: 在此添加控件通知处理程序代码
	//显示文件打开对话框
	//同时打开多个类型文件
// 	CFileDialog dlg(TRUE, _T("EC"), _T("*.hex"),
// 		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("Hex File(*.hex)|*.hex"));
	CFileDialog dlg(TRUE, _T("EC"), _T("*.hex"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("(*.hex)|*.hex|All Files(*.*)|*.*|"));//同时打开多个类型文件
	if ( dlg.DoModal()!=IDOK ) return;
	//获取文件的绝对路径
	CString sPathName = dlg.GetPathName();
	CString	sFileName = dlg.GetFileName();
	CString strOutput = _T("Open file path: ");
	strOutput += sPathName;
	m_ctrlListMsg.AddString(strOutput);
	if ((_taccess(sPathName,0)) != -1)
	{
		size_t sFileName_size = sFileName.GetLength();
		//后缀名
		CString suffix = sFileName.Mid((int)sFileName_size - 4, 4);
		if ((_T(".hex") == suffix) || (_T(".HEX") == suffix))
		{
			CString str = _T("");
			CString strTemp = _T("");
			unsigned int uiDataLength = 0;
			unsigned int uiLoadOffset = 0;
			unsigned int uiRecType = 0;
			unsigned int uiDataTemp = 0;
			LPBYTE lpchar=_T('');
			unsigned int uiDataNum = 0;
			vector <unsigned char> data_vector;
			for (int i=0; i<DefaultBufSize; i++)
			{
				data_vector.push_back(DefaultBufValue);
			}
			CStdioFile file(sPathName, CFile::modeRead); //只读
			// 每读完一行，指向下一行，如果读到文件末尾，退出循环
			while(file.ReadString(str))
			{
				// Mid(string,start,length)
				// 从string字符串的start字符开始取得length长度的字符串，
				// 如果省略第三个参数表示从start字符开始到字符串结尾的字符串。
				int iPos = 1;
				strTemp = str.Mid(iPos, 2);
				iPos += 2;
				strTemp = _T("0x") + strTemp;
				swscanf_s(strTemp, _T("%x"), &uiDataLength);
				strTemp = str.Mid(iPos, 4);
				iPos += 4;
				strTemp = _T("0x") + strTemp;
				swscanf_s(strTemp, _T("%x"), &uiLoadOffset);
				strTemp = str.Mid(iPos, 2);
				iPos += 2;
				strTemp = _T("0x") + strTemp;
				swscanf_s(strTemp, _T("%x"), &uiRecType);
				if (uiDataLength > 0)
				{
					if (uiLoadOffset < m_uiAddrMin)
					{
						m_uiAddrMin = uiLoadOffset;
					}
					if (uiLoadOffset > m_uiAddrMax)
					{
						m_uiAddrMax = uiLoadOffset + uiDataLength - 1;
					}
					for (unsigned int i=0; i<uiDataLength; i++)
					{
						strTemp = str.Mid(iPos, 2);
						iPos += 2;
						strTemp = _T("0x") + strTemp;
						swscanf_s(strTemp, _T("%x"), &uiDataTemp);
						data_vector[i + uiLoadOffset] = ((unsigned char)uiDataTemp);
					}
				}
			}
			if (m_pProData != NULL)
			{
				delete[] m_pProData;
				m_pProData = NULL;
				m_ctrlEditHex.Clear();
			}
			uiDataNum = m_uiAddrMax - m_uiAddrMin + 1;
			m_pProData = new BYTE[uiDataNum];
			for (unsigned int i=0; i<uiDataNum; i++)
			{
				m_pProData[i] = data_vector[i + m_uiAddrMin];
			}
			data_vector.clear();
			m_ctrlEditHex.SetAddressBase(m_uiAddrMin);
			m_ctrlEditHex.SetData(m_pProData, uiDataNum, uiDataNum);
			m_bySCR = m_pProData[SRCAddr - m_uiAddrMin];
			m_byIRC1K = m_pProData[IRC1KAddr - m_uiAddrMin];
			m_byIRC4M = m_pProData[IRC4MAddr - m_uiAddrMin];
			SetDlgItemInt(IDC_EDIT_SRC, m_bySCR);
			SetDlgItemInt(IDC_EDIT_IRC1K, m_byIRC1K);
			SetDlgItemInt(IDC_EDIT_IRC4M, m_byIRC4M);
		}
		else
		{
			AfxMessageBox(_T("The file's type is not ' *.hex ' !"));
		}
	}
	else
	{
		AfxMessageBox(_T("The file is not exit!"));
	}
}


void CIC_TESTDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (m_pProData != NULL)
	{
		delete[] m_pProData;
		m_pProData = NULL;
	}
}
