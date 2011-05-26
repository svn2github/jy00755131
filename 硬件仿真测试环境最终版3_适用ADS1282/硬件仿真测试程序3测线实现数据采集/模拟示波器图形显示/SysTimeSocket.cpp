// SysTimeSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "模拟示波器图形显示.h"
#include "SysTimeSocket.h"
#include "Parameter.h"

// CSysTimeSocket

CSysTimeSocket::CSysTimeSocket()
: m_uiSysTime(0)
, m_uiIPAim(0)
{
}

CSysTimeSocket::~CSysTimeSocket()
{
}


// CSysTimeSocket 成员函数
// CMySocket2 成员函数
void CSysTimeSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	int ret=0;

	ret=Receive(udp_buf,1024);

	if(ret==ERROR)
	{
		TRACE("ERROR!");
	}
	else if(ret != 0xffffffff) 
	{
		byte	usCommand = 0;
		int iPos = 28;
// 		// 仪器SN号
// 		memcpy(&usCommand, &udp_buf[iPos], 1);
// 		iPos++;
// 		if (usCommand != 0x04)
// 		{
// 			return;
// 		}
// 		memcpy(&m_uiSysTime, &udp_buf[iPos], 4);
// 		for (int i=0; i<GraphViewNum; i++)
// 		{
// 			if ((pHardwareTestDlg->m_uiSampleNb & (0x01<<i)) != 0)
// 			{
// 				m_uiIPAim	= 111 - 10*i;
// 				MakeFrameData(m_uiIPAim, m_uiSysTime);
// 				SendFrameData();
// 			}
// 		}
// 		MakeFrameData(101, m_uiSysTime);
//  		SendFrameData();

	}
	CAsyncSocket::OnReceive(nErrorCode);
}

// 生成数据采样设置帧
void CSysTimeSocket::MakeFrameData(unsigned int uiIPAim)
{
	MakeHeadFrame(uiIPAim, 1);


	int iPos = 32;

	//自动AD返回地址（主机127.0.0.1）
	unsigned int IP= 0x0100007f;
	ADCSampleCmd[iPos]= 0x07;
	iPos++;
	memmove(ADCSampleCmd + iPos, &IP, 4);
	iPos = iPos + 4;

	//ad返回端口固定 80035005 ad返回端口递增
	unsigned int Port= 0x00038300;
	ADCSampleCmd[iPos]= 0x08;
	iPos++;
	memmove(ADCSampleCmd + iPos, &Port, 4);
	iPos = iPos + 4;

	//端口递增上下限命令
	unsigned int PortMove = 0x83008300;
	ADCSampleCmd[iPos]= 0x09;
	iPos++;
	memmove(ADCSampleCmd + iPos, &PortMove, 4);
	iPos = iPos + 4;
	
	//0-结束发送数据
	ADCSampleCmd[iPos]= 0;
}

// 发送数据采样设置帧
void CSysTimeSocket::SendFrameData(void)
{
	SendTo(ADCSampleCmd, 128, SendPort, IPBroadcastAddr);
}
// ADC初始化
void CSysTimeSocket::OnADCInit(unsigned int uiTestNb)
{
	// 只对第一个采集站
	// 设置正弦函数输出频率
	unsigned char buf0[5] = {0x02, 0x05, 0x00, 0x00, 0x80};
	// 停止ADC连续读取数据
	unsigned char buf1[4] = {0x00, 0x04, 0x00, 0x11};
	// 模拟开关打开P_ADC_ON和P_DAC_ON
	unsigned char buf2[5] = {0x03, 0x05, 0x00, 0x00, 0x30};
	//模拟开关打开P_ADC_ON、P_DAC_ON 、 ADC_RESET和ADC_PWR
	unsigned char buf3[5] = {0x03, 0x05, 0x00, 0x00, 0xf0};
	// 从0x00开始写ADC的11个寄存器
	unsigned char buf4[16] = {0x00,0x10,0x00,0x40,0x0A,0x00,0x52,0x08,0x32,0x03,0x6F,0x0C,0xFF,0x7D,0x52,0x40};
	// 从0x00开始读ADC的11个寄存器
	unsigned char buf5[4] ={0, 0, 0, 0};
	MakeHeadFrame(111, 1);

	// 设置正弦函数输出频率
	MakeADCSetFrame(buf0, 5);
	SendFrameData();

	// 停止ADC连续读取数据
	MakeADCSetFrame(buf1, 4);
	SendFrameData();

	// 模拟开关打开P_ADC_ON和P_DAC_ON
	MakeADCSetFrame(buf2, 5);
	SendFrameData();

	Sleep(100);

	//模拟开关打开P_ADC_ON、P_DAC_ON 、 ADC_RESET和ADC_PWR
	MakeADCSetFrame(buf3, 5);
	SendFrameData();

	Sleep(100);
	
	// 停止ADC连续读取数据
	MakeADCSetFrame(buf1, 4);
	SendFrameData();

	Sleep(100);

	// 从0x00开始写ADC的11个寄存器
	MakeADCSetFrame(buf4, 16);
	SendFrameData();

	Sleep(100);

	// 从0x00开始读ADC的11个寄存器
	MakeHeadFrame(111, 2);
	MakeADCSetFrame(buf5, 4);
	SendFrameData();

	// 设置ADC采集数据返回端口和IP地址
	MakeFrameData(111);
	SendFrameData();
}

