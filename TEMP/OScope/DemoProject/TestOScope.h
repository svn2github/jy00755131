// TestOScope.h : main header file for the TESTOSCOPE application
//

#if !defined(AFX_TESTOSCOPE_H__B6D56D26_8E1A_11D2_9136_BC7DCDD30D52__INCLUDED_)
#define AFX_TESTOSCOPE_H__B6D56D26_8E1A_11D2_9136_BC7DCDD30D52__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
  #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"    // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestOScopeApp:
// See TestOScope.cpp for the implementation of this class
//

class CTestOScopeApp : public CWinApp
{
public:
  CTestOScopeApp();

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTestOScopeApp)
  public:
  virtual BOOL InitInstance();
  //}}AFX_VIRTUAL

// Implementation

  //{{AFX_MSG(CTestOScopeApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTOSCOPE_H__B6D56D26_8E1A_11D2_9136_BC7DCDD30D52__INCLUDED_)
