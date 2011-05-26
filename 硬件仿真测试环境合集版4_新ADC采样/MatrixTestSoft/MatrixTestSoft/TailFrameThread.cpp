// TailFrameThread.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixTestSoft.h"
#include "TailFrameThread.h"
#include "Parameter.h"

// CTailFrameThread

IMPLEMENT_DYNCREATE(CTailFrameThread, CWinThread)

CTailFrameThread::CTailFrameThread()
: m_bclose(false)
, m_uiSN(0)
, m_uiNetTime(0)
, m_uiSystemTime(0)
, m_usTailRecTime(0)
, m_usTailSendTime(0)
, m_uiSendTailTimeFrameCount(0)
, m_uiRecTailTimeFrameCount(0)
, m_iTimeHigh(0)
, m_iTimeLow(0)
, m_uiDelayTimeCount(0)
, m_csIPSource(_T(""))
, m_uiSendPort(0)
{
}

CTailFrameThread::~CTailFrameThread()
{
}

BOOL CTailFrameThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CTailFrameThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CTailFrameThread, CWinThread)
END_MESSAGE_MAP()


// 初始化

void CTailFrameThread::OnInit(void)
{
	m_hTailFrameThreadClose = ::CreateEvent(false, false, NULL, NULL);	// 创建事件对象
	::ResetEvent(m_hTailFrameThreadClose);	// 设置事件对象为无信号状态
}
// 线程函数在Run中执行
int CTailFrameThread::Run()
{
	DWORD dwTailFrameCount = 0;
	int iTailcount = 0;
	DWORD dwTailTimeFrameCount = 0;
	int iTailTimecount = 0;
	// 循环，直到关闭标志为真
	while(true)
	{
		if(m_bclose == true)
		{
			break;
		}
		// 得到网络接收缓冲区数据字节数
		m_TailFrameSocket.IOCtl(FIONREAD, &dwTailFrameCount);
		dwTailFrameCount = dwTailFrameCount/256;
		if(0 != dwTailFrameCount)	
		{
			for(unsigned int i = 0; i < dwTailFrameCount; i++)	
			{
				iTailcount = m_TailFrameSocket.Receive(m_pTailFrameData,256);
				if (iTailcount == 256)
				{
					// 单个尾包处理
					ProcTailFrameOne();
				}
				else
				{
					break;
				}
			}
		}
		// 得到网络接收缓冲区数据字节数
		m_TailTimeFrameSocket.IOCtl(FIONREAD, &dwTailTimeFrameCount);
		dwTailTimeFrameCount = dwTailTimeFrameCount/256;
		if(0 != dwTailTimeFrameCount)	
		{
			for(unsigned int i = 0; i < dwTailTimeFrameCount; i++)
			{
				iTailTimecount = m_TailTimeFrameSocket.Receive(m_pTailTimeRecData,256);
				if (iTailTimecount == 256)
				{
					// 单个尾包时刻查询处理
					ProcTailTimeFrameOne();
				}
				else
				{
					break;
				}
			}
		}
		if (m_bclose == true)
		{
			break;
		}
		Sleep(250);
	}
	::SetEvent(m_hTailFrameThreadClose);	// 设置事件对象为有信号状态
	// 返回
	return CWinThread::Run();
}
// 关闭并结束线程
void CTailFrameThread::OnClose(void)
{
//	m_TailFrameSocket.ShutDown(2);
//	m_TailTimeFrameSocket.ShutDown(2);
	m_TailFrameSocket.Close();
	m_TailTimeFrameSocket.Close();
	m_bclose = true;
}

