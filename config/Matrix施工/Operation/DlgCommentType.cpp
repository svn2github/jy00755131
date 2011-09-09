// DlgCommentType.cpp : 实现文件
//

#include "stdafx.h"
#include "Operation.h"
#include "DlgCommentType.h"
#include "MainFrm.h"
#include "OperationParseXML.h"
// CDlgCommentType 对话框

IMPLEMENT_DYNAMIC(CDlgCommentType, CBCGPDialog)

CDlgCommentType::CDlgCommentType(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgCommentType::IDD, pParent)
	, m_strComment(_T(""))
{
	EnableVisualManagerStyle();
}

CDlgCommentType::~CDlgCommentType()
{
}

void CDlgCommentType::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATICGRIDEDIT, m_ctrlEditGridLocation);
	DDX_Control(pDX, IDC_STATICGRIDLIST, m_ctrlListGridLocation);
	DDX_Text(pDX, IDC_EDITCOMMENT, m_strComment);
}


BEGIN_MESSAGE_MAP(CDlgCommentType, CBCGPDialog)
	ON_MESSAGE(WM_SETUPGRID_CLICK,OnGridItemClick)
	ON_REGISTERED_MESSAGE(BCGM_GRID_ITEM_DBLCLICK,OnGridItemDblClk)
	ON_BN_CLICKED(IDC_BTNADD, &CDlgCommentType::OnBnClickedBtnadd)
	ON_BN_CLICKED(IDC_BTNCHANGE, &CDlgCommentType::OnBnClickedBtnchange)
	ON_BN_CLICKED(IDC_BTNDELETE, &CDlgCommentType::OnBnClickedBtndelete)
	ON_BN_CLICKED(ID_BTNAPPLY, &CDlgCommentType::OnBnClickedBtnapply)
	ON_BN_CLICKED(ID_BTNRESET, &CDlgCommentType::OnBnClickedBtnreset)
END_MESSAGE_MAP()


// CDlgCommentType 消息处理程序

BOOL CDlgCommentType::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rectGrid;	
	m_ctrlEditGridLocation.GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndEditGrid.CreateGrid(rectGrid,this);
	m_wndEditGrid.InsertColumn (0, _T("Nb"), rectGrid.Width()*60/265);
	m_wndEditGrid.InsertColumn (1, _T("Label"), rectGrid.Width()*190/265);	
	m_wndEditGrid.InsertColumn (2, _T("Comment"), 0);
	m_wndEditGrid.SetColumnVisible(2,FALSE);
	m_wndEditGrid.AddRow();
	m_wndEditGrid.AdjustLayout();
	m_ctrlListGridLocation.GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndListGrid.CreateGrid(rectGrid,this);
	m_wndListGrid.InsertColumn (0, _T("Nb"), m_wndEditGrid.GetColumnWidth(0));
	m_wndListGrid.InsertColumn (1, _T("Label"),m_wndEditGrid.GetColumnWidth(1));
	m_wndListGrid.InsertColumn (2, _T("Comment"), 0);
	m_wndListGrid.SetColumnVisible(2,FALSE);

	// 装载内存中施工注释表
	Load();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

/**
 * @brief 向列表框追加单条记录
 * @note  
 * @param  
 * @return void
 */
void  CDlgCommentType::AppendRecord(CShotComment*	pComment)
{
	if(NULL==pComment)
		return ;
	_variant_t  oVariant;
	CBCGPGridRow* pRow = m_wndListGrid.CreateRow(m_wndListGrid.GetColumnCount());
	oVariant = pComment->m_dwCommentNb;	
	oVariant.ChangeType(VT_BSTR);// 必须要改变类型为字符串，也为在两个网格的对应条目拷贝时，类型应该一致。否则会报错
	pRow->GetItem (0)->SetValue(oVariant);

	oVariant = pComment->m_strLabel;
	oVariant.ChangeType(VT_BSTR);
	pRow->GetItem (1)->SetValue (oVariant);

	oVariant = pComment->m_strComment;
	oVariant.ChangeType(VT_BSTR);
	pRow->GetItem (2)->SetValue (oVariant);

	// 设置行的属性数据为震源的编号
	pRow->SetData(pComment->m_dwCommentNb);
	// 增加一行记录
	m_wndListGrid.AddRow(pRow);		
	m_wndListGrid.AdjustLayout();
}
/**
 * @brief 从列表框解析单条记录
 * @note  
 * @param  
 * @return void
 */
