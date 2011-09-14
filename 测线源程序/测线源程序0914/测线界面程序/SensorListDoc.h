#pragma once

// CSensorListDoc 文档

class CSensorListDoc : public CDocument
{
	DECLARE_DYNCREATE(CSensorListDoc)

public:
	CSensorListDoc();
	virtual ~CSensorListDoc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // 为文档 I/O 重写
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
