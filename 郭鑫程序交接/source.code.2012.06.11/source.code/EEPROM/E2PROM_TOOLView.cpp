// E2PROM_TOOLView.cpp : implementation of the CE2PROM_TOOLView class
//

#include "stdafx.h"
#include "E2PROM_TOOL.h"
#include <list>
#include "E2PROM_TOOLDoc.h"
#include "E2PROM_TOOLView.h"
#include <process.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef struct __tagTHREAD_FUNCTION_ARG
{
	CE2PROM_TOOLDoc* doc_;
	CE2PROM_TOOLView* view_;
	bool is_read_;
	int page_index_;//如果等于-1表示整个读写，否则为页数但不能超过0-31
}THREAD_FUNCTION_ARG, *ptTHREAD_FUNCTION_ARG;

THREAD_FUNCTION_ARG thread_arg_;

/////////////////////////////////////////////////////////////////////////////
// CE2PROM_TOOLView

IMPLEMENT_DYNCREATE(CE2PROM_TOOLView, CFormView)

BEGIN_MESSAGE_MAP(CE2PROM_TOOLView, CFormView)
	//{{AFX_MSG_MAP(CE2PROM_TOOLView)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CE2PROM_TOOLView::OnCbnSelchangeCombo1)
	ON_CBN_EDITCHANGE(IDC_COMBO1, &CE2PROM_TOOLView::OnCbnEditchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON2, &CE2PROM_TOOLView::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CE2PROM_TOOLView::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CE2PROM_TOOLView::OnBnClickedButton3)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CE2PROM_TOOLView::OnCbnSelchangeCombo2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CE2PROM_TOOLView construction/destruction

CE2PROM_TOOLView::CE2PROM_TOOLView()
	: CFormView(CE2PROM_TOOLView::IDD)
{
	//{{AFX_DATA_INIT(CE2PROM_TOOLView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	current_combox_index_ = 0;
	m_static_font_ = new CFont;
	m_static_font_->CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_MODERN, _T("黑体"));

	m_text_font_ = new CFont;
	m_text_font_->CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_MODERN, _T("Arial"));

}

CE2PROM_TOOLView::~CE2PROM_TOOLView()
{
	CEdit* ed = NULL;
	CStatic* st = NULL;

	if(m_text_font_){
		delete m_text_font_;
		m_text_font_ = NULL;
	}

	if(m_static_font_){
		delete m_static_font_;
		m_static_font_ = NULL;
	}

	for(std::list<CEdit*>::iterator it = edit_control_list.begin(); it != edit_control_list.end(); ++it){
		ed = *it;
		delete ed;
		ed = NULL;
	}

	for(std::list<CStatic*>::iterator it_c = static_control_list.begin(); it_c != static_control_list.end(); ++it_c){
		st = *it_c;
		delete st;
		st = NULL;
	}

}

void CE2PROM_TOOLView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CE2PROM_TOOLView)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress_);
	DDX_Control(pDX, IDC_BUTTON1, m_button_);
	DDX_Control(pDX, IDC_COMBO1, m_combox_);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON2, m_read_button);
	DDX_Control(pDX, IDC_BUTTON3, m_write_this_page);
	DDX_Control(pDX, IDC_BUTTON4, m_read_this_page);
	DDX_Control(pDX, IDC_COMBO2, eeprom_type_combobox_);
}