// 处理单个尾包
void CTailFrameThread::ProcTailFrameOne(void)
{
	if (ParseTailFrame() == FALSE)
	{
		return;
	}
	CInstrument* pInstrument = NULL;
	// 在索引表中则找到该仪器,得到该仪器指针
	if (TRUE == m_oInstrumentList->GetInstrumentFromMap(m_uiSN, pInstrument))
	{
		// 如果是交叉站尾包则不处理，如果是采集站尾包
		if (pInstrument->m_uiInstrumentType == 3)
		{
			// 尾包计数器加一
			pInstrument->m_iTailFrameCount++;
			// 如果尾包时刻查询在下一次尾包来临之际还未收全，则放弃上次查询结果并对未收到尾包时刻查询的仪器设为过期标志
			if (m_uiSendTailTimeFrameCount != m_uiRecTailTimeFrameCount)
			{
				m_uiRecTailTimeFrameCount = 0;
				m_uiSendTailTimeFrameCount = 0;
				m_oInstrumentList->ClearExperiedTailTimeResult();
			}
		}
	}
	else
	{
		return;
	}
	// 判断尾包计数器达到设定值
	if (pInstrument->m_iTailFrameCount == TailFrameCount)
	{
		pInstrument->m_iTailFrameCount = 0;
		// 判断采集站位置后面是否还有仪器，有则删除
		m_oInstrumentList->TailFrameDeleteInstrument(pInstrument);

		// 发送尾包时刻查询帧
		SendTailTimeFrame();
	}
}

// 解析尾包
BOOL CTailFrameThread::ParseTailFrame(void)
{
	byte	usCommand = 0;

	// 仪器SN号
	memcpy(&usCommand, &m_pTailFrameData[33], 1);
	if (usCommand != 0x01)
	{
		return FALSE;
	}
	memcpy(&m_uiSN, &m_pTailFrameData[34], 4);
	
	return TRUE;
}

// 向仪器发送尾包时刻查询帧
void CTailFrameThread::SendTailTimeFrame(void)
{
	CInstrument* pInstrument = NULL;	// 仪器	
	POSITION pos = NULL;	// 位置	
	unsigned int uiKey;	// 索引键

	// 得到索引表起始仪器位置
	pos = m_oInstrumentList->m_oInstrumentMap.GetStartPosition();
	// 当前位置有仪器
	while(NULL != pos)
	{
		pInstrument = NULL;
		// 得到仪器
		m_oInstrumentList->m_oInstrumentMap.GetNextAssoc(pos, uiKey, pInstrument);
		if(NULL != pInstrument)	// 得到仪器
		{
			if (pInstrument->m_bIPSetOK == true)
			{
				// 生成仪器尾包时刻查询帧
				MakeTailTimeFrameData(pInstrument);
				// 发送仪器尾包时刻查询帧
				SendTailTimeFrameToSocket();
				m_uiSendTailTimeFrameCount++;
				pInstrument->m_bSendTailTimeFrame = true;
			}
		}
	}
}

// 生成尾包时刻查询帧
void CTailFrameThread::MakeTailTimeFrameData(CInstrument* pInstrument)
{
	unsigned int uiIPSource =	0;
	unsigned int uiIPAim	=	0;
	unsigned int usPortAim	=	0;
	unsigned int usCommand	=	0;
	m_pTailTimeSendData[0] = 0x11;
	m_pTailTimeSendData[1] = 0x22;
	m_pTailTimeSendData[2] = 0x33;
	m_pTailTimeSendData[3] = 0x44;
	m_pTailTimeSendData[4] = 0x00;
	m_pTailTimeSendData[5] = 0x00;
	m_pTailTimeSendData[6] = 0x00;
	m_pTailTimeSendData[7] = 0x00;
	m_pTailTimeSendData[8] = 0x00;
	m_pTailTimeSendData[9] = 0x00;
	m_pTailTimeSendData[10] = 0x00;
	m_pTailTimeSendData[11] = 0x00;
	m_pTailTimeSendData[12] = 0x00;
	m_pTailTimeSendData[13] = 0x00;
	m_pTailTimeSendData[14] = 0x00;
	m_pTailTimeSendData[15] = 0x00;

	uiIPSource	=	inet_addr(m_csIPSource);
	uiIPAim		=	pInstrument->m_uiIPAddress;
	usPortAim	=	TailTimeFramePort;
	usCommand	=	2;
	// 源IP地址
	memcpy(&m_pTailTimeSendData[16], &uiIPSource, 4);
	// 目标IP地址
	memcpy(&m_pTailTimeSendData[20], &uiIPAim, 4);
	TRACE1("尾包时刻查询帧-仪器IP地址：%d\r\n", uiIPAim);
	// 目标端口号
	memcpy(&m_pTailTimeSendData[24], &usPortAim, 2);
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&m_pTailTimeSendData[26], &usCommand, 2);

	int iPos = 32;
	// 命令字0x01读取设备SN
	m_pTailTimeSendData[iPos] = 0x01;
	iPos++;
	memset(&m_pTailTimeSendData[iPos], 0, 4);
	iPos = iPos + 4;

	// 命令字0x19读取网络时刻
	m_pTailTimeSendData[iPos] = 0x19;
	iPos++;
	memset(&m_pTailTimeSendData[iPos], 0, 4);
	iPos = iPos + 4;

	// 命令字0x04读取本地系统时间
	m_pTailTimeSendData[iPos] = 0x04;
	iPos++;
	memset(&m_pTailTimeSendData[iPos], 0, 4);
	iPos = iPos + 4;

	// 仪器类型	1-交叉站
	if(1 == pInstrument->m_uiInstrumentType)
	{
		// 命令字0x1B  交叉站大线尾包接收时刻
		m_pTailTimeSendData[iPos] = 0x1B;
		iPos++;
		memset(&m_pTailTimeSendData[iPos], 0, 4);
		iPos = iPos + 4;
	}
	else
	{
		// 命令字0x16接收、发送时刻低位
		m_pTailTimeSendData[iPos] = 0x16;
		iPos++;
		memset(&m_pTailTimeSendData[iPos], 0, 4);
		iPos = iPos + 4;
	}

	// 设置命令字结束
	m_pTailTimeSendData[iPos] = 0x00;

}

