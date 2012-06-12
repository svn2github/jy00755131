// E2PROM_TOOLView.h : interface of the CE2PROM_TOOLView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_E2PROM_TOOLVIEW_H__E6BD2C0D_BAF4_4283_B59D_9A8F24BB5C98__INCLUDED_)
#define AFX_E2PROM_TOOLVIEW_H__E6BD2C0D_BAF4_4283_B59D_9A8F24BB5C98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "afxwin.h"
#include "eeprom.h"

class CE2PROM_TOOLView : public CFormView
{
protected: // create from serialization only
	CE2PROM_TOOLView();
	DECLARE_DYNCREATE(CE2PROM_TOOLView)

public:
	//{{AFX_DATA(CE2PROM_TOOLView)
	enum { IDD = IDD_E2PROM_TOOL_FORM };
	CProgressCtrl	m_progress_;
	CButton	m_button_;
	CComboBox m_combox_;
	//}}AFX_DATA
	int current_combox_index_;

// Attributes
public:
	std::list<CEdit*> edit_control_list;
private:
	std::list<CStatic*> static_control_list;
	CFont* m_static_font_;
	CFont* m_text_font_;

public:
	CE2PROM_TOOLDoc* GetDocument();
	void AddDataIntoEdits(int page_index, BOOL is_refresh_combox = FALSE);
	void SaveDataFromEdits();

	void enableWindows(bool is_enable);

// Operations
private:
	void init_gui();
	void init_combox();
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CE2PROM_TOOLView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CE2PROM_TOOLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CE2PROM_TOOLView)
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnEditchangeCombo1();
	afx_msg void OnBnClickedButton2();       
	CButton m_read_button;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
	CButton m_write_this_page;
	CButton m_read_this_page;
	CComboBox eeprom_type_combobox_;
	eeprom::__enuEEPROM_TYPE eeprom_type_;
	int eeprom_type_combobox_index_;
	afx_msg void OnCbnSelchangeCombo2();
};

#ifndef _DEBUG  // debug version in E2PROM_TOOLView.cpp
inline CE2PROM_TOOLDoc* CE2PROM_TOOLView::GetDocument()
   { return (CE2PROM_TOOLDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_E2PROM_TOOLVIEW_H__E6BD2C0D_BAF4_4283_B59D_9A8F24BB5C98__INCLUDED_)
