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
	CRect rectGrid;
	GetDlgItem(IDC_STATIC_AUX_GRID_EDIT)->GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndAuxEditGrid.CreateGrid(rectGrid,this);
	GetDlgItem(IDC_STATIC_AUX_GRID)->GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndAuxListGrid.CreateGrid(rectGrid,this);
	GetDlgItem(IDC_STATIC_PRO_GRID_EDIT)->GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndProTypeEditGrid.CreateGrid(rectGrid,this);
	GetDlgItem(IDC_STATIC_PRO_GRID)->GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndProTypeListGrid.CreateGrid(rectGrid,this);

	GetDlgItem(IDC_STATIC_ACQ_GRID_EDIT)->GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndAcqEditGrid.CreateGrid(rectGrid,this);
	GetDlgItem(IDC_STATIC_ACQ_GRID)->GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndAcqListGrid.CreateGrid(rectGrid,this);

	OnShowProcessTypeWindow(PROCESS_IMPULSIVE);
	OnShowProTypeWindow();
	MoveTitleToCenter((CWnd*)this);
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
	switch(iProcessType)
	{
	case PROCESS_IMPULSIVE:
		m_oProTypeControlsShow.m_bTbWindow = true;
		m_oProTypeControlsShow.m_bPeakTime = false;
		m_oProTypeControlsShow.m_bRaw = false;

		m_oAuxControlsShow.m_bAuxCor = false;
		m_oAuxControlsShow.m_bAuxCheck = false;

		m_oAcqControlsShow.m_bAcqGroup = false;
		m_oAcqControlsShow.m_bAcq = false;
		m_oAcqControlsShow.m_bAcqNb = false;
		m_oAcqControlsShow.m_bAcqType = false;
		m_oAcqControlsShow.m_bAcqSignStack = false;
		m_oAcqControlsShow.m_bAcqOutput = false;
		m_oAcqControlsShow.m_bAcqGridEdit = false;
		m_oAcqControlsShow.m_bAcqGridList = false;
		m_oAcqControlsShow.m_bAcqEdit = false;
		break;
	case PROCESS_STACKIMPULSIVE:
		m_oProTypeControlsShow.m_bTbWindow = true;
		m_oProTypeControlsShow.m_bPeakTime = false;
		m_oProTypeControlsShow.m_bRaw = true;

		m_oAuxControlsShow.m_bAuxCor = false;
		m_oAuxControlsShow.m_bAuxCheck = false;

		m_oAcqControlsShow.m_bAcqGroup = true;
		m_oAcqControlsShow.m_bAcq = true;
		m_oAcqControlsShow.m_bAcqNb = true;
		m_oAcqControlsShow.m_bAcqType = false;
		m_oAcqControlsShow.m_bAcqSignStack = false;
		m_oAcqControlsShow.m_bAcqOutput = true;
		m_oAcqControlsShow.m_bAcqGridEdit = true;
		m_oAcqControlsShow.m_bAcqGridList = true;
		m_oAcqControlsShow.m_bAcqEdit = true;
//		ReSetProStackImpulsiveCtrlsRect();
		break;
	case PROCESS_CORRELAFTER:
		m_oProTypeControlsShow.m_bTbWindow = false;
		m_oProTypeControlsShow.m_bPeakTime = true;
		m_oProTypeControlsShow.m_bRaw = true;

		m_oAuxControlsShow.m_bAuxCor = true;
		m_oAuxControlsShow.m_bAuxCheck = true;

		m_oAcqControlsShow.m_bAcqGroup = true;
		m_oAcqControlsShow.m_bAcq = true;
		m_oAcqControlsShow.m_bAcqNb = true;
		m_oAcqControlsShow.m_bAcqType = true;
		m_oAcqControlsShow.m_bAcqSignStack = true;
		m_oAcqControlsShow.m_bAcqOutput = true;
		m_oAcqControlsShow.m_bAcqGridEdit = true;
		m_oAcqControlsShow.m_bAcqGridList = true;
		m_oAcqControlsShow.m_bAcqEdit = true;
		break;
	case PROCESS_CORRELBEFORE:
		m_oProTypeControlsShow.m_bTbWindow = false;
		m_oProTypeControlsShow.m_bPeakTime = true;
		m_oProTypeControlsShow.m_bRaw = true;

		m_oAuxControlsShow.m_bAuxCor = true;
		m_oAuxControlsShow.m_bAuxCheck = true;

		m_oAcqControlsShow.m_bAcqGroup = true;
		m_oAcqControlsShow.m_bAcq = true;
		m_oAcqControlsShow.m_bAcqNb = true;
		m_oAcqControlsShow.m_bAcqType = true;
		m_oAcqControlsShow.m_bAcqSignStack = false;
		m_oAcqControlsShow.m_bAcqOutput = true;
		m_oAcqControlsShow.m_bAcqGridEdit = true;
		m_oAcqControlsShow.m_bAcqGridList = true;
		m_oAcqControlsShow.m_bAcqEdit = true;
		break;
	case PROCESS_STACK:
		m_oProTypeControlsShow.m_bTbWindow = false;
		m_oProTypeControlsShow.m_bPeakTime = false;
		m_oProTypeControlsShow.m_bRaw = true;

		m_oAuxControlsShow.m_bAuxCor = false;
		m_oAuxControlsShow.m_bAuxCheck = true;

		m_oAcqControlsShow.m_bAcqGroup = true;
		m_oAcqControlsShow.m_bAcq = true;
		m_oAcqControlsShow.m_bAcqNb = true;
		m_oAcqControlsShow.m_bAcqType = true;
		m_oAcqControlsShow.m_bAcqSignStack = true;
		m_oAcqControlsShow.m_bAcqOutput = true;
		m_oAcqControlsShow.m_bAcqGridEdit = true;
		m_oAcqControlsShow.m_bAcqGridList = true;
		m_oAcqControlsShow.m_bAcqEdit = true;
		break;
	default:
		break;
	}
	OnShowControls();
	OnShowAcqWindow();
	OnShowAuxWindow();
}


