#if !defined(AFX_FILE_OPRT_H__55157DD8_0AC2_429F_90B7_96575E7DF102__INCLUDED_)
#define AFX_FILE_OPRT_H__55157DD8_0AC2_429F_90B7_96575E7DF102__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class my_edit_control : public CEdit
{
	
protected: // create from serialization only
	DECLARE_DYNCREATE(my_edit_control)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(my_edit_control)
	//}}AFX_VIRTUAL

// Implementation
public:
	my_edit_control();
	virtual ~my_edit_control();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(my_edit_control)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILE_OPRT_H__55157DD8_0AC2_429F_90B7_96575E7DF102__INCLUDED_)
