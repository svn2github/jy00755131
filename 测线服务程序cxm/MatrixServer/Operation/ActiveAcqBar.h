#pragma once
#include "bcgpdockingcontrolbar.h"
#include "..\\MatrixServerDll\\ConfigOperationXml.h"
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
	void LoadAcqInfo(m_oProcessAcqStruct* pStruct);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 是否需要重新载入数据
	bool m_bReload;
	/** 施工客户端信息*/
	m_oOptSetupDataStruct* m_pOptSetupData;
};

