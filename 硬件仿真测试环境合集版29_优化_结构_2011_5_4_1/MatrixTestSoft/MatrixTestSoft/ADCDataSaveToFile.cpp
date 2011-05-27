#include "StdAfx.h"
#include "ADCDataSaveToFile.h"

CADCDataSaveToFile::CADCDataSaveToFile(void)
: m_bOpenADCSaveFile(FALSE)
, m_uiADCSaveFileNum(0)
, m_pFileSave(NULL)
, m_uiADCDataToSaveNum(0)
{
}

CADCDataSaveToFile::~CADCDataSaveToFile(void)
{
}

// 将ADC数据保存到文件中
int CADCDataSaveToFile::OnSaveADCToFile(double(* dpADCDataBuf)[ADCDataTempDataSize], unsigned int* upADCDataNum,
										CString csSaveFilePath, unsigned int uiADCFileLength, int* pSelectObject,
										int* pSelectObjectNoise, char** cSelectObjectName, bool bFinish)
{
	CString strOutput = _T("");
	CString strTemp = _T("");
	unsigned int uiADCDataToSaveNum = 0;
	unsigned int uiDataLength = 0;
	unsigned int uiMinSize = 0;
	unsigned int uiMaxSize = 0;

	if(m_bOpenADCSaveFile == FALSE)
	{
		OnOpenADCSaveFile(csSaveFilePath, cSelectObjectName);
	}
	uiMinSize = OnADCRecDataMinNum(upADCDataNum, pSelectObject, pSelectObjectNoise);
	uiMaxSize = OnADCRecDataMaxNum(upADCDataNum, pSelectObject, pSelectObjectNoise);
	if (bFinish == false)
	{
		uiADCDataToSaveNum = uiMinSize;
	}
	else
	{
		uiADCDataToSaveNum = uiMaxSize;
	}
	// 如果设备掉线则不影响其它设备ADC数据采集
	if ((uiMinSize == 0) && (uiMaxSize == ADCDataTempDataSize))
	{
		uiADCDataToSaveNum  = uiMaxSize;
	}

	if (uiADCDataToSaveNum == 0)
	{
		if (bFinish == true)
		{
			OnCloseADCSaveFile();
			m_uiADCDataToSaveNum = 0;
			m_uiADCSaveFileNum = 0;
		}
		return uiADCDataToSaveNum;
	}
	if (bFinish == false)
	{	
		if ((m_uiADCDataToSaveNum + uiADCDataToSaveNum) > uiADCFileLength)
		{
			uiADCDataToSaveNum = uiADCFileLength - m_uiADCDataToSaveNum;
		}
	}
	// @@@@@@@@停止采集时跨文件的情况
	// 输出各仪器采样数据
	for (unsigned int i=0; i<uiADCDataToSaveNum; i++)
	{
		ProcessMessages();
		for (int j=0; j<InstrumentNum; j++)
		{
			ProcessMessages();
			if (pSelectObject[j] == 1)
			{
				if (pSelectObjectNoise[j] == 0)
				{
					uiDataLength = upADCDataNum[j];
					if (uiDataLength == 0 )
					{
						strOutput += _T("\t\t");
						continue;
					}
					if (uiDataLength > i)
					{	
						strTemp.Format(_T("%2.*lf\t"),DecimalPlaces, dpADCDataBuf[j][i]);
						strOutput += strTemp;
					}
					else
					{
						strOutput += _T("\t\t");
					}
				}
				else
				{
					strOutput += _T("\t\t");
				}
			}
			else
			{
				strOutput += _T("\t\t");
			}
		}
		strOutput += _T("\r\n");
	}
	fprintf(m_pFileSave, _T("%s"), strOutput); 

	if (bFinish == false)
	{
		m_uiADCDataToSaveNum += uiADCDataToSaveNum;
		if (m_uiADCDataToSaveNum == uiADCFileLength)
		{
			m_uiADCDataToSaveNum = 0;
			OnCloseADCSaveFile();
		}
	}
	else
	{
		OnCloseADCSaveFile();
		m_uiADCDataToSaveNum = 0;
		m_uiADCSaveFileNum = 0;
	}
	return uiADCDataToSaveNum;
}
// 创建并打开ADC保存数据文件
void CADCDataSaveToFile::OnOpenADCSaveFile(CString csSaveFilePath, char** cSelectObjectName)
{
	m_uiADCSaveFileNum++;
	CString strFileName = _T("");
	CString strOutput = _T("");
	CString strTemp = _T("");
	errno_t err;
	CString str = _T("");
	SYSTEMTIME  sysTime;
	unsigned int uiADCDataFrameCount = 0;
	strFileName += csSaveFilePath;
	strTemp.Format(_T("\\%d.text"), m_uiADCSaveFileNum);
	strFileName += strTemp;
	// 将ADC采样数据保存成ANSI格式的文件
	if((err = fopen_s(&m_pFileSave,strFileName,"w+"))!=NULL)
	{
		AfxMessageBox(_T("ADC数据存储文件创建失败！"));	
		return;
	}

	GetLocalTime(&sysTime);
	str.Format(_T("%04d年%02d月%02d日%02d:%02d:%02d:%03d开始记录ADC采样数据：\r\n\r\n"), sysTime.wYear,sysTime.wMonth,sysTime.wDay,
		sysTime.wHour,sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds);
	strOutput += str;

	// 输出仪器标签
	if (cSelectObjectName != NULL)
	{
		for (int i=0; i<InstrumentNum; i++)
		{
			ProcessMessages();
			strTemp.Format(_T("%s\t\t"), cSelectObjectName[i]);
			strOutput += strTemp;
		}
	}

	strOutput += _T("\r\n");
	fprintf(m_pFileSave, _T("%s"), strOutput); 
	m_bOpenADCSaveFile = TRUE;
}

// 被选择仪器的ADC数据个数的最小值
unsigned int CADCDataSaveToFile::OnADCRecDataMinNum(unsigned int* upADCDataNum, int* pSelectObject,
													int* pSelectObjectNoise)
{
	unsigned int uiMinSize = 10000;
	bool bSelect = false;
	for (int i=0; i<InstrumentNum; i++)
	{
		ProcessMessages();
		if (pSelectObject[i] == 1)
		{
			if (pSelectObjectNoise[i] == 0)
			{
				if (uiMinSize > upADCDataNum[i])
				{
					uiMinSize = upADCDataNum[i];
					bSelect = true;
				}
			}
		}
	}
	if (bSelect == false)
	{
		uiMinSize = 0;
	}
	return uiMinSize;
}

// 关闭ADC保存数据文件
void CADCDataSaveToFile::OnCloseADCSaveFile(void)
{
	if (m_pFileSave == NULL)
	{
		return;
	}
	fclose(m_pFileSave); 
	m_bOpenADCSaveFile = FALSE;
}
// 防止程序在循环中运行无法响应消息
void CADCDataSaveToFile::ProcessMessages(void)
{
	MSG msg;
	::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	::DispatchMessage(&msg);
}
// 被选择仪器的ADC数据个数的最大值
unsigned int CADCDataSaveToFile::OnADCRecDataMaxNum(unsigned int* upADCDataNum, int* pSelectObject, int* pSelectObjectNoise)
{
	unsigned int uiMaxSize = 0;
	for (int i=0; i<InstrumentNum; i++)
	{
		ProcessMessages();
		if (pSelectObject[i] == 1)
		{
			if (pSelectObjectNoise[i] == 0)
			{
				if (uiMaxSize < upADCDataNum[i])
				{
					uiMaxSize = upADCDataNum[i];
				}
			}
		}
	}
	return uiMaxSize;
}
