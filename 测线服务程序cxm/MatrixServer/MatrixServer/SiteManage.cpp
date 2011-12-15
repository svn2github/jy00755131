#include "StdAfx.h"
#include "SiteManage.h"
#include "..\\parameter\\Parameter.h"
#include <string>
using std::string;
using std::wstring;
// 宽字节字符串转换为多字节字符串
string WideCharToMultiChar(wstring str)
{
	string return_value;
	//获取缓冲区的大小，并申请空间，缓冲区大小是按字节计算的
	LPCWSTR lpWideCharStr = str.c_str();
	int cchWideChar = (int)str.size();
	int len = WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, cchWideChar, NULL, 0, NULL, NULL);
	char *buffer = new char[len+1];
	WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, cchWideChar, buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//删除缓冲区并返回值
	return_value.append(buffer);
	delete []buffer;
	return return_value;
}
const char* ConvertCStringToConstCharPointer(CString str)
{
	const char* pChar = NULL;
	char pach[2000];
	CStringW strw;
	wstring wstr;
	strw = str;
	wstr = strw;
	string mstring = WideCharToMultiChar(wstr);
	strcpy_s( pach, sizeof(pach), mstring.c_str());
	pChar = pach;
	return pChar;
}
CSiteManage::CSiteManage()
{
	/** 程序配置文件名，包含全部路径*/
	m_strAppIniXMLFile = _T("..\\parameter\\MatrixLineApp.xml");

// 	/** 配置文件名，包含全部路径*/
// 	m_strMatrixIniXMLFile = "..\\parameter\\Matrix.xml";

// 	/** FTP服务器IP地址*/
// 	m_strIPFTPServer = "192.168.0.11";

	// LCI设备IP地址
	m_strIPLCI = _T("192.168.0.11");
	//  LCI设备IP地址端口号
	m_uiPortLCI = 0x5010;

	// 和现场仪器通讯的本机IP地址
	m_strIPForInstrument = _T("192.168.0.11");
 	// 和现场仪器通讯的本机IP地址
 	m_uiIPForInstrument = inet_addr(ConvertCStringToConstCharPointer(m_strIPForInstrument));
	// 和内部网络通讯的本机IP地址
	m_strIPForInterface = _T("192.168.0.11");

	// 用于心跳帧发送的端口号
	m_uiPortForHeartBeat = 0x7000;	
	// 用于首包接收的端口号
	m_uiPortForHeadFrame = 0x8000;
	// 用于尾包接收的端口号
	m_uiPortForTailFrame = 0x9000;
	// 用于尾包转发的端口号
	m_uiPortForTailFrameSend = 0xC000;
	// 用于仪器IP地址设置的端口号
	m_uiPortForIPSet = 0x9100;
	// 用于仪器尾包时刻查询的端口号
	m_uiPortForTailTimeFrame = 0x9200;
	// 用于仪器时延设置的端口号
	m_uiPortForTimeSet = 0x9300;
	// 用于测试设置端口号
	m_uiPortForTestSetBase = 0x9400;
	// 用于测试数据接收的端口号
	m_uiPortForTestDataFrameBase = 0x9500;
	// 用于测试设置端口号，噪声监测
	m_uiPortForTestSetNoise = 0x9600;
	// 用于测试数据接收的端口号，噪声监测
	m_uiPortForTestDataFrameNoise = 0x9700;


	// 用于服务器内部网络命令接收的信宿端口号
	m_uiPortForCmdRev = 0x6000;
	// 用于服务器内部网络命令发送的信源端口号
	m_uiPortForCmdSend = 0x6200;
	// 用于客户端内部网络命令发送的信宿端口号，测线
	m_uiPortForCmdSendToLine = 0x6100;
	// 用于客户端内部网络命令发送的信宿端口号，施工
	m_uiPortForCmdSendToOperation = 0xC010;
	// 用于服务器内部网络数据发送的信源端口号，测试数据
	m_uiPortForDataSendTest = 0x6400;
	// 用于服务器内部网络数据发送的信源端口号，噪声监测
	m_uiPortForDataSendMonitor = 0x6600;
	// 用于客户端内部网络数据发送的信宿端口号
	m_uiPortForDataSendTo = 0x6300;

	// 仪器队列仪器总数
	m_uiInstrumentCountAll = 20000;
// 	// IP地址队列临时IP地址总数
// 	m_uiIPTempCountAll = 2000;

// 	// 测线数组总数
// 	m_uiLineCountAll = 50;
// 	// 测点数组总数
// 	m_uiPointCountAll = 20000;
// 	// 测道数组总数
// 	m_uiChannelCountAll = 20000;
// 	// 测试单元数组总数
// 	m_uiTestElementCountAll = 20000;
}

