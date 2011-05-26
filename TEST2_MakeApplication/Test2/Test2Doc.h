// Test2Doc.h : interface of the CTest2Doc class
//


#pragma once


class CTest2Doc : public CDocument
{
protected: // create from serialization only
	CTest2Doc();
	DECLARE_DYNCREATE(CTest2Doc)

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
	virtual ~CTest2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


