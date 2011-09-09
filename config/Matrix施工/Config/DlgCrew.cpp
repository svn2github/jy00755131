// DlgCrew.cpp : 实现文件
//

#include "stdafx.h"
#include "Config.h"
#include "DlgCrew.h"


// CDlgCrew 对话框

IMPLEMENT_DYNAMIC(CDlgCrew, CBCGPDialog)

CDlgCrew::CDlgCrew(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgCrew::IDD, pParent)
	, m_strCrewName(_T(""))
	, m_nSampleRate(0)
	, m_nGain3301(0)
	, m_nFilterFIR(0)
	, m_nFilterIIR(0)
	, m_nMux3301(0)
	, m_nMode5373(0)
	, m_nAttr5373(0)
{
	EnableVisualManagerStyle();
}

CDlgCrew::~CDlgCrew()
{
}

void CDlgCrew::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITCREWNAME, m_strCrewName);
	DDX_CBIndex(pDX, IDC_COMBOSAMPLERATE, m_nSampleRate);
	DDX_CBIndex(pDX, IDC_COMBOGAIN, m_nGain3301);
	DDX_CBIndex(pDX, IDC_COMBOFILTERFIR, m_nFilterFIR);
	DDX_CBIndex(pDX, IDC_COMBOFILTERIIR, m_nFilterIIR);
	DDX_CBIndex(pDX, IDC_COMBOANALOGGATECHANNEL, m_nMux3301);
	DDX_CBIndex(pDX, IDC_COMBO5373MODE, m_nMode5373);
	DDX_CBIndex(pDX, IDC_COMBODAWEAKEN, m_nAttr5373);
}


BEGIN_MESSAGE_MAP(CDlgCrew, CBCGPDialog)
	ON_BN_CLICKED(IDC_BUTTONAPPLY, &CDlgCrew::OnBnClickedButtonapply)
	ON_BN_CLICKED(IDC_BUTTONRESET, &CDlgCrew::OnBnClickedButtonreset)
END_MESSAGE_MAP()


// CDlgCrew 消息处理程序

void CDlgCrew::SetParams(CFDUConfig* pFDUConfig)
{
	if(!pFDUConfig)
		return;
	m_strCrewName=pFDUConfig->m_strCrewName;
	m_nSampleRate=pFDUConfig->m_dwSampleRateIndex;
	m_nGain3301 = pFDUConfig->m_dwGain3301Index;
	m_nFilterFIR=pFDUConfig->m_dwFilterFIRIndex;
	m_nFilterIIR=pFDUConfig->m_dwFilterIIRIndex;
	m_nMux3301 = pFDUConfig->m_dwMux3301Index;
	m_nMode5373 = pFDUConfig->m_dwMode5373Index;
	m_nAttr5373 = pFDUConfig->m_dwAttr5373Index;
}
void CDlgCrew::ApplyParams(CFDUConfig* pFDUConfig)
{
	if(!pFDUConfig)
		return;
	pFDUConfig->m_strCrewName=m_strCrewName;
	pFDUConfig->m_dwSampleRateIndex=m_nSampleRate;
	pFDUConfig->m_dwGain3301Index =m_nGain3301;
	pFDUConfig->m_dwFilterFIRIndex=m_nFilterFIR;
	pFDUConfig->m_dwFilterIIRIndex=m_nFilterIIR;
	pFDUConfig->m_dwMux3301Index = m_nMux3301;
	pFDUConfig->m_dwMode5373Index = m_nMode5373;
	pFDUConfig->m_dwAttr5373Index = m_nAttr5373;
}
BOOL CDlgCrew::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	OnBnClickedButtonreset();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgCrew::OnBnClickedButtonapply()
{
	CConfigParseXML  ParseXML;
	CFDUConfig       FDUConfig;
	UpdateData(TRUE);
	ApplyParams(&FDUConfig);
	ParseXML.WriteCrewConfig(theApp.m_strLocalXMLFile,&FDUConfig);	
	theApp.WriteLog(_T("CDlgCrew::OnBnClickedButtonapply"),_T("Crew setup has been modified!"),LOGTYPE_NORMAL,TRUE);
	theApp.SaveXMLToFTPServer();
}

void CDlgCrew::OnBnClickedButtonreset()
{
	CConfigParseXML  ParseXML;
	CFDUConfig       FDUConfig;
	ParseXML.ParseCrewConfig(theApp.m_strLocalXMLFile,&FDUConfig);
	SetParams(&FDUConfig);
	UpdateData(FALSE);
}
