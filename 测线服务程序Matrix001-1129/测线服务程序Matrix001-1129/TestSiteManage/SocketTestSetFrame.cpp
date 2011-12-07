// SocketTestSetFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "TestSiteManage.h"
#include "SocketTestSetFrame.h"
#include "Matrixline.h"
#include "Parameter.h"
#include "TestElementData.h"

// CSocketTestSet

CSocketTestSetFrame::CSocketTestSetFrame()

{
	// 帧字节数
	m_iFrameSize = FrameLength;
	// ADC命令设置序号
	m_uiADCSetOperationNb = 0;
	// 采集站本地时间
	m_uiTnow = 0;
	m_pTestElementList = NULL;
	m_uiIPCount = 0;
	m_uiUdpCount = 0;
}

CSocketTestSetFrame::~CSocketTestSetFrame()
{
}

/**
* 初始化
* @param CString strIPForInstrument 信源IP地址
* @param UINT uiIPForInstrument 信源IP地址
* @param UINT uiPortForTestSet	信源端口号
* @param CString strIPLCI 信宿IP地址
* @param UINT uiPortLCI	信宿端口号
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CSocketTestSetFrame::OnInit(CString strIPForInstrument, UINT uiIPForInstrument, UINT uiPortForTestSet, CString strIPLCI, UINT uiPortLCI)
{
	BOOL bReturn = false;

	// LCI设备IP地址
	m_strIPLCI = strIPLCI;
	//  LCI设备IP地址端口号
	m_uiPortLCI = uiPortLCI;

	// 和现场仪器通讯的本机IP地址
	m_strIPForInstrument = strIPForInstrument;
	// 和现场仪器通讯的本机IP地址
	m_uiIPForInstrument = uiIPForInstrument;
	// 用于测试设置的端口号
	m_uiPortForTestSet = uiPortForTestSet;
	// 生成网络端口
	bReturn = Create(m_uiPortForTestSet, SOCK_DGRAM, m_strIPForInstrument);
	Bind(m_uiPortForTestSet,m_strIPForInstrument);
	// 设置接收缓冲区大小
	int RevBuf = ADCSetReturnBufSize;
	if (this->SetSockOpt( SO_RCVBUF, (char*)&RevBuf, sizeof(RevBuf)) == SOCKET_ERROR) 
	{
		this->Close();		
		return false;
	}

	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	SetSockOpt(SO_BROADCAST, &iOptval, iOptlen, SOL_SOCKET);
	DWORD   dwBytesReturned   =   0; 
	BOOL     bNewBehavior   =   FALSE; 
	DWORD   status; 
	status   =   WSAIoctl(this->m_hSocket,SIO_UDP_CONNRESET,&bNewBehavior, sizeof   (bNewBehavior), 
                                                    NULL,   0,   &dwBytesReturned, NULL,   NULL); 

	return bReturn;
}

/**
* 解析AD寄存器查询应答帧
* @param void
* @return 
*/
void CSocketTestSetFrame::ParseFrameHead(int iCount,char *ReceiveBuf)
{
	int iIPSource,iIPAim,iPortAim=0,iCommand=0,isubCommand=0;
	// 源IP地址
	memcpy(&iIPSource, &ReceiveBuf[16], 4);
	// 目标IP地址
	memcpy(&iIPAim, &ReceiveBuf[20], 4);
	// 目标端口号
	memcpy(&iPortAim, &ReceiveBuf[24], 2);
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&iCommand, &ReceiveBuf[26], 2);

	memcpy(&isubCommand, &ReceiveBuf[28], 4);
	//TRACE3("Receive IP=%d %d %x\r\n",iIPSource, iCommand,isubCommand);
	// 数据索引
	if(iCount==FrameLength && iPortAim==m_uiPortForTestSet && iIPSource==m_oFrameTestSet.m_uiIPAim && iCommand==0x02)
	{
		memmove(ofccheck,ReceiveBuf+0x29,2);
		memmove(ofccheck+2,ReceiveBuf+0x2c,2);
		memmove(ofccheck+4,ReceiveBuf+0x2e,2);
	}

}

/**
* 解析命令字
* @param void
* @return bool true：成功；false：失败
*/
bool CSocketTestSetFrame::ParseCommandWord(byte  * pBuf)
{
	byte pData[5];	// 命令字数据
	byte byCommandWord;	// 命令字
	int iMaxCommandWordCount = 8;	// 最多解析多少个命令字
	int iCommandWordCount = 0;	// 命令字计数
	char charTemp[128];
	int iport=0;
	DWORD  dwIP=0;
	memcpy_s(&dwIP,4,pBuf+16,4);
	memcpy(&iport, pBuf+24, 2);

	while(true)
	{
		// 得到命令字数据
		memcpy(pData, &pBuf[28 + iCommandWordCount * 5], 5);
		// 命令字
		byCommandWord = pData[0];

		if(0x0 == byCommandWord)	//无命令字
		{
			break;
		}

		if(0x08 == byCommandWord)	// 命令字	
		{
			int TBH=0;
			memcpy(&TBH, &pData[1], 4);
			//gettestcount++;
			//TRACE2("TBH SET ：IP=%d TBH=%x\r\n", dwIP,TBH);
		}

		if(0x0f == byCommandWord)	// 命令字	
		{
			int TBH=0;
			memcpy(&TBH, &pData[1], 4);
			if(TBH<ADcheckrecv)
				checkrecv[TBH]++;
			gettestcount++;
			if(TBH==71)
				getTBcount=getTBcount | 0x01;
			if(TBH==72)
				getTBcount=getTBcount | 0x02;
			/*charTemp[0]=0;
			sprintf(charTemp,"rIP=%d getnum=%d %d \r\n",dwIP,TBH,gettestcount);
			strrecv += charTemp;
			
			if(gettestcount>=1300)
				TRACE3("rIP=%d getnum=%d %d\r\n", dwIP,TBH,adsetcount+gettestcount);
				*/
		}

		if(0x0c == byCommandWord)	// 命令字	
		{
			//getTBcount++;
			//gettestcount++;
			//TRACE2("TBH SET0d ：IP=%d TBH=%x\r\n", dwIP,TBH);
		}
		iCommandWordCount++;
		if(iCommandWordCount == iMaxCommandWordCount)	//所有命令字解析完
		{
			break;
		}
	}

	return true;
}


/**
* 设置缓冲区可以发送的帧数量
* @param void
* @return DWORD 帧数量
*/
BOOL CSocketTestSetFrame::SetBufferSize(int iFrameCount)
{
	BOOL bReturn = false;

	int iOptionValue = iFrameCount * m_iFrameSize;
	int iOptionLen = sizeof(int);
	bReturn = SetSockOpt(SO_SNDBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);

	return bReturn;
}