// ADC零漂矫正
void CSysTimeSocket::OnADCOffset(unsigned int uiTestNb)
{
	// 打开模拟开关SW_ADC+A0、SW_ADC+EN、SW_ADC-EN、P_ADC_ON、P_DAC_ON、ADC_RESET、ADC_PWR
	unsigned char buf0[5] = {0x03, 0x05, 0x00, 0x05, 0xF8};
	// 停止ADC连续读取数据
	unsigned char buf1[4] = {0x00, 0x04, 0x00, 0x11};
	// 从0x00开始写ADC的11个寄存器
	unsigned char buf2[16] = {0x00, 0x10, 0x00, 0x40, 0x0A, 0x00, 0x52, 0x18, 0x32, 0x03, 0x6F, 0x0C, 0xFF, 0x7D, 0x52, 0x40};
	// 从0x00开始读ADC的11个寄存器
	unsigned char buf3[4] = {0, 0, 0, 0};
	// 同步ADC转换
	unsigned char buf4[4] = {0x00, 0x04, 0x00, 0x04};
	// 开始ADC连续读取数据
	unsigned char buf5[4] = {0x00, 0x04, 0x00, 0x10};
	//ADC 零漂矫正
	unsigned char buf6[4] = {0x00, 0x04, 0x00, 0x60};

	MakeHeadFrame(111, 1);

	// 打开模拟开关SW_ADC+A0、SW_ADC+EN、SW_ADC-EN、P_ADC_ON、P_DAC_ON、ADC_RESET、ADC_PWR
	MakeADCSetFrame(buf0, 5);
	SendFrameData();

	// 停止ADC连续读取数据
	MakeADCSetFrame(buf1, 4);
	SendFrameData();

	Sleep(100);

	// 停止ADC连续读取数据
	MakeADCSetFrame(buf1, 4);
	SendFrameData();

	Sleep(100);

	// 从0x00开始写ADC的11个寄存器
	MakeADCSetFrame(buf2, 16);
	SendFrameData();

	// 从0x00开始读ADC的11个寄存器
	MakeHeadFrame(111, 2);
	MakeADCSetFrame(buf3, 4);
	SendFrameData();

	// 停止ADC连续读取数据
	MakeHeadFrame(111, 1);
	MakeADCSetFrame(buf1, 4);
	SendFrameData();

	Sleep(100);

	// 同步ADC转换
	MakeADCSetFrame(buf4, 4);
	SendFrameData();

	Sleep(100);

	// 开始ADC连续读取数据
	MakeADCSetFrame(buf5, 4);
	SendFrameData();

	Sleep(500);

	// 停止ADC连续读取数据
	MakeADCSetFrame(buf1, 4);
	SendFrameData();

	Sleep(100);

	// 停止ADC连续读取数据
	MakeADCSetFrame(buf1, 4);
	SendFrameData();

	Sleep(100);

	//ADC 零漂矫正
	MakeADCSetFrame(buf6, 4);
	SendFrameData();

	Sleep(100);

	// 开始ADC连续读取数据
	MakeADCSetFrame(buf5, 4);
	SendFrameData();

	Sleep(500);

	// 停止ADC连续读取数据
	MakeADCSetFrame(buf1, 4);
	SendFrameData();

	Sleep(100);

	// 停止ADC连续读取数据
	MakeADCSetFrame(buf1, 4);
	SendFrameData();

	Sleep(100);

	// 从0x00开始读ADC的11个寄存器
	MakeHeadFrame(111, 2);
	MakeADCSetFrame(buf3, 4);
	SendFrameData();
}

