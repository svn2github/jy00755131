
// NetWinPcapDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NetWinPcap.h"
#include "NetWinPcapDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNetWinPcapDlg 对话框




CNetWinPcapDlg::CNetWinPcapDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNetWinPcapDlg::IDD, pParent)
	, m_lEditDownStreamRcvNum(0)
	, m_lEditDownStreamSndNum(0)
	, m_lEditUpStreamRcvNum(0)
	, m_lEditUpStreamSndNum(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNetWinPcapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DOWNSTREAM_RCVNUM, m_lEditDownStreamRcvNum);
	DDX_Text(pDX, IDC_EDIT_DOWNSTREAM_SNDNUM, m_lEditDownStreamSndNum);
	DDX_Text(pDX, IDC_EDIT_UPSTREAM_RCVNUM, m_lEditUpStreamRcvNum);
	DDX_Text(pDX, IDC_EDIT_UPSTREAM_SNDNUM, m_lEditUpStreamSndNum);
}

BEGIN_MESSAGE_MAP(CNetWinPcapDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_RESETNUM, &CNetWinPcapDlg::OnBnClickedButtonResetnum)
END_MESSAGE_MAP()


// CNetWinPcapDlg 消息处理程序

BOOL CNetWinPcapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	// TODO: 在此添加额外的初始化代码
	m_oNetPcapComm.OnInit();
	m_oPcapRcvThread.m_pNetPcapComm = &m_oNetPcapComm;
	m_oPcapRcvThread.OnInit();
	m_oPcapSndThread.m_pNetPcapComm = &m_oNetPcapComm;
	m_oPcapSndThread.OnInit();
	SetTimer(RevSndFrameNumTimerEventID, RevSndFrameNumTimer, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNetWinPcapDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CNetWinPcapDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 命令行参数解析
void CNetWinPcapDlg::PhraseCommandLine(CString str)
{
	int iPos = 0;
	CString strCmd = _T("");
	CString strTemp = _T("");
	iPos = str.Find('=');
	strCmd = str.Left(iPos);
	strTemp = str.Right(str.GetLength() - (iPos + 1));
	if (strCmd == _T("NetCardId"))	// 网卡序号，从0开始
	{
		m_oNetPcapComm.m_uiNetCardId = _ttoi(strTemp);
	}
	else if (strCmd == _T("DownStreamRcvSndPort"))
	{
		str = strTemp;
		while(1)
		{
			iPos = str.Find(',');
			if (iPos == -1)
			{
				m_oNetPcapComm.PhraseRcvSndPort(str, &m_oNetPcapComm.m_oDownStreamRcvSndPortMap);
				break;
			}
			strTemp = str.Left(iPos);

			m_oNetPcapComm.PhraseRcvSndPort(strTemp, &m_oNetPcapComm.m_oDownStreamRcvSndPortMap);
			str = str.Right(str.GetLength() - (iPos + 1));
		}
	}
	else if (strCmd == _T("UpStreamRcvSndPort"))
	{
		str = strTemp;
		while(1)
		{
			iPos = str.Find(',');
			if (iPos == -1)
			{
				m_oNetPcapComm.PhraseRcvSndPort(str, &m_oNetPcapComm.m_oUpStreamRcvSndPortMap);
				break;
			}
			strTemp = str.Left(iPos);

			m_oNetPcapComm.PhraseRcvSndPort(strTemp, &m_oNetPcapComm.m_oUpStreamRcvSndPortMap);
			str = str.Right(str.GetLength() - (iPos + 1));
		}
	}
	else if (strCmd == _T("NetDownStreamSrcPort"))
	{
		m_oNetPcapComm.m_usPcapDownStreamSrcPort = htons(_ttoi(strTemp));
	}
	else if (strCmd == _T("NetUpStreamSrcPort"))
	{
		m_oNetPcapComm.m_usPcapUpStreamSrcPort = htons(_ttoi(strTemp));
	}
	else if (strCmd == _T("DownStreamSndBufSize"))
	{
		m_oNetPcapComm.m_uiDownStreamSndBufSize = _ttoi(strTemp);
	}
	else if (strCmd == _T("UpStreamSndBufSize"))
	{
		m_oNetPcapComm.m_uiUpStreamSndBufSize = _ttoi(strTemp);
	}
	else if (strCmd == _T("WinpcapBufSize"))
	{
		m_oNetPcapComm.m_uiPcapBufSize = _ttoi(strTemp);
	}
	else if (strCmd == _T("HighIP"))
	{
		m_oNetPcapComm.m_uiHighStreamIP = inet_addr((CStringA)strTemp);
	}
	else if (strCmd == _T("LowIP"))
	{
		m_oNetPcapComm.m_uiLowStreamIP = inet_addr((CStringA)strTemp);
	}
// 	else if (strCmd == _T("NetIP"))
// 	{
// 		m_oNetPcapComm.m_uiNetIP = inet_addr((CStringA)strTemp);
// 	}
// 	else if (strCmd == _T("HighMacAddr"))
// 	{
// 		str = strTemp;
// 		while(1)
// 		{
// 			iPos = str.Find(',');
// 			if (iPos == -1)
// 			{
// 				m_oNetPcapComm.m_ucHighMacAddr[iCount] = _ttoi(str);
// 				break;
// 			}
// 			strTemp = str.Left(iPos);
// 			m_oNetPcapComm.m_ucHighMacAddr[iCount] = _ttoi(strTemp);
// 			str = str.Right(str.GetLength() - (iPos + 1));
// 			iCount++;
// 		}
// 	}
// 	else if (strCmd == _T("LowMacAddr"))
// 	{
// 		str = strTemp;
// 		while(1)
// 		{
// 			iPos = str.Find(',');
// 			if (iPos == -1)
// 			{
// 				m_oNetPcapComm.m_ucLowMacAddr[iCount] = _ttoi(str);
// 				break;
// 			}
// 			strTemp = str.Left(iPos);
// 			m_oNetPcapComm.m_ucLowMacAddr[iCount] = _ttoi(strTemp);
// 			str = str.Right(str.GetLength() - (iPos + 1));
// 			iCount++;
// 		}
// 	}
// 	else if (strCmd == _T("NetMacAddr"))
// 	{
// 		str = strTemp;
// 		while(1)
// 		{
// 			iPos = str.Find(',');
// 			if (iPos == -1)
// 			{
// 				m_oNetPcapComm.m_ucNetMacAddr[iCount] = _ttoi(str);
// 				break;
// 			}
// 			strTemp = str.Left(iPos);
// 			m_oNetPcapComm.m_ucNetMacAddr[iCount] = _ttoi(strTemp);
// 			str = str.Right(str.GetLength() - (iPos + 1));
// 			iCount++;
// 		}
// 	}
	else if (strCmd == _T("MaxPackageSize"))
	{
		m_oNetPcapComm.m_uiPcapPacketMaxSize = _ttoi(strTemp);
	}
	else if (strCmd == _T("PcapTimeOut"))
	{
		m_oNetPcapComm.m_uiPcapTimeOut = _ttoi(strTemp);
	}
	else if (strCmd == _T("PcapQueueSize"))
	{
		m_oNetPcapComm.m_uiPcapQueueSize = _ttoi(strTemp);
	}
	else if (strCmd == _T("PcapSndWaitTime"))
	{
		m_oPcapSndThread.m_uiThreadSleepTime = _ttoi(strTemp);
	}
	else if (strCmd == _T("PcapRcvWaitTime"))
	{
		m_oPcapRcvThread.m_uiThreadSleepTime = _ttoi(strTemp);
	}
}
// 隐藏对话框程序界面
void CNetWinPcapDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	// TODO: 在此处添加消息处理程序代码
// 	if(lpwndpos-> flags & SWP_SHOWWINDOW)
// 	{
// 		lpwndpos-> flags &= ~SWP_SHOWWINDOW;
// 		PostMessage(WM_WINDOWPOSCHANGING, 0, (LPARAM)lpwndpos); 
// 		ShowWindow(SW_HIDE); 
// 	} 
// 	else
// 	{
		CDialogEx::OnWindowPosChanging(lpwndpos);
//	}
}


void CNetWinPcapDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_oPcapRcvThread.OnClose();
	m_oPcapSndThread.OnClose();
	m_oNetPcapComm.OnClose();
	KillTimer(RevSndFrameNumTimerEventID);
	CDialogEx::OnClose();
}


void CNetWinPcapDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == RevSndFrameNumTimerEventID)
	{
		m_lEditDownStreamRcvNum = _InterlockedCompareExchange(&m_oNetPcapComm.m_lDownStreamNetRevFrameNum, 0, 0);
		m_lEditDownStreamSndNum = _InterlockedCompareExchange(&m_oNetPcapComm.m_lDownStreamNetSndFrameNum, 0, 0);
		m_lEditUpStreamRcvNum = _InterlockedCompareExchange(&m_oNetPcapComm.m_lUpStreamNetRevFrameNum, 0, 0);
		m_lEditUpStreamSndNum = _InterlockedCompareExchange(&m_oNetPcapComm.m_lUpStreamNetSndFrameNum, 0, 0);
		UpdateData(FALSE);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CNetWinPcapDlg::OnBnClickedButtonResetnum()
{
	// TODO: 在此添加控件通知处理程序代码
	_InterlockedExchange(&m_oNetPcapComm.m_lDownStreamNetRevFrameNum, 0);
	_InterlockedExchange(&m_oNetPcapComm.m_lDownStreamNetSndFrameNum, 0);
	_InterlockedExchange(&m_oNetPcapComm.m_lUpStreamNetRevFrameNum, 0);
	_InterlockedExchange(&m_oNetPcapComm.m_lUpStreamNetSndFrameNum, 0);
}
