
#pragma once
//////////////////////////////////////////////////////////////////////////
/**
 * @class CDiskRecordConfig
 * @brief 磁盘记录配置信息
 *
 * 对于放炮或者测试过程中产生的数据文件，按照该类提供的配置信息保存到对应的文件目录下。
 */
class CDiskRecordConfig
{

public:
	CDiskRecordConfig(void);
	~CDiskRecordConfig(void);
// 属性
private:
	/** 存盘设置模式：Standard、Advanced */
	int		m_nDiskRecordMode;
	/** 备份设置，Flat和Day */
	int		m_nBackupSetting;
	/** 文件备份的父目录 */
	CString	m_strFilePath;
	/** 正常采集数据存盘路径 */
	CString m_strNormalFilePath;
	/** 测试数据存盘路径 */
	CString m_strTestFilePath;
private:
	// 通过备份设置和父目录两个参数计算采集数据目录和测试数据目录
	void   GetAllFilePath(void);
public:
	int    GetDiskRecordMode(void)	{ return m_nDiskRecordMode;}
	int	   GetBackupSetting(void)   { return m_nBackupSetting; }
	LPCTSTR GetFilePath(void)		{ return (LPCTSTR)m_strFilePath;}
	LPCTSTR	GetNormalFilePath(void) { return (LPCTSTR)m_strNormalFilePath;}
	LPCTSTR GetTestFilePath(void)   { return (LPCTSTR)m_strTestFilePath;}
	void   SetDiskRecordMode(int nMode);
	void   SetBackupSetting(int nBackupSetting);
	void   SetFilePath(LPCTSTR strFilePath);

};
