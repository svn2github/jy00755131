// Test3_MakeApplicationDoc.h : interface of the CTest3_MakeApplicationDoc class
//


#pragma once


class CTest3_MakeApplicationDoc : public CDocument
{
protected: // create from serialization only
	CTest3_MakeApplicationDoc();
	DECLARE_DYNCREATE(CTest3_MakeApplicationDoc)

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
	virtual ~CTest3_MakeApplicationDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


