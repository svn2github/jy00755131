// ParameterSet.cpp : 实现文件
//

#include "stdafx.h"
#include "绘图程序.h"
#include "ParameterSet.h"


// CParameterSet 对话框

IMPLEMENT_DYNAMIC(CParameterSet, CDialog)

CParameterSet::CParameterSet(CWnd* pParent /*=NULL*/)
	: CDialog(CParameterSet::IDD, pParent)
	, m_csIPAddrAim(_T("192.168.0.19"))
	, m_csSendPort(_T("0x9401"))
	, m_csRcvPort(_T("0x7800"))
	, m_csShowGraphType(_T("纵向显示"))
	, m_csXMLPath(_T("MatrixTestSoft.xml"))
	, m_uiShowDirection(1)
	, m_iMarkHighColor(Chart::Transparent)
	, m_iMarkLowColor(0)
	, m_iComboMarkHigh(0)
	, m_iComboMarkLow(1)
{
}

CParameterSet::~CParameterSet()
{
}

void CParameterSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SENDPORT, m_csSendPort);
	DDX_Text(pDX, IDC_EDIT_RECVPORT, m_csRcvPort);
	DDX_CBString(pDX, IDC_COMBO_SHOWGRAPHTYPE, m_csShowGraphType);
	DDX_CBIndex(pDX, IDC_COMBO_MARKHIGH, m_iComboMarkHigh);
	DDX_CBIndex(pDX, IDC_COMBO_MARKLOW, m_iComboMarkLow);
}


BEGIN_MESSAGE_MAP(CParameterSet, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CParameterSet::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_SELECTXMLPATH, &CParameterSet::OnBnClickedButtonSelectxmlpath)
	ON_CBN_SELCHANGE(IDC_COMBO_MARKHIGH, &CParameterSet::OnCbnSelchangeComboMarkhigh)
	ON_CBN_SELCHANGE(IDC_COMBO_MARKLOW, &CParameterSet::OnCbnSelchangeComboMarklow)
END_MESSAGE_MAP()


// CParameterSet 消息处理程序
BOOL CParameterSet::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_IPADDRESS_AIM)->SetWindowText(m_csIPAddrAim);
	GetDlgItem(IDC_EDIT_SENDPORT)->SetWindowText(m_csSendPort);
	GetDlgItem(IDC_EDIT_RECVPORT)->SetWindowText(m_csRcvPort);
	GetDlgItem(IDC_COMBO_SHOWGRAPHTYPE)->SetWindowText(m_csShowGraphType);
	return TRUE;
}

void CParameterSet::OnBnClickedButtonApply()
{
	// TODO: 在此添加控件通知处理程序代码
	SaveXmlFile();
	if (m_csShowGraphType == _T("纵向显示"))
	{
		m_uiShowDirection = 1;
	}
	else if (m_csShowGraphType == _T("横向显示"))
	{
		m_uiShowDirection = 2;
	}
	OnOK();
}

void CParameterSet::OnBnClickedButtonSelectxmlpath()
{
	// TODO: 在此添加控件通知处理程序代码
	OnSelectXMLPath();
	LoadXmlFile();
	GetDlgItem(IDC_IPADDRESS_AIM)->SetWindowText(m_csIPAddrAim);
	GetDlgItem(IDC_EDIT_SENDPORT)->SetWindowText(m_csSendPort);
	GetDlgItem(IDC_EDIT_RECVPORT)->SetWindowText(m_csRcvPort);
	GetDlgItem(IDC_COMBO_SHOWGRAPHTYPE)->SetWindowText(m_csShowGraphType);
}

// 载入配置文件
void CParameterSet::LoadXmlFile(void)
{
	if (FALSE == OpenXmlFile())
	{
		// @@@@提交用户时修改
//		AfxMessageBox(_T("请选择按钮参数设置->点击选择配置文件路径按钮重新选择路径！"));
		CloseXmlFile();
		return;
	}
	if (false == LoadGraphShowSetup())
	{
		CloseXmlFile();
		return;
	}
	CloseXmlFile();
}
// 保存配置文件
void CParameterSet::SaveXmlFile(void)
{
	if (FALSE == OpenXmlFile())
	{
		// @@@@提交用户时修改
//		AfxMessageBox(_T("请选择按钮参数设置->点击选择配置文件路径按钮重新选择路径！"));
		CloseXmlFile();
		return;
	}
	GetDlgItem(IDC_IPADDRESS_AIM)->GetWindowText(m_csIPAddrAim);
	GetDlgItem(IDC_EDIT_SENDPORT)->GetWindowText(m_csSendPort);
	GetDlgItem(IDC_EDIT_RECVPORT)->GetWindowText(m_csRcvPort);
	GetDlgItem(IDC_COMBO_SHOWGRAPHTYPE)->GetWindowText(m_csShowGraphType);
	if (false == SaveGraphShowSetup())
	{
		CloseXmlFile();
		return;
	}
	COleVariant oVariant;
	oVariant = m_csXMLPath;
	m_oXMLDOMDocument.save(oVariant);
	CloseXmlFile();
}
// 打开XML配置文件
BOOL CParameterSet::OpenXmlFile(void)
{
	BOOL bReturn = FALSE;
		// 初始化COM库
	CoInitialize(NULL);

	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;

	strOLEObject = "msxml2.domdocument";
	bReturn = m_oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);
	if (bReturn == FALSE)
	{
		return bReturn;
	}
	oVariant = m_csXMLPath;
	bReturn = m_oXMLDOMDocument.load(oVariant);

	return bReturn;
}

