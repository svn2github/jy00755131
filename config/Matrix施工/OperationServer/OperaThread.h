#pragma once
#include "..\common\OperaStruct.h"
#include "ShotController.h"
#include "FDUSocket.h"
#include "ClientSocket.h"
#include "SocketShotBlaster.h"


// 定义线程的状态
#define  THREAD_IDLE	   0
#define  THREAD_SHOT	   1
#define  THREAD_SAMPLE	   2
#define  THREAD_PROCESS	   3
#define  THREAD_OUTPUT	   4

//////////////////////////////////////////////////////////////////////////
// 定义每个数据采样帧的采样数据字节数
#define  SAMPFRM_DATALEN   222

///////////////////////////////////////////////////////////////////////////
/**
 * @class CRevBuffer
 * @brief 数据采集缓冲区
 *
 * 用于多道采集的数据缓存，缓冲区按照最大容量2万道分配。
 */
class CRevBuffer
{
public:
	CRevBuffer(void);
	~CRevBuffer(void);
private:
	/** 当前所保存的AD数据对应采集道的帧指针 */
	WORD		   m_wFrmIndex;
	/** 接收缓冲区 */
	unsigned char  m_RevBuf[FDU_MAXINUM][SAMPFRM_DATALEN];
	/** 数据是否接收的标志缓冲区 */
	unsigned char  m_RevFlagBuf[FDU_MAXINUM];
	/** 表示是否所有的数据都收到，仅仅是为缓存，优化查询速度 */
	BOOL		   m_bAllRev;

	/** 将采集站数据发送给客户端的缓冲区 */
	unsigned char  m_pBuf[246];
public:
	// 读取AD数据帧指针
	WORD GetFrmIndex()	{return m_wFrmIndex;};
	/** 设置帧索引号 */
	void SetFrmIndex(WORD wFrmIndex){ m_wFrmIndex = wFrmIndex; };
	/** 设置是否所有的数据都收到 */
	void SetIsAllOK(BOOL bOK){m_bAllRev=bOK;};
	BOOL IsAllOK(){ return m_bAllRev; };
	/** 清标志缓冲区 */
	void ResetBuf(int nFDUCount);
	// 读标志
	unsigned char GetRevFlag(int nIndex){ if(nIndex<FDU_MAXINUM) return m_RevFlagBuf[nIndex]; else return 1;};
	// 清除缓冲区指定位置的数据
	void ClearBuf(int nIndex);
	// 将数据写入缓冲区中
	bool WriteData(int nIndex,unsigned char* pBuf,int nSize);
	int ReadData(int nIndex,unsigned char* pBuf,int nSize);
	// 将缓冲区数据写入文件
	int  SaveToFile(CFile* pFile,int nFDUCount);
	// 将数据发送到客户端
	int  SendToClient(CSocket*	pSocket,int nFDUIndex);
};
//////////////////////////////////////////////////////////////////////////
/**
 * @class CShotTime
 * @brief 施工时间属性管理类
 *
 * 用于管理一次施工所需要的时间属性：TB时间、TE时间、确认TB时间等；
 */
class CShotTime
{
public:
	CShotTime();
	~CShotTime();

public:
	// ---------以下时间变量的量纲是测量系统时间---------
	/** 发送给采集站的TB高位，是采样启动时刻，单位毫秒(ms) */
	DWORD			m_dwStartTBH;
	/** 发送给采集站的TB低位，是采样启动时刻，单位毫秒(ms) */
	DWORD			m_dwStartTBL;	
	/** 采样结束时间TE高位，单位毫秒(ms) */
	DWORD			m_dwEndTEH;
	/** 采样结束时间TE低位，单位毫秒(ms) */
	DWORD			m_dwEndTEL;

	/** 接收的来自爆炸机控制器的确认TB时刻，单位毫秒(ms) */
	//float			m_fConfirmedTB;
	/** 井口时间，单位毫秒(ms)，以0.5 毫秒步幅  */
	//float			m_fUpholeTime;
	// ---------以下时间变量的量纲是毫秒（ms）---------
	// DWORD			m_dwRecordLen;
public:	
	static void	GetFDUTimeHighAndTimeLow(DWORD dwFDUCurrentTime,DWORD dwmsAddTime,DWORD& dwFDUTimeH,DWORD& dwFDUTimeL);
};
//////////////////////////////////////////////////////////////////////////
/**
 * @class COperaThread
 * @brief 施工线程管理类
 *
 * 用于管理一组施工线程及线程所需的参数属性。一组施工线程包括：
 放炮线程、数据采集线程、数据处理线程及数据IO线程。
 */
