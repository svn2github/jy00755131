#include "StdAfx.h"
#include "OperationServer.h"
#include "OperaThread.h"
#include "..\common\\FileSEGD.h"

extern UINT __cdecl ShotControlProc(LPVOID lpParameter);
extern UINT __cdecl ShotReceiveProc(LPVOID lpParameter);
extern UINT __cdecl ShotProcessProc(LPVOID lpParameter);
extern UINT __cdecl ShotOutputProc(LPVOID lpParameter);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////    CRevBuffer      ///////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief CRevBuffer构造函数

 * 初始化数据缓冲区
 */
CRevBuffer::CRevBuffer()
{
	memset(m_RevFlagBuf,0,FDU_MAXINUM);
	m_bAllRev = FALSE;

}
/**
 * @brief CRevBuffer析构函数
 */
CRevBuffer::~CRevBuffer()
{

}
/**
 * @brief 清除指定位置缓冲区数据
 * @note  将指定采集道的缓冲区数据清零，一般用于处理哑道的数据。
 * @param  int nIndex，数据位置，与采集排列中采集站数组的位置一致
 * @return 如果成功，则返回true；如果失败，则返回false。
 */
void CRevBuffer::ClearBuf(int nIndex)
{
	memset(&(m_RevBuf[nIndex][0]),0,SAMPFRM_DATALEN);
	// 仍然置数据到达标志
	m_RevFlagBuf[nIndex] = 1;
}
/**
 * @brief 写数据。
 * @note  将采样接收的数据写入数据缓冲区中。
 * @param  int nIndex，应保存的数据位置，与采集排列中采集站数组的位置一致
 * @param  unsigned char* pBuf，采样数据
 * @param  int nSize，pBuf数据字节长度
 * @return 如果成功，则返回true；如果失败，则返回false。
 */
bool CRevBuffer::WriteData(int nIndex,unsigned char* pBuf,int nSize)
{ 
	if(nSize>SAMPFRM_DATALEN)
		memcpy(&(m_RevBuf[nIndex][0]),pBuf,SAMPFRM_DATALEN);
	else
		memcpy(&(m_RevBuf[nIndex][0]),pBuf,nSize);
	m_RevFlagBuf[nIndex] = 1;
	return true;
}
/**
 * @brief 读缓冲区数据。
 * @note  从缓冲区中读取数据，写入指定的位置。
 * @param  int nIndex，应读取的数据位置，与采集排列中采集站数组的位置一致
 * @param  unsigned char* pBuf，保存读取的数据
 * @param  int nSize，pBuf数据字节长度
 * @return 返回读取的数据长度
 */
int CRevBuffer::ReadData(int nIndex,unsigned char* pBuf,int nSize)
{ 
	if(nSize>SAMPFRM_DATALEN)
		nSize =SAMPFRM_DATALEN;		
	memcpy(pBuf, &(m_RevBuf[nIndex][0]),nSize);
	return nSize;
}
/**
 * @brief 清除缓冲区
 * @note  从缓冲区中写入文件中。
 * @param  CFile* pFile，文件指针
 * @param  int nFDUCount，写入的采集站总数，小于 FDU_MAXINUM
 * @return 写入成功则返回1；失败则返回错误代码
 */
void CRevBuffer::ResetBuf(int nFDUCount)
{ 
	memset(m_RevBuf,0xFF,nFDUCount * SAMPFRM_DATALEN);
	memset(m_RevFlagBuf,0,FDU_MAXINUM);
	m_bAllRev=FALSE; 
}
/**
 * @brief 存储到文件
 * @note  从缓冲区中写入文件中。
 * @param  CFile* pFile，文件指针
 * @param  int nFDUCount，写入的采集站总数，小于 FDU_MAXINUM
 * @return 写入成功则返回1；失败则返回错误代码
 */
int  CRevBuffer::SaveToFile(CFile* pFile,int nFDUCount)
{
	if(pFile==NULL)
		return 0;
	try
	{
		pFile->Write(m_RevBuf,nFDUCount * SAMPFRM_DATALEN);
		return 1;
	}
	catch (CFileException* e)
	{
		TRACE0(" 1 RevFile.Close()");
		return e->m_cause;
	}	
}
/**
 * @brief 把缓冲区的数据发送道客户端
 * @note  将当前缓冲区的数据发送到显示界面，供用户显示。
 * @param  CSocket*	pSocket，Socket指针
 * @return 写入成功则返回1；失败则返回错误代码
 */
