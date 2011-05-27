// PortMonitoringSendThread.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixTestSoft.h"
#include "PortMonitoringSendThread.h"


// CPortMonitoringSendThread

IMPLEMENT_DYNCREATE(CPortMonitoringSendThread, CWinThread)

CPortMonitoringSendThread::CPortMonitoringSendThread()
: m_usudp_count(0)
, m_iRecPort(0)
, m_iSendPort(0)
, m_csIP(_T(""))
, m_uiHeadFrameNum(0)
, m_uiIPSetReturnNum(0)
, m_uiTailFrameNum(0)
, m_uiTailTimeReturnNum(0)
, m_uiDelayTimeReturnNum(0)
, m_uiADCSetReturnNum(0)
, m_uiErrorCodeReturnNum(0)
, m_csHeadFrameShow(_T(""))
, m_csIPSetReturnShow(_T(""))
, m_csTailFrameShow(_T(""))
, m_csTailTimeReturnShow(_T(""))
, m_csDelayTimeReturnShow(_T(""))
, m_csErrorCodeReturnShow(_T(""))
, m_uiRecFrameNum(0)
, m_pwnd(NULL)
, m_pWndTab(NULL)
, m_pSaveFile(NULL)
, m_bPortDistribution(FALSE)
, m_bclose(false)
{
}

CPortMonitoringSendThread::~CPortMonitoringSendThread()
{
}

BOOL CPortMonitoringSendThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CPortMonitoringSendThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CPortMonitoringSendThread, CWinThread)
END_MESSAGE_MAP()


// CPortMonitoringSendThread 消息处理程序

