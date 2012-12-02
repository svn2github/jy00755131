#pragma once
typedef void(CALLBACK* ONUARTREAD)(void* pFatherPtr /** 父对象指针*/
	, BYTE buf /** 接收数据*/);
class CUart
{
public:
	CUart(void);
	~CUart(void);
private:
	// 已打开的串口句柄
	HANDLE m_hComm;
	// 读线程句柄
	HANDLE m_hReadThread;
	// 读线程退出事件句柄
	HANDLE m_hReadCloseEvent;
	// 读线程ID标识
	DWORD m_dwReadThreadID;
	// 串口打开标志位
	bool m_bOpened;
	// 指定父对象指针
	void* m_pFatherPtr;
public:
	// 串口读取数据回调函数
	ONUARTREAD m_OnUartRead;
private:
	// 串口读线程函数
	static DWORD WINAPI ReadThreadProc(LPVOID lparam);
	// 关闭读线程
	void CloseReadThread(void);
public:
	// 打开串口
	BOOL OpenPort(void* pFatherPtr	/** 指向父指针*/
		, UINT uiPortNo = 1		/** 串口号*/
		, UINT uiBaud = 9600	/** 波特率*/
		, UINT uiParity = NOPARITY	/** 奇偶校验*/
		, UINT uiDatabits = 8	/** 数据位*/
		, UINT uiStopbits = 0);	/** 停止位*/
	// 关闭串口
	void ClosePort(void);
	// 往串口写入数据
	BOOL WriteSyncPort(const BYTE* pbuf, DWORD dwbufLen);
	// 设置串口读取、写入超时
	BOOL SetUartTimeouts(COMMTIMEOUTS CommTimeOuts);
	// 得到串口是否打开
	BOOL GetComOpened(void);
};
