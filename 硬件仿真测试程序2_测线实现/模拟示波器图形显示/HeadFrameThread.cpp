// HeadFrameThread.cpp : 实现文件
//

#include "stdafx.h"
#include "模拟示波器图形显示.h"
#include "HeadFrameThread.h"
#include "Parameter.h"

// CHeadFrameThread

IMPLEMENT_DYNCREATE(CHeadFrameThread, CWinThread)

CHeadFrameThread::CHeadFrameThread()
: m_bclose(false)
, m_uiSN(0)
, m_uiHeadFrameTime(0)
, m_uiRoutAddress(0)
, m_uiIPAddress(0)
{
}

CHeadFrameThread::~CHeadFrameThread()
{
}

BOOL CHeadFrameThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CHeadFrameThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CHeadFrameThread, CWinThread)
END_MESSAGE_MAP()


// 初始化

void CHeadFrameThread::OnInit(void)
{
	BOOL flag = FALSE; 
	flag =  m_HeadFrameSocket.Create(HeadFramePort,SOCK_DGRAM);
	if (flag == FALSE)
	{
		AfxMessageBox(_T("接收端口创建失败！"));
	}
	BOOL bReturn = FALSE;
	// 生成网络端口，接收发送命令应答帧，create函数写入第三个参数IP地址则接收固定IP地址发送的帧，不写则全网接收
	bReturn = m_IPSetSocket.Create(IPSetPort, SOCK_DGRAM);
	if (bReturn == FALSE)
	{
		AfxMessageBox(_T("命令广播端口创建失败！"));
	}

	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	m_IPSetSocket.SetSockOpt(SO_BROADCAST, &iOptval, iOptlen, SOL_SOCKET);
}
// 线程函数在Run中执行
int CHeadFrameThread::Run()
{
	DWORD dwHeadFrameCount = 0;
	int iHeadcount = 0;
	DWORD dwIPSetReturnFrameCount = 0;
	int iIPSetReturncount = 0;
	// 循环，直到关闭标志为真
	while(true)
	{
		if(m_bclose == true)
		{
			break;
		}
		// 得到网络接收缓冲区数据字节数
		m_HeadFrameSocket.IOCtl(FIONREAD, &dwHeadFrameCount);
		dwHeadFrameCount = dwHeadFrameCount/256;
		if(0 != dwHeadFrameCount)	
		{
			for(unsigned int i = 0; i < dwHeadFrameCount; i++)	// 依次检查每个仪器IP设置应答帧
			{
				iHeadcount = m_HeadFrameSocket.Receive(m_pHeadFrameData,256);
				if (iHeadcount == 256)
				{
					// 单个首包处理
					ProcHeadFrameOne();
				}
				else
				{
					break;
				}
			}
		}

		// 得到网络接收缓冲区数据字节数
		m_IPSetSocket.IOCtl(FIONREAD, &dwIPSetReturnFrameCount);
		dwIPSetReturnFrameCount = dwIPSetReturnFrameCount/256;
		if(0 != dwIPSetReturnFrameCount)	
		{
			for(unsigned int i = 0; i < dwIPSetReturnFrameCount; i++)	// 依次检查每个仪器IP设置应答帧
			{
				iIPSetReturncount = m_IPSetSocket.Receive(m_pIPSetReturnFrameData,256);
				if (iIPSetReturncount == 256)
				{
					// 单个IP地址设置应答帧处理
					ProcIPSetReturnFrameOne();
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
		Sleep(50);
	}

	// 返回
	return CWinThread::Run();
}


// 关闭并结束线程
void CHeadFrameThread::OnClose(void)
{
// 	m_HeadFrameSocket.ShutDown(2);
 	m_HeadFrameSocket.Close();
	m_IPSetSocket.Close();
	m_bclose = true;
}

// 处理单个首包
void CHeadFrameThread::ProcHeadFrameOne()
{
	if (ParseHeadFrame() == FALSE)
	{
		return;
	}
	// 新仪器指针为空
	CInstrument* pInstrument = NULL;
	// 如果在索引表中没找到该仪器
	if (TRUE != m_oInstrumentList->IfIndexExistInMap(m_uiSN))
	{
		// 得到新仪器
		pInstrument = m_oInstrumentList->GetFreeInstrument();

		// 设置新仪器的SN
		pInstrument->m_uiSN = m_uiSN;
		// 设置新仪器的路由IP地址
		pInstrument->m_uiRoutAddress = m_uiRoutAddress;

		if (m_uiRoutAddress == 0)
		{
			// 设置新仪器的仪器类型
			pInstrument->m_uiInstrumentType = 1;
		}
		else
		{
			pInstrument->m_uiInstrumentType = 3;
		}
		// 设置新仪器的首包时刻
		pInstrument->m_uiHeadFrameTime = m_uiHeadFrameTime;
		// 首包计数器加一
		pInstrument->m_iHeadFrameCount++;

		// 新仪器加入SN索引表
		m_oInstrumentList->AddInstrumentToMap(pInstrument->m_uiSN, pInstrument);
	}
	else
	{
		// 在索引表中则找到该仪器,得到该仪器指针
		if (TRUE == m_oInstrumentList->GetInstrumentFromMap(m_uiSN, pInstrument))
		{
			// 首包计数器加一
			pInstrument->m_iHeadFrameCount++;
		}
	}
	
	// 设置仪器IP地址，如果是交叉站还需设置路由地址
	if (pInstrument->m_iHeadFrameCount == HeadFrameCount)
	{
		pInstrument->m_iHeadFrameCount = 0;
		MakeIPSetFrame(pInstrument);
		SendIPSetFrame();
	}
}

// 解析首包
BOOL CHeadFrameThread::ParseHeadFrame(void)
{
	byte	usCommand = 0;

	// 仪器SN号
	memcpy(&usCommand, &m_pHeadFrameData[28], 1);
	if (usCommand != 0x01)
	{
		return FALSE;
	}
	memcpy(&m_uiSN, &m_pHeadFrameData[29], 4);
	// 仪器首包时刻
	memcpy(&usCommand, &m_pHeadFrameData[33], 1);
	if (usCommand != 0x02)
	{
		return FALSE;
	}
	memcpy(&m_uiHeadFrameTime, &m_pHeadFrameData[34], 4);
	// 路由IP地址
	memcpy(&usCommand, &m_pHeadFrameData[38], 1);
	if (usCommand == 0x00)
	{
		m_uiRoutAddress = 0;
	}
	else if (usCommand == 0x3F)
	{
		memcpy(&m_uiRoutAddress, &m_pHeadFrameData[39], 4);
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

// 生成IP地址设置帧
void CHeadFrameThread::MakeIPSetFrame(CInstrument* pInstrument)
{
	unsigned int	uiIPSource;
	unsigned int	uiIPAim;
	unsigned short	usPortAim;
	unsigned short	usCommand;
	unsigned int	uiRoutIPTop;
	unsigned int	uiRoutIPDown;
	unsigned int	uiRoutIPLeft;
	unsigned int	uiRoutIPRight;
	m_pIPSetFrameData[0] = 0x11;
	m_pIPSetFrameData[1] = 0x22;
	m_pIPSetFrameData[2] = 0x33;
	m_pIPSetFrameData[3] = 0x44;
	m_pIPSetFrameData[4] = 0x00;
	m_pIPSetFrameData[5] = 0x00;
	m_pIPSetFrameData[6] = 0x00;
	m_pIPSetFrameData[7] = 0x00;
	m_pIPSetFrameData[8] = 0x00;
	m_pIPSetFrameData[9] = 0x00;
	m_pIPSetFrameData[10] = 0x00;
	m_pIPSetFrameData[11] = 0x00;
	m_pIPSetFrameData[12] = 0x00;
	m_pIPSetFrameData[13] = 0x00;
	m_pIPSetFrameData[14] = 0x00;
	m_pIPSetFrameData[15] = 0x00;

	// 源IP地址
	uiIPSource	=	inet_addr(IPSource);
	// 目标IP地址
	uiIPAim		=	0xFFFFFFFF;
	// 目标端口号
	usPortAim	=	IPSetPort;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	usCommand	=	1;

	uiRoutIPTop		=	5;
	uiRoutIPDown	=	6;
	uiRoutIPLeft	=	7;
	uiRoutIPRight	=	8;
	// 源IP地址
	memcpy(&m_pIPSetFrameData[16], &uiIPSource, 4);
	// 目标IP地址
	memcpy(&m_pIPSetFrameData[20], &uiIPAim, 4);
	// 目标端口号
	memcpy(&m_pIPSetFrameData[24], &usPortAim, 2);
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&m_pIPSetFrameData[26], &usCommand, 2);

	int iPos = 32;
	// 仪器串号设置命令字
	m_pIPSetFrameData[iPos] = 0x01;
	iPos++;
	// 仪器SN号
	memcpy(&m_pIPSetFrameData[iPos], &m_uiSN, 4);
	iPos = iPos + 4;

	// 仪器本地站点IP地址设置命令字
	m_pIPSetFrameData[iPos] = 0x03;
	iPos++;
	// 仪器本地IP
	memcpy(&m_pIPSetFrameData[iPos], &pInstrument->m_uiIPAddress, 4);
	iPos = iPos + 4;
	if(1 != pInstrument->m_uiInstrumentType)	// 仪器类型 1-交叉站；2-电源站；3-采集站
	{
		// 设置命令字结束
		m_pIPSetFrameData[iPos] = 0x00;
	}
	else	// 需要设置路由IP地址
	{
		// 仪器本地站点IP地址设置命令字
		m_pIPSetFrameData[iPos] = 0x1F;
		iPos++;
		// 路由IP地址，路由方向 1-上
		memcpy(&m_pIPSetFrameData[iPos], &uiRoutIPTop, 4);
		iPos = iPos + 4;
		// 仪器本地站点IP地址设置命令字
		m_pIPSetFrameData[iPos] = 0x1F;
		iPos++;
		// 路由IP地址，路由方向 2-下
		memcpy(&m_pIPSetFrameData[iPos], &uiRoutIPDown, 4);
		iPos = iPos + 4;
		// 仪器本地站点IP地址设置命令字
		m_pIPSetFrameData[iPos] = 0x1F;
		iPos++;
		// 路由IP地址，路由方向 3-左
		memcpy(&m_pIPSetFrameData[iPos], &uiRoutIPLeft, 4);
		iPos = iPos + 4;
		// 仪器本地站点IP地址设置命令字
		m_pIPSetFrameData[iPos] = 0x1F;
		iPos++;
		// 路由IP地址，路由方向 4-右
		memcpy(&m_pIPSetFrameData[iPos], &uiRoutIPRight, 4);
		iPos = iPos + 4;
		// 设置命令字结束
		m_pIPSetFrameData[iPos] = 0x00;
	}
}

// 发送IP地址设置帧
void CHeadFrameThread::SendIPSetFrame(void)
{
	// 发送帧
	// 需要创建广播端口
	int iCount = m_IPSetSocket.SendTo(m_pIPSetFrameData, 128, SendPort, IPBroadcastAddr);
}

// 单个IP地址设置应答帧处理
void CHeadFrameThread::ProcIPSetReturnFrameOne(void)
{
	if (ParseIPSetReturnFrame() == FALSE)
	{
		return;
	}
	// 在索引表中找到该仪器
	if (TRUE == m_oInstrumentList->IfIndexExistInMap(m_uiSN))
	{
		// 在索引表中找到该仪器,得到该仪器指针
		CInstrument* pInstrument = NULL;
		if (TRUE == m_oInstrumentList->GetInstrumentFromMap(m_uiSN, pInstrument))
		{
			if (m_uiIPAddress == pInstrument->m_uiIPAddress)
			{
				pInstrument->m_bIPSetOK = true;
			}
		}
	}

}

// 解析IP地址设置应答帧
BOOL CHeadFrameThread::ParseIPSetReturnFrame(void)
{
	byte	usCommand = 0;

	// 仪器SN号
	memcpy(&usCommand, &m_pIPSetReturnFrameData[28], 1);
	if (usCommand != 0x01)
	{
		return FALSE;
	}
	memcpy(&m_uiSN, &m_pIPSetReturnFrameData[29], 4);
	// 仪器被设置的IP地址
	memcpy(&usCommand, &m_pIPSetReturnFrameData[33], 1);
	if (usCommand != 0x03)
	{
		return FALSE;
	}
	memcpy(&m_uiIPAddress, &m_pIPSetReturnFrameData[34], 4);
	return TRUE;
}
