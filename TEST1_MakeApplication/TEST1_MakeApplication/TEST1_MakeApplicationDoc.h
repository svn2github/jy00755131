// TEST1_MakeApplicationDoc.h : interface of the CTEST1_MakeApplicationDoc class
//


#pragma once


class CTEST1_MakeApplicationDoc : public CDocument
{
protected: // create from serialization only
	CTEST1_MakeApplicationDoc();
	DECLARE_DYNCREATE(CTEST1_MakeApplicationDoc)

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
	virtual ~CTEST1_MakeApplicationDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


