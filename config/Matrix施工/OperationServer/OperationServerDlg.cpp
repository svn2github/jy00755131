// OperationServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OperationServer.h"
#include "OperationServerDlg.h"
#include "ClientSocket.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// 确保程序只运行一次,自己定义一个属性值
extern CString	 g_strProgName;
extern HANDLE   g_hProgValue;               
//////////////////////////////////////////////////////////////////////////
// 全局函数，检查文件夹是否存在
BOOL  FolderExist(LPCTSTR strPath)
{
	WIN32_FIND_DATA   wfd;
	BOOL rValue = FALSE;
	CString strPathFile =strPath;
	strPathFile +=_T("\\*");
	HANDLE hFind = FindFirstFile(strPathFile, &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = TRUE;   
	}
	FindClose(hFind);
	return rValue;
}
//////////////////////////////////////////////////////////////////////////
// 全局函数，创建多级目录
bool CreateMultipleDirectory(LPCTSTR szPath)
{
	CString strDir(szPath); // 存放要创建的目录字符串
	// 确保以'\'结尾以创建最后一个目录
	if (strDir.GetAt(strDir.GetLength()-1)!=_T('\\'))
	{
		strDir.AppendChar(_T('\\'));
	}
	std::vector<CString> vPath;// 存放每一层目录字符串
	CString strTemp;// 一个临时变量,存放目录字符串
	bool bSuccess = false;// 成功标志
	// 遍历要创建的字符串
	for (int i=0;i<strDir.GetLength();++i)
	{
		if (strDir.GetAt(i) != _T('\\')) 
		{// 如果当前字符不是'\\'
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else 
		{// 如果当前字符是'\\'
			vPath.push_back(strTemp);// 将当前层的字符串添加到数组中
			strTemp.AppendChar(_T('\\'));
		}
	}
	// 遍历存放目录的数组,创建每层目录
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		// 如果CreateDirectory执行成功,返回true,否则返回false
		bSuccess = CreateDirectory(*vIter, NULL) ? true : false;    
	}
	return bSuccess;
}


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


// COperationServerDlg 对话框



COperationServerDlg::COperationServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COperationServerDlg::IDD, pParent)
	, m_pClientSocket(NULL)
	// , m_strHostIP(_T("192.168.0.12"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COperationServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTMSG, m_ListMsg);
}

BEGIN_MESSAGE_MAP(COperationServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTNREV, &COperationServerDlg::OnBnClickedBtnrev)	
END_MESSAGE_MAP()


// COperationServerDlg 消息处理程序

BOOL COperationServerDlg::OnInitDialog()
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
	// 设置程序属性，确保程序仅允许一次	
	SetProp(m_hWnd,g_strProgName,g_hProgValue);

	// CSEGDFile   SegdFile;
	//WORD wTemp=SegdFile.ConvertWORDToBCD(8058);
  	int  i;	
	// ----------初始化尾包接收端口-----------------------------------------
	if(!m_sockTailFrame.CreateTailFrameSocket(OPERATION_LOCALTIMEPORT,theApp.m_strHostIP))
	{
		WriteLogMsg(_T("COperationServerDlg::OnInitDialog()"),_T("打开接收尾包的端口0xC000失败！"),LOGTYPE_ERROR,TRUE);
	}	
	// ------------初始化与客户端通信接口---------------------------------------------------
	m_pClientSocket = new CSocketShotClient;
	if(m_pClientSocket)
	{
		if(!m_pClientSocket->CreateClientSocket(OPERATION_SVRPORT,theApp.m_strHostIP,this))
		{
			WriteLogMsg(_T("COperationServerDlg::OnInitDialog()"),_T("Fail to open OPERATION_SVRPORT socket！"),LOGTYPE_ERROR,TRUE);
			delete m_pClientSocket;
			m_pClientSocket = NULL;
		}
	}	
	// ------------初始化与爆炸机控制器所连采集站通信接口------------------------------
	m_pBlasterSocket = new CSocketShotBlaster;
	if(m_pBlasterSocket)
	{
		if(!m_pBlasterSocket->CreateSocket(OPERATION_FDUBLASTERPORT,theApp.m_strHostIP,this))
		{
			WriteLogMsg(_T("COperationServerDlg::OnInitDialog()"),_T("Creat Blaster socket Fail！"),LOGTYPE_ERROR,TRUE);
			delete m_pBlasterSocket;
			m_pBlasterSocket = NULL;
		}
	}	
	
	// ----------初始化施工线程-----------------------------------------	
	for (i=0;i<OPERATION_THREAD_MAXIUM;i++)
	{
		m_pOperaThead[i] = new COperaThread;	
		m_pOperaThead[i]->Create(i,theApp.m_dwHostIP,m_pClientSocket,m_pBlasterSocket);
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void COperationServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void COperationServerDlg::OnPaint()
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
HCURSOR COperationServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void COperationServerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	/*m_OperaThead.EndThread();*/

	int i;
	// 退出线程
	for (i=0;i<OPERATION_THREAD_MAXIUM;i++)
	{
		m_pOperaThead[i]->Destory();
		delete m_pOperaThead[i];
		m_pOperaThead[i] = NULL;	
	}
	// 关闭与客户端通信socket
	if(m_pClientSocket)
	{
		m_pClientSocket->Close();
		delete m_pClientSocket;
		m_pClientSocket = NULL;
	}
	// 关闭与爆炸机控制器通信的端口
	if(m_pBlasterSocket)
	{
		m_pBlasterSocket->Close();
		delete m_pBlasterSocket;
		m_pBlasterSocket = NULL;
	}
	// 关闭端口
	m_sockTailFrame.Close();
}
//////////////////////////////////////////////////////////////////////////
// 程序退出
void COperationServerDlg::OnCancel()
{
	if (AfxMessageBox(_T("Are you sure ?"), MB_YESNO) == IDNO)
	{
		return; 
	}

	CDialog::OnCancel();
}