/**
* 生成测试设置帧
* @param void
* @return void
*/
void CSocketTestSetFrame::MakeFrameDataForTestSet()
{	
	m_oFrameTestSet.m_uiIPSource = m_uiIPForInstrument;	// 源IP地址	
	m_oFrameTestSet.m_usPortAim =(unsigned short) m_uiPortForTestSet;	// 目标端口号	
	m_oFrameTestSet.m_usCommand = 1;	// 命令号 1-设置命令；2-查询命令；3-AD采样数据重发
	// 生成帧
	m_oFrameTestSet.MakeFrameDataForTestSet();
}

/**
* 生成测试AD设置
* @param void
* @return void
*/
void CSocketTestSetFrame::MakeFrameADSet()
{	
	int iPos=0;
	byte cSetSine[4] = {0xb2, 0x00, 0x80, 0x00};
	byte cStopSample[4]={0x81, 0x11, 0x00, 0x00};
	byte cOpenPower[4]={0xa3, 0x00, 0x30, 0x00};
	byte cOpenPowerTB[4]={0xa3, 0x00, 0x30, 0x40};
	byte cOpenSwith[4]={0xa3, 0x00, 0xf0, 0x00};
	byte cOpenSwitchTB[4]={0xa3, 0x00, 0xf0, 0x40};
	byte cADCRegisterWrite[16]={0x8d, 0x40, 0x0a, 0x00, 0x52, 0x08, 0x32, 0x03, 0x6f, 0x0c, 0xff, 0x7d, 0x52, 0x40, 0x00, 0x00};
	byte cADCRegisterRead[4]={0x82, 0x20, 0x0a, 0x00};
	byte cADCReadContinuous[4]={0x81, 0x10, 0x00, 0x00};

	//广播
	m_oFrameTestSet.m_uiIPAim = 0xFFFFFFFF;	// 目标IP地址

	iPos=MakeFrameHead();
	m_oFrameTestSet.OnSetTB(iPos, 0, 0, true);;
	SendTestSetFrame();
	Sleep(100);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,cSetSine,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,cStopSample,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,cOpenPower,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,cOpenPowerTB,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,cOpenSwith,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,cOpenSwitchTB,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,cADCRegisterWrite,16);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,cADCRegisterRead,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);

	iPos=MakeFrameHead();
	m_oFrameTestSet.OnADCSetReturn(iPos);
	SendTestSetFrame();
	Sleep(100);
	
	//零漂
	MakeFrameADClick0();
}

/**
* 生成发送帧头
* @param void
* @return void
*/
int CSocketTestSetFrame::MakeFrameHead()
{	
	m_oFrameTestSet.m_uiIPSource = m_uiIPForInstrument;	// 源IP地址	
	m_oFrameTestSet.m_usPortAim = (unsigned short)m_uiPortForTestSet;	// 目标端口号	
	//m_oFrameTestSet.m_uiIPAim = 0xFFFFFFFF;	// 数据采集输出的目的IP地址
	m_oFrameTestSet.m_usCommand = 1;	// 命令号 1-设置命令；2-查询命令；3-AD采样数据重发
	// 生成帧
	//m_oFrameTestSet.MakeFrameDataForTestSet();
//	int iPos=m_oFrameTestSet.MakeFrameHead();
	int iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, m_oFrameTestSet.m_usCommand, m_oFrameTestSet.m_usPortAim);
	return iPos;
}

/**
* 生成发送帧头,非广播
* @param void
* @return void
*/
int CSocketTestSetFrame::MakeFrameHead1()
{	
	m_oFrameTestSet.m_uiIPSource = m_uiIPForInstrument;	// 源IP地址	
	m_oFrameTestSet.m_usPortAim =(unsigned short)m_uiPortForTestSet;	// 目标端口号	
	m_oFrameTestSet.m_usCommand = 1;	// 命令号 1-设置命令；2-查询命令；3-AD采样数据重发
//	int iPos=m_oFrameTestSet.MakeFrameHead1();
	int iPos = m_oFrameTestSet.ADCSetFrameHead(m_oFrameTestSet.m_uiIPAim, m_oFrameTestSet.m_usCommand, m_oFrameTestSet.m_usPortAim);

	return iPos;
}

/**
* 生成17广播命令帧
* @param void
* @return void
*/
int CSocketTestSetFrame::MakeFrameADListen()
{	
	m_oFrameTestSet.m_uiIPSource = m_uiIPForInstrument;	// 源IP地址	
	m_oFrameTestSet.m_usPortAim = (unsigned short)m_uiPortForTestSet;	// 目标端口号	
	m_oFrameTestSet.m_usCommand = 1;	// 命令号 1-设置命令；2-查询命令；3-AD采样数据重发
	// 生成帧
	//m_oFrameTestSet.MakeFrameDataForTestSet();
//	int iPos=m_oFrameTestSet.MakeFrameHead();
	int iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, m_oFrameTestSet.m_usCommand, m_oFrameTestSet.m_usPortAim);

	// 仪器串号设置命令字
	m_oFrameTestSet.m_pFrameData[iPos] = 0x17;
	iPos++;
	// 广播命令，端口0x5000
	//int m_usPortADListen=PortADListen;
	int m_usPortADListen=PortADListen;
	memcpy(&m_oFrameTestSet.m_pFrameData[iPos], &m_usPortADListen, 4);
	iPos = iPos + 4;
	// 设置命令字结束
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	return iPos;
}

/**
* 生成测试开始帧
* @param void
* @return void
*/
void CSocketTestSetFrame::MakeFrameDataForTestStart()
{	
	m_oFrameTestSet.m_uiIPSource = m_uiIPForInstrument;	// 源IP地址	
	m_oFrameTestSet.m_uiIPAim = 0xFFFFFFFF;	// 目标IP地址	
	m_oFrameTestSet.m_usPortAim = (unsigned short)m_uiPortForTestSet;	// 目标端口号
	m_oFrameTestSet.m_usCommand = 1;	// 命令号 1-设置命令；2-查询命令；3-AD采样数据重发
	m_oFrameTestSet.m_uiIPDataOut = 0xFFFFFFFF;	// 数据采集输出的目的IP地址
	
	// 生成帧
	m_oFrameTestSet.MakeFrameDataForTestStart();
}