CSiteManage::~CSiteManage()
{
}
/**
* 初始化现场管理
* @param void
* @return bool	true：成功；false：失败
*/
bool CSiteManage::OnInit()
{
	//加载测线服务器程序设置数据
	LoadLineServerAppSetupData();

// 	// 建立测线设置数据对象	
// 	m_oLogicData.m_strIniFile = m_strMatrixIniXMLFile;	// 配置文件名，包含全部路径
// 	m_oLogicData.m_uiLineCountAll = m_uiLineCountAll;	// 测线数组总数	
// 	m_oLogicData.m_uiPointCountAll = m_uiPointCountAll;	// 测点数组总数	
// 	m_oLogicData.m_uiChannelCountAll = m_uiChannelCountAll;	// 测道数组总数
// 	m_oLogicData.m_uiTestElementCountAll = m_uiTestElementCountAll;	// 测试单元数组总数
// 	m_oLogicData.m_strIPFTPServer = m_strIPFTPServer;	// FTP服务器IP地址
// 	m_oLogicData.OnInit();
 
	// 建立现场数据对象
	m_oSiteData.m_uiInstrumentCountAll = m_uiInstrumentCountAll;	// 仪器队列仪器总数
// 	m_oSiteData.m_uiIPTempCountAll = m_uiIPTempCountAll;	// IP地址队列临时IP地址总数
// 	m_oSiteData.m_pLogicData = &m_oLogicData;	// 测线设置数据对象
// 	m_oSiteData.m_strIPFTPServer = m_strIPFTPServer;	// FTP服务器IP地址
 	m_oSiteData.OnInit();

	// 建立心跳帧发送网络对象
	m_oSocketHeartBeatFrame.OnInit(m_strIPForInstrument, m_uiPortForHeartBeat, m_strIPLCI, m_uiPortLCI);
 
	// 建立首包接收网络对象
	m_oSocketHeadFrame.OnInit(m_strIPForInstrument, m_uiPortForHeadFrame);
	// 初始化网络端口接收缓冲区大小
	m_oSocketHeadFrame.SetBufferSize(m_uiInstrumentCountAll);

	// 建立尾包接收网络对象
	m_oSocketTailFrame.OnInit(m_strIPForInstrument, m_uiPortForTailFrame, m_uiPortForTailFrameSend);
	// 初始化网络端口接收缓冲区大小
	m_oSocketTailFrame.SetBufferSize(m_uiInstrumentCountAll);

	// 建立仪器IP地址设置网络对象
	m_oSocketIPSetFrame.OnInit(m_strIPForInstrument, m_uiIPForInstrument, m_uiPortForIPSet, m_strIPLCI, m_uiPortLCI);
	// 初始化网络端口接收缓冲区大小
	m_oSocketIPSetFrame.SetBufferSize(m_uiInstrumentCountAll);
// 
// 	// 建立仪器时延设置网络对象
// 	m_oSocketTimeSetFrame.OnInit(m_strIPForInstrument, m_uiIPForInstrument, m_uiPortForTimeSet, m_strIPLCI, m_uiPortLCI);
// 	// 初始化网络端口接收缓冲区大小
// 	m_oSocketTimeSetFrame.SetBufferSize(20000);
// 
// 	// 建立仪器尾包时刻查询网络对象
// 	m_oSocketTailTimeFrame.OnInit(m_strIPForInstrument, m_uiIPForInstrument, m_uiPortForTailTimeFrame, m_strIPLCI, m_uiPortLCI);
// 	// 初始化网络端口接收缓冲区大小
// 	m_oSocketTailTimeFrame.SetBufferSize(20000);
// 
// 	// 建立仪器测试设置网络对象，基本测试 PortForTestSetBase 37888 0x9400
// 	m_oSocketTestSetFrameBase.OnInit(m_strIPForInstrument, m_uiIPForInstrument, m_uiPortForTestSetBase, m_strIPLCI, m_uiPortLCI);
// 	// 初始化网络端口接收缓冲区大小
// 	//m_oSocketTestSetFrameBase.SetBufferSize(20000);
// 	m_oSocketTestSetFrameBase.m_oRunTimeDataList=&m_oSiteData.m_oRunTimeDataList;
// 
// 	// 建立仪器测试数据接收网络对象，基本测试
// 	m_oSocketTestDataFrameBase.OnInit(m_strIPForInstrument, m_uiPortForTestDataFrameBase);
// 	// 初始化网络端口接收缓冲区大小
// 	m_oSocketTestDataFrameBase.SetBufferSize(20000);
// 	m_oSocketTestDataFrameBase.m_oProcTestThreadID=m_oSiteData.m_oProcTestThreadID;
// 	m_oSocketTestDataFrameBase.OnReceive_buf=m_oSiteData.OnReceive_buf;
// 
// 	// 建立仪器测试设置网络对象，噪声监测 PortForTestSetNoise  0x9401 37889
// 	m_oSocketTestSetFrameNoise.OnInit(m_strIPForInstrument, m_uiIPForInstrument, m_uiPortForTestSetNoise, m_strIPLCI, m_uiPortLCI);
// 	// 初始化网络端口接收缓冲区大小
// 	//m_oSocketTestSetFrameNoise.SetBufferSize(20000);
// 	m_oSocketTestSetFrameNoise.m_oRunTimeDataList=&m_oSiteData.m_oRunTimeDataList;
// 
// 	// 建立仪器测试数据接收网络对象，噪声监测
// 	m_oSocketTestDataFrameNoise.OnInit(m_strIPForInstrument, m_uiPortForTestDataFrameNoise);
// 	// 初始化网络端口接收缓冲区大小
// 	m_oSocketTestDataFrameNoise.SetBufferSize(20000);
// 	m_oSocketTestDataFrameNoise.m_oProcTestThreadID=m_oSiteData.m_oProcTestThreadID;
// 	m_oSocketTestDataFrameNoise.OnReceive_buf=m_oSiteData.OnReceive_buf;
// 
// 	// 建立服务器内部网络命令发送处理网络对象
// 	m_oSocketInterfaceCmdSend.OnInit(m_strIPForInterface, m_uiPortForCmdSend, m_uiPortForCmdSendToLine, m_uiPortForCmdSendToOperation);	
// 	m_oSocketInterfaceCmdSend.SetBufferSize(65536 * 10);	// 初始化网络端口接收缓冲区大小	
// 
// 	// 建立服务器内部网络数据发送处理网络对象，测试数据
// 	m_oSocketInterfaceDataSendTest.OnInit(m_strIPForInterface, m_uiPortForDataSendTest, m_uiPortForDataSendTo);	
// 	m_oSocketInterfaceDataSendTest.SetBufferSize(65536 * 10);	// 初始化网络端口接收缓冲区大小
// 
// 	// 建立服务器内部网络数据发送处理网络对象，噪声监测
// 	m_oSocketInterfaceDataSendMonitor.OnInit(m_strIPForInterface, m_uiPortForDataSendMonitor, m_uiPortForDataSendTo);	
// 	m_oSocketInterfaceDataSendMonitor.SetBufferSize(65536 * 10);	// 初始化网络端口接收缓冲区大小	
// 
	// 建立心跳处理对象
	m_oThreadProcHeartBeat.m_pSiteData = &m_oSiteData;	// 现场数据对象
	m_oThreadProcHeartBeat.m_pSocketHeartBeatFrame = &m_oSocketHeartBeatFrame;	// 心跳帧发送网络对象
	m_oThreadProcHeartBeat.OnInit();	// 初始化
	m_oThreadProcHeartBeat.CreateThread();	// 生成处理线程

	// 建立仪器IP地址设置对象
	m_oThreadProcIPSet.m_pSiteData = &m_oSiteData;	// 现场数据对象
	m_oThreadProcIPSet.m_pSocketIPSetFrame = &m_oSocketIPSetFrame;	// 仪器IP地址设置网络对象
	m_oThreadProcIPSet.OnInit();	// 初始化
	m_oThreadProcIPSet.CreateThread();	// 生成处理线程

	// 建立首包处理对象
//	m_oThreadProcHeadFrame.m_pLogicData = &m_oLogicData;	// 测线设置数据对象
	m_oThreadProcHeadFrame.m_pSiteData = &m_oSiteData;	// 现场数据对象
	m_oThreadProcHeadFrame.m_pSocketHeadFrame = &m_oSocketHeadFrame;	// 首包接收网络对象
//	m_oThreadProcHeadFrame.m_pThreadProcIPSet = &m_oThreadProcIPSet;	// 仪器IP地址设置对象	
	m_oThreadProcHeadFrame.OnInit();	// 初始化
	m_oThreadProcHeadFrame.CreateThread();	// 生成处理线程
// 
// 	// 建立时延处理线程对象
// 	m_oThreadProcTimeDelay.m_pSiteData = &m_oSiteData;	// 现场数据对象
// 	m_oThreadProcTimeDelay.m_pSocketTailTimeFrame = &m_oSocketTailTimeFrame;	// 仪器尾包时刻查询网络对象
// 	m_oThreadProcTimeDelay.m_pSocketTimeSetFrame = &m_oSocketTimeSetFrame;	// 仪器尾包时刻查询网络对象
// 	//m_oThreadProcTimeDelay.m_pThreadProcDelete = &m_oThreadProcDelete;	// 仪器删除对象
// 	m_oThreadProcTimeDelay.OnInit();	// 初始化
// 	m_oThreadProcTimeDelay.CreateThread(CREATE_SUSPENDED);	// 生成处理线程
// 
	// 建立尾包处理对象
	m_oThreadProcTailFrame.m_pSiteData = &m_oSiteData;	// 现场数据对象
	m_oThreadProcTailFrame.m_pSocketTailFrame = &m_oSocketTailFrame;	// 首包接收网络对象
// 	m_oThreadProcTailFrame.m_pThreadProcTimeDelay = &m_oThreadProcTimeDelay;	// 时延处理线程对象指针
// 	m_oThreadProcTailFrame.m_pThreadProcDelete = &m_oThreadProcDelete;	// 仪器删除对象
	m_oThreadProcTailFrame.OnInit();	// 初始化
	m_oThreadProcTailFrame.CreateThread();	// 生成处理线程
// 
// 	// 建立仪器IP地址分配对象
// 	m_oThreadProcIPDistribute.m_pLogicData = &m_oLogicData;	// 测线设置数据对象
// 	m_oThreadProcIPDistribute.m_pSiteData = &m_oSiteData;	// 现场数据对象
// 	m_oThreadProcIPDistribute.m_pThreadProcIPSet = &m_oThreadProcIPSet;	// 仪器IP地址设置对象
// 	m_oThreadProcIPDistribute.OnInit();	// 初始化
// 	m_oThreadProcIPDistribute.CreateThread();	// 生成处理线程
// 
// 	// 建立路由监视对象
// 	m_oThreadProcMonitorRout.m_pSiteData = &m_oSiteData;	// 现场数据对象
// 	m_oThreadProcMonitorRout.m_pThreadProcDelete = &m_oThreadProcDelete;	// 仪器删除对象
// 	m_oThreadProcMonitorRout.OnInit();	// 初始化
// 	m_oThreadProcMonitorRout.CreateThread();	// 生成处理线程
// 
// 	// 建立仪器删除对象
// 	m_oThreadProcDelete.m_pSiteData = &m_oSiteData;	// 现场数据对象
// 	m_oThreadProcDelete.m_pThreadProcTimeDelay = &m_oThreadProcTimeDelay;	// 时延处理线程对象
// 	m_oThreadProcDelete.OnInit();	// 初始化
// 	m_oThreadProcDelete.CreateThread(CREATE_SUSPENDED);	// 生成处理线程
// 
// 	// 建立现场数据输出对象
// 	m_oThreadProcSiteDataOutput.m_pLogicData = &m_oLogicData;	// 测线设置数据对象
// 	m_oThreadProcSiteDataOutput.m_pSiteData = &m_oSiteData;	// 现场数据对象
// 	m_oThreadProcSiteDataOutput.m_pThreadProcDelete = &m_oThreadProcDelete;	// 仪器删除对象
// 	m_oThreadProcSiteDataOutput.m_pThreadProcTestBase = &m_oThreadProcTestBase;	// 测试对象，基本测试
// 	m_oThreadProcSiteDataOutput.m_pSocketInterfaceCmdSend = &m_oSocketInterfaceCmdSend;	// 服务器内部网络命令发送处理网络对象
// 	m_oThreadProcSiteDataOutput.OnInit();	// 初始化
// 	m_oThreadProcSiteDataOutput.CreateThread();	// 生成处理线程
// 
// 	// 建立测试对象，基本测试
// 	m_oThreadProcTestBase.m_pLogicData = &m_oLogicData;	// 测线设置数据对象
// 	m_oThreadProcTestBase.m_pSiteData = &m_oSiteData;	// 现场数据对象
// 	m_oThreadProcTestBase.m_pSocketTestSetFrame = &m_oSocketTestSetFrameBase;	// 仪器测试设置网络对象，基本测试
// 	m_oThreadProcTestBase.m_pSocketTestDataFrame = &m_oSocketTestDataFrameBase;	// 仪器测试数据接收网络对象，基本测试
// 	m_oThreadProcTestBase.m_pSocketInterfaceDataSend = &m_oSocketInterfaceDataSendTest;	// 服务器内部网络数据发送处理网络对象，测试数据
// 	m_oThreadProcTestBase.m_iProcTestMethod = 1;	// 处理方式 1-基本测试；2-噪声监测
// 	m_oThreadProcTestBase.OnInit();	// 初始化
// 	m_oThreadProcTestBase.CreateThread(CREATE_SUSPENDED);	// 生成处理线程
// 
// 	// 建立测试对象，噪声监测
// 	m_oThreadProcTestNoise.m_pLogicData = &m_oLogicData;	// 测线设置数据对象
// 	m_oThreadProcTestNoise.m_pSiteData = &m_oSiteData;	// 现场数据对象
// 	m_oThreadProcTestNoise.m_pSocketTestSetFrame = &m_oSocketTestSetFrameNoise;	// 仪器测试设置网络对象，噪声监测
// 	m_oThreadProcTestNoise.m_pSocketTestDataFrame = &m_oSocketTestDataFrameNoise;	// 仪器测试数据接收网络对象，噪声监测
// 	m_oThreadProcTestNoise.m_pSocketInterfaceDataSend = &m_oSocketInterfaceDataSendMonitor;	// 服务器内部网络数据发送处理网络对象，噪声监测
// 	m_oThreadProcTestNoise.m_iProcTestMethod = 2;	// 处理方式 1-基本测试；2-噪声监测
// 	m_oThreadProcTestNoise.OnInit();	// 初始化
// 	m_oThreadProcTestNoise.CreateThread(CREATE_SUSPENDED);	// 生成处理线程
// 
 	// 建立内部网络命令接收处理网络对象
// 	m_oNetProcInterface.m_pLogicData = &m_oLogicData;	// 测线设置数据对象
 	m_oNetProcInterface.m_pSiteData = &m_oSiteData;	// 现场数据对象
// 	m_oNetProcInterface.m_pSocketInterfaceCmdRev = &m_oSocketInterfaceCmdRev;	// 内部网络命令接收处理网络对象
// 	m_oNetProcInterface.m_pSocketInterfaceCmdSend = &m_oSocketInterfaceCmdSend;	// 服务器内部网络命令发送处理网络对象
 
 	m_oNetProcInterface.m_pThreadProcHeartBeat = &m_oThreadProcHeartBeat;	// 心跳处理对象指针	
 	m_oNetProcInterface.m_pThreadProcIPSet = &m_oThreadProcIPSet;	// 仪器IP地址设置对象指针	
 	m_oNetProcInterface.m_pThreadProcHeadFrame = &m_oThreadProcHeadFrame;	// 首包处理对象指针	
// 	m_oNetProcInterface.m_pThreadProcTimeDelay = &m_oThreadProcTimeDelay;	// 时延处理线程对象指针指针	
 	m_oNetProcInterface.m_pThreadProcTailFrame = &m_oThreadProcTailFrame;	// 尾包处理对象指针	
// 	m_oNetProcInterface.m_pThreadProcIPDistribute = &m_oThreadProcIPDistribute;	// 仪器IP地址分配对象指针	
// 	m_oNetProcInterface.m_pThreadProcMonitorRout = &m_oThreadProcMonitorRout;	// 路由监视对象指针	
// 	m_oNetProcInterface.m_pThreadProcDelete = &m_oThreadProcDelete;	// 仪器删除对象指针	
// 	m_oNetProcInterface.m_pThreadProcSiteDataOutput = &m_oThreadProcSiteDataOutput;	// 现场数据输出对象指针
// 	m_oNetProcInterface.m_pThreadProcDelete = &m_oThreadProcDelete;	// 仪器删除对象指针	
// 	m_oNetProcInterface.m_pThreadProcTestBase = &m_oThreadProcTestBase;	// 测试对象指针，基本测试
// 	m_oNetProcInterface.m_pThreadProcTestNoise = &m_oThreadProcTestNoise;	// // 测试对象指针，噪声监测
// 
// 	// 建立内部网络命令帧接收和处理对象
// 	m_oSocketInterfaceCmdRev.m_pNetProcInterface = &m_oNetProcInterface;	
// 	m_oSocketInterfaceCmdRev.OnInit(m_strIPForInterface, m_uiPortForCmdRev);	
// 	m_oSocketInterfaceCmdRev.SetBufferSize(65536 * 10);	// 初始化网络端口接收缓冲区大小
// 
// 	// 输出重置后的现场数据
// 	m_oThreadProcSiteDataOutput.OutputToFileForReset();
// 
// 	// 设置运行状态数据
// 	m_oSiteData.m_oRunTimeDataList.Set(1, "CSiteManage", "OnInit", "Click Init Button");

	return true;
}