//////////////////////////////////////////////////////////////////////////
void COperationServerDlg::OnBnClickedBtnrev()
{	
	//// 以下为测试代码
	/*
	CProcessImpulsive  *pProcess = new CProcessImpulsive;
	pProcess->m_dwRecordLen = 10000;
	CShotSpread		*pSpread = new CShotSpread;
	UINT i;
	for (i=0;i<100;i++)
	{
		pSpread->Append(i,0);
	}
	for (i=0;i<pSpread->GetCount();i++)
	{
		pSpread->m_arrSpread[i].m_lAckedType = TRUE;
	}
	m_pOperaThead[0]->m_pShotClient->m_pSpread = pSpread;
	m_pOperaThead[0]->m_pShotClient->m_pProcessType = pProcess;
	*/
//	m_pOperaThead[0]->m_pShotGOEvent->SetEvent();

/*  测试将采样数据发送到客户端的代码
	static unsigned char	pBuf[250];
	static unsigned long  dwData=0;
	pBuf[0] = 0xEB;
	pBuf[1] = 0x90;

	// 帧长
	pBuf[2] = 246;
	pBuf[3] = 0;
	// 帧计数
	// memcpy_s(pBuf+4,4,&dwFrmIndex,4);
	memset(pBuf+4,0,4);
	// 帧类型
	pBuf[8] = 0x02;
	// 命令码
	pBuf[9]  =0;
	pBuf[10] = 0;
	// 数据个数
	pBuf[11] =1;
	pBuf[12] =0;
	// 空闲线程索引号
	memset(pBuf+13,0,4);
	memset(pBuf+17,0,4);

	for(int i=0;i<74;i++)
	{
		memcpy_s(pBuf+21+i*3,4,&dwData,4);
	}
	dwData++;
	if(dwData>0xFFFFF0)
		dwData = 0;
	// 帧校验
	pBuf[243]=0;
	// 帧尾
	pBuf[244] = 0x14;
	pBuf[245] = 0x6F;

	m_sockTailFrame.SendTo(pBuf,246,0xDF00,_T("192.168.0.12"));
*/
		
//	SegdFile.SaveSEGD();
}
/******************************************************************************
    函    数：CreateClientSocket
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 创建与客户端通信socket

    修改历史：

******************************************************************************
bool COperationServerDlg::CreateClientSocket(UINT dwPort, LPCTSTR strHostIP)
{
	if(m_pClientSocket)
	{
		m_pClientSocket->Close();
		delete m_pClientSocket;
		m_pClientSocket = NULL;
	}
	m_pClientSocket = new CSocketShotClient;
	m_pClientSocket->m_pDlg = this;
	// 创建socket
	if(!m_pClientSocket->Create(dwPort,SOCK_DGRAM,strHostIP))
	{
		delete m_pClientSocket;
		m_pClientSocket =NULL;
		return false;
	}
	// 设置接收缓冲区大小
	int RevBuf = 2048*1024;
	if (m_pClientSocket->SetSockOpt( SO_RCVBUF, (char*)&RevBuf, sizeof(RevBuf)) == SOCKET_ERROR) 
	{
		m_pClientSocket->Close();
		delete m_pClientSocket;
		m_pClientSocket = NULL;
		return false;
	}
	// 设置发送缓冲区大小
	if (m_pClientSocket->SetSockOpt(SO_SNDBUF, (char*)&RevBuf, sizeof(RevBuf)) == SOCKET_ERROR) 
	{
		m_pClientSocket->Close();
		delete m_pClientSocket;
		m_pClientSocket = NULL;
		return false;
	}
	// 设置允许接受广播信息
	RevBuf = 1;
	if (m_pClientSocket->SetSockOpt(SO_BROADCAST, (char*)&RevBuf, sizeof(RevBuf)) == SOCKET_ERROR) 
	{
		m_pClientSocket->Close();
		delete m_pClientSocket;
		m_pClientSocket = NULL;
		return false;
	}
	return true;
}*/
/******************************************************************************
    函    数： FindIdleThread
    
    输    入： 
    
    输    出： DWORD* pThreadID - 返回线程组在数组中索引号
  
    返 回 值： 
    
    功    能： 查询放炮线程组的状态

    修改历史：

*******************************************************************************/
int COperationServerDlg::FindIdleThread()
{	
	int  i;
	for ( i = 0; i<OPERATION_THREAD_MAXIUM; i++)
	{
		if(m_pOperaThead[i]->GetThreadState()==THREAD_IDLE)
		{		
			return i;
		}		
	}
	return -1;
}

