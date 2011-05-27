// ShowThread.cpp : 实现文件
//

#include "stdafx.h"
#include "端口监视器.h"
#include "ShowThread.h"
#include "Parameter.h"

// CShowThread

IMPLEMENT_DYNCREATE(CShowThread, CWinThread)

CShowThread::CShowThread()
{
}

CShowThread::~CShowThread()
{
}

BOOL CShowThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CShowThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CShowThread, CWinThread)
END_MESSAGE_MAP()


// CShowThread 消息处理程序
// 初始化

void CShowThread::Init()
{
	m_close = false;
	m_icsEditShowLength = 0;
}
// 线程函数在Run中执行
int CShowThread::Run()
{
	// 循环，直到关闭标志为真
	while(true)
	{
		if(m_close == true)
		{
			break;
		}
		CString strtemp1;
		CString strtemp2;
// 		strtemp1 = m_csEditShow;
// 		if (m_icsEditShowLength < strtemp1.GetLength())
// 		{
// 			m_icsEditShowLength = strtemp1.GetLength();
// 			pDlg->GetDlgItem(IDC_EDIT_RECEIVE)->SetWindowText(strtemp1);
// 		}
		strtemp2 = m_csSaveFile;
		if (strtemp2.GetLength()> (pDlg->m_iSaveSize*1024))
		{	
			pDlg->OnSaveToFile();
		}
		if (m_close == true)
		{
			break;
		}
		Sleep(250);
	}


	// 返回
	return CWinThread::Run();
}
void CShowThread::OnClose()
{
	m_close = true;
}
