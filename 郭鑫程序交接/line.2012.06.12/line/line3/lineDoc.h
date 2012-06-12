// line3Doc.h : interface of the Cline3Doc class
//


#pragma once


class CLineDoc : public CDocument
{
protected: // create from serialization only
	CLineDoc();
	DECLARE_DYNCREATE(CLineDoc)

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
	virtual ~CLineDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


