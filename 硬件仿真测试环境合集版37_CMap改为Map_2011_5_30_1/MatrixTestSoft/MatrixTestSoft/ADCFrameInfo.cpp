#include "StdAfx.h"
#include "ADCFrameInfo.h"
CADCFrameInfo::CADCFrameInfo(void)
: m_csSaveFilePath(_T(""))
/*, m_pFileSave(NULL)*/
{
}

CADCFrameInfo::~CADCFrameInfo(void)
{
}

// 打开文件
void CADCFrameInfo::OnOpenFile(void)
{
	CString strFileName = _T("");
	CString strTemp = _T("");
//	errno_t err;
	CString str = _T("");
	SYSTEMTIME  sysTime;
	strFileName += m_csSaveFilePath;
	strTemp = _T("\\ADC数据帧信息.txt");
	strFileName += strTemp;
// 	// 将ADC采样数据保存成ANSI格式的文件
// 	if((err = fopen_s(&m_pFileSave,strFileName,"w+"))!=NULL)
// 	{
// 		AfxMessageBox(_T("ADC数据帧信息文件创建失败！"));	
// 		return;
// 	}
	//保存成UNICODE格式的文件
	if(m_FileSave.Open(strFileName, CFile::modeCreate|CFile::modeWrite) == FALSE)
	{
		AfxMessageBox(_T("ADC数据帧信息文件创建失败！"));	
		return;
	}

	GetLocalTime(&sysTime);
	str.Format(_T("%04d年%02d月%02d日%02d:%02d:%02d:%03d开始记录ADC帧信息：\r\n"), sysTime.wYear,sysTime.wMonth,sysTime.wDay,
		sysTime.wHour,sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds);

//	fprintf(m_pFileSave, _T("%s"), strOutput);
	//因为需要保存的内容包含中文，所以需要如下的转换过程
	WriteCHToCFile(&m_FileSave, str);
}

// 保存文件
void CADCFrameInfo::OnSaveFile(unsigned int uiInstrumentNb, unsigned int uiDataPointNum, unsigned int uiSysTime)
{
	CString strOutput = _T("");
	strOutput.Format(_T("设备ID %d\t帧的PointNum %d\t\t帧发送的SysTime %d\n"), uiInstrumentNb, uiDataPointNum, uiSysTime);
//	fprintf(m_pFileSave, _T("%s"), strOutput); 
// 	CArchive ar(&m_FileSave, CArchive::store);
// 	ar.WriteString(strOutput);
// 	ar.Close();
	//因为需要保存的内容包含中文，所以需要如下的转换过程
	WriteCHToCFile(&m_FileSave, strOutput);
}

// 关闭文件
void CADCFrameInfo::OnCloseFile(void)
{
// 	if (m_pFileSave == NULL)
// 	{
// 		return;
// 	}
// 	fclose(m_pFileSave); 
	m_FileSave.Close();
}
