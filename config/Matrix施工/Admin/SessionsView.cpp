// SessionsView.cpp : 实现文件
//

#include "stdafx.h"
#include "Admin.h"
#include "SessionsView.h"


// CSessionsView
IMPLEMENT_DYNCREATE(CSessionsView, CBCGPFormView)

CSessionsView::CSessionsView()
	: CBCGPFormView(CSessionsView::IDD)
{
	EnableVisualManagerStyle();
}

CSessionsView::~CSessionsView()
{
}

void CSessionsView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREEUSERINFOS, m_ctrlSessionTree);
	DDX_Control(pDX, IDC_LISTUSERINFO, m_ctrlUserInfo);
}

BEGIN_MESSAGE_MAP(CSessionsView, CBCGPFormView)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREEUSERINFOS, &CSessionsView::OnTvnSelchangedTreeuserinfos)
END_MESSAGE_MAP()


// CSessionsView 诊断

#ifdef _DEBUG
void CSessionsView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSessionsView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSessionsView 消息处理程序
void CSessionsView::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();
	m_ImageList.Create(IDB_BITMAPUSER,20,0,RGB(255,255,255));
	m_ctrlSessionTree.SetImageList(&m_ImageList,TVSIL_NORMAL);
}
// 响应OnSize消息
void CSessionsView::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFormView::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	if (m_ctrlSessionTree.GetSafeHwnd())
	{
		m_ctrlSessionTree.SetWindowPos (NULL, 1, 1, cx-2, cy-140, SWP_NOZORDER | SWP_NOACTIVATE); // SWP_NOMOVE | 
	}
	if (m_ctrlUserInfo.GetSafeHwnd())
	{
		m_ctrlUserInfo.SetWindowPos (NULL,1 , cy-130, cx-2, 130, SWP_NOZORDER | SWP_NOACTIVATE); // SWP_NOMOVE | 
	}
}
/**
 * @brief 加载单个用户的详细信息
 * @note  加载单个用户的详细信息，并列表框中显示出会话ID，客户端IP,状态，创建时间及上次访问时间
 * @param void
 * @param void
 * @return void
 */
void   CSessionsView::LoadSingleUserInfo(CUserInfo* pUserInfo)
{
	if(!pUserInfo)
		return;
	CString      strTemp;
	struct  in_addr sin_addr;
	// 删除原来属性
	m_ctrlUserInfo.ResetContent();
	// Session ID
	strTemp.Format(_T("Session ID     : %d"),pUserInfo->m_dwSessionID);
	m_ctrlUserInfo.AddString(strTemp);
	// Client IP
	sin_addr.S_un.S_addr = pUserInfo->m_dwClientIP;
	strTemp.Format(_T("Client ID      : %d.%d.%d.%d"),sin_addr.S_un.S_un_b.s_b1,sin_addr.S_un.S_un_b.s_b2
		                                             ,sin_addr.S_un.S_un_b.s_b3,sin_addr.S_un.S_un_b.s_b4);
	m_ctrlUserInfo.AddString(strTemp);
	// State
	if(pUserInfo->m_nState ==USERINFO_STATE_ALINE)
	{
		strTemp = _T("State          : Alive");
	}
	else
	{
		strTemp = _T("State          : Offline");
	}
	m_ctrlUserInfo.AddString(strTemp);
	// Created
	COleDateTime		OleDate;
	OleDate = pUserInfo->m_tmRegister;
	strTemp=_T("Created        : ");
	strTemp+=OleDate.Format();
	m_ctrlUserInfo.AddString(strTemp);
	// Last Used
	OleDate = pUserInfo->m_tmLastUsed;
	strTemp=_T("Last Used      : ");
	strTemp+=OleDate.Format();
	m_ctrlUserInfo.AddString(strTemp);
}
/**
 * @brief 加载所有在线用户信息
 * @note  加载所有的在线用户对象，并显示第一个用户的具体信息。
 在接收到管理服务器发送的全部用户信息后，调用该函数显示所有在线用户
 * @param void
 * @param void
 * @return void
 */
void   CSessionsView::LoadAliveUserInfo(void)
{
	int   nUserCount = theApp.m_AllUserInfo.GetCount();
	int   nAliveUserCount = theApp.m_AllUserInfo.GetAliveCount();
	int   i;
	CString		strTemp;
	CUserInfo*  pUserInfo = NULL;

	strTemp.Format(_T("sessions(%d/%d)"),nAliveUserCount,nUserCount);
	HTREEITEM   hRoot = m_ctrlSessionTree.InsertItem(strTemp,0,0);
	HTREEITEM   hItem;
	for (i=0;i<nUserCount;i++)
	{
		pUserInfo = theApp.m_AllUserInfo.GetUserInfo(i);
		if( !pUserInfo)	
			continue;
		if(pUserInfo->m_nState == USERINFO_STATE_ALINE)
		{
			hItem=m_ctrlSessionTree.InsertItem(pUserInfo->m_strUserName,1,1,hRoot);
			// 设置条目的数据属性为该用户对象在用户数组中索引位置
			m_ctrlSessionTree.SetItemData(hItem,i);
		}
	}
	m_ctrlSessionTree.Expand(hRoot,TVE_EXPAND);
}
// 当前选择的条目发生改变
void CSessionsView::OnTvnSelchangedTreeuserinfos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM  hItem=pNMTreeView->itemNew.hItem;
	int  nIndex = m_ctrlSessionTree.GetItemData(hItem);
	
	// 得到当前被选中的用户对象
	CUserInfo* pUserInfo = NULL;
	pUserInfo=theApp.m_AllUserInfo.GetUserInfo(nIndex);
	if(!pUserInfo)
		return;
	// 加载被选中的用户对象属性
	LoadSingleUserInfo(pUserInfo);

	*pResult = 0;
}
