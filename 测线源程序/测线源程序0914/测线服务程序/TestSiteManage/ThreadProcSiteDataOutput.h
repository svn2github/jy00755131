#pragma once

#include "LogicData.h"
#include "SiteData.h"
#include "ThreadProcDelete.h"
#include "ThreadProcTest.h"
#include "SocketInterfaceCmdSend.h"

/**
*@brief 现场数据输出线程类
*
@note 功能：
@note 监视现场数据变化；现场数据改变，且5秒后无新现场数据改变时，输出现场数据到输出文件。
@note 输出运行日志文件。
@note 输出文件：
@note FileInstrument.dat-现场仪器数据文件；FileRout.dat-现场路由数据文件；FileChannel.dat-现场测道数据文件。文件格式为二进制数据文件。
@note 现场仪器数据文件为CInstrument类对象的顺序数组。
@note 现场路由数据文件为CRout类对象的顺序数组。
@note 现场测道数据文件为CChannelSetupData类对象的顺序数组。
@note 运行日志文件。文件格式为文本数据文件。文本格式见CRunTimeData类。
@note 执行流程：
@note 启动后处于停止状态；测线管理程序可以启动/停止线程。
*/
class CThreadProcSiteDataOutput : public CWinThread
{
	friend class CSiteManage;

	DECLARE_DYNCREATE(CThreadProcSiteDataOutput)

protected:
	CThreadProcSiteDataOutput();           // 动态创建所使用的受保护的构造函数
	virtual ~CThreadProcSiteDataOutput();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();

public: //属性
	/** 是否工作状态*/
	bool m_bWork;
	/** 是否关闭线程*/
	bool m_bClose;

	/** 测线设置数据对象指针*/
	CLogicData* m_pLogicData;
	/** 现场数据对象指针*/
	CSiteData* m_pSiteData;

	/** 仪器删除对象指针*/
	CThreadProcDelete* m_pThreadProcDelete;
	/** 测试对象指针，基本测试*/
	CThreadProcTest* m_pThreadProcTestBase;

	/** 服务器内部网络命令发送处理网络对象*/
	CSocketInterfaceCmdSend* m_pSocketInterfaceCmdSend;

	/** 日志文件对象*/
	CStdioFile m_oLogFile;
	/** 日志文件对象是否关闭*/
	bool m_bLogFileClose;

public: //方法
	// 初始化
	bool OnInit();
	// 关闭
	bool OnClose();
	// 工作
	bool OnWork();
	// 停止
	bool OnStop();

	// 等待
	void Wait(int iWaitStopCount);
	// 判断是否可以处理的条件
	bool JudgeProcCondition();

	// 现场数据输出
	void ProcSiteDataOutput();
	// 等待其他线程暂停
	bool WaitOtherThreadProcPause();
	// 复制数组到输出数组
	bool CopyArrayForOutput();
	// 输出到数据文件
	bool OutputToFile();
	// 数据文件保存到FTP服务器
	bool SaveDataFileToFTPServer();
	// 数据文件保存到FTP服务器
	bool SaveDataFileToFTPServer(CFtpConnection* pConnection, CString strFileSource, CString strFileAim);
	// 输出到数据文件
	bool OutputToFile(CString strFileName, byte* pData, UINT uiFileSize);
	// 向客户端发送现场数据输出更新通知命令
	void SendSiteOutputCmdToClient();
	// 输出重置后的现场数据
	void OutputToFileForReset();
	// 处理新加检波器的初始测试
	void ProcSensorTestOnField();

	// 打开日志文件
	void LogFileOpen();
	// 关闭日志文件
	void LogFileClose();
	// 输出运行信息到日志文件
	void OutputRunTimeDataToLogFile();
};


