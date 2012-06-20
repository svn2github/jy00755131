// afxthreadDoc.h : interface of the CafxthreadDoc class
//


#pragma once


class CafxthreadDoc : public CDocument
{
protected: // create from serialization only
	CafxthreadDoc();
	DECLARE_DYNCREATE(CafxthreadDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CafxthreadDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