/**
* 生成AD测试开始帧 废弃
* @param void
* @return void
*/
void CSocketTestSetFrame::MakeFrameADTestStart()
{	
	int iPos=0;
	unsigned int tnow;
	byte cStopSample[4]={0x81, 0x11, 0x00, 0x00};
	byte cADCReadContinuous[16]={0x8d, 0x40, 0x0a, 0x00, 0x52, 0x08, 0x32, 0x03, 0x6f, 0x0c, 0xff, 0x7d, 0x52, 0x40, 0x00, 0x00};
	byte cADCRegisterWrite[4]={0x81, 0x10, 0x00, 0x00};

	m_oFrameTestSet.MakeFrameDataForADTestStart();
	//tnow=m_oFrameTestSet.m_uiADTimeBegin;
	tnow=(unsigned int)(((double)m_oFrameTestSet.m_uiADTimeBegin / 4096.0) * 1000.0);
	CString strDesc;
	strDesc.Format("Test AD Start IP=%x TBnow=%d",m_oFrameTestSet.m_uiIPAim,tnow);
	// 设置运行状态数据
	m_oRunTimeDataList->Set(1, "CThreadProcTest", "ProcSendTestADStart", strDesc);

	//设置广播端口
	//m_oFrameTestSet.m_uiIPAim = 0xFFFFFFFF;	// 目标IP地址	
	//MakeFrameADListen();		//0x17命令
	//SendTestSetFrame();
	//Sleep(100);

	// 生成帧
	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,cStopSample,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,cADCReadContinuous,16);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(200);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,cADCRegisterWrite,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(800);

	iPos=MakeFrameHead();
	m_oFrameTestSet.OnSetTB(iPos, tnow + TBSleepTimeHigh, TBSleepTimeLow + CmdTBCtrl, false);
	SendTestSetFrame();
}

/**
* 生成AD测试开始帧
* @param void
* @return void
*/
void CSocketTestSetFrame::MakeFrameADIPTestStart()
{	
	int iPos=0; 
	byte hpf0=0x32,hpf1=0x03,pga0=0,pga1=0,pga2=0;
	//sps1=1 1000k sps1=0 250k,sps12=1 4000k
	byte mux0=0,mux1=0,mux2=0,sps0=0,sps1=1,sps2=0,filtr0=0,filtr1=1;
	byte mode=1,sync=0,phs=0,chop=1;

	//byte ad_init41[4]={0x8d,0x40,0x0a,0x00};
	byte ad_init41[4]={0x87,0x40,0x04,0x00};
	/*
	ofccheck[0]=0x6f;
	ofccheck[1]=0x0c;
	ofccheck[2]=0x6f;
	ofccheck[3]=0xff;	//fsccheck0
	ofccheck[4]=0x52;
	ofccheck[5]=0x40;
*/
	switch(m_uiSamplingRate)
	{
		case 250:	
			sps0=0;
			sps1=0;
			sps2=0;
			break;
		case 500:	
			sps0=1;
			sps1=0;
			sps2=0;
			break;
		case 1000:	
			sps0=0;
			sps1=1;
			sps2=0;
			break;
		case 2000:	
			sps0=1;
			sps1=1;
			sps2=0;
			break;
		case 4000:	
			sps0=0;
			sps1=0;
			sps2=1;
			break;
		default:
			sps0=0;
			sps1=0;
			sps2=1;
			break;
	}

//for(IP)
	iPos=MakeFrameHead1();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,ad_init41,4);
	byte m=0x18;
    m_oFrameTestSet.m_pFrameData[iPos]=m;
    iPos++;
	m = (8*sync+4*mode+2*sps2+1*sps1)*16 + (8*sps0+4*phs+2*filtr1+1*filtr0);
	m_oFrameTestSet.m_pFrameData[iPos]=m;
	iPos++;
	m = (4*mux2+2*mux1+1*mux0)*16 + (8*chop+4*pga2+2*pga1+1*pga0);
	m_oFrameTestSet.m_pFrameData[iPos]=m;
	iPos++;
	m_oFrameTestSet.m_pFrameData[iPos]=hpf0;
	iPos++;
	m_oFrameTestSet.m_pFrameData[iPos]=hpf1;
	iPos++;
/*
	m=0x18;
    m_oFrameTestSet.m_pFrameData[iPos]=m;
    iPos++;
	m_oFrameTestSet.m_pFrameData[iPos]=ofccheck[0];
	iPos++;
	m_oFrameTestSet.m_pFrameData[iPos]=ofccheck[1];
	iPos++;
	m_oFrameTestSet.m_pFrameData[iPos]=ofccheck[2];
	iPos++;
	m_oFrameTestSet.m_pFrameData[iPos]=ofccheck[3];	//fsccheck0
	iPos++;

	m=0x18;
    m_oFrameTestSet.m_pFrameData[iPos]=m;
    iPos++;
	m_oFrameTestSet.m_pFrameData[iPos]=ofccheck[4];
	iPos++;
	m_oFrameTestSet.m_pFrameData[iPos]=ofccheck[5];
	iPos++;
	m_oFrameTestSet.m_pFrameData[iPos]=0x0;
	iPos++;
	m_oFrameTestSet.m_pFrameData[iPos]=0x0;
	iPos++;
	*/
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	//Sleep(100);
}

/**
* 生成AD测试开始帧
* @param void
* @return void
*/
void CSocketTestSetFrame::MakeFrameADClickTestStart1()
{	
	int iPos=0; 
	byte ad_02[4]={0xa3,0x05,0xf8,0x40};
	byte cStopSample[4]={0x81, 0x11, 0x00, 0x00};

	//设置广播端口
	//m_oFrameTestSet.m_uiIPAim = 0xFFFFFFFF;	// 目标IP地址	
	//MakeFrameADListen();		//0x17命令
	//SendTestSetFrame();
	//Sleep(100);

	// 生成帧
	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,ad_02,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,cStopSample,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);
}

/**
* 生成AD测试开始帧
* @param void
* @return void
*/
void CSocketTestSetFrame::MakeFrameADClickTestStart2()
{	
	int iPos=0; 
	unsigned int tnow;
	byte cADCRegisterWrite[4]={0x81, 0x10, 0x00, 0x00};

	m_oFrameTestSet.MakeFrameDataForADTestStart();
	//tnow=m_oFrameTestSet.m_uiADTimeBegin;
	tnow=(unsigned int)(((double)m_oFrameTestSet.m_uiADTimeBegin / 4096.0) * 1000.0);
	CString strDesc;
	strDesc.Format("Test AD Start IP=%x TBnow=%d",m_oFrameTestSet.m_uiIPAim,tnow);
	// 设置运行状态数据
	m_oRunTimeDataList->Set(1, "CThreadProcTest", "ProcSendTestADStart", strDesc);
	TRACE3("Test AD Start IP=%x m_uiADTimeBegin=%d,TBnow=%d\r\n",m_oFrameTestSet.m_uiIPAim,m_oFrameTestSet.m_uiADTimeBegin,tnow);
	
	OnADCSetTBTime(tnow);
// 	iPos=MakeFrameHead();
// 	iPos = m_oFrameTestSet.ADCCommand_18(iPos,cADCRegisterWrite,4);
// 	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
// 	SendTestSetFrame();
// 	Sleep(500);
// 
// 	iPos=MakeFrameHead();
// //	m_oFrameTestSet.OnSetTB(tnow);
// 	m_oFrameTestSet.OnSetTB(iPos, tnow, TBSleepTimeLow + CmdTBCtrl, false);
// 	SendTestSetFrame();
// 	Sleep(100);

/*	//检查TB，查询TB时间
	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.GetTB();
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);
	*/
}	


