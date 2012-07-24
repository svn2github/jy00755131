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

using std::list;
using std::map;
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
class CCommSocket;
class MATRIXCOMMDLL_API CCommSndFrame : public CCommFrame
{
public:
	CCommSndFrame(void);
	~CCommSndFrame(void);
public:
	/** Socket类指针*/
	CCommSocket* m_pCommSocket;
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
	void MakeSetFrame(unsigned short usCmd, char* pChar, unsigned int uiSize);
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
public:
	/** 线程句柄*/
	HANDLE m_hThread;
	/** 线程ID*/
	DWORD m_dwThreadID;
	/** 线程关闭事件*/
	HANDLE m_hThreadClose;
	/** 线程关闭标志位*/
	bool m_bClose;
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
* @class CCommRecThread
* @brief 通讯接收线程类
* @note 继承自CCommThread
*/
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
* @class CCommSocket
* @brief 从Dll导出的Socket类
*/
class MATRIXCOMMDLL_API CCommSocket : public CAsyncSocket
{
public:
	/**
	* @fn CCommSocket(void)
	* @detail 构造函数
	*/
	CCommSocket();
	/**
	* @fn ~CCommSocket(void)
	* @detail 析构函数
	*/
	virtual ~CCommSocket();
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
	/** 接收帧*/
	CCommRecFrame m_oRecFrame;
	/** 发送帧*/
	CCommSndFrame m_oSndFrame;
	/** 接收线程*/
	CCommRecThread m_oRecThread;
	/** 发送线程*/
	CCommSndThread m_oSndThread;
public:
	// 处理接收帧
	void OnProcRec(int iSize);
	// 初始化
	void OnInit();
	// 关闭
	void OnClose();
};

/**
* @class CCommClient
* @brief 从Dll导出的客户端Socket类
*/
class MATRIXCOMMDLL_API CCommClient : public CCommSocket
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
	virtual void OnConnect(int nErrorCode);
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
	// TODO: 在此添加您的方法。
	/**
	* @fn void OnInitSocketLib(void)
	* @detail 初始化套接字库
	*/
	virtual void OnInitSocketLib(void);
	/**
	* @fn void OnCloseSocketLib(void)
	* @detail 释放套接字库
	*/
	virtual void OnCloseSocketLib(void);
	/**
	* @fn void CreateSocket(CAsyncSocket* pSocket, unsigned int uiSocketPort, long lEvent)
	* @detail 创建客户端套接字
	*/
	virtual void CreateSocket(CAsyncSocket* pSocket, unsigned int uiSocketPort, long lEvent);
	/**
	* @fn void SetSocketBuffer(SOCKET s, int iSndBufferSize, int iRcvBufferSize)
	* @detail 设置Socket缓冲区大小
	*/
	virtual void SetSocketBuffer(SOCKET s, int iSndBufferSize, int iRcvBufferSize);
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