/**
* 关闭现场管理
* @param void
* @return bool	true：成功；false：失败
*/
bool CSiteManage::OnClose()
{
	int count = 0;

	// 关闭心跳处理对象
	m_oThreadProcHeartBeat.OnClose();
	// 关闭仪器IP地址设置对象
	m_oThreadProcIPSet.OnClose();
	// 关闭首包处理对象	
	m_oThreadProcHeadFrame.OnClose();
// 	// 关闭时延处理线程对象
// 	m_oThreadProcTimeDelay.OnClose();
	// 关闭尾包处理对象
	m_oThreadProcTailFrame.OnClose();
// 	// 关闭仪器IP地址分配对象
// 	m_oThreadProcIPDistribute.OnClose();
// 	// 关闭路由监视对象
// 	m_oThreadProcMonitorRout.OnClose();
// 	// 关闭仪器删除对象
// 	m_oThreadProcDelete.OnClose();
// 	// 关闭现场数据输出对象
// 	m_oThreadProcSiteDataOutput.OnClose();
// 	// 关闭测试对象，基本测试
// 	m_oThreadProcTestBase.OnClose();
// 	// 关闭测试对象，噪声监测
// 	m_oThreadProcTestNoise.OnClose();
// 
	while(true)	// 等待线程关闭
	{
		if((true == m_oSiteData.m_bProcHeartBeatClose)	// 心跳处理线程关闭
 			&& (true == m_oSiteData.m_bProcHeadFrameClose)	// 首包处理线程关闭
 			&& (true == m_oSiteData.m_bProcTailFrameClose)	// 尾包处理线程关闭
// 			&& (true == m_oSiteData.m_bProcMonitorRoutClose)	// 路由监视线程关闭
// 			&& (true == m_oSiteData.m_bProcIPDistributeClose)	// 仪器IP地址分配线程关闭
 			&& (true == m_oSiteData.m_bProcIPSetClose)	// 仪器IP地址设置线程关闭
// 			&& (true == m_oSiteData.m_bProcSiteDataOutputClose)	// 现场数据输出线程线程关闭
// 			&& (true == m_oSiteData.m_bProcTimeDelayClose)	// 时延处理线程关闭
// 			&& (true == m_oSiteData.m_bProcDeleteClose)	// 仪器删除线程关闭
// 			&& (true == m_oSiteData.m_bProcTestBaseClose)	// 测试处理线程关闭，基本测试
// 			&& (true == m_oSiteData.m_bProcTestNoiseClose)	// 测试处理线程关闭，噪声监测
			)
		{
			break;
		}
		Sleep(OneSleepTime);	// 休眠，等待线程处理关闭
		if(count == CloseAllThreadSleepTimes){

// 			if(m_oThreadProcDelete.m_hThread){	
// 				::TerminateThread(m_oThreadProcDelete.m_hThread, 0x0); 
// 				m_oThreadProcDelete.m_hThread = NULL;
// 			}
// 
// 			if(m_oThreadProcTestBase.m_hThread){	
// 				::TerminateThread(m_oThreadProcTestBase.m_hThread, 0x0); 
// 				m_oThreadProcTestBase.m_hThread = NULL;
// 			}
// 
// 			if(m_oThreadProcSiteDataOutput.m_hThread){	
// 				::TerminateThread(m_oThreadProcSiteDataOutput.m_hThread, 0x0); 
// 				m_oThreadProcSiteDataOutput.m_hThread = NULL;
// 			}
// 
// 			if(m_oThreadProcTestNoise.m_hThread){	
// 				::TerminateThread(m_oThreadProcTestNoise.m_hThread, 0x0); 
// 				m_oThreadProcTestNoise.m_hThread = NULL;
// 			}
// 
// 			if(m_oThreadProcMonitorRout.m_hThread){	
// 				::TerminateThread(m_oThreadProcMonitorRout.m_hThread, 0x0); 
// 				m_oThreadProcMonitorRout.m_hThread = NULL;
// 			}
// 
// 			if(m_oThreadProcIPDistribute.m_hThread){	
// 				::TerminateThread(m_oThreadProcIPDistribute.m_hThread, 0x0); 
// 				m_oThreadProcIPDistribute.m_hThread = NULL;
// 			}
// 
			if(m_oThreadProcTailFrame.m_hThread){	
				::TerminateThread(m_oThreadProcTailFrame.m_hThread, 0x0); 
				m_oThreadProcTailFrame.m_hThread = NULL;
			}
// 
// 			if(m_oThreadProcTimeDelay.m_hThread){	
// 				::TerminateThread(m_oThreadProcTimeDelay.m_hThread, 0x0); 
// 				m_oThreadProcTimeDelay.m_hThread = NULL;
// 			}
			if(m_oThreadProcHeadFrame.m_hThread){	
				::TerminateThread(m_oThreadProcHeadFrame.m_hThread, 0); 
				m_oThreadProcHeadFrame.m_hThread = NULL;
			}

			if(m_oThreadProcIPSet.m_hThread){	
				::TerminateThread(m_oThreadProcIPSet.m_hThread, 0x0); 
				m_oThreadProcIPSet.m_hThread = NULL;
			}

			if(m_oThreadProcHeartBeat.m_hThread){	
				::TerminateThread(m_oThreadProcHeartBeat.m_hThread, 0); 
				m_oThreadProcHeartBeat.m_hThread = NULL;
			}

			break;
		}
		else{ ++count; }
	}

	// 关闭现场数据对象
	m_oSiteData.OnClose();
// 	// 关闭测线设置数据对象
// 	m_oLogicData.OnClose();

	return true;
}

