// OperationTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OperationTest.h"
#include "OperationTestDlg.h"
//#include "OperationParseXML.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// COperationTestDlg 对话框




COperationTestDlg::COperationTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COperationTestDlg::IDD, pParent)
	, m_pFDUSocket(NULL)
	, m_nTime(20)
	, m_nTimeLen(2000)
	, m_nTimeCount(0)	
	, m_nFDUNum(100)
	, m_dwFrmCount(0)
	, m_strMainIP(_T("192.168.0.11"))
	, m_strError(_T(""))
	, m_dwFDUFrmCount(0)
	, m_strFDUIP(_T(""))
	, m_strFDUError(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COperationTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITTIME, m_nTime);
	DDX_Text(pDX, IDC_EDITTIMELEN, m_nTimeLen);
	DDX_Text(pDX, IDC_EDITSAMPNUM, m_nTimeCount);
	DDX_Text(pDX, IDC_EDITFDUNUM, m_nFDUNum);
	DDX_Text(pDX, IDC_EDIT1, m_dwFrmCount);
	DDX_Text(pDX, IDC_EDIT2, m_strMainIP);
	DDX_Text(pDX, IDC_EDIT3, m_strError);
	DDX_Text(pDX, IDC_EDITFDUCOUNT, m_dwFDUFrmCount);
	DDX_Text(pDX, IDC_EDITFDUIP, m_strFDUIP);
	DDX_Text(pDX, IDC_EDITFDUERROR, m_strFDUError);
}

BEGIN_MESSAGE_MAP(COperationTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTNSTART, &COperationTestDlg::OnBnClickedBtnstart)
	ON_BN_CLICKED(IDC_BTNSTOP, &COperationTestDlg::OnBnClickedBtnstop)
