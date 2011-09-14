#if !defined(AFX_BCGCBDOTNETEXAMPLE_H__AEAC1B76_E65E_4075_9A38_537CC10CEC1F__INCLUDED_)
#define AFX_BCGCBDOTNETEXAMPLE_H__AEAC1B76_E65E_4075_9A38_537CC10CEC1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "../common/MatrixDef.h"
/**
*@brief 测线客户端App类
*/
class CBCGPVisualStudioGUIDemoApp : public CWinApp , public CBCGPWorkspace
{
public:
	CBCGPVisualStudioGUIDemoApp();
	virtual ~CBCGPVisualStudioGUIDemoApp();

	BOOL	m_bHiColorIcons;
	
	// Override from CBCGPWorkspace
	virtual void PreLoadState();
	virtual void LoadCustomState ();
	virtual void SaveCustomState ();

	/** 仪器列表文档模板*/
	CMultiDocTemplate* m_pDocTemplateInstrumentList;
	/** 检波器列表文档模板*/
	CMultiDocTemplate* m_pDocTemplateSensorList;

	/** 仪器图形文档模板*/
	CMultiDocTemplate* m_pDocTemplateInstrumentGraph;
	/** 检波器图形文档模板*/
	CMultiDocTemplate* m_pDocTemplateSensorGraph;
	/** 噪声监测图形文档模板*/
	CMultiDocTemplate* m_pDocTemplateSeismonitorGraph;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPVisualStudioGUIDemoApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL SaveAllModified();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CBCGPVisualStudioGUIDemoApp)
	afx_msg void OnAppAbout();
	afx_msg void OnViewAppLook();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CBCGPVisualStudioGUIDemoApp theApp;


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGCBDOTNETEXAMPLE_H__AEAC1B76_E65E_4075_9A38_537CC10CEC1F__INCLUDED_)
