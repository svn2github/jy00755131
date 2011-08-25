// TestFileOpenSpeedDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "测试文件打开读取速度.h"
#include "TestFileOpenSpeedDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CTestFileOpenSpeedDlg 对话框




CTestFileOpenSpeedDlg::CTestFileOpenSpeedDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestFileOpenSpeedDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pbufRead = NULL;
	m_uiSamplingRate = 2000;
	m_uiFindLineCount = 0;
	m_uiBufSize = 0;
	m_ullPointer = 0;
}

void CTestFileOpenSpeedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestFileOpenSpeedDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CTestFileOpenSpeedDlg::OnBnClickedButtonTest)
END_MESSAGE_MAP()


// CTestFileOpenSpeedDlg 消息处理程序

BOOL CTestFileOpenSpeedDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestFileOpenSpeedDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestFileOpenSpeedDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTestFileOpenSpeedDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestFileOpenSpeedDlg::OnBnClickedButtonTest()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str = _T("");
	CString strOut = _T("");
	CString csOpenFilePath = _T("");
	SYSTEMTIME  sysTime;
	unsigned int uiInstrument = 40;
	unsigned int uiADCDataSize = 10;
	m_uiBufSize = uiInstrument * uiADCDataSize + 2;
	if (m_pbufRead != NULL)
	{
		delete[] m_pbufRead;
	}
	if (m_uiBufSize == 0)
	{
		return;
	}
	m_pbufRead = new wchar_t[m_uiBufSize];
	m_uiFindLineCount = 0;
	m_ullPointer = 0;
	// 打开文件
	const wchar_t pszFilter[] = _T("文本文件(*.text)|*.text|文本文件(*.txt)|*.txt|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T(".text"), _T("1.text"), OFN_HIDEREADONLY| OFN_OVERWRITEPROMPT, pszFilter, this);

	if ( dlg.DoModal()!=IDOK )
	{
		return;
	}
	csOpenFilePath = dlg.GetPathName();
	GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(FALSE);
	GetLocalTime(&sysTime);
	str.Format(_T("%04d年%02d月%02d日%02d:%02d:%02d:%03d开始测试\r\n"), sysTime.wYear,sysTime.wMonth,sysTime.wDay,
		sysTime.wHour,sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds);
	strOut += str;
	for (int i=0; i<1000; i++)
	{
		OpenFile(csOpenFilePath);
	}
	GetLocalTime(&sysTime);
	str.Format(_T("%04d年%02d月%02d日%02d:%02d:%02d:%03d停止测试\r\n"), sysTime.wYear,sysTime.wMonth,sysTime.wDay,
		sysTime.wHour,sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds);
	strOut += str;
	GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_MESSAGE)->SetWindowText(strOut);
}

// 打开文件读取数据
void CTestFileOpenSpeedDlg::OpenFile(CString csOpenFilePath)
{
	if ((_taccess(csOpenFilePath,0)) != -1)
	{
		if(file.Open(csOpenFilePath, CFile::modeRead) == FALSE)
		{
			AfxMessageBox(_T("打开数据采样文件出错！"));	
			return;
		}
		else
		{
			CString str = _T("");
			CArchive ar(&file, CArchive::load);
			LONGLONG lOffset = 0;
			ULONGLONG lActual;
			unsigned int uiLength = 0;
			unsigned int uiRead = 0;
			file.SeekToBegin();
			ULONGLONG ullEnd = file.SeekToEnd();

			// 光标移动为字符长度+2(\n长度)
			file.SeekToBegin();
			// ADC数据采样开始时间
			ar.ReadString(str);
			uiLength += (str.GetLength() + 2) * 2;
			lActual = file.Seek(uiLength, CFile::begin);
			// ADC数据采样信息
			ar.ReadString(str);
			uiLength  += (str.GetLength() + 2) * 2;
			lActual = file.Seek(uiLength, CFile::begin);
			// 采集站设备标签
			ar.ReadString(str);
			uiLength  += (str.GetLength() + 2) * 2;
			lActual = file.Seek(uiLength, CFile::begin);
			// 跳过
			lActual = file.Seek(m_uiBufSize * 2* m_ullPointer, CFile::current);
			do 
			{
				if (m_uiFindLineCount == 1915210)
				{
					break;
				}
				else
				{
					// 读1行数据
					uiRead = file.Read(m_pbufRead, m_uiBufSize * sizeof(wchar_t));
					if (uiRead == 0)
					{
						break;
					}
					else
					{
						m_uiFindLineCount++;
						// 光标移动为字符长度+4(\r\n长度)
						// 跳过
						lActual = file.Seek(m_uiBufSize * 2* (m_uiSamplingRate - 1), CFile::current);
					}
				}
			} while (lActual <ullEnd);
			m_ullPointer = (lActual - ullEnd) / (m_uiBufSize * 2); 
	
			file.SeekToEnd();
			ar.Close();
			file.Close();
		}
	}
}