/**
* 开始现场管理
* @param void
* @return bool	true：成功；false：失败
*/
bool CSiteManage::OnWork()
{
	m_oNetProcInterface.ProcInterFaceCommandForField(FieldOn);
	return true;
}

/**
* 停止现场管理
* @param void
* @return bool	true：成功；false：失败
*/
bool CSiteManage::OnStop()
{
	m_oNetProcInterface.ProcInterFaceCommandForField(FieldOff);
	return true;
}

/**
* 打开程序配置文件
* @param void
* @return void
*/
void CSiteManage::OpenAppIniXMLFile()
{
	// 初始化COM库
	CoInitialize(NULL);

	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;

	strOLEObject = _T("msxml2.domdocument");
	BOOL bData = m_oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);

	oVariant = m_strAppIniXMLFile;
	bData = m_oXMLDOMDocument.load(oVariant);
}

/**
* 关闭程序配置文件
* @param void
* @return void
*/
void CSiteManage::CloseAppIniXMLFile()
{
	m_oXMLDOMDocument.DetachDispatch();
	// 释放COM库
	CoUninitialize();
}

/**
* 加载测线服务器程序设置数据
* @param void
* @return void
*/
void CSiteManage::LoadLineServerAppSetupData()
{
	// 打开程序配置文件
	OpenAppIniXMLFile();

	//加载IP地址设置数据
	LoadIPSetupData();
	//加载端口设置数据
	LoadPortSetupData();
	//加载数据缓冲区大小数据
	LoadBufferSizeSetupData();

	// 关闭程序配置文件
	CloseAppIniXMLFile();
}

