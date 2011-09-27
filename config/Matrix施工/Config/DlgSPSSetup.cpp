// DlgSPSSetup.cpp : 实现文件
//

#include "stdafx.h"
#include "Config.h"
#include "DlgSPSSetup.h"
#include "..\common\ConfigParseXML.h"

// DlgSPSSetup 对话框

IMPLEMENT_DYNAMIC(CDlgSPSSetup, CBCGPDialog)

CDlgSPSSetup::CDlgSPSSetup(CWnd* pParent /*=NULL*/)
: CBCGPDialog(CDlgSPSSetup::IDD, pParent)
, m_csSPSFilePath(_T(""))
{
	EnableVisualManagerStyle();
	m_csLocalOptXMLFile = CLIENTDIR_XMLFILE;
	m_csLocalOptXMLFile += OPERATION_XMLFILE;
	m_csLocalLineXMLFile = CLIENTDIR_XMLFILE;
	m_csLocalLineXMLFile += MATRIX_XMLFILE;
	m_csLocalLineInitTXTFile = CLIENTDIR_XMLFILE;
	m_csLocalLineInitTXTFile += LINEINIT_TXTFILE;
	GetCurrentDirectory(MAX_PATH, m_wcExeCurrentPath);
}

CDlgSPSSetup::~CDlgSPSSetup()
{
}

void CDlgSPSSetup::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSPSSetup, CBCGPDialog)
	ON_BN_CLICKED(IDC_BTNLOADSPS, &CDlgSPSSetup::OnBnClickedBtnloadsps)
END_MESSAGE_MAP()

// DlgSPSSetup 消息处理程序

void CDlgSPSSetup::OnBnClickedBtnloadsps()
{
	// TODO: 在此添加控件通知处理程序代码
	CConfigParseXML  ParseXML;
	// 查找SPS文件路径
	OnSelectSPSFilePath();
	// 返回到程序当前路径
	SetCurrentDirectory(m_wcExeCurrentPath);
	// 手动查找SPS文件不能正确读取文件则改为默认路径下查找
	if (FALSE == m_oFraseSPSToXML.LoadSPSFile(m_csSPSFilePath + _T(".R"), m_csSPSFilePath + _T(".X")))
	{
		// 在配置文件中找到SPS文件默认路径
		ParseXML.ParseSPSFile(theApp.m_strLocalXMLFile, m_csSPSFilePath);
		if (FALSE == m_oFraseSPSToXML.LoadSPSFile(m_csSPSFilePath + _T(".R"), m_csSPSFilePath + _T(".X")))
		{
			AfxMessageBox(_T("Please push this button again to find the correct sps file Path!"));
		}
		else
		{
			AfxMessageBox(_T("Open the sps file by default path!"));
		}
	}
	else
	{
		// 将SPS文件路径保存到config配置文件中
		ParseXML.WriteSPSFile(theApp.m_strLocalXMLFile, m_csSPSFilePath);
		// 将配置文件上传到FTP
		PutMatrixXMLToFTPServer(theApp.m_strFTPServerIP,CONFIG_XMLFILE,theApp.m_strLocalXMLFile);
		// 发送消息通知本地的其他程序
		::SendMessage(HWND_BROADCAST,WM_NEWXMLFILE,MATRIX_CONFIG,MATRIX_CONFIG_SPSSETUP);
	}
	OnGenLineInitTXT();
	OnGenOptXML();
	OnGenLineXML();
}
// 生成施工XML
void CDlgSPSSetup::OnGenOptXML()
{
	// TODO: 在此添加控件通知处理程序代码
	m_oFraseSPSToXML.SaveOperationXML(m_csLocalOptXMLFile);
	// 将配置文件上传到FTP
	PutMatrixXMLToFTPServer(theApp.m_strFTPServerIP,OPERATION_XMLFILE,m_csLocalOptXMLFile);
	// 发送消息通知本地的其他程序
	::SendMessage(HWND_BROADCAST,WM_NEWXMLFILE,MATRIX_OPERATION,MATRIX_OPERATION_ALL);
}
// 生成测线XML
void CDlgSPSSetup::OnGenLineXML()
{
	// TODO: 在此添加控件通知处理程序代码
	m_oFraseSPSToXML.SaveLineXML(m_csLocalLineXMLFile);
	// 将配置文件上传到FTP
	PutMatrixXMLToFTPServer(theApp.m_strFTPServerIP,MATRIX_XMLFILE,m_csLocalLineXMLFile);
	// 发送消息通知本地的其他程序
	::SendMessage(HWND_BROADCAST,WM_NEWXMLFILE,MATRIX_LINE,MATRIX_LINE_ALL);
}

// 查找SPS文件路径
void CDlgSPSSetup::OnSelectSPSFilePath(void)
{
	// 打开文件
	CString str = _T("");
	const wchar_t pszFilter[] = _T("SPS文件(*.R)|*.R|SPS文件(*.X)|*.X|SPS文件(*.S)|*.S|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T(".R"), _T(""), OFN_HIDEREADONLY| OFN_OVERWRITEPROMPT, pszFilter, this);
	m_csSPSFilePath = _T("");
	if ( dlg.DoModal()!=IDOK )
	{
		return;
	}
	str = dlg.GetPathName();
	int iFind = str.ReverseFind(_T('.'));
	m_csSPSFilePath = str.Left(iFind);
}
// 生成测线仿真排列文件
void CDlgSPSSetup::OnGenLineInitTXT()
{
	// TODO: 在此添加控件通知处理程序代码
	m_oFraseSPSToXML.GenLineInitTXT(m_csLocalLineInitTXTFile);
}