//	ON_BN_CLICKED(IDC_BUTTONSETFDU, &COperationTestDlg::OnBnClickedButtonsetfdu)
	ON_BN_CLICKED(IDC_BTNREADXML, &COperationTestDlg::OnBnClickedBtnreadxml)
	ON_BN_CLICKED(IDC_BUTTON1, &COperationTestDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// COperationTestDlg 消息处理程序

BOOL COperationTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	CString		strFileName;
	SYSTEMTIME  sysTime;
	GetLocalTime(&sysTime);	
	strFileName.Format(_T("%04d%02d%02d%02d%02d%02d%04d.UP"),sysTime.wYear,sysTime.wMonth,sysTime.wDay,
		sysTime.wHour,sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds);		
	
	if(!m_ReFile.Open(strFileName,CFile::modeCreate|CFile::modeWrite))
	{
		AfxMessageBox(_T("创建Re文件错误!"));
	}
	
	strFileName.Replace(_T("UP"),_T("DOWN"));
	if(!m_TxFile.Open(strFileName,CFile::modeCreate|CFile::modeWrite))
	{
		AfxMessageBox(_T("创建Tx文件错误!"));
	}
	// ------上行256字节
	m_sockMainCross.m_pDlg = this;
	if(!m_sockMainCross.Create(0x9001,SOCK_DGRAM))
	{
		AfxMessageBox(_T("创建0x9001端口错误!"));
	}

	int RevBuf =10*2048*1024;
	if (m_sockMainCross.SetSockOpt( SO_RCVBUF, (char*)&RevBuf, sizeof(RevBuf)) == SOCKET_ERROR) 
	{
		m_sockMainCross.Close();
		AfxMessageBox(_T("创建0x9001端口错误!"));
		
	}
	if (m_sockMainCross.SetSockOpt( SO_SNDBUF, (char*)&RevBuf, sizeof(RevBuf)) == SOCKET_ERROR) 
	{
		m_sockMainCross.Close();	
		AfxMessageBox(_T("创建0x9001端口错误!"));
	}
	RevBuf=1;
	m_sockMainCross.SetSockOpt( SO_BROADCAST, (char*)&RevBuf, sizeof(RevBuf));

	// 创建下行帧接口
	m_sockMainCrossDown.m_pDlg = this;
	if(!m_sockMainCrossDown.Create(0x7777,SOCK_DGRAM))		// ?????? 25344 
	{
		AfxMessageBox(_T("创建0x7777端口错误!"));
	}

	RevBuf =10*2048*1024;
	if (m_sockMainCrossDown.SetSockOpt( SO_RCVBUF, (char*)&RevBuf, sizeof(RevBuf)) == SOCKET_ERROR) 
	{
		m_sockMainCrossDown.Close();
		AfxMessageBox(_T("创建m_sockMainCrossDown端口错误!"));

	}
	if (m_sockMainCrossDown.SetSockOpt( SO_SNDBUF, (char*)&RevBuf, sizeof(RevBuf)) == SOCKET_ERROR) 
	{
		m_sockMainCrossDown.Close();	
		AfxMessageBox(_T("创建m_sockMainCrossDown端口错误!"));
	}
	RevBuf=1;
	m_sockMainCrossDown.SetSockOpt( SO_BROADCAST, (char*)&RevBuf, sizeof(RevBuf));



	InitFDUDevice(m_nFDUNum);
	CreateFDUSocket();


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void COperationTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COperationTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COperationTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

////////////////////////////////////////////////////////////////////////////
// 创建socket, 仿真主交叉站
bool COperationTestDlg::CreateFDUSocket(void)
{
	if(m_pFDUSocket)
		return false;

	m_pFDUSocket = new CFDUSocket;
	m_pFDUSocket->m_pDlg = this;
	m_pFDUSocket->Create(0xC000,SOCK_DGRAM);

	int RevBuf =10*2048*1024;
	if (m_pFDUSocket->SetSockOpt( SO_RCVBUF, (char*)&RevBuf, sizeof(RevBuf)) == SOCKET_ERROR) 
	{
		m_pFDUSocket->Close();
		delete m_pFDUSocket;
		m_pFDUSocket = NULL;
		return false;
	}
	if (m_pFDUSocket->SetSockOpt( SO_SNDBUF, (char*)&RevBuf, sizeof(RevBuf)) == SOCKET_ERROR) 
	{
		m_pFDUSocket->Close();
		delete m_pFDUSocket;
		m_pFDUSocket = NULL;
		return false;
	}

	return true;
}

void COperationTestDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码

	m_sockMainCross.Close();
	m_sockMainCrossDown.Close();

	if(m_pFDUSocket)
	{
		m_pFDUSocket->Close();
		delete m_pFDUSocket;
		m_pFDUSocket = NULL;
	}
	int i,nCount = m_arrFDU.GetCount();
	for (i=0;i<nCount;i++)
	{
		delete m_arrFDU[i];
	}
	m_arrFDU.RemoveAll();
	if(m_ReFile.m_hFile!=CFile::hFileNull)
	{
		m_ReFile.Close();
	}
	if(m_TxFile.m_hFile!=CFile::hFileNull)
	{
		m_TxFile.Close();
	}
}
////////////////////////////////////////////////////////////////////////////
// 初始化FDU设备
int COperationTestDlg::InitFDUDevice(int nCount)
{
	int i,nSum;
	nSum = m_arrFDU.GetCount();
	for(i=0;i<nSum;i++)
	{
		delete m_arrFDU[i];		
	}
	m_arrFDU.RemoveAll();
	CFDUDevice* pDevice =NULL;
	for (i=0;i<nCount;i++)
	{
		pDevice = new CFDUDevice;
		pDevice->m_dwIP = i;
		m_arrFDU.Add(pDevice);
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
// 定时发送采样数据
void COperationTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_nTimeCount++;
	int i;

	// 模拟数据丢帧
	SendFDUData(m_arrFDU[0],TRUE);

	for(i=1;i<m_arrFDU.GetCount();i++)
	{
		SendFDUData(m_arrFDU[i],FALSE);
	}	
	if(m_nTimeLen<m_nTimeCount*m_nTime)
	{
		KillTimer(100);
		m_nTimeCount = 0;
	}
	UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);
}
//////////////////////////////////////////////////////////////////////////
// 启动定时发送
void COperationTestDlg::OnBnClickedBtnstart()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
/*
	m_nTimeCount =0;
	SetTimer(100,m_nTime,NULL);

	CString		strFileName;
	CTime		CurrTime;
	// 先创建文件
	CurrTime = CTime::GetCurrentTime();
	strFileName = CurrTime.Format(_T("%Y%m%d%H%M%S"));
	strFileName += _T(".bin");
	strFileName = _T("E:\\temp\\")+strFileName;

	m_File.Open(strFileName,CFile::modeCreate|CFile::modeWrite);

	strFileName +=_T("_Re1");
	m_ReFile.Open(strFileName,CFile::modeCreate|CFile::modeWrite);
*/	

	// 计数
	static unsigned char byData=0;
	unsigned char byData1=0;

