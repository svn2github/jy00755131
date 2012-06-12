#if !defined(AFX_FILE_OPRT_H__55157DD8_0AC2_429F_90B7_96575E7DF102__INCLUDED_)
#define AFX_FILE_OPRT_H__55157DD8_0AC2_429F_90B7_96575E7DF102__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class data_text_file : public CStdioFile
{
	
protected: // create from serialization only
	DECLARE_DYNCREATE(data_text_file)
// Attributes
public:
	
// Operations
public:
	BOOL Open( LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError = NULL );
	int parse();
	std::list<CString>* get_data_list(){ return &m_data_;};
// Implementation
public:
	data_text_file();
	data_text_file( LPCTSTR lpszFileName, UINT nOpenFlags);
	~data_text_file();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	std::list<CString> m_data_;
	CString m_file_name_;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILE_OPRT_H__55157DD8_0AC2_429F_90B7_96575E7DF102__INCLUDED_)