// 选择XML文件路径
void CParameterSet::OnSelectXMLPath(void)
{
	// 打开文件
	const wchar_t pszFilter[] = _T("配置文件(*.xml)|*.xml|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T(".xml"), _T("MatrixTestSoft.xml"), OFN_HIDEREADONLY| OFN_OVERWRITEPROMPT, pszFilter, this);

	if ( dlg.DoModal()!=IDOK )
	{
		return;
	}
	m_csXMLPath = dlg.GetPathName();
	if (FALSE == OpenXmlFile())
	{
		// @@@@提交用户时修改
//		AfxMessageBox(_T("请选择按钮参数设置->点击选择配置文件路径按钮重新选择路径！"));
		CloseXmlFile();
		return;
	}
	if (false == SaveXmlFilePath())
	{
		CloseXmlFile();
		return;
	}
	COleVariant oVariant;
	oVariant = m_csXMLPath;
	m_oXMLDOMDocument.save(oVariant);
	CloseXmlFile();
}
// 关闭程序配置文件
void CParameterSet::CloseXmlFile(void)
{
	m_oXMLDOMDocument.DetachDispatch();
	// 释放COM库
	CoUninitialize();
}
// 载入图形显示设置
bool CParameterSet::LoadGraphShowSetup(void)
{
	CString strKey = _T("");
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;	
	CString cstemp = _T("");
	try
	{
		// 找到GraphShowSetup
		strKey =_T("GraphShowSetup");
		lpDispatch = m_oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);

		// 得到目标IP地址
		strKey =_T("IPAddrAim");
		cstemp = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		m_csIPAddrAim = cstemp;

		// 得到发送端口
		strKey =_T("SendPort");
		cstemp = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		m_csSendPort = cstemp;

		// 得到接收端口
		strKey =_T("RcvPort");
		cstemp = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		m_csRcvPort = cstemp;

		// 得到图形显示方式
		strKey =_T("GraphShowType");
		cstemp = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		m_csShowGraphType = cstemp;
		if (m_csShowGraphType == _T("纵向显示"))
		{
			m_uiShowDirection = 1;
		}
		else if (m_csShowGraphType == _T("横向显示"))
		{
			m_uiShowDirection = 2;
		}

		// 得到XML文件路径
		strKey =_T("XMLFilePath");
		cstemp = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		m_csXMLPath = cstemp;

		return true;
	}
	catch (CException* )
	{
		return false;
	}
}

// 保存图形显示设置
bool CParameterSet::SaveGraphShowSetup(void)
{
	CString strKey = _T("");
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	COleVariant oVariant;

	try
	{
		// 找到GraphShowSetup设置区
		strKey = "GraphShowSetup";
		lpDispatch = m_oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);

		// 得到目标IP地址
		strKey =_T("IPAddrAim");
		oVariant = m_csIPAddrAim;
		oElement.setAttribute(strKey, oVariant);

		// 得到发送端口
		strKey =_T("SendPort");
		oVariant = m_csSendPort;
		oElement.setAttribute(strKey, oVariant);

		// 得到接收端口
		strKey =_T("RcvPort");
		oVariant = m_csRcvPort;
		oElement.setAttribute(strKey, oVariant);

		// 得到图形显示方式
		strKey =_T("GraphShowType");
		oVariant = m_csShowGraphType;
		oElement.setAttribute(strKey, oVariant);

		return true;
	}
	catch (CException* )
	{
		return false;
	}
}
// 保存XML文件路径
bool CParameterSet::SaveXmlFilePath(void)
{
	CString strKey = _T("");
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	COleVariant oVariant;

	try
	{
		// 找到GraphShowSetup设置区
		strKey = "GraphShowSetup";
		lpDispatch = m_oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);

		// 得到XML文件路径
		strKey =_T("XMLFilePath");
		oVariant = m_csXMLPath;
		oElement.setAttribute(strKey, oVariant);

		return true;
	}
	catch (CException* )
	{
		return false;
	}
}

void CParameterSet::OnCbnSelchangeComboMarkhigh()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_iComboMarkHigh == 0)
	{
		m_iMarkHighColor = Chart::Transparent;
	}
	else if (m_iComboMarkHigh == 1)
	{
		m_iMarkHighColor = OnSelectColor();
	}
}

void CParameterSet::OnCbnSelchangeComboMarklow()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_iComboMarkLow == 0)
	{
		m_iMarkLowColor = Chart::Transparent;
	}
	else if (m_iComboMarkLow == 1)
	{
		m_iMarkLowColor = OnSelectColor();
	}
}

// 选择颜色
int CParameterSet::OnSelectColor(void)
{
	CColorDialog colorDialog;
	int reasult = colorDialog.DoModal();
	int iColor = 0;
	int rr = 0;
	int gg = 0;
	int bb = 0;
	//用这两行代码可以得到那个框框。这时候就可以在上面选择自己
	//要用的颜色了。
	//如果按下颜色对话框的OK键
	if (reasult == IDOK)
	{
		iColor = colorDialog.GetColor();//把对话框选中的颜色给变量iColor 0x00bbggrr

		Invalidate();//销毁当前页面，刷新到颜色修改后的页面
	}
	// 将0x00bbggrr转换为0x00rrggbb
	rr = iColor & 0xff;
	gg = (iColor >> 8) & 0xff;
	bb = (iColor >> 16) & 0xff;
	iColor = (rr << 16) + (gg << 8) + bb;
	return iColor;
}