/**
 * @brief 解析爆炸机控制器所连采集站数据帧
 * @note  从数据帧中解析出爆炸机控制器传送的参数信息
 * @param void
 * @return 成功则返回true，失败则返回false
 */
void COperationServerDlg::AnalysisProtocolOfBlasterSock(void)
{
	static unsigned char	pBuf[256];
	int		i,nSize;
	DWORD	dwFDUIP;
	CShotController*	pShotController=NULL;
	nSize = m_pBlasterSocket->Receive( pBuf,256);
	if(nSize==256)
	{
		// 解析出IP地址
		memcpy_s(&dwFDUIP,4,pBuf+16,4);
		for (i=0;i<OPERATION_THREAD_MAXIUM;i++)
		{
			if(dwFDUIP != m_pOperaThead[i]->m_pShotCtrl->m_dwFDUIP)
			    continue;
			// 解析数据帧，该函数要考虑FDU设置回令的帧格式
			pShotController=m_pOperaThead[i]->m_pShotCtrl;
			if(pShotController)
				pShotController->AnalysisFrame(pBuf,256);
			// 如果爆炸机状态更新，则发送给客户端
			if(pShotController->IsNewState())
			{
				m_pOperaThead[i]->SendBlasterState();
			}
		}
	}
}
/************************************************************************************************
    函    数：AnalysisProtocolOfClientSock
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 解析来自客户端的数据包
		
			   数据帧格式：
			   帧头（2Byte）＋帧长度（2Byte）  ＋帧计数（4Byte）＋帧类型（1Byte）			   
			   ＋命令(2Byte)＋数据个数（2Byte）＋数据区（变长） ＋校验位（1Byte）＋帧尾（2Byte）

    修改历史：

*************************************************************************************************/
void COperationServerDlg::AnalysisProtocolOfClientSock(void)
{
	static unsigned char	pBuf[32768];	// 数据缓冲区最大长度是发送采集排列的数据帧
	WORD			wFrmLen = 32768;		// 帧长: 初始化为缓冲区的大小，被ReceiveFrom函数使用
	WORD			wFrmCmdCode;			// 命令码
	DWORD			dwFrmIndex;				// 帧计数	
	int				nSize,nReadSize;
	DWORD			dwTemp;
	CString         strTemp;
	CShotClient*	pShotClient=NULL;

	if(!m_pClientSocket)
		return;
	sockaddr_in sockClient;
	sockClient.sin_family = AF_INET;
	nSize = sizeof(sockClient);
	// 接收来自客户端的数据
	nReadSize = m_pClientSocket->ReceiveFrom( pBuf, wFrmLen, (SOCKADDR*)&sockClient , &nSize );
	switch (nReadSize)
	{
	case 0:		// 没有收到数据 
		return;
		break;
	case SOCKET_ERROR:	// 发生错误，记录日志
		if (GetLastError() != WSAEWOULDBLOCK) 
		{  //发生socket错误
			strTemp=_T("Communication error with operation client！");
			WriteLogMsg(_T("COperationServerDlg::AnalysisProtocol"),strTemp,LOGTYPE_ERROR,FALSE);
			return;
		}
		break;
	default:
		break;
	}
	// -----------解析帧格式---------------------------------------------------------
	// 判断帧头是否0xEB, 0x90
	if (pBuf[0]!=0xEB || pBuf[1]!=0x90)
	{
		strTemp.Format(_T("Frame head(%d,%d)is wrong！"),pBuf[0],pBuf[1]);
		WriteLogMsg(_T("COperationServerDlg::AnalysisProtocol"),strTemp,LOGTYPE_ERROR,FALSE);
		return;
	}
	// 计算帧长度
	wFrmLen = MAKEWORD(pBuf[2],pBuf[3]);
// 	if(wFrmLen>32768)
// 	{	// 帧长度超长，发生错误记录日志		
// 		strTemp.Format(_T("Frame length(%d)is wrong！"),wFrmLen);
// 		WriteLogMsg(strTemp);		
// 		return;
// 	}
	// 计算帧计数
	memcpy_s(&dwFrmIndex,sizeof(dwFrmIndex),pBuf+4 , 4);
	// 帧尾
	if(pBuf[wFrmLen-2]!=0x14 || pBuf[wFrmLen-1]!=0x6F)
	{
		// 帧尾错误，需要重新定位帧头
		strTemp.Format(_T("Frame end(%d,%d)is wrong！"),pBuf[wFrmLen-2],pBuf[wFrmLen-1]);
		WriteLogMsg(_T("COperationServerDlg::AnalysisProtocol"),strTemp,LOGTYPE_ERROR,FALSE);
		return;
	}
	// -------------------解析命令码，按照命令码进行相应操作-------------------------------
	// 命令码
	wFrmCmdCode=MAKEWORD(pBuf[9],pBuf[10]);
	switch(wFrmCmdCode)
	{
	case OPERATION_CMD_REQUEST:	// 请求放炮服务帧
		{
			nReadSize=FindIdleThread();
			if(nReadSize < 0)	// 查找空闲线程
				return;
			// 帧类型
			pBuf[8] = 0xFF;
			memcpy_s(pBuf+29,4,&nReadSize,4);			
			m_pClientSocket->SendTo(pBuf,36,(SOCKADDR*)&sockClient,sizeof(sockaddr_in));
			
			strTemp.Format(_T("Receive shoting request: Thread ID# %d is Idle！"),nReadSize);
			WriteLogMsg(_T("COperationServerDlg::AnalysisProtocol"),strTemp,LOGTYPE_NORMAL,TRUE);
			return;
		}
		break;
	case OPERATION_CMD_CONFIRM:	// 确认放炮服务帧
		{
			// 空闲线程ID
			memcpy_s(&nSize,4,pBuf+29,4); 
			// 判断线程索引号是否在范围内
			if(nSize>=OPERATION_THREAD_MAXIUM || nSize<0)
				return;
			// 判断线程状态是否空闲
			if(m_pOperaThead[nSize]->GetThreadState()!=THREAD_IDLE)
			{	// 线程不是空闲，则不给回令	
				strTemp.Format(_T("Thread ID# %d is not Idle！"),nSize);
				WriteLogMsg(_T("COperationServerDlg::AnalysisProtocol"),strTemp,LOGTYPE_WARNING,TRUE);
				return ;
			}
			// 设置线程状态
			m_pOperaThead[nSize]->SetThreadState(THREAD_SHOT);
			pShotClient=new CShotClient;		// 此处创建（new）的对象 在线程对象中释放(COperaThread )
			if(!pShotClient)	
				return;
			// 保存客户端的信息:IP地址和端口号
			pShotClient->m_dwClientIP = sockClient.sin_addr.s_addr;
			pShotClient->m_wClientPort = ntohs(sockClient.sin_port);
			
			// 依据震源类型创建震源控制器
			m_pOperaThead[nSize]->SetShotClient(pShotClient);
			
			// 向客户端发送确认回令
			pBuf[8] = 0xFF;
			dwTemp = GetTickCount(); // 生成唯一的ID
			m_pOperaThead[nSize]->m_dwServerID =dwTemp;
			memcpy_s(pBuf+25,4,&dwTemp,4);
			m_pClientSocket->SendTo((unsigned char*)pBuf,nReadSize,(SOCKADDR*)&sockClient,sizeof(sockaddr_in));
			strTemp.Format(_T("Shot Confirmed: Thread ID# %d ！"),nSize);
			WriteLogMsg(_T("COperationServerDlg::AnalysisProtocol"),strTemp,LOGTYPE_NORMAL,TRUE);
			return;
			
		}
		break;
	case OPERATION_CMD_SHOT:	// 炮号及震源
		{
			AnalysisCmd_ShotPointAndSource(pBuf);	
		}
		break;
	case OPERATION_CMD_PROCESS:	// 收到处理方法
		{
			AnalysisCmd_ProcessType(pBuf);
		}
		break;
	case OPERATION_CMD_SPREAD:	// 收到采集排列
		{
			AnalysisCmd_SpreadLabel(pBuf);				
			
		}
		break;
	case OPERATION_CMD_CHANNEL:	// 收到采集排列所有采集道数据
		{
			AnalysisCmd_SpreadChannel(pBuf);				
		}
		break;
	case OPERATION_CMD_FDUCONF:	// 收到FDU配置信息
		{
			AnalysisCmd_FDUConfig(pBuf);				
		}
		break;
	case OPERATION_CMD_START:	// 准备开始放炮，触发点名事件
		{
			// 线程ID
			memcpy_s(&dwFrmIndex,4,pBuf+17,4);
			if(dwFrmIndex>=OPERATION_THREAD_MAXIUM || dwFrmIndex<0)
				return ;
			// 设置开始放炮事件
			m_pOperaThead[dwFrmIndex]->m_pShotEvent->SetEvent();
			pShotClient = m_pOperaThead[dwFrmIndex]->GetShotClient();
			strTemp.Format(_T("Starting Shot: Thread ID# %d,Shot Point ID# %d, Shot Source ID# %d ."),m_pOperaThead[dwFrmIndex]->GetShotThreadID(),
				pShotClient->m_pShotPoint->m_dwShotNb,pShotClient->m_pShotSource->m_dwSourceNb);
			WriteLogMsg(_T("COperationServerDlg::AnalysisProtocol"),strTemp,LOGTYPE_NORMAL,TRUE);			
		}
		break;
	case OPERATION_CMD_SHOTGO:		// 开始放炮服务帧,用户点击“GO”
		{
			// 线程ID
			memcpy_s(&dwFrmIndex,4,pBuf+17,4);
			if(dwFrmIndex>=OPERATION_THREAD_MAXIUM || dwFrmIndex<0)
				return ;
			// 设置Go事件
			m_pOperaThead[dwFrmIndex]->m_pShotGOEvent->SetEvent();
			pShotClient = m_pOperaThead[dwFrmIndex]->GetShotClient();
			strTemp.Format(_T("Starting Receiving: Shot Point ID# %d, Shot Source ID# %d."),pShotClient->m_pShotPoint->m_dwShotNb,pShotClient->m_pShotSource->m_dwSourceNb);
			WriteLogMsg(_T("COperationServerDlg::AnalysisProtocol"),strTemp,LOGTYPE_NORMAL,TRUE);
		}
		break;
	case OPERATION_CMD_PROCGO:	// 采集结束，准备处理数据，将原始数据转化为SEGD码
		{
			// 线程ID
			memcpy_s(&dwFrmIndex,4,pBuf+17,4);
			if(dwFrmIndex>=OPERATION_THREAD_MAXIUM || dwFrmIndex<0)
				return ;
			// 设置开始处理事件，将原始数据处理为SEGD格式
			m_pOperaThead[dwFrmIndex]->m_pProcessEvent->SetEvent();
			pShotClient = m_pOperaThead[dwFrmIndex]->GetShotClient();
			strTemp.Format(_T("Start processing: Shot Point ID# %d, Shot Source ID# %d."),pShotClient->m_pShotPoint->m_dwShotNb,pShotClient->m_pShotSource->m_dwSourceNb);
			WriteLogMsg(_T("COperationServerDlg::AnalysisProtocol"),strTemp,LOGTYPE_NORMAL,TRUE);
		}
		break;
	case OPERATION_CMD_AGAIN:	// 停止当前放炮，恢复到初始放炮状态，重新放这一炮。
		{
			// 线程ID
			memcpy_s(&dwFrmIndex,4,pBuf+17,4);
			if(dwFrmIndex>=OPERATION_THREAD_MAXIUM || dwFrmIndex<0)
				return ;
			// 停止放炮
			m_pOperaThead[dwFrmIndex]->RestartShot();
			pShotClient = m_pOperaThead[dwFrmIndex]->GetShotClient();			
			if(pShotClient)
			{
				strTemp.Format(_T("Reset shot and Re-shooting: Shot Point ID# %d, Shot Source ID# %d!"),pShotClient->m_pShotPoint->m_dwShotNb,pShotClient->m_pShotSource->m_dwSourceNb);
			}
						
			WriteLogMsg(_T("COperationServerDlg::AnalysisProtocol"),strTemp,LOGTYPE_NORMAL,TRUE);
			// 设置开始放炮事件
			m_pOperaThead[dwFrmIndex]->m_pShotEvent->SetEvent();
		}
		break;
	case OPERATION_CMD_CANCEL:	// 停止放炮服务帧,发生ITB错误，用户取消，或者按下Stop 或者Abort按钮，终止放炮
		{
			// 线程ID
			memcpy_s(&dwFrmIndex,4,pBuf+17,4);
			if(dwFrmIndex>=OPERATION_THREAD_MAXIUM || dwFrmIndex<0)
				return ;
			m_pOperaThead[dwFrmIndex]->m_pShotStopEvent->SetEvent();
			m_pOperaThead[dwFrmIndex]->SetThreadState(THREAD_IDLE);		// 释放线程，置线程状态为空
			pShotClient = m_pOperaThead[dwFrmIndex]->GetShotClient();
			if(pShotClient)
			{
				strTemp.Format(_T("Stop Shot: Shot Point ID# %d, Shot Source ID# %d!"),pShotClient->m_pShotPoint->m_dwShotNb,pShotClient->m_pShotSource->m_dwSourceNb);				
			}
			WriteLogMsg(_T("COperationServerDlg::AnalysisProtocol"),strTemp,LOGTYPE_NORMAL,TRUE);
						
		}
		break;
	default:					// 错误命令码
		{
			strTemp.Format(_T("Frame command code ( %d ) is wrong!"),wFrmCmdCode);
			WriteLogMsg(_T("COperationServerDlg::AnalysisProtocol"),strTemp,LOGTYPE_WARNING,TRUE);			
		}
		return;
		break;
	}

}

