// ADDataThread.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixTestSoft.h"
#include "ADDataThread.h"
#include "Parameter.h"

// ADDataThread

IMPLEMENT_DYNCREATE(CADDataThread, CWinThread)

CADDataThread::CADDataThread()
: m_bclose(false)
, m_pSelectObject(NULL)
, m_pSelectObjectNoise(NULL)
{
}

CADDataThread::~CADDataThread()
{
}

BOOL CADDataThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CADDataThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CADDataThread, CWinThread)
END_MESSAGE_MAP()


// ADDataThread 消息处理程序
// 初始化
void CADDataThread::OnInit(void)
{
	m_hADDDataThreadClose = ::CreateEvent(false, false, NULL, NULL);	// 创建事件对象
	::ResetEvent(m_hADDDataThreadClose);	// 设置事件对象为无信号状态
}
// 线程函数在Run中执行
int CADDataThread::Run()
{
	DWORD dwFrameCount = 0;
	int icount = 0;
	// 循环，直到关闭标志为真
	while(true)
	{
		if(m_bclose == true)
		{
			break;
		}

		// 得到网络接收缓冲区数据字节数
		m_ADDataSocket.IOCtl(FIONREAD, &dwFrameCount);
		dwFrameCount = dwFrameCount/256;
		if(0 != dwFrameCount)	
		{
			for(unsigned int i = 0; i < dwFrameCount; i++)	// 依次检查每个仪器IP设置应答帧
			{
				icount = m_ADDataSocket.Receive(ADData,256);
				if (icount == 256)
				{
					byte* pSampleData = &(ADData[16]);
					
					ReceiveSampleData(pSampleData);
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
	::SetEvent(m_hADDDataThreadClose);	// 设置事件对象为有信号状态
	// 返回
	return CWinThread::Run();
}
// 关闭并结束线程
void CADDataThread::OnClose(void)
{
	m_ADDataSocket.ShutDown(2);
	m_ADDataSocket.Close();
	m_bclose = true;
}

// 打开
void CADDataThread::OnOpen(void)
{
	m_ADDataSocket.Close();
	BOOL flag = FALSE; 
	flag =  m_ADDataSocket.Create(ADRecPort,SOCK_DGRAM);
	if (flag == FALSE)
	{
		AfxMessageBox(_T("接收端口创建失败！"));
	}
}

// 停止
void CADDataThread::OnStop(void)
{
	m_ADDataSocket.ShutDown(2);
	m_ADDataSocket.Close();
}
// 接收采样数据
void CADDataThread::ReceiveSampleData(byte* pSampleData)
{
		// uiTestNb表明接收到帧所对应的仪器号
	unsigned int	uiTestNb = 0;
	unsigned short	uisDataNb = 0; // 接收到帧的数据的序号
	int	iSampleData = 0;
	int pos = 0;
	double dSampleDataToV = 0.0;
	// 设m_pFrameData[16]到m_pFrameData[19]为源地址做为通道号
	memcpy(&uiTestNb, &pSampleData[pos], 4);
	pos += 4;
	// 设通道1,2,3,4分别对应四个仪器IP地址
	if (uiTestNb == 111)
	{
		uiTestNb = 1;
	}
	else if (uiTestNb == 101)
	{
		uiTestNb = 2;
	}
	else if (uiTestNb == 91)
	{
		uiTestNb = 4;
	}
	else if (uiTestNb == 81)
	{
		uiTestNb = 8;
	}
	// append the new value to the plot
	for (UINT32 i=0; i<GraphViewNum; i++)
	{
		if (uiTestNb == (UINT32)(1 << i))
		{
			// 接收数据缓存
			double m_dpReceiveData [ReceiveDataSize]; 
			pos += 8;
			// m_pFrameData[28]到m_pFrameData[29]做为采样个数的序号，暂时不用
			memcpy(&uisDataNb, &pSampleData[pos], 2);
			pos += 2;

			// 之后为数据区
			for (int j=0; j<ReceiveDataSize; j++)
			{
				memcpy(&iSampleData, &pSampleData[pos], 3);
				pos += 3;

				// 24位数转换为电压值
				dSampleDataToV = iSampleData;
				if (dSampleDataToV < 0x7FFFFF)
				{
					dSampleDataToV = dSampleDataToV/( 0x7FFFFE )*2.5;
				}
				else if (dSampleDataToV > 0x800000)
				{
					dSampleDataToV = -(0xFFFFFF - dSampleDataToV)/( 0x7FFFFE )*2.5;
				}
				else
				{
					CString str;
					str.Format("ADC采样数值溢出，溢出值为%d", dSampleDataToV);
					AfxMessageBox(str);
				}
				m_dpReceiveData[j] = dSampleDataToV;

				if (m_pSelectObject[i] == 1)
				{
					if (m_pSelectObjectNoise[i] == 0)
					{
 						m_Sec.Lock();
						m_pSampleData[i]->push_back(dSampleDataToV);
 						m_Sec.Unlock();
					}
				}
			}
			m_Sec.Lock();
 			m_pOScopeCtrl[i]->AppendPoint(m_dpReceiveData, ReceiveDataSize);
 			m_Sec.Unlock();
		}
	}
}