/**
* 生成AD测试开始帧
* @param void
* @return void
*/
void CSocketTestSetFrame::ViewSet(int itype)
{
	CString m_strIPLCI=ViewIPStr;
	UINT m_uiPortLCI=ViewPort;
	m_oFrameTestSet.m_uiIPAim=inet_addr(m_strIPLCI);
	m_oFrameTestSet.m_uiIPSource = m_uiIPForInstrument;	// 源IP地址	
	m_oFrameTestSet.m_usPortAim = (unsigned short)m_uiPortForTestSet;	// 目标端口号	
	if(itype==1)
		m_oFrameTestSet.m_usCommand = 3;	// 命令号 1-设置命令；2-查询命令；3-AD采样数据重发
	else
		m_oFrameTestSet.m_usCommand = 4;	// 命令号 1-结束命令
	int iPos = m_oFrameTestSet.ADCSetFrameHead(m_oFrameTestSet.m_uiIPAim, m_oFrameTestSet.m_usCommand, m_oFrameTestSet.m_usPortAim);
	memcpy(&m_oFrameTestSet.m_pFrameData[24], &m_uiPortLCI, FramePacketSize2B);
	int subcmd=1;
	memcpy(&m_oFrameTestSet.m_pFrameData[26], &subcmd, FramePacketSize2B);
	memcpy(&m_oFrameTestSet.m_pFrameData[28], &m_uiIPCount, FramePacketSize2B);
	int iCount = SendTo(m_oFrameTestSet.m_pFrameData, FrameLength, m_uiPortLCI, ViewIPStr);
}
/**
* 生成AD测试停止
* @param unsigned int uiInstrumentIP 仪器IP地址
* @return void
*/
void CSocketTestSetFrame::MakeFrameADTestSTOP(unsigned int uiInstrumentIP)
{	
	int iPos=0;
	byte cStopSample[4]={0x81, 0x11, 0x00, 0x00};

	// 生成帧
	//m_oFrameTestSet.MakeFrameDataForTestEnd();
	m_oFrameTestSet.MakeFrameDataForADTestEnd();
	for(int i=0;i<1;i++){
		iPos=MakeFrameHead();
		iPos = m_oFrameTestSet.ADCCommand_18(iPos,cStopSample,4);
		m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
		SendTestSetFrame();
		Sleep(100);
		iPos=MakeFrameHead();
		m_oFrameTestSet.OnSetTB(iPos, 0, 0, true);
		SendTestSetFrame();
		Sleep(100);		
	}
}

/**
* 生成AD零漂帧
* @param 
* @return void
*/
void CSocketTestSetFrame::MakeFrameADClick0()
{	
	int iPos=0;
	byte ad_01[4]={0xa3,0x05,0xf8,0x00};
	byte ad_03[4]={0x81,0x11,0x00,0x00};
	byte ad_04[4]={0x81,0x11,0x00,0x00};
	byte ad_05[4]={0x8d,0x40,0x0a,0x00};
	//byte cADCReadContinuous[16]={0x8d, 0x40, 0x0a, 0x00, 0x52, 0x08, 0x32, 0x03, 0x6f, 0x0c, 0xff, 0x7d, 0x52, 0x40, 0x00, 0x00};
	byte ad_06[4]={0x82,0x20,0x0a,0x00};
	byte ad_sch[4] = {0x00,0x00,0x00,0x00};
	byte ad_07[4]={0x81,0x11,0x00,0x00};
	byte ad_08[4]={0x81,0x04,0x00,0x00};
	byte ad_09[4]={0x81,0x10,0x00,0x00};
	byte ad_010[4]={0x81,0x11,0x00,0x00};
	byte ad_011[4]={0x81,0x11,0x00,0x00};
	byte ad_012[4]={0x81,0x60,0x00,0x00};
	byte ad_013[4]={0x81,0x10,0x00,0x00};
	byte ad_014[4]={0x81,0x11,0x00,0x00};
	byte ad_015[4]={0x81,0x11,0x00,0x00};
	byte ad_016[4]={0x82,0x20,0x0a,0x00};
	byte ofc0=0x6f,ofc1=0x0c,ofc2=0xff;
	byte hpf0=0x32,hpf1=0x03,pga0=0,pga1=0,pga2=0;
	byte fsc0=0x7d,fsc1=0x52,fsc2=0x40;
	//sps1=1 1000k sps1=0 250k,sps2=1 4000k
	byte mux0=0,mux1=0,mux2=0,sps0=0,sps1=1,sps2=0,filtr0=0,filtr1=1;
	byte mode=1,sync=0,phs=0,chop=1;

	switch(m_uiSamplingRate)
	{
		case 250:	
			sps0=0;
			sps1=0;
			sps2=0;
			break;
		case 500:	
			sps0=1;
			sps1=0;
			sps2=0;
			break;
		case 1000:	
			sps0=0;
			sps1=1;
			sps2=0;
			break;
		case 2000:	
			sps0=1;
			sps1=1;
			sps2=0;
			break;
		case 4000:	
			sps0=0;
			sps1=0;
			sps2=1;
			break;
		default:
			sps0=0;
			sps1=0;
			sps2=1;
			break;
	}

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,ad_01,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,ad_03,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,ad_04,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,ad_05,4);

	byte m=0x18;
    m_oFrameTestSet.m_pFrameData[iPos]=m;
    iPos++;
	m = (8*sync+4*mode+2*sps2+1*sps1)*16 + (8*sps0+4*phs+2*filtr1+1*filtr0);
    m_oFrameTestSet.m_pFrameData[iPos]=m;
	iPos++;
	m = (4*mux2+2*mux1+1*mux0)*16 + (8*chop+4*pga2+2*pga1+1*pga0);
	m_oFrameTestSet.m_pFrameData[iPos]=m;
	iPos++;
	m_oFrameTestSet.m_pFrameData[iPos]=hpf0;
	iPos++;
	m_oFrameTestSet.m_pFrameData[iPos]=hpf1;
	iPos++;

	m=0x18;
    m_oFrameTestSet.m_pFrameData[iPos]=m;
    iPos++;
	m_oFrameTestSet.m_pFrameData[iPos]=ofc0;
	iPos++;
	m_oFrameTestSet.m_pFrameData[iPos]=ofc1;
	iPos++;
	m_oFrameTestSet.m_pFrameData[iPos]=ofc2;
	iPos++;
	m_oFrameTestSet.m_pFrameData[iPos]=fsc0;
	iPos++;

	m=0x18;
    m_oFrameTestSet.m_pFrameData[iPos]=m;
    iPos++;
	m_oFrameTestSet.m_pFrameData[iPos]=fsc1;
	iPos++;
	m_oFrameTestSet.m_pFrameData[iPos]=fsc2;
	iPos++;
	m_oFrameTestSet.m_pFrameData[iPos]=0x0;
	iPos++;
	m_oFrameTestSet.m_pFrameData[iPos]=0x0;
	iPos++;
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,ad_06,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);

	//iPos=MakeFrameHead1();
