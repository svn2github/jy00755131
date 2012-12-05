#include "StdAfx.h"
#include "Uart.h"

CUart::CUart(void)
	: m_bOpened(false)
	, m_pFatherPtr(NULL)
	, m_dwReadThreadID(0)
{
	m_hComm = INVALID_HANDLE_VALUE;
	m_hReadThread = INVALID_HANDLE_VALUE;
	m_hReadCloseEvent = INVALID_HANDLE_VALUE;
}

CUart::~CUart(void)
{
	if (m_bOpened)
	{
		// 关闭串口
		ClosePort();
	}
}

// 串口读线程函数
DWORD CUart::ReadThreadProc(LPVOID lparam)
{
	CUart* pUart = (CUart*)lparam;
	// 读取数据缓冲区
	BYTE readBuf;
	// 实际读取字节数
	DWORD dwActualReadLen = 0;
	OVERLAPPED wol;
	wol.Internal=0;
	wol.InternalHigh=0;
	wol.Offset=0;
	wol.OffsetHigh=0;
	wol.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
	// 检查串口是否打开
	ASSERT(pUart->m_hComm != INVALID_HANDLE_VALUE);
	// 清空串口接收和发送缓冲区
	PurgeComm(pUart->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);
	while (TRUE)
	{
		ReadFile(pUart->m_hComm, &readBuf, 1, &dwActualReadLen, &wol);
		if (dwActualReadLen == 1)
		{
			// 触发回调函数
			if (pUart->m_OnUartRead)
			{
				pUart->m_OnUartRead(pUart->m_pFatherPtr, readBuf);
			}
		}
		if (WaitForSingleObject(pUart->m_hReadCloseEvent, 10) == WAIT_OBJECT_0)
		{
			break;
		}
	}
	return 0;
}

// 关闭读线程
void CUart::CloseReadThread(void)
{
	SetEvent(m_hReadCloseEvent);
	// 清空所有要读的数据
	PurgeComm(m_hComm, PURGE_RXCLEAR);
	// 如果读线程没有退出则强制退出
	if (WaitForSingleObject(m_hReadThread, 1000) == WAIT_TIMEOUT)
	{
		TerminateThread(m_hReadThread, 0);
	}
	m_hReadThread = NULL;
}

// 打开串口
BOOL CUart::OpenPort(void* pFatherPtr, UINT uiPortNo, UINT uiBaud, UINT uiParity, UINT uiDatabits, UINT uiStopbits)
{
	DCB commParam;
	TCHAR szPortName[16];
	ASSERT(pFatherPtr != NULL);
	m_pFatherPtr = pFatherPtr;
	// 串口已经打开
	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	// 设置串口名，此处需注意一定要有“:”
	wsprintf(szPortName, _T("COM%d:"), uiPortNo);
	// 打开串口
	m_hComm = CreateFile(
		szPortName,
		GENERIC_READ | GENERIC_WRITE, // 允许读写
		0,
		NULL,
		OPEN_EXISTING, // 打开而不是创建
		0,
		NULL
		);
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	// 得到打开串口的当前属性参数，修改后再重新设置串口
	if (!GetCommState(m_hComm, &commParam))
	{
		// 关闭串口
		CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		return FALSE;
	}
	// 设置串口参数
	commParam.BaudRate = uiBaud;			// 设置波特率
	commParam.fBinary = TRUE;				// 设置二进制模式，此处必须设置为TRUE
	commParam.fParity = TRUE;				// 支持奇偶校验
	commParam.ByteSize = uiDatabits;		// 数据位，范围4-8
	commParam.Parity = uiParity;			// 校验模式
	commParam.StopBits = uiStopbits;		// 停止位

	commParam.fOutxCtsFlow = FALSE;			// No CTS output flow control
	commParam.fOutxDsrFlow = FALSE;			// No DSR output flow control
	commParam.fDtrControl = DTR_CONTROL_ENABLE;
	// DTR flow control type
	commParam.fDsrSensitivity = FALSE;		// DSR Sensitivity
	commParam.fTXContinueOnXoff = TRUE;		// XOFF continues TX
	commParam.fOutX = FALSE;				// No XON/XOFF out flow control
	commParam.fInX = FALSE;					// No XON/XOFF in flow control
	commParam.fErrorChar = FALSE;			// Disable error replacement
	commParam.fNull = FALSE;				// Disable null stripping
	commParam.fRtsControl = RTS_CONTROL_ENABLE;
	// RTS flow control
	commParam.fAbortOnError = FALSE;		// 当串口发生错误时并不终止串口读写

	if (!SetCommState(m_hComm, &commParam))
	{
		CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		return FALSE;
	}

	// 设置串口读写时间
	COMMTIMEOUTS CommTimeOuts;
	GetCommTimeouts(m_hComm, &CommTimeOuts);
	CommTimeOuts.ReadIntervalTimeout = MAXDWORD;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 50;
	CommTimeOuts.WriteTotalTimeoutConstant = 2000;
	if (!SetCommTimeouts(m_hComm, &CommTimeOuts))
	{
		CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		return FALSE;
	}
	// 设置串口设备缓冲区
	SetupComm(m_hComm, 512, 512);
	// 清空接收和发送缓冲区
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);
	CString strEvent;
	strEvent.Format(_T("Com_ReadCloseEvent%d"), uiPortNo);
	m_hReadCloseEvent = CreateEvent(NULL, TRUE, FALSE, strEvent);

	// 创建串口读数据接收线程
	m_hReadThread = CreateThread(NULL, 0, ReadThreadProc, this, 0, &m_dwReadThreadID);
	m_bOpened = TRUE;
	return TRUE;
}

// 关闭串口
void CUart::ClosePort(void)
{
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return;
	}
	// 关闭读线程
	CloseReadThread();
	// 关闭串口
	CloseHandle(m_hComm);
	m_hComm = INVALID_HANDLE_VALUE;
	// 关闭事件
	CloseHandle(m_hReadCloseEvent);
	m_bOpened = FALSE;
}

// 往串口写入数据
void CUart::WriteSyncPort(const BYTE buf)
{
	ASSERT(m_hComm != INVALID_HANDLE_VALUE);
	OVERLAPPED wol;
	wol.Internal=0;
	wol.InternalHigh=0;
	wol.Offset=0;
	wol.OffsetHigh=0;
	wol.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
	WriteFile(m_hComm, &buf, 1, NULL, &wol);
}

// 设置串口读取、写入超时
BOOL CUart::SetUartTimeouts(COMMTIMEOUTS CommTimeOuts)
{
	ASSERT(m_hComm != INVALID_HANDLE_VALUE);
	return SetCommTimeouts(m_hComm, &CommTimeOuts);
}

// 得到串口是否打开
BOOL CUart::GetComOpened(void)
{
	return m_bOpened;
}
