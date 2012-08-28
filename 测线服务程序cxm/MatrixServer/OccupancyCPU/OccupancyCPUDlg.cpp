
// OccupancyCPUDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OccupancyCPU.h"
#include "OccupancyCPUDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COccupancyCPUDlg 对话框




COccupancyCPUDlg::COccupancyCPUDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COccupancyCPUDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pThreadArray = NULL;
	m_uiThreadNum = 1;
}

void COccupancyCPUDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COccupancyCPUDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_START, &COccupancyCPUDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &COccupancyCPUDlg::OnBnClickedButtonStop)
END_MESSAGE_MAP()


// COccupancyCPUDlg 消息处理程序

BOOL COccupancyCPUDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetDlgItemInt(IDC_COMBO_THREAD_NUM, m_uiThreadNum);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COccupancyCPUDlg::OnPaint()
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
HCURSOR COccupancyCPUDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void COccupancyCPUDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnBnClickedButtonStop();
	CDialogEx::OnClose();
}


void COccupancyCPUDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	m_uiThreadNum = GetDlgItemInt(IDC_COMBO_THREAD_NUM);
	if (m_uiThreadNum == 0)
	{
		return;
	}
	m_pThreadArray = new CThread[m_uiThreadNum];
	for (int i=0; i<m_uiThreadNum; i++)
	{
		m_pThreadArray[i].CreateThread();
	}
}


void COccupancyCPUDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pThreadArray != NULL)
	{
		for (int i=0; i<m_uiThreadNum; i++)
		{
			m_pThreadArray[i].Close();
		}
		Sleep(100);
		delete[] m_pThreadArray;
		m_pThreadArray = NULL;
	}
}
