#pragma once

/**
 * @class CShotComment
 * @brief 操作员注释
 *
 * 定义一个操作员注释对象，用于描述操作员日志中一次放炮记录
 */
class CShotComment
{
public:
	DWORD		m_dwCommentNb;
	CString		m_strLabel;
	CString     m_strComment;
public:
	CShotComment(void);
	~CShotComment(void);
};
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/**
 * @class CShotComments
 * @brief 操作员注释表
 *
 * 操作员注释表，保存所有的操作员注释
 */
class CShotComments
{
public:
	CArray<CShotComment*,CShotComment*>		m_AllComments;
public:
	CShotComments();
	~CShotComments();

	INT_PTR    GetCount(void)						{ return m_AllComments.GetCount();	   }
	INT_PTR    Add(CShotComment* pComment)			{ return m_AllComments.Add(pComment);  }
	CShotComment* GetShotComment(int nIndex);
	void   RemoveAll(void);
};