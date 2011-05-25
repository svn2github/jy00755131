// RecThread.cpp : 实现文件
//

#include "stdafx.h"
#include "GPSAnalysis.h"
#include "RecThread.h"


// CRecThread

IMPLEMENT_DYNCREATE(CRecThread, CWinThread)

CRecThread::CRecThread()
: m_bclose(false)
, m_csHeadFrame(_T(""))
, m_dUTCTime(0)
, m_csStatus(_T(""))
, m_dLatitudeValue(0)
, m_csLatitude(_T(""))
, m_dLongitudeValue(0)
, m_csLongitude(_T(""))
, m_dSpeed(0)
, m_dAzimuth(0)
, m_csUTCData(_T(""))
, m_dMagneticdeclination(0)
, m_csMagneticdeclination_Direction(_T(""))
, m_csMode(_T(""))
, m_cCheckSum(0)
{
}

CRecThread::~CRecThread()
{
}

BOOL CRecThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CRecThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CRecThread, CWinThread)
END_MESSAGE_MAP()


// CRecThread 消息处理程序
// 线程函数在Run中执行
int CRecThread::Run()
{
	DWORD dwFrameCount = 0;
	int iCount = 0;
	// 循环，直到关闭标志为真
	while(true)
	{
		if(m_bclose == true)
		{
			break;
		}
		// 得到网络接收缓冲区数据字节数
		m_Socket.IOCtl(FIONREAD, &dwFrameCount);
		dwFrameCount = dwFrameCount/256;
		if(0 != dwFrameCount)	
		{
			for(unsigned int i = 0; i < dwFrameCount; i++)	// 依次检查每个仪器IP设置应答帧
			{
				iCount = m_Socket.Receive(m_pRecFrameData,256);
				if (iCount == 256)
				{
					// 单个GPS帧处理
					ProcGPSFrameOne();
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
	::SetEvent(m_hRecThreadClose);	// 设置事件对象为有信号状态
	// 返回
	return CWinThread::Run();
}
// 线程初始化
void CRecThread::Init(void)
{
	// 创建Socket端口
	BOOL flag = FALSE; 
	flag =  m_Socket.Create(RecGPSPort,SOCK_DGRAM);
	if (flag == FALSE)
	{
		AfxMessageBox(_T("接收端口创建失败！"));
	}
	m_hRecThreadClose = ::CreateEvent(false, false, NULL, NULL);	// 创建事件对象
	::ResetEvent(m_hRecThreadClose);	// 设置事件对象为无信号状态
}

// 线程关闭
void CRecThread::OnClose(void)
{
	m_Socket.Close();
	m_bclose = true;
}

// 处理单个GPS帧
void CRecThread::ProcGPSFrameOne(void)
{
	DWORD dwHeadFrame = 0;
	memcpy(&dwHeadFrame, &m_pRecFrameData[0], 4);
	if (dwHeadFrame != 0x44332211)
	{
		return;
	}
	OnReset();
	byte checksum = 0;
	CString str = _T("");
	CString strtemp = _T("");
	CWnd* pWnd = AfxGetMainWnd();
	int iPos  = 0;
	int iPos0 = 0;
	int iPos1 = 0;
	int iPos2 = 0;
	str = &m_pRecFrameData[28];
	iPos = str.Find(_T("\r\n"), 0);
	str = str.Mid(0, iPos);
	// 校验和，$与*之间所有字符代码的校验和（各字节做异或运算，得到校验和后，再转换16进制格式的ASCII字符）
	for (int i=0; i<(str.GetLength()-4); i++)
	{
		checksum ^= m_pRecFrameData[29+i];
	}
	iPos0 = str.Find(_T(","), 0);
	strtemp = str.Mid(0, iPos0);
	m_csHeadFrame = strtemp;
	pWnd->GetDlgItem(IDC_EDIT_HEADFRAME)->SetWindowText(strtemp);
	if (m_csHeadFrame == _T("$GPRMC"))
	{
		// UTC时间
		iPos1 = str.Find(_T(","), iPos0+1);
		strtemp = str.Mid(iPos0+1, iPos1-iPos0-1);
		m_dUTCTime = atof(strtemp);
		pWnd->GetDlgItem(IDC_EDIT_UTCTIME)->SetWindowText(strtemp);

		// 状态
		iPos2 = str.Find(_T(","), iPos1+1);
		strtemp = str.Mid(iPos1+1, iPos2-iPos1-1);
		m_csStatus = strtemp;
		pWnd->GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strtemp);
		
		// 纬度值
		iPos0 = str.Find(_T(","), iPos2+1);
		strtemp = str.Mid(iPos2+1, iPos0-iPos2-1);
		m_dLatitudeValue = atof(strtemp);
		pWnd->GetDlgItem(IDC_EDIT_LATITUDEVALUE)->SetWindowText(strtemp);

		// 纬度
		iPos1 = str.Find(_T(","), iPos0+1);
		strtemp = str.Mid(iPos0+1, iPos1-iPos0-1);
		m_csLatitude = strtemp;
		pWnd->GetDlgItem(IDC_EDIT_LATITUDE)->SetWindowText(strtemp);

		// 经度值
		iPos2 = str.Find(_T(","), iPos1+1);
		strtemp = str.Mid(iPos1+1, iPos2-iPos1-1);
		m_dLongitudeValue = atof(strtemp);
		pWnd->GetDlgItem(IDC_EDIT_LONGITUDEVALUE)->SetWindowText(strtemp);

		// 经度
		iPos0 = str.Find(_T(","), iPos2+1);
		strtemp = str.Mid(iPos2+1, iPos0-iPos2-1);
		m_csLongitude = strtemp;
		pWnd->GetDlgItem(IDC_EDIT_LONGITUDE)->SetWindowText(strtemp);
		
		// 速度
		iPos1 = str.Find(_T(","), iPos0+1);
		strtemp = str.Mid(iPos0+1, iPos1-iPos0-1);
		m_dSpeed = atof(strtemp);
		pWnd->GetDlgItem(IDC_EDIT_SPEED)->SetWindowText(strtemp);

		// 方位角
		iPos2 = str.Find(_T(","), iPos1+1);
		strtemp = str.Mid(iPos1+1, iPos2-iPos1-1);
		m_dAzimuth = atof(strtemp);
		pWnd->GetDlgItem(IDC_EDIT_AZIMUTH)->SetWindowText(strtemp);

		// UTC日期
		iPos0 = str.Find(_T(","), iPos2+1);
		strtemp = str.Mid(iPos2+1, iPos0-iPos2-1);
		m_csUTCData = strtemp;
		pWnd->GetDlgItem(IDC_EDIT_UTCDATA)->SetWindowText(strtemp);

		// 磁偏角
		iPos1 = str.Find(_T(","), iPos0+1);
		strtemp = str.Mid(iPos0+1, iPos1-iPos0-1);
		m_dMagneticdeclination = atof(strtemp);
		pWnd->GetDlgItem(IDC_EDIT_MAGNETICDECLINATION)->SetWindowText(strtemp);

		// 磁偏角方向
		iPos2 = str.Find(_T(","), iPos1+1);
		strtemp = str.Mid(iPos1+1, iPos2-iPos1-1);
		m_csMagneticdeclination_Direction = strtemp;
		pWnd->GetDlgItem(IDC_EDIT_MAGNETICDECLINATION_DIRECTION)->SetWindowText(strtemp);

		// 模式指示
		iPos0 = str.Find(_T("*"), iPos2+1);
		strtemp = str.Mid(iPos2+1, iPos0-iPos2-1);
		m_csMode = strtemp;
		pWnd->GetDlgItem(IDC_EDIT_MODE)->SetWindowText(strtemp);

		// 校验和
		strtemp = str.Mid(iPos0+1, 2);
		sscanf_s(strtemp, _T("%x"), &m_cCheckSum);
		if (checksum == m_cCheckSum)
		{
			pWnd->GetDlgItem(IDC_EDIT_CHECKSUM)->SetWindowText(strtemp);
		}
		else
		{
			OnReset();
		}
	}
	if (m_csHeadFrame == _T("$GPGLL"))
	{
		// 纬度值
		iPos1 = str.Find(_T(","), iPos0+1);
		strtemp = str.Mid(iPos0+1, iPos1-iPos0-1);
		m_dLatitudeValue = atof(strtemp);
		pWnd->GetDlgItem(IDC_EDIT_LATITUDEVALUE)->SetWindowText(strtemp);

		// 纬度
		iPos2 = str.Find(_T(","), iPos1+1);
		strtemp = str.Mid(iPos1+1, iPos2-iPos1-1);
		m_csLatitude = strtemp;
		pWnd->GetDlgItem(IDC_EDIT_LATITUDE)->SetWindowText(strtemp);

		// 经度值
		iPos0 = str.Find(_T(","), iPos2+1);
		strtemp = str.Mid(iPos2+1, iPos0-iPos2-1);
		m_dLongitudeValue = atof(strtemp);
		pWnd->GetDlgItem(IDC_EDIT_LONGITUDEVALUE)->SetWindowText(strtemp);

		// 经度
		iPos1 = str.Find(_T(","), iPos0+1);
		strtemp = str.Mid(iPos0+1, iPos1-iPos0-1);
		m_csLongitude = strtemp;
		pWnd->GetDlgItem(IDC_EDIT_LONGITUDE)->SetWindowText(strtemp);

		// UTC时间
		iPos2 = str.Find(_T(","), iPos1+1);
		strtemp = str.Mid(iPos1+1, iPos2-iPos1-1);
		m_dUTCTime = atof(strtemp);
		pWnd->GetDlgItem(IDC_EDIT_UTCTIME)->SetWindowText(strtemp);

		// 状态
		iPos0 = str.Find(_T("*"), iPos2+1);
		strtemp = str.Mid(iPos2+1, iPos0-iPos2-1);
		m_csStatus = strtemp;
		pWnd->GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strtemp);

		// 校验和
		strtemp = str.Mid(iPos0+1, 2);
		sscanf_s(strtemp, _T("%x"), &m_cCheckSum);
		if (checksum == m_cCheckSum)
		{
			pWnd->GetDlgItem(IDC_EDIT_CHECKSUM)->SetWindowText(strtemp);
		}
		else
		{
			OnReset();
		}
	}
}

// 重置
void CRecThread::OnReset(void)
{
	m_csHeadFrame = _T("");
	m_dUTCTime = 0.0;
	m_csStatus = _T("");
	m_dLatitudeValue = 0;
	m_csLatitude = _T("");
	m_dLongitudeValue = 0;
	m_csLongitude = _T("");
	m_dSpeed = 0;
	m_dAzimuth = 0;
	m_csUTCData = _T("");
	m_dMagneticdeclination = 0;
	m_csMagneticdeclination_Direction = _T("");
	m_csMode = _T("");
	m_cCheckSum = 0;

	CString strtemp = _T("");
	CWnd* pWnd = AfxGetMainWnd();
	// 帧头
	pWnd->GetDlgItem(IDC_EDIT_HEADFRAME)->SetWindowText(strtemp);

	// UTC时间
	pWnd->GetDlgItem(IDC_EDIT_UTCTIME)->SetWindowText(strtemp);

	// 状态
	pWnd->GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strtemp);

	// 纬度值
	pWnd->GetDlgItem(IDC_EDIT_LATITUDEVALUE)->SetWindowText(strtemp);

	// 纬度
	pWnd->GetDlgItem(IDC_EDIT_LATITUDE)->SetWindowText(strtemp);

	// 经度值
	pWnd->GetDlgItem(IDC_EDIT_LONGITUDEVALUE)->SetWindowText(strtemp);

	// 经度
	pWnd->GetDlgItem(IDC_EDIT_LONGITUDE)->SetWindowText(strtemp);

	// 速度
	pWnd->GetDlgItem(IDC_EDIT_SPEED)->SetWindowText(strtemp);

	// 方位角
	pWnd->GetDlgItem(IDC_EDIT_AZIMUTH)->SetWindowText(strtemp);

	// UTC日期
	pWnd->GetDlgItem(IDC_EDIT_UTCDATA)->SetWindowText(strtemp);

	// 磁偏角
	pWnd->GetDlgItem(IDC_EDIT_MAGNETICDECLINATION)->SetWindowText(strtemp);

	// 磁偏角方向
	pWnd->GetDlgItem(IDC_EDIT_MAGNETICDECLINATION_DIRECTION)->SetWindowText(strtemp);

	// 模式指示
	pWnd->GetDlgItem(IDC_EDIT_MODE)->SetWindowText(strtemp);

	// 校验和
	pWnd->GetDlgItem(IDC_EDIT_CHECKSUM)->SetWindowText(strtemp);
}
