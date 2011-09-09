#include "StdAfx.h"
#include "InstallParam.h"

CInstallParam::CInstallParam(void)
: m_nVibratorType(0)
, m_nBlasterType(0)
, m_nBlasterBaudRate(9600)
, m_nNbOfPlotters(1)
, m_nPlotterType1(0)
, m_strPlotterName1(_T("Plotter1"))
, m_strPlotterMacAddress1(_T("00:00:00:00:00:00"))
, m_nPlotterType2(0)
, m_strPlotterName2(_T("Plotter2"))
, m_strPlotterMacAddress2(_T("00:00:00:00:00:00"))
{
}

CInstallParam::~CInstallParam(void)
{
}

/**
 * @brief 获得可控震源类型名称
 * @note  该函数封装可控震源的类型编号，VE432;Other
 这些名称与安装程序设置对话框中列表框的选择项一致
 * @param LPCTSTR pstrType，可控震源名称 
 * @return 可控震源内部编号
 */
int CInstallParam::GetVibratorType(CString& strType)
{	
	switch(m_nVibratorType)
	{
	case 0:
		strType= _T("VE432");
		break;
	default:
		strType = _T("Other");
		break;
	}	
	return m_nVibratorType;
}
/**
 * @brief 设置可控震源类型，转化为内部编号
 * @note  该函数封装可控震源的类型编号，VE432;Other
 这些名称与安装程序设置对话框中列表框的选择项一致
 * @param LPCTSTR pstrType，可控震源名称 
 * @return 可控震源内部编号
 */
void CInstallParam::SetVibratorType(LPCTSTR pstrType)
{
	CString strTemp = _T("VE432");
	if(!strTemp.CompareNoCase(pstrType))
	{
		m_nVibratorType= 0;
		return;
	}
	m_nVibratorType= 1;
	
}
/**
 * @brief 获得爆炸机类型内部编号
 * @note  该函数封装爆炸机的类型编号：SHOT PRO;SGS;BoomBox;SHALLOW;MACHA;Other;
 这些名称与安装程序设置对话框中列表框的选择项一致
 * @param LPCTSTR pstrType，可控震源名称 
 * @return 可控震源内部编号
 */
int CInstallParam::GetBlasterType(CString& strType)
{
	switch(m_nBlasterType)
	{
	case 0:
		strType = _T("SHOT PRO");
		break;
	case 1:
		strType = _T("SGS");
		break;
	case 2:
		strType = _T("BoomBox");
		break;
	case 3:
		strType = _T("SHALLOW");
		break;
	case 4:
		strType = _T("MACHA");
		break;
	default:
		strType = _T("Other");
		break;
	}
	return m_nBlasterType;
}
void CInstallParam::SetBlasterType(LPCTSTR pstrType)
{	
	CString strTemp = _T("SHOT PRO");
	if(!strTemp.CompareNoCase(pstrType))
	{
		m_nBlasterType= 0;
		return;
	}
	strTemp = _T("SGS");
	if(!strTemp.CompareNoCase(pstrType))
	{
		m_nBlasterType= 1;
		return;
	}
	strTemp = _T("BoomBox");
	if(!strTemp.CompareNoCase(pstrType))
	{
		m_nBlasterType= 2;
		return;
	}
	strTemp = _T("SHALLOW");
	if(!strTemp.CompareNoCase(pstrType))
	{
		m_nBlasterType= 3;
		return;
	}
	strTemp = _T("MACHA");
	if(!strTemp.CompareNoCase(pstrType))
	{
		m_nBlasterType= 4;
		return;
	}
	m_nBlasterType= 5;
	
}
void CInstallParam::GetBlasterTypeByNb(int nPlotterType,CString& strType)
{
	switch(nPlotterType)
	{
	case 0:
		strType =_T("Plotter12");
		break;
	default:
		strType =_T("Plotter24");
		break;
	}
}
int CInstallParam::GetBlasterNbByType(LPCTSTR pstrType)
{
	CString strTemp=_T("Plotter12");
	if(!strTemp.CompareNoCase(pstrType))
	{
		return 0;
	}
	return 1;
}
