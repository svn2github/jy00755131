#pragma once
///////////////////////////////////////////////////////////////////////////
/**
 * @class CSocketFDU
 * @brief 与采集站通信接口
 *
 * 用于与采集站之间的数据通信接口，下发命令，接收回令。该接口实际是与主交叉站的网络接口通信，
 由主交叉站进行数据转发。
 */
class CSocketFDU
{
public:
	CSocketFDU(void);
	~CSocketFDU(void);	
	
	/** 放炮点名socket，可以下发命令 */
	SOCKET		  m_FDUSocket;
	/** 放炮点名socket端口：与采集线程组的编号有关 */
	WORD		  m_wLocalPort;
private:
	/** 本服务器的通信IP地址 */
	unsigned long m_dwLocalIP;
	

	/** 主交叉站以太网网络端口IP地址 */
	//DWORD		  m_dwMainCrossIP;
	/** 主交叉站以太网网络端口 */
	//WORD		  m_wMainCrossPort;
	/** 下行数据缓冲区 */
	unsigned char   m_pBuf[128];
private:
	int  SendToFDU(unsigned char* pBuf,int nSize = 128);

public:

	bool CreateSocket(DWORD dwIP,WORD wPort);
	bool CloseSocket(void);	


//	bool SendCallCmdToFDU(SHOTSPREAD& stShotSpread);
//	bool SendCallCmdToSpread(void);
//  bool SendSampleCmdToSpread(void);

	// 清空socket缓冲区
	bool ClearRevBufOfSocket();
	//int RecvData(char* pBuf,int nSize);
	// 从上行数据包中获得FDU的IP地址
	DWORD  GetFDUIPFromUplinkPack(unsigned char* pBuf,int nSize);
	int SendRetranCmdToFDU(DWORD dwFDUIP, WORD wHostPort, WORD wFrmIndex);
	//int SendCallCmdToFDU(DWORD dwFDUIP,WORD wMinPort,WORD wMaxPort);
	int SendBroadcastPortCmdToFDU(DWORD dwFDUIP,WORD wBroadcastPort);
	int SendConfigCmdToFDU(DWORD dwFDUIP,WORD wBroadcastPort,DWORD dwConfig,DWORD dwFilter17,DWORD dwFilter18);
	// 发送查询系统时间
	int SendQueryTimeCmd(DWORD dwFDUIP);
	// 接收系统时间
	// DWORD RevSystemTime(void);
	int SendTEBroadcastCmdToFDU(DWORD dwTE_H, DWORD dwTE_L);
	//向采集站广播采样参数
	int SendSampParamCmdToFDU(DWORD dwIP,WORD wMinPort,WORD wMaxPort,DWORD dwTBH,DWORD dwTBL,DWORD dwTEH,DWORD dwTEL);
};
