#include "stdafx.h"
#include <list>
#include "E2PROM_TOOL.h"

#include "file_oprt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(data_text_file, CStdioFile)

BOOL data_text_file::Open( LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError)
{
	if(lpszFileName != NULL) 
		m_file_name_.Format("%s", lpszFileName);

	return CStdioFile::Open(lpszFileName, nOpenFlags);
}

data_text_file::data_text_file( LPCTSTR lpszFileName, UINT nOpenFlags) : CStdioFile(lpszFileName, nOpenFlags)
{
	if(lpszFileName != NULL) 
		m_file_name_.Format("%s", lpszFileName);
}

data_text_file::data_text_file()
{
}

data_text_file::~data_text_file()
{
}

int data_text_file::parse()
{
	int result = 0;
	DWORD cur_size = 0;
	DWORD file_size = 0;

	CString line;

	if(m_hFile == CFile::hFileNull){
		AfxMessageBox("必须打开文件后，才能分析文件格式，请重新打开文件！",MB_ICONERROR);
	}
	
	ReadString(line);
	line.TrimLeft();
	line.TrimRight();
	if(line.GetLength() > 0)
		m_data_.push_back(line);

	while(!feof(m_pStream)){
		ReadString(line);
		line.TrimLeft();
		line.TrimRight();
		if(line.GetLength() > 0){
			m_data_.push_back(line);
		}
	}

	return result;
}

#ifdef _DEBUG
void data_text_file::AssertValid() const
{
	CStdioFile::AssertValid();
}

void data_text_file::Dump(CDumpContext& dc) const
{
	CStdioFile::Dump(dc);
}
#endif