BOOL CE2PROM_TOOLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CE2PROM_TOOLView::init_gui()
{
	CString str;

	int static_edit_align = 3;

	int edit_length = 35;
	int edit_high = 20;
	int edit_haz_align = 25;
	int edit_ver_align = 10;

	int rect_left = 7;
	int rect_top = 9;

	int static_length = 35;
	int static_high = 20;
	int static_hoz_align = 25;
	int static_ver_align = 10;

	RECT edit_rc = {0,0, 0,0};
	RECT static_rc = {0,0, 0,0};

	for(int i = 0; i < 16; ++i){

		static_rc.left = 7;
		static_rc.top = 9;
		static_rc.right = 0;

		edit_rc.left = static_length + static_edit_align;
		edit_rc.top = 9;
		edit_rc.right = 0;

		//edit control
		if(edit_rc.bottom != 0){
			edit_rc.top = edit_rc.bottom + edit_ver_align;
		}
		edit_rc.bottom = edit_rc.top + edit_high;

		//static control
		if(static_rc.bottom != 0){
			static_rc.top = static_rc.bottom + static_ver_align;
		}
		static_rc.bottom = static_rc.top + static_high;

		for(int j = 0; j < 16; ++j){

			//static
			if(static_rc.right != 0)
				static_rc.left = static_rc.right + static_hoz_align;
			else
				static_rc.left = static_rc.right + 14;

			static_rc.right = static_rc.left + static_length;				

			CStatic* pStatic = new CStatic;

			str.Format("%02X", (i * 16 + j));

			pStatic->Create( str, WS_CHILD|WS_VISIBLE|SS_CENTER , static_rc, this, 1);
			pStatic->SetFont(m_static_font_);
			pStatic->ShowWindow(SW_SHOW);

			//printf("STATIC: %d-%d, CRECT: %d, %d, %d, %d\n", i,j,static_rc.left,static_rc.top, static_rc.right, static_rc.bottom);

			static_control_list.push_back(pStatic);

			//edit	
			if(edit_rc.right != 0)
				edit_rc.left = edit_rc.right + edit_haz_align;

			edit_rc.right = edit_rc.left + edit_length;				

			CEdit* pEdit = new CEdit;
			pEdit->Create( WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_CENTER,
				edit_rc, this, 1);
			pEdit->SetFont(m_text_font_);
			pEdit->ShowWindow(SW_SHOW);

			//printf("EDIT: %d-%d, CRECT: %d, %d, %d, %d\n", i,j,edit_rc.left,edit_rc.top, edit_rc.right, edit_rc.bottom);

			edit_control_list.push_back(pEdit);

		}
	}

}

void CE2PROM_TOOLView::init_combox()
{
	CString str;

	for(int i = 0; i < 32; ++i){
		
		str.Format("%d", i);
		m_combox_.AddString(str);
	}
	current_combox_index_ = 0;

	eeprom_type_combobox_.AddString("4K");
	eeprom_type_combobox_.AddString("8K");
	eeprom_type_combobox_.AddString("32K");
	eeprom_type_combobox_.AddString("64K");

	AddDataIntoEdits(0, TRUE);
}

void CE2PROM_TOOLView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	init_gui();
	init_combox();
}

/////////////////////////////////////////////////////////////////////////////
// CE2PROM_TOOLView diagnostics

#ifdef _DEBUG
void CE2PROM_TOOLView::AssertValid() const
{
	CFormView::AssertValid();
}

