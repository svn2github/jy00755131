// E2PROM_TOOLDoc.h : interface of the CE2PROM_TOOLDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_E2PROM_TOOLDOC_H__1F2415A8_D68A_4EE4_8ED2_7D0F9EE5F058__INCLUDED_)
#define AFX_E2PROM_TOOLDOC_H__1F2415A8_D68A_4EE4_8ED2_7D0F9EE5F058__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_PAGE_SIZE	32
#define MAX_NUM_PERPAGE	256

typedef struct __tagEEPROM_DATA_ITEM{
	unsigned short address_;
	unsigned char data_;
}EEPROM_DATA_ITEM;


class CE2PROM_TOOLDoc : public CDocument
{
protected: // create from serialization only
	CE2PROM_TOOLDoc();
	DECLARE_DYNCREATE(CE2PROM_TOOLDoc)

// Attributes
public:
	EEPROM_DATA_ITEM m_data_[MAX_PAGE_SIZE][MAX_NUM_PERPAGE];
	bool m_is_progressing_;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CE2PROM_TOOLDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CE2PROM_TOOLDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CE2PROM_TOOLDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_E2PROM_TOOLDOC_H__1F2415A8_D68A_4EE4_8ED2_7D0F9EE5F058__INCLUDED_)
