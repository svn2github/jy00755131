// SendThread.cpp : 实现文件
//

#include "stdafx.h"
#include "端口监视器.h"
#include "SendThread.h"
#include "Parameter.h"

// CSendThread

IMPLEMENT_DYNCREATE(CSendThread, CWinThread)

CSendThread::CSendThread()
: m_uiHeadFrameNum(0)
, m_uiIPSetReturnNum(0)
, m_uiTailFrameNum(0)
, m_uiTailTimeReturnNum(0)
, m_uiDelayTimeReturnNum(0)
, m_uiADCSetReturnNum(0)
, m_csHeadFrameShow(_T(""))
, m_csIPSetReturnShow(_T(""))
, m_csTailFrameShow(_T(""))
, m_csTailTimeReturnShow(_T(""))
, m_csDelayTimeReturnShow(_T(""))
, m_csADCDataRecShow(_T(""))
, m_uiRecFrameNum(0)
, m_uiADCDataRecNum(0)
, m_usADCLastDataPoint(0)
{
	
}

CSendThread::~CSendThread()
{

}

BOOL CSendThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CSendThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSendThread, CWinThread)
END_MESSAGE_MAP()


// 初始化

void CSendThread::Init()
{
	udp_count = 0;
	m_close = false;
	m_hSendThreadClose = ::CreateEvent(false, false, NULL, NULL);	// 创建事件对象
	::ResetEvent(m_hSendThreadClose);	// 设置事件对象为无信号状态
	int flag =  m_hSocket2.Create(RcvPort2,SOCK_DGRAM);
	m_hSocket2.SetSockOpt(NULL,&m_hSocket2,65536,SOL_SOCKET);
}
// 线程函数在Run中执行
int CSendThread::Run()
{
	unsigned short uiPort = 0;
	DWORD dwFrameCount = 0;
	int icount = 0;
	unsigned short usADCDataPoint = 0;
	unsigned short usADCDataPointMove = 0;
	// 循环，直到关闭标志为真
	while(true)
	{
		if (m_close == true)
		{
			break;
		}

		DWORD dwFrameCount = 0;

		// 得到网络接收缓冲区数据字节数
		m_hSocket2.IOCtl(FIONREAD, &dwFrameCount);
		dwFrameCount = dwFrameCount/256;
		if(0 != dwFrameCount)	
		{
			for(unsigned int i = 0; i < dwFrameCount; i++)	// 依次检查每个仪器IP设置应答帧
			{

				icount = m_hSocket2.Receive(udp_buf[udp_count],256);
				if (icount == 256)
				{
					CString strTemp = _T("");
					unsigned int uiCommand = 0;
					// 加入端口分发功能
					memcpy(&uiPort, &udp_buf[udp_count][24], 2);
					if (uiPort == HeadFramePort)
					{
						memcpy(&uiCommand, &udp_buf[udp_count][28], 1);
						if (uiCommand == 0x01)
						{
							// 显示SN
							strTemp.Format(_T("%02x "),udp_buf[udp_count][29]);
							m_csHeadFrameShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][30]);
							m_csHeadFrameShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][31]);
							m_csHeadFrameShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][32]);
							m_csHeadFrameShow += strTemp;
							m_csHeadFrameShow += _T("\t");
							// 显示首包时刻
							strTemp.Format(_T("%02x "),udp_buf[udp_count][34]);
							m_csHeadFrameShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][35]);
							m_csHeadFrameShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][36]);
							m_csHeadFrameShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][37]);
							m_csHeadFrameShow += strTemp;
							m_csHeadFrameShow += _T("\r\n");

							pDlg->GetDlgItem(IDC_EDIT_HEADFRAMESHOW)->SetWindowText(m_csHeadFrameShow);
							m_uiHeadFrameNum++;
							strTemp.Format(_T("%d"),m_uiHeadFrameNum);
							pDlg->GetDlgItem(IDC_STATIC_HEADFRAMENUM)->SetWindowText(strTemp);
						}
					}
					else if (uiPort == IPSetPort)
					{
						memcpy(&uiCommand, &udp_buf[udp_count][28], 1);
						if (uiCommand == 0x01)
						{
							// 显示SN
							strTemp.Format(_T("%02x "),udp_buf[udp_count][29]);
							m_csIPSetReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][30]);
							m_csIPSetReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][31]);
							m_csIPSetReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][32]);
							m_csIPSetReturnShow += strTemp;
							m_csIPSetReturnShow += _T("\t");
							// 显示IP
							strTemp.Format(_T("%02x "),udp_buf[udp_count][34]);
							m_csIPSetReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][35]);
							m_csIPSetReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][36]);
							m_csIPSetReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][37]);
							m_csIPSetReturnShow += strTemp;
							m_csIPSetReturnShow += _T("\r\n");
							pDlg->GetDlgItem(IDC_EDIT_IPSETRETURNSHOW)->SetWindowText(m_csIPSetReturnShow);
							m_uiIPSetReturnNum++;
							strTemp.Format(_T("%d"),m_uiIPSetReturnNum);
							pDlg->GetDlgItem(IDC_STATIC_IPSETRETURNNUM)->SetWindowText(strTemp);
						}
					}
					else if (uiPort == TailFramePort)
					{
						memcpy(&uiCommand, &udp_buf[udp_count][33], 1);
						if (uiCommand == 0x01)
						{
							// 显示SN
							strTemp.Format(_T("%02x "),udp_buf[udp_count][34]);
							m_csTailFrameShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][35]);
							m_csTailFrameShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][36]);
							m_csTailFrameShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][37]);
							m_csTailFrameShow += strTemp;
							m_csTailFrameShow += _T("\t");

							// 显示IP
							strTemp.Format(_T("%02x "),udp_buf[udp_count][29]);
							m_csTailFrameShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][30]);
							m_csTailFrameShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][31]);
							m_csTailFrameShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][32]);
							m_csTailFrameShow += strTemp;
							m_csTailFrameShow += _T("\r\n");

							pDlg->GetDlgItem(IDC_EDIT_TAILFRAMESHOW)->SetWindowText(m_csTailFrameShow);
							m_uiTailFrameNum++;
							strTemp.Format(_T("%d"),m_uiTailFrameNum);
							pDlg->GetDlgItem(IDC_STATIC_TAILFRAMENUM)->SetWindowText(strTemp);
						}
					}
					else if (uiPort == TailTimeFramePort)
					{
						memcpy(&uiCommand, &udp_buf[udp_count][28], 1);
						if (uiCommand == 0x01)
						{	// 显示SN
							strTemp.Format(_T("%02x "),udp_buf[udp_count][29]);
							m_csTailTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][30]);
							m_csTailTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][31]);
							m_csTailTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][32]);
							m_csTailTimeReturnShow += strTemp;
							m_csTailTimeReturnShow += _T("\t");
							// 显示网络时间
							strTemp.Format(_T("%02x "),udp_buf[udp_count][34]);
							m_csTailTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][35]);
							m_csTailTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][36]);
							m_csTailTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][37]);
							m_csTailTimeReturnShow += strTemp;
							m_csTailTimeReturnShow += _T("\t");
							// 显示本地时间
							strTemp.Format(_T("%02x "),udp_buf[udp_count][39]);
							m_csTailTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][40]);
							m_csTailTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][41]);
							m_csTailTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][42]);
							m_csTailTimeReturnShow += strTemp;
							m_csTailTimeReturnShow += _T("\t");
							// 显示尾包接收发送时刻（前14位有效）
							strTemp.Format(_T("%02x "),udp_buf[udp_count][44]);
							m_csTailTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][45]);
							m_csTailTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][46]);
							m_csTailTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][47]);
							m_csTailTimeReturnShow += strTemp;
							m_csTailTimeReturnShow += _T("\r\n");
							pDlg->GetDlgItem(IDC_EDIT_TAILTIMERETURNSHOW)->SetWindowText(m_csTailTimeReturnShow);
							m_uiTailTimeReturnNum++;
							strTemp.Format(_T("%d"),m_uiTailTimeReturnNum);
							pDlg->GetDlgItem(IDC_STATIC_TAILTIMERETURNNUM)->SetWindowText(strTemp);
						}
					}
					else if (uiPort == TimeSetPort)
					{
						memcpy(&uiCommand, &udp_buf[udp_count][28], 1);
						if (uiCommand == 0x05)
						{
							// 显示IP
							strTemp.Format(_T("%02x "),udp_buf[udp_count][16]);
							m_csDelayTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][17]);
							m_csDelayTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][18]);
							m_csDelayTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][19]);
							m_csDelayTimeReturnShow += strTemp;
							m_csDelayTimeReturnShow += _T("\t");

							// 显示延时修正高位
							strTemp.Format(_T("%02x "),udp_buf[udp_count][29]);
							m_csDelayTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][30]);
							m_csDelayTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][31]);
							m_csDelayTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][32]);
							m_csDelayTimeReturnShow += strTemp;
							m_csDelayTimeReturnShow += _T("\t");

							// 显示延时修正低位
							strTemp.Format(_T("%02x "),udp_buf[udp_count][34]);
							m_csDelayTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][35]);
							m_csDelayTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][36]);
							m_csDelayTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),udp_buf[udp_count][37]);
							m_csDelayTimeReturnShow += strTemp;
							m_csDelayTimeReturnShow += _T("\r\n");

							pDlg->GetDlgItem(IDC_EDIT_DELAYTIMERETURNSHOW)->SetWindowText(m_csDelayTimeReturnShow);
							m_uiDelayTimeReturnNum++;
							strTemp.Format(_T("%d"),m_uiDelayTimeReturnNum);
							pDlg->GetDlgItem(IDC_STATIC_DELAYTIMERETURNNUM)->SetWindowText(strTemp);
						}
					}
					else if (uiPort == ADSetReturnPort)
					{
						// 每次采集只有一个ADC设置应答帧
						m_uiADCSetReturnNum++;
						strTemp.Format(_T("%d"),m_uiADCSetReturnNum);
						pDlg->GetDlgItem(IDC_STATIC_ADCSETRETURNNUM)->SetWindowText(strTemp);
					}
					else if (uiPort == ADRecPort)
					{
						// 显示IP
						strTemp.Format(_T("%02x "),udp_buf[udp_count][16]);
						m_csADCDataRecShow += strTemp;
						strTemp.Format(_T("%02x "),udp_buf[udp_count][17]);
						m_csADCDataRecShow += strTemp;
						strTemp.Format(_T("%02x "),udp_buf[udp_count][18]);
						m_csADCDataRecShow += strTemp;
						strTemp.Format(_T("%02x "),udp_buf[udp_count][19]);
						m_csADCDataRecShow += strTemp;
						m_csADCDataRecShow += _T(" ");

						// 显示ADC数据采样帧个数
						m_uiADCDataRecNum++;
						strTemp.Format(_T("%d"),m_uiADCDataRecNum);
						pDlg->GetDlgItem(IDC_STATIC_ADCDATARECNUM)->SetWindowText(strTemp);
						m_csADCDataRecShow += strTemp;
						m_csADCDataRecShow += _T("\t");

						memcpy(&usADCDataPoint, &udp_buf[udp_count][28], 2);
						strTemp.Format(_T("%d"),usADCDataPoint);
						m_csADCDataRecShow += strTemp;
						m_csADCDataRecShow += _T("\t");

						usADCDataPointMove = usADCDataPoint - m_usADCLastDataPoint;
						strTemp.Format(_T("%d"),usADCDataPointMove);
						m_csADCDataRecShow += strTemp;
						m_csADCDataRecShow += _T("\r\n");
						pDlg->GetDlgItem(IDC_EDIT_ADCDATARECSHOW)->SetWindowText(m_csADCDataRecShow);
						m_usADCLastDataPoint = usADCDataPoint;
					}
					else
					{
						
					}
					m_uiRecFrameNum++;
					strTemp.Format(_T("%d"),m_uiRecFrameNum);
					pDlg->GetDlgItem(IDC_STATIC_RECFRAMENUM)->SetWindowText(strTemp);

					m_hSocket2.SendTo(udp_buf[udp_count],256,uiPort,pDlg->m_strSendIP);
					//m_hSocket2.SendTo(udp_buf[udp_count],256,SendPort2,pDlg->m_strSendIP);
					pDlg->OnSaveSendData(udp_buf[udp_count],256);
					udp_count +=1;
					udp_count = udp_count%8;
				}
				else
				{
					break;
				}
			}
		}

		if (m_close == true)
		{
			break;
		}
		Sleep(50);
	}
	::SetEvent(m_hSendThreadClose);	// 设置事件对象为有信号状态
	// 返回
	return CWinThread::Run();
}
void CSendThread::OnClose()
{
	m_hSocket2.ShutDown(2);
	m_hSocket2.Close();
	m_close = true;
}
void CSendThread::OnReset()
{
	CString strTemp;

	// 首包计数
	m_uiHeadFrameNum = 0;
	strTemp.Format(_T("%d"),m_uiHeadFrameNum);
	pDlg->GetDlgItem(IDC_STATIC_HEADFRAMENUM)->SetWindowText(strTemp);

	// IP地址设置计数
	m_uiIPSetReturnNum = 0;
	strTemp.Format(_T("%d"),m_uiIPSetReturnNum);
	pDlg->GetDlgItem(IDC_STATIC_IPSETRETURNNUM)->SetWindowText(strTemp);

	// 尾包计数
	m_uiTailFrameNum = 0;
	strTemp.Format(_T("%d"),m_uiTailFrameNum);
	pDlg->GetDlgItem(IDC_STATIC_TAILFRAMENUM)->SetWindowText(strTemp);

	// 尾包时刻查询应答计数
	m_uiTailTimeReturnNum = 0;
	strTemp.Format(_T("%d"),m_uiTailTimeReturnNum);
	pDlg->GetDlgItem(IDC_STATIC_TAILTIMERETURNNUM)->SetWindowText(strTemp);

	// 时延设置应答计数
	m_uiDelayTimeReturnNum = 0;
	strTemp.Format(_T("%d"),m_uiDelayTimeReturnNum);
	pDlg->GetDlgItem(IDC_STATIC_DELAYTIMERETURNNUM)->SetWindowText(strTemp);

	// ADC设置应答计数
	m_uiADCSetReturnNum = 0;
	strTemp.Format(_T("%d"),m_uiADCSetReturnNum);
	pDlg->GetDlgItem(IDC_STATIC_ADCSETRETURNNUM)->SetWindowText(strTemp);

	// ADC数据采集帧个数
	m_uiADCDataRecNum = 0;
	strTemp.Format(_T("%d"),m_uiADCDataRecNum);
	pDlg->GetDlgItem(IDC_STATIC_ADCDATARECNUM)->SetWindowText(strTemp);

	// 接收帧数
	m_uiRecFrameNum = 0;
	strTemp.Format(_T("%d"),m_uiRecFrameNum);
	pDlg->GetDlgItem(IDC_STATIC_RECFRAMENUM)->SetWindowText(strTemp);

	// ADC数据采集上一帧的数据指针偏移量
	m_usADCLastDataPoint = 0;

	// 显示首包帧，包含SN和首包时刻
	m_csHeadFrameShow = _T("");
	pDlg->GetDlgItem(IDC_EDIT_HEADFRAMESHOW)->SetWindowText(_T(""));

	// 显示IP地址设置应答帧，包含SN和设置的IP地址
	m_csIPSetReturnShow = _T("");
	pDlg->GetDlgItem(IDC_EDIT_IPSETRETURNSHOW)->SetWindowText(_T(""));

	// 显示尾包帧，包含SN
	m_csTailFrameShow = _T("");
	pDlg->GetDlgItem(IDC_EDIT_TAILFRAMESHOW)->SetWindowText(_T(""));

	// 显示尾包时刻查询应答帧，包含IP地址和查询结果
	m_csTailTimeReturnShow = _T("");
	pDlg->GetDlgItem(IDC_EDIT_TAILTIMERETURNSHOW)->SetWindowText(_T(""));

	// 显示时延设置应答帧，包含IP地址和设置内容
	m_csDelayTimeReturnShow = _T("");
	pDlg->GetDlgItem(IDC_EDIT_DELAYTIMERETURNSHOW)->SetWindowText(_T(""));

	// 显示ADC采样数据帧，包含IP地址、帧计数和采样数据帧个数
	m_csADCDataRecShow = _T("");
	pDlg->GetDlgItem(IDC_EDIT_ADCDATARECSHOW)->SetWindowText(_T(""));

}