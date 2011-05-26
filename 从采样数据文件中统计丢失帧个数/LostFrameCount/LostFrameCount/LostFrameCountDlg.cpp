// LostFrameCountDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LostFrameCount.h"
#include "LostFrameCountDlg.h"
#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLostFrameCountDlg 对话框




CLostFrameCountDlg::CLostFrameCountDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLostFrameCountDlg::IDD, pParent)
	, m_uiFileStartNb(1)
	, m_uiFileEndNb(0)
	, m_uiSaveADCDataFrameNum(406)
	, m_uiInstrumentTotalNum(48)
	, m_uiInstrumentSampleNum(5)
	, m_uiFrameSize(74)
	, m_csFilePath(_T(""))
	, m_uipLostFrameCount(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CLostFrameCountDlg::~CLostFrameCountDlg()
{
	if (m_uipLostFrameCount != NULL)
	{
		m_uipLostFrameCount = NULL;
		delete m_uipLostFrameCount;
	}
}
void CLostFrameCountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILESTARTNB, m_uiFileStartNb);
	DDX_Text(pDX, IDC_EDIT_FILEENDNB, m_uiFileEndNb);
	DDX_Text(pDX, IDC_EDIT_SAVEADCDATANUM, m_uiSaveADCDataFrameNum);
	DDX_Text(pDX, IDC_EDIT_INSTRUMENTTOTALNUM, m_uiInstrumentTotalNum);
	DDX_Text(pDX, IDC_EDIT_INSTRUMENTSAMPLENUM, m_uiInstrumentSampleNum);
	DDX_Text(pDX, IDC_EDIT_FRAMESIZE, m_uiFrameSize);
}

BEGIN_MESSAGE_MAP(CLostFrameCountDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CALLOSTDATAFRAME, &CLostFrameCountDlg::OnBnClickedButtonCallostdataframe)
	ON_BN_CLICKED(IDC_BUTTON_FILEPATH, &CLostFrameCountDlg::OnBnClickedButtonFilepath)
END_MESSAGE_MAP()


// CLostFrameCountDlg 消息处理程序

BOOL CLostFrameCountDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLostFrameCountDlg::OnPaint()
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
HCURSOR CLostFrameCountDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CLostFrameCountDlg::OnBnClickedButtonCallostdataframe()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_csFilePath == _T(""))
	{
		AfxMessageBox(_T("请选择采样数据存储目录！"));
		return;
	}
	if (m_uiFileEndNb < m_uiFileStartNb)
	{
		AfxMessageBox(_T("请输入正确的文件结束序号！"));
		return;
	}
	if ((m_uiInstrumentTotalNum < m_uiInstrumentSampleNum) && (m_uiInstrumentSampleNum == 0))
	{
		AfxMessageBox(_T("请输入正确的采样设备个数！"));
		return;
	}
	// 清空结果显示
	GetDlgItem(IDC_EDIT_RESULTSHOW)->SetWindowText(_T(""));
	// 计算丢失帧个数
	OnCalLostDataFrameNum();
}

void CLostFrameCountDlg::OnBnClickedButtonFilepath()
{
	// TODO: 在此添加控件通知处理程序代码
	char szDir[MAX_PATH];

	BROWSEINFO bi;

	ITEMIDLIST *pidl;

	bi.hwndOwner = this->m_hWnd; // 指定父窗口，在对话框显示期间，父窗口将被禁用 


	bi.pidlRoot = NULL; // 如果指定NULL，就以“桌面”为根 

	bi.pszDisplayName = szDir; 

	bi.lpszTitle = _T("请选择采样数据存储目录"); // 这一行将显示在对话框的顶端 

	bi.ulFlags = BIF_STATUSTEXT | BIF_USENEWUI | BIF_RETURNONLYFSDIRS;

	bi.lpfn = NULL;

	bi.lParam = 0;

	bi.iImage = 0;

	pidl = SHBrowseForFolder(&bi);

	if(pidl == NULL) 
	{
		return;
	}
	if(!SHGetPathFromIDList(pidl, szDir))
	{	
		return;
	}
	else
	{
		m_csFilePath = szDir;
	}
}