class COperaThread
{
public:
	COperaThread(void);
	~COperaThread(void);

public:
	/** 服务器与客户端通信的标识ID，在帧格式中唯一标识一对客户与服务器 */
	DWORD		m_dwServerID;
	// ---------通信------------------------------------
	/** 施工服务器IP地址 */
	DWORD       m_dwLocalIP;
	/** 缓存8个端口 */
	WORD		m_wRevPort[8];
	/** 与测网通信，发送点名、采样命令; 由该线程管理对象负责创建并释放。 */
	CSocketFDU* m_pFDUSocket;

	/** 与施工客户端通信socket，四组线程共用一个对象，不需要该线程释放，由主对话框类创建释放。 */
	CSocketShotClient* m_pClientSocket;
	
	// ---------线程事件---------------------------------
	
    /** 线程退出事件 */
	CEvent*		  m_pExitEvent;
	/** 放炮开始事件 */
	CEvent*		  m_pShotEvent;
	/** 放炮GO事件 */
	CEvent*		  m_pShotGOEvent;
	/** 放炮终止事件 */
	CEvent*		  m_pShotStopEvent;
	/** 采集开始事件 */
	CEvent*		  m_pRevEvent;
	/** 采集结束事件 */
	CEvent*		  m_pRevEndEvent;
	/** 开始处理事件 */
	CEvent*		  m_pProcessEvent;
	/** 开始存储打印事件 */
	CEvent*		  m_pOutputEvent;

	/** 爆炸机控制器，与线程管理对象一起创建，线程对象退出时释放 */
	CShotController*  m_pShotCtrl;
	
	/** 爆炸时间管理对象 */
	CShotTime		m_ShotTime;
	
   
private:
	
	// ---------需要放炮的测网参数---------------------------------
	/** 保存放炮所需的参数，每放完一炮后释放该对象 */
	CShotClient*	 m_pShotClient;

	//  ---------需要管理的四个线程----------------------------
	/** 放炮线程指针 */
	CWinThread* m_pShotThread;
	/** 接收线程 */
	CWinThread* m_pReceiveThread;
	/** 数据处理线程 */
	CWinThread* m_pProcessThread;
	/** 存储、输出打印线程 */
	CWinThread* m_pOutputThread;

	/** 线程组的编号，用于标识第几组线程，从0开始 */
	DWORD       m_dwThreadID;

	/** 线程状态 0, 1, 2 */
	unsigned char m_ThreadState;
private:
	// 创建并启动线程
	bool StartThread(void);
	bool EndThread(void);	
	bool CreateFDUSocket(DWORD dwIP, WORD wPort);
public:
	// 得到线程ID
	DWORD	GetShotThreadID(void)	{return m_dwThreadID;};
	// 得到线程状态
	unsigned char GetThreadState()	{ return m_ThreadState; };
	// 设置线程状态
	void SetThreadState(unsigned char ThreadState)	{ m_ThreadState= ThreadState; };
	// unsigned int GetThreadID()	{ return m_dwThreadID; };
	// 创建该对象后首先被调用
	bool Create(WORD wThreadID, DWORD dwHostIP,CSocketShotClient* pClientSocket,CSocketShotBlaster* pBlasterSocket);
	// 删除对象时被调用
	void Destory(void);
	// 每次放炮结束时调用，删除对象
	bool ResetShotParam();
	//重新启动这次放炮
	bool RestartShot();
	// 写日志
	void    WriteLogMsg(LPCTSTR strPosition,LPCTSTR strDesc,WORD wType,BOOL bShowed);
	// 每次放炮开始前调用，初始化参数
	void SetShotClient( CShotClient* pShotClient);
	void SetShotProcess( CProcessType* pProcessType);
	CShotClient*	GetShotClient(void)		{ return m_pShotClient;};
	// CShotSource*	GetShotClient(void)		{ return m_pShotClient;};

	// ---------与采集排列通信函数---------------------
	bool ClearRevBufOfSocket(SOCKET sock);
	bool SendConfigCmdToSpread(WORD wBroadcastPort);
	void SendBroadcastPortCmdToSpread(WORD wBroadcastPort);
//	bool SendCallCmdToSpread(void);	
	long RecvAckedFrmOfCall(long lAckedType);
	int  RecvSampleData(SOCKET RevSocket, CRevBuffer *pBuf);
	int  SendRetransCmdToSpread(CRevBuffer *pRevBuf,WORD wPort);
	// 向采集排列发送TE命令，停止噪声采集
	bool SendTEBroadcastCmdToSpread(DWORD dwFDUSysTime);
	// 向采集排列广播采样参数：TB、TE、滤波器参数
	bool SendSampParamBroadcastCmdToSpread(DWORD dwFDUSysTime,DWORD dwDelaySampleMS);
	
	// -------向客户端发送命令-----------------
	bool SendCallNoResponseToClient(void);
	int  SendGeneralCMDToClient(WORD  wCmdCode);
	int  SendBlasterState(void);
};