//	WORD   wPort;
	DWORD  dwIP=123456;
	BYTE   pBuf[256];


	memset(pBuf,byData,256);

	// 源地址: FDUIP
	memcpy_s(pBuf+16,4,&(dwIP),sizeof(DWORD));

	// 目标地址: 主机IP
	dwIP = 65432100;
	memcpy_s(pBuf+20,4,&dwIP,sizeof(DWORD));

	pBuf[24] = 0x30;
	pBuf[25] = 0x00;

	pBuf[26] = 0x03;
	pBuf[27] = 0;
	
	memset(pBuf+28,0x11,4);
	
	// pBuf[28] = 0x07;
	
	pBuf[32] = 0x08;
	memset(pBuf+33,0x22,4);

	SaveMatrixDownData(pBuf,256);
	SaveMatrixDownData(pBuf,256);
	SaveMatrixDownData(pBuf,256);
	SaveMatrixDownData(pBuf,256);
	SaveMatrixDownData(pBuf,256);

}
//////////////////////////////////////////////////////////////////////////
// 在定时结束后，点击停止，关闭文件
void COperationTestDlg::OnBnClickedBtnstop()
{
	// TODO: 在此添加控件通知处理程序代码
	/*
	KillTimer(100);
	m_nTimeCount = 0;
	m_File.Close();
	m_ReFile.Close();
	*/
}
////////////////////////////////////////////////////////////////////////////
//  向主机发送采样数据
int COperationTestDlg::SendFDUData(CFDUDevice* pDevice,BOOL bLost)
{	
	// 计数
	static unsigned char byData=0;
	unsigned char byData1=0;
	
	WORD   wPort;
	DWORD  dwIP;
	char   pBuf[256];
	
	
	memset(pBuf,byData,256);

	// 源地址: FDUIP
	memcpy_s(pBuf+16,4,&(pDevice->m_dwIP),sizeof(DWORD));
	
	// 目标地址: 主机IP
	dwIP = inet_addr("192.168.0.12");
	memcpy_s(pBuf+20,4,&dwIP,sizeof(DWORD));
	
	// 端口暂时不设
//	memcpy_s(pBuf+24,2,&wHostPort,sizeof(WORD));

	pBuf[26] = 0x03;
	pBuf[27] = 0;

	// 数据指针
	pBuf[28] = pDevice->m_nIndex;
	pBuf[29] = 0x00;	
	
	// 前面一部分数据为IP地址
	byData1 = (unsigned char )pDevice->m_dwIP;
	memset(pBuf+30,byData1,16);

	wPort = 0xB100 + pDevice->m_nIndex;
//	m_File.Write(&(wPort),2);		
	m_File.Write(pBuf+30,222);
	
	// 发送, 模拟丢数
	if(!bLost)
		m_pFDUSocket->SendTo(pBuf,256 , 0xB100 + pDevice->m_nIndex ,_T("192.168.0.12"));

	// 保存在缓冲区
	pDevice->WriteData(pBuf,256);
	byData++;
	return 0;
}
//////////////////////////////////////////////////////////////////////////
//  处理主机请求重发
int COperationTestDlg::ProcessRetrans(char* pBuf)
{
	//char	pDataBuf[256];
	int		i;
	DWORD	dwFDUIP;
	WORD    wPort;
	WORD	wFrmIndex;

	CFDUDevice* pDevice=NULL;
	if(pBuf[26]!=0x03 || pBuf[27]!=0)
		return 0;

	memcpy_s(&dwFDUIP,4,pBuf+20,sizeof(DWORD));
	memcpy_s(&wPort,2,pBuf+24,sizeof(WORD));
	memcpy_s(&wFrmIndex,2,pBuf+32,sizeof(WORD));

	for (i=0;i<m_arrFDU.GetCount();i++)
	{
		pDevice = m_arrFDU[i];
		if(pDevice->m_dwIP != dwFDUIP)
			continue;
/*		
		// 源地址
		memcpy_s(pDataBuf+16,4,&(pDevice->m_dwIP),sizeof(DWORD));
		// 目标地址
		dwHostIP = inet_addr("192.168.0.12");
		memcpy_s(pDataBuf+20,4,&dwHostIP,sizeof(DWORD));

		pDataBuf[26] = 0x03;
		pDataBuf[27] = 0;

		pDataBuf[28] = LOBYTE(wFrmIndex);
		pDataBuf[29] = HIBYTE(wFrmIndex);

		memcpy(pDataBuf+30,pDevice->m_pBuf[wFrmIndex],222);
		m_pFDUSocket->SendTo(pDataBuf,256 , wPort ,_T("192.168.0.12"));
*/		
		m_pFDUSocket->SendTo(pDevice->m_pBuf[wFrmIndex],256 , wPort ,_T("192.168.0.12"));
	
		return 1;
	}
	return 0;
}

