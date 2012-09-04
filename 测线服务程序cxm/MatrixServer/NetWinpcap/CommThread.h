#pragma once
class CCommThread
{
public:
	CCommThread(void);
	~CCommThread(void);
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

