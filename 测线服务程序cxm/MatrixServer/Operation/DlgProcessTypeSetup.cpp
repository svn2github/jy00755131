// DlgProcessTypeSetup.cpp : 实现文件
//

#include "stdafx.h"
#include "Operation.h"
#include "DlgProcessTypeSetup.h"
#include "afxdialogex.h"


// CDlgProcessTypeSetup 对话框

IMPLEMENT_DYNAMIC(CDlgProcessTypeSetup, CBCGPDialog)

CDlgProcessTypeSetup::CDlgProcessTypeSetup(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgProcessTypeSetup::IDD, pParent)
	, m_radioProcessSetup(PROCESS_STANDARD)
	, m_cbindexProcessType(PROCESS_IMPULSIVE)
	, m_cbindexPlug(0)
	, m_cbindexBoxType(0)
	, m_editindexSerialNb(2442269)
	, m_editindexRecordLength(4000)
	, m_editindexTBWindow(3000)
	, m_editindexPeakTime(0)
	, m_bCheckRaw(FALSE)
	, m_strCorrelWith(_T(""))
	, m_bAuxesFromDSD(FALSE)
{
//	EnableVisualManagerStyle();
}

CDlgProcessTypeSetup::~CDlgProcessTypeSetup()
{
}

void CDlgProcessTypeSetup::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_STANDARD, m_radioProcessSetup);
	DDX_CBIndex(pDX, IDC_COMBO_PROCESSTYPESELECT, m_cbindexProcessType);
	DDX_CBIndex(pDX, IDC_COMBO_PLUG, m_cbindexPlug);
	DDX_CBIndex(pDX, IDC_COMBO_BOXTYPE, m_cbindexBoxType);
	DDX_Text(pDX, IDC_EDIT_SERIALNB, m_editindexSerialNb);
	DDX_Text(pDX, IDC_EDIT_RECORDLENGTH, m_editindexRecordLength);
	DDX_Text(pDX, IDC_EDIT_TBWINDOW, m_editindexTBWindow);
	DDX_Text(pDX, IDC_EDIT_PEAKTIME, m_editindexPeakTime);
	DDX_Check(pDX, IDC_CHECK_RAW, m_bCheckRaw);
	DDX_Text(pDX, IDC_EDIT_AUX_COR, m_strCorrelWith);
	DDX_Check(pDX, IDC_CHECK_AUX, m_bAuxesFromDSD);
}


BEGIN_MESSAGE_MAP(CDlgProcessTypeSetup, CBCGPDialog)
	ON_BN_CLICKED(IDC_RADIO_STANDARD, &CDlgProcessTypeSetup::OnBnClickedRadioStandard)
	ON_BN_CLICKED(IDC_RADIO_ADVANCED, &CDlgProcessTypeSetup::OnBnClickedRadioAdvanced)
	ON_CBN_SELCHANGE(IDC_COMBO_PROCESSTYPESELECT, &CDlgProcessTypeSetup::OnCbnSelchangeComboProcesstypeselect)
	ON_BN_CLICKED(IDC_BUTTON_AUX_ADD, &CDlgProcessTypeSetup::OnBnClickedButtonAuxAdd)
	ON_BN_CLICKED(IDC_BUTTON_AUX_CHANGE, &CDlgProcessTypeSetup::OnBnClickedButtonAuxChange)
	ON_BN_CLICKED(IDC_BUTTON_AUX_DELETE, &CDlgProcessTypeSetup::OnBnClickedButtonAuxDelete)
	ON_BN_CLICKED(IDC_BUTTON_ACQ_ADD, &CDlgProcessTypeSetup::OnBnClickedButtonAcqAdd)
	ON_BN_CLICKED(IDC_BUTTON_ACQ_CHANGE, &CDlgProcessTypeSetup::OnBnClickedButtonAcqChange)
	ON_BN_CLICKED(IDC_BUTTON_ACQ_DELETE, &CDlgProcessTypeSetup::OnBnClickedButtonAcqDelete)
	ON_BN_CLICKED(IDC_BUTTON_PRO_ADD, &CDlgProcessTypeSetup::OnBnClickedButtonProAdd)
	ON_BN_CLICKED(IDC_BUTTON_PRO_CHANGE, &CDlgProcessTypeSetup::OnBnClickedButtonProChange)
	ON_BN_CLICKED(IDC_BUTTON_PRO_DELETE, &CDlgProcessTypeSetup::OnBnClickedButtonProDelete)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDlgProcessTypeSetup::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CDlgProcessTypeSetup::OnBnClickedButtonReset)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgProcessTypeSetup 消息处理程序
