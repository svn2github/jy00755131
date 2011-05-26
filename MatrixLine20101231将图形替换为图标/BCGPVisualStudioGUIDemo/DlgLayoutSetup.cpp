// DlgMarkerSetup.cpp : 实现文件
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"
#include "DlgLayoutSetup.h"


// CDlgLayoutSetup 对话框

IMPLEMENT_DYNAMIC(CDlgLayoutSetup, CDialog)

CDlgLayoutSetup::CDlgLayoutSetup(CWnd* pParent /*=NULL*/)
: CDialog(CDlgLayoutSetup::IDD, pParent)
{

}

CDlgLayoutSetup::~CDlgLayoutSetup()
{
}

void CDlgLayoutSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLayoutSetup, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDlgLayoutSetup::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CDlgLayoutSetup::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgLayoutSetup::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &CDlgLayoutSetup::OnBnClickedButtonChange)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDlgLayoutSetup::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CDlgLayoutSetup::OnBnClickedButtonClear)
END_MESSAGE_MAP()

// CDlgLayoutSetup 消息处理程序

/**
* OnInitDialog()
* @param void
* @return BOOL
*/
BOOL CDlgLayoutSetup::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 得到Tab区域
	CRect oRect;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_TAB);
	pWnd->GetWindowRect(&oRect);
	ScreenToClient(&oRect);

	// 生成Tab页容器
	m_oTabWnd.Create(CBCGPTabWnd::STYLE_3D, oRect, this, 1, CBCGPTabWnd::LOCATION_TOP);

	// 生成属性页标记点
	m_oTabMarker.m_pSiteData = m_pSiteData;
	m_oTabMarker.Create("", WS_CHILD | WS_VISIBLE, oRect, &m_oTabWnd, 1);
	// 添加属性页标记点
	m_oTabWnd.AddTab(&m_oTabMarker, "    Marker    ");

	// 生成属性页辅助道
	m_oTabAux.m_pSiteData = m_pSiteData;
	m_oTabAux.Create("", WS_CHILD | WS_VISIBLE, oRect, &m_oTabWnd, 2);
	// 添加属性页标记点
	m_oTabWnd.AddTab(&m_oTabAux, "    Aux    ");

	// 生成属性页迂回道
	m_oTabDetour.m_pSiteData = m_pSiteData;
	m_oTabDetour.Create("", WS_CHILD | WS_VISIBLE, oRect, &m_oTabWnd, 3);
	// 添加属性页迂回道
	m_oTabWnd.AddTab(&m_oTabDetour, "    Detour    ");

	// 生成属性页哑点
	m_oTabMute.m_pSiteData = m_pSiteData;
	m_oTabMute.Create("", WS_CHILD | WS_VISIBLE, oRect, &m_oTabWnd, 4);
	// 添加属性页哑点
	m_oTabWnd.AddTab(&m_oTabMute, "    Mute    ");

	// 生成属性页爆炸机控制器
	m_oTabBlastMachine.m_pSiteData = m_pSiteData;
	m_oTabBlastMachine.Create("", WS_CHILD | WS_VISIBLE, oRect, &m_oTabWnd, 5);
	// 添加属性页爆炸机控制器
	m_oTabWnd.AddTab(&m_oTabBlastMachine, "    BlastMachine    ");

	// 显示
	m_oTabWnd.RecalcLayout();
	m_oTabWnd.SetActiveTab(0);

	// 加载标记点设置数据
	m_pSiteData->OnLoadLayoutSetupDataOfMarker();
	// 加载辅助道设置数据
	m_pSiteData->OnLoadLayoutSetupDataOfAux();
	// 加载迂回道设置数据
	m_pSiteData->OnLoadLayoutSetupDataOfDetour();
	// 加载哑点设置数据
	m_pSiteData->OnLoadLayoutSetupDataOfMute();
	// 加载爆炸机控制器设置数据
	m_pSiteData->OnLoadLayoutSetupDataOfBlastMachine();

	m_oTabMarker.OnBnClickedButtonReset();
	m_oTabAux.OnBnClickedButtonReset();
	m_oTabDetour.OnBnClickedButtonReset();
	m_oTabMute.OnBnClickedButtonReset();
	m_oTabBlastMachine.OnBnClickedButtonReset();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

/**
* OnClose()
* @param void
* @return void
*/
void CDlgLayoutSetup::OnClose()
{
	ShowWindow(SW_HIDE);
}

