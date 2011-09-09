#pragma once
#include "bcgpdockingcontrolbar.h"
#include "bcgpgridview.h"
#include "ShotPoints.h"
/***************************************************************************************/
/**
 * @class CGridWorkspace
 * @brief 创建浮动窗口，内嵌网格控件
 *
 *        可浮动、支持拖拉的窗口类，内嵌网格，作为显示各种状态的放炮表基类
 * 
 * 
 */
/***************************************************************************************/
class CGridWorkspace :
	public CBCGPDockingControlBar
	// public CBCGPTabbedControlBar
{

public:	
	CCreateContext* m_pContext;
	CBCGPGridCtrl   m_GridView;
	// CBCGPTabView*  m_pTabView;
	
	// 方法
public:
	CGridWorkspace(void);
	~CGridWorkspace(void);
	
	void SetContext (CCreateContext* pContext)
	{
		m_pContext = pContext;
	}
	
	DECLARE_MESSAGE_MAP()
	virtual afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	
	
};
/***************************************************************************************/
/**
 * @class CVPGridWorkspace
 * @brief 放炮表显示浮动窗口基类
 *
 *        显示放炮表的浮动窗口基类 
 * 
 * 
 */
/***************************************************************************************/
class CVPGridWorkspace : public CGridWorkspace
{	
	// 属性
private:
	CImageList	m_ImageList;
	// 方法
public:
	CVPGridWorkspace(void);
	~CVPGridWorkspace(void);
	DECLARE_MESSAGE_MAP()
	virtual afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual bool SetGridHead(void);
	virtual void LoadShotPoints(CShotPoints* pAllVP);
		
private:	
	virtual void LoadShotPoint(CShotPoint* pVP);
public:
	/** 刷新显示炮点的状态 */
	virtual int RefreshShotPointState(CShotPoint* pShotPoint);
	// 得到当前选中的炮点编号
	bool GetActiveShotPointNb(DWORD* pdwNb);
};
/***************************************************************************************/
/**
 * @class CVPAllGridWnd
 * @brief 放炮表显示窗口
 *
 *        显示放炮表中所有状态的炮点。
 * 
 * 
 * 
 */
/***************************************************************************************/
class CVPAllGridWnd :
	public CVPGridWorkspace
{
public:
	CVPAllGridWnd(void);
	~CVPAllGridWnd(void);
	virtual bool SetGridHead(void);
	virtual void LoadShotPoints(CShotPoints* pAllVP);
private:	
	virtual void LoadShotPoint(CShotPoint* pVP);
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	// afx_msg void OnVpshotOne();
};

/******************************************************************************
    类    型： 

    功    能： 显示 To Do 将要放炮的窗口

    修改历史：

*******************************************************************************/
// CVPToDoGridWnd 视图

class CVPToDoGridWnd : public CVPGridWorkspace
{
	DECLARE_DYNCREATE(CVPToDoGridWnd)
public:
	CVPToDoGridWnd();           // 动态创建所使用的受保护的构造函数
	virtual ~CVPToDoGridWnd();

public:

	virtual bool SetGridHead(void);
	virtual void LoadShotPoints(CShotPoints* pAllVP);

private:	
	virtual void LoadShotPoint(CShotPoint* pVP);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};

/******************************************************************************
    类    型： 

    功    能： 显示Done 放炮的窗口

    修改历史：

*******************************************************************************/
// CVPDoneGridWnd

class CVPDoneGridWnd : public CVPGridWorkspace
{
	DECLARE_DYNAMIC(CVPDoneGridWnd)

public:
	CVPDoneGridWnd();
	virtual ~CVPDoneGridWnd();

	virtual bool SetGridHead(void);
	virtual void LoadShotPoints(CShotPoints* pAllVP);

private:	
	virtual void LoadShotPoint(CShotPoint* pVP);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	virtual int RefreshShotPointState(CShotPoint* pShotPoint);
};


