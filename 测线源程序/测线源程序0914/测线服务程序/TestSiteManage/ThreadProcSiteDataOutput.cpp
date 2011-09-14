// ThreadProcSiteDataOutput.cpp : 实现文件
//

#include "stdafx.h"
#include "TestSiteManage.h"
#include "ThreadProcSiteDataOutput.h"


// CThreadProcSiteDataOutput

IMPLEMENT_DYNCREATE(CThreadProcSiteDataOutput, CWinThread)

CThreadProcSiteDataOutput::CThreadProcSiteDataOutput()
{
	// 是否工作状态
	m_bWork = false;
	// 是否关闭线程
	m_bClose = false;
}

CThreadProcSiteDataOutput::~CThreadProcSiteDataOutput()
{
}

BOOL CThreadProcSiteDataOutput::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CThreadProcSiteDataOutput::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadProcSiteDataOutput, CWinThread)
END_MESSAGE_MAP()

// CThreadProcSiteDataOutput 消息处理程序

/**
* 线程主函数
* @param void
* @return int
*/
int CThreadProcSiteDataOutput::Run()
{
	// 循环，直到关闭标志为真
	while(true)
	{
		// 判断关闭标志是否为真
		if(true == m_bClose)
		{
			break;
		}
		// 现场数据输出
		ProcSiteDataOutput();
		// 判断关闭标志是否为真
		if(true == m_bClose)
		{
			break;
		}
		// 休眠1000毫秒
		Wait(20);
	}
	// 现场数据输出线程线程关闭标志为真
	m_pSiteData->m_bProcSiteDataOutputClose = true;
	// 返回
	return CWinThread::Run();
}

/**
* 初始化线程
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcSiteDataOutput::OnInit()
{
	// 是否工作状态
	m_bWork = false;
	// 是否关闭线程
	m_bClose = false;

	// 打开日志文件
	LogFileOpen();
	// 日志文件对象是否关闭
	m_bLogFileClose = false;	
	return true;
}

/**
* 关闭线程
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcSiteDataOutput::OnClose()
{
	// 是否关闭线程
	m_bClose = true;
	// 日志文件对象是否关闭
	m_bLogFileClose = true;	
	// 打开日志文件
	LogFileClose();
	return true;
}

/**
* 线程开始工作
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcSiteDataOutput::OnWork()
{
	// 是否工作状态
	m_bWork = true;
	return true;
}

/**
* 线程停止工作
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcSiteDataOutput::OnStop()
{
	// 是否工作状态
	m_bWork = false;
	return true;
}

/**
* 线程等待
* @param int iWaitStopCount 最多等待次数；每次50毫秒
* @return void
*/
void CThreadProcSiteDataOutput::Wait(int iWaitStopCount)
{
	// 等待次数
	int iWaitCount = 0;
	while(true)
	{	
		Sleep(50);	// 休眠
		iWaitCount++;	// 等待次数
		// 判断是否可以处理的条件
		if(false == JudgeProcCondition())
		{
			return;
		}
		// 等待次数
		if(iWaitStopCount <= iWaitCount)
		{
			return;
		}		
	}
}

/**
* 判断是否可以处理的条件
* @param void
* @return bool true：继续处理；false：停止处理
*/
bool CThreadProcSiteDataOutput::JudgeProcCondition()
{	
	// 删除仪器或路由处理线程正在运行
	if(true == m_pSiteData->m_bProcDeleteRunning)
	{		
		m_pSiteData->m_bProcSiteDataOutputPause = true;	// 线程暂停
		return false;
	}
	else
	{		
		m_pSiteData->m_bProcSiteDataOutputPause = false;	// 线程运行
	}
	// 非工作状态
	if(false == m_bWork)
	{		
		m_pSiteData->m_bProcSiteDataOutputStop = true;	// 现场数据输出线程线程停止
		return false;
	}
	else
	{
		m_pSiteData->m_bProcSiteDataOutputStop = false;	// 现场数据输出线程线程工作
	}
	return true;
}