// ADC满量程矫正
void CSysTimeSocket::OnADCFullScale(unsigned int uiTestNb)
{
	// 打开模拟开关SW_ADC+A0、SW_ADC+EN、SW_ADC-A0、SW_ADC-EN、P_ADC_ON、P_DAC_ON、ADC_RESET、ADC_PWR
	unsigned char buf0[5] = {0x03, 0x05, 0x00, 0x0D, 0xF8};
	// 停止ADC连续读取数据
	unsigned char buf1[4] = {0x00, 0x04, 0x00, 0x11};
	// 从0x00开始写ADC的11个寄存器
	unsigned char buf2[16] = {0x00, 0x10, 0x00, 0x40, 0x0A, 0x00, 0x52, 0x18, 0x32, 0x03, 0x6F, 0x0C, 0xFF, 0x7D, 0x52, 0x40};
	// 从0x00开始读ADC的11个寄存器
	unsigned char buf3[4] = {0, 0, 0, 0};
	// 同步ADC转换
	unsigned char buf4[4] = {0x00, 0x04, 0x00, 0x04};
	// 开始ADC连续读取数据
	unsigned char buf5[4] = {0x00, 0x04, 0x00, 0x10};
	//ADC 零漂矫正
	unsigned char buf6[4] = {0x00, 0x04, 0x00, 0x61};

	MakeHeadFrame(111, 1);

	// 打开模拟开关SW_ADC+A0、SW_ADC+EN、SW_ADC-A0、SW_ADC-EN、P_ADC_ON、P_DAC_ON、ADC_RESET、ADC_PWR
	MakeADCSetFrame(buf0, 5);
	SendFrameData();

	// 停止ADC连续读取数据
	MakeADCSetFrame(buf1, 4);
	SendFrameData();

	Sleep(100);

	// 停止ADC连续读取数据
	MakeADCSetFrame(buf1, 4);
	SendFrameData();

	Sleep(100);

	// 从0x00开始写ADC的11个寄存器
	MakeADCSetFrame(buf2, 16);
	SendFrameData();

	// 从0x00开始读ADC的11个寄存器
	MakeHeadFrame(111, 2);
	MakeADCSetFrame(buf3, 4);
	SendFrameData();

	// 停止ADC连续读取数据
	MakeHeadFrame(111, 1);
	MakeADCSetFrame(buf1, 4);
	SendFrameData();

	Sleep(100);

	// 同步ADC转换
	MakeADCSetFrame(buf4, 4);
	SendFrameData();

	Sleep(100);

	// 开始ADC连续读取数据
	MakeADCSetFrame(buf5, 4);
	SendFrameData();

	Sleep(500);

	// 停止ADC连续读取数据
	MakeADCSetFrame(buf1, 4);
	SendFrameData();

	Sleep(100);

	// 停止ADC连续读取数据
	MakeADCSetFrame(buf1, 4);
	SendFrameData();

	Sleep(100);

	// ADC增益矫正
	MakeADCSetFrame(buf6, 4);
	SendFrameData();

	Sleep(100);

	// 开始ADC连续读取数据
	MakeADCSetFrame(buf5, 4);
	SendFrameData();

	Sleep(500);

	// 停止ADC连续读取数据
	MakeADCSetFrame(buf1, 4);
	SendFrameData();

	Sleep(100);

	// 停止ADC连续读取数据
	MakeADCSetFrame(buf1, 4);
	SendFrameData();

	Sleep(100);

	// 从0x00开始读ADC的11个寄存器
	MakeHeadFrame(111, 2);
	MakeADCSetFrame(buf3, 4);
	SendFrameData();
}

