// Test4Doc.h : interface of the CTest4Doc class
//


#pragma once


class CTest4Doc : public CDocument
{
protected: // create from serialization only
	CTest4Doc();
	DECLARE_DYNCREATE(CTest4Doc)

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
	virtual ~CTest4Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