int  CRevBuffer::SendToClient(CSocket*	pSocket,int nFDUIndex)
{
	unsigned char* pBuf = m_pBuf;
	if(!pSocket)
		return 0;
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
	// 空闲线程索引号			// ????????????? 临时调试状态，与详细设计文档不一致
	memset(pBuf+13,0,4);
	memset(pBuf+17,0,4);

	memcpy_s(pBuf+21,222,m_RevBuf[nFDUIndex],222);
	
	// 帧校验
	pBuf[243]=0;
	// 帧尾
	pBuf[244] = 0x14;
	pBuf[245] = 0x6F;

	return pSocket->SendTo(pBuf,246,0xDF00,_T("255.255.255.255"));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////    CShotTime    ///////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CShotTime::CShotTime()
{
	m_dwStartTBH = 0;
	m_dwStartTBL = 0;
	//m_fConfirmedTB = 0;
	m_dwEndTEH = 0;
	m_dwEndTEL = 0;

}
CShotTime::~CShotTime()
{

}

/**
 * @brief 测网的时间计算公式
 * @note  在测网系统当前时间dwFDUCurrentTime的基础上，加上以ms为单位的时间dwmsAddTime；
 返回测网系统新的系统时间高位dwFDUTimeH和低位dwFDUTimeL
 * @param  DWORD dwFDUCurrentTime,
 * @param  DWORD dwmsAddTime,
 * @param  DWORD& dwFDUTimeH,
 * @param  DWORD& dwFDUTimeL
 * @return void
 */
void	CShotTime::GetFDUTimeHighAndTimeLow(DWORD dwFDUCurrentTime,DWORD dwmsAddTime,DWORD& dwFDUTimeH,DWORD& dwFDUTimeL)
{	
	// 增加的毫秒时间左移16位
	dwmsAddTime= dwmsAddTime << 16;
	dwFDUTimeH= dwFDUCurrentTime + dwmsAddTime/0x3fff;
	dwFDUTimeL= dwmsAddTime%0x3fff;
	return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////    COperaThread    ///////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief COperaThread构造函数
 */
COperaThread::COperaThread(void)
: m_pShotThread(NULL), m_pReceiveThread(NULL)
, m_pProcessThread(NULL), m_pOutputThread(NULL)
, m_ThreadState(THREAD_IDLE)
, m_pShotCtrl(0) 
, m_dwLocalIP(0)
, m_pExitEvent(NULL), m_pShotEvent(NULL),m_pShotGOEvent(NULL), m_pShotStopEvent(NULL)
, m_pRevEvent(NULL),m_pRevEndEvent(NULL),m_pProcessEvent(NULL),m_pOutputEvent(NULL)
, m_dwThreadID(1)
, m_pShotClient(NULL)
, m_ShotTime()
, m_pFDUSocket(NULL)
, m_pClientSocket(NULL)
, m_dwServerID(0)
{	
}
/**
 * @brief COperaThread析构函数
 */
COperaThread::~COperaThread(void)
{	
	if(m_pShotThread)
	{
		TerminateThread(m_pShotThread,0);
		delete m_pShotThread;
		m_pShotThread =NULL;
	}
	if(m_pReceiveThread)
	{
		TerminateThread(m_pReceiveThread,0);
		delete m_pReceiveThread;
		m_pReceiveThread =NULL;
	}
	if(m_pProcessThread)
	{
		TerminateThread(m_pProcessThread,0);
		delete m_pProcessThread;
		m_pProcessThread =NULL;
	}
	if(m_pOutputThread)
	{
		TerminateThread(m_pOutputThread,0);
		delete m_pOutputThread;
		m_pOutputThread =NULL;
	}
	if(m_pExitEvent)
	{
		delete m_pExitEvent;
		m_pExitEvent = NULL;
	}
	if(m_pShotEvent)
	{
		delete m_pShotEvent;
		m_pShotEvent = NULL;
	}
	if(m_pShotGOEvent)
	{
		delete m_pShotGOEvent;
		m_pShotGOEvent = NULL;
	}
	if(m_pShotStopEvent)
	{
		delete m_pShotStopEvent;
		m_pShotStopEvent = NULL;
	}
	if(m_pRevEvent)
	{
		delete m_pRevEvent;
		m_pRevEvent = NULL;
	}
	if(m_pRevEndEvent)
	{
		delete m_pRevEndEvent;
		m_pRevEndEvent = NULL;
	}
	if(m_pProcessEvent)
	{
		delete m_pProcessEvent;
		m_pProcessEvent = NULL;
	}
	if(m_pOutputEvent)
	{
		delete m_pOutputEvent;
		m_pOutputEvent = NULL;
	}
}
/**
 * @brief 创建对象
 * @note  在申明管理对象后，调用该函数初始化参数，打开网络端口，
 并启动放炮、数据采集、数据处理和数据IO四个线程。
 * @param  WORD wThreadID，线程ID，程序可以创建多个COperaThread实例，每个实例必须有一个编号，
 用于唯一标识该组线程。另一个含义是对需要用到的网络通信端口自动加上该ID值，避免多组线程
 同时打开一个网络端口发生错误。
 * @param  DWORD dwHostIP，当前施工服务器的IP地址。
 * @param  DWORD dwHostIP，当前施工服务器的IP地址。
 * @param  DWORD dwMainCrossIP，主交叉站的IP地址。
 * @param  WORD wMainCrossPort，主交叉站的端口。
 * @return 创建成功则返回1；失败则false。
 */
bool COperaThread::Create(WORD wThreadID,DWORD dwHostIP,CSocketShotClient* pClientSocket,CSocketShotBlaster* pBlasterSocket)
{
	int i;
	m_dwThreadID = wThreadID;
	m_dwLocalIP = dwHostIP;
	m_pClientSocket= pClientSocket;	
	wThreadID=wThreadID<<8;
	for(i=0;i<8;i++)
	{
		m_wRevPort[i]= SOCKET_REV8PORT + wThreadID + i;
	}
	// 创建与采集站通信的命令端口
	CreateFDUSocket(dwHostIP, SOCKET_FDUPORT + (WORD)m_dwThreadID);
	// 创建与爆炸机控制器所连的采集站通信的对象实例，目前仅支持ShotProII爆炸机 // ????????????????
    m_pShotCtrl = new CShotCtrlProII;
	m_pShotCtrl->m_pHostSocket = pBlasterSocket;
	return StartThread();
	
}

/**
 * @brief 释放对象
 * @note  在需要释放该对象时，调用该函数通知四个线程退出，关闭网络接口，释放放炮所涉及的对象。
 * @param  void
 * @return void
 */
void COperaThread::Destory(void)
{
	EndThread();
	if(m_pFDUSocket)
	{
		m_pFDUSocket->CloseSocket();
		delete m_pFDUSocket;
		m_pFDUSocket =NULL;
	}
	ResetShotParam();
	if(m_pShotCtrl)
	{
		delete m_pShotCtrl;
		m_pShotCtrl = NULL;
	}	
	return ;
}
/**
 * @brief 设置放炮参数对象
 * @note  在每次放炮前，调用该函数，设置当前要放炮的参数信息，包括炮点、震源、采集排列、处理类型、
 爆炸机控制器以及与客户端的连接。
 * @param  CShotClient* pShotClient,封装炮点、震源、采集排列、处理类型的对象。
 * @param  CShotController* pShotCtrl,爆炸机控制器对象
 * @param  CSocketShotClient* pClientSocket，与客户端的连接对象
 * @return void
 */
void COperaThread::SetShotClient( CShotClient* pShotClient)
{
	// 先清除前一次放炮数据
	ResetShotParam();
	m_pShotClient = pShotClient;		
	return ;
}
/**
 * @brief 设置数据处理类型，初始化爆炸机控制器
 * @note  在程序收到炮点对应的处理类型后，保存该对象，并从该对象中取出爆炸机控制器所连的采集站，并初始化采集站。
 * @param  CProcessType* pProcessType,数据处理类型对象，不能为NULL
 * @return void。
 */
void COperaThread::SetShotProcess( CProcessType* pProcessType)
{
	if(m_pShotClient)
	{
		m_pShotClient->m_pProcessType = pProcessType;
		m_pShotCtrl->Reset();
		m_pShotCtrl->SetFDUSN(m_pShotClient->m_pProcessType->m_dwFDUSN);
	}
}
/**
 * @brief 重新启动此次放炮参数对象
 * @note  用户中途重新再开始此次放炮，本函数恢复放炮管理对象的初始状态。
 * @param  void
 * @return 释放成功返回true；释放失败则返回false。
 */
bool COperaThread::RestartShot()
{
	if(!m_pShotClient)
		return false;
	m_pShotStopEvent->SetEvent();
	SetThreadState(THREAD_SHOT);
	
	m_pShotCtrl->Reset();
	m_pShotCtrl->SetFDUSN(m_pShotClient->m_pProcessType->m_dwFDUSN);
	
	return true;
}
/**
 * @brief 释放放炮参数对象
 * @note  在每次放炮结束后或者程序退出前，调用该函数，释放当前放炮的参数信息，包括炮点、震源、采集排列、处理类型、
 爆炸机控制器以及与客户端的连接。
 * @param  void
 * @return 释放成功返回true；释放失败则返回false。
 */
bool COperaThread::ResetShotParam()
{
	if(m_pShotClient)
	{
		delete m_pShotClient;
		m_pShotClient = NULL;
	}	
    return true;
}
/**
 * @brief 启动线程
 * @note  该函数被Create函数调用，创建并启动四个线程。
 * @param  void
 * @return 成功返回true；失败则返回false。
 */
bool COperaThread::StartThread(void)
{
	try
	{
		if(m_pShotThread || m_pReceiveThread || m_pProcessThread || m_pOutputThread )
			return false;
		if(m_pExitEvent)
		{
			delete m_pExitEvent;
			m_pExitEvent = NULL;
		}
		m_pExitEvent = new CEvent(FALSE,TRUE);
		m_pShotEvent = new CEvent(FALSE,FALSE);
		m_pShotGOEvent = new CEvent(FALSE,FALSE);
		m_pShotStopEvent =  new CEvent(FALSE,TRUE);
		m_pRevEvent = new CEvent(FALSE,FALSE);
		m_pRevEndEvent = new CEvent(FALSE,FALSE);
		m_pProcessEvent = new CEvent(FALSE,FALSE);
		m_pOutputEvent = new CEvent(FALSE,FALSE);

		// 创建放炮控制线程
		m_pShotThread = AfxBeginThread(&ShotControlProc,this, 0, 0, CREATE_SUSPENDED, NULL);
		m_pShotThread->m_bAutoDelete = FALSE;
		m_pShotThread->ResumeThread();
		// 创建数据接收线程
		m_pReceiveThread = AfxBeginThread(&ShotReceiveProc,this, 0, 0, CREATE_SUSPENDED, NULL);
		m_pReceiveThread->m_bAutoDelete = FALSE;
		m_pReceiveThread->ResumeThread();
		// 创建数据处理线程
		m_pProcessThread = AfxBeginThread(&ShotProcessProc,this, 0, 0, CREATE_SUSPENDED, NULL);
		m_pProcessThread->m_bAutoDelete = FALSE;
		m_pProcessThread->ResumeThread();
		// 创建数据存储及打印输出线程
		m_pOutputThread = AfxBeginThread(&ShotOutputProc,this, 0, 0, CREATE_SUSPENDED, NULL);
		m_pOutputThread->m_bAutoDelete = FALSE;
		m_pOutputThread->ResumeThread();
		
		return true;
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause,255);		
		return false;
	}
	
}

/**
 * @brief 退出线程
 * @note  该函数被Destory函数调用，通知四个线程退出。
 * @param  void
 * @return 成功返回true；失败则返回false。
 */
bool COperaThread::EndThread(void)
{
	if(!m_pExitEvent)
		return false;
	m_pExitEvent->SetEvent();

	// ??????????检查线程当前状态，如果四个线程正在放炮处理，则产生错误信息
    Sleep(1000);
	return false;
}

/**
 * @brief 创建与测网通信接口
 * @note  在Create函数中被调用，初始化对象时，调用该函数创建与测网通信的接口，该接口主要给测网发送命令并接收回令。
 * @param  DWORD dwIP，主机IP
 * @param  WORD wPort，主机端口
 * @param  DWORD dwMainCrossIP，主交叉站IP
 * @param  WORD wMainCrossPort，主交叉站端口
 * @return 成功返回true；失败则返回false。
 */
bool COperaThread::CreateFDUSocket(DWORD dwIP, WORD wPort)
{
	if(m_pFDUSocket)
	{
		m_pFDUSocket->CloseSocket();
		delete m_pFDUSocket;
		m_pFDUSocket =NULL;
	}
	m_pFDUSocket = new CSocketFDU;
	
	return m_pFDUSocket->CreateSocket(dwIP,wPort);;
}
/**
 * @brief 日志管理类
 * @note  程序的所有运行记录通过该函数发送出去，由AdminServer程序接收后再通知客户端。
 * @param  LPCTSTR strPosition,日志发生的位置描述
 * @param LPCTSTR strDesc,日志描述
 * @param WORD wType,日志类型：正常、异常、警告、调试等
 * @param BOOL bShowed，是否显示给用户
 * @return 成功则返回true，失败则返回false
 */
void    COperaThread::WriteLogMsg(LPCTSTR strPosition,LPCTSTR strDesc,WORD wType,BOOL bShowed)
{
	if(!m_pClientSocket)
		return;

	CLogBase	ProgramLog;	
	// 设置日志基本属性：主机IP及应用程序
	ProgramLog.SetHostAndProgram(theApp.m_strHostIP,MATRIX_OPERATIONSERVER);
	ProgramLog.SetLog(strPosition,strDesc,wType,bShowed);
	// 向服务器发送日志，由服务器记录日志信息
	BYTE    pBuf[SOCKET_LOGFRM_MAXLEN];
	int nSize = ProgramLog.WriteToSocketFrame(pBuf);
	m_pClientSocket->SendTo(pBuf,nSize,ADMINSERVER_LOGPORT,_T("255.255.255.255"));	
}
/**
 * @brief 清空接收缓冲区
 * @note  读空Socket接收缓冲区。对于不需要处理的回复，调用该函数可清空接收缓冲区
 * @param  void
 * @return 成功则返回true；反之则返回false。
 */
bool COperaThread::ClearRevBufOfSocket(SOCKET sock)
{
	char  byBuf[256];
	if(sock==NULL)
		return false;
	ULONG	dwRevBufSize;
	// 读空缓冲区
	while(1)
	{
		// 判断接收缓冲区是否有数据
		if(ioctlsocket(sock,FIONREAD,&dwRevBufSize))
		{
			return true;
		}
		// 缓冲区字节数大小
		if(dwRevBufSize==0)
			return true;	
		// 读缓冲区数据
		if(recvfrom(sock, byBuf, 256, 0,NULL,NULL)<=0)
			return true;
	}
	return true;

}
/**
 * @brief 向采集排列发送配置命令
 * @note  向采集排列发送配置命令,作为点名命令，设置的配置命令包括：
 * @note  网络等待端口和命令,命令码为0x0A；命令格式为16bits端口+16bits命令，
				16位端口为接收上位机广播命令的端口，低位在前，高位在后；
				16位命令码默认为0x0003,顺序为03 00。
 * @note  采样参数配置命令,命令码为0x3d；
 * @note  滤波器设置命令,命令码为0x17,0x18；
 * @note  该命令作为初始化采集排列时点名命令，在点名无应答时需要重发三次，哑道不点名。对于点名有应答的采集站不重复发送。
 * @param  void
 * @return 成功返回true；失败则返回false。
 */
bool COperaThread::SendConfigCmdToSpread(WORD wBroadcastPort)
{
	if(m_pShotClient==NULL)
		return false;
	DWORD	dwConfig;
	DWORD	dwFilter17;
	DWORD	dwFilter18;
	unsigned char TB_ad,TB_amp,TC_ad,TE_ad;	

	CShotSpread*	  pSpread = m_pShotClient->m_pSpread;
	CFDUConfig*		  pFDUConfig = m_pShotClient->m_pFDUConfig;
	if(pSpread==NULL|| pFDUConfig==NULL)
		return false;
	// 滤波器的参数配置相同	
	dwFilter18 = 0;
	dwFilter18 = pFDUConfig->GetSampleRateCode()| pFDUConfig->GetFilterFIR()|pFDUConfig->GetFilterIIRLow();
	//filter_par18= 0;
	//filter_par18= ad_rate[ComboBoxADRate->ItemIndex]|filter_fir[ComboBoxFilterFIR->ItemIndex]|
	//filter_iir_low[ComboBoxFilterIIR->ItemIndex];

	dwFilter17 = pFDUConfig->GetFilterIIRHigh();
	// filter_par17= filter_iir_high[ComboBoxFilterIIR->ItemIndex];	
	
	int i,nCount = pSpread->GetCount();
	for(i=0;i<nCount;i++)
	{
		// 如果是哑道，不点名
		if(pSpread->m_arrSpread[i].m_byType==CHN_MUTE)
		{
			pSpread->m_arrSpread[i].m_lAckedType = TRUE;
			continue;
		}
		// 如果已经收到应答，则继续寻找
		if(pSpread->m_arrSpread[i].m_lAckedType)
			continue;
		//设置参数
		TB_amp= pFDUConfig->GetGainCode(pSpread->m_arrSpread[i].m_dwGain);		// 每个采集道的增益不同	
		TB_amp= TB_amp|pFDUConfig->Get3301Mux();
		TB_ad = pFDUConfig->Get5373Mode() | pFDUConfig->Get5373Attr();
		// TB_ad = set_5373_mode[ComboBox5373Mode->ItemIndex]|set_5373_att[ComboBox5373Att->ItemIndex];
		TC_ad = TB_ad;	
		TE_ad = 0x80;
	
		// 先从高位开始
		dwConfig = TE_ad;
		dwConfig = dwConfig << 8;
		dwConfig |= TC_ad;				// 0x80 | 
		dwConfig = dwConfig << 8;
		dwConfig |= 0x80 | TB_amp;
		dwConfig = dwConfig << 8;
		dwConfig |= 0x80 | TB_ad;

		m_pFDUSocket->SendConfigCmdToFDU(pSpread->m_arrSpread[i].m_dwIP,wBroadcastPort,dwConfig,dwFilter17,dwFilter18);
	}
	return true;
}
/**
 * @brief 向采集排列广播发送设置广播端口命令
 * @note  向采集排列广播发送设置端口命令。
 * @note  在完成采样命令的设置，发送TB等命令后，
		  应该取消当前广播端口，以备下一个采集排列使用。
 * @param  WORD wBroadcastPort , 新的广播端口
 * @return void。
 */
void COperaThread::SendBroadcastPortCmdToSpread(WORD wBroadcastPort)
{
	m_pFDUSocket->SendBroadcastPortCmdToFDU(0xFFFFFFFF,wBroadcastPort);
}
/**
 * @brief 向采集排列发送点名命令
 * @note  向采集排列发送点名命令。循环向每个采集道发送点名命令。
 * @param  void
 * @return 成功返回true；失败则返回false。
 
bool COperaThread::SendCallCmdToSpread()
{
	if(m_pShotClient==NULL)
		return false;

	CShotSpread*	  pSpread = m_pShotClient->m_pSpread;
	if(pSpread==NULL)
		return false;
	int i,nCount = pSpread->m_arrSpread.GetCount();
	for(i=0;i<nCount;i++)
	{
		// 如果是哑道，不点名
		if(pSpread->m_arrSpread[i].m_byType==CHN_MUTE)
		{
			pSpread->m_arrSpread[i].m_lAckedType = TRUE;
		    continue;
		}
		// 如果已经收到应答，则继续寻找
		if(pSpread->m_arrSpread[i].m_lAckedType)
			continue;
		m_pFDUSocket->SendCallCmdToFDU(pSpread->m_arrSpread[i].m_dwIP,m_wRevPort[0],m_wRevPort[7]);
	}
	return true;
}*/

/**
 * @brief 向采集排列发送TE命令
 * @note  向采集排列发送TE命令，停止噪声采集。
 * @param DWORD dwStopSampleTime，停止采样时间(ms)，是据当前时刻dwStopSampleTime后停止采样
 * @return 成功返回true；失败则返回false。
 */
bool COperaThread::SendTEBroadcastCmdToSpread(DWORD dwFDUSysTime)
{
	if(m_pShotClient==NULL)
		return false;

	CShotSpread*	  pSpread = m_pShotClient->m_pSpread;
	if(pSpread==NULL)
		return false;
	/*
	int i,nCount = pSpread->m_arrSpread.GetCount();
	for(i=0;i<nCount;i++)
	{	// 向采集排列的某一道发送TE命令		
		m_pFDUSocket->SendTECmdToFDU(pSpread->m_arrSpread[i].m_dwIP,dwTE_H,dwTE_L);
	}
	*/
	DWORD	dwTE,dwTEH,dwTEL;
	dwTE= 5000;					// 缺省是1000ms后停止采样
	dwTE= dwTE << 16;
//	dwTE+=dwFDUSysTime;

	dwTEH= dwFDUSysTime + dwTE/0x3fff;
	dwTEL= dwTE%0x3fff;
	dwTEL|=0x00800000;

	m_pFDUSocket->SendTEBroadcastCmdToFDU(dwTEH,dwTEL);
	return true;
}

/**
 * @brief 向采集排列发送数据采集命令
 * @note  在放炮点名、爆炸机控制器准备好后，收到用户的“GO”命令，先通知采集线程
 准备数据采集，然后调用该函数向采集排列广播发送TB、TE、滤波器0，滤波器1等参数
 通知采集站开始采集。
 * @note  采样截止时间TE是在TB的基础上增加740ms。
 * @param  DWORD dwFDUSysTime，测网系统的当前时间 
 * @param  DWORD dwDelaySampleMS，以ms为单位，延时启动采样
 * @return 成功返回true；失败则返回false。
 */
bool COperaThread::SendSampParamBroadcastCmdToSpread(DWORD dwFDUSysTime,DWORD dwDelaySampleMS)
{
	if(m_pShotClient==NULL)
		return false;
	CProcessType* pProcess = m_pShotClient->m_pProcessType;
	if(pProcess==NULL)
		return false;
	
	DWORD dwTEDelayMS,dwTBH,dwTBL,dwTEH,dwTEL;
	// 计算采样开始时间: 当前FDU系统时间基础上延时 dwDelaySampleMS 开始采样		
	CShotTime::GetFDUTimeHighAndTimeLow(dwFDUSysTime,dwDelaySampleMS,dwTBH,dwTBL);
	
	// 计算采样结束时间TE(ms): 采样长度 + TB延时启动时间+TB窗口+2000ms
	switch(pProcess->m_byProcessType)
	{
	case PROCESS_IMPULSIVE:
		dwTEDelayMS = (pProcess->m_dwRecordLen + dwDelaySampleMS + 2000 + ((CProcessImpulsive*)pProcess)->m_dwTBWindow);
		break;
	case PROCESS_STACKIMPULSIVE:
		break;
	case PROCESS_CORRELBEFORE:
		break;
	case PROCESS_CORRELAFTER:
		break;
	case PROCESS_STACK:
		break;
	default:
		dwTEDelayMS = (pProcess->m_dwRecordLen + dwDelaySampleMS * 2);
		break;

	}
	// 计算采样结束时间TE（基于系统时间）
	CShotTime::GetFDUTimeHighAndTimeLow(dwFDUSysTime,dwTEDelayMS,dwTEH,dwTEL);
	// 保留TB、TE的取值
	m_ShotTime.m_dwStartTBH = dwTBH;
	m_ShotTime.m_dwStartTBL = dwTBL;
	m_ShotTime.m_dwEndTEH = dwTEH;
	m_ShotTime.m_dwEndTEL = dwTEL;
/*
	dwTE = dwTE << 16;
	// m_dwTE+= m_dwTB;

	dwTBH = dwFDUSysTime + m_dwTB/0x3fff;
	dwTBL = m_dwTB%0x3fff;
	//dwTEL|= 0x00800000;
	
	dwTEH = dwFDUSysTime + m_dwTE/0x3fff;
	dwTEL = m_dwTE%0x3fff;
	dwTEL|= 0x00800000;
*/
	// m_pFDUSocket->SendSampParamCmdToFDU(m_pShotClient->m_pSpread->m_arrSpread[0].m_dwIP,m_wRevPort[0],m_wRevPort[7],dwTBH, dwTBL,dwTEH, dwTEL);
	m_pFDUSocket->SendSampParamCmdToFDU(0xFFFFFFFF,m_wRevPort[0],m_wRevPort[7],dwTBH, dwTBL,dwTEH, dwTEL);
	return true;
}


/**
 * @brief 接收点名应答
 * @note  接收并处理采集排列的点名回应。如果收到该采集排列的回应，则对应的应答标志。
 * @param  long lAckedType,应答类型，对应采集排列CShotSpread对象的m_lAckedType属性
 * @return 网络接收缓冲区处理完毕，则返回-1；反之则返回1。
 */
long COperaThread::RecvAckedFrmOfCall(long lAckedType)
{
	/*
	timeval  timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec =200000;		// 等待超时：200ms

	fd_set  readfs;
	unsigned char  pBuf[256];
	int   nSize=0;
	DWORD dwFDUIP;
	// int i=0;
	do 
	{
		readfs.fd_count =1;
		readfs.fd_array[0]=m_pFDUSocket->m_FDUSocket;
		if(select(0,&readfs,NULL,NULL,&timeout)<=0)
		{
			// TRACE1(" RecvAckOfCall  %d\n",i);
			return -1;
		}
		// 	if(!FD_ISSET(m_pFDUSocket->m_FDUSocket,&readfs))
		// 		return -1;
		nSize = recvfrom(m_pFDUSocket->m_FDUSocket, (char*)pBuf, 256, 0,NULL,NULL);
		if(nSize>0)
		{  
			// i++;
			// 解析数据包，获得IP地址
			dwFDUIP = m_pFDUSocket->GetFDUIPFromUplinkPack(pBuf,nSize);
			// 设置相应的采集道应答标志
			m_pShotClient->m_pSpread->SetAck(dwFDUIP,lAckedType);
		}

	} while (nSize>0);
	
	return 1;
*/
	//////////////////////////////////////////////////////////////////////////
	DWORD	dwFDUIP;
	BYTE	pBuf[256];	
	int		nSize ;
	ULONG	dwRevBufSize;
	while(1)
	{
		// 判断接收缓冲区是否有数据
		if(ioctlsocket(m_pFDUSocket->m_FDUSocket,FIONREAD,&dwRevBufSize))
		{
			return 0;
		}
		if(dwRevBufSize==0)
			return 0;
		// 通过Socket端口接收数据
		nSize = recvfrom(m_pFDUSocket->m_FDUSocket,(char*)pBuf, 256, 0,NULL,NULL);	
		// 接收数据
		if(nSize<=0)
			return 0;
		// 解析数据包，获得IP地址
		dwFDUIP = m_pFDUSocket->GetFDUIPFromUplinkPack(pBuf,nSize);
		// 设置相应的采集道应答标志
		m_pShotClient->m_pSpread->SetAck(dwFDUIP,lAckedType);
	}
	return nSize;

}

/**
 * @brief 接收采样数据
 * @note  接收某一个端口的采样数据。该函数被数据接收线程调用，循环接收多个网络端口的采样数据。
 * @param  SOCKET RevSocket，接收数据的socket端口
 * @param  CRevBuffer* pRevBuf，接收缓冲区
 * @return 缓冲区接收完毕则返回0；发生错误返回-1
 */
int COperaThread::RecvSampleData(SOCKET RevSocket,CRevBuffer* pRevBuf)
{
	unsigned char    pTempBuf[256];
	int     nSize=0; 
	DWORD   dwIP;
	int     nIndex;

	timeval  timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec =0;		// 等待超时：300ms
	fd_set  readfs;

	do 
	{
		readfs.fd_count =1;
		readfs.fd_array[0]=RevSocket;
		if(select(0,&readfs,NULL,NULL,&timeout)<=0)
			return 0;
		nSize = recvfrom(RevSocket,(char *)pTempBuf,256,0,NULL,NULL);
		if(nSize<=0)
		{
			if(GetLastError()!=WSAEWOULDBLOCK)
			{
				// ?????????通信错误，记录日志
			}
		}
		dwIP = m_pFDUSocket->GetFDUIPFromUplinkPack(pTempBuf,256);
		nIndex = m_pShotClient->m_pSpread->Find(dwIP);
		if (nIndex>=0)
		{
			if(pTempBuf[26]==0x03 && pTempBuf[27]==00) // 判断是否采样数据帧
			{
				pRevBuf->SetFrmIndex(MAKEWORD(pTempBuf[28],pTempBuf[29]));
				// 如果是哑道，则将接收的数据置为0;该操作在请求重发时执行
				/*if(m_pShotClient->m_pSpread->m_arrSpread[nIndex].m_byType == CHN_MUTE)
				{
					memset((unsigned char*)(pTempBuf+30),0,SAMPFRM_DATALEN);
				}*/
				pRevBuf->WriteData(nIndex,(unsigned char*)(pTempBuf+30),SAMPFRM_DATALEN);				
			}			
		}
		else
		{
			// 没有找到IP地址，错误数据，丢掉
		}
		
	} while (nSize>0);
	
	return 0;
}

/**
 * @brief 向采集排列发送数据重发命令
 * @note  检查每个采集道的数据是否正常接收。如果没有收到数据，则调用该函数下发数据重发命令。
 如果采集道为哑道，则将对应的缓冲区清零，置接收数据标志。
 * @param  WORD wPort，需要重发的数据端口
 * @param  CRevBuffer* pRevBuf，接收缓冲区对象，查询对应的采集道数据标志
 * @return 返回需要重发的采集道总数
 */
int COperaThread::SendRetransCmdToSpread(CRevBuffer *pRevBuf,WORD wPort)
{
	if(pRevBuf==NULL || pRevBuf->IsAllOK())
		return 0;

	int   i;
	int   nReTranCount=0;		// 保存重发次数，如果没有重发，则设置CRevBuffer的标志m_bAllRev
	CShotSpread* pSpread=m_pShotClient->m_pSpread;
	int  nCount=pSpread->GetCount();
	for(i=0;i<nCount;i++)
	{	
		// 判断不需要重发条件：已经收到，或者 点名时没有应答
		if(pRevBuf->GetRevFlag(i) || !pSpread->m_arrSpread[i].m_lAckedType)    
			continue;
		if(pSpread->m_arrSpread[i].m_byType==CHN_MUTE)
		{	// 哑道，将数据清零
			pRevBuf->ClearBuf(i);	
			continue;
		}
		// 有需要重发数据
		nReTranCount++;
		// 发送数据重发命令
		m_pFDUSocket->SendRetranCmdToFDU(pSpread->m_arrSpread[i].m_dwIP,wPort,pRevBuf->GetFrmIndex());		
	}
	// 判断该次采样数据是否全部接收
	if(nReTranCount==0)
	{
		pRevBuf->SetIsAllOK(TRUE);
	}
	else
		pRevBuf->SetIsAllOK(FALSE);
	return nReTranCount;
}
/**
 * @brief 通用命令发送函数
 * @note  在施工服务器和施工客户端通信过程中，有部分的命令帧格式相同，仅仅命令代码不同，采用该通用函数向施工服务器发送数据。
 * @note  帧格式为:
 * @note  帧头（2Byte）＋帧长度（2Byte）＋帧计数（4Byte）＋帧类型（1Byte）＋命令码(2Byte)＋数据个数（2Byte）＋数据区（通信编号ID(4byte)和线程索引号(4byte)）＋校验位（1Byte）＋帧尾（2Byte）
 * @param  WORD  wCmdCode 命令码
 * @return 返回已发送的字节数。
 */
int  COperaThread::SendGeneralCMDToClient(WORD  wCmdCode)
{	
	unsigned char		pBuf[26];
	// 帧头
	pBuf[0] = 0xEB;
	pBuf[1] = 0x90;
	// 帧长
	pBuf[2] = 24;
	pBuf[3] = 0;
	// 帧计数
	memset(pBuf+4,0,4);
	// 帧类型
	pBuf[8] = 0x01;
	// 命令码
	pBuf[9]  = LOBYTE(wCmdCode);
	pBuf[10] = HIBYTE(wCmdCode);
	// 数据个数
	pBuf[11] =0x08;
	pBuf[12] =0;
	// 炮号、震源
	memcpy_s(pBuf+13,4,&m_dwServerID,4);
	memcpy_s(pBuf+17,4,&m_dwThreadID,4);
	// 帧校验
	pBuf[21]=0;
	// 帧尾
	pBuf[22] = 0x14;
	pBuf[23] = 0x6F;

	return m_pClientSocket->SendDataToClient(m_pShotClient->m_dwClientIP,m_pShotClient->m_wClientPort,pBuf,24);
}
/**
 * @brief 向客户端发送爆炸机状态
 * @note  接收到爆炸机的确认TB及井口时间后，调用该函数发送给客户端
 * @note  传送的参数包括：炮点(4Byte)、爆炸机号(4Byte)、爆炸机状态(4Byte)、确认TB时刻(浮点，4Byte)、井口时间(浮点，4Byte)
 * @param  WORD  wCmdCode 命令码
 * @return 返回已发送的字节数。
 */
int  COperaThread::SendBlasterState(void)
{
	unsigned char		pBuf[44];
	// 帧头
	pBuf[0] = 0xEB;
	pBuf[1] = 0x90;
	// 帧长
	pBuf[2] = 44;
	pBuf[3] = 0;
	// 帧计数
	memset(pBuf+4,0,4);
	// 帧类型
	pBuf[8] = 0x02;
	// 命令码
	pBuf[9]  = LOBYTE(OPERATION_CMD_TBUPHOLE);
	pBuf[10] = HIBYTE(OPERATION_CMD_TBUPHOLE);
	// 数据个数
	pBuf[11] =5;
	pBuf[12] =0;
	// 服务器ID及线程ID
	memcpy_s(pBuf+13,4,&m_dwServerID,4);
	memcpy_s(pBuf+17,4,&m_dwThreadID,4);
	// 炮号、震源
	memcpy_s(pBuf+21,4,&m_pShotCtrl->m_dwShotNb,4);
	memcpy_s(pBuf+25,4,&m_pShotCtrl->m_dwSourceNb,4);
	// 状态
	pBuf[29] = m_pShotCtrl->m_byBlasterState;
	pBuf[30] = 0;
	pBuf[31] = 0;
	pBuf[32] = 0;
	// 确认TB
	memcpy_s(pBuf+33,4,&m_pShotCtrl->m_fConfirmedTB,4);
	// 井口时间
	memcpy_s(pBuf+37,4,&m_pShotCtrl->m_fUpholeTime,4);
	
	// 帧校验
	pBuf[41]=0;
	// 帧尾
	pBuf[42] = 0x14;
	pBuf[43] = 0x6F;

	return m_pClientSocket->SendDataToClient(m_pShotClient->m_dwClientIP,m_pShotClient->m_wClientPort,pBuf,44);
}
/**
 * @brief 向客户端发送点名结果
 * @note  将采集排列的点名结果发送给客户端，只发送点名无应答的采集排列。
 * @note  帧格式：帧头（2Byte）＋帧长度（2Byte）  ＋帧计数（4Byte）＋帧类型（1Byte）			   
 ＋命令(2Byte)＋数据个数（2Byte）＋数据区（变长） ＋校验位（1Byte）＋帧尾（2Byte）
  * @note  数据区： 数据个数8+4=12个byte包括:  唯一编号ID(4byte)，线程索引号(4byte), 
 采集站无应答个数（4Byte）
  * @note 暂时不向客户端发送无应答IP列表（4Byte*n）
 * @param  void
 * @return 成功返回true；失败则返回false。
 */
bool COperaThread::SendCallNoResponseToClient()
{
	CShotSpread*	  pSpread = NULL;
	if(m_pShotClient==NULL)
		return false;

	pSpread = m_pShotClient->m_pSpread;
	if(pSpread==NULL)
		return false;
    
	unsigned char * pBuf=NULL;
	int   i,nFrmPos,nCount,nNoAckedCount;	
	WORD  wFrmLen;

	// 计算采集排列中采集道的总数
	nCount =  pSpread->GetCount();
	// 计算无应答采集道的总数
	nNoAckedCount = nCount - pSpread->TotalOfAcked()-pSpread->GetAuxiCount();
	// 最大仅允许传送2000道的无应答采集道IP
	if(nNoAckedCount>2000)
	{		
		wFrmLen = WORD(28+4*2000);
	}
	else
	{
		// 计算帧长度
		wFrmLen = WORD(28+4*nNoAckedCount);
	}
	
	pBuf = new unsigned char[wFrmLen];
	pBuf[0] = 0xEB;
	pBuf[1] = 0x90;	
	pBuf[2] = LOBYTE(wFrmLen);
	pBuf[3] = HIBYTE(wFrmLen);
	// 帧计数
	nFrmPos = 1;
	memcpy_s(pBuf+4,4,&nFrmPos,4);
	
	// 帧类型,回令帧
	pBuf[8] = 0xFF;
	// 命令码
	pBuf[9]  = LOBYTE(OPERATION_ACK_NAME);
	pBuf[10] = HIBYTE(OPERATION_ACK_NAME);
	
	// 数据个数	
	pBuf[11] =LOBYTE(wFrmLen-16);
	pBuf[12] =HIBYTE(wFrmLen-16);

	// 服务器ID、线程ID
	memcpy_s(pBuf+13,4,&m_dwServerID,4);
	memcpy_s(pBuf+17,4,&m_dwThreadID,4);	
	// 无应答的总数
	memcpy_s(pBuf+21,4,&nNoAckedCount,4);
	nFrmPos = 25;
	if(nNoAckedCount)
	{
		for(i=0;i<nCount;i++)
		{	// 已应答的采集点和辅助道不计算在内
			if(pSpread->m_arrSpread[i].m_lAckedType || pSpread->m_arrSpread[i].m_byType==CHN_AUXI)
				continue;
			memcpy_s(pBuf+nFrmPos,4,&(pSpread->m_arrSpread[i].m_dwIP),4);
			nFrmPos+=4;
			if((nFrmPos-25)/4>=2000)
				break;
		}
	}	
	// 帧校验
	pBuf[nFrmPos++]=0;
	// 帧尾
	pBuf[nFrmPos++] = 0x14;
	pBuf[nFrmPos++] = 0x6F;
	
	m_pClientSocket->SendDataToClient(m_pShotClient->m_dwClientIP,m_pShotClient->m_wClientPort,pBuf,wFrmLen);
	delete []pBuf;
	return true;
}
/**
 * @brief 放炮线程
 * @note  该线程主要是完成放炮采集前的初始化工作，处理爆炸机控制器信息，控制放炮过程。
 * @param  LPVOID lpParameter，线程管理对象COperaThread的指针
 * @return 返回0，线程退出
 */
UINT __cdecl ShotControlProc(LPVOID lpParameter)
{
    COperaThread*  pThread = (COperaThread*) lpParameter;
	if(!pThread)
		return 0;
	CShotClient*	pShotClient=NULL;
	// 测网系统时间
	DWORD		   dwFDUSystemTime;		// 保存测网的系统时间
	DWORD		   dwCount = 0;			// 作为计数器
	DWORD		   dwDelayMS=0;			// 延时时间(ms)
	DWORD		   dwTimeMS = 0;		// 主机的TickCount时间(ms)
    DWORD		   dwEvent;
	HANDLE		   hEvents[3];
	while (true)
	{		
		// 等待两个事件：退出线程或者准备放炮
		hEvents[0] = pThread->m_pExitEvent->m_hObject;
		hEvents[1] = pThread->m_pShotEvent->m_hObject;
		dwEvent = WaitForMultipleObjects(2,hEvents,FALSE,INFINITE);
		switch(dwEvent)
		{
		case WAIT_OBJECT_0 + 0: // 线程退出
			pThread->WriteLogMsg(_T("ShotControlProc"),_T("ShotControlProc Thread Exit!"),LOGTYPE_DEBUG,FALSE);
			return 0L;
			break;
		case WAIT_OBJECT_0 + 1: // 放炮开始：m_pShotEvent
			break;
		default:
			continue;
			//break;
		}
		// 清除停止放炮的状态
		pThread->m_pShotStopEvent->ResetEvent();
		// ---------------------------------------------------------------------------------------------------
		// ---------------首先点名：向采集排列发送0x0A、0x3D、0x17、0x18命令 ---------------------------------
		// ---------------------------------------------------------------------------------------------------
		// 1、清除缓冲区
		pThread->m_pFDUSocket->ClearRevBufOfSocket();
		// 2、向采集排列发送点名命令：0A命令、采样参数配置、滤波器设置等	
		pThread->SendConfigCmdToSpread(pThread->m_pFDUSocket->m_wLocalPort);
		// 3、接收点名应答：在 RecvAckOfCall函数中等待接收；每隔3次循环无应答，则重发应答命令
		pShotClient=pThread->GetShotClient();
		dwCount =0;
		while(dwCount<20)
		{
			Sleep(100);
			// 从Socket类接收应答回令
			if(pThread->RecvAckedFrmOfCall(SHOTSPREAD_ACKED_CONFIGCMD)>0)
				continue;
			if(pShotClient->m_pSpread->IsAllAcked(SHOTSPREAD_ACKED_CONFIGCMD))
				break;
			dwCount++;
			// 重发
			if(!(dwCount%6))
			{
				pThread->SendConfigCmdToSpread(pThread->m_pFDUSocket->m_wLocalPort);
			}
		}
	    // 4、获得点名结果,并返回给主机,只发送没有应答的IP列表
		pThread->SendCallNoResponseToClient();
		
		// 5、判断爆炸机控制器是否发送Ready信号，如果不发送Ready信号，则模拟向客户端发送Ready信号
		// ?????????? 模拟发送Ready 

		// ---------------------------------------------------------------------------------------------------
		// ------------等待主机按下"GO"按钮，同时检测三个事件-------------------------------------------------
		// ---------------------------------------------------------------------------------------------------
		hEvents[0] = pThread->m_pExitEvent->m_hObject;		// 退出事件
		hEvents[1] = pThread->m_pShotGOEvent->m_hObject;	// GO事件
		hEvents[2] = pThread->m_pShotStopEvent->m_hObject;	// STOP事件，停止采样
		dwEvent = WaitForMultipleObjects(3,hEvents,FALSE,INFINITE);
		switch(dwEvent)
		{
		case WAIT_OBJECT_0 + 0: // 线程退出
			return 0L;
			break;
		case WAIT_OBJECT_0 + 1: // 点火事件：m_pShotGOEvent
			pThread->m_pRevEvent->SetEvent(); 
			break;
		case WAIT_OBJECT_0 + 2: // 停止事件：m_pShotStopEvent
			{
				// 先删除对象,此时数据采集线程还没有开始数据采集
				pThread->ResetShotParam();
				pThread->SetThreadState(THREAD_IDLE);
			}
			continue;
			break;
		default:
			continue;
			break;
		}	
		// ---------------------------------------------------------------------------------------------------
		// ----------开始放炮：读取系统时间，发送停止噪声采集的命令-------------------------------------------
		// ---------------------------------------------------------------------------------------------------
		// 1、先查询系统时间，通过0xC000端口收尾包来计算当前系统时间	
		/*dwFDUSystemTime = theApp.GetFDULocalTime();
		
		// 2、向采集排列连续三次广播发送TE命令，停止噪声采集	
		
		pThread->SendTEBroadcastCmdToSpread(dwFDUSystemTime);	
		Sleep(15);
		pThread->SendTEBroadcastCmdToSpread(dwFDUSystemTime);
		Sleep(15);
		pThread->SendTEBroadcastCmdToSpread(dwFDUSystemTime);
		// 清空接收缓冲区
		pThread->m_pFDUSocket->ClearRevBufOfSocket();
		Sleep(5000);
		*/
		// ------------------------------------------------------------------------------------
		// --------- 向采集排列发送采样命令：07、08、09；TB，TE,----------------------		
		// 1、得到系统时间	
		// ??????????????????????
		
		//pThread->m_pFDUSocket->SendQueryTimeCmd(pShotClient->m_pSpread->m_arrSpread[0].m_dwIP);
		//Sleep(20);
		//pThread->m_pFDUSocket->SendQueryTimeCmd(pShotClient->m_pSpread->m_arrSpread[0].m_dwIP);
		//Sleep(50);
		//pThread->m_pFDUSocket->SendQueryTimeCmd(pShotClient->m_pSpread->m_arrSpread[0].m_dwIP);
			
		//---------------------------------------------------------------------------
		// 2、向采集站广播发送三次采集命令: 07、08、09；TB，TE,----------------------
		// 清空接收缓冲区
		pThread->m_pFDUSocket->ClearRevBufOfSocket();
		// 延时启动采样的时间，以ms为单位
		dwDelayMS = 10000;
		dwFDUSystemTime = theApp.GetFDULocalTime();	
		// 发送采样命令:启动采样时刻为 当前测网时间dwFDUSystemTime + 延时dwDelayMS
		pThread->SendSampParamBroadcastCmdToSpread(dwFDUSystemTime,dwDelayMS);
		pThread->m_pRevEvent->SetEvent(); 
		// 记录下主机当前时间，计算出采样启动时间
		dwTimeMS = GetTickCount() + dwDelayMS;
		dwCount =0;
		while(GetTickCount()<dwTimeMS)
		{
			Sleep(dwDelayMS/20);
			// 从Socket类接收应答回令
			if(pThread->RecvAckedFrmOfCall(SHOTSPREAD_ACKED_SAMPLECMD)>0)
				continue;
			if(pShotClient->m_pSpread->IsAllAcked(SHOTSPREAD_ACKED_SAMPLECMD))
				break;
			dwCount++;
			// 将总的延时，仅重发三次
			if(!(dwCount%5) && dwCount<20)
			{
				pThread->SendSampParamBroadcastCmdToSpread(dwFDUSystemTime,dwDelayMS);
			}
		}	
		/*
		// ??????????????????????测试代码
		pThread->m_pFDUSocket->SendQueryTimeCmd(pThread->m_pShotClient->m_pSpread->m_arrSpread[0].m_dwIP);
		Sleep(5000);
		pThread->m_pFDUSocket->SendQueryTimeCmd(pThread->m_pShotClient->m_pSpread->m_arrSpread[0].m_dwIP);
		Sleep(2000);
		pThread->m_pFDUSocket->SendQueryTimeCmd(pThread->m_pShotClient->m_pSpread->m_arrSpread[0].m_dwIP);
		Sleep(2000);
		pThread->m_pFDUSocket->SendQueryTimeCmd(pThread->m_pShotClient->m_pSpread->m_arrSpread[0].m_dwIP);
		Sleep(1000);
		*/
		
		// 3、适当延时(延时时间是TB时刻与采集排列当前时间的差值，即增量)，然后再发点火命令
		//dwDelayMS = GetTickCount();
		//if(dwDelayMS>dwTimeMS)
		//	Sleep(dwDelayMS-dwTimeMS);
		// pThread->m_pFDUSocket->SendQueryTimeCmd(pThread->m_pShotClient->m_pSpread->m_arrSpread[0].m_dwIP);
		// -----------------向爆炸机控制器发送点火命令-----------------------------------
		// 1、发送点火指令
		pThread->m_pShotCtrl->SendFireOrder(pShotClient);
		// 2、是否需要等待点火确认；爆炸机控制器返回确认TB时刻；找到爆炸机状态后，依据状态进行处理，并清除爆炸机对象的状态		
		// 3、将确认TB时间加上采样时间(应该终止采样时刻)与已经发下的TE时刻相比较；
		//    如果下发的TE时刻较大，则不处理；否则应该重新下发TE命令(不需要考虑此问题，目前TE为TB+TB窗口时间+)

		// 4、 在指定时间内收到确认TB，发送给客户端；如果规定时间内没有收到，向客户端返回TB错误
		if(   pShotClient->m_pProcessType->m_byProcessType == PROCESS_IMPULSIVE 
			||pShotClient->m_pProcessType->m_byProcessType == PROCESS_STACKIMPULSIVE )
		{
			// ------------等待开始采集或者程序退出事件-----------------------------------------------------------------------------
			hEvents[0] = pThread->m_pExitEvent->m_hObject;
			hEvents[1] = pThread->m_pShotStopEvent->m_hObject;
			dwEvent = WaitForMultipleObjects(2,hEvents,FALSE,pShotClient->m_pProcessType->m_dwTBWindow);
			switch(dwEvent)
			{
			case WAIT_OBJECT_0 + 0: // 线程退出事件,是该线程唯一的退出位置，其他地方接收到退出事件后，仅仅是设置标志，返回到此处退出
				{
					return 0L;				
				}
				break;
			case WAIT_OBJECT_0 + 1: // 停止采样 m_pShotStopEvent
				continue;
				break;
			case WAIT_TIMEOUT:
				{
					if(!pThread->m_pShotCtrl->IsNewState())
					{
						// 发送1TB错误
						pThread->SendGeneralCMDToClient(OPERATION_CMD_ITBERR);
						pThread->WriteLogMsg(_T("ShotControlProc"),_T("Internal Time Break!"),LOGTYPE_WARNING,TRUE);
					}
				}
				break;
			default:								
				break;
			}			
		}		
		// 5、发送网络命令等待端口，将当前端口释放掉
		pThread->SendBroadcastPortCmdToSpread(1);			// ?????????? 1端口是否可行，调用该函数最好不用广播，单点通信
		// ----------------放炮控制结束----------------------------------------------------
	} // while(true)
	
	return 0L;
}
/************************************************************************************************************
    函    数：ReceiveProc
    
    输    入：LPVOID lpParameter -- 指向线程管理对象COperaThread的指针
    
    输    出：NULL
  
    返 回 值：UINT
    
    功    能： 数据采集线程：多道采集算法，8个端口循环接收数据。

    修改历史：

              dwFrmCount是接收多道采集中已接收采样的次数，每完成一个端口的接收（即74个采样时刻）后加一；
			  NPortCount是一次接收查询中需要监听端口的总数，初始值为2个端口，在第四次之后取值为一直为6；
			  NPort[6]保存需要监听的端口编号，最多时是6个；
			  三个参数之间的关系如下：
			  dwFrmCount	NPortCount	NPort[6]
			      1			   2		  0,1
				  2			   3		  0,1,2
				  3			   4		  0,1,2,3
				  4			   5		  0,1,2,3,4
				  5			   6		  0,1,2,3,4,5	----开始存盘第一个端口数据
				  6			   6		  1,2,3,4,5,6
				  7			   6		  2,3,4,5,6,7
				  8			   6		  3,4,5,6,7,0
				  9			   6		  4,5,6,7,0,1

*************************************************************************************************************/
UINT __cdecl ShotReceiveProc(LPVOID lpParameter)
{
    COperaThread*  pThread = (COperaThread*) lpParameter;
	HANDLE		   hEvents[3];
	int			   i;
    // 测网系统时间	
	DWORD			dwFDUSystemTime=0;
	// 保存采样结束时间
	DWORD			dwSampleTE=0;
	// 是否终止数据采样
	BOOL			bStopReceiving=FALSE;
	
	fd_set		  readfds;
	timeval       timeout;
	timeout.tv_sec=0;
	timeout.tv_usec = 100000;	    // 每次等待100ms接收数据

	// 8个Socket,循环接收
	unsigned char  byPortCount = 8;
	int            nValidSocket;
	SOCKET*		   pRevSocket=new SOCKET[byPortCount];
	struct sockaddr_in HostIP;
	HostIP.sin_family = AF_INET;
	HostIP.sin_addr.S_un.S_addr = pThread->m_dwLocalIP;

	// ---------------- 创建8个SOCKET ---------------------------	
	for (i=0;i<byPortCount;i++)
	{
		HostIP.sin_port = htons(pThread->m_wRevPort[i]);
		pRevSocket[i] = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
		if(SOCKET_ERROR == bind(pRevSocket[i],(struct sockaddr*)&HostIP,sizeof(HostIP)))
		{
			// 创建端口失败，发送错误信息
			pRevSocket[i] = 0;
			pThread->WriteLogMsg(_T("ShotReceiveProc"),_T("Fail to bind Receive data port!"),LOGTYPE_ERROR,TRUE);
			continue;
		}
		// 接收缓冲区大小
		nValidSocket = 10*1024*1024;	
		if (setsockopt(pRevSocket[i], SOL_SOCKET, SO_RCVBUF, (char*)&nValidSocket, sizeof(nValidSocket)) == SOCKET_ERROR) 
		{
			closesocket(pRevSocket[i]);
			// 创建端口失败，发送错误信息
			pThread->WriteLogMsg(_T("ShotReceiveProc"),_T("Fail to set  receive data buffer!"),LOGTYPE_ERROR,TRUE);
			pRevSocket[i] = 0;
			continue;
		}
		// 作为临时变量，发送缓冲区大小
		nValidSocket = 2048*1024;	
		if (setsockopt(pRevSocket[i], SOL_SOCKET, SO_SNDBUF, (char*)&nValidSocket, sizeof(nValidSocket)) == SOCKET_ERROR) 
		{
			closesocket(pRevSocket[i]);
			// 创建端口失败，发送错误信息
			pThread->WriteLogMsg(_T("ShotReceiveProc"),_T("Fail to set  send data buffer!"),LOGTYPE_ERROR,TRUE);
			pRevSocket[i] = 0;
			continue;
		}
		readfds.fd_array[i]=pRevSocket[i];
	}
	readfds.fd_count = byPortCount;
	
	// 创建缓冲区
	CRevBuffer*	pRevBuf=new CRevBuffer[byPortCount];		
	// 保存接收的数据文件
	CFile		RevFile;
	CString		strFileName;
	// SYSTEMTIME  sysTime;
	

	// --------以下三个变量用于多道接收的处理-----------------------------
	// 需要查询的socket端口数量
	int			NPortCount;
	// 最多同时查询处理6个端口的数据; NPort[0]为已重发三次,要存盘的数据端口编号;
	// NPort[1]正在第三次重发的端口,NPort[2]正在第2次重发的端口,
	// NPort[3]正在第1次重发的端口，NPort[4]正在接收数据的端口,NPort[5]正在接收数据的端口			
	int			NPort[6];			
	DWORD		dwFrmCount=2;		// 采样数据帧的计数器，相当于某一道的帧计数
	CShotClient* pShotClient=NULL;
    DWORD  dwEvent;
	while (true)
	{		
		// ---------------------------------------------------------------------------------------------------------------------
		// ------------等待开始采集或者程序退出事件-----------------------------------------------------------------------------
		hEvents[0] = pThread->m_pExitEvent->m_hObject;
		hEvents[1] = pThread->m_pRevEvent->m_hObject;
		dwEvent = WaitForMultipleObjects(2,hEvents,FALSE,INFINITE);
		switch(dwEvent)
		{
		case WAIT_OBJECT_0 + 0: // 线程退出事件,是该线程唯一的退出位置，其他地方接收到退出事件后，仅仅是设置标志，返回到此处退出
			{
				for (i=0;i<byPortCount;i++)
				{
					closesocket(pRevSocket[i]);						
				}
				delete []pRevSocket;
				delete []pRevBuf;				
			}
			pThread->WriteLogMsg(_T("ShotReceiveProc"),_T("ShotReceiveProc Thread Exit!"),LOGTYPE_DEBUG,FALSE);
			return 0L;
			break;
		case WAIT_OBJECT_0 + 1: // 采集开始：m_pRevEvent
			break;
		default:
			continue;
			break;
		}
		// 清除停止放炮的状态
		pThread->m_pShotStopEvent->ResetEvent();
		// ---------------------------------------------------------------------------------------------------------------------
		// ---------------开始数据采集------------------------------------------------------------------------------------------
		// -------------向客户端发送状态，创建数据文件--------------------------------------------------------------------------
		pThread->SetThreadState(THREAD_SAMPLE);
		pShotClient= pThread->GetShotClient();
		// 通知客户端“开始采样”
		pThread->SendGeneralCMDToClient(OPERATION_CMD_RCVING);
		// 先创建文件	
		//GetLocalTime(&sysTime);	
		//strFileName.Format(_T("%04d%02d%02d%02d%02d%02d%04d.Rev"),sysTime.wYear,sysTime.wMonth,sysTime.wDay,
		//						sysTime.wHour,sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds);		
		pShotClient->m_dwFileNumber = theApp.GetGlobalFileNumber();									// 计算得到文件编号
		pShotClient->m_strFileName.Format(_T("%d.dat"),pShotClient->m_dwFileNumber);			// 原码文件名
		strFileName = pShotClient->m_strNormalFilePath +_T("\\")+pShotClient->m_strFileName;	// 文件路径
		// 如果文件已打开，则先关闭原有文件
		if(RevFile.m_hFile!= CFile::hFileNull)
		{
			RevFile.Close();
		}
		// 创建新数据文件，先存储数据原码
		if(!RevFile.Open(strFileName,CFile::modeCreate|CFile::modeWrite))
		{
			// 创建文件错误,通知主机
			TRACE1("Failed to Create file %s !",strFileName);
			// 通知客户端，创建文件错误
		}
		// --------------------------------------------------------------------------------------------------------
		// ----------接收第一个端口的数据--------------------------------------------------------------------------
		// ----------把第一帧数据的时间作为采样零时刻，与爆炸机发送的确认TB时刻比较--------------------------------
		bStopReceiving = FALSE;			
		// 清数据到达标志
		pRevBuf[0].ResetBuf(pShotClient->m_pSpread->GetCount());
		
		// 等待采样控制线程发送开始采样命令，向测网发送TB、TE等；
		// 必须等待发送TE命令，m_ShotTime.m_dwEndTEH才能够有值
		// WaitForSingleObject(pThread->m_pRevEvent->m_hObject,INFINITE);
		hEvents[0] = pThread->m_pRevEvent->m_hObject;
		hEvents[1] = pThread->m_pShotStopEvent->m_hObject;
		hEvents[2] = pThread->m_pExitEvent->m_hObject;
		dwEvent = WaitForMultipleObjects(3,hEvents,FALSE,INFINITE);				
		if(dwEvent==WAIT_OBJECT_0+1)
			bStopReceiving = TRUE;// 停止采样
		else if(dwEvent==WAIT_OBJECT_0+2)
			continue; // 退出线程

		// 记录当前测网系统时间
		dwFDUSystemTime = theApp.GetFDULocalTime();	
		// 记录本次采样停止时间，为保证数据接收，在测网停止采样时刻TE基础上加1秒（对应测网时间为4096）
		dwSampleTE = pThread->m_ShotTime.m_dwEndTEH + 4096;	
		// 循环结束条件：用户终止采样或者测网系统时间已经大于终止采样时间TE		
		while(!bStopReceiving && dwFDUSystemTime <= dwSampleTE )		
		{
			TRACE2("1 FDU Time:%d; TEH:%d\n",dwFDUSystemTime,pThread->m_ShotTime.m_dwEndTEH);
			dwFDUSystemTime = theApp.GetFDULocalTime();
			// 需要查询的Socket数组,Select函数需要
			readfds.fd_count = 2;
			readfds.fd_array[0]=pRevSocket[0];
			readfds.fd_array[1]=pRevSocket[1]; 
			// 判断两个端口是否有数据
			nValidSocket = select(0,&readfds,NULL,NULL,&timeout);
			if(nValidSocket <=0)
			{
				// 没有收到数据，判断是否要停止采集或者退出线程
				hEvents[0] = pThread->m_pExitEvent->m_hObject;
				hEvents[1] = pThread->m_pShotStopEvent->m_hObject;
				dwEvent = WaitForMultipleObjects(2,hEvents,FALSE,0);				
				if(dwEvent==WAIT_OBJECT_0 || dwEvent==WAIT_OBJECT_0+1)
					bStopReceiving = TRUE;
				// 结束本次循环，开始下一次循环
				continue;
			}
			if(FD_ISSET(pRevSocket[0],&readfds))
			{	// 接收第一个端口数据			
				pThread->RecvSampleData(pRevSocket[0],&pRevBuf[0]);				
			}
			if(FD_ISSET(pRevSocket[1],&readfds))
			{	// 第二个端口有数据，表示第一个端口数据已经发送完毕			
				break;
			}
		} // 第一个端口的数据处理完毕
		if(bStopReceiving)
		{
			RevFile.Close();
			pThread->WriteLogMsg(_T("ShotReceiveProc"),_T("Stop to receive data!"),LOGTYPE_NORMAL,TRUE);
			continue;
		}
		// ----------------------------------------------------------------------------------------------------------------------
		// ------------从第二帧数据开始，采用下面的循环接收----------------------------------------------------------------------
		// ----------------------------------------------------------------------------------------------------------------------
		dwFrmCount=2;		// 采样数据帧计数器，相当于某一道的数据帧计数
		NPort[0]=0;
		dwFDUSystemTime = theApp.GetFDULocalTime();
		while( (!bStopReceiving)  && (dwFDUSystemTime<=dwSampleTE) ) //  
		{			
			// 通过帧计数计算出需要用到的端口总数
			if(dwFrmCount<5)
				NPortCount = dwFrmCount+1;
			else
				NPortCount = 6;
			// 依次求出需要使用的端口编号，相临端口相差1，0~7之间循环往复
			for(i=1;i<NPortCount;i++)
			{
				NPort[i] = ( (NPort[i-1]+1)>7 ) ? 0 : (NPort[i-1]+1);				
			}
			TRACE( "NPort = %d,%d,%d,%d,%d,%d\n", NPort[0],NPort[1],NPort[2],NPort[3],NPort[4],NPort[5] );
			
			// 向第N端口发送重发指令
			if(NPortCount>=6)
			{	
				// 保存 NPort[0]端口数据,向NPort[1]、NPort[2]、NPort[3]请求重发
				// TRACE0("pRevBuf[NPort[0]].SaveToFile \n");
				pRevBuf[NPort[0]].SaveToFile(&RevFile,pShotClient->m_pSpread->GetCount());
				// ????????????????测试代码，向客户端发送第一道的采样数据
				pRevBuf[NPort[0]].SendToClient(pThread->m_pClientSocket,0);	
				// 判断其他端口是否需要重发
				if(!pRevBuf[NPort[1]].IsAllOK())
					pThread->SendRetransCmdToSpread(&pRevBuf[NPort[1]],pThread->m_wRevPort[NPort[1]]);			
				if(!pRevBuf[NPort[2]].IsAllOK())
					pThread->SendRetransCmdToSpread(&pRevBuf[NPort[2]],pThread->m_wRevPort[NPort[2]]);
				if(!pRevBuf[NPort[3]].IsAllOK())
					pThread->SendRetransCmdToSpread(&pRevBuf[NPort[3]],pThread->m_wRevPort[NPort[3]]);
			}
			else
			{
				for(i=0;i<NPortCount-2;i++)
				{
					if(!pRevBuf[NPort[i]].IsAllOK())
						pThread->SendRetransCmdToSpread(&pRevBuf[NPort[i]],pThread->m_wRevPort[NPort[i]]);
				}
			}			
			// 准备接收第NPort[NPortCount-2]个端口的采样数据，先清标志,当NPortCount>=6时，始终是NPort[4]			
			pRevBuf[NPort[NPortCount-2]].ResetBuf(pShotClient->m_pSpread->GetCount());
			// 接收第NPort[4]端口采样数据
			pThread->RecvSampleData(pRevSocket[NPort[NPortCount-2]],&pRevBuf[NPort[NPortCount-2]]);
						
			// 处理NPort[4]新端口数据，接收前三个端口重发数据,检查后一个端口数据是否到达
			while( (!bStopReceiving)  && (dwFDUSystemTime<=dwSampleTE) ) //  
			{				
				// 需要使用端口数
				readfds.fd_count = NPortCount;	
				for(i=0;i<NPortCount;i++)
				{
					readfds.fd_array[i] = pRevSocket[NPort[i]];
				}
				nValidSocket = select(0,&readfds,NULL,NULL,&timeout);
				if(nValidSocket <=0)
				{	// 在没有数据时，才更新测网系统时间；确保网络数据被完全接收
					dwFDUSystemTime = theApp.GetFDULocalTime();
					TRACE2("2 FDU Time:%d; TEH:%d\n",dwFDUSystemTime,pThread->m_ShotTime.m_dwEndTEH);
					// 没有收到数据，判断是否要停止采集或者退出线程
					hEvents[0] = pThread->m_pExitEvent->m_hObject;
					hEvents[1] = pThread->m_pShotStopEvent->m_hObject;
					dwEvent = WaitForMultipleObjects(2,hEvents,FALSE,0);				
					if(dwEvent==WAIT_OBJECT_0 || dwEvent==WAIT_OBJECT_0+1)
						bStopReceiving = TRUE;
					continue;
				}
				// 先接收前几个端口数据
				for (i=0;i<NPortCount-1;i++)
				{					
					if(FD_ISSET(pRevSocket[NPort[i]],&readfds))
					{				
						pThread->RecvSampleData(pRevSocket[NPort[i]],&pRevBuf[NPort[i]]);
					}
				}	
				// 判断最后一个端口是否有数据
				if(FD_ISSET(pRevSocket[NPort[NPortCount-1]],&readfds))
				{	// 后一个端口有数，则退出当前端口的数据接收
					if(dwFrmCount>=5)
					{	// 计算下一时刻需要查询的第一个端口编号
						NPort[0] = ( (NPort[0]+1)>7 ) ? 0 : (NPort[0]+1);						
					}
					dwFrmCount++;
					break;;
				}
			}// 接收数据内循环While
		} // 从接收第二帧开始的数据循环接收
		// 判断是否按下Stop键停止采样
		if(bStopReceiving)
		{
			RevFile.Close();
			continue;
		}
		// ----------------------------------------------------------------------------------------------------------------------
		// ----------------------------------------采样结束----------------------------------------------------------------------		
		// 由于每次循环只保存第0组数据，1,2,3,4组的数据在循环处理中没有存盘，将最后的四组数据存盘。		
		pRevBuf[NPort[1]].SaveToFile(&RevFile,pShotClient->m_pSpread->GetCount());
		pRevBuf[NPort[2]].SaveToFile(&RevFile,pShotClient->m_pSpread->GetCount());
		pRevBuf[NPort[3]].SaveToFile(&RevFile,pShotClient->m_pSpread->GetCount());
		pRevBuf[NPort[4]].SaveToFile(&RevFile,pShotClient->m_pSpread->GetCount());	
		RevFile.Close();
		pThread->WriteLogMsg(_T("ShotReceiveProc"),_T("Data sampling end!"),LOGTYPE_NORMAL,FALSE);
		// 采集结束，通知主机客户端				
		pThread->SendGeneralCMDToClient(OPERATION_CMD_RCVED);		
		
	} // end while(TRUE)
	
	return 0L;
}

/**
 * @brief 数据处理线程
 * @note  该线程主要是对采集的原始数据进行处理。
	
	 数据处理步骤：1、先按照TB等确定采样起点，进行数据对齐

 * @param  LPVOID lpParameter，线程管理对象COperaThread的指针
 * @return 返回0，线程退出
 */
UINT __cdecl ShotProcessProc(LPVOID lpParameter)
{
    COperaThread*  pThread = (COperaThread*) lpParameter;
	// 等待事件函数返回的代码
    DWORD		   dwEvent;
	// 需要等待的事件句柄
	HANDLE		   hEvents[3];
	CSEGDFile	   segdFile;
	CFile		   DataFile;
	CString        strTemp;
	CShotClient*   pShotClient=NULL;
	while (true)
	{		
		// 等待两个事件：退出线程或者开始处理
		hEvents[0] = pThread->m_pExitEvent->m_hObject;
		hEvents[1] = pThread->m_pProcessEvent->m_hObject;
		dwEvent = WaitForMultipleObjects(2,hEvents,FALSE,INFINITE);
		switch(dwEvent)
		{
		case WAIT_OBJECT_0 + 0: // 线程退出: m_pExitEvent
			{
				// 线程退出，进行清理
				pThread->WriteLogMsg(_T("ShotProcessProc"),_T("ShotProcessProc Thread Exit!"),LOGTYPE_DEBUG,FALSE);
				return 0L;
			}			
			// break;
		case WAIT_OBJECT_0 + 1: // 开始处理：m_pProcessEvent
			pThread->SetThreadState(THREAD_PROCESS);
			break;
		default:
			continue;
			break;
		}
		// 清除停止放炮的状态
		pThread->m_pShotStopEvent->ResetEvent();
		// 通知客户端数据处理开始
		pThread->SendGeneralCMDToClient(OPERATION_CMD_PROCING);
		// 得到放炮对象的属性
		pShotClient= pThread->GetShotClient();
		// 得到数据文件的路径：来自Config程序中队Disk Record设置
		strTemp = pShotClient->m_strNormalFilePath +_T("\\")+pShotClient->m_strFileName;	// 文件路径		
		// 读取数据原码文件
		if(!DataFile.Open(strTemp,CFile::modeRead))
		{	// 创建文件错误,通知主机
			// TRACE1("Failed to Create file %s !",strTemp);
			strTemp =_T("Failed to Create file") + strTemp;
			pThread->WriteLogMsg(_T("ShotProcessProc"),strTemp,LOGTYPE_ERROR,TRUE);
			continue;
		}
		// 写SEGD文件
		segdFile.SaveSEGDRev21(&DataFile,pThread,&theApp.m_ChannelList);
		// 关闭源码文件
		DataFile.Close();
		// 通知客户端数据处理结束
		pThread->SendGeneralCMDToClient(OPERATION_CMD_PROCED);
		// pThread->m_pOutputEvent->SetEvent();
		pThread->SetThreadState(THREAD_IDLE);	// ????????????????测试代码，设置线程为空闲状态；应该删除
	}
}

/**
 * @brief 数据存储及打印输出线程
 * @note  该线程主要是对处理的结果数据进行打印输出。
 * @param  LPVOID lpParameter，线程管理对象COperaThread的指针
 * @return 返回0，线程退出
 */
UINT __cdecl ShotOutputProc(LPVOID lpParameter)
{
    COperaThread*  pThread = (COperaThread*) lpParameter;	
	// 等待事件函数返回的代码
    DWORD		   dwEvent;
	// 需要等待的事件句柄
	HANDLE		   hEvents[3];
	while (true)
	{
		
		// 等待两个事件：退出线程或者开始存储及打印
		hEvents[0] = pThread->m_pExitEvent->m_hObject;
		hEvents[1] = pThread->m_pOutputEvent->m_hObject;
		dwEvent = WaitForMultipleObjects(2,hEvents,FALSE,INFINITE);
		switch(dwEvent)
		{
		case WAIT_OBJECT_0 + 0: // 线程退出: m_pExitEvent
			{
				// 线程退出，清理内存
				return 0L;
			}			
			// break;
		case WAIT_OBJECT_0 + 1: // 开始处理：m_pOutputEvent
			pThread->SetThreadState(THREAD_OUTPUT);
			break;
		default:
			continue;
			break;
		}
		// 清除停止放炮的状态
		pThread->m_pShotStopEvent->ResetEvent();
		// 通知客户端数据处理开始
		pThread->SendGeneralCMDToClient(OPERATION_CMD_OUTING);
		


		// 通知客户端数据处理结束
		pThread->SendGeneralCMDToClient(OPERATION_CMD_OUTED);		
		pThread->SetThreadState(THREAD_IDLE);
	}
}