/**
 * @brief 解析采集站的基本配置
 * @note  从数据帧中解析出采集站的基本配置信息和数据文件存盘路径，主要
 包括：前置放大、模拟门选择、5373模式、DA衰减、采样率、IIR选择、FIR选择
 * @param unsigned char* pBuf，数据帧缓冲区
 * @return 成功则返回true，失败则返回false
 */
bool  COperationServerDlg::AnalysisCmd_FDUConfig(unsigned char* pBuf)
{

	DWORD  dwThreadID;
	CShotClient* pShotClient = NULL;
	// 线程ID
	memcpy_s(&dwThreadID,4,pBuf+17,4);
	if(dwThreadID>=OPERATION_THREAD_MAXIUM || dwThreadID<0)
		return false;
	pShotClient=m_pOperaThead[dwThreadID]->GetShotClient();
	// 采集站属性配置
	CFDUConfig* pFDUConfig = new CFDUConfig;
	memcpy_s(&pFDUConfig->m_dwGain3301Index,4,pBuf+21,4);	
	memcpy_s(&pFDUConfig->m_dwMux3301Index,4,pBuf+25,4);	
	memcpy_s(&pFDUConfig->m_dwMode5373Index,4,pBuf+29,4);
	memcpy_s(&pFDUConfig->m_dwAttr5373Index,4,pBuf+33,4);
	memcpy_s(&pFDUConfig->m_dwSampleRateIndex,4,pBuf+37,4);
	memcpy_s(&pFDUConfig->m_dwFilterIIRIndex,4,pBuf+41,4);
	memcpy_s(&pFDUConfig->m_dwFilterFIRIndex,4,pBuf+45,4);
	pShotClient->m_pFDUConfig = pFDUConfig;

	// 数据文件存盘路径
	pShotClient->m_strNormalFilePath = LPCTSTR(pBuf+49);
	int nStrLen=pShotClient->m_strNormalFilePath.GetLength()*sizeof(TCHAR);
	pShotClient->m_strTestFilePath = LPCTSTR(pBuf+51+nStrLen);
	// 创建文件目录
	CString strTemp;
	if(!FolderExist(pShotClient->m_strNormalFilePath))
	{	// 创建数据文件存盘目录，将SEGD文件编号复位
		theApp.SetGlobalFileNumber(SEGDFILENUMBER_INIT);
		if(!CreateMultipleDirectory(pShotClient->m_strNormalFilePath))
		{
			strTemp.Format(_T("Thread ID# %d: Normal File directory  %s Failed to create!"),dwThreadID,pShotClient->m_strNormalFilePath);
			WriteLogMsg(_T("COperationServerDlg::AnalysisProtocol"),strTemp,LOGTYPE_ERROR,TRUE);
		}		
	}
	// 
	if(!FolderExist(pShotClient->m_strTestFilePath))
	{	// 创建测试文件存盘路径，将SEGD文件编号复位
		theApp.SetGlobalFileNumber(SEGDFILENUMBER_INIT);
		if(!CreateMultipleDirectory(pShotClient->m_strTestFilePath))
		{
			strTemp.Format(_T("Thread ID# %d: Normal Test directory %s Failed to create!"),dwThreadID,pShotClient->m_strTestFilePath);
			WriteLogMsg(_T("COperationServerDlg::AnalysisProtocol"),strTemp,LOGTYPE_ERROR,TRUE);
		}		
	}
	strTemp.Format(_T("Received FDU Configuration: Thread ID# %d, Shot Point ID# %d, Shot Source ID# %d."),dwThreadID,pShotClient->m_pShotPoint->m_dwShotNb,pShotClient->m_pShotSource->m_dwSourceNb);
	WriteLogMsg(_T("COperationServerDlg::AnalysisCmd_FDUConfig"),strTemp,LOGTYPE_NORMAL,TRUE);
	return true;
}