/*	iPos=MakeFrameHead();
	m_oFrameTestSet.m_pFrameData[26]=0x02;
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,ad_sch,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);
*/
	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,ad_07,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,ad_08,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(200);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,ad_09,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(700);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,ad_010,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,ad_011,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,ad_012,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(200);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,ad_013,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(700);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,ad_014,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,ad_015,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);

	iPos=MakeFrameHead();
	iPos = m_oFrameTestSet.ADCCommand_18(iPos,ad_016,4);
	m_oFrameTestSet.m_pFrameData[iPos] = 0x00;
	SendTestSetFrame();
	Sleep(100);
}


/**
* 生成数据查询帧
* @param void
* @return void
*/
void CSocketTestSetFrame::ReloadTestDataFrame(unsigned int UiIP,int i_idx)
{	
	int iPos=0;
	m_oFrameTestSet.m_uiIPSource = m_uiIPForInstrument;	// 源IP地址	
	m_oFrameTestSet.m_usPortAim = (unsigned short)m_uiPortForTestSet;	// 目标端口号	
	m_oFrameTestSet.m_usCommand = 3;	// 命令号 1-设置命令；2-查询命令；3-AD采样数据重发
	// 生成帧
	//m_oFrameTestSet.MakeFrameDataForTestSet();
//	iPos=m_oFrameTestSet.MakeFrameHead();
	iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, m_oFrameTestSet.m_usCommand, m_oFrameTestSet.m_usPortAim);
	iPos=32;
	memcpy(&m_oFrameTestSet.m_pFrameData[20], &UiIP, 4);
	unsigned int PortMove = 0x5000;
	memcpy(&m_oFrameTestSet.m_pFrameData[24], &PortMove, 2);
	memcpy(&m_oFrameTestSet.m_pFrameData[iPos], &i_idx, 2);
	m_oFrameTestSet.m_pFrameData[iPos+5]=0;
	SendTestSetFrame();
}


/**
* 生成测试停止帧
* @param unsigned int uiInstrumentIP 仪器IP地址
* @return void
*/
void CSocketTestSetFrame::MakeFrameDataForTestEnd(unsigned int uiInstrumentIP)
{	
	m_oFrameTestSet.m_uiIPSource = m_uiIPForInstrument;	// 源IP地址	
	m_oFrameTestSet.m_uiIPAim = uiInstrumentIP;	// 目标IP地址	
	m_oFrameTestSet.m_usPortAim = 0;	// 目标端口号	
	m_oFrameTestSet.m_usCommand = 1;	// 命令号 1-设置命令；2-查询命令；3-AD采样数据重发
	// 生成帧
	m_oFrameTestSet.MakeFrameDataForTestEnd();
}

/**
* 发送测试设置帧
* @param void
* @return void
*/
void CSocketTestSetFrame::SendTestSetFrame()
{
	// 发送帧
	//AppCommandWord(m_oFrameTestSet.m_pFrameData);
	int iCount = SendTo(m_oFrameTestSet.m_pFrameData, FrameSetLength, m_uiPortLCI, m_strIPLCI);
	if(iCount==FrameSetLength)
		adsetcount++;
//	else
//		TRACE2("SocketTestSetFrame Error m_uiPortLCI=%d,\r\n", m_uiPortLCI,iCount);
	Sleep(1);
}


/**
* 发送测试停止帧
* @param void
* @return void
*/
void CSocketTestSetFrame::SendTestEndFrame()
{
	// 发送帧
	//int iCount = SendTo(m_oFrameTestSet.m_pFrameData, FrameSetLength, m_uiPortLCI, m_strIPLCI);
	SendTestSetFrame();
	Sleep(1);
}

/**
* 解析命令字
* @param void
* @return bool true：成功；false：失败
*/
bool CSocketTestSetFrame::AppCommandWord(unsigned char * pBuf)
{
	byte pData[5];	// 命令字数据
	byte byCommandWord;	// 命令字
	int iMaxCommandWordCount = 8;	// 最多解析多少个命令字
	int iCommandWordCount = 0;	// 命令字计数

	while(true)
	{
		// 得到命令字数据
		memcpy(pData, &pBuf[32 + iCommandWordCount * 5], 5);
		// 命令字
		byCommandWord = pData[0];

		if(0x0 == byCommandWord)	//无命令字
		{
			byte cmd0f=0x0f;
			pBuf[32 + iCommandWordCount * 5]=cmd0f;
			memcpy(&pBuf[32 + iCommandWordCount * 5+1], &adsetcount, 4);
			iCommandWordCount++;
			cmd0f=0;
			pBuf[32 + iCommandWordCount * 5]=cmd0f;
			break;
		}

		if(iCommandWordCount == iMaxCommandWordCount)	//所有命令字解析完
		{
			break;
		}
		iCommandWordCount++;
	}

	return true;
}

void CSocketTestSetFrame::resetcheckrecv()
{
	for(int i=0;i<ADcheckrecv;i++)
		checkrecv[i]=0;
}
// 处理零漂校正查询应答
//************************************
// Method:    OnProcADCZeroDriftReturn
// FullName:  CSysTimeSocket::OnProcADCZeroDriftReturn
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int iPos
//************************************
void CSocketTestSetFrame::OnProcADCZeroDriftReturn(unsigned int uiIPAim)
{
	int iPos = 0;
	byte	ucCommand = 0;
	CTestElementData* pTestElementData = NULL;	// 测试单元
	if (TRUE == m_pTestElementList->GetTestElementDataFromMap(uiIPAim, pTestElementData))
	{
		iPos = 33;
		memcpy(&ucCommand, &ReceiveBuf[iPos], FrameCmdSize1B);
		iPos += FrameCmdSize1B;
		if (ucCommand == CmdADCSet)
		{
			iPos = 41;
			memcpy(&pTestElementData->ofccheck[0], &ReceiveBuf[iPos], FramePacketSize2B);
			iPos += FramePacketSize2B;
			iPos += FrameCmdSize1B;
			memcpy(&pTestElementData->ofccheck[2], &ReceiveBuf[iPos], FramePacketSize2B);
			iPos += FramePacketSize2B;
			memcpy(&pTestElementData->ofccheck[4], &ReceiveBuf[iPos], FramePacketSize2B);
		}
	}
}
// 处理ADC设置应答帧
void CSocketTestSetFrame::OnProcADCSetReturn(unsigned int uiIP)
{
	if (m_uiADCSetOperationNb == 0)
	{
		return;
	}
	CTestElementData* pTestElementData = NULL;	// 测试单元
	if (TRUE == m_pTestElementList->GetTestElementDataFromMap(uiIP, pTestElementData))
	{
		pTestElementData->m_uiADCSetOperationNb = m_uiADCSetOperationNb;
	} 
}

