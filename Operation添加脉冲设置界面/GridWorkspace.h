#pragma once
#include "bcgpdockingcontrolbar.h"
#include "bcgpgridview.h"
#include "ShotPoints.h"
class CGridWorkspace :
	public CBCGPDockingControlBar
{
	// 属性
private:
	CImageList	m_ImageList;
public:	
	CCreateContext* m_pContext;
	CBCGPGridCtrl* m_pGridView;
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
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	
	virtual bool SetGridHead(void);
	virtual void LoadShotPoints(CShotPoints* pAllVP);
		
private:	
	virtual void LoadShotPoint(CShotPoint* pVP);
public:
	virtual int SetShotPointState(CShotPoint* pShotPoint,unsigned char byState);
	// 得到当前选中的炮点编号
	bool GetActiveShotPointNb(DWORD* pdwNb);
};
/******************************************************************************
    类    型： 

    功    能： 显示所有放炮的窗口

    修改历史：

*******************************************************************************/
class CVPAllGridWnd :
	public CGridWorkspace
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

#pragma once

/******************************************************************************
    类    型： 

    功    能： 显示 To Do 将要放炮的窗口

    修改历史：

*******************************************************************************/
// CVPToDoGridWnd 视图

class CVPToDoGridWnd : public CGridWorkspace
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


#pragma once

/******************************************************************************
    类    型： 

    功    能： 显示Done 放炮的窗口

    修改历史：

*******************************************************************************/
// CVPDoneGridWnd

class CVPDoneGridWnd : public CGridWorkspace
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
	virtual int SetShotPointState(CShotPoint* pShotPoint,unsigned char byState);
};