/**
 * @brief 解析采集排列的基本属性
 * @note  发送采集排列时，先发送采集排列的基本信息：排列号、标签、类型等。
 该函数从数据帧中解析出采集排列的基本属性。
 * @param unsigned char* pBuf，数据帧缓冲区
 * @return 成功则返回true，失败则返回false
 */
bool  COperationServerDlg::AnalysisCmd_SpreadLabel(unsigned char* pBuf)
{
	DWORD  i,dwThreadID;
	CShotClient* pShotClient = NULL;
	// 线程ID
	memcpy_s(&dwThreadID,4,pBuf+17,4);
	if(dwThreadID>=OPERATION_THREAD_MAXIUM || dwThreadID<0)
		return false;
	pShotClient=m_pOperaThead[dwThreadID]->GetShotClient();
	// 创建采集对象，保存排列号、标签、类型等
	pShotClient ->m_pSpread = new CShotSpread;
	// 采集排列ID
	memcpy_s(&dwThreadID,4,pBuf+21,4);
	pShotClient->m_pSpread->m_dwNb = dwThreadID;
	pShotClient->m_pSpread->m_bySpreadType = pBuf[25];
	pShotClient->m_pSpread->m_strLabel = LPCTSTR(pBuf+26);
	
	// 将辅助道增加到采集排列中
	int wLabelLen=pShotClient->m_pSpread->m_strLabel.GetLength()*sizeof(TCHAR);
	//dwThreadID = m_AuxChannels.GetCount();
	// CAuxChannel* pAuxChn = NULL;
	WORD  wAuxiCount = MAKEWORD(pBuf[28+wLabelLen],pBuf[29+wLabelLen]);
	pShotClient->m_pSpread->SetAuxiCount(wAuxiCount);
	SHOTSPREAD sSpread;
	for(i=0;i<wAuxiCount;i++) 
	{
		// 对于辅助道而言，线号保存辅助道的编号，点号保存SN，道号仍然保存道号
		memcpy_s(&sSpread.m_dwLineNb,4,pBuf+30+wLabelLen+i*20,4);
		memcpy_s(&sSpread.m_dwPointNb,4,pBuf+34+wLabelLen+i*20,4);
		memcpy_s(&sSpread.m_dwChannelNb,4,pBuf+38+wLabelLen+i*20,4);
		memcpy_s(&sSpread.m_dwIP,4,pBuf+42+wLabelLen+i*20,4);
		memcpy_s(&sSpread.m_dwGain,4,pBuf+46+wLabelLen+i*20,4);			
		sSpread.m_byType = CHN_AUXI;		
		pShotClient->m_pSpread->Append(sSpread);
	}
	CString strTemp;
	strTemp.Format(_T("Starting Received Spread #%d ,Shot Point ID# %d, Shot Source ID# %d."),pShotClient->m_pSpread->m_dwNb,pShotClient->m_pShotPoint->m_dwShotNb,pShotClient->m_pShotSource->m_dwSourceNb);
	WriteLogMsg(_T("COperationServerDlg::AnalysisCmd_SpreadLabel"),strTemp,LOGTYPE_NORMAL,TRUE);
	return true;
}
/**
 * @brief 解析采集排列的采集道
 * @note  发送采集排列时，先发送采集排列的采集道信息，第11和12字节保存采集道个数，数据区依次
 保存采集道数据。
 * @param unsigned char* pBuf，数据帧缓冲区
 * @return 成功则返回true，失败则返回false
 */