// 检查是否收到所有采集站的ADC命令应答
BOOL CSocketTestSetFrame::OnCheckADCSetReturn(void)
{
	CString str = _T("");
	CTestElementData* pTestElementData = NULL;	// 测试单元
	for(unsigned int i = 0; i < m_uiIPCount; i++)
	{
		// 得到一个单元
		pTestElementData = m_pTestElementList->GetTestElementDataByIndex(i);
		if (pTestElementData->m_uiADCSetOperationNb != m_uiADCSetOperationNb)
		{
			TRACE2("未收到ADC参数设置应答的设备序号为%d Nb=%d!\r\n", i,m_uiADCSetOperationNb);
			str.Format(_T("未收到ADC参数设置应答的设备序号为%d! IP=%d Nb=%d "), i,pTestElementData->m_uiIP,m_uiADCSetOperationNb);
			m_oRunTimeDataList->Set(1, "CThreadProcTest", "ProcSendTestADSet", str);
			//return FALSE;
		}
	}
	str.Format(_T("ADC参数设置应答接收完全序号为%d!\r\n"), m_uiADCSetOperationNb);
	//TRACE(str);
	return TRUE;
}
// 重置ADC参数设置操作序号
void CSocketTestSetFrame::OnResetADCOperationNb(void)
{
	CTestElementData* pTestElementData = NULL;	// 测试单元
	for(unsigned int i = 0; i < m_uiIPCount; i++)
	{
		// 得到一个单元
		pTestElementData = m_pTestElementList->GetTestElementDataByIndex(i);
		pTestElementData->m_uiADCSetOperationNb = 0;
	}
}

// ADC参数设置
//************************************
// Method:    OnADCSet
// FullName:  CADCSet::OnADCSet
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CSocketTestSetFrame::OnADCSet(void)
{
	m_uiADCSetOperationNb = 1;
	OnADCSetOperation();
}

// ADC数据采集停止
//************************************
// Method:    OnADCSampleStop
// FullName:  CADCSet::OnADCSampleStop
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CSocketTestSetFrame::OnADCSampleStop(void)
{
	m_uiADCSetOperationNb = 35;
	OnADCSetOperation();
}

// ADC零漂校正
//************************************
// Method:    OnADCZeroDrift
// FullName:  CADCSet::OnADCZeroDrift
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CSocketTestSetFrame::OnADCZeroDrift(void)
{
	m_uiADCSetOperationNb = 12;
	OnADCSetOperation();
}

// ADC设置TB时刻开始采集
//************************************
// Method:    OnADCStartSample
// FullName:  CADCSet::OnADCStartSample
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: unsigned int uiIPAim
// Parameter: unsigned int tnow
//************************************
void CSocketTestSetFrame::OnADCPrepareToSample(void)
{
	m_uiADCSetOperationNb = 28;
	OnADCSetOperation();
}
// ADC设置TB时刻
void CSocketTestSetFrame::OnADCSetTBTime(unsigned int tnow)
{
	m_uiADCSetOperationNb = 33;
	m_uiTnow = tnow;
	OnADCSetOperation();
}
// 发送ADC命令设置帧
void CSocketTestSetFrame::OnSendADCSetCmd(void)
{
	int iPos = 0;
	CString str = _T("");
	CTestElementData* pTestElementData = NULL;	// 测试单元
	// 1~11为ADC参数设置命令
	// 12~27为ADC零漂校正指令
	str.Format(_T("ADC参数设置发送命令序号%d!\r\n"), m_uiADCSetOperationNb);
	//TRACE(str);
	switch (m_uiADCSetOperationNb)
	{
	case 1:
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd, (unsigned short)m_uiPortForTestSet);
		m_oFrameTestSet.OnSetTB(iPos, 0, 0, true);
		SendTestSetFrame();
/*		for(unsigned int i = 0; i < 16; i++){
			Sleep(50);
			m_oFrameTestSet.OnNULL(iPos);
			SendTestSetFrame();
		}
*/		break;
	case 2:
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd, (unsigned short)m_uiPortForTestSet);
		m_oFrameTestSet.OnSetSine(iPos);
		SendTestSetFrame();
		break;
	case 3:
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd,(unsigned short) m_uiPortForTestSet);
		m_oFrameTestSet.OnStopSample(iPos);
		SendTestSetFrame();
		break;
	case 4:
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd,(unsigned short) m_uiPortForTestSet);
		m_oFrameTestSet.OnOpenPowerTBLow(iPos);
		SendTestSetFrame();
		break;
	case 5:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd,(unsigned short) m_uiPortForTestSet);
		m_oFrameTestSet.OnOpenPowerTBHigh(iPos);
		SendTestSetFrame();
		break;
	case 6:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd,(unsigned short) m_uiPortForTestSet);
		m_oFrameTestSet.OnOpenSwitchTBLow(iPos);
		SendTestSetFrame();
		break;
	case 7:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd,(unsigned short) m_uiPortForTestSet);
		m_oFrameTestSet.OnOpenSwitchTBHigh(iPos);
		SendTestSetFrame();
		break;
	case 8:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd, (unsigned short)m_uiPortForTestSet);
		m_oFrameTestSet.OnStopSample(iPos);
		SendTestSetFrame();
		break;
	case 9:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd, (unsigned short)m_uiPortForTestSet);
		m_oFrameTestSet.OnADCRegisterWrite(iPos, false);
		SendTestSetFrame();
		break;
	case 10:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd,(unsigned short) m_uiPortForTestSet);
		m_oFrameTestSet.OnADCRegisterRead(iPos);
		SendTestSetFrame();
		break;
	case 11:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd, (unsigned short)m_uiPortForTestSet);
		m_oFrameTestSet.OnADCSetReturn(iPos);
		SendTestSetFrame();
		break;
	case 12:
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd,(unsigned short) m_uiPortForTestSet);
		m_oFrameTestSet.OnOpenPowerZeroDrift(iPos);
		SendTestSetFrame();
		break;
	case 13:
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd,(unsigned short) m_uiPortForTestSet);
		m_oFrameTestSet.OnStopSample(iPos);
		SendTestSetFrame();
		break;
	case 14:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd, (unsigned short)m_uiPortForTestSet);
		m_oFrameTestSet.OnStopSample(iPos);
		SendTestSetFrame();
		break;
	case 15:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd, (unsigned short)m_uiPortForTestSet);
		//m_oFrameTestSet.OnADCRegisterWrite(iPos, true);
		m_oFrameTestSet.OnADCRegisterWrite(iPos, false);
		SendTestSetFrame();
