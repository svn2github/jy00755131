// DlgProcessImpulsive.cpp : 实现文件
//

#include "stdafx.h"
#include "Operation.h"
#include "DlgProcessImpulsive.h"


// CDlgProcessImpulsive 对话框

IMPLEMENT_DYNAMIC(CDlgProcessImpulsive, CBCGPDialog)

CDlgProcessImpulsive::CDlgProcessImpulsive(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgProcessImpulsive::IDD, pParent)
	,m_pProcess(NULL)
{
	EnableVisualManagerStyle();
}

CDlgProcessImpulsive::~CDlgProcessImpulsive()
{
}

void CDlgProcessImpulsive::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATICGRIDEDIT, m_ctrlEditGridLocation);
	DDX_Control(pDX, IDC_STATICGRIDLIST, m_ctrlListGridLocation);
}


BEGIN_MESSAGE_MAP(CDlgProcessImpulsive, CBCGPDialog)
	ON_MESSAGE(WM_SETUPGRID_CLICK,OnGridItemClick)
	ON_REGISTERED_MESSAGE(BCGM_GRID_ITEM_DBLCLICK,OnGridItemDblClk)
	ON_BN_CLICKED(IDC_BTNADD, &CDlgProcessImpulsive::OnBnClickedBtnadd)
	ON_BN_CLICKED(IDC_BTNCHANGE, &CDlgProcessImpulsive::OnBnClickedBtnchange)
	ON_BN_CLICKED(IDC_BTNDELETE, &CDlgProcessImpulsive::OnBnClickedBtndelete)
END_MESSAGE_MAP()


// CDlgProcessImpulsive 消息处理程序

BOOL CDlgProcessImpulsive::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rectGrid;
	// 显示处理类型列表
	m_ctrlEditGridLocation.GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndEditGrid.CreateGrid(rectGrid,this);

	m_wndEditGrid.InsertColumn (0, _T("Nb"), rectGrid.Width()*60/430);
	m_wndEditGrid.InsertColumn (1, _T("Processing"), rectGrid.Width()*355/430);	
	m_wndEditGrid.AddRow();
	m_wndEditGrid.AdjustLayout();

	m_ctrlListGridLocation.GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndListGrid.CreateGrid(rectGrid,this);
	m_wndListGrid.InsertColumn (0, _T("Nb"), m_wndEditGrid.GetColumnWidth(0));
	m_wndListGrid.InsertColumn (1, _T("Processing"), m_wndEditGrid.GetColumnWidth(1));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
/**
 * @brief 向辅助道设置列表框追加单条辅助道记录
 * @note  
 * @param  
 * @return void
 */
void	CDlgProcessImpulsive::AppendRecord(AUXICHANNEL* pAuxiChannel)
{	
	_variant_t  oVariant;
	CBCGPGridRow* pRow = m_wndListGrid.CreateRow(m_wndListGrid.GetColumnCount());
	
	oVariant = pAuxiChannel->m_dwNb;
	// 必须要改变类型为字符串，也为在两个网格的对应条目拷贝时，类型应该一致。否则会报错
	oVariant.ChangeType(VT_BSTR);
	pRow->GetItem (0)->SetValue(oVariant);

	oVariant = pAuxiChannel->m_szProcessing;
	oVariant.ChangeType(VT_BSTR);
	pRow->GetItem (1)->SetValue (oVariant);

	// 增加一行记录
	m_wndListGrid.AddRow(pRow);	
	// 设置行的属性数据为震源的编号
	pRow->SetData(pAuxiChannel->m_dwNb);
	m_wndListGrid.AdjustLayout();
}
/**
 * @brief 从辅助道列表框读取单条辅助道记录
 * @note  
 * @param  
 * @return void
 */
bool	CDlgProcessImpulsive::ApplyRecord(int nRow,AUXICHANNEL* pAuxiChannel)
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
		pAuxiChannel->m_dwNb = oVariant.ulVal;
		// Label
		oVariant = pRow->GetItem(1)->GetValue();
		oVariant.ChangeType(VT_BSTR);
		//AuxiChannel.m_szProcessing = oVariant.bstrVal;
		_tcscpy_s(pAuxiChannel->m_szProcessing,_countof(pAuxiChannel->m_szProcessing),COLE2T(oVariant.bstrVal));
			
	}
	catch (CException* )
	{
		return false;
	}
	return true;
}
/**
 * @brief 将处理类型装载到对话框
 * @note  
 * @param  
 * @return void
 */
void	CDlgProcessImpulsive::Load(CProcessImpulsive* pProcess)
{
	if(!pProcess || pProcess->m_byProcessType!=PROCESS_IMPULSIVE)
		return;
	int				i,nCount;
	AUXICHANNEL*	pAuxiChannel=NULL;
	// 保存处理对象
	m_pProcess = pProcess;
	nCount = pProcess->m_arrAuxiChannel.GetCount();
	m_wndListGrid.RemoveAll();
	for (i=0;i<nCount;i++)
	{
		pAuxiChannel = &pProcess->m_arrAuxiChannel[i];
		AppendRecord(pAuxiChannel);		
	}
}
/**
 * @brief 保存辅助道信息
 * @note 将辅助道处理信息写入处理类型中 
 * @param  CProcessImpulsive* pProcess
 * @return void
 */
void    CDlgProcessImpulsive::Apply(CProcessImpulsive* pProcess)
{
	if(!pProcess)
		return;
	pProcess->m_arrAuxiChannel.RemoveAll();
	m_pProcess = pProcess;
	AUXICHANNEL  AuxiChannel;
	int i,nNewCount = m_wndListGrid.GetRowCount();
	for (i=0;i<nNewCount;i++)
	{
		ApplyRecord(i,&AuxiChannel);
		pProcess->m_arrAuxiChannel.Add(AuxiChannel);
	}
}

/**
 * @brief 单击辅助道列表框
 * @note  
 * @param  
 * @return 
 */
LRESULT CDlgProcessImpulsive::OnGridItemClick(WPARAM wParam, LPARAM lParam)
{
	CGridCtrlOperation::OnGridItemClick(&m_wndEditGrid,&m_wndListGrid);	
	return 0;
}  
/**
 * @brief 双击辅助道列表框
 * @note  
 * @param  
 * @return 
 */
LRESULT  CDlgProcessImpulsive::OnGridItemDblClk(WPARAM wParam, LPARAM lParam)
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
void CDlgProcessImpulsive::OnBnClickedBtnadd()
{
	if(m_wndEditGrid.IsColumnEmpty())
	{
		return;
	}
	CGridCtrlOperation::OnBnClickedButtonAddB(&m_wndEditGrid,&m_wndListGrid);
	if(m_pProcess)
		Apply(m_pProcess);
}
/**
 * @brief 点击Change按钮
 * @note  
 * @param  
 * @return 
 */
void CDlgProcessImpulsive::OnBnClickedBtnchange()
{
	if(m_wndEditGrid.IsColumnEmpty())
	{
		return;
	}
	CGridCtrlOperation::OnBnClickedButtonChangeB(&m_wndEditGrid,&m_wndListGrid);
	if(m_pProcess)
		Apply(m_pProcess);
}
/**
 * @brief 点击Delete按钮
 * @note  
 * @param  
 * @return 
 */
void CDlgProcessImpulsive::OnBnClickedBtndelete()
{
	CGridCtrlOperation::OnBnClickedButtonDeleteA(&m_wndEditGrid,&m_wndListGrid);
	if(m_pProcess)
		Apply(m_pProcess);
}