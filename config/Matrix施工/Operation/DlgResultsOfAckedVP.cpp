// DlgResultsOfAckedVP.cpp : 实现文件
//

#include "stdafx.h"
#include "Operation.h"
#include "DlgResultsOfAckedVP.h"


// CDlgResultsOfAckedVP 对话框

IMPLEMENT_DYNAMIC(CDlgResultsOfAckedVP, CBCGPDialog)

CDlgResultsOfAckedVP::CDlgResultsOfAckedVP(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgResultsOfAckedVP::IDD, pParent)
	, m_strResult(_T(""))
{
	EnableVisualManagerStyle();
}

CDlgResultsOfAckedVP::~CDlgResultsOfAckedVP()
{
	m_arrResult.RemoveAll();
}

void CDlgResultsOfAckedVP::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTRESULT, m_ctrlListResult);
	DDX_Text(pDX, IDC_STATICRESULT, m_strResult);
}


BEGIN_MESSAGE_MAP(CDlgResultsOfAckedVP, CBCGPDialog)
END_MESSAGE_MAP()


// CDlgResultsOfAckedVP 消息处理程序

BOOL CDlgResultsOfAckedVP::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int  i,nCount;
	nCount = m_arrResult.GetCount();
	for (i=0;i<nCount;i++)
	{
		// m_ctrlListResult.InsertItem(m_ctrlListResult.GetItemCount(),m_arrResult[i]);
		// 将每个无应答采集道信息写入列表框
		m_ctrlListResult.AddString(m_arrResult[i]);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
