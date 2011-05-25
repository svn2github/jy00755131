// TestOScopeDlg.h : header file
//

#if !defined(AFX_TESTOSCOPEDLG_H__B6D56D28_8E1A_11D2_9136_BC7DCDD30D52__INCLUDED_)
#define AFX_TESTOSCOPEDLG_H__B6D56D28_8E1A_11D2_9136_BC7DCDD30D52__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OScopeCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTestOScopeDlg dialog

class CTestOScopeDlg : public CDialog
{
// Construction
public:
  CTestOScopeDlg(CWnd* pParent = NULL);  // standard constructor

// Dialog Data
  //{{AFX_DATA(CTestOScopeDlg)
  enum { IDD = IDD_TESTOSCOPE_DIALOG };
    // NOTE: the ClassWizard will add data members here
  //}}AFX_DATA

  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTestOScopeDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:
  HICON m_hIcon;
  COScopeCtrl m_OScopeCtrl;
  BOOL        m_bStartStop;


  // Generated message map functions
  //{{AFX_MSG(CTestOScopeDlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void OnRunstop();
  afx_msg void OnTimer(UINT nIDEvent);
  virtual void OnOK();
  virtual void OnCancel();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTOSCOPEDLG_H__B6D56D28_8E1A_11D2_9136_BC7DCDD30D52__INCLUDED_)
