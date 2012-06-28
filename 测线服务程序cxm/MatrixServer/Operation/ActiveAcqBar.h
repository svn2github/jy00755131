#pragma once
#include "bcgpdockingcontrolbar.h"
class CActiveAcqBar : public CBCGPDockingControlBar
{
public:
	CActiveAcqBar(void);
	~CActiveAcqBar(void);
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
private:
	CBCGPGridCtrl   m_GridView;
	CImageList	m_ImageList;
public:
	// 设置表头
	bool SetGridHead(void);
	// 载入全部Acq信息
	void LoadAcqInfos(void);
	// 载入单条Acq信息
	void LoadAcqInfo(void);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};

