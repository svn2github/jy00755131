// Ex1_04Doc.h : interface of the CEx1_04Doc class
//


#pragma once


class CEx1_04Doc : public CDocument
{
protected: // create from serialization only
	CEx1_04Doc();
	DECLARE_DYNCREATE(CEx1_04Doc)

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
	virtual ~CEx1_04Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


