#pragma once
#include "SiteData.h"
#include "NetProcInterface.h"
#include "SocketHeartBeatFrame.h"
#include "SocketHeadFrame.h"
#include "ThreadProcHeartBeat.h"
#include "ThreadProcHeadFrame.h"
class CSiteManage
{
public:
	CSiteManage(void);
	~CSiteManage(void);
public: //属性
	/** 程序配置文件名，包含全部路径*/
	CString m_strAppIniXMLFile;

// 	/** 配置文件名，包含全部路径*/
// 	CString m_strMatrixIniXMLFile;
//
	/** XMLDOM文件对象*/
	CXMLDOMDocument m_oXMLDOMDocument;

// 	/** FTP服务器IP地址*/
// 	CString m_strIPFTPServer;

	/** LCI设备IP地址*/
	CString m_strIPLCI;
	/** LCI设备IP地址端口号*/
	UINT m_uiPortLCI;

	/** 和现场仪器通讯的本机IP地址*/
	CString m_strIPForInstrument;
	/** 和现场仪器通讯的本机IP地址*/
	UINT m_uiIPForInstrument;
	/** 和内部网络通讯的本机IP地址*/
	CString m_strIPForInterface;

	/** 用于心跳帧发送的端口号*/
	UINT m_uiPortForHeartBeat;
	/** 用于首包接收的端口号*/
	UINT m_uiPortForHeadFrame;
	/** 用于尾包接收的端口号*/
	UINT m_uiPortForTailFrame;
	/** 用于尾包转发的端口号*/
	UINT m_uiPortForTailFrameSend;
	/** 用于仪器IP地址设置的端口号*/
	UINT m_uiPortForIPSet;
	/** 用于仪器尾包时刻查询的端口号*/
	UINT m_uiPortForTailTimeFrame;
	/** 用于仪器时延设置的端口号*/
	UINT m_uiPortForTimeSet;
	/** 用于测试设置端口号，基本测试*/
	UINT m_uiPortForTestSetBase;
	/** 用于测试数据接收的端口号，基本测试*/
	UINT m_uiPortForTestDataFrameBase;
	/** 用于测试设置端口号，噪声监测*/
	UINT m_uiPortForTestSetNoise;
	/** 用于测试数据接收的端口号，噪声监测*/
	UINT m_uiPortForTestDataFrameNoise;

	/** 用于服务器内部网络命令接收的信宿端口号*/
	UINT m_uiPortForCmdRev;
	/** 用于服务器内部网络命令发送的信源端口号*/
	UINT m_uiPortForCmdSend;
	/** 用于客户端内部网络命令发送的信宿端口号，测线*/
	UINT m_uiPortForCmdSendToLine;
	/** 用于客户端内部网络命令发送的信宿端口号，施工*/
	UINT m_uiPortForCmdSendToOperation;
	/** 用于服务器内部网络数据发送的信源端口号，测试数据*/
	UINT m_uiPortForDataSendTest;
	/** 用于服务器内部网络数据发送的信源端口号，噪声监测*/
	UINT m_uiPortForDataSendMonitor;
	/** 用于客户端内部网络数据发送的信宿端口号*/
	UINT m_uiPortForDataSendTo;

	/** 仪器队列仪器总数*/
	unsigned int m_uiInstrumentCountAll;
// 	/** IP地址队列临时IP地址总数*/
// 	unsigned int m_uiIPTempCountAll;

// 	/** 测线数组总数*/
// 	unsigned int m_uiLineCountAll;
// 	/** 测点数组总数*/
// 	unsigned int m_uiPointCountAll;
// 	/** 测道数组总数*/
// 	unsigned int m_uiChannelCountAll;
// 	/** 测试单元数组总数*/
// 	unsigned int m_uiTestElementCountAll;

// 	/** 测线设置数据对象*/
// 	CLogicData m_oLogicData;
	/** 现场数据对象*/
	CSiteData m_oSiteData;

