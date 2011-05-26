#include "StdAfx.h"
#include "ADCSet.h"
#include "Parameter.h"
CADCSet::CADCSet(void)
{
}

CADCSet::~CADCSet(void)
{
}

// ADC设置帧头
void CADCSet::ADCSetFrameHead(unsigned int uiIPAim)
{
	unsigned int	uiIPSource	=	0;
	unsigned short	usPortAim	=	0;
	unsigned short	usCommand	=	0;
	unsigned int	itmp		=	0;
	unsigned char	ucCommand;
	unsigned int	uiADCBroadcastPort;
	m_cFrameData[0] = 0x11;
	m_cFrameData[1] = 0x22;
	m_cFrameData[2] = 0x33;
	m_cFrameData[3] = 0x44;
	m_cFrameData[4] = 0x00;
	m_cFrameData[5] = 0x00;
	m_cFrameData[6] = 0x00;
	m_cFrameData[7] = 0x00;
	m_cFrameData[8] = 0x00;
	m_cFrameData[9] = 0x00;
	m_cFrameData[10] = 0x00;
	m_cFrameData[11] = 0x00;
	m_cFrameData[12] = 0x00;
	m_cFrameData[13] = 0x00;
	m_cFrameData[14] = 0x00;
	m_cFrameData[15] = 0x00;

	// 得到本机IP地址
	char		name[255]; 
	CString		ip; 
	PHOSTENT	hostinfo; 
	if(   gethostname   (name, sizeof(name)) == 0) 
	{ 
		if((hostinfo = gethostbyname(name)) != NULL) 
		{ 
			ip = inet_ntoa(*(struct in_addr*)*hostinfo-> h_addr_list); 
		} 
	} 

	uiIPSource	=	inet_addr(ip);
	usPortAim	=	ADSetReturnPort;
	usCommand	=	0x0001;
	// 源IP地址
	memcpy(&m_cFrameData[16], &uiIPSource, 4);
	// 广播
	uiIPAim = 0xffffffff;
	// 目标IP地址
	memcpy(&m_cFrameData[20], &uiIPAim, 4);
	TRACE1("尾包时刻查询帧-仪器IP地址：%d\r\n", uiIPAim);
	// 目标端口号
	memcpy(&m_cFrameData[24], &usPortAim, 2);
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&m_cFrameData[26], &usCommand, 2);

	memcpy(&m_cFrameData[28], &itmp, 4);
 	//广播命令
 	ucCommand = 0x17;
 	memcpy(&m_cFrameData[32],&ucCommand,1);
 	//广播命令端口
 	uiADCBroadcastPort = ADCSetBroadcastPort;
 	memcpy(&m_cFrameData[33],&uiADCBroadcastPort,4);
}

// 发送ADC设置的18指令
int CADCSet::ADCCommand_18(int iPos, byte * cADCSet, unsigned int uiLength)
{
	for(unsigned int i=0; i<uiLength; i+=4)
	{
		m_cFrameData[iPos] = 0x18;
		iPos++;
		memcpy(&m_cFrameData[iPos], cADCSet+i, 4);
		iPos = iPos + 4;
	}
	return iPos;
}

// 设置正弦输出
void CADCSet::OnSetSine(void)
{
	int iPos = 37;
	byte cSetSine[4] = {0xb2, 0x00, 0x80, 0x00};
	iPos = ADCCommand_18(iPos,cSetSine,4);
	m_cFrameData[iPos] = 0x00;
}

// 设置停止采集
void CADCSet::OnStopSample(void)
{
	int iPos = 37;
	byte cStopSample[4]={0x81, 0x11, 0x00, 0x00};
	iPos = ADCCommand_18(iPos, cStopSample, 4);
	m_cFrameData[iPos] = 0x00;
}

// 打开电源
void CADCSet::OnOpenPower(void)
{
	int iPos = 37;
	byte cOpenPower[4]={0xa3, 0x00, 0x30, 0x00};
	iPos = ADCCommand_18(iPos, cOpenPower, 4);
	m_cFrameData[iPos] = 0x00;
}

