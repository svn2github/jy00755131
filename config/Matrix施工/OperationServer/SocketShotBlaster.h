#pragma once

// CSocketShotBlaster 命令目标
class COperationServerDlg;

///////////////////////////////////////////////////////////////////////////
/**
 * @class CSocketShotBlaster
 * @brief 与爆炸机控制器通信的Socket类
 *
 * 与爆炸机控制器相连的采集站通信接口，负责接收来自所有爆炸机控制器的数据，
 并向爆炸机控制器发送命令。
     与爆炸机通信的协议依据爆炸机不同而不同，爆炸机协议处理的基类为CShotController。
 */
class CSocketShotBlaster : public CSocket
{
	
public:// 属性
	// 缓冲区，缓存来自采集站的爆炸机数据
	unsigned char   m_pBuf[256];
	// 本机IP地址
	DWORD           m_dwHostIP;
	// 本机接收端口
	WORD			m_wBlasterPort;

	COperationServerDlg* m_pDlg;

public:// 方法
	CSocketShotBlaster();
	virtual ~CSocketShotBlaster();
	virtual void OnReceive(int nErrorCode);

	// 创建socket
	bool CreateSocket(WORD wPort,LPCTSTR strIP,COperationServerDlg* pDlg);
	int  SendToFDU(unsigned char* pBuf,int nSize);
};


