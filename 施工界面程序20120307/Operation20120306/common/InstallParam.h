#pragma once


//////////////////////////////////////////////////////////////////////////
/**
 * @class CInstallParam
 * @brief 安装设置参数类
 *
 * 用于管理一组安装程序需要配置的参数。
 */
class CInstallParam
{
public:
	CInstallParam(void);
	~CInstallParam(void);

public:
	int m_nVibratorType;
	int m_nBlasterType;
	int m_nBlasterBaudRate;
	int m_nNbOfPlotters;
    int m_nPlotterType1;
	CString m_strPlotterName1;
	CString m_strPlotterMacAddress1;
	int m_nPlotterType2;
	CString m_strPlotterName2;
	CString m_strPlotterMacAddress2;

public:
	int GetVibratorType(CString& strType);
	void SetVibratorType(LPCTSTR pstrType);

	int GetBlasterType(CString& strType);
	void SetBlasterType(LPCTSTR pstrType);

	void GetBlasterTypeByNb(int nPlotterType,CString& strType);
	int GetBlasterNbByType(LPCTSTR pstrType);
};
