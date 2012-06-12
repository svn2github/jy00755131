// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "E2PROM_TOOL.h"
#include "E2PROM_TOOLdoc.h"
#include "E2PROM_TOOLview.h"
#include "MainFrm.h"

#include <fstream>
#include <list>
#include <math.h>
#include "file_oprt.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static TCHAR BASED_CODE szFilter[] = _T("EEPROM文件 (*.eeprom)|*.eeprom");


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_SAVE_MY_FILE, &CMainFrame::OnFileSave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_current_file_name_ = _T("");	
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
unsigned short CMainFrame::str_to_hex(char* buf, int buf_size)
{
	unsigned short value = 0,
		size = 0,
		tmp_val = 0;
	float base_value = 16;

	for(int i = 0; i < buf_size; ++i) {

		if(buf[i] == ' ') continue;

		if((buf[i] >= '0') && (buf[i] <= '9')) 
			tmp_val = (buf[i] - '0');
		else if((buf[i] >= 'A') && (buf[i] <= 'F')) 
			tmp_val = (buf[i] - 'A' + 10);
		else if((buf[i] >= 'a') && (buf[i] <= 'f')) 
			tmp_val = (buf[i] - 'a' + 10);

		switch(i){

			case 0:
				value += pow(base_value, 3) * tmp_val; 
				break;
			case 1:
				value += pow(base_value, 2) * tmp_val;
				break;
			case 2:
				value += 16 * tmp_val;
				break;
			case 3:
				value += tmp_val;
				break;
			default:
				break;
		}
	}
	return value;
}

void CMainFrame::OnFileOpen() 
{
	// TODO: Add your command handler code here
	int i = 0, j = 0;
	char *ptr = NULL;
	CString str;
	std::list<CEdit*>::iterator it;
	CEdit* ed = NULL;
	CE2PROM_TOOLView* pView = (CE2PROM_TOOLView*)GetActiveView();
	CE2PROM_TOOLDoc* pDoc = (CE2PROM_TOOLDoc*)GetActiveDocument();

	if(pDoc->m_is_progressing_){
		
		AfxMessageBox("当前程序正在写EEPROM， 不能进行其他操作，请稍后再试！");
		return;
	}

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	char buf[MAX_PATH]={0};


	if(!m_current_file_name_.IsEmpty()){

		int id_ok = MessageBox("是否将数据保存到原文件中？", NULL, MB_YESNO);
		if(id_ok == IDYES)
			SaveFile();
	}

	int ok = dlg.DoModal();
	if(ok != IDOK) return;

	m_current_file_name_ = dlg.GetPathName();

	if(m_current_file_name_.Find("EEPROM") == -1){
		if(m_current_file_name_.Find("eeprom") == -1){

			MessageBox("不能打开该文件，文件格式不对，请重新打开文件！", NULL, MB_OK);
			return;
		}
	}
#if defined(_DEBUG)
	printf("%s\n", m_current_file_name_);
#endif
	std::ifstream in(m_current_file_name_.GetBuffer());
	while(!in.eof()){
		
		in.getline(buf, 256);
		str = buf;
		str.Trim();
		
		memset(buf, 0x0, sizeof(buf) *sizeof(char));
		strcpy(buf, str.GetBuffer());

		if(strlen(buf) > 0){

			ptr = strstr(buf, ":");

			if(i < MAX_PAGE_SIZE && j < MAX_NUM_PERPAGE){

				pDoc->m_data_[i][j].address_ = i * MAX_NUM_PERPAGE + j;
				pDoc->m_data_[i][j].data_ = str_to_hex2(ptr+1, strlen(buf) - (ptr - buf + 1));

				if(j == 255){
					++i;
					j = 0;
				}
				else{
					++j;
				}
			}
		}
	}

	//write to edits & combox
	pView->AddDataIntoEdits(0, TRUE);
}

void CMainFrame::SaveFile()
{
	CStdioFile file;
	CString str;

	CE2PROM_TOOLDoc* pDoc = (CE2PROM_TOOLDoc*)GetActiveDocument();

	//save the edits value into data array
	((CE2PROM_TOOLView*)GetActiveView())->SaveDataFromEdits();
#if defined(_DEBUG)
	printf("%s\n", m_current_file_name_.GetBuffer());
#endif 

	file.Open(m_current_file_name_, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite);

	for(int i = 0; i < MAX_PAGE_SIZE; ++i){

		for(int j = 0; j < MAX_NUM_PERPAGE; ++j){

			str.Format("%04X:%02X\n", pDoc->m_data_[i][j].address_, pDoc->m_data_[i][j].data_);
			file.WriteString(str);
			file.Flush();
		}
	}

	file.Close();
}

void CMainFrame::OnFileSave()
{
	int id_ok = IDOK; 
	CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	CStdioFile file;
	CString str;

	CE2PROM_TOOLDoc* pDoc = (CE2PROM_TOOLDoc*)GetActiveDocument();

	if(pDoc->m_is_progressing_){

		AfxMessageBox("当前程序正在写EEPROM， 不能进行其他操作，请稍后再试！");
		return;
	}


	if(!m_current_file_name_.IsEmpty()){

		id_ok = MessageBox("原文件已经存在，是否将数据另存为新文件？", NULL, MB_YESNO);
		if(id_ok == IDYES){

			int ok = dlg.DoModal();
			if(ok != IDOK) return;
			m_current_file_name_ = dlg.GetPathName();
			m_current_file_name_ += ".EEPROM";
		}
	}
	else{

		int ok = dlg.DoModal();
		if(ok != IDOK) return;
		m_current_file_name_ = dlg.GetPathName();
		m_current_file_name_ += ".EEPROM";
	}

	SaveFile();
}