/*		for(unsigned int i = 0; i < 16; i++){
			Sleep(50);
			m_oFrameTestSet.OnNULL(iPos);
			SendTestSetFrame();
		}
*/		break;
	case 16:
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd,(unsigned short) m_uiPortForTestSet);
		m_oFrameTestSet.OnADCRegisterRead(iPos);
		SendTestSetFrame();
		break;
	case 17:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendQueryCmd, (unsigned short)m_uiPortForTestSet);
		m_oFrameTestSet.OnADCRegisterQuery(iPos);
		SendTestSetFrame();
		break;
	case 18:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd, (unsigned short)m_uiPortForTestSet);
		m_oFrameTestSet.OnStopSample(iPos);
		SendTestSetFrame();
		break;
	case 19:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd, (unsigned short)m_uiPortForTestSet);
		m_oFrameTestSet.OnADCSampleSynchronization(iPos);
		SendTestSetFrame();
		break;
	case 20:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd, (unsigned short)m_uiPortForTestSet);
		m_oFrameTestSet.OnADCReadContinuous(iPos);
		SendTestSetFrame();
		break;
	case 21:
		Sleep(ADCReadContinuousSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd, (unsigned short)m_uiPortForTestSet);
		m_oFrameTestSet.OnStopSample(iPos);
		SendTestSetFrame();
		break;
	case 22:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd, (unsigned short)m_uiPortForTestSet);
		m_oFrameTestSet.OnStopSample(iPos);
		SendTestSetFrame();
		break;
	case 23:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd,(unsigned short) m_uiPortForTestSet);
		m_oFrameTestSet.OnADCZeroDriftCorrection(iPos);
		SendTestSetFrame();
		break;
	case 24:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd, (unsigned short)m_uiPortForTestSet);
		m_oFrameTestSet.OnADCReadContinuous(iPos);
		SendTestSetFrame();
		break;
	case 25:
		Sleep(ADCReadContinuousSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd,(unsigned short) m_uiPortForTestSet);
		m_oFrameTestSet.OnStopSample(iPos);
		SendTestSetFrame();
		break;
	case 26:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd, (unsigned short)m_uiPortForTestSet);
		m_oFrameTestSet.OnStopSample(iPos);
		SendTestSetFrame();
		break;
	case 27:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd, (unsigned short)m_uiPortForTestSet);
		m_oFrameTestSet.OnADCRegisterRead(iPos);
		SendTestSetFrame();
		break;
	case 28:
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd,(unsigned short) m_uiPortForTestSet);
		m_oFrameTestSet.OnSetTBSwitchOpen(iPos);
		SendTestSetFrame();
		break;
	case 29:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd,(unsigned short) m_uiPortForTestSet);
		m_oFrameTestSet.OnStopSample(iPos);
		SendTestSetFrame();
		break;
	case 30:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd, (unsigned short)m_uiPortForTestSet);
		m_oFrameTestSet.OnSetTB(iPos, 0, 0, true);
		SendTestSetFrame();
/*		for(unsigned int i = 0; i < 16; i++){
			Sleep(50);
			m_oFrameTestSet.OnNULL(iPos);
			SendTestSetFrame();
		}
*/		break;
	case 31:
		//Sleep(500);
		Sleep(ADCOperationSleepTime);
/*		// 按照IP地址发送零漂矫正帧写寄存器
		for(unsigned int i = 0; i < m_uiIPCount; i++)
		{
			// 得到一个单元
			pTestElementData = m_pTestElementList->GetTestElementDataByIndex(i);
			iPos = m_oFrameTestSet.ADCSetFrameHead(pTestElementData->m_uiIP, SendSetCmd, m_uiPortForTestSet);
			m_oFrameTestSet.OnADCZeroDriftSetFromIP(iPos, &pTestElementData->ofccheck[0]);
			SendTestSetFrame();
			Sleep(100);
		}
*/		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd,(unsigned short) m_uiPortForTestSet);
		m_oFrameTestSet.OnADCZeroDriftSetFromIP(iPos, &pTestElementData->ofccheck[0]);
		SendTestSetFrame();
		//Sleep(500);
		break;
	case 32:
		Sleep(ADCOperationSleepTime);
		// 按照IP地址发送ADC连续采集命令
/*		for(unsigned int i = 0; i < m_uiIPCount; i++)
		{
			// 得到一个单元
			pTestElementData = m_pTestElementList->GetTestElementDataByIndex(i);
			iPos = m_oFrameTestSet.ADCSetFrameHead(pTestElementData->m_uiIP, SendSetCmd, m_uiPortForTestSet);
			m_oFrameTestSet.OnADCReadContinuous(iPos);
			SendTestSetFrame();
			Sleep(100);
		}
*/		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd, (unsigned short)m_uiPortForTestSet);
		m_oFrameTestSet.OnADCReadContinuous(iPos);
		SendTestSetFrame();
		//Sleep(100);
		break;
	case 33:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd, (unsigned short)m_uiPortForTestSet);
		m_oFrameTestSet.OnSetTB(iPos, m_uiTnow, TBSleepTimeLow + CmdTBCtrl, false);
		SendTestSetFrame();
