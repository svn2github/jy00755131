// ConfigDoc.h : interface of the CConfigDoc class
//


#pragma once


class CConfigDoc : public CDocument
{
protected: // create from serialization only
	CConfigDoc();
	DECLARE_DYNCREATE(CConfigDoc)

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
	virtual ~CConfigDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