/**
* 按钮点击相应；Apply
* @param void
* @return void
*/
void CDlgLayoutSetup::OnBnClickedButtonApply()
{
	int iActiveTab = m_oTabWnd.GetActiveTab();
	switch(iActiveTab)
	{
	case 0:
		try
		{
			m_oTabMarker.OnBnClickedButtonApply();
			// Apply标记点设置数据
			m_pSiteData->OnApplyLayoutSetupDataOfMarker();
			// 发送标记点设置数据变化通知命令帧
			m_pSiteData->OnSendCMDFrameForApplyLayoutSetupDataOfMarker();
		}
		catch (CException* e)
		{
			AfxMessageBox("Illegal Data.\r\nPlease Check Data.", MB_OK | MB_ICONSTOP);
			return;
		}
		break;
	case 1:
		try
		{
			m_oTabAux.OnBnClickedButtonApply();
			// Apply辅助道设置数据
			m_pSiteData->OnApplyLayoutSetupDataOfAux();
			// 发送辅助道设置数据变化通知命令帧
			m_pSiteData->OnSendCMDFrameForApplyLayoutSetupDataOfAux();
		}
		catch (CException* e)
		{
			AfxMessageBox("Illegal Data.\r\nPlease Check Data.", MB_OK | MB_ICONSTOP);
			return;
		}
		break;
	case 2:
		try
		{
			m_oTabDetour.OnBnClickedButtonApply();
			// Apply迂回道设置数据
			m_pSiteData->OnApplyLayoutSetupDataOfDetour();
			// 发送迂回道设置数据变化通知命令帧
			m_pSiteData->OnSendCMDFrameForApplyLayoutSetupDataOfDetour();
		}
		catch (CException* e)
		{
			AfxMessageBox("Illegal Data.\r\nPlease Check Data.", MB_OK | MB_ICONSTOP);
			return;
		}
		break;
	case 3:
		try
		{
			m_oTabMute.OnBnClickedButtonApply();
			// Apply哑点设置数据
			m_pSiteData->OnApplyLayoutSetupDataOfMute();
			// 发送哑点设置数据变化通知命令帧
			m_pSiteData->OnSendCMDFrameForApplyLayoutSetupDataOfMute();
		}
		catch (CException* e)
		{
			AfxMessageBox("Illegal Data.\r\nPlease Check Data.", MB_OK | MB_ICONSTOP);
			return;
		}
		break;
	case 4:
		try
		{
			m_oTabBlastMachine.OnBnClickedButtonApply();
			// Apply爆炸机控制器设置数据
			m_pSiteData->OnApplyLayoutSetupDataOfBlastMachine();
			// 发送爆炸机控制器设置数据变化通知命令帧
			m_pSiteData->OnSendCMDFrameForApplyLayoutSetupDataOfBlastMachine();
		}
		catch (CException* e)
		{
			AfxMessageBox("Illegal Data.\r\nPlease Check Data.", MB_OK | MB_ICONSTOP);
			return;
		}
		break;
	}
}

/**
* 按钮点击相应；Reset
* @param void
* @return void
*/
void CDlgLayoutSetup::OnBnClickedButtonReset()
{
	int iActiveTab = m_oTabWnd.GetActiveTab();
	switch(iActiveTab)
	{
	case 0:
		// 加载标记点设置数据
		m_pSiteData->OnLoadLayoutSetupDataOfMarker();
		m_oTabMarker.OnBnClickedButtonReset();
		break;
	case 1:
		// 加载辅助道设置数据
		m_pSiteData->OnLoadLayoutSetupDataOfAux();
		m_oTabAux.OnBnClickedButtonReset();
		break;
	case 2:
		// 加载迂回道设置数据
		m_pSiteData->OnLoadLayoutSetupDataOfDetour();
		m_oTabDetour.OnBnClickedButtonReset();
		break;
	case 3:
		// 加载哑点设置数据
		m_pSiteData->OnLoadLayoutSetupDataOfMute();
		m_oTabMute.OnBnClickedButtonReset();
		break;
	case 4:
		// 加载爆炸机控制器设置数据
		m_pSiteData->OnLoadLayoutSetupDataOfBlastMachine();
		m_oTabBlastMachine.OnBnClickedButtonReset();
		break;
	}
}

/**
* 按钮点击相应；Add
* @param void
* @return void
*/
void CDlgLayoutSetup::OnBnClickedButtonAdd()
{
	int iActiveTab = m_oTabWnd.GetActiveTab();
	switch(iActiveTab)
	{
	case 0:
		m_oTabMarker.OnBnClickedButtonAdd();
		break;
	case 1:
		m_oTabAux.OnBnClickedButtonAdd();
		break;
	case 2:
		m_oTabDetour.OnBnClickedButtonAdd();
		break;
	case 3:
		m_oTabMute.OnBnClickedButtonAdd();
		break;
	case 4:
		m_oTabBlastMachine.OnBnClickedButtonAdd();
		break;
	}
}

