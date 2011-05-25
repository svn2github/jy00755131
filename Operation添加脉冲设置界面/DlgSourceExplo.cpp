// DlgSourceExplo.cpp : 实现文件
//

#include "stdafx.h"
#include "Operation.h"
#include "DlgSourceExplo.h"
#include "MainFrm.h"

// CDlgSourceExplo 对话框

IMPLEMENT_DYNAMIC(CDlgSourceExplo, CBCGPDialog)

CDlgSourceExplo::CDlgSourceExplo(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgSourceExplo::IDD, pParent)	
{
	EnableVisualManagerStyle();
}

CDlgSourceExplo::~CDlgSourceExplo()
{
}

void CDlgSourceExplo::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATICGRID, m_ctrlGridLocation);
	DDX_Control(pDX, IDC_STATICGRIDEDIT, m_ctrlEditGridLocation);
	
}
// DDX_Text(pDX, IDC_EDITEXPLONB, m_strSourceNb);
// DDX_Text(pDX, IDC_EDITEXPLOLABEL, m_strLabel);
// DDX_Text(pDX, IDC_EDITEXPLOSHOOTER, m_dwShooterNb);
// DDX_Text(pDX, IDC_EDITEXPLOINCR, m_lIncrNb);
// DDX_Text(pDX, IDC_EDITEXPLOCOMMENT, m_strComment);
//ON_MESSAGE(WM_SETUPGRID_DBCLICK,OnGridItemDblClk)	
BEGIN_MESSAGE_MAP(CDlgSourceExplo, CBCGPDialog)
	ON_MESSAGE(WM_SETUPGRID_CLICK,OnGridItemClick)
	ON_REGISTERED_MESSAGE(BCGM_GRID_ITEM_DBLCLICK,OnGridItemDblClk)
	ON_BN_CLICKED(IDC_BTNADD, &CDlgSourceExplo::OnBnClickedBtnadd)
	ON_BN_CLICKED(IDC_BTNDELETE, &CDlgSourceExplo::OnBnClickedBtndelete)
	ON_BN_CLICKED(IDC_BTNCHANGE, &CDlgSourceExplo::OnBnClickedBtnchange)
END_MESSAGE_MAP()


// CDlgSourceExplo 消息处理程序

BOOL CDlgSourceExplo::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rectGrid;	
	m_ctrlGridLocation.GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndListGrid.CreateGrid(rectGrid,this);
	m_wndListGrid.InsertColumn (0, _T("Nb"), 70);
	m_wndListGrid.InsertColumn (1, _T("Label"), 119);
	m_wndListGrid.InsertColumn (2, _T("Shooter Nb"), 80);
	m_wndListGrid.InsertColumn (3, _T("Incr Nb"), 80);
	m_wndListGrid.InsertColumn (4, _T("Comment"), 120);

	m_ctrlEditGridLocation.GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndEditGrid.CreateGrid(rectGrid,this);
	
	m_wndEditGrid.InsertColumn (0, _T("Nb"), 70);
	m_wndEditGrid.InsertColumn (1, _T("Label"), 119);
	m_wndEditGrid.InsertColumn (2, _T("Shooter Nb"), 80);
	m_wndEditGrid.InsertColumn (3, _T("Incr Nb"), 80);
	m_wndEditGrid.InsertColumn (4, _T("Comment"), 120);
	m_wndEditGrid.AddRow();
	m_wndEditGrid.AdjustLayout();

	Load();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
/**
 * @brief 向列表框追加单条震源记录
 * @note  
 * @param  
 * @return void
 */
void  CDlgSourceExplo::AppendRecord(CShotSource*	pShotSource)
{
	if(NULL==pShotSource)
		return ;
	_variant_t  oVariant;
	CBCGPGridRow* pRow = m_wndListGrid.CreateRow(m_wndListGrid.GetColumnCount());
	oVariant = pShotSource->m_dwSourceNb;
	// 必须要改变类型为字符串，也为在两个网格的对应条目拷贝时，类型应该一致。否则会报错
	oVariant.ChangeType(VT_BSTR);
	pRow->GetItem (0)->SetValue(oVariant);

	pRow->GetItem (1)->SetValue (LPCTSTR(pShotSource->m_strLabel));
	
	oVariant = pShotSource->m_dwShooterNb;
	oVariant.ChangeType(VT_BSTR);
	pRow->GetItem (2)->SetValue (oVariant);
	
	oVariant = pShotSource->m_lStep;
	oVariant.ChangeType(VT_BSTR);
	pRow->GetItem (3)->SetValue (oVariant);
	
	pRow->GetItem (4)->SetValue (LPCTSTR(pShotSource->m_strComment));
	
	// 设置行的属性数据为震源的编号
	pRow->SetData(pShotSource->m_dwSourceNb);
	// 增加一行记录
	m_wndListGrid.AddRow(pRow);	
	//pRow->SetData(0xFFFFFFFF);
	m_wndListGrid.AdjustLayout();
}
/**
 * @brief 向列表框追加单条震源记录
 * @note  
 * @param  
 * @return 
 */