int  COperationServerDlg::AnalysisCmd_SpreadChannel(unsigned char* pBuf)
{
	int		i;
	DWORD	nThreadIndex;
	DWORD	dwTemp;

	// 线程ID
	memcpy_s(&nThreadIndex,4,pBuf+17,4);
	// 判断线程索引号是否在范围内
	if(nThreadIndex>=OPERATION_THREAD_MAXIUM || nThreadIndex<0)
		return 0;
	// 通信ID，m_dwServerID
	memcpy_s(&dwTemp,4,pBuf+13,4);
	if(dwTemp != m_pOperaThead[nThreadIndex]->m_dwServerID)
		return 0;
	
	SHOTSPREAD sSpread;
	CShotClient* pShotClient=m_pOperaThead[nThreadIndex]->GetShotClient();
	WORD  wFDUCount;
	// 计算采集道的个数
	wFDUCount = MAKEWORD(pBuf[11],pBuf[12]);
	// wFDUCount = (wFDUCount-8)/9;
	for (i=0;i<wFDUCount;i++)
	{
		memcpy_s(&sSpread.m_dwIP,4,pBuf+21+21*i,4);
		memcpy_s(&sSpread.m_dwGain,4,pBuf+25+21*i,4);
		memcpy_s(&sSpread.m_dwLineNb,4,pBuf+29+21*i,4);
		memcpy_s(&sSpread.m_dwPointNb,4,pBuf+33+21*i,4);
		memcpy_s(&sSpread.m_dwChannelNb,4,pBuf+37+21*i,4);			
		sSpread.m_byType=pBuf[41+21*i];
		pShotClient->m_pSpread->Append(sSpread);
	}
	CString strTemp;
	strTemp.Format(_T("End of reception Spread #%d ,Shot Point ID# %d, Shot Source ID# %d."),pShotClient->m_pSpread->m_dwNb,pShotClient->m_pShotPoint->m_dwShotNb,pShotClient->m_pShotSource->m_dwSourceNb);
	WriteLogMsg(_T("COperationServerDlg::AnalysisCmd_SpreadLabel"),strTemp,LOGTYPE_NORMAL,TRUE);
	return wFDUCount;
}