int CPortMonitoringSendThread::Run()
{
	// TODO: 在此添加专用代码和/或调用基类

	unsigned short uiPort = 0;
	DWORD dwFrameCount = 0;
	int icount = 0;
	// 循环，直到关闭标志为真
	while(true)
	{
		if (m_bclose == true)
		{
			break;
		}

		// 得到网络接收缓冲区数据字节数
		m_SendSocket.IOCtl(FIONREAD, &dwFrameCount);
		dwFrameCount = dwFrameCount/RcvFrameSize;
		if(0 != dwFrameCount)	
		{
			for(unsigned int i = 0; i < dwFrameCount; i++)	// 依次检查每个仪器IP设置应答帧
			{
				ProcessMessages();
				icount = m_SendSocket.Receive(m_ucudp_buf[m_usudp_count],RcvFrameSize);
				if (icount == RcvFrameSize)
				{
					CString strTemp = _T("");
					unsigned int uiTemp = 0;
					unsigned int uiCommand = 0;
					// 加入端口分发功能
					memcpy(&uiPort, &m_ucudp_buf[m_usudp_count][24], 2);
					if (uiPort == HeadFramePort)
					{
						memcpy(&uiCommand, &m_ucudp_buf[m_usudp_count][28], 1);
						if (uiCommand == 0x01)
						{
							// 显示SN
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][29]);
							m_csHeadFrameShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][30]);
							m_csHeadFrameShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][31]);
							m_csHeadFrameShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][32]);
							m_csHeadFrameShow += strTemp;
							m_csHeadFrameShow += _T("\t");
							// 显示首包时刻
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][34]);
							m_csHeadFrameShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][35]);
							m_csHeadFrameShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][36]);
							m_csHeadFrameShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][37]);
							m_csHeadFrameShow += strTemp;
							m_csHeadFrameShow += _T("\r\n");

							m_pwnd->GetDlgItem(IDC_EDIT_HEADFRAMESHOW)->SetWindowText(m_csHeadFrameShow);
							m_uiHeadFrameNum++;
							strTemp.Format(_T("%d"),m_uiHeadFrameNum);
							m_pwnd->GetDlgItem(IDC_STATIC_HEADFRAMENUM)->SetWindowText(strTemp);
						}
					}
					else if (uiPort == IPSetPort)
					{
						memcpy(&uiCommand, &m_ucudp_buf[m_usudp_count][28], 1);
						if (uiCommand == 0x01)
						{
							// 显示SN
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][29]);
							m_csIPSetReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][30]);
							m_csIPSetReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][31]);
							m_csIPSetReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][32]);
							m_csIPSetReturnShow += strTemp;
							m_csIPSetReturnShow += _T("\t");
							// 显示IP
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][34]);
							m_csIPSetReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][35]);
							m_csIPSetReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][36]);
							m_csIPSetReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][37]);
							m_csIPSetReturnShow += strTemp;
							m_csIPSetReturnShow += _T("\r\n");
							m_pwnd->GetDlgItem(IDC_EDIT_IPSETRETURNSHOW)->SetWindowText(m_csIPSetReturnShow);
							m_uiIPSetReturnNum++;
							strTemp.Format(_T("%d"),m_uiIPSetReturnNum);
							m_pwnd->GetDlgItem(IDC_STATIC_IPSETRETURNNUM)->SetWindowText(strTemp);
						}
					}
					else if (uiPort == TailFramePort)
					{
						memcpy(&uiCommand, &m_ucudp_buf[m_usudp_count][33], 1);
						if (uiCommand == 0x01)
						{
							// 显示SN
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][34]);
							m_csTailFrameShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][35]);
							m_csTailFrameShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][36]);
							m_csTailFrameShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][37]);
							m_csTailFrameShow += strTemp;
							m_csTailFrameShow += _T("\t");

							// 显示IP
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][29]);
							m_csTailFrameShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][30]);
							m_csTailFrameShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][31]);
							m_csTailFrameShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][32]);
							m_csTailFrameShow += strTemp;
							m_csTailFrameShow += _T("\r\n");

							m_pwnd->GetDlgItem(IDC_EDIT_TAILFRAMESHOW)->SetWindowText(m_csTailFrameShow);
							m_uiTailFrameNum++;
							strTemp.Format(_T("%d"),m_uiTailFrameNum);
							m_pwnd->GetDlgItem(IDC_STATIC_TAILFRAMENUM)->SetWindowText(strTemp);
						}
					}
					else if (uiPort == TailTimeFramePort)
					{
						memcpy(&uiCommand, &m_ucudp_buf[m_usudp_count][28], 1);
						if (uiCommand == 0x01)
						{	// 显示SN
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][29]);
							m_csTailTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][30]);
							m_csTailTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][31]);
							m_csTailTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][32]);
							m_csTailTimeReturnShow += strTemp;
							m_csTailTimeReturnShow += _T(" ");
							// 显示网络时间
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][34]);
							m_csTailTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][35]);
							m_csTailTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][36]);
							m_csTailTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][37]);
							m_csTailTimeReturnShow += strTemp;
							m_csTailTimeReturnShow += _T(" ");
							// 显示本地时间
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][39]);
							m_csTailTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][40]);
							m_csTailTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][41]);
							m_csTailTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][42]);
							m_csTailTimeReturnShow += strTemp;
					
							m_csTailTimeReturnShow += _T("\r\n");
							m_pwnd->GetDlgItem(IDC_EDIT_TAILTIMERETURNSHOW)->SetWindowText(m_csTailTimeReturnShow);
							m_uiTailTimeReturnNum++;
							strTemp.Format(_T("%d"),m_uiTailTimeReturnNum);
							m_pwnd->GetDlgItem(IDC_STATIC_TAILTIMERETURNNUM)->SetWindowText(strTemp);
						}
					}
					else if (uiPort == TimeSetPort)
					{
						memcpy(&uiCommand, &m_ucudp_buf[m_usudp_count][28], 1);
						if (uiCommand == 0x05)
						{
							// 显示IP
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][16]);
							m_csDelayTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][17]);
							m_csDelayTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][18]);
							m_csDelayTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][19]);
							m_csDelayTimeReturnShow += strTemp;
							m_csDelayTimeReturnShow += _T(" ");

							// 显示延时修正高位
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][29]);
							m_csDelayTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][30]);
							m_csDelayTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][31]);
							m_csDelayTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][32]);
							m_csDelayTimeReturnShow += strTemp;
							m_csDelayTimeReturnShow += _T(" ");

							// 显示延时修正低位
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][34]);
							m_csDelayTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][35]);
							m_csDelayTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][36]);
							m_csDelayTimeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][37]);
							m_csDelayTimeReturnShow += strTemp;
							m_csDelayTimeReturnShow += _T("\r\n");

							m_pwnd->GetDlgItem(IDC_EDIT_DELAYTIMERETURNSHOW)->SetWindowText(m_csDelayTimeReturnShow);
							m_uiDelayTimeReturnNum++;
							strTemp.Format(_T("%d"),m_uiDelayTimeReturnNum);
							m_pwnd->GetDlgItem(IDC_STATIC_DELAYTIMERETURNNUM)->SetWindowText(strTemp);
						}
					}
					else if (uiPort == ADSetReturnPort)
					{
						// 每次采集只有一个ADC设置应答帧
						m_uiADCSetReturnNum++;
						strTemp.Format(_T("%d"),m_uiADCSetReturnNum);
						m_pwnd->GetDlgItem(IDC_STATIC_ADCSETRETURNNUM)->SetWindowText(strTemp);
					}
					else if (uiPort == QueryErrorCodePort)
					{
						unsigned int uiIP = 0;
						unsigned int uiInstrumentNb = 0;
						// 显示IP
						strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][16]);
						m_csErrorCodeReturnShow += strTemp;
						strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][17]);
						m_csErrorCodeReturnShow += strTemp;
						strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][18]);
						m_csErrorCodeReturnShow += strTemp;
						strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][19]);
						m_csErrorCodeReturnShow += strTemp;
						memcpy(&uiIP, &m_ucudp_buf[m_usudp_count][16], 4);
						for (int j=0; j<InstrumentMaxCount; j++)
						{
							if (uiIP == (71 + j * 10))
							{
								uiInstrumentNb = j;
								m_uiErrorCodeReturnCount[uiInstrumentNb]++;
								break;
							}
						}
						memcpy(&uiCommand, &m_ucudp_buf[m_usudp_count][33], 1);
						if (uiCommand == 0x0b)
						{
							m_csErrorCodeReturnShow += _T("\t");
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][34]);
							m_csErrorCodeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][35]);
							m_csErrorCodeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][36]);
							m_csErrorCodeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][37]);
							m_csErrorCodeReturnShow += strTemp;
							/*m_csErrorCodeReturnShow += _T("\r\n");*/
							/*m_csErrorCodeReturnShow += _T("\t");*/
							m_csErrorCodeReturnShow += _T("\t");
							if (m_uiErrorCodeReturnCount[uiInstrumentNb] == 1)
							{
								m_ucErrorCodeReturn[uiInstrumentNb][0] = m_ucudp_buf[m_usudp_count][34];
								strTemp.Format(_T("%02x "), m_ucudp_buf[m_usudp_count][34]);
								m_csErrorCodeReturnShow += strTemp;
								m_ucErrorCodeReturn[uiInstrumentNb][1] = m_ucudp_buf[m_usudp_count][35];
								strTemp.Format(_T("%02x "), m_ucudp_buf[m_usudp_count][35]);
								m_csErrorCodeReturnShow += strTemp;
								m_ucErrorCodeReturn[uiInstrumentNb][2] = m_ucudp_buf[m_usudp_count][36];
								strTemp.Format(_T("%02x "), m_ucudp_buf[m_usudp_count][36]);
								m_csErrorCodeReturnShow += strTemp;
								m_ucErrorCodeReturn[uiInstrumentNb][3] = m_ucudp_buf[m_usudp_count][37];
								strTemp.Format(_T("%02x "), m_ucudp_buf[m_usudp_count][37]);
								m_csErrorCodeReturnShow += strTemp;
							}
							else
							{
								if (m_ucudp_buf[m_usudp_count][34] < m_ucErrorCodeReturn[uiInstrumentNb][0])
								{
									uiTemp = 256 + m_ucudp_buf[m_usudp_count][34] - m_ucErrorCodeReturn[uiInstrumentNb][0];
								}
								else
								{
									uiTemp = m_ucudp_buf[m_usudp_count][34] - m_ucErrorCodeReturn[uiInstrumentNb][0] ;
								}
								strTemp.Format(_T("%02x "), uiTemp);
								m_csErrorCodeReturnShow += strTemp;
								if (m_ucudp_buf[m_usudp_count][35] < m_ucErrorCodeReturn[uiInstrumentNb][1])
								{
									uiTemp = 256 + m_ucudp_buf[m_usudp_count][35] - m_ucErrorCodeReturn[uiInstrumentNb][1];
								}
								else
								{
									uiTemp = m_ucudp_buf[m_usudp_count][35] - m_ucErrorCodeReturn[uiInstrumentNb][1] ;
								}
								strTemp.Format(_T("%02x "), uiTemp);
								m_csErrorCodeReturnShow += strTemp;
								if (m_ucudp_buf[m_usudp_count][36] < m_ucErrorCodeReturn[uiInstrumentNb][2])
								{
									uiTemp = 256 + m_ucudp_buf[m_usudp_count][36] - m_ucErrorCodeReturn[uiInstrumentNb][2];
								}
								else
								{
									uiTemp = m_ucudp_buf[m_usudp_count][36] - m_ucErrorCodeReturn[uiInstrumentNb][2] ;
								}
								strTemp.Format(_T("%02x "), uiTemp);
								m_csErrorCodeReturnShow += strTemp;
								if (m_ucudp_buf[m_usudp_count][37] < m_ucErrorCodeReturn[uiInstrumentNb][3])
								{
									uiTemp = 256 + m_ucudp_buf[m_usudp_count][37] - m_ucErrorCodeReturn[uiInstrumentNb][3];
								}
								else
								{
									uiTemp = m_ucudp_buf[m_usudp_count][37] - m_ucErrorCodeReturn[uiInstrumentNb][3] ;
								}
								strTemp.Format(_T("%02x "), uiTemp);
								m_csErrorCodeReturnShow += strTemp;
							}
							m_csErrorCodeReturnShow += _T("\r\n");
						}
						else if (uiCommand == 0x1d)
						{
							m_csErrorCodeReturnShow += _T("\t");
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][34]);
							m_csErrorCodeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][35]);
							m_csErrorCodeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][39]);
							m_csErrorCodeReturnShow += strTemp;
							strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][40]);
							m_csErrorCodeReturnShow += strTemp;
							/*m_csErrorCodeReturnShow += _T("\r\n");*/
							/*m_csErrorCodeReturnShow += _T("\t");*/
							m_csErrorCodeReturnShow += _T("\t");
							if (m_uiErrorCodeReturnCount[uiInstrumentNb] == 1)
							{
								m_ucErrorCodeReturn[uiInstrumentNb][0] = m_ucudp_buf[m_usudp_count][34];
								strTemp.Format(_T("%02x "), m_ucudp_buf[m_usudp_count][34]);
								m_csErrorCodeReturnShow += strTemp;
								m_ucErrorCodeReturn[uiInstrumentNb][1] = m_ucudp_buf[m_usudp_count][35];
								strTemp.Format(_T("%02x "), m_ucudp_buf[m_usudp_count][35]);
								m_csErrorCodeReturnShow += strTemp;
								m_ucErrorCodeReturn[uiInstrumentNb][2] = m_ucudp_buf[m_usudp_count][39];
								strTemp.Format(_T("%02x "), m_ucudp_buf[m_usudp_count][39]);
								m_csErrorCodeReturnShow += strTemp;
								m_ucErrorCodeReturn[uiInstrumentNb][3] = m_ucudp_buf[m_usudp_count][40];
								strTemp.Format(_T("%02x "), m_ucudp_buf[m_usudp_count][40]);
								m_csErrorCodeReturnShow += strTemp;
							}
							else
							{
								if (m_ucudp_buf[m_usudp_count][34] < m_ucErrorCodeReturn[uiInstrumentNb][0])
								{
									uiTemp = 256 + m_ucudp_buf[m_usudp_count][34] - m_ucErrorCodeReturn[uiInstrumentNb][0];
								}
								else
								{
									uiTemp = m_ucudp_buf[m_usudp_count][34] - m_ucErrorCodeReturn[uiInstrumentNb][0] ;
								}
								strTemp.Format(_T("%02x "), uiTemp);
								m_csErrorCodeReturnShow += strTemp;
								if (m_ucudp_buf[m_usudp_count][35] < m_ucErrorCodeReturn[uiInstrumentNb][1])
								{
									uiTemp = 256 + m_ucudp_buf[m_usudp_count][35] - m_ucErrorCodeReturn[uiInstrumentNb][1];
								}
								else
								{
									uiTemp = m_ucudp_buf[m_usudp_count][35] - m_ucErrorCodeReturn[uiInstrumentNb][1] ;
								}
								strTemp.Format(_T("%02x "), uiTemp);
								m_csErrorCodeReturnShow += strTemp;
								if (m_ucudp_buf[m_usudp_count][39] < m_ucErrorCodeReturn[uiInstrumentNb][2])
								{
									uiTemp = 256 + m_ucudp_buf[m_usudp_count][39] - m_ucErrorCodeReturn[uiInstrumentNb][2];
								}
								else
								{
									uiTemp = m_ucudp_buf[m_usudp_count][39] - m_ucErrorCodeReturn[uiInstrumentNb][2] ;
								}
								strTemp.Format(_T("%02x "), uiTemp);
								m_csErrorCodeReturnShow += strTemp;
								if (m_ucudp_buf[m_usudp_count][40] < m_ucErrorCodeReturn[uiInstrumentNb][3])
								{
									uiTemp = 256 + m_ucudp_buf[m_usudp_count][40] - m_ucErrorCodeReturn[uiInstrumentNb][3];
								}
								else
								{
									uiTemp = m_ucudp_buf[m_usudp_count][40] - m_ucErrorCodeReturn[uiInstrumentNb][3] ;
								}
								strTemp.Format(_T("%02x "), uiTemp);
								m_csErrorCodeReturnShow += strTemp;
							}
							m_csErrorCodeReturnShow += _T("\r\n");
						}
						m_pwnd->GetDlgItem(IDC_EDIT_ERRORCODERETURNSHOW)->SetWindowText(m_csErrorCodeReturnShow);
						m_uiErrorCodeReturnNum++;
						strTemp.Format(_T("%d"),m_uiErrorCodeReturnNum);
						m_pwnd->GetDlgItem(IDC_STATIC_ERRORCODERETURNNUM)->SetWindowText(strTemp);
					}
					m_uiRecFrameNum++;
