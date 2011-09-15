#pragma once
/******************************************************************************
    类    型：
    
    功    能： 记录施工日志信息
	           日志分为3类： 1、正常施工信息，写入施工日志中，给最终用户查看；
			                 2、程序正常信息，用于程序调试；
							 3、程序异常信息，用于程序调试；
			    

    修改历史：

*******************************************************************************/
/**
 * @class COperationLog
 * @brief 日志记录类
 *
 * @note 保存施工程序运行的日志。日志进行分类管理：
 * @note 1、正常施工信息，写入施工日志中，给最终用户查看；
 * @note 2、程序正常信息，用于程序调试；
 * @note 3、程序异常信息，用于程序调试；
 */
class COperationLog
{
	// 属性
public:

	CListBox*	m_pListBox;
	// 信息
private:
	FILE* 		m_pLogFile;
	CString		m_strLogFile;
	CString		m_strLog;
public:
	COperationLog(void);
	~COperationLog(void);
	void WriteLog(LPCTSTR strLog,int nType=1);
	void WriteBinary(BYTE* pBuf,int nSize );
};