/**
* 现场数据输出
* @param void
* @return void
*/
void CThreadProcSiteDataOutput::ProcSiteDataOutput()
{	
	// 判断是否可以处理的条件
	if(false == JudgeProcCondition())
	{
		return;
	}

	// 输出运行信息到日志文件
	OutputRunTimeDataToLogFile();

	// 判断现场数据是否在规定时间内无变化
	if(false == m_pSiteData->JudgeSiteDataChangedTime())
	{
		return;
	}
	// 判断锁定删除线程失败
	if(false == m_pThreadProcDelete->Lock())
	{
		return;
	}
	// 设置现场数据输出线程正在运行标志为真
	m_pSiteData->m_bProcSiteDataOutputRunning = true;
	// 判断等待其他线程暂停成功
	if(true == WaitOtherThreadProcPause())
	{
		// 复制数组到输出数组
		CopyArrayForOutput();
		// 设置现场数据输出线程正在运行标志为假
		m_pSiteData->m_bProcSiteDataOutputRunning = false;
		// 解锁删除线程
		m_pThreadProcDelete->Unlock();
		// 重置上次现场数据变化时刻
		m_pSiteData->ResetSiteDataChangedTime();
		// 输出到数据文件
		if(true == OutputToFile())
		{
			// 数据文件保存到FTP服务器
			if(true == SaveDataFileToFTPServer())
			{
				// 向客户端发送现场数据输出更新通知命令
				SendSiteOutputCmdToClient();
				// 设置运行状态数据
				m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcSiteDataOutput", "ProcSiteDataOutput", "");
			}
			else
			{
				// 设置运行状态数据
				m_pSiteData->m_oRunTimeDataList.Set(2, "CThreadProcSiteDataOutput", "ProcSiteDataOutput", "SaveFileToFTPServer Fail");
			}
		}
		else
		{
			// 设置运行状态数据
			m_pSiteData->m_oRunTimeDataList.Set(2, "CThreadProcSiteDataOutput", "ProcSiteDataOutput", "OutputToFile Fail");
		}
		// 处理新加检波器的初始测试
		ProcSensorTestOnField();
	}
	else	// 等待其他线程暂停失败
	{
		// 设置现场数据输出线程正在运行标志为假
		m_pSiteData->m_bProcSiteDataOutputRunning = false;
		// 解锁删除线程
		m_pThreadProcDelete->Unlock();
	}
}

/**
* 等待其他线程暂停
* @note 删除线程工作时，需要暂停的线程：首包处理线程、尾包处理线程、路由监视线程、仪器IP地址分配线程
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcSiteDataOutput::WaitOtherThreadProcPause()
{
	// 等待次数
	int iWaitCount = 0;
	while(true)
	{	
		if((true == m_pSiteData->m_bProcHeadFramePause)	// 首包处理线程暂停
			&& (true == m_pSiteData->m_bProcTailFramePause)	// 尾包处理线程暂停
			&& (true == m_pSiteData->m_bProcMonitorRoutPause)	// 路由监视线程暂停
			&& (true == m_pSiteData->m_bProcIPDistributePause))	// 仪器IP地址分配线程暂停
		{
			break;
		}
		// 等待次数
		iWaitCount++;
		// 休眠
		Sleep(50);
		// 等待次数
		if(40 == iWaitCount)
		{
			return false;
		}
	}
	return true;
}

/**
* 复制现场数据数组到输出数据数组
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcSiteDataOutput::CopyArrayForOutput()
{
	// 复制仪器数组到输出数组
	m_pSiteData->m_oInstrumentList.CopyInstrumentArrayForOutput();
	// 复制路由数组到输出数组
	m_pSiteData->m_oRoutList.CopyRoutArrayForOutput();
	// 复制测道数组到输出数组
	m_pLogicData->m_oSurveySetupData.m_oChannelList.CopyChannelArrayForOutput();
	return true;
}

/**
* 输出现场数据到数据文件
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcSiteDataOutput::OutputToFile()
{
	try
	{
		CString strFileInstrument, strFileRout, strFileChannel;
		UINT uiFileSize;
		strFileInstrument = "..\\data\\FileInstrument.dat";
		strFileRout = "..\\data\\FileRout.dat";
		strFileChannel = "..\\data\\FileChannel.dat";
		uiFileSize = sizeof(CInstrument) * m_pSiteData->m_oInstrumentList.m_uiCountAll;
		OutputToFile(strFileInstrument, (byte*)m_pSiteData->m_oInstrumentList.m_pArrayInstrumentOutput, uiFileSize);
		uiFileSize = sizeof(CRout) * m_pSiteData->m_oRoutList.m_uiCountAll;
		OutputToFile(strFileRout, (byte*)m_pSiteData->m_oRoutList.m_pArrayRoutOutput, uiFileSize);
		uiFileSize = sizeof(CChannelSetupData) * m_pLogicData->m_oSurveySetupData.m_oChannelList.m_uiCountAll;
		OutputToFile(strFileChannel, (byte*)m_pLogicData->m_oSurveySetupData.m_oChannelList.m_pArrayChannelOutput, uiFileSize);
		return true;
	}
	catch (CException* e)
	{
		return false;
	}
}

/**
* 数据文件保存到FTP服务器
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcSiteDataOutput::SaveDataFileToFTPServer()
{
	bool bReturn = false;
	CInternetSession oSession;
	BOOL bData;
	CFtpConnection* pConnection = oSession.GetFtpConnection(m_pSiteData->m_strIPFTPServer);

	bData = SaveDataFileToFTPServer(pConnection, "..\\data\\FileInstrument.dat", "\\data\\FileInstrument.dat");
	if(TRUE == bData)
	{
		bData = SaveDataFileToFTPServer(pConnection, "..\\data\\FileRout.dat", "\\data\\FileRout.dat");
		if(TRUE == bData)
		{
			bData = SaveDataFileToFTPServer(pConnection, "..\\data\\FileChannel.dat", "\\data\\FileChannel.dat");
			if(TRUE == bData)
			{
				bReturn = true;
			}
		}
	}
	pConnection->Close();
	delete pConnection;
	return bReturn;
}

/**
* 数据文件保存到FTP服务器
* @param CFtpConnection* pConnection FTP链接
* @param CString strFile 源文件名
* @param CString strFile 目的文件名
* @return bool true：成功；false：失败
*/
bool CThreadProcSiteDataOutput::SaveDataFileToFTPServer(CFtpConnection* pConnection, CString strFileSource, CString strFileAim)
{
	BOOL bData;
	int iCount = 0;
	while(true)
	{
		bData = pConnection->PutFile(strFileSource, strFileAim);
		if(TRUE == bData)
		{
			return true;
		}
		// 判断是否可以处理的条件
		if(false == JudgeProcCondition())
		{
			return false;
		}
		iCount++;
		if(4 <= iCount)
		{
			return false;
		}
		Sleep(50);
	}
	return false;
}

