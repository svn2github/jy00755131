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
, m_csSPSRFilePath(_T(""))
, m_csSPSXFilePath(_T(""))
{
	EnableVisualManagerStyle();
	m_csLocalOptXMLFile = CLIENTDIR_XMLFILE;
	m_csLocalOptXMLFile += OPERATION_XMLFILE;
	m_csLocalLineXMLFile = CLIENTDIR_XMLFILE;
	m_csLocalLineXMLFile += MATRIX_XMLFILE;
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
	ON_BN_CLICKED(IDC_BTNGENOPTXML, &CDlgSPSSetup::OnBnClickedBtngenoptxml)
	ON_BN_CLICKED(IDC_BTNGENLINEXML, &CDlgSPSSetup::OnBnClickedBtngenlinexml)
END_MESSAGE_MAP()

// DlgSPSSetup 消息处理程序

void CDlgSPSSetup::OnBnClickedBtnloadsps()
{
	// TODO: 在此添加控件通知处理程序代码
	// 在配置文件中找到SPS文件默认路径
	CConfigParseXML  ParseXML;
	ParseXML.ParseSPSFile(theApp.m_strLocalXMLFile, m_csSPSRFilePath, m_csSPSXFilePath);
	// 按照路径不能正确读取文件则改为手动查找SPS文件
	if (FALSE == m_oFraseSPSToXML.LoadSPSFile(m_csSPSRFilePath, m_csSPSXFilePath))
	{
		// 查找SPS文件路径
		OnSelectSPSRFilePath();
		OnSelectSPSXFilePath();
		if (TRUE == m_oFraseSPSToXML.LoadSPSFile(m_csSPSRFilePath, m_csSPSXFilePath))
		{
			// 将SPS文件路径保存到config配置文件中
			ParseXML.WriteSPSFile(theApp.m_strLocalXMLFile, m_csSPSRFilePath, m_csSPSXFilePath);
			// 将配置文件上传到FTP
			PutMatrixXMLToFTPServer(theApp.m_strFTPServerIP,CONFIG_XMLFILE,theApp.m_strLocalXMLFile);
			// 发送消息通知本地的其他程序
			::SendMessage(HWND_BROADCAST,WM_NEWXMLFILE,MATRIX_CONFIG,MATRIX_CONFIG_SPSSETUP);
		}
	}
}

void CDlgSPSSetup::OnBnClickedBtngenoptxml()
{
	// TODO: 在此添加控件通知处理程序代码
	m_oFraseSPSToXML.SaveOperationXML(m_csLocalOptXMLFile);
	// 将配置文件上传到FTP
	PutMatrixXMLToFTPServer(theApp.m_strFTPServerIP,OPERATION_XMLFILE,m_csLocalOptXMLFile);
	// 发送消息通知本地的其他程序
	::SendMessage(HWND_BROADCAST,WM_NEWXMLFILE,MATRIX_OPERATION,MATRIX_OPERATION_ALL);
}

void CDlgSPSSetup::OnBnClickedBtngenlinexml()
{
	// TODO: 在此添加控件通知处理程序代码
	m_oFraseSPSToXML.SaveLineXML(m_csLocalLineXMLFile);
	// 将配置文件上传到FTP
	PutMatrixXMLToFTPServer(theApp.m_strFTPServerIP,MATRIX_XMLFILE,m_csLocalLineXMLFile);
	// 发送消息通知本地的其他程序
	::SendMessage(HWND_BROADCAST,WM_NEWXMLFILE,MATRIX_LINE,MATRIX_LINE_ALL);
}

// 查找SPS_R文件路径
void CDlgSPSSetup::OnSelectSPSRFilePath(void)
{
	// 打开文件
	const wchar_t pszFilter[] = _T("SPS_R文件(*.R)|*.R|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T(".R"), _T("JO.R"), OFN_HIDEREADONLY| OFN_OVERWRITEPROMPT, pszFilter, this);
	if ( dlg.DoModal()!=IDOK )
	{
		return;
	}
	m_csSPSRFilePath = dlg.GetPathName();
}

// 查找SPS_X文件路径
void CDlgSPSSetup::OnSelectSPSXFilePath(void)
{
	// 打开文件
	const wchar_t pszFilter[] = _T("SPS_X文件(*.X)|*.X|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T(".X"), _T("JO.X"), OFN_HIDEREADONLY| OFN_OVERWRITEPROMPT, pszFilter, this);

	if ( dlg.DoModal()!=IDOK )
	{
		return;
	}
	m_csSPSXFilePath = dlg.GetPathName();
}