// 发送尾包时刻查询帧
void CTailFrameThread::SendTailTimeFrameToSocket(void)
{
	// 发送帧
	int iCount = m_TailTimeFrameSocket.SendTo(m_pTailTimeSendData, 128, m_uiSendPort, IPBroadcastAddr);
}

// 单个尾包时刻查询处理
void CTailFrameThread::ProcTailTimeFrameOne(void)
{
	if (PraseTailTimeFrame() == FALSE)
	{
		return;
	}
	if (m_uiRecTailTimeFrameCount == m_uiSendTailTimeFrameCount)
	{
		m_uiSendTailTimeFrameCount = 0;
		m_uiRecTailTimeFrameCount = 0;
		// 完成时统
		TimeDelayCalculation();
	}
}

// 解析尾包时刻查询帧
BOOL CTailFrameThread::PraseTailTimeFrame(void)
{
	byte	usCommand = 0;
	int iPos = 28;
	// 仪器SN号
	memcpy(&usCommand, &m_pTailTimeRecData[iPos], 1);
	iPos++;
	if (usCommand != 0x01)
	{
		return FALSE;
	}
	memcpy(&m_uiSN, &m_pTailTimeRecData[iPos], 4);
	iPos = iPos + 4;

	// 新仪器指针为空
	CInstrument* pInstrument = NULL;
	// 在索引表中则找到该仪器,得到该仪器指针
	if (TRUE == m_oInstrumentList->GetInstrumentFromMap(m_uiSN, pInstrument))
	{	
		// 没有过期尾包，只有丢失帧
		// 如果是过期的尾包时刻查询帧
// 		if (pInstrument->m_bTailTimeExpired == true)
// 		{
// 			pInstrument->m_bTailTimeExpired = false;
// 			return FALSE;
//		}
		// 仪器网络时刻
		memcpy(&usCommand, &m_pTailTimeRecData[iPos], 1);
		iPos++;
		if (usCommand != 0x19)
		{
			return FALSE;
		}
		memcpy(&m_uiNetTime, &m_pTailTimeRecData[iPos], 4);
		iPos = iPos + 4;

		// 仪器本地系统时间
		memcpy(&usCommand, &m_pTailTimeRecData[iPos], 1);
		iPos++;
		if (usCommand != 0x04)
		{
			return FALSE;
		}
		memcpy(&m_uiSystemTime, &m_pTailTimeRecData[iPos], 4);
		iPos = iPos + 4;

		// 仪器类型	1-交叉站
		if(1 == pInstrument->m_uiInstrumentType)
		{
			// 命令字0x1B  交叉站大线尾包接收时刻
			memcpy(&usCommand, &m_pTailTimeRecData[iPos], 1);
			iPos++;
			if (usCommand != 0x1B)
			{
				return FALSE;
			}
			memcpy(&m_usTailRecTime, &m_pTailTimeRecData[iPos], 2);
			iPos = iPos + 2;
			memcpy(&m_usTailSendTime, &m_pTailTimeRecData[iPos], 2);
			iPos = iPos + 2;
		}
		else
		{
			// 命令字0x16接收、发送时刻低位
			memcpy(&usCommand, &m_pTailTimeRecData[iPos], 1);
			iPos++;
			if (usCommand != 0x16)
			{
				return FALSE;
			}
			memcpy(&m_usTailRecTime, &m_pTailTimeRecData[iPos], 2);
			iPos = iPos + 2;
			memcpy(&m_usTailSendTime, &m_pTailTimeRecData[iPos], 2);
			iPos = iPos + 2;
		}
		pInstrument->m_uiNetTime = m_uiNetTime;
		pInstrument->m_uiSystemTime = m_uiSystemTime;
		// 低14位为帧内时间
		pInstrument->m_usTailRecTime = m_usTailRecTime & 0x3fff;
		pInstrument->m_usTailSendTime = m_usTailSendTime & 0x3fff;
		pInstrument->m_bTailTimeReturnOK = true;
		m_uiRecTailTimeFrameCount++;
	}
	else
	{
		return FALSE;
	}
	
	return TRUE;
}