/**
* 加载IP地址设置数据
* @param void
* @return void
*/
void CSiteManage::LoadIPSetupData()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	// 找到IP地址设置区
	strKey = _T("LineServerAppIPSetup");
	lpDispatch = m_oXMLDOMDocument.getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);

// 	strKey = _T("IPFTPServer");
// 	m_strIPFTPServer = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);

	strKey = _T("IPForInstrument");
	m_strIPForInstrument = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
	// 和现场仪器通讯的本机IP地址
	m_uiIPForInstrument = inet_addr(ConvertCStringToConstCharPointer(m_strIPForInstrument));
	strKey = _T("IPForInterface");
	m_strIPForInterface = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
	strKey = _T("IPLCI");
	m_strIPLCI = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
}

/**
* 加载端口设置数据
* @param void
* @return void
*/
void CSiteManage::LoadPortSetupData()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	// 找到IP地址设置区
	strKey = _T("LineServerAppPortSetup");
	lpDispatch = m_oXMLDOMDocument.getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);

	strKey = _T("PortLCI");
	m_uiPortLCI = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = _T("PortForHeartBeat");
	m_uiPortForHeartBeat = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = _T("PortForHeadFrame");
	m_uiPortForHeadFrame = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = _T("PortForTailFrame");
	m_uiPortForTailFrame = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = _T("PortForTailFrameSend");
	m_uiPortForTailFrameSend = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = _T("PortForIPSet");
	m_uiPortForIPSet = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

	strKey = _T("PortForTailTimeFrame");
	m_uiPortForTailTimeFrame = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = _T("PortForTimeSet");
	m_uiPortForTimeSet = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = _T("PortForTestSetBase");
	m_uiPortForTestSetBase = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = _T("PortForTestDataFrameBase");
	m_uiPortForTestDataFrameBase = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = _T("PortForTestSetNoise");
	m_uiPortForTestSetNoise = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

	strKey = _T("PortForTestDataFrameNoise");
	m_uiPortForTestDataFrameNoise = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = _T("PortForCmdRev");
	m_uiPortForCmdRev = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = _T("PortForCmdSend");
	m_uiPortForCmdSend = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = _T("PortForCmdSendToLine");
	m_uiPortForCmdSendToLine = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = _T("PortForCmdSendToOperation");
	m_uiPortForCmdSendToOperation = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = _T("PortForDataSendTest");
	m_uiPortForDataSendTest = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

	strKey = _T("PortForDataSendMonitor");
	m_uiPortForDataSendMonitor = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = _T("PortForDataSendTo");
	m_uiPortForDataSendTo = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
}

/**
* 加载数据缓冲区大小数据
* @param void
* @return void
*/
void CSiteManage::LoadBufferSizeSetupData()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	// 找到数据缓冲区大小设置区
	strKey = _T("LineServerAppBufferSizeSetup");
	lpDispatch = m_oXMLDOMDocument.getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);

	strKey = _T("InstrumentCountAll");
	m_uiInstrumentCountAll = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
// 	strKey = _T("IPTempCountAll");
// 	m_uiIPTempCountAll = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
// 	strKey = "LineCountAll";
// 	m_uiLineCountAll = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
// 	strKey = "PointCountAll";
// 	m_uiPointCountAll = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
// 	strKey = "ChannelCountAll";
// 	m_uiChannelCountAll = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
// 	strKey = "TestElementCountAll";
// 	m_uiTestElementCountAll = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
}