// 					strTemp.Format(_T("%d"),m_uiRecFrameNum);
// 					m_pWndTab->GetDlgItem(IDC_STATIC_RECFRAMENUM)->SetWindowText(strTemp);
					if (m_bPortDistribution == TRUE)
					{
						// 开启端口分发功能
						m_SendSocket.SendTo(m_ucudp_buf[m_usudp_count],RcvFrameSize,uiPort,m_csIP);
					}
					else
					{
						m_SendSocket.SendTo(m_ucudp_buf[m_usudp_count],RcvFrameSize,m_iSendPort,m_csIP);
					}

					m_pSaveFile->OnSaveSendData(m_ucudp_buf[m_usudp_count],RcvFrameSize);
					m_usudp_count +=1;
					m_usudp_count = m_usudp_count%8;
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
		Sleep(1);
	}
	::SetEvent(m_hPortMonitoringSendThreadClose);	// 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	return CWinThread::Run();
}

// 初始化
void CPortMonitoringSendThread::OnInit(void)
{
	m_hPortMonitoringSendThreadClose = ::CreateEvent(false, false, NULL, NULL);	// 创建事件对象
	::ResetEvent(m_hPortMonitoringSendThreadClose);	// 设置事件对象为无信号状态
	for (int i=0; i<GraphViewNum; i++)
	{
		 m_uiErrorCodeReturnCount[i] = 0;
		 m_ucErrorCodeReturn[i][0] = 0;
		 m_ucErrorCodeReturn[i][1] = 0;
		 m_ucErrorCodeReturn[i][2] = 0;
		 m_ucErrorCodeReturn[i][3] = 0;
	}
}