BOOL CDlgProcessTypeSetup::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	OnShowAuxWindow();
	OnShowProTypeWindow();
	OnShowProcessTypeWindow(PROCESS_IMPULSIVE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgProcessTypeSetup::OnBnClickedRadioStandard()
{
	// TODO: 在此添加控件通知处理程序代码
	m_radioProcessSetup = PROCESS_STANDARD;
}


void CDlgProcessTypeSetup::OnBnClickedRadioAdvanced()
{
	// TODO: 在此添加控件通知处理程序代码
	m_radioProcessSetup = PROCESS_ADVANCE;
}


void CDlgProcessTypeSetup::OnCbnSelchangeComboProcesstypeselect()
{
	// TODO: 在此添加控件通知处理程序代码
	// 更新控件变量的值
	UpdateData ();
	OnShowProcessTypeWindow(m_cbindexProcessType);
	// 刷新界面
	Invalidate(TRUE);
}


void CDlgProcessTypeSetup::OnBnClickedButtonAuxAdd()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgProcessTypeSetup::OnBnClickedButtonAuxChange()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgProcessTypeSetup::OnBnClickedButtonAuxDelete()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgProcessTypeSetup::OnBnClickedButtonAcqAdd()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgProcessTypeSetup::OnBnClickedButtonAcqChange()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgProcessTypeSetup::OnBnClickedButtonAcqDelete()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgProcessTypeSetup::OnBnClickedButtonProAdd()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgProcessTypeSetup::OnBnClickedButtonProChange()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgProcessTypeSetup::OnBnClickedButtonProDelete()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgProcessTypeSetup::OnBnClickedButtonApply()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgProcessTypeSetup::OnBnClickedButtonReset()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgProcessTypeSetup::OnDestroy()
{
	CBCGPDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_wndAuxEditGrid.RemoveAll();
	m_wndAuxListGrid.RemoveAll();
	m_wndAcqEditGrid.RemoveAll();
	m_wndAcqListGrid.RemoveAll();
	m_wndProTypeEditGrid.RemoveAll();
	m_wndProTypeListGrid.RemoveAll();
}


// 显示处理类型窗口
void CDlgProcessTypeSetup::OnShowProcessTypeWindow(int iProcessType)
{
	CRect rectAux, rectAcq;
	switch(iProcessType)
	{
	case PROCESS_IMPULSIVE:
		GetDlgItem(IDC_STATIC_RAW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_RAW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PEAKTIME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PEAKTIME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TBWINDOW)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_TBWINDOW)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_AUX_COR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_AUX_COR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_AUX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ACQGROUP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ACQ)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ACQNB)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ACQTYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ACQSIGNSTACK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ACQOUTPUT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ACQ_GRID_EDIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ACQ_GRID)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ACQ_ADD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ACQ_CHANGE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ACQ_DELETE)->ShowWindow(SW_HIDE);
		SetProcessImpulsiveCtrlsRect();
		break;
	case PROCESS_STACKIMPULSIVE:
		break;
	case PROCESS_CORRELAFTER:
		break;
	case PROCESS_CORRELBEFORE:
		break;
	case PROCESS_STACK:
		break;
	default:
		break;
	}
}


// 显示Aux窗口
void CDlgProcessTypeSetup::OnShowAuxWindow(void)
{
	m_wndAuxEditGrid.RemoveAll();
	m_wndAuxListGrid.RemoveAll();
	CRect rectGrid;
	int iNbWidth, iProcessingWidth;
	GetDlgItem(IDC_STATIC_AUX_GRID_EDIT)->GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndAuxEditGrid.CreateGrid(rectGrid,this);
	GetDlgItem(IDC_STATIC_AUXNB)->GetWindowRect (&rectGrid);
	iNbWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_AUXPRO)->GetWindowRect (&rectGrid);
	iProcessingWidth = rectGrid.Width();
	m_wndAuxEditGrid.InsertColumn (0, _T("Nb"), iNbWidth);
	m_wndAuxEditGrid.InsertColumn (1, _T("Processing"), iProcessingWidth);

	GetDlgItem(IDC_STATIC_AUX_GRID)->GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndAuxListGrid.CreateGrid(rectGrid,this);
	m_wndAuxListGrid.InsertColumn (0, _T("Nb"), iNbWidth);
	m_wndAuxListGrid.InsertColumn (1, _T("Processing"), iProcessingWidth);
	for(int i=0;i<2;i++)
	{
		m_wndAuxEditGrid.SetColumnLocked(i,TRUE);
		m_wndAuxListGrid.SetColumnLocked(i,TRUE);
	}
	m_wndAuxEditGrid.AddRow();
	m_wndAuxEditGrid.AdjustLayout();
}