// 显示Aux窗口
void CDlgProcessTypeSetup::OnShowAuxWindow(void)
{

	CRect rect;
	int iNbWidth, iProcessingWidth;
	m_wndAuxEditGrid.DeleteAllColumns();
	m_wndAuxListGrid.DeleteAllColumns();
	m_wndAuxEditGrid.RemoveAll();
	m_wndAuxListGrid.RemoveAll();
	GetDlgItem(IDC_STATIC_AUXNB)->GetWindowRect (&rect);
	iNbWidth = rect.Width();
	GetDlgItem(IDC_STATIC_AUXPRO)->GetWindowRect (&rect);
	iProcessingWidth = rect.Width();
	m_wndAuxEditGrid.InsertColumn (0, _T("Nb"), iNbWidth);
	m_wndAuxListGrid.InsertColumn (0, _T("Nb"), iNbWidth);
	m_wndAuxEditGrid.InsertColumn (1, _T("Processing"), iProcessingWidth-1);
	m_wndAuxListGrid.InsertColumn (1, _T("Processing"), iProcessingWidth-1);
	for(int i=0;i<m_wndAuxEditGrid.GetColumnCount();i++)
	{
		m_wndAuxEditGrid.SetColumnLocked(i,TRUE);
		m_wndAuxListGrid.SetColumnLocked(i,TRUE);
	}
	m_wndAuxEditGrid.AddRow();
	m_wndAuxEditGrid.AdjustLayout();
}