// 开始ADC数据采集
void CSysTimeSocket::OnADCStart(unsigned int uiTestNb)
{
	// 停止ADC连续读取数据
	unsigned char buf0[4] = {0x00, 0x04, 0x00, 0x11};
	// 从0x00开始写ADC的11个寄存器
	unsigned char buf1[16] = {0x00, 0x10, 0x00, 0x40, 0x0A, 0x00, 0x52, 0x08, 0x32, 0x03, 0x6F, 0x0C, 0xFF, 0x7D, 0x52, 0x40};
	// 开始ADC连续读取数据
	unsigned char buf2[4] = {0x00, 0x04, 0x00, 0x10};

	MakeHeadFrame(111, 1);

	// 停止ADC连续读取数据
	MakeADCSetFrame(buf0, 4);
	SendFrameData();

	Sleep(100);

	// 从0x00开始写ADC的11个寄存器
	MakeADCSetFrame(buf1, 16);
	SendFrameData();

	Sleep(100);

	// 开始ADC连续读取数据
	MakeADCSetFrame(buf2, 4);
	SendFrameData();

}

// 停止ADC数据采集
void CSysTimeSocket::OnADCStop(unsigned int uiTestNb)
{
	// 停止ADC连续读取数据
	unsigned char buf0[4] = {0x00, 0x04, 0x00, 0x11};

	MakeHeadFrame(111, 1);

	// 停止ADC连续读取数据
	MakeADCSetFrame(buf0, 4);
	SendFrameData();

	Sleep(100);

	// 停止ADC连续读取数据
	MakeADCSetFrame(buf0, 4);
	SendFrameData();

	Sleep(100);

	// 停止ADC连续读取数据
	MakeADCSetFrame(buf0, 4);
	SendFrameData();
}

// 生成包头
void CSysTimeSocket::MakeHeadFrame(unsigned int uiIPAim, unsigned short usCommand)
{
	unsigned int uiIPSource =	0;
	unsigned int usPortAim	=	0;

	ADCSampleCmd[0] = 0x11;
	ADCSampleCmd[1] = 0x22;
	ADCSampleCmd[2] = 0x33;
	ADCSampleCmd[3] = 0x44;
	ADCSampleCmd[4] = 0x00;
	ADCSampleCmd[5] = 0x00;
	ADCSampleCmd[6] = 0x00;
	ADCSampleCmd[7] = 0x00;
	ADCSampleCmd[8] = 0x00;
	ADCSampleCmd[9] = 0x00;
	ADCSampleCmd[10] = 0x00;
	ADCSampleCmd[11] = 0x00;
	ADCSampleCmd[12] = 0x00;
	ADCSampleCmd[13] = 0x00;
	ADCSampleCmd[14] = 0x00;
	ADCSampleCmd[15] = 0x00;

	uiIPSource	=	inet_addr(IPSource);
	usPortAim	=	ADSetReturnPort;
	// 源IP地址
	memcpy(&ADCSampleCmd[16], &uiIPSource, 4);
	// 目标IP地址
	memcpy(&ADCSampleCmd[20], &uiIPAim, 4);
	TRACE1("尾包时刻查询帧-仪器IP地址：%d\r\n", uiIPAim);
	// 目标端口号
	memcpy(&ADCSampleCmd[24], &usPortAim, 2);
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&ADCSampleCmd[26], &usCommand, 2);
}

// 生成ADC设置帧
void CSysTimeSocket::MakeADCSetFrame(unsigned char* buf, int iBufLength)
{
	int uiCmdNum = 0;
	int iPos = 32;
	int iNum = 0;
	uiCmdNum = iBufLength/4;
	if ((iBufLength%4) != 0)
	{
		uiCmdNum += 1;
	}
	for (int i=0; i<uiCmdNum; i++)
	{
		ADCSampleCmd[iPos] = 0x18;
		iPos++;
		iNum = i*4;
		ADCSampleCmd[iPos] = buf[iNum];
		iPos++;
		iNum = i*4+1;
		if (iBufLength > iNum)
		{
			ADCSampleCmd[iPos] = buf[iNum];
		}
		else
		{
			ADCSampleCmd[iPos] = 0x00;
		}
		iPos++;
		iNum = i*4+2;
		if (iBufLength > iNum)
		{
			ADCSampleCmd[iPos] = buf[iNum];
		}
		else
		{
			ADCSampleCmd[iPos] = 0x00;
		}
		iPos++;
		iNum = i*4+3;
		if (iBufLength > iNum)
		{
			ADCSampleCmd[iPos] = buf[iNum];
		}
		else
		{
			ADCSampleCmd[iPos] = 0x00;
		}
		iPos++;
	}
	//0-结束发送数据
	ADCSampleCmd[iPos]= 0;
}