/*		for(unsigned int i = 0; i < 16; i++){
			Sleep(50);
			m_oFrameTestSet.OnNULL(iPos);
			SendTestSetFrame();
		}
*/		break;
	case 34:
		Sleep(ADCOperationSleepTime);
		// 广播查询采集站TB时刻
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendQueryCmd, (unsigned short)m_uiPortForTestSet);
		m_oFrameTestSet.OnQueryTBTime(iPos);
		SendTestSetFrame();
		break;
	case 35:
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd, (unsigned short)m_uiPortForTestSet);
		m_oFrameTestSet.OnStopSample(iPos);
		SendTestSetFrame();
		break;
	case 36:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd, (unsigned short)m_uiPortForTestSet);
		m_oFrameTestSet.OnStopSample(iPos);
		SendTestSetFrame();
		break;
	case 37:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd, (unsigned short)m_uiPortForTestSet);
		m_oFrameTestSet.OnStopSample(iPos);
		SendTestSetFrame();
		break;
	case 38:
		Sleep(ADCOperationSleepTime);
		iPos = m_oFrameTestSet.ADCSetFrameHead(BroadCastPort, SendSetCmd,(unsigned short) m_uiPortForTestSet);
		m_oFrameTestSet.OnSetTB(iPos, 0, 0, true);
		SendTestSetFrame();
		break;
	default:
		break;
	}
}
// 清空ADC参数设置接收缓冲区
void CSocketTestSetFrame::OnClearADCSetReturnBuf(void)
{
	// 得到网络接收缓冲区数据字节数
	DWORD dwFrameCount = 0;
	int icount = 0;
	while (IOCtl(FIONREAD, &dwFrameCount))
	{
		if(dwFrameCount > 0) 
		{
			icount = Receive(m_ucUdpBuf, ADCSetReturnBufSize);
		}
		else
		{
			break;
		}
	}
}
// 接收ADC参数设置应答帧
void CSocketTestSetFrame::OnReceiveADCSetReturn(int itype)
{
	if(itype==0)
		Sleep(ADCSetReturnSleepTime);
	// 得到网络接收缓冲区数据字节数
	DWORD dwFrameCount = 0;
	int icount = 0;
	CString str = _T("");
	while (IOCtl(FIONREAD, &dwFrameCount))
	{
		if(dwFrameCount > 0) 
		{
			icount = Receive(m_ucUdpBuf, ADCSetReturnBufSize);
			if (icount != SOCKET_ERROR)
			{
				OnProcess(icount);
			}
		}
		else
		{
			break;
		}
	}
}
// 清空ADC参数设置接收缓冲区
// 数据处理
//************************************
// Method:    OnProcess
// FullName:  CADCDataRecThread::OnProcess
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int iCount
//************************************
void CSocketTestSetFrame::OnProcess(int iCount)
{
	for (int i=0; i<iCount; i++)
	{
		switch(m_uiUdpCount)
		{
		case 0:
			if (m_ucUdpBuf[i] == FrameHeadCheck0)
			{
				ReceiveBuf[0] = m_ucUdpBuf[i];
				m_uiUdpCount++;
				break;
			}
			else
			{
				//	m_uiUdpCount = 0;
				m_uiUdpCount++;
				break;
			}

		case 1:
			if (m_ucUdpBuf[i] == FrameHeadCheck1)
			{
				ReceiveBuf[1] = m_ucUdpBuf[i];
				m_uiUdpCount++;
				break;
			}
			else
			{
				//	m_uiUdpCount = 0;
				m_uiUdpCount++;
				break;
			}
		case 2:
			if (m_ucUdpBuf[i] == FrameHeadCheck2)
			{
				ReceiveBuf[2] = m_ucUdpBuf[i];
				m_uiUdpCount++;
				break;
			}
			else
			{
				//	m_uiUdpCount = 0;
				m_uiUdpCount++;
				break;
			}
		case 3:
			if (m_ucUdpBuf[i] == FrameHeadCheck3)
			{
				ReceiveBuf[3] = m_ucUdpBuf[i];
				m_uiUdpCount++;
				break;
			}
			else
			{
				//	m_uiUdpCount = 0;
				m_uiUdpCount++;
				break;
			}
		default:
			if (m_uiUdpCount >=  RcvFrameSize)
			{
				m_uiUdpCount = 0;
				break;
			}
			ReceiveBuf[m_uiUdpCount] = m_ucUdpBuf[i];
			m_uiUdpCount++;
			if (m_uiUdpCount == RcvFrameSize)
			{
				m_uiUdpCount = 0;
				//TRACE("1");
				OnProcessOneADCSetReturn();
			}
			break;
		}
	}
}
// 处理一帧ADC参数设置应答帧
void CSocketTestSetFrame::OnProcessOneADCSetReturn(void)
{
	unsigned short usCommand = 0;
	byte	ucCommand = 0;
	unsigned int uiIPAim = 0;
	int iPos = 0;
	iPos = 16;
	memcpy(&uiIPAim, &ReceiveBuf[iPos], FramePacketSize4B);
	iPos = 26;
	memcpy(&usCommand, &ReceiveBuf[iPos], FramePacketSize2B);
	iPos += FramePacketSize2B;
	if (usCommand == SendQueryCmd)
	{
		memcpy(&ucCommand, &ReceiveBuf[iPos], FrameCmdSize1B);
		iPos += FrameCmdSize1B;
		if (ucCommand == CmdBroadCastPortSet)
		{
			// ADC零漂校正查询应答
			OnProcADCZeroDriftReturn(uiIPAim);
			OnProcADCSetReturn(uiIPAim);
		}
	}
	else if (usCommand == SendSetCmd)
	{
		iPos += FramePacketSize2B;
		OnProcADCSetReturn(uiIPAim);
	}
	else if (usCommand == ViewStartCmd)
	{
		int subcmd=0;
		memcpy(&subcmd, &ReceiveBuf[iPos], FrameCmdSize1B);
		if(subcmd==0x20){
			m_uiSamplingView=0;
			memcpy(&m_uiSamplingView, &ReceiveBuf[iPos+1], FramePacketSize2B);
			Viewtype=1;
			ViewSet(1);
		}
	}
	else if (usCommand == ViewStopCmd)
	{
		iPos += FramePacketSize2B;
		int subcmd=0;
		memcpy(&subcmd, &ReceiveBuf[iPos], FrameCmdSize1B);
		Viewtype=0;
	}
}
// 检查ADC参数设置应答帧是否接收完全
void CSocketTestSetFrame::OnCheckADCSetReturnComplete(void)
{
	if(TRUE == OnCheckADCSetReturn())
	{
		// 执行下一步ADC命令发送并开启应答监视定时器
		m_uiADCSetOperationNb++;
		// 完成ADC参数设置
		if (m_uiADCSetOperationNb == 12)
		{
			m_uiADCSetOperationNb = 0;
			return;
		}
		// 完成ADC零漂校正
		else if (m_uiADCSetOperationNb == 28)
		{
			m_uiADCSetOperationNb = 0;
			return;
		}
		// 完成ADC数据采集准备
		else if (m_uiADCSetOperationNb == 33)
		{
			m_uiADCSetOperationNb = 0;
			return;
		}
		// 完成TB时间设置
		else if (m_uiADCSetOperationNb == 35)
		{
			m_uiADCSetOperationNb = 0;
			return;
		}
		// 完成ADC停止数据采集
		else if (m_uiADCSetOperationNb == 39)
		{
			m_uiADCSetOperationNb = 0;
			return;
		}
	}
}

// ADC参数设置操作过程
void CSocketTestSetFrame::OnADCSetOperation(void)
{
	// 清空ADC接收缓冲区
	OnClearADCSetReturnBuf();
	int iCount=0;
	int iNb=m_uiADCSetOperationNb;
	int irevCount=0;
	while(m_uiADCSetOperationNb != 0)
	{
		// 重置设备的ADC参数设置操作序号
		OnResetADCOperationNb();
		iNb=m_uiADCSetOperationNb;
		// 发送ADC参数设置命令
		OnSendADCSetCmd();
		// 延时一定时间后接收设置应答
		OnReceiveADCSetReturn(0);
		// 检查是否收到所有采集站的ADC命令应答
		OnCheckADCSetReturnComplete();
		if(iNb==m_uiADCSetOperationNb)
			irevCount++;
		else
			irevCount=0;
		if(irevCount>3){
			break;
		}
		if(iCount>100){
			m_uiADCSetOperationNb =0;
			OnClearADCSetReturnBuf();
			continue;
		}
		if(iCount>300){
			break;
		}
		iCount++;
	}
}