// 时间同步计算
void CTailFrameThread::TimeDelayCalculation(void)
{
	CInstrument* pInstrument = NULL;	// 仪器	
	POSITION pos = NULL;	// 位置	
	unsigned int uiKey;	// 索引键
	unsigned int uiInstrumentCount = 0;
	unsigned short usSendTailTime = 0;
	unsigned short usRecTailTime = 0;
	unsigned int uiNetTime = 0;
	// 得到索引表起始仪器位置
	pos = m_oInstrumentList->m_oInstrumentMap.GetStartPosition();
	// 当前位置有仪器
	while(NULL != pos)
	{
		pInstrument = NULL;
		// 得到仪器
		m_oInstrumentList->m_oInstrumentMap.GetNextAssoc(pos, uiKey, pInstrument);
		if(NULL != pInstrument)	// 得到仪器
		{
			if (pInstrument->m_bTailTimeReturnOK == true)
			{
				m_oInstrumentLocationMap.SetAt(pInstrument->m_uiLocation, pInstrument);
			}
			pInstrument->m_bSendTailTimeFrame = false;
			pInstrument->m_bTailTimeReturnOK = false;
		}
	}
	uiInstrumentCount = m_oInstrumentLocationMap.GetCount();
	// 只有交叉站则不需做时延设置
	if (uiInstrumentCount == 1)
	{
		m_oInstrumentLocationMap.RemoveAll();
		return;
	}
	// 记录交叉站接收大线方向尾包的接收时刻
	m_oInstrumentLocationMap.Lookup(0, pInstrument);
	usRecTailTime = pInstrument->m_usTailRecTime;
	uiNetTime = pInstrument->m_uiNetTime;
	m_iTimeLow = 0;
	m_iTimeHigh = 0;
	for (unsigned int i=1; i<uiInstrumentCount;i++)
	{
		if (m_oInstrumentLocationMap.Lookup(i, pInstrument) == FALSE)
		{
			break;
		}
		// 高位做修正也只能修正一次，毕竟是查询帧，误差较大，如果连续修正高位反而不准
		if (m_uiDelayTimeCount == 0)
		{
			m_iTimeHigh = m_iTimeHigh + uiNetTime - pInstrument->m_uiNetTime;
		}
		else
		{
			m_iTimeHigh = 0;
		}
		m_iTimeHigh = 0;
//		m_iTimeLow = m_iTimeLow - (usRecTailTime - pInstrument->m_usTailSendTime)/2 - 10;
		int temp = usRecTailTime - pInstrument->m_usTailSendTime;
		if (temp<0)
		{
			temp += 0x4000;
		}
		// 张经理算法
		m_iTimeLow = m_iTimeLow -(temp- 4112);
		// 赵总算法
//		m_iTimeLow = m_iTimeLow - temp/2 - 10;
		usRecTailTime = pInstrument->m_usTailRecTime;
		// 生成时统报文设置帧
		MakeTimeDelayFrameData(pInstrument);
		// 发送时统报文设置帧
		SendTimeDelayFrameData();
	}
	m_oInstrumentLocationMap.RemoveAll();
	m_uiDelayTimeCount++; 
}