// 打开
void CPortMonitoringSendThread::OnOpen(void)
{
	m_SendSocket.Close();
	int flag =  m_SendSocket.Create(m_iRecPort,SOCK_DGRAM);
	m_SendSocket.SetSockOpt(SO_RCVBUF,&m_SendSocket,PortMonitoringBufSize,SOL_SOCKET);
	m_SendSocket.SetSockOpt(SO_SNDBUF,&m_SendSocket,PortMonitoringBufSize,SOL_SOCKET);
}

// 停止
void CPortMonitoringSendThread::OnStop(void)
{
	m_SendSocket.Close();
	for (int i=0; i<GraphViewNum; i++)
	{
		m_uiErrorCodeReturnCount[i] = 0;
		m_ucErrorCodeReturn[i][0] = 0;
		m_ucErrorCodeReturn[i][1] = 0;
		m_ucErrorCodeReturn[i][2] = 0;
		m_ucErrorCodeReturn[i][3] = 0;
	}
}

// 关闭
void CPortMonitoringSendThread::OnClose(void)
{
	m_SendSocket.Close();
	m_bclose = true;
}
// 重置界面
void CPortMonitoringSendThread::OnReset(void)
{
	CString strTemp;
	// 首包计数
	m_uiHeadFrameNum = 0;
	strTemp.Format(_T("%d"),m_uiHeadFrameNum);

	m_pwnd->GetDlgItem(IDC_STATIC_HEADFRAMENUM)->SetWindowText(strTemp);

	// IP地址设置计数
	m_uiIPSetReturnNum = 0;
	strTemp.Format(_T("%d"),m_uiIPSetReturnNum);
	m_pwnd->GetDlgItem(IDC_STATIC_IPSETRETURNNUM)->SetWindowText(strTemp);

	// 尾包计数
	m_uiTailFrameNum = 0;
	strTemp.Format(_T("%d"),m_uiTailFrameNum);
	m_pwnd->GetDlgItem(IDC_STATIC_TAILFRAMENUM)->SetWindowText(strTemp);

	// 尾包时刻查询应答计数
	m_uiTailTimeReturnNum = 0;
	strTemp.Format(_T("%d"),m_uiTailTimeReturnNum);
	m_pwnd->GetDlgItem(IDC_STATIC_TAILTIMERETURNNUM)->SetWindowText(strTemp);

	// 时延设置应答计数
	m_uiDelayTimeReturnNum = 0;
	strTemp.Format(_T("%d"),m_uiDelayTimeReturnNum);
	m_pwnd->GetDlgItem(IDC_STATIC_DELAYTIMERETURNNUM)->SetWindowText(strTemp);

	// ADC设置应答计数
	m_uiADCSetReturnNum = 0;
	strTemp.Format(_T("%d"),m_uiADCSetReturnNum);
	m_pwnd->GetDlgItem(IDC_STATIC_ADCSETRETURNNUM)->SetWindowText(strTemp);

	// 硬件设备错误查询应答帧个数
	m_uiErrorCodeReturnNum = 0;
	strTemp.Format(_T("%d"),m_uiErrorCodeReturnNum);
	m_pwnd->GetDlgItem(IDC_STATIC_ERRORCODERETURNNUM)->SetWindowText(strTemp);


	// 显示首包帧，包含SN和首包时刻
	m_csHeadFrameShow = _T("");
	m_pwnd->GetDlgItem(IDC_EDIT_HEADFRAMESHOW)->SetWindowText(_T(""));

	// 显示IP地址设置应答帧，包含SN和设置的IP地址
	m_csIPSetReturnShow = _T("");
	m_pwnd->GetDlgItem(IDC_EDIT_IPSETRETURNSHOW)->SetWindowText(_T(""));

	// 显示尾包帧，包含SN
	m_csTailFrameShow = _T("");
	m_pwnd->GetDlgItem(IDC_EDIT_TAILFRAMESHOW)->SetWindowText(_T(""));

	// 显示尾包时刻查询应答帧，包含IP地址和查询结果
	m_csTailTimeReturnShow = _T("");
	m_pwnd->GetDlgItem(IDC_EDIT_TAILTIMERETURNSHOW)->SetWindowText(_T(""));

	// 显示时延设置应答帧，包含IP地址和设置内容
	m_csDelayTimeReturnShow = _T("");
	m_pwnd->GetDlgItem(IDC_EDIT_DELAYTIMERETURNSHOW)->SetWindowText(_T(""));

	// 显示硬件错误码和码差
	m_csErrorCodeReturnShow = _T("");
	m_pwnd->GetDlgItem(IDC_EDIT_ERRORCODERETURNSHOW)->SetWindowText(_T(""));

	// 接收帧数
	m_uiRecFrameNum = 0;
	strTemp.Format(_T("%d"),m_uiRecFrameNum);
	m_pWndTab->GetDlgItem(IDC_STATIC_RECFRAMENUM)->SetWindowText(strTemp);

}
// 防止程序在循环中运行无法响应消息
void CPortMonitoringSendThread::ProcessMessages(void)
{
	MSG msg;
	::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	::DispatchMessage(&msg);
}