// 显示Acq窗口
void CDlgProcessTypeSetup::OnShowAcqWindow(void)
{

	CRect rect;
	int iNbWidth, iTypeWidth, iSignStackWidth, iOutPutWidth;
	int iCount = 0;
	m_wndAcqEditGrid.DeleteAllColumns();
	m_wndAcqListGrid.DeleteAllColumns();
	m_wndAcqEditGrid.RemoveAll();
	m_wndAcqListGrid.RemoveAll();
	if (m_oAcqControlsShow.m_bAcqNb == true)
	{
		GetDlgItem(IDC_STATIC_ACQNB)->GetWindowRect (&rect);
		iNbWidth = rect.Width();
		m_wndAcqEditGrid.InsertColumn (iCount, _T("Nb"), iNbWidth);
		m_wndAcqListGrid.InsertColumn (iCount, _T("Nb"), iNbWidth);
		iCount++;
	}
	if (m_oAcqControlsShow.m_bAcqType == true)
	{
		GetDlgItem(IDC_STATIC_ACQTYPE)->GetWindowRect (&rect);
		iTypeWidth = rect.Width();
		m_wndAcqEditGrid.InsertColumn (iCount, _T("Type"), iTypeWidth);
		m_wndAcqListGrid.InsertColumn (iCount, _T("Type"), iTypeWidth);
		iCount++;
	}
	if (m_oAcqControlsShow.m_bAcqSignStack == true)
	{
		GetDlgItem(IDC_STATIC_ACQSIGNSTACK)->GetWindowRect (&rect);
		iSignStackWidth = rect.Width();
		m_wndAcqEditGrid.InsertColumn (iCount, _T("Sign Stack"), iSignStackWidth);
		m_wndAcqListGrid.InsertColumn (iCount, _T("Sign Stack"), iSignStackWidth);
		iCount++;
	}
	if (m_oAcqControlsShow.m_bAcqOutput == true)
	{
		GetDlgItem(IDC_STATIC_ACQOUTPUT)->GetWindowRect (&rect);
		iOutPutWidth = rect.Width();
		m_wndAcqEditGrid.InsertColumn (iCount, _T("OutPut"), iOutPutWidth);
		m_wndAcqListGrid.InsertColumn (iCount, _T("OutPut"), iOutPutWidth);
		iCount++;
	}

	for(int i=0;i<iCount;i++)
	{
		m_wndAcqEditGrid.SetColumnLocked(i,TRUE);
		m_wndAcqListGrid.SetColumnLocked(i,TRUE);
	}
	CBCGPGridRow* pRow = m_wndAcqEditGrid.CreateRow (iCount);
	int iColumnNum = 0;
	if (iCount > 0)
	{
		if (m_oAcqControlsShow.m_bAcqNb == true)
		{
			pRow->GetItem (iColumnNum)->SetValue (_T(""));
			iColumnNum++;
		}
		if (m_oAcqControlsShow.m_bAcqType == true)
		{
			pRow->GetItem (iColumnNum)->SetValue (1);
			iColumnNum++;
		}
		if (m_oAcqControlsShow.m_bAcqSignStack == true)
		{
			CBCGPGridItem* pItem = new CBCGPGridItem (ACQSIGNSTACK_STRING_PLUS);
			pItem->AddOption (ACQSIGNSTACK_STRING_PLUS, TRUE);
			pRow->ReplaceItem (iColumnNum, pItem);
			iColumnNum++;
		}
		if (m_oAcqControlsShow.m_bAcqOutput == true)
		{
			CBCGPGridItem* pItem = new CBCGPGridItem (ACQOUTPUT_STRING_NONE);
			pItem->AddOption (ACQOUTPUT_STRING_NONE, TRUE);
			pItem->AddOption (ACQOUTPUT_STRING_DUMP, TRUE);
			pItem->AddOption (ACQOUTPUT_STRING_XDUMP, TRUE);
			pRow->ReplaceItem (iColumnNum, pItem);
			iColumnNum++;
		}
	}
	m_wndAcqEditGrid.AddRow(pRow, TRUE);
	m_wndAcqEditGrid.AdjustLayout();
}

// 显示Process Type窗口
void CDlgProcessTypeSetup::OnShowProTypeWindow(void)
{
	m_wndProTypeEditGrid.RemoveAll();
	m_wndProTypeListGrid.RemoveAll();
	CRect rect;
	int iNbWidth, iLabelWidth;
	GetDlgItem(IDC_STATIC_PROTYPE_NB)->GetWindowRect (&rect);
	iNbWidth = rect.Width();
	GetDlgItem(IDC_STATIC_PROTYPE_LABEL)->GetWindowRect (&rect);
	iLabelWidth = rect.Width();
	m_wndProTypeEditGrid.InsertColumn (0, _T("Nb"), iNbWidth);
	m_wndProTypeEditGrid.InsertColumn (1, _T("Label"), iLabelWidth-1);
	m_wndProTypeListGrid.InsertColumn (0, _T("Nb"), iNbWidth);
	m_wndProTypeListGrid.InsertColumn (1, _T("Label"), iLabelWidth-1);
	for(int i=0;i<m_wndProTypeEditGrid.GetColumnCount();i++)
	{
		m_wndProTypeEditGrid.SetColumnLocked(i,TRUE);
		m_wndProTypeListGrid.SetColumnLocked(i,TRUE);
	}
	m_wndProTypeEditGrid.AddRow();
	m_wndProTypeEditGrid.AdjustLayout();
}

