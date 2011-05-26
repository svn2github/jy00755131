// Test5Doc.h : interface of the CTest5Doc class
//


#pragma once


class CTest5Doc : public CDocument
{
protected: // create from serialization only
	CTest5Doc();
	DECLARE_DYNCREATE(CTest5Doc)

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
	virtual ~CTest5Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


