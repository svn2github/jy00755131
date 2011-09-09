// DlgDiskRecord.cpp : 实现文件
//

#include "stdafx.h"
#include "Config.h"
#include "DlgDiskRecord.h"


// CDlgDiskRecord 对话框

IMPLEMENT_DYNAMIC(CDlgDiskRecord, CBCGPDialog)

CDlgDiskRecord::CDlgDiskRecord(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgDiskRecord::IDD, pParent)
	, m_nDiskRecordMode(0)
	, m_nBackupSettings(0)
	, m_strFilePath(_T("D:\\Data\\"))
	, m_strNormalPath(_T("D:\\Data\\Normal\\"))
	, m_strTestPath(_T("D:\\Data\\Test\\"))
{
	EnableVisualManagerStyle();
}

CDlgDiskRecord::~CDlgDiskRecord()
{
}

void CDlgDiskRecord::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBODISKRECORDMODE, m_nDiskRecordMode);
	DDX_Radio(pDX, IDC_RADIOFLAT, m_nBackupSettings);
	DDX_Text(pDX, IDC_EDITFILEDIR, m_strFilePath);
	DDX_Text(pDX, IDC_STATICNORMALDIR, m_strNormalPath);
	DDX_Text(pDX, IDC_STATICTESTDIR, m_strTestPath);
	DDX_Control(pDX, IDC_EDITFILEDIR, m_ctrlFilePath);
}


BEGIN_MESSAGE_MAP(CDlgDiskRecord, CBCGPDialog)
	ON_BN_CLICKED(IDC_BTNOK, &CDlgDiskRecord::OnBnClickedBtnok)
	ON_BN_CLICKED(IDC_BTNAPPLY, &CDlgDiskRecord::OnBnClickedBtnapply)
	ON_BN_CLICKED(IDC_BTNRESET, &CDlgDiskRecord::OnBnClickedBtnreset)	
	ON_BN_CLICKED(IDC_RADIOFLAT, &CDlgDiskRecord::OnBnClickedRadioflat)
	ON_BN_CLICKED(IDC_RADIODAY, &CDlgDiskRecord::OnBnClickedRadioday)
END_MESSAGE_MAP()

// CDlgDiskRecord 消息处理程序
BOOL CDlgDiskRecord::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ctrlFilePath.EnableFolderBrowseButton();
	OnBnClickedBtnreset();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgDiskRecord::SetParams(CDiskRecordConfig* pDiskRecord)
{
	if(!pDiskRecord)
		return;
	m_nDiskRecordMode = pDiskRecord->GetDiskRecordMode();
	m_nBackupSettings = pDiskRecord->GetBackupSetting();
	m_strFilePath = pDiskRecord->GetFilePath();
	m_strNormalPath = pDiskRecord->GetNormalFilePath();
	m_strTestPath = pDiskRecord->GetTestFilePath();
	//m_ctrlFilePath.SetWindowText(m_strFilePath);
}
void CDlgDiskRecord::ApplyParams(CDiskRecordConfig* pDiskRecord)
{
	if(!pDiskRecord)
		return;
	
	pDiskRecord->SetDiskRecordMode(m_nDiskRecordMode);
	pDiskRecord->SetBackupSetting(m_nBackupSettings);
	pDiskRecord->SetFilePath(m_strFilePath);
	
}
/**
 * @brief 响应O按钮
 * @note  用户按下Ok按钮后，先按照Apply按钮处理逻辑保存参数，在退出设置对话框
 * @param void
 * @return void
 */
void CDlgDiskRecord::OnBnClickedBtnok()
{
	OnBnClickedBtnapply();
	OnOK();
}
/**
 * @brief 响应Apply按钮
 * @note  用户按下Apply后，保存当前窗口的设置参数，并写入XML配置文件中。
 * @param void
 * @return void
 */
void CDlgDiskRecord::OnBnClickedBtnapply()
{
	UpdateData(TRUE);
	CDiskRecordConfig   DiskRecord;
	ApplyParams(&DiskRecord);
	CConfigParseXML  ParseXML;
	ParseXML.WriteDiskRecord(theApp.m_strLocalXMLFile,&DiskRecord);	
	SetParams(&DiskRecord);
	UpdateData(FALSE);
	theApp.SaveXMLToFTPServer();
}
/**
 * @brief 响应Reset按钮
 * @note  用户按下Reset后，从XML配置文件中读取存盘设置参数，并更新用户界面
 * @param void
 * @return void
 */
void CDlgDiskRecord::OnBnClickedBtnreset()
{
	CConfigParseXML  ParseXML;
	CDiskRecordConfig   DiskRecord;
	ParseXML.ParseDiskRecord(theApp.m_strLocalXMLFile,&DiskRecord);
	SetParams(&DiskRecord);
	UpdateData(FALSE);
}

/**
 * @brief 选择Flat模式
 * @note  用户选择Flat模式，更新显示界面的数据文件目录和测试文件目录，
 采集数据文件存放到normal子目录下，测试数据文件放到test下面
 * @param void
 * @return void
 */
void CDlgDiskRecord::OnBnClickedRadioflat()
{
	UpdateData(TRUE);
	CDiskRecordConfig   DiskRecord;
	ApplyParams(&DiskRecord);
	SetParams(&DiskRecord);
	UpdateData(FALSE);
}
/**
 * @brief 选择Day模式
 * @note  用户选择Flat模式，更新显示界面的数据文件目录和测试文件目录，
 采集数据文件存放到“normal\当前时间”子目录下，测试数据文件放到“test\当前时间”下面
 * @param void
 * @return void
 */
void CDlgDiskRecord::OnBnClickedRadioday()
{
	OnBnClickedRadioflat();
}
