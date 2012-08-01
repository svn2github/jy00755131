// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 MATRIXCOMMDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// MATRIXCOMMDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

#ifndef _MatrixCommDll_H
#define _MatrixCommDll_H
#include "Parameter.h"
#include <list>
#include <map>
#include <hash_map>
using std::list;
using std::map;
using std::hash_map;
/*
*	if using C++ Compiler to compile the file, adopting C linkage mode
*/
#ifdef __cplusplus
extern "C" {
#endif

	// according to the control macro, deciding whether export or import functions
#ifdef MATRIXCOMMDLL_EXPORTS
#define MATRIXCOMMDLL_API __declspec(dllexport)
#else
#define MATRIXCOMMDLL_API __declspec(dllimport)
#endif

// 此类是从 MatrixCommDll.dll 导出的
/**
* @class CCommFrame
* @brief 通讯帧基类
*/
class MATRIXCOMMDLL_API CCommFrame
{
public:
	/**
	* @fn CCommFrame(void)
	* @detail CCommFrame构造函数
	*/
	CCommFrame(void);
	/**
	* @fn ~CCommFrame(void)
	* @detail CCommFrame析构函数
	* @note 虚函数
	*/
	virtual ~CCommFrame(void);
public:
	/** 空闲的帧结构体数*/
	unsigned int m_uiCountFree;
	/** 帧结构体数组*/
	m_oCommFrameStruct m_oCommFrameArray[FrameStructNumMax];
	/** 队列资源同步对象*/
	CRITICAL_SECTION m_oSecClientFrame;
	/** 空闲帧结构体队列*/
	list<m_oCommFrameStructPtr> m_olsCommFrameFree;
	/** 等待处理帧结构体*/
	list<m_oCommFrameStructPtr> m_olsCommWorkFrame;
	/** 包的流水号*/
	unsigned int m_uiPacketIndex;
	/** 命令流水号*/
	unsigned int m_uiCmdIndex;
	/** 数据处理缓冲区*/
	char m_cProcBuf[ProcBufferSize];
public:
	/**
	* @fn void OnResetFrameStruct(m_oCommFrameStructPtr pFrameStruct)
	* @detail 重置接收帧结构体
	* @param[in] pFrameStruct 通讯帧指针	
	* @return void
	*/
	void OnResetFrameStruct(m_oCommFrameStructPtr pFrameStruct);
	/**
	* @fn void OnInit(void)
	* @detail 初始化
	* @param[in] void	
	* @return void
	*/
	void OnInit(void);
	/**
	* @fn void OnClose(void)
	* @detail 关闭
	* @param[in] void
	* @return void
	*/
	void OnClose(void);
	/**
	* @fn m_oCommFrameStructPtr GetFreeFrameStruct(void)
	* @detail 得到一个空闲接收帧结构体
	* @param[in] void	
	* @return 返回一个通讯帧指针
	*/
	m_oCommFrameStructPtr GetFreeFrameStruct(void);
	/**
	* @fn void AddFreeFrameStruct(m_oCommFrameStructPtr pFrameStruct)
	* @detail 增加一个空闲接收帧结构体
	* @param[in] pFrameStruct 通讯帧指针
	* @return void
	*/
	void AddFreeFrameStruct(m_oCommFrameStructPtr pFrameStruct);
	/**
	* @fn void OnResetProcBuf(void)
	* @detail 重置数据处理缓冲区
	* @param[in] void
	* @return void
	*/
	void OnResetProcBuf(void);
};
/**
* @class CRecFrame
* @brief 通讯接收帧类
* @note 继承自CCommFrame
*/
class MATRIXCOMMDLL_API CCommRecFrame : public CCommFrame
{
public:
	/**
	* @fn CRecFrame(void)
	* @detail CRecFrame构造函数
	*/
	CCommRecFrame(void);
	/**
	* @fn ~CRecFrame(void)
	* @detail CRecFrame析构函数
	*/
	~CCommRecFrame(void);
public:
	/**
	* @fn void PhraseFrame(char* cpFrame, unsigned short usSize)
	* @detail 解析帧
	* @param[in] cpFrame 缓冲区指针
	* @param[in] usSize 需要处理的字节数
	* @return void
	*/
	void PhraseFrame(char* cpFrame, unsigned short usSize);
};
/**
* @class CSndFrame
* @brief 通讯发送帧类
*/
class CClientSocket;
class MATRIXCOMMDLL_API CCommSndFrame : public CCommFrame
{
public:
	CCommSndFrame(void);
	~CCommSndFrame(void);
public:
	/** 客户端Socket类指针*/
	CClientSocket* m_pClientSocket;
	/** 客户端连接有效标志符*/
	bool m_bConnectValid;
	/**
	* @brief 已发送帧索引关键字
	*/
	typedef struct SndFrameKey
	{
		SndFrameKey(unsigned short usCmd, unsigned int uiTimeStep, unsigned int uiPacketIndex)
		{
			this->m_usCmd = usCmd;
			this->m_uiServerTimeStep = uiTimeStep;
			this->m_uiPacketIndex = uiPacketIndex;
		}
		~SndFrameKey()
		{
		}
		bool operator < (const SndFrameKey& rhs) const
		{
			if (m_usCmd == rhs.m_usCmd)
			{
				if (m_uiServerTimeStep == rhs.m_uiServerTimeStep)
				{
					return(m_uiPacketIndex < rhs.m_uiPacketIndex);
				} 
				else
				{
					return(m_uiServerTimeStep < rhs.m_uiServerTimeStep);
				}
			}
			else
			{
				return (m_usCmd < rhs.m_usCmd);
			}
		}
		/** 帧命令字，表明帧的功能*/
		unsigned short m_usCmd;
		/** 服务端时间戳*/
		unsigned int m_uiServerTimeStep;
		/** 包流水号*/
		unsigned int m_uiPacketIndex;
	}m_oSndFrameKey;
	/** 已发送帧索引*/
	map<m_oSndFrameKey, m_oCommFrameStructPtr> m_oSndFrameMap;
public:
	// 生成应答帧
	void MakeReturnFrame(m_oCommFrameStructPtr ptrFrame);
	// 生成设置帧
	virtual void MakeSetFrame(unsigned short usCmd, char* pChar, unsigned int uiSize);
	// 生成发送帧
	void MakeSendFrame(m_oCommFrameStructPtr ptrFrame);
	// 重发过期帧
	bool OnReSendFrame(void);
	// 判断索引号是否已加入索引表
	BOOL IfFramePtrExistInSndMap(unsigned short m_usCmd, unsigned int m_uiServerTimeStep, 
		unsigned int m_uiPacketIndex, map<m_oSndFrameKey, m_oCommFrameStructPtr>* pMap);
	// 增加对象到索引表
	void AddFramePtrToSndMap(unsigned short m_usCmd, unsigned int m_uiServerTimeStep, 
		unsigned int m_uiPacketIndex, m_oCommFrameStructPtr pFrameStruct, 
		map<m_oSndFrameKey, m_oCommFrameStructPtr>* pMap);
	// 根据输入索引号，由索引表得到指针
	m_oCommFrameStructPtr GetFramePtrFromSndMap(unsigned short m_usCmd, unsigned int m_uiServerTimeStep, 
		unsigned int m_uiPacketIndex, map<m_oSndFrameKey, m_oCommFrameStructPtr>* pMap);
	// 从索引表删除索引号指向的仪器指针
	BOOL DeleteFramePtrFromSndMap(unsigned short m_usCmd, unsigned int m_uiServerTimeStep, 
		unsigned int m_uiPacketIndex, map<m_oSndFrameKey, m_oCommFrameStructPtr>* pMap);
};
/**
* @class CCommThread
* @brief 通讯线程基类
*/
class MATRIXCOMMDLL_API CCommThread
{
public:
	/**
	* @fn CCommThread(void)
	* @detail CCommThread构造函数
	*/
	CCommThread(void);
	/**
	* @fn CCommThread(void)
	* @detail CCommThread析构函数
	* @note 虚函数
	*/
	virtual ~CCommThread(void);
protected:
	/** 线程ID*/
	DWORD m_dwThreadID;
	/** 线程关闭事件*/
	HANDLE m_hThreadClose;
	/** 线程关闭标志位*/
	bool m_bClose;
public:
	/** 线程句柄*/
	HANDLE m_hThread;
	/** 线程函数等待一次的时间*/
	unsigned int m_uiThreadSleepTime;
	/** 线程函数等待的次数*/
	unsigned int m_uiThreadSleepCount;
public:
	/**
	* @fn void OnInit(void)
	* @detail 初始化
	*/
	void OnInit(void);
	/**
	* @fn void OnClose(void)
	* @detail 关闭
	*/
	void OnClose(void);
	/**
	* @fn DWORD ThreadRunFunc(void)
	* @detail 线程函数
	* @return DWORD
	*/
	DWORD ThreadRunFunc(void);
	/**
	* @fn void WaitForThread(void)
	* @detail 线程等待函数
	*/
	void WaitForThread(void);
	/**
	* @fn void OnProc(void)
	* @detail 处理函数
	* @note 虚函数
	*/
	virtual void OnProc(void);
};
/**
* @brief 仪器区域结构体
*/
typedef struct AreaStruct
{
	/** 线号，从1开始*/
	unsigned int m_uiLineNb;
	/** 区域号，从1开始*/
	unsigned int m_uiAreaNb;
	bool operator == (const AreaStruct& rhs) const
	{
		return ((m_uiLineNb == rhs.m_uiLineNb) && (m_uiAreaNb == rhs.m_uiAreaNb));
	}
	bool operator < (const AreaStruct& rhs) const
	{
			if (m_uiLineNb == rhs.m_uiLineNb)
		{
			return (m_uiAreaNb < rhs.m_uiAreaNb);
		}
		else
		{
			return (m_uiLineNb < rhs.m_uiLineNb);
		}
	}
}m_oAreaStruct;

/**
* @struct InstrumentLocation_Struct
* @brief 仪器位置点结构体
*/
typedef struct Location_Struct
{
	Location_Struct(int iLineIndex, int iPointIndex)
	{
		this->m_iLineIndex = iLineIndex;
		this->m_iPointIndex = iPointIndex;
	}
	~Location_Struct()
	{
	}
	bool operator == (const Location_Struct& rhs) const
	{
		return ((m_iLineIndex == rhs.m_iLineIndex) && (m_iPointIndex == rhs.m_iPointIndex));
	}
	bool operator < (const Location_Struct& rhs) const
	{
		if (m_iLineIndex == rhs.m_iLineIndex)
		{
			return (m_iPointIndex < rhs.m_iPointIndex);
		}
		else
		{
			return (m_iLineIndex < rhs.m_iLineIndex);
		}
	}
	/** 线号*/
	int m_iLineIndex;
	/** 点号*/
	int m_iPointIndex;
}m_oLocationStruct;
/**
* @class CCommRecThread
* @brief 通讯接收线程类
* @note 继承自CCommThread
*/
class CCommClient;
class CCommRecThread;
/** 回调函数-接收数据处理*/
typedef void (CALLBACK* ProcRecCmdCallBack)(unsigned short usCmd, char* pChar, 
	unsigned int uiSize, CCommRecThread* pRecThread);
class MATRIXCOMMDLL_API CCommRecThread : public CCommThread
{
public:
	/**
	* @fn CCommRecThread(void)
	* @detail CCommRecThread构造函数
	*/
	CCommRecThread(void);
	/**
	* @fn ~CCommRecThread(void)
	* @detail CCommRecThread析构函数
	*/
	~CCommRecThread(void);
public:
	/** 接收帧类指针*/
	CCommRecFrame* m_pCommRecFrame;
	/** 发送帧类指针*/
	CCommSndFrame* m_pCommSndFrame;
	/** 连接客户端索引指针*/
	hash_map<SOCKET, CCommClient*>* m_pComClientMap;
	/** 回调函数-接收数据处理*/
	ProcRecCmdCallBack m_oProcRecCmdCallBack;
	/** 连接验证*/
	bool m_bCheckConnected;
 	/** 客户端设备位置索引表*/
 	map<m_oLocationStruct, unsigned int> m_oInstrumentWholeTableMap;
	/** 客户端设备更新区域索引表*/
	map<m_oAreaStruct, m_oAreaStruct> m_oInstrumentUpdateArea;
private:
	/** 客户端验证时间次数计数*/
	unsigned int m_uiClientCheckCount;
		/** 客户端活跃时间间隔计数*/
	unsigned int m_uiClientActiveCount;
public:
	/**
	* @fn void OnProc(void)
	* @detail 处理函数
	* @param[in] void
	* @return void
	*/
 	void OnProc(void);
	/**
	* @fn void SaveRecFrameToTask(m_oCommFrameStructPtr ptrFrame)
	* @detail 将帧内容加入待处理任务中
	* @param[in] ptrFrame 通讯帧指针
	* @return void
	*/
	void SaveRecFrameToTask(m_oCommFrameStructPtr ptrFrame);
	/**
	* @fn void OnProcRecCmd(unsigned short usCmd, char* pChar, unsigned int uiSize)
	* @detail 处理接收命令函数
	* @param[in] usCmd 命令字
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 待处理的缓冲区字节数
	* @return void
	*/
	virtual void OnProcRecCmd(unsigned short usCmd, char* pChar, unsigned int uiSize);
	// 判断仪器位置索引号是否已加入索引表
	virtual BOOL IfLocationExistInMap(int iLineIndex, int iPointIndex);
	// 增加对象到索引表
	virtual void AddLocationToMap(int iLineIndex, int iPointIndex, unsigned int uiSN);
	// 根据输入索引号，由索引表得到仪器指针
	virtual unsigned int* GetSnPtrFromLocationMap(int iLineIndex, int iPointIndex);
	// 向所有在线客户端广播配置文件变更
	virtual void BroadCastXMLChange(unsigned short usCmd, char* pChar, unsigned int uiSize);
	// 监视客户端是否活跃
	void MonitorClientActive(bool bActive);

};
/**
* @class CCommSndThread
* @brief 通讯发送线程类
*/
class MATRIXCOMMDLL_API CCommSndThread : public CCommThread
{
public:
	CCommSndThread(void);
	~CCommSndThread(void);
public:
	/** 发送帧类指针*/
	CCommSndFrame* m_pCommSndFrame;
public:
	// 处理函数
	void OnProc(void);
};
/**
* @class CCommHeartBeatThread
* @brief 通讯心跳线程类
*/
class MATRIXCOMMDLL_API CCommHeartBeatThread : public CCommThread
{
public:
	CCommHeartBeatThread(void);
	~CCommHeartBeatThread(void);
public:
	/** 发送帧类指针*/
	CCommSndFrame* m_pCommSndFrame;
public:
	// 处理函数
	void OnProc(void);
};
/**
* @class CClientSocket
* @brief 从Dll导出的Socket类
*/
class MATRIXCOMMDLL_API CClientSocket : public CAsyncSocket
{
public:
	/**
	* @fn CCommSocket(void)
	* @detail 构造函数
	*/
	CClientSocket();
	/**
	* @fn ~CCommSocket(void)
	* @detail 析构函数
	*/
	virtual ~CClientSocket();
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
public:
	/** 接收缓冲区*/
	char m_cRecBuf[ServerRecBufSize];
	/** 发送缓冲区*/
	char m_cSndBuf[FrameSizeLimit];
	/** 接收点坐标*/
	int m_iPosRec;
	/** 处理点坐标*/
	int m_iPosProc;
	/** 帧内容长度*/
	unsigned short m_usFrameInfoSize;
	/** 连接客户端类指针*/
	CCommClient* m_pComClient;
	/** 服务器IP地址*/
	CString m_strServerIP;
	/** 服务器端口*/
	unsigned int m_uiServerPort;
public:
	// 初始化
	void OnInit(CCommClient* pComClient,int iSndBufferSize, int iRcvBufferSize);
	/**
	* @fn void SetSocketBuffer(SOCKET s, int iSndBufferSize, int iRcvBufferSize)
	* @detail 设置Socket缓冲区大小
	*/
	virtual void SetSocketBuffer(int iSndBufferSize, int iRcvBufferSize);
	// 关闭
	void OnClose();
	// 处理接收帧
	void OnProcRec(int iSize);
	/** 连接服务器*/
	void ConnectServer();
};

/**
* @class CCommClient
* @brief 与服务器连接的客户端成员类
*/
class MATRIXCOMMDLL_API CCommClient
{
public:
	/**
	* @fn CCommClient(void)
	* @detail 构造函数
	*/
	CCommClient();
	/**
	* @fn ~CCommClient(void)
	* @detail 析构函数
	*/
	~CCommClient();
public:
	/** 接收帧成员类*/
	CCommRecFrame m_oRecFrame;
	/** 接收帧的处理线程成员类*/
	CCommRecThread m_oRecThread;
	/** 发送帧成员类*/
	CCommSndFrame m_oSndFrame;
	/** 发送帧的处理线程成员类*/
	CCommSndThread m_oSndThread;
	/** 发送客户端心跳帧线程成员类*/
	CCommHeartBeatThread m_oHeartBeatThread;
	/** 接收客户端通讯成员类*/
	CClientSocket m_oClientSocket;
// 	/** DLL函数调用类成员*/
// 	CMatrixDllCall* m_pMatrixDllCall;
	/** 连接客户端索引指针*/
	hash_map<SOCKET, CCommClient*>* m_pComClientMap;
	/** 回调函数-接收数据处理*/
	ProcRecCmdCallBack m_oProcRecCmdCallBack;
public:
	// 创建一个客户端连接信息
	virtual void OnInit(bool bClient = false);
	// 释放一个客户端连接信息
	virtual void OnClose();
};

/**
* @class CCommServer
* @brief 服务端的Socket类
*/
class MATRIXCOMMDLL_API CCommServer : public CAsyncSocket
{
public:
	CCommServer();
	virtual ~CCommServer();
	virtual void OnAccept(int nErrorCode);
public:
	/** 客户端连接索引*/
	hash_map<SOCKET, CCommClient*>* m_pComClientMap;
	/** 需要关闭的客户端指针队列*/
	list<CCommClient*> m_olsClientClose;
	/** 回调函数-接收数据处理*/
	ProcRecCmdCallBack m_oProcRecCmdCallBack;
public:
	// 初始化
	virtual void OnInit(unsigned int uiSocketPort = ServerListenPort, int iSocketType = SOCK_STREAM, LPCTSTR lpszSocketAddress = NULL);
	// 关闭
	virtual void OnClose(void);
	/** 关闭无效的客户端连接*/
	void CloseInvalidClient(void);
};
/**
* @class CMatrixCommDll
* @brief 从Dll导出的通讯接口类
*/
class MATRIXCOMMDLL_API CMatrixCommDll {
public:
	/**
	* @fn CMatrixCommDll()
	* @detail 构造函数
	*/
	CMatrixCommDll();
	/**
	* @fn ~CMatrixCommDll()
	* @detail 析构函数
	*/
	~CMatrixCommDll();
public:
	/** 客户端连接索引*/
	hash_map<SOCKET, CCommClient*> m_oComClientMap;
public:
	// TODO: 在此添加您的方法。
	/**
	* @fn CCommClient* CreateCommClient(void)
	* @detail 创建客户端
	*/
	virtual CCommClient* CreateCommClient(void);
	/**
	* @fn void DeleteCommClient(CCommClient* pClass)
	* @detail 释放客户端
	*/
	virtual void DeleteCommClient(CCommClient* pClass);
	/**
	* @fn CCommServer* CreateCommServer(void)
	* @detail 创建客户端
	*/
	virtual CCommServer* CreateCommServer(void);
	/**
	* @fn void DeleteCommServer(CCommServer* pClass)
	* @detail 释放客户端
	*/
	virtual void DeleteCommServer(CCommServer* pClass);
	/** 初始化*/
	virtual void OnInit(void);
	/** 关闭*/
	virtual void OnClose(void);
};

extern MATRIXCOMMDLL_API int nMatrixCommDll;

MATRIXCOMMDLL_API int fnMatrixCommDll(void);

MATRIXCOMMDLL_API CMatrixCommDll* CreateMatrixCommDll(void);
MATRIXCOMMDLL_API void DeleteMatrixCommDll(CMatrixCommDll* pClass);
#undef MATRIXCOMMDLL_API
#ifdef __cplusplus
}
#endif

#endif