/**
* 按钮点击相应；Change
* @param void
* @return void
*/
void CDlgLayoutSetup::OnBnClickedButtonChange()
{
	int iActiveTab = m_oTabWnd.GetActiveTab();
	switch(iActiveTab)
	{
	case 0:
		m_oTabMarker.OnBnClickedButtonChange();
		break;
	case 1:
		m_oTabAux.OnBnClickedButtonChange();
		break;
	case 2:
		m_oTabDetour.OnBnClickedButtonChange();
		break;
	case 3:
		m_oTabMute.OnBnClickedButtonChange();
		break;
	case 4:
		m_oTabBlastMachine.OnBnClickedButtonChange();
		break;
	}
}

/**
* 按钮点击相应；Delete
* @param void
* @return void
*/
void CDlgLayoutSetup::OnBnClickedButtonDelete()
{
	int iActiveTab = m_oTabWnd.GetActiveTab();
	switch(iActiveTab)
	{
	case 0:
		m_oTabMarker.OnBnClickedButtonDelete();
		break;
	case 1:
		m_oTabAux.OnBnClickedButtonDelete();
		break;
	case 2:
		m_oTabDetour.OnBnClickedButtonDelete();
		break;
	case 3:
		m_oTabMute.OnBnClickedButtonDelete();
		break;
	case 4:
		m_oTabBlastMachine.OnBnClickedButtonDelete();
		break;
	}
}

/**
* 按钮点击相应；Clear
* @param void
* @return void
*/
void CDlgLayoutSetup::OnBnClickedButtonClear()
{
	int iActiveTab = m_oTabWnd.GetActiveTab();
	switch(iActiveTab)
	{
	case 0:
		m_oTabMarker.OnBnClickedButtonClear();
		break;
	case 1:
		m_oTabAux.OnBnClickedButtonClear();
		break;
	case 2:
		m_oTabDetour.OnBnClickedButtonClear();
		break;
	case 3:
		m_oTabMute.OnBnClickedButtonClear();
		break;
	case 4:
		m_oTabBlastMachine.OnBnClickedButtonClear();
		break;
	}
}

/**
* 响应仪器图形视图弹出菜单设置标记点
* @param unsigned int uiInstrumentType 仪器类型 1-交叉站；2-电源站；3-采集站
* @param unsigned int uiSN 仪器设备号
* @return void
*/
void CDlgLayoutSetup::OnGraphViewInstrumentSetMarker(unsigned int uiInstrumentType, unsigned int uiSN)
{
	m_oTabMarker.OnGraphViewInstrumentSetMarker(uiInstrumentType, uiSN);
}

/**
* 响应仪器图形视图弹出菜单设置辅助道
* @param unsigned int uiInstrumentType 仪器类型 1-交叉站；2-电源站；3-采集站
* @param unsigned int uiSN 仪器设备号
* @return void
*/
void CDlgLayoutSetup::OnGraphViewInstrumentSetAuxiliary(unsigned int uiInstrumentType, unsigned int uiSN)
{
	m_oTabAux.OnGraphViewInstrumentSetAuxiliary(uiInstrumentType, uiSN);
}

/**
* 响应仪器图形视图弹出菜单设置迂回道
* @param unsigned int uiInstrumentType 低端仪器类型 1-交叉站；2-电源站；3-采集站
* @param unsigned int uiSN 低端仪器设备号
* @param unsigned int uiInstrumentType 高端仪器类型 1-交叉站；2-电源站；3-采集站
* @param unsigned int uiSN 高端仪器设备号
* @return void
*/
void CDlgLayoutSetup::OnGraphViewInstrumentSetDetour(unsigned int uiInstrumentTypeLow, unsigned int uiSNLow, unsigned int uiInstrumentTypeHigh, unsigned int uiSNHight)
{
	m_oTabDetour.OnGraphViewInstrumentSetDetour(uiInstrumentTypeLow, uiSNLow, uiInstrumentTypeHigh, uiSNHight);
}

/**
* 响应检波器图形视图弹出菜单Mute设置
* @param unsigned int uiNbLine 测线号
* @param unsigned int uiNbPoint	测点号
* @return void
*/
void CDlgLayoutSetup::OnGraphViewSensorSetMute(unsigned int uiNbLine, unsigned int uiNbPoint)
{
	m_oTabMute.OnGraphViewSensorSetMute(uiNbLine, uiNbPoint);
}