/**
* 输出现场数据到数据文件
* @param CString strFileName 文件名
* @param byte* pData 数据指针
* @param UINT uiFileSize 数据长度
* @return bool true：成功；false：失败
*/
bool CThreadProcSiteDataOutput::OutputToFile(CString strFileName, byte* pData, UINT uiFileSize)
{
	CStdioFile oFile;
	oFile.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
	oFile.Write(pData, uiFileSize);
	oFile.Close();
	return true;
}

/**
* 向客户端发送现场数据输出更新通知命令
* @param void
* @return void
*/
void CThreadProcSiteDataOutput::SendSiteOutputCmdToClient()
{
	// 判断：测试数据无变化
	if(false == m_pSiteData->m_bTestDataChanged)
	{
		// 为服务器端命令帧发送网络接口准备的线程同步对象锁定
		m_pSiteData->m_oCriticalSectionInterfaceCmdSend.Lock();
			// 生成现场数据输出通知命令帧
			m_pSocketInterfaceCmdSend->m_oFrameInterface.MakeSiteDataOutputCmdFrame();
			// 发送命令帧
			m_pSocketInterfaceCmdSend->SendCommandFrame();
		// 为服务器端命令帧发送网络接口准备的线程同步对象解锁
		m_pSiteData->m_oCriticalSectionInterfaceCmdSend.Unlock();
	}
	// 判断：测试数据有变化
	else
	{
		m_pSiteData->m_bTestDataChanged = false;
	}
}

/**
* 输出重置后的现场数据
* @param void
* @return void
*/
void CThreadProcSiteDataOutput::OutputToFileForReset()
{
	// 复制数组到输出数组
	CopyArrayForOutput();
	// 输出到数据文件
	if(true == OutputToFile())
	{
		// 数据文件保存到FTP服务器
		if(true == SaveDataFileToFTPServer())
		{
			// 向客户端发送现场数据输出更新通知命令
			SendSiteOutputCmdToClient();
			// 设置运行状态数据
			m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcSiteDataOutput", "OutputToFileForReset", "");
			return;
		}
	}
	// 设置运行状态数据
	m_pSiteData->m_oRunTimeDataList.Set(2, "CThreadProcSiteDataOutput", "OutputToFileForReset", "Reset Fail");
}

/**
* 打开日志文件
* @param void
* @return void
*/
void CThreadProcSiteDataOutput::LogFileOpen()
{
	SYSTEMTIME oSystemTime;
	GetLocalTime(&oSystemTime);
	CString strDate;	// 日期
	CString strTime;	// 时间
	CString strFileName;	// 时间
	strDate.Format("%d-%d-%d", oSystemTime.wYear, oSystemTime.wMonth, oSystemTime.wDay);	// 日期
	strTime.Format("%d %d %d", oSystemTime.wHour, oSystemTime.wMinute, oSystemTime.wSecond);	// 时间
	strFileName.Format("..\\LogFileLine\\LogFileLine%s %s.xml", strDate, strTime);
	m_oLogFile.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
}

/**
* 关闭日志文件
* @param void
* @return void
*/
void CThreadProcSiteDataOutput::LogFileClose()
{
	m_oLogFile.Close();
}

/**
* 输出运行信息到日志文件
* @param void
* @return void
*/
void CThreadProcSiteDataOutput::OutputRunTimeDataToLogFile()
{
	// 日志文件对象是否关闭
	if(true == m_bLogFileClose)
	{
		return;
	}
	CString strData = "";
	while(true)
	{
		strData = m_pSiteData->m_oRunTimeDataList.Get();
		if(strData == "")
		{
			break;
		}
		m_oLogFile.WriteString(strData);
	}
}

/**
* 处理新加检波器的初始测试
* @param void
* @return void
*/
void CThreadProcSiteDataOutput::ProcSensorTestOnField()
{
	m_pThreadProcTestBase->ProcSensorTestOnField();
}