void COperationTestDlg::OnBnClickedButtonsetfdu()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	InitFDUDevice(m_nFDUNum);
}

int COperationTestDlg::ReadXML(void)
{
   CoInitialize(NULL);
	MSXML2::IXMLDOMDocumentPtr pDoc;
	HRESULT hr;
try
{
	hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
	if(FAILED(hr))
	{  
		MessageBox(_T("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!")); 
		return 1;
	} 


	//加载文件 
	_variant_t vt = _T("E:\\temp\\Matrix_Operation.xml");
	VARIANT_BOOL  vb=pDoc->load(vt); 

	MSXML2::IXMLDOMNodePtr  pNode;

	//在树中查找名为Book的节点,"//"表示在任意一层查找 
	pNode=pDoc->selectSingleNode(_T("//OperationTable"));
	MSXML2::DOMNodeType nodeType; 

	//得到节点类型 
	pNode->get_nodeType(&nodeType); 

	//节点名称 
	CString strName;

	strName=(char *)pNode->GetnodeName();

	//节点属性,放在链表中 
	MSXML2::IXMLDOMNamedNodeMapPtr pAttrMap=NULL;
	MSXML2::IXMLDOMNodePtr   pAttrItem;
	_variant_t variantValue;
	pNode->get_attributes(&pAttrMap);

	long count;
	count=pAttrMap->get_length(&count);

	pAttrMap->get_item(0,&pAttrItem);
	//取得节点的值
	pAttrItem->get_nodeTypedValue(&variantValue);
	CString strRevFilePath=(LPCTSTR)(_bstr_t)variantValue;

	UpdateData(FALSE);
/**/
	CoUninitialize();
}
catch (CMemoryException*  )
{

}
catch (CFileException*  )
{
}
catch (CException* e)
{
	e->ReportError();
}
	return 0;
}

void COperationTestDlg::OnBnClickedBtnreadxml()
{
	// TODO: 在此添加控件通知处理程序代码
//	ReadXML();
	SaveXML();

//	COperationParseXML     PXML;
//	PXML.ParseOperationTable(_T("Matrix.xml"));

	/*
	SYSTEMTIME   sysTime;
	GetLocalTime(&sysTime);	
	CString strLogFile;
	strLogFile.Format(_T("..\\DEBUG\\Operation%02d%02d%02d.Log"),sysTime.wHour,sysTime.wMinute,sysTime.wSecond);

	FILE*   hFile;
	_tfopen_s(&hFile,strLogFile,_T("a"));

	strLogFile.Format(_T("中文字符串测试 %d ,%d\n"),100,200);
	_ftprintf(hFile,_T("%s\n"),strLogFile);


	fclose( hFile);
	*/

}

int COperationTestDlg::SaveXML(void)
{
	UpdateData();

	CoInitialize(NULL);

	MSXML2::IXMLDOMDocumentPtr pDoc; 
	MSXML2::IXMLDOMNodeListPtr  pNodeList;
	MSXML2::IXMLDOMElementPtr  xmlRoot ; 
	MSXML2::IXMLDOMElementPtr pNode;
	//MSXML2::IXMLDOMElementPtr pNode1;
	//MSXML2::IXMLDOMElementPtr pNode2;

	//创建DOMDocument对象 
	HRESULT hr = pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60)); 
	if(!SUCCEEDED(hr)) 
	{  
		MessageBox("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!"); 
		return 0;
	}
	try
	{
	
		 //_variant_t vt = _T("E:\\temp\\Matrix_Operation.xml");
		_variant_t vt = _T("E:\\he.xml");
		//_variant_t vt = _T("MatrixOperationServer.xml");
		VARIANT_BOOL vtBool = pDoc->load(vt);
		if(vtBool == VARIANT_FALSE)
		{
			return -1;
		}

		//根节点的名称为Book
		//创建元素并添加到文档中
		//xmlRoot=pDoc->createElement((_bstr_t)"Book");
		pNodeList = pDoc->getElementsByTagName((_bstr_t)(_T("Matrix")));

		//设置属性
		xmlRoot = pNodeList->Getitem(0);
		xmlRoot->setAttribute("id","bookoo1");
		xmlRoot->setAttribute("index","77");
		// pDoc->appendChild(xmlRoot);


		//添加“author”元素
		pNode=pDoc->createElement((_bstr_t)_T("Author"));
		pNode->setAttribute((_bstr_t)_T("属性"),(_variant_t)_T("attribute"));
		pNode->Puttext((_bstr_t)"AAAA");
		xmlRoot->appendChild(pNode);

		//添加“Title”元素
		pNode=pDoc->createElement("Title");
		pNode->Puttext("bbbbbb");
		xmlRoot->appendChild(pNode);

		//保存到文件 
		//如果不存在就建立,存在就覆盖 
		pDoc->save("E:\\he.xml");


		//pNode.Release();
		//pNode1.Release();
		//xmlRoot.Release();
		//pDoc.Release();

		CoUninitialize();
		return 0;
	}

	catch (CException* )
	{
		return 0;
	}
}
// 关闭接收文件、创建新文件
void COperationTestDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_ReFile.m_hFile!=CFile::hFileNull)
	{
		m_ReFile.Close();
	}
	if(m_TxFile.m_hFile!=CFile::hFileNull)
	{
		m_TxFile.Close();
	}
	CString		strFileName;
	SYSTEMTIME  sysTime;
	GetLocalTime(&sysTime);	

	strFileName.Format(_T("%04d%02d%02d%02d%02d%02d%04d.UP"),sysTime.wYear,sysTime.wMonth,sysTime.wDay,
		sysTime.wHour,sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds);		

	if(!m_ReFile.Open(strFileName,CFile::modeCreate|CFile::modeWrite))
	{
		AfxMessageBox(_T("创建Re文件错误!"));
	}

	strFileName.Replace(_T("UP"),_T("DOWN"));
	if(!m_TxFile.Open(strFileName,CFile::modeCreate|CFile::modeWrite))
	{
		AfxMessageBox(_T("创建Tx文件错误!"));
	}
}