/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能：

    修改历史：

*******************************************************************************/
int  COperationServerDlg::AnalysisCmd_ShotPointAndSource(unsigned char* pBuf)
{
	int    nThreadIndex;
	DWORD  dwTemp;

	// 线程ID
	memcpy_s(&nThreadIndex,4,pBuf+17,4);
	// 判断线程索引号是否在范围内
	if(nThreadIndex>=OPERATION_THREAD_MAXIUM || nThreadIndex<0)
		return 0;
	
	CShotClient* pShotClient=m_pOperaThead[nThreadIndex]->GetShotClient();
	if(!pShotClient)
		return 0;

	int nPointLen ;
	int nSourceLen;
	
	// 创建炮点对象
	CShotPoint* pShotPoint = new CShotPoint;
	CShotSource* pShotSource = NULL;
	pShotClient->m_pShotPoint = pShotPoint;
	// 恢复CShotPoint对象		
	pShotPoint->m_VPState = pBuf[21];
	pShotPoint->m_byBreakPoint = pBuf[22];
	memcpy_s(&(pShotPoint->m_dwShotNb),4,pBuf+23,4);
	memcpy_s(&(pShotPoint->m_dwSwathNb),4,pBuf+27,4);
	memcpy_s(&(pShotPoint->m_dwSourcePointIndex),4,pBuf+31,4);
	memcpy_s(&(pShotPoint->m_fSourceLine),4,pBuf+35,4);
	memcpy_s(&(pShotPoint->m_fSourceNb),4,pBuf+39,4);
	memcpy_s(&(pShotPoint->m_dwSpreadSFL),4,pBuf+43,4);
	memcpy_s(&(pShotPoint->m_dwSpreadSFN),4,pBuf+47,4);
	memcpy_s(&(pShotPoint->m_dwSpreadNb),4,pBuf+51,4);
	memcpy_s(&(pShotPoint->m_dwSuperSpread),4,pBuf+55,4);
	memcpy_s(&(pShotPoint->m_dwProcessNb),4,pBuf+59,4);
	pShotPoint->m_szComments =LPCTSTR(pBuf+63);
	nPointLen = pShotPoint->m_szComments.GetLength()*sizeof(TCHAR);
	// 创建震源CShotSource对象
	// 判断震源类型
	memcpy_s(&(dwTemp),4,pBuf+65+nPointLen,4);
	if(dwTemp==SHOTTYPE_EXPLO)
	{
		pShotSource = new CShotSourceExplo;
	}
	else
	{
		pShotSource = new CShotSourceVibro;
	}
	pShotClient->m_pShotSource = pShotSource;	
	// 类型
	pShotSource->m_dwSourceType = dwTemp;
	pShotSource->m_bySourceState=pBuf[69+nPointLen];
	memcpy_s(&(pShotSource->m_dwSourceNb),4,pBuf+70+nPointLen,4);
	memcpy_s(&(pShotSource->m_dwShooterNb),4,pBuf+74+nPointLen,4);
	memcpy_s(&(pShotSource->m_lStep),4,pBuf+78+nPointLen,4);
	memcpy_s(&(pShotSource->m_dwSourceIndex),4,pBuf+82+nPointLen,4);
	memcpy_s(&(pShotSource->m_dwReadyVPNb),4,pBuf+86+nPointLen,4);
	memcpy_s(&(pShotSource->m_bReady),4,pBuf+90+nPointLen,4);
	pShotSource->m_strLabel = LPCTSTR(pBuf+94+nPointLen);
	nSourceLen=pShotSource->m_strLabel.GetLength()*sizeof(TCHAR);
	pShotSource->m_strComment = LPCTSTR(pBuf+96+nPointLen+nSourceLen);
	// 计算帧长
	dwTemp = MAKEWORD(pBuf[2],pBuf[3]);
	if(pShotSource->m_dwSourceType==SHOTTYPE_VIBRO)
	{
		CShotSourceVibro* pVibroSrc = (CShotSourceVibro*)pShotSource;
		pVibroSrc->m_byMoving = pBuf[dwTemp-12];
		memcpy_s(&(pVibroSrc->m_bWorkByAcq),4,pBuf+dwTemp-11,4);
		memcpy_s(&(pVibroSrc->m_dwClusterNb),4,pBuf+dwTemp-7,4);
	}
	// 记录日志信息
	CString strTemp;
	strTemp.Format(_T("Received Shot Point and Source: Shot Point ID# %d, Shot Source ID# %d."),pShotPoint->m_dwShotNb,pShotSource->m_dwSourceNb);
	WriteLogMsg(_T("COperationServerDlg::AnalysisProtocol"),strTemp,LOGTYPE_NORMAL,TRUE);
	return 1;
}
/**
 * @brief 解析处理类型
 * @note  不同的数据处理类型采用不同的类实现，都继承基类的ReadFromBuf
 函数。因此先判断数据处理类型，再依据类型创建不同的对象，调用ReadFromBuf函数。
 * @param  unsigned char* pBuf，数据缓冲区
 * @return 失败返回0，成功返回非0
 */