// 计算丢失数据帧个数
void CLostFrameCountDlg::OnCalLostDataFrameNum(void)
{
	CString strOutput = _T("");
	CString strtmp = _T("");
	m_uipLostFrameCount = NULL;
	m_uipLostFrameCount = new unsigned int[m_uiInstrumentSampleNum];
	// 清空结果显示
	GetDlgItem(IDC_EDIT_RESULTSHOW)->SetWindowText(_T(""));
	for (unsigned int i=0; i<m_uiInstrumentSampleNum; i++)
	{
		m_uipLostFrameCount[i] = 0;
	}
	if (m_uiFileStartNb == m_uiFileEndNb)
	{
		// 处理最后一个保存的数据文件
		OnCalLastFile(m_uiFileEndNb);
	}
	else if (m_uiFileEndNb > m_uiFileStartNb)
	{
		for (unsigned int i=m_uiFileStartNb; i<m_uiFileEndNb; i++)
		{
			// 处理自动保存完的数据文件
			OnCalFinishFile(i);
		}
		// 处理最后一个保存的数据文件
		OnCalLastFile(m_uiFileEndNb);
	}
	// 打印计算结果
	for (unsigned int i=0; i<m_uiInstrumentSampleNum; i++)
	{
		strtmp.Format(_T("仪器%d 丢帧个数为%d\r\n"), (i+1), m_uipLostFrameCount[i]);
		strOutput += strtmp;
	}
	GetDlgItem(IDC_EDIT_RESULTSHOW)->SetWindowText(strOutput);
}

// 处理最后一个保存的数据文件
void CLostFrameCountDlg::OnCalLastFile(unsigned int uiLastFileNb)
{
	CString strtmp = _T("");
	CString strFilePath = _T("");
// 	strtmp.Format(_T("\\%d.text"), uiLastFileNb);
// 	strFilePath = m_csFilePath + strtmp;
	ifstream fp_str;
	double datatemp = 0.0;
	strtmp.Format(_T("\\%d.text"), uiLastFileNb);
	strFilePath = m_csFilePath + strtmp;
	setlocale(LC_ALL,"Chinese-simplified");//设置中文环境
	fp_str.open(strFilePath, ios::in);

	if(fp_str.fail())
	{
		::AfxMessageBox(_T("打开数据存储文件出错！"));
		return;
	}
	else
	{
		char wholeline[1024];
		unsigned int * uipZeroCount = NULL;
		uipZeroCount = new unsigned int[m_uiInstrumentSampleNum];
		unsigned int uiDataNum = 0;
		unsigned int uitmp = 0;
		for (unsigned int i=0; i<m_uiInstrumentSampleNum; i++)
		{
			uipZeroCount[i] = 0;
		}
		uiDataNum = m_uiInstrumentSampleNum * m_uiFrameSize * m_uiSaveADCDataFrameNum; 
		fp_str.getline(wholeline, 1024, '\n');
		fp_str.getline(wholeline, 1024, '\n');
		fp_str.getline(wholeline, 1024, '\n');
		for (unsigned int m=0; m<uiDataNum; m++)
		{
			fp_str >>datatemp;
			uitmp = m % m_uiInstrumentSampleNum;
			if (datatemp == 0)
			{
				uipZeroCount[uitmp]++;
			}
			else
			{
				uipZeroCount[uitmp] = 0;
			}
			if (uipZeroCount[uitmp] == m_uiFrameSize)
			{
				uipZeroCount[uitmp] = 0;
				m_uipLostFrameCount[uitmp]++;
			}
		}
		fp_str.close();
		uipZeroCount = NULL;
		delete uipZeroCount;
	}
}

// 处理自动保存完得数据文件
void CLostFrameCountDlg::OnCalFinishFile(unsigned int uiFinishFileNb)
{
	CString strtmp = _T("");
	CString strFilePath = _T("");
	ifstream fp_str;
	double datatemp = 0.0;
	strtmp.Format(_T("\\%d.text"), uiFinishFileNb);
	strFilePath = m_csFilePath + strtmp;
	setlocale(LC_ALL,"Chinese-simplified");//设置中文环境
	fp_str.open(strFilePath, ios::in);

	if(fp_str.fail())
	{
		::AfxMessageBox(_T("打开数据存储文件出错！"));
		return;
	}
	else
	{
		char wholeline[1024];
		unsigned int * uipZeroCount = NULL;
		uipZeroCount = new unsigned int[m_uiInstrumentSampleNum];
		unsigned int uiDataNum = 0;
		unsigned int uitmp = 0;
		for (unsigned int i=0; i<m_uiInstrumentSampleNum; i++)
		{
			uipZeroCount[i] = 0;
		}
		uiDataNum = m_uiInstrumentSampleNum * m_uiFrameSize * m_uiSaveADCDataFrameNum; 
		fp_str.getline(wholeline, 1024, '\n');
		fp_str.getline(wholeline, 1024, '\n');
		fp_str.getline(wholeline, 1024, '\n');
		for (unsigned int m=0; m<uiDataNum; m++)
		{
			fp_str >>datatemp;
			uitmp = m % m_uiInstrumentSampleNum;
			if (datatemp == 0)
			{
				uipZeroCount[uitmp]++;
			}
			else
			{
				uipZeroCount[uitmp] = 0;
			}
			if (uipZeroCount[uitmp] == m_uiFrameSize)
			{
				uipZeroCount[uitmp] = 0;
				m_uipLostFrameCount[uitmp]++;
			}
		}
		fp_str.close();
		uipZeroCount = NULL;
		delete uipZeroCount;
	}
}