void COperationTestDlg::SaveMatrixData(BYTE* pBuf, int nSize)
{
	int      i;
	DWORD		dwTemp;
	WORD		wTemp;
	CString		strTemp,strResult;
	if(m_ReFile.m_hFile==CFile::hFileNull)
	{
		return;
	}
	if(nSize<256)
	{		
		m_strError = _T("数据长度不足256字节");
		UpdateData(FALSE);
		return;
	}
	
	// 源地址
	memcpy(&dwTemp,pBuf+16,4);
	strResult.Format(_T("%010u "),dwTemp);
	// 目的地址
	memcpy(&dwTemp,pBuf+20,4);
	strTemp.Format(_T("%010u "),dwTemp);
	strResult+=strTemp;
	// 端口
	wTemp=MAKEWORD(pBuf[24],pBuf[25]);
	strTemp.Format(_T("0x%04X "),wTemp);
	strResult+=strTemp;
	// 命令字
	wTemp=MAKEWORD(pBuf[26],pBuf[27]);
	strTemp.Format(_T("%02u "),wTemp);
	strResult+=strTemp;
	
	for(i=28;i<250;i+=5)
	{
		strTemp.Format(_T("%02X%02X%02X%02X%02X "),pBuf[i],pBuf[i+1],pBuf[i+2],pBuf[i+3],pBuf[i+4]);
		strResult+=strTemp;
	}
	strResult+=_T("\n");
	m_ReFile.Write(strResult,strResult.GetLength());
}

void COperationTestDlg::SaveMatrixDownData(BYTE* pBuf, int nSize)
{
	int      i;
	DWORD		dwTemp;
	WORD		wTemp;
	CString		strTemp,strResult;	
	if(nSize<128)
	{
		m_strFDUError = _T("数据长度不足128字节");
		UpdateData(FALSE);
		return;
	}

	// 源地址
	memcpy(&dwTemp,pBuf+16,4);
	strResult.Format(_T("%010u "),dwTemp);
	// 目的地址
	memcpy(&dwTemp,pBuf+20,4);
	m_strFDUIP.Format(_T("%010u "),dwTemp);
	m_dwFDUFrmCount++;
	UpdateData(FALSE);
	strResult+=m_strFDUIP;
	// 端口
	wTemp=MAKEWORD(pBuf[24],pBuf[25]);
	strTemp.Format(_T("0x%04X "),wTemp);
	strResult+=strTemp;
	// 命令字
	wTemp=MAKEWORD(pBuf[26],pBuf[27]);
	strTemp.Format(_T("%02u "),wTemp);
	strResult+=strTemp;

	// 包时刻
	memcpy(&dwTemp,pBuf+28,4);
	strTemp.Format(_T("%08X "),dwTemp);
	strResult+=strTemp;

	for(i=32;i<123;i+=5)
	{
		strTemp.Format(_T("%02X%02X%02X%02X%02X "),pBuf[i],pBuf[i+1],pBuf[i+2],pBuf[i+3],pBuf[i+4]);
		strResult+=strTemp;
	}
	strResult+=_T("\n");
	m_TxFile.Write(strResult,strResult.GetLength());
}