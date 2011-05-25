// DlgCommentType.cpp : 实现文件
//

#include "stdafx.h"
#include "Operation.h"
#include "DlgCommentType.h"


// CDlgCommentType 对话框

IMPLEMENT_DYNAMIC(CDlgCommentType, CBCGPDialog)

CDlgCommentType::CDlgCommentType(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgCommentType::IDD, pParent)
	, m_strComment(_T(""))
{
	EnableVisualManagerStyle();
}

CDlgCommentType::~CDlgCommentType()
{
}

void CDlgCommentType::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATICGRIDEDIT, m_ctrlEditGridLocation);
	DDX_Control(pDX, IDC_STATICGRIDLIST, m_ctrlListGridLocation);
	DDX_Text(pDX, IDC_EDITCOMMENT, m_strComment);
}


BEGIN_MESSAGE_MAP(CDlgCommentType, CBCGPDialog)
END_MESSAGE_MAP()


// CDlgCommentType 消息处理程序

BOOL CDlgCommentType::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rectGrid;	
	m_ctrlListGridLocation.GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndListGrid.CreateGrid(rectGrid,this);
	m_wndListGrid.InsertColumn (0, _T("Nb"), 65);
	m_wndListGrid.InsertColumn (1, _T("Label"), 200);
	
	m_ctrlEditGridLocation.GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndEditGrid.CreateGrid(rectGrid,this);

	m_wndEditGrid.InsertColumn (0, _T("Nb"), 65);
	m_wndEditGrid.InsertColumn (1, _T("Label"), 200);
	
	m_wndEditGrid.AddRow();
	m_wndEditGrid.AdjustLayout();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