// 打开TB电源
void CADCSet::OnOpenPowerTB(void)
{
	int iPos = 37;
	byte cOpenPowerTB[4]={0xa3, 0x00, 0x30, 0x40};
	iPos = ADCCommand_18(iPos, cOpenPowerTB, 4);
	m_cFrameData[iPos] = 0x00;
}

// 打开开关
void CADCSet::OnOpenSwitch(void)
{
	int iPos = 37;
	byte cOpenSwith[4]={0xa3, 0x00, 0xf0, 0x00};
	iPos = ADCCommand_18(iPos, cOpenSwith, 4);
	m_cFrameData[iPos] = 0x00;
}

// 打开TB开关
void CADCSet::OnOpenSwitchTB(void)
{
	int iPos = 37;
	byte cOpenSwitchTB[4]={0xa3, 0x00, 0xf0, 0x40};
	iPos = ADCCommand_18(iPos, cOpenSwitchTB, 4);
	m_cFrameData[iPos] = 0x00;
}

// 读ADC寄存器
void CADCSet::OnADCRegisterRead(void)
{
	int iPos = 37;
	byte cADCRegisterRead[4]={0x82, 0x20, 0x0a, 0x00};
	iPos = ADCCommand_18(iPos, cADCRegisterRead, 4);
	m_cFrameData[iPos] = 0x00;
}

// 写ADC寄存器
void CADCSet::OnADCRegisterWrite(void)
{
	int iPos = 37;
	byte cADCRegisterWrite[16]={0x8d, 0x40, 0x0a, 0x00, 0x52, 0x08, 0x32, 0x03, 0x6f, 0x0c, 0xff, 0x7d, 0x52, 0x40, 0x00, 0x00};
	iPos = ADCCommand_18(iPos, cADCRegisterWrite, 16);
	m_cFrameData[iPos] = 0x00;
}

// ADC设置参数返回
void CADCSet::OnADCSetReturn(void)
{
	int iPos = 37;

	//自动AD返回地址（主机127.0.0.1）
	unsigned int IP= 0x0100007f;
	m_cFrameData[iPos]= 0x07;
	iPos++;
	memmove(m_cFrameData + iPos, &IP, 4);
	iPos = iPos + 4;

	//ad返回端口固定 80035005 ad返回端口递增
	unsigned int Port= 0x00038300;
	m_cFrameData[iPos]= 0x08;
	iPos++;
	memmove(m_cFrameData + iPos, &Port, 4);
	iPos = iPos + 4;

	//端口递增上下限命令
	unsigned int PortMove = 0x83008300;
	m_cFrameData[iPos]= 0x09;
	iPos++;
	memmove(m_cFrameData + iPos, &PortMove, 4);
	iPos = iPos + 4;

	//0-结束发送数据
	m_cFrameData[iPos]= 0;
}

// ADC连续读取数据
void CADCSet::OnADCReadContinuous(void)
{
	int iPos = 37;
	byte cADCRegisterWrite[4]={0x81, 0x10, 0x00, 0x00};
	iPos = ADCCommand_18(iPos, cADCRegisterWrite, 4);
	m_cFrameData[iPos] = 0x00;
}

void CADCSet::OnSetTB(unsigned int tnow)
{
	int iPos = 37;
	unsigned int tbh;
	unsigned int tbl;
	unsigned int uitmp = 0x00000000;
	
	tbh= tnow + 0x5000;
	tbl= 250;
	
	//写TB时刻高位
	m_cFrameData[iPos] = 0x0c;
	iPos++;
	memmove(m_cFrameData + iPos, &tbh, 4);
	iPos = iPos + 4;
	//写TB时刻低位
	m_cFrameData[iPos] = 0x0d;
	iPos++;
	memmove(m_cFrameData + iPos, &tbl, 2);
	iPos=iPos+2;

	// 0-结束发送数据
	memmove(m_cFrameData + iPos, &uitmp, 4);
}