// 显示Process Type窗口
void CDlgProcessTypeSetup::OnShowProTypeWindow(void)
{
	m_wndProTypeEditGrid.RemoveAll();
	m_wndProTypeListGrid.RemoveAll();
	CRect rectGrid;
	int iNbWidth, iLabelWidth;
	GetDlgItem(IDC_STATIC_PRO_GRID_EDIT)->GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndProTypeEditGrid.CreateGrid(rectGrid,this);
	GetDlgItem(IDC_STATIC_PROTYPE_NB)->GetWindowRect (&rectGrid);
	iNbWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_PROTYPE_LABEL)->GetWindowRect (&rectGrid);
	iLabelWidth = rectGrid.Width();
	m_wndProTypeEditGrid.InsertColumn (0, _T("Nb"), iNbWidth);
	m_wndProTypeEditGrid.InsertColumn (1, _T("Label"), iLabelWidth);

	GetDlgItem(IDC_STATIC_PRO_GRID)->GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndProTypeListGrid.CreateGrid(rectGrid,this);
	m_wndProTypeListGrid.InsertColumn (0, _T("Nb"), iNbWidth);
	m_wndProTypeListGrid.InsertColumn (1, _T("Label"), iLabelWidth);
	for(int i=0;i<2;i++)
	{
		m_wndProTypeEditGrid.SetColumnLocked(i,TRUE);
		m_wndProTypeListGrid.SetColumnLocked(i,TRUE);
	}
	m_wndProTypeEditGrid.AddRow();
	m_wndProTypeEditGrid.AdjustLayout();
}


// 设置Impulsive处理类型控件的位置
void CDlgProcessTypeSetup::SetProcessImpulsiveCtrlsRect(void)
{
	CRect rect, rectAuxGroupOld, rectAuxGroupNew;
	int iWidth = 0;
	// 设置IDC_STATIC_AUXGROUP控件位置
	GetDlgItem(IDC_STATIC_PROTYPEGROUP)->GetWindowRect(&rect);
	iWidth = rect.Width();
	GetDlgItem(IDC_STATIC_AUXGROUP)->GetWindowRect(&rectAuxGroupOld);
	if (rectAuxGroupOld.Width() == iWidth)
	{
		return;
	}
	rect = rectAuxGroupOld;
	rect.right = rect.left + iWidth;
	rectAuxGroupNew = rect;
	ScreenToClient(&rect);
	GetDlgItem(IDC_STATIC_AUXGROUP)->MoveWindow(&rect);
	// 设置IDC_BUTTON_AUX_ADD控件位置
	GetDlgItem(IDC_BUTTON_AUX_ADD)->GetWindowRect(&rect);
	iWidth = rect.Width();
	rect.left = rectAuxGroupNew.right - 5 - iWidth;
	rect.right = rect.left + iWidth;
	ScreenToClient(&rect);
	GetDlgItem(IDC_BUTTON_AUX_ADD)->MoveWindow(&rect);
	// 设置IDC_BUTTON_AUX_CHANGE控件位置
	GetDlgItem(IDC_BUTTON_AUX_CHANGE)->GetWindowRect(&rect);
	iWidth = rect.Width();
	rect.left = rectAuxGroupNew.right - 5 - iWidth;
	rect.right = rect.left + iWidth;
	ScreenToClient(&rect);
	GetDlgItem(IDC_BUTTON_AUX_CHANGE)->MoveWindow(&rect);
	// 设置IDC_BUTTON_AUX_DELETE控件位置
	GetDlgItem(IDC_BUTTON_AUX_DELETE)->GetWindowRect(&rect);
	iWidth = rect.Width();
	rect.left = rectAuxGroupNew.right - 5 - iWidth;
	rect.right = rect.left + iWidth;
	ScreenToClient(&rect);
	GetDlgItem(IDC_BUTTON_AUX_DELETE)->MoveWindow(&rect);
}
