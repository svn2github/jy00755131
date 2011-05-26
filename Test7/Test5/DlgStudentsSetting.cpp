// DlgStudentsSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "Test5.h"
#include "DlgStudentsSetting.h"
#include "Variable.h"

// CDlgStudentsSetting 对话框

IMPLEMENT_DYNAMIC(CDlgStudentsSetting, CBCGPDialog)

CDlgStudentsSetting::CDlgStudentsSetting(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgStudentsSetting::IDD, pParent)
{
	EnableVisualManagerStyle();
}

CDlgStudentsSetting::~CDlgStudentsSetting()
{
}

void CDlgStudentsSetting::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATICGRIDEDIT, m_ctrlEditGridLocation);
	DDX_Control(pDX, IDC_STATICGRID, m_ctrlGridLocation);
}


BEGIN_MESSAGE_MAP(CDlgStudentsSetting, CBCGPDialog)
END_MESSAGE_MAP()


// CDlgStudentsSetting 消息处理程序

BOOL CDlgStudentsSetting::OnInitDialog(void)
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rectGrid;	
	m_ctrlGridLocation.GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndListGrid.CreateGrid(rectGrid,this);

	m_wndListGrid.InsertColumn (0, _T(" 姓名 "), 100);			//此处的nWidth为比例，并不是真实尺寸，会随着控件宽度的变化而变化
	m_wndListGrid.InsertColumn (1, _T(" 年龄 "), 100);			//设定的列表宽度为控件宽度width*(100/(100+100+100+150+100+150+100))
	m_wndListGrid.InsertColumn (2, _T(" 籍贯 "), 100);
	m_wndListGrid.InsertColumn (3, _T(" 出生地 "), 150);
	m_wndListGrid.InsertColumn (4, _T(" 政治面貌 "), 100);
	m_wndListGrid.InsertColumn (5, _T(" 婚姻状况 "), 150);
	m_wndListGrid.InsertColumn (6, _T(" 联系电话 "), 100);

	m_ctrlEditGridLocation.GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndEditGrid.CreateGrid(rectGrid,this);

	m_wndEditGrid.InsertColumn (0, _T(" 姓名 "), 100);
	m_wndEditGrid.InsertColumn (1, _T(" 年龄 "), 100);
	m_wndEditGrid.InsertColumn (2, _T(" 籍贯 "), 100);
	m_wndEditGrid.InsertColumn (3, _T(" 出生地 "), 150);
	m_wndEditGrid.InsertColumn (4, _T(" 政治面貌 "), 100);
	m_wndEditGrid.InsertColumn (4, _T(" 婚姻状况 "), 150);
	m_wndEditGrid.InsertColumn (4, _T(" 联系电话 "), 100);


	CBCGPGridRow* pRow = m_wndEditGrid.CreateRow (m_wndEditGrid.GetColumnCount ());
	CBCGPGridItem* pItem = new CBCGPGridItem (_T("中共党员"));
	pItem->AddOption (_T("中共党员"), TRUE);
	pItem->AddOption (_T("群众"), TRUE);
	// pItem->AllowEdit(FALSE);
	pRow->ReplaceItem (4, pItem);
	pRow->ReplaceItem (5, new CBCGPGridCheckItem (FALSE));
	m_wndEditGrid.AddRow(pRow);
	m_wndEditGrid.AdjustLayout();

	Load(members);
	return TRUE; 
}

// 载入表格数据
void CDlgStudentsSetting::Load(CString csstudents)
{
	CString member1,member2,member3;
	int n = 0;
	int m = 0;
	int p = 0;
	n = csstudents.Find(_T(";"), 0);
	member1 = csstudents.Mid(0, n);
	m = csstudents.Find(_T(";"), n+1);
	member2 = csstudents.Mid(n+1, m-n-1);
	p = csstudents.Find(_T(";"), m+1);
	member3 = csstudents.Mid(m+1, p-m-1);

	LoadStudent(member2);
	LoadStudent(member3);
}

// 载入一个学生个人信息
void CDlgStudentsSetting::LoadStudent(CString csstudent)
{
	CString name, age, homeplace, birthplace, political, homeaddr, telephonenum;
	int n = 0;
	int m = 0;
	int p = 0;
	n = csstudent.Find(_T(" "), 0);
	name = csstudent.Mid(0, n);
	m = csstudent.Find(_T(" "), n+1);
	age = csstudent.Mid(n+1, m-n-1);
	p = csstudent.Find(_T(" "), m+1);
	homeplace = csstudent.Mid(m+1, p-m-1);
	n = csstudent.Find(_T(" "), p+1);
	birthplace = csstudent.Mid(p+1, n-p-1);
	m = csstudent.Find(_T(" "), n+1);
	political = csstudent.Mid(n+1, m-n-1);
	p = csstudent.Find(_T(" "), m+1);
	homeaddr = csstudent.Mid(m+1, p-m-1);
	n = csstudent.Find(_T(" "), p+1);
	telephonenum = csstudent.Mid(p+1, n-p-1);

	CBCGPGridRow* pRow = m_wndListGrid.CreateRow(m_wndListGrid.GetColumnCount());

	pRow->GetItem (0)->SetValue (CstringToWcharT(name));
	pRow->GetItem (1)->SetValue (CstringToWcharT(age));
	pRow->GetItem (2)->SetValue (CstringToWcharT(homeplace));
	pRow->GetItem (3)->SetValue (CstringToWcharT(birthplace));
	pRow->GetItem (4)->SetValue (CstringToWcharT(political));
	pRow->GetItem (5)->SetValue (CstringToWcharT(homeaddr));
	pRow->GetItem (6)->SetValue (CstringToWcharT(telephonenum));
	// 增加一行记录
	m_wndListGrid.AddRow(pRow);	
	m_wndListGrid.AdjustLayout();
}