bool  CDlgCommentType::ApplyRecord(int nRow,CShotComment*	pComment)
{
	CBCGPGridRow* pRow = m_wndListGrid.GetRow(nRow);
	if(!pRow)
		return false;
	COleVariant oVariant;
	CString		strTemp;
	try
	{
		// Nb
		oVariant=pRow->GetItem (0)->GetValue();	
		oVariant.ChangeType(VT_UI4);
		pComment->m_dwCommentNb = oVariant.ulVal;	
		// label
		oVariant = pRow->GetItem(1)->GetValue();
		oVariant.ChangeType(VT_BSTR);
		pComment->m_strLabel= oVariant.bstrVal;
		// Comments
		oVariant = pRow->GetItem(2)->GetValue();
		oVariant.ChangeType(VT_BSTR);
		pComment->m_strComment= oVariant.bstrVal;

	}
	catch (CException* )
	{
		return false;
	}
	return true;
}
/**
 * @brief 从内存加载所有的施工注释记录
 * @note  
 * @param  
 * @return void
 */
void    CDlgCommentType::Load(void)
{
	CMainFrame*	pFrm = (CMainFrame*)AfxGetMainWnd();
	if(!pFrm)
		return;
	int				i,nCount;
	CShotComment*	pComment=NULL;
	nCount = pFrm->m_AllComments.GetCount();
	m_wndListGrid.RemoveAll();
	for (i=0;i<nCount;i++)
	{
		pComment = pFrm->m_AllComments.GetShotComment(i);
		if(pComment )
		{
			AppendRecord(pComment);
		}
	}
}
/**
 * @brief 保存施工注释表
 * @note  
 * @param  
 * @return 
 */
void	CDlgCommentType::Apply(CShotComments *pAllComments)
{
	if(!pAllComments)
		return;
	CShotComment*	pComment = NULL;	
	int i,nCount;
	nCount = m_wndListGrid.GetRowCount();
	for(i=0;i<nCount;i++)
	{
		pComment = new CShotComment;
		if(!ApplyRecord(i,pComment))
		{	
			continue;
		}
		
		pAllComments->Add(pComment);
	}	
}

/**
 * @brief 单击列表框
 * @note  
 * @param  
 * @return 
 */
LRESULT CDlgCommentType::OnGridItemClick(WPARAM wParam, LPARAM lParam)
{
	CGridCtrlOperation::OnGridItemClick(&m_wndEditGrid,&m_wndListGrid);	
	return 0;
}  
/**
 * @brief 双击列表框
 * @note  
 * @param  
 * @return 
 */
LRESULT  CDlgCommentType::OnGridItemDblClk(WPARAM wParam, LPARAM lParam)
{	
	CBCGPGridItem*	pItem = (CBCGPGridItem*)lParam;
	if(!pItem)
		return 0;
	CBCGPGridRow*   pRow = pItem->GetParentRow();
	if(!pRow)
		return 0;
	if(pRow->GetOwnerList()!=&m_wndListGrid)
		return 0;
	CGridCtrlOperation::OnGridItemDblClk(&m_wndEditGrid,&m_wndListGrid);

	m_strComment = m_wndEditGrid.GetRow(0)->GetItem(2)->GetValue();
	UpdateData(FALSE);
	return 0;
}
/**
 * @brief 点击Add按钮
 * @note  
 * @param  
 * @return 
 */
void CDlgCommentType::OnBnClickedBtnadd()
{
	UpdateData(TRUE);
	_variant_t  vt;
	vt=(LPCTSTR)m_strComment;
	m_wndEditGrid.GetRow(0)->GetItem(2)->SetValue(vt);
	if(m_wndEditGrid.IsColumnEmpty())
	{
		return;
	}
	CGridCtrlOperation::OnBnClickedButtonAddA(&m_wndEditGrid,&m_wndListGrid);
}
/**
 * @brief 点击change按钮
 * @note  
 * @param  
 * @return 
 */
void CDlgCommentType::OnBnClickedBtnchange()
{
	UpdateData(TRUE);
	_variant_t  vt;
	vt=(LPCTSTR)m_strComment;
	m_wndEditGrid.GetRow(0)->GetItem(2)->SetValue(vt);
	if(m_wndEditGrid.IsColumnEmpty())
	{
		return;
	}
	CGridCtrlOperation::OnBnClickedButtonChangeA(&m_wndEditGrid,&m_wndListGrid);
}
/**
 * @brief 点击Delete按钮
 * @note  
 * @param  
 * @return 
 */
void CDlgCommentType::OnBnClickedBtndelete()
{
	CGridCtrlOperation::OnBnClickedButtonDeleteA(&m_wndEditGrid,&m_wndListGrid);
}
/**
 * @brief 点击Apply按钮
 * @note  
 * @param  
 * @return 
 */
void CDlgCommentType::OnBnClickedBtnapply()
{
	CMainFrame*	pFrm = (CMainFrame*)AfxGetMainWnd();
	if(!pFrm)
		return;
	pFrm->m_AllComments.RemoveAll();
	Apply(&pFrm->m_AllComments);

	COperationParseXML   OperaXML;
	OperaXML.SaveComments(theApp.m_strLocalXMLFile,&pFrm->m_AllComments);
}
/**
 * @brief 点击Reset按钮
 * @note  
 * @param  
 * @return 
 */
void CDlgCommentType::OnBnClickedBtnreset()
{
	Load();
}