int  COperationServerDlg::AnalysisCmd_ProcessType(unsigned char* pBuf)
{
	int    nThreadIndex;

	// 线程ID
	memcpy_s(&nThreadIndex,4,pBuf+17,4);
	// 判断线程索引号是否在范围内
	if(nThreadIndex>=OPERATION_THREAD_MAXIUM || nThreadIndex<0)
		return 0;

	if(!m_pOperaThead[nThreadIndex]->GetShotClient())
		return 0;

	// 根据类型创建对象
	CProcessType* pProcessType=NULL;
	switch(pBuf[21])
	{
	case PROCESS_IMPULSIVE:
		pProcessType = new CProcessImpulsive;
		pProcessType->m_byProcessType = PROCESS_IMPULSIVE;
		break;
	default:
		pProcessType = new CProcessImpulsive;
		pProcessType->m_byProcessType = PROCESS_IMPULSIVE;
		break;
	}
	// 调用SetShotProcess函数，设置处理类型，同时更新爆炸机控制器所连采集站SN号
	m_pOperaThead[nThreadIndex]->SetShotProcess(pProcessType);
	// m_pOperaThead[nThreadIndex]->m_pShotClient->m_pProcessType = pProcessType;
	pProcessType->ReadFromBuf(pBuf+21);
	// 记录日志信息
	CString strTemp;
	strTemp.Format(_T("Received Process Type: Process Type ID# %d ."),pProcessType->m_dwProcessNb);
	WriteLogMsg(_T("COperationServerDlg::AnalysisProtocol"),strTemp,LOGTYPE_NORMAL,TRUE);
	return 1;
}
/**
 * @brief 日志记录
 * @note  将系统运行中各种情况记录在日志文件中。
 * @param  LPCTSTR strMsg，日志信息
 * @param  int nType，日志类别
 * @return void

void COperationServerDlg::WriteLogMsg(LPCTSTR strMsg,int nType)
{
	m_OperationLog.WriteLog(strMsg,nType);
	return ;
} */
/**
 * @brief 日志管理类
 * @note  程序的所有运行记录通过该函数发送出去，由AdminServer程序接收后再通知客户端。
 * @param  LPCTSTR strPosition,日志发生的位置描述
 * @param LPCTSTR strDesc,日志描述
 * @param WORD wType,日志类型：正常、异常、警告、调试等
 * @param BOOL bShowed，是否显示给用户
 * @return 成功则返回true，失败则返回false
 */
void    COperationServerDlg::WriteLogMsg(LPCTSTR strPosition,LPCTSTR strDesc,WORD wType,BOOL bShowed)
{
	CLogBase	ProgramLog;
	if(!m_pClientSocket)
		return;
	ProgramLog.SetHostAndProgram(theApp.m_strHostIP,MATRIX_OPERATIONSERVER);
	ProgramLog.SetLog(strPosition,strDesc,wType,bShowed);
	// 向服务器发送日志，由服务器记录日志信息
	BYTE    pBuf[SOCKET_LOGFRM_MAXLEN];
	int nSize = ProgramLog.WriteToSocketFrame(pBuf);
	m_pClientSocket->SendTo(pBuf,nSize,ADMINSERVER_LOGPORT,_T("255.255.255.255"));	
}