// 显示控件
void CDlgProcessTypeSetup::OnShowControls(void)
{
	OnShowProTypeControls();
	OnShowAcqControls();
	OnShowAuxControls();
}
// 依据左边界设置控件位置和尺寸
void CDlgProcessTypeSetup::OnSetControlsLocationByLeftPos(int ID, int iLeftPos)
{
	int iWidth = 0;
	CRect rect;
	GetDlgItem(ID)->GetWindowRect(&rect);
	iWidth = rect.Width();
	rect.left = iLeftPos;
	rect.right = rect.left + iWidth;
	ScreenToClient(&rect);
	GetDlgItem(ID)->MoveWindow(&rect);
}
// 依据右边界设置控件位置和尺寸
void CDlgProcessTypeSetup::OnSetControlsLocationByRightPos(int ID, int iRightPos)
{
	int iWidth = 0;
	CRect rect;
	GetDlgItem(ID)->GetWindowRect(&rect);
	iWidth = rect.Width();
	rect.right = iRightPos;
	rect.left = rect.right - iWidth;
	ScreenToClient(&rect);
	GetDlgItem(ID)->MoveWindow(&rect);
}
// 显示处理类型控件
void CDlgProcessTypeSetup::OnShowProTypeControls(void)
{
	CRect rect;
	int iLeftPos = 0;
	GetDlgItem(IDC_EDIT_RECORDLENGTH)->GetWindowRect(&rect);
	iLeftPos = rect.right + ControlsInterval;
	if (m_oProTypeControlsShow.m_bTbWindow == true)
	{
		GetDlgItem(IDC_STATIC_TBWINDOW)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_TBWINDOW)->ShowWindow(SW_SHOW);
		OnSetControlsLocationByLeftPos(IDC_STATIC_TBWINDOW, iLeftPos);
		OnSetControlsLocationByLeftPos(IDC_EDIT_TBWINDOW, iLeftPos);
		GetDlgItem(IDC_EDIT_TBWINDOW)->GetWindowRect(&rect);
		iLeftPos = rect.right + ControlsInterval;
	}
	else
	{
		GetDlgItem(IDC_STATIC_TBWINDOW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_TBWINDOW)->ShowWindow(SW_HIDE);
	}
	if (m_oProTypeControlsShow.m_bPeakTime == true)
	{
		GetDlgItem(IDC_STATIC_PEAKTIME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_PEAKTIME)->ShowWindow(SW_SHOW);
		OnSetControlsLocationByLeftPos(IDC_STATIC_PEAKTIME, iLeftPos);
		OnSetControlsLocationByLeftPos(IDC_EDIT_PEAKTIME, iLeftPos);
		GetDlgItem(IDC_EDIT_PEAKTIME)->GetWindowRect(&rect);
		iLeftPos = rect.right + ControlsInterval;
	}
	else
	{
		GetDlgItem(IDC_STATIC_PEAKTIME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PEAKTIME)->ShowWindow(SW_HIDE);
	}
	if (m_oProTypeControlsShow.m_bRaw == true)
	{
		GetDlgItem(IDC_STATIC_RAW)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_RAW)->ShowWindow(SW_SHOW);
		OnSetControlsLocationByLeftPos(IDC_STATIC_RAW, iLeftPos);
		OnSetControlsLocationByLeftPos(IDC_CHECK_RAW, iLeftPos);
		GetDlgItem(IDC_CHECK_RAW)->GetWindowRect(&rect);
		iLeftPos = rect.right + ControlsInterval;
	}
	else
	{
		GetDlgItem(IDC_STATIC_RAW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_RAW)->ShowWindow(SW_HIDE);
	}
}
// 显示Aux控件
void CDlgProcessTypeSetup::OnShowAuxControls(void)
{
	CRect rect;
	int iRightPos = 0;
	int iWidth = 0;
	int iBottom = 0;
	// 设置控件IDC_STATIC_AUXGROUP位置
	if (m_oAcqControlsShow.m_bAcqGroup == false)
	{
		GetDlgItem(IDC_STATIC_PROTYPEGROUP)->GetWindowRect(&rect);
		iRightPos = rect.right;
	}
	else
	{
		GetDlgItem(IDC_STATIC_ACQGROUP)->GetWindowRect(&rect);
		iRightPos = rect.left - ControlsInterval;
	}
	GetDlgItem(IDC_STATIC_AUXGROUP)->GetWindowRect(&rect);
	rect.right = iRightPos;
	ScreenToClient(&rect);
	GetDlgItem(IDC_STATIC_AUXGROUP)->MoveWindow(&rect);
	iRightPos -= ControlsInterval;

	OnSetControlsLocationByRightPos(IDC_BUTTON_AUX_ADD, iRightPos);
	OnSetControlsLocationByRightPos(IDC_BUTTON_AUX_CHANGE, iRightPos);
	OnSetControlsLocationByRightPos(IDC_BUTTON_AUX_DELETE, iRightPos);
	GetDlgItem(IDC_BUTTON_AUX_ADD)->GetWindowRect(&rect);
	iRightPos -= rect.Width();
	iRightPos -= ControlsInterval;

	GetDlgItem(IDC_STATIC_AUX)->GetWindowRect(&rect);
	rect.right = iRightPos;
	ScreenToClient(&rect);
	GetDlgItem(IDC_STATIC_AUX)->MoveWindow(&rect);
	iWidth = rect.Width();

	GetDlgItem(IDC_STATIC_AUXNB)->GetWindowRect(&rect);
	rect.right = rect.left + iWidth / 5;
	ScreenToClient(&rect);
	GetDlgItem(IDC_STATIC_AUXNB)->MoveWindow(&rect);

	GetDlgItem(IDC_STATIC_AUXPRO)->GetWindowRect(&rect);
	rect.right = iRightPos;
	rect.left = rect.right - (iWidth - iWidth / 5);
	ScreenToClient(&rect);
	GetDlgItem(IDC_STATIC_AUXPRO)->MoveWindow(&rect);

	m_wndAuxEditGrid.GetWindowRect(&rect);
	rect.right = iRightPos;
	ScreenToClient(&rect);
	m_wndAuxEditGrid.MoveWindow(&rect);

	m_wndAuxListGrid.GetWindowRect(&rect);
	rect.right = iRightPos;
	ScreenToClient(&rect);
	m_wndAuxListGrid.MoveWindow(&rect);

	GetDlgItem(IDC_EDIT_AUX_COR)->GetWindowRect(&rect);
	iBottom = rect.top - ControlsInterval;
	if (m_oAuxControlsShow.m_bAuxCor == true)
	{
		GetDlgItem(IDC_STATIC_AUX_COR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_AUX_COR)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_STATIC_AUX_COR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_AUX_COR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_AUX_COR)->GetWindowRect(&rect);
		iBottom = rect.bottom;
	}
	if (m_oAuxControlsShow.m_bAuxCheck == true)
	{
		GetDlgItem(IDC_CHECK_AUX)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_CHECK_AUX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_AUX)->GetWindowRect(&rect);
		iBottom = rect.bottom;
	}
	m_wndAuxListGrid.GetWindowRect(&rect);
	rect.bottom = iBottom;
	ScreenToClient(&rect);
	m_wndAuxListGrid.MoveWindow(&rect);
}
// 显示Acq控件
void CDlgProcessTypeSetup::OnShowAcqControls(void)
{
	CRect rect, rectTemp;
	int iRightPos = 0;
	GetDlgItem(IDC_STATIC_PROTYPEGROUP)->GetWindowRect(&rect);
	iRightPos = rect.right - ControlsInterval;
	if (m_oAcqControlsShow.m_bAcqEdit == true)
	{
		GetDlgItem(IDC_BUTTON_ACQ_ADD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_ACQ_CHANGE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_ACQ_DELETE)->ShowWindow(SW_SHOW);
		OnSetControlsLocationByRightPos(IDC_BUTTON_ACQ_ADD, iRightPos);
		OnSetControlsLocationByRightPos(IDC_BUTTON_ACQ_CHANGE, iRightPos);
		OnSetControlsLocationByRightPos(IDC_BUTTON_ACQ_DELETE, iRightPos);
		GetDlgItem(IDC_BUTTON_ACQ_ADD)->GetWindowRect(&rect);
		iRightPos = rect.left - ControlsInterval;
	}
	else
	{
		GetDlgItem(IDC_BUTTON_ACQ_ADD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ACQ_CHANGE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ACQ_DELETE)->ShowWindow(SW_HIDE);
	}
	if (m_oAcqControlsShow.m_bAcqOutput == true)
	{
		GetDlgItem(IDC_STATIC_ACQOUTPUT)->ShowWindow(SW_SHOW);
		OnSetControlsLocationByRightPos(IDC_STATIC_ACQOUTPUT, iRightPos);
		GetDlgItem(IDC_STATIC_ACQOUTPUT)->GetWindowRect(&rect);
		iRightPos = rect.left;
	}
	else
	{
		GetDlgItem(IDC_STATIC_ACQOUTPUT)->ShowWindow(SW_HIDE);
	}
	if (m_oAcqControlsShow.m_bAcqSignStack == true)
	{
		GetDlgItem(IDC_STATIC_ACQSIGNSTACK)->ShowWindow(SW_SHOW);
		OnSetControlsLocationByRightPos(IDC_STATIC_ACQSIGNSTACK, iRightPos);
		GetDlgItem(IDC_STATIC_ACQSIGNSTACK)->GetWindowRect(&rect);
		iRightPos = rect.left;
	}
	else
	{
		GetDlgItem(IDC_STATIC_ACQSIGNSTACK)->ShowWindow(SW_HIDE);
	}
	if (m_oAcqControlsShow.m_bAcqType == true)
	{
		GetDlgItem(IDC_STATIC_ACQTYPE)->ShowWindow(SW_SHOW);
		OnSetControlsLocationByRightPos(IDC_STATIC_ACQTYPE, iRightPos);
		GetDlgItem(IDC_STATIC_ACQTYPE)->GetWindowRect(&rect);
		iRightPos = rect.left;
	}
	else
	{
		GetDlgItem(IDC_STATIC_ACQTYPE)->ShowWindow(SW_HIDE);
	}
	if (m_oAcqControlsShow.m_bAcqNb == true)
	{
		GetDlgItem(IDC_STATIC_ACQNB)->ShowWindow(SW_SHOW);
		OnSetControlsLocationByRightPos(IDC_STATIC_ACQNB, iRightPos);
		GetDlgItem(IDC_STATIC_ACQNB)->GetWindowRect(&rect);
		iRightPos = rect.left - ControlsInterval;
	}
	else
	{
		GetDlgItem(IDC_STATIC_ACQNB)->ShowWindow(SW_HIDE);
	}
	if (m_oAcqControlsShow.m_bAcq == true)
	{
		GetDlgItem(IDC_STATIC_ACQ)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_ACQ)->GetWindowRect(&rect);
		GetDlgItem(IDC_STATIC_ACQOUTPUT)->GetWindowRect(&rectTemp);
		rect.right = rectTemp.right;
		GetDlgItem(IDC_STATIC_ACQNB)->GetWindowRect(&rectTemp);
		rect.left = rectTemp.left;
		ScreenToClient(&rect);
		GetDlgItem(IDC_STATIC_ACQ)->MoveWindow(&rect);
	}
	else
	{
		GetDlgItem(IDC_STATIC_ACQ)->ShowWindow(SW_HIDE);
	}
	GetDlgItem(IDC_STATIC_ACQ_GRID_EDIT)->ShowWindow(SW_HIDE);
	if (m_oAcqControlsShow.m_bAcqGridEdit == true) 
	{
		m_wndAcqEditGrid.ShowWindow(SW_SHOW);
		m_wndAcqEditGrid.GetWindowRect(&rect);
		GetDlgItem(IDC_STATIC_ACQ)->GetWindowRect(&rectTemp);
		rect.right = rectTemp.right;
		rect.left = rectTemp.left - 20;
		ScreenToClient(&rect);
		m_wndAcqEditGrid.MoveWindow(&rect);
		iRightPos -= 20;
	}
	else
	{
		m_wndAcqEditGrid.ShowWindow(SW_HIDE);
	}
	GetDlgItem(IDC_STATIC_ACQ_GRID)->ShowWindow(SW_HIDE);
	if (m_oAcqControlsShow.m_bAcqGridList == true)
	{
		m_wndAcqListGrid.ShowWindow(SW_SHOW);
		m_wndAcqListGrid.GetWindowRect(&rect);
		m_wndAcqEditGrid.GetWindowRect(&rectTemp);
		rect.right = rectTemp.right;
		rect.left = rectTemp.left;
		ScreenToClient(&rect);
		m_wndAcqListGrid.MoveWindow(&rect);
	}
	else
	{
		m_wndAcqListGrid.ShowWindow(SW_HIDE);
	}
	if (m_oAcqControlsShow.m_bAcqGroup == true)
	{
		GetDlgItem(IDC_STATIC_ACQGROUP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_ACQGROUP)->GetWindowRect(&rect);
		GetDlgItem(IDC_STATIC_PROTYPEGROUP)->GetWindowRect(&rectTemp);
		rect.left = iRightPos;
		rect.right = rectTemp.right;
		ScreenToClient(&rect);
		GetDlgItem(IDC_STATIC_ACQGROUP)->MoveWindow(&rect);
	}
	else
	{
		GetDlgItem(IDC_STATIC_ACQGROUP)->ShowWindow(SW_HIDE);
	}
}