// 生成时统报文设置帧
void CTailFrameThread::MakeTimeDelayFrameData(CInstrument* pInstrument)
{
	unsigned int uiIPSource =	0;
	unsigned int uiIPAim	=	0;
	unsigned int usPortAim	=	0;
	unsigned int usCommand	=	0;
	m_pTimeDelayData[0] = 0x11;
	m_pTimeDelayData[1] = 0x22;
	m_pTimeDelayData[2] = 0x33;
	m_pTimeDelayData[3] = 0x44;
	m_pTimeDelayData[4] = 0x00;
	m_pTimeDelayData[5] = 0x00;
	m_pTimeDelayData[6] = 0x00;
	m_pTimeDelayData[7] = 0x00;
	m_pTimeDelayData[8] = 0x00;
	m_pTimeDelayData[9] = 0x00;
	m_pTimeDelayData[10] = 0x00;
	m_pTimeDelayData[11] = 0x00;
	m_pTimeDelayData[12] = 0x00;
	m_pTimeDelayData[13] = 0x00;
	m_pTimeDelayData[14] = 0x00;
	m_pTimeDelayData[15] = 0x00;

	uiIPSource	=	inet_addr(m_csIPSource);
	uiIPAim		=	pInstrument->m_uiIPAddress;
	usPortAim	=	TimeSetPort;
	usCommand	=	1;
	// 源IP地址
	memcpy(&m_pTimeDelayData[16], &uiIPSource, 4);
	// 目标IP地址
	memcpy(&m_pTimeDelayData[20], &uiIPAim, 4);
	TRACE1("尾包时刻查询帧-仪器IP地址：%d\r\n", uiIPAim);
	// 目标端口号
	memcpy(&m_pTimeDelayData[24], &usPortAim, 2);
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&m_pTimeDelayData[26], &usCommand, 2);

	int iPos = 32;
	// 命令字0x05修正时延高位
	m_pTimeDelayData[iPos] = 0x05;
	iPos++;
	memcpy(&m_pTimeDelayData[iPos], &m_iTimeHigh, 4);
	iPos = iPos + 4;

	// 命令字0x06修正时延低位
	m_pTimeDelayData[iPos] = 0x06;
	iPos++;
	memcpy(&m_pTimeDelayData[iPos], &m_iTimeLow, 4);
	iPos = iPos + 4;

	// 设置命令字结束
	m_pTimeDelayData[iPos] = 0x00;
}

// 发送时统报文设置帧
void CTailFrameThread::SendTimeDelayFrameData(void)
{
	int iCount = m_TailTimeFrameSocket.SendTo(m_pTimeDelayData, 128, m_uiSendPort, IPBroadcastAddr);
}

// 打开
void CTailFrameThread::OnOpen(void)
{
	m_TailFrameSocket.Close();
	m_TailTimeFrameSocket.Close();
	BOOL flag = FALSE; 
	flag =  m_TailFrameSocket.Create(TailFramePort,SOCK_DGRAM);
	if (flag == FALSE)
	{
		AfxMessageBox(_T("接收端口创建失败！"));
	}

	BOOL bReturn = FALSE;
	// 生成网络端口，接收发送命令应答帧，create函数写入第三个参数IP地址则接收固定IP地址发送的帧，不写则全网接收
	bReturn = m_TailTimeFrameSocket.Create(TailTimeFramePort, SOCK_DGRAM);
	if (bReturn == FALSE)
	{
		AfxMessageBox(_T("命令广播端口创建失败！"));
	}

	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	m_TailTimeFrameSocket.SetSockOpt(SO_BROADCAST, &iOptval, iOptlen, SOL_SOCKET);
}

// 停止
void CTailFrameThread::OnStop(void)
{
//	m_TailFrameSocket.ShutDown(2);
//	m_TailTimeFrameSocket.ShutDown(2);
	m_TailFrameSocket.Close();
	m_TailTimeFrameSocket.Close();
}