	/** 心跳帧发送网络对象*/
	CSocketHeartBeatFrame m_oSocketHeartBeatFrame;
	/** 首包接收网络对象*/
	CSocketHeadFrame m_oSocketHeadFrame;
// 	/** 尾包接收网络对象*/
// 	CSocketTailFrame m_oSocketTailFrame;
// 	/** 仪器IP地址设置网络对象*/
// 	CSocketIPSetFrame m_oSocketIPSetFrame;
// 	/** 仪器尾包时刻查询网络对象*/
// 	CSocketTailTimeFrame m_oSocketTailTimeFrame;
// 	/** 仪器时延设置网络对象*/
// 	CSocketTimeSetFrame m_oSocketTimeSetFrame;
// 
// 	/** 仪器测试设置网络对象，基本测试*/
// 	CSocketTestSetFrame m_oSocketTestSetFrameBase;
// 	/** 仪器测试数据接收网络对象，基本测试*/
// 	CSocketTestDataFrame m_oSocketTestDataFrameBase;
// 	/** 仪器测试设置网络对象，噪声监测*/
// 	CSocketTestSetFrame m_oSocketTestSetFrameNoise;
// 	/** 仪器测试数据接收网络对象，噪声监测*/
// 	CSocketTestDataFrame m_oSocketTestDataFrameNoise;
// 
// 	/** 服务器内部网络命令接收处理网络对象*/
// 	CSocketInterfaceCmdRev m_oSocketInterfaceCmdRev;
// 	/** 服务器内部网络命令发送处理网络对象*/
// 	CSocketInterfaceCmdSend m_oSocketInterfaceCmdSend;
// 	/** 服务器内部网络数据发送处理网络对象，测试数据*/
// 	CSocketInterfaceDataSend m_oSocketInterfaceDataSendTest;
// 	/** 服务器内部网络数据发送处理网络对象，噪声监测*/
// 	CSocketInterfaceDataSend m_oSocketInterfaceDataSendMonitor;

	/** 心跳处理对象*/
	CThreadProcHeartBeat m_oThreadProcHeartBeat;
// 	/** 仪器IP地址设置对象*/
// 	CThreadProcIPSet m_oThreadProcIPSet;
	/** 首包处理对象*/
	CThreadProcHeadFrame m_oThreadProcHeadFrame;
// 	/** 时延处理线程对象*/
// 	CThreadProcTimeDelay m_oThreadProcTimeDelay;
// 	/** 尾包处理对象*/
// 	CThreadProcTailFrame m_oThreadProcTailFrame;
// 	/** 仪器IP地址分配对象*/
// 	CThreadProcIPDistribute m_oThreadProcIPDistribute;
// 	/** 路由监视对象*/
// 	CThreadProcMonitorRout m_oThreadProcMonitorRout;
// 	/** 仪器删除对象*/
// 	CThreadProcDelete m_oThreadProcDelete;
// 	/** 现场数据输出对象*/
// 	CThreadProcSiteDataOutput m_oThreadProcSiteDataOutput;
// 	/** 测试对象，基本测试*/
// 	CThreadProcTest m_oThreadProcTestBase;
// 	/** 测试对象，噪声监测*/
// 	CThreadProcTest m_oThreadProcTestNoise;

	/** 内部网络命令帧接收和处理对象*/
	CNetProcInterface m_oNetProcInterface;


public: //方法
	// 初始化
	bool OnInit();
	// 关闭
	bool OnClose();
	// 工作
	bool OnWork();
	// 停止
	bool OnStop();

	// 打开程序配置文件
	void OpenAppIniXMLFile();
	// 关闭程序配置文件
	void CloseAppIniXMLFile();

	//加载测线服务器程序设置数据
	void LoadLineServerAppSetupData();
	//加载IP地址设置数据
	void LoadIPSetupData();
	//加载端口设置数据
	void LoadPortSetupData();
	//加载数据缓冲区大小数据
	void LoadBufferSizeSetupData();
};