void CE2PROM_TOOLView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CE2PROM_TOOLDoc* CE2PROM_TOOLView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CE2PROM_TOOLDoc)));
	return (CE2PROM_TOOLDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CE2PROM_TOOLView message handlers
//CStdioFile file;
//CString str;

//file.Open(_T("c:\\test.txt"), CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite);
//for(int i = 0; i < 8192; ++i){

//	str.Format("%04X:11\n", i);
//	file.WriteString(str);
//	file.Flush();
//}

//file.Close();

unsigned __stdcall thread_function(void* pArguments)
{
	int x = 0, y = 0, i = 0;
	int size = MAX_NUM_PERPAGE * MAX_PAGE_SIZE;
	ptTHREAD_FUNCTION_ARG arg = (ptTHREAD_FUNCTION_ARG)pArguments;
	eeprom eeprom(eeprom::DEFAULT_PORT_ADDRESS, eeprom::DEFAULT_DEVICE_ADDRESS, arg->view_->eeprom_type_);

	eeprom::uint8_t addr[2] = {0};
	eeprom::uint8_t data = 0;
	eeprom::uint8_t data_ary[16] = {0};

	printf("Disable window...\n");
	arg->view_->enableWindows(false);

	printf("init EEPROM...\n");
	eeprom.init();

	char lpbuf[128] = {0};

	arg->doc_->m_is_progressing_ = true;

	if(arg->page_index_ == -1){	

		arg->view_->m_progress_.SetRange(0, 1000);
		arg->view_->m_progress_.SetStep(2);
		i = 0;
		if(!arg->is_read_) size = MAX_PAGE_SIZE;
	}
	else{

		arg->view_->m_progress_.SetRange(0, 100);
		arg->view_->m_progress_.SetStep(2);
		if(!arg->is_read_){
			i = arg->page_index_;
			size = arg->page_index_ + 1;
		}
		else{
			i = arg->page_index_ * 256;
			size = 256 * (arg->page_index_ + 1);
		}
	}

	if(arg->is_read_){
		
		for(; i < size; ++i){

			x = i/256;
			y = i%256;
			memcpy(addr, &arg->doc_->m_data_[x][y].address_, sizeof(addr));
			arg->doc_->m_data_[x][y].data_ = eeprom.read_random_address_byte(addr);

			if(arg->page_index_ == -1)
				arg->view_->m_progress_.SetPos(i/8);
			else
				arg->view_->m_progress_.SetPos(i/2);
		}
	}
	else{
		
		while(i < size){

			y = 0;
			while(y < MAX_NUM_PERPAGE){

				memcpy(addr, &arg->doc_->m_data_[i][y].address_, sizeof(addr));
				for(x = 0; x < sizeof(data_ary); ++x)
					data_ary[x] = arg->doc_->m_data_[i][y + x].data_;

				eeprom.write_block(addr, data_ary, sizeof(data_ary));
				y += sizeof(data_ary);

				if(arg->page_index_ == -1)	arg->view_->m_progress_.SetPos((i*256+y)/8);
				else arg->view_->m_progress_.SetPos(y/2);
				Sleep(2);
			}			
			++i;
		}


		//for(x = 0; x < 10; ++x){
		//	for(y = 0; y < MAX_NUM_PERPAGE; ++y){
		//				memcpy(addr, &arg->doc_->m_data_[x][y].address_, sizeof(addr));

		//				eeprom.write_byte(addr, arg->doc_->m_data_[x][y].data_);

		//				if(arg->page_index_ == -1)	arg->view_->m_progress_.SetPos((i*256+y)/8);
		//				else arg->view_->m_progress_.SetPos(y/2);
		//				Sleep(1);

		//	}
		//}
	}


	//for(; i < size; ++i){

	//	x = i/256;
	//	y = i%256;
	//	memcpy(addr, &arg->doc_->m_data_[x][y].address_, sizeof(addr));
	//	if(arg->is_read_){

	//		arg->doc_->m_data_[i/256][i%256].data_ = eeprom.read_random_address_byte(addr);
	//	}
	//	else{

	//		data = arg->doc_->m_data_[i/256][i%256].data_;
	//		eeprom.write_byte(addr, data);
	//	}

	//	if(arg->page_index_ == -1)
	//		arg->view_->m_progress_.SetPos(i/8);
	//	else
	//		arg->view_->m_progress_.SetPos(i/2);
	//}

	printf("******Release********\n");
	eeprom.release();

	arg->view_->enableWindows(true);

	if(arg->is_read_)
		arg->view_->AddDataIntoEdits(arg->view_->current_combox_index_, FALSE);

	arg->view_->m_progress_.SetPos(0);
	arg->doc_->m_is_progressing_ = false;

	_endthreadex( 0 );
	return 0;
} 

int CE2PROM_TOOLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CE2PROM_TOOLView::AddDataIntoEdits(int page_index, BOOL is_refresh_combox)
{
	int i = 0;
	CString str;
	std::list<CEdit*>::iterator it;

	CEdit* ed = NULL;
	CE2PROM_TOOLDoc* pDoc = (CE2PROM_TOOLDoc*)GetDocument();

	//write to edits
	for(it = edit_control_list.begin(), i = 0; it != edit_control_list.end(); ++it, ++i){

		ed = *it;
		str.Format("%02X", pDoc->m_data_[page_index][i].data_);
		ed->SetWindowText(str);		
	}

	//set combox
	if(is_refresh_combox)
		m_combox_.SetCurSel(page_index);
}

void CE2PROM_TOOLView::SaveDataFromEdits()
{
	int i = 0;
	int size = 0;
	CString str;
	std::list<CEdit*>::iterator it;

	CEdit* ed = NULL;
	CE2PROM_TOOLDoc* pDoc = (CE2PROM_TOOLDoc*)GetDocument();

	//write to edits
	for(it = edit_control_list.begin(), i = 0; it != edit_control_list.end(); ++it, ++i){

		ed = *it;
		ed->GetWindowText(str);
		str.Trim();

		size = str.GetLength();

		if(size == 0)
			str = "00";
		else if(size == 1)
			str = "0"+ str;
		else if(size > 2)
			str = str.Mid(0,2);

		pDoc->m_data_[current_combox_index_][i].data_ = str_to_hex2(str.GetBuffer(), str.GetLength());
	}
}

void CE2PROM_TOOLView::enableWindows(bool is_enable)
{
	std::list<CEdit*>::iterator it;

	//write to edits
	for(it = edit_control_list.begin(); it != edit_control_list.end(); ++it)
		(*it)->EnableWindow(is_enable ? TRUE : FALSE);

	m_combox_.EnableWindow(is_enable ? TRUE : FALSE);
	m_button_.EnableWindow(is_enable ? TRUE : FALSE);
	
	m_read_button.EnableWindow(is_enable ? TRUE : FALSE);
	m_write_this_page.EnableWindow(is_enable ? TRUE : FALSE);
	m_read_this_page.EnableWindow(is_enable ? TRUE : FALSE);
	
}

void CE2PROM_TOOLView::OnCbnSelchangeCombo1()
{
	
	int idx = m_combox_.GetCurSel();

	if(idx == current_combox_index_)return ;
	
	//save edits-value into data array
	SaveDataFromEdits();

	AddDataIntoEdits(idx, FALSE);
	current_combox_index_ = idx;
}

void CE2PROM_TOOLView::OnCbnEditchangeCombo1()
{

}

void CE2PROM_TOOLView::OnBnClickedButton2()
{
	HANDLE hThread;

	thread_arg_.doc_ = (CE2PROM_TOOLDoc*)GetDocument();
	thread_arg_.view_ = this;
	thread_arg_.is_read_ = true;
	thread_arg_.page_index_ = -1;

	hThread = (HANDLE)_beginthreadex( NULL, 0, &thread_function, &thread_arg_, 0, NULL );
	CloseHandle(hThread);
}

void CE2PROM_TOOLView::OnBnClickedButton4()//read a page
{
	HANDLE hThread;

	thread_arg_.doc_ = (CE2PROM_TOOLDoc*)GetDocument();
	thread_arg_.view_ = this;
	thread_arg_.is_read_ = true;
	thread_arg_.page_index_ = current_combox_index_;

	hThread = (HANDLE)_beginthreadex( NULL, 0, &thread_function, &thread_arg_, 0, NULL );
	CloseHandle(hThread);
}

void CE2PROM_TOOLView::OnBnClickedButton3()//write a page
{
	HANDLE hThread;

	thread_arg_.doc_ = (CE2PROM_TOOLDoc*)GetDocument();
	thread_arg_.view_ = this;
	thread_arg_.is_read_ = false;
	thread_arg_.page_index_ = current_combox_index_;

	SaveDataFromEdits();

	hThread = (HANDLE)_beginthreadex( NULL, 0, &thread_function, &thread_arg_, 0, NULL );
	CloseHandle(hThread);
}

void CE2PROM_TOOLView::OnButton1() 
{

	HANDLE hThread;
	unsigned threadID;

	thread_arg_.doc_ = (CE2PROM_TOOLDoc*)GetDocument();
	thread_arg_.view_ = this;
	thread_arg_.is_read_ = false;
	thread_arg_.page_index_ = -1;

	//save edits-value into data array
	SaveDataFromEdits();

	hThread = (HANDLE)_beginthreadex( NULL, 0, &thread_function, &thread_arg_, 0, &threadID );
	CloseHandle( hThread );
}

void CE2PROM_TOOLView::OnCbnSelchangeCombo2()
{
	int idx = eeprom_type_combobox_.GetCurSel();
	if(idx == eeprom_type_combobox_index_)	return;
	eeprom_type_combobox_index_ = idx;
	switch(eeprom_type_combobox_index_){
		case 0:
			eeprom_type_ = eeprom::EEPROM_4K;
			break;
		case 1:
			eeprom_type_ = eeprom::EEPROM_8K;
			break;
		case 2:
			eeprom_type_ = eeprom::EEPROM_32K;
			break;
		case 3:
			eeprom_type_ = eeprom::EEPROM_64K;
			break;
		default:	
			break;
	}
}