bool  CDlgSourceExplo::ApplyRecord(int nRow,CShotSource*	pShotSource)
{
	CBCGPGridRow* pRow = m_wndListGrid.GetRow(nRow);
	if(!pRow)
		return false;
	COleVariant oVariant;
	try
	{
		// Nb
		oVariant = pRow->GetItem(0)->GetValue();
		oVariant.ChangeType(VT_UI4);
		pShotSource->m_dwSourceNb = oVariant.ulVal;
		// Label
		oVariant = pRow->GetItem(1)->GetValue();
		oVariant.ChangeType(VT_BSTR);
		pShotSource->m_strLabel = oVariant.bstrVal;
		// Shooter Nb
		oVariant = pRow->GetItem(2)->GetValue();
		oVariant.ChangeType(VT_UI4);
		pShotSource->m_dwShooterNb = oVariant.ulVal;
		// Incr Nb
		oVariant = pRow->GetItem(3)->GetValue();
		oVariant.ChangeType(VT_I4);
		pShotSource->m_lStep = oVariant.lVal;
		// Comment
		oVariant = pRow->GetItem(4)->GetValue();
		oVariant.ChangeType(VT_BSTR);
		pShotSource->m_strComment = oVariant.bstrVal;		
	}
	catch (CException* )
	{
		return false;
	}
	return true;
}
/**
 * @brief 装载所有脉冲震源对象
 * @note  
 * @param  
 * @return 
 */
void	CDlgSourceExplo::Load(void)
{
	CMainFrame*	pFrm = (CMainFrame*)AfxGetMainWnd();
	if(!pFrm)
		return;
	int				i,nCount;
	CShotSource*	pShotSource=NULL;
	nCount = pFrm->m_AllShotSrcs.GetCount();
	m_wndListGrid.RemoveAll();
	for (i=0;i<nCount;i++)
	{
		pShotSource = pFrm->m_AllShotSrcs.GetShotSource(i);
		if(pShotSource && pShotSource->m_dwShotType==SHOTTYPE_EXPLO)
		{
			AppendRecord(pShotSource);
		}
	}
}
/**
 * @brief 保存脉冲震源对象
 * @note  在设置对话框中震源对象保存到内存的震源对象表中，已有的脉冲震源的状态要保持不变 
 * @param  
 * @return 
 */
void	CDlgSourceExplo::Apply(CShotSources *pOldSources,CShotSources *pNewSources)
{
	if(!pOldSources || !pNewSources)
		return;
	CShotSourceExplo*	pShotSrc = NULL;
	CShotSource*		pOldSrc=NULL;
	int i,nCount;
	nCount = m_wndListGrid.GetRowCount();
	for(i=0;i<nCount;i++)
	{
		pShotSrc = new CShotSourceExplo;
		if(!ApplyRecord(i,pShotSrc))
		{	// ????????????
			continue;
		}
		pOldSrc = pOldSources->GetShotSourceByNb(pShotSrc->m_dwSourceNb);
		if(pOldSrc)
		{	// 如果内存中已经该对象，则状态不变
			pShotSrc->m_bySourceState = pOldSrc->m_bySourceState;
			pShotSrc->m_dwSourceIndex = pOldSrc->m_dwSourceIndex;
			pShotSrc->m_dwReadyVPNb = pOldSrc->m_dwReadyVPNb;
			pShotSrc->m_bReady = pOldSrc->m_bReady;
		}
		pNewSources->Add(pShotSrc);
	}	
}
/**
 * @brief 单击脉冲震源列表框
 * @note  
 * @param  
 * @return 
 */
LRESULT CDlgSourceExplo::OnGridItemClick(WPARAM wParam, LPARAM lParam)
{
	CGridCtrlOperation::OnGridItemClick(&m_wndEditGrid,&m_wndListGrid);	
	return 0;
}  
/**
 * @brief 双击脉冲震源列表框
 * @note  
 * @param  
 * @return 
 */
LRESULT  CDlgSourceExplo::OnGridItemDblClk(WPARAM wParam, LPARAM lParam)
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
	return 0;
}
/**
 * @brief 点击Add按钮
 * @note  
 * @param  
 * @return 
 */
void CDlgSourceExplo::OnBnClickedBtnadd()
{
	CGridCtrlOperation::OnBnClickedButtonAddA(&m_wndEditGrid,&m_wndListGrid);
}
/**
 * @brief 点击Delete按钮
 * @note  
 * @param  
 * @return 
 */
void CDlgSourceExplo::OnBnClickedBtndelete()
{	
	CGridCtrlOperation::OnBnClickedButtonDeleteA(&m_wndEditGrid,&m_wndListGrid);
}
/**
 * @brief 点击Change按钮
 * @note  
 * @param  
 * @return 
 */
void CDlgSourceExplo::OnBnClickedBtnchange()
{
	CGridCtrlOperation::OnBnClickedButtonChangeA(&m_wndEditGrid,&m_wndListGrid);
}
