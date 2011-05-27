// TabADCSettings.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixTestSoft.h"
#include "TabADCSettings.h"


// CTabADCSettings 对话框

IMPLEMENT_DYNAMIC(CTabADCSettings, CDialog)

CTabADCSettings::CTabADCSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CTabADCSettings::IDD, pParent)
{

}

CTabADCSettings::~CTabADCSettings()
{
}

void CTabADCSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SYNC, m_comboSync);
	DDX_Control(pDX, IDC_COMBO_MODE, m_comboMode);
	DDX_Control(pDX, IDC_COMBO_SPS, m_comboSps);
	DDX_Control(pDX, IDC_COMBO_PHS, m_comboPhs);
	DDX_Control(pDX, IDC_COMBO_FILTER, m_comboFilter);
	DDX_Control(pDX, IDC_COMBO_MUX, m_comboMux);
	DDX_Control(pDX, IDC_COMBO_CHOP, m_comboChop);
	DDX_Control(pDX, IDC_COMBO_PGA, m_comboPga);
	DDX_Control(pDX, IDC_EDIT_HPFLOW, m_editHpfLow);
	DDX_Control(pDX, IDC_EDIT_HPFHIGH, m_editHpfHigh);
}


BEGIN_MESSAGE_MAP(CTabADCSettings, CDialog)
END_MESSAGE_MAP()


// CTabADCSettings 消息处理程序
BOOL CTabADCSettings::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置同步
	m_comboSync.InsertString(0, _T("脉冲同步模式"));
	m_comboSync.InsertString(1, _T("连续同步模式"));
	m_comboSync.SetCurSel(0);

	// 设置模式
	m_comboMode.InsertString(0, _T("低功耗模式"));
	m_comboMode.InsertString(1, _T("高精度模式"));
	m_comboMode.SetCurSel(1);

	// 设置采样率
	m_comboSps.InsertString(0, _T("4000"));
	m_comboSps.InsertString(1, _T("2000"));
	m_comboSps.InsertString(2, _T("1000"));
	m_comboSps.InsertString(3, _T("500"));
	m_comboSps.InsertString(4, _T("250"));
	m_comboSps.SetCurSel(2);
	
	// 设置FIR相位
	m_comboPhs.InsertString(0, _T("线性相位"));
	m_comboPhs.InsertString(1, _T("最小相位"));
	m_comboPhs.SetCurSel(0);

	// 设置数字滤波器
	m_comboFilter.InsertString(0, _T("调制器输出模式，可忽略片上滤波器"));
	m_comboFilter.InsertString(1, _T("Sinc滤波器"));
	m_comboFilter.InsertString(2, _T("Sinc+LPF滤波器"));
	m_comboFilter.InsertString(3, _T("Sinc+LPF+HPF滤波器"));
	m_comboFilter.SetCurSel(2);

	// 设置多路器
	m_comboMux.InsertString(0, _T("AINP1+AINN1"));
	m_comboMux.InsertString(1, _T("Internal short via 400Ω"));
	m_comboMux.InsertString(2, _T("AINP1 + AINP2连接AINP2 + AINN2"));
	m_comboMux.InsertString(3, _T("External short to AINN2"));
	m_comboMux.SetCurSel(0);

	// 设置CHOP
	m_comboChop.InsertString(0, _T("PGA chopping disabled"));
	m_comboChop.InsertString(1, _T("PGA chopping enabled"));
	m_comboChop.SetCurSel(1);

	// 设置PGA增益
	m_comboPga.InsertString(0, _T("G = 1"));
	m_comboPga.InsertString(1, _T("G = 2"));
	m_comboPga.InsertString(2, _T("G = 4"));
	m_comboPga.InsertString(3, _T("G = 8"));
	m_comboPga.InsertString(4, _T("G = 16"));
	m_comboPga.InsertString(5, _T("G = 32"));
	m_comboPga.InsertString(60, _T("G = 64"));
	m_comboPga.SetCurSel(0);

	m_editHpfLow.SetWindowText(_T("50"));
	m_editHpfHigh.SetWindowText(_T("3"));
	return TRUE;  // return TRUE  unless you set the focus to a control
}