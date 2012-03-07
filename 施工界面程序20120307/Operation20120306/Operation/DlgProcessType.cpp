// DlgProcessType.cpp : 实现文件
//

#include "stdafx.h"
#include "Operation.h"
#include "DlgProcessType.h"
#include "MainFrm.h"
#include "OperationParseXML.h"
// CDlgProcessType 对话框

IMPLEMENT_DYNAMIC(CDlgProcessType, CBCGPDialog)

CDlgProcessType::CDlgProcessType(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgProcessType::IDD, pParent)
	, m_ProcessSetup(PROCESS_STANDARD)
	, m_dwFDUSN(0)
	, m_dwRecordLen(0)
	, m_dwTBWindow(0)
	, m_dwRefractionDelay(0)
{
	EnableVisualManagerStyle();
}

CDlgProcessType::~CDlgProcessType()
{
}

void CDlgProcessType::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATICAUXACQ, m_wndDlgLocation);
	DDX_Control(pDX, IDC_COMBOPROCESSTYPE, m_ctrlProcessType);
	DDX_Control(pDX, IDC_STATICGRIDEDIT, m_ctrlEditGridLocation);
	DDX_Control(pDX, IDC_STATICGRIDLIST, m_ctrlListGridLocation);
	DDX_Radio(pDX, IDC_RADIOSTAND, m_ProcessSetup);
	DDX_Control(pDX, IDC_STATICREFRACTION, m_ctrlRefraction);
	DDX_Text(pDX, IDC_EDITFDUSN, m_dwFDUSN);
	DDX_Text(pDX, IDC_EDITRECORDLEN, m_dwRecordLen);
	DDX_Control(pDX, IDC_EDITTBWINDOW, m_EditTbWindow);
	DDX_Text(pDX, IDC_EDITTBWINDOW, m_dwTBWindow);
	DDX_Control(pDX, IDC_STATICTBWINDOW, m_ctrlTBWindow);
	DDX_Text(pDX, IDC_EDITREFRACTION, m_dwRefractionDelay);
}


BEGIN_MESSAGE_MAP(CDlgProcessType, CBCGPDialog)	
	ON_MESSAGE(WM_SETUPGRID_CLICK,OnGridItemClick)
	ON_REGISTERED_MESSAGE(BCGM_GRID_ITEM_DBLCLICK,OnGridItemDblClk)
	ON_BN_CLICKED(IDC_BTNADD, &CDlgProcessType::OnBnClickedBtnadd)
	ON_BN_CLICKED(IDC_BTNCHANGE, &CDlgProcessType::OnBnClickedBtnchange)
	ON_BN_CLICKED(IDC_BTNDELETE, &CDlgProcessType::OnBnClickedBtndelete)
	ON_BN_CLICKED(ID_BTNAPPLY, &CDlgProcessType::OnBnClickedBtnapply)
	ON_BN_CLICKED(ID_BTNRESET, &CDlgProcessType::OnBnClickedBtnreset)
END_MESSAGE_MAP()


// CDlgProcessType 消息处理程序

BOOL CDlgProcessType::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rectGrid;
	// 显示处理类型列表
	m_ctrlEditGridLocation.GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndEditGrid.CreateGrid(rectGrid,this);

	m_wndEditGrid.InsertColumn (0, _T("Nb"), rectGrid.Width()*60/220);
	m_wndEditGrid.InsertColumn (1, _T("Label"), rectGrid.Width()*145/220);	
	m_wndEditGrid.AddRow();
	m_wndEditGrid.AdjustLayout();

	m_ctrlListGridLocation.GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndListGrid.CreateGrid(rectGrid,this);
	m_wndListGrid.InsertColumn (0, _T("Nb"), m_wndEditGrid.GetColumnWidth(0));
	m_wndListGrid.InsertColumn (1, _T("Label"), m_wndEditGrid.GetColumnWidth(1));

	// 显示不同数据处理类型对话框
	m_wndDlgLocation.GetWindowRect (&rectGrid);	
	ScreenToClient(&rectGrid);
	rectGrid.left++;
	rectGrid.top++;
	m_dlgImpulsive.Create(IDD_PROCESSIMPULSIVEDLG,this);	
	m_dlgImpulsive.SetWindowPos(NULL,rectGrid.left,rectGrid.top,rectGrid.Width(),rectGrid.Height(),SWP_SHOWWINDOW);

	// 解析处理类型
	COperationParseXML OperaXML;
	OperaXML.ParseProcessType(theApp.m_strLocalXMLFile,&m_AllProcesses);		// ??????? 先从XML文件中读出
	Load();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
/**
 * @brief 根据处理类型显示不同的对话框
 * @note  
 * @param  int nProcessType 处理类型编号
 * @return void
 */
void    CDlgProcessType::ShowProcessDialog(int nProcessType)
{
	int nCmdShow;
	// 脉冲类型
	nCmdShow = (nProcessType==PROCESS_IMPULSIVE) ? SW_SHOW:SW_HIDE;
	m_dlgImpulsive.ShowWindow(nCmdShow);
	
	// 脉冲叠加

	// 叠加前相关

	// 叠加后相关

	// 可控震源
}
/**
 * @brief 装载脉冲处理类型
 * @note  在对话框界面中显示脉冲处理类型的属性。
 * @param  CProcessType* pProcess ，需要显示的处理类型对象
 * @return void
 */
void	CDlgProcessType::LoadImpulsive(CProcessImpulsive* pProcess)
{
	if(!pProcess)
		return;
	
	m_ProcessSetup = pProcess->m_ProcessSetup;
	m_ctrlProcessType.SetCurSel(PROCESS_IMPULSIVE);
	m_dwFDUSN = pProcess->m_dwFDUSN;
	m_dwRecordLen = pProcess->m_dwRecordLen;
	m_dwTBWindow = pProcess->m_dwTBWindow;
	m_dwRefractionDelay = pProcess->m_dwRefractionDelay;
	UpdateData(FALSE);
	m_dlgImpulsive.Load(pProcess);

}
/**
 * @brief 装载一条处理类型记录
 * @note  加载一条处理类型记录，将Nb、Label写入NbLabel列表框中
 * @param  CProcessType* pProcess 处理类型
 * @return void
 */
void	CDlgProcessType::AppendRecord(CProcessType* pProcessType)
{
	_variant_t  oVariant;
	CBCGPGridRow* pRow = m_wndListGrid.CreateRow(m_wndListGrid.GetColumnCount());

	oVariant = pProcessType->m_dwProcessNb;
	// 必须要改变类型为字符串，也为在两个网格的对应条目拷贝时，类型应该一致。否则会报错
	oVariant.ChangeType(VT_BSTR);
	pRow->GetItem (0)->SetValue(oVariant);

	oVariant = pProcessType->m_strLabel;
	oVariant.ChangeType(VT_BSTR);
	pRow->GetItem (1)->SetValue (oVariant);

	// 增加一行记录
	m_wndListGrid.AddRow(pRow);	
	// 设置行的属性数据为震源的编号
	pRow->SetData(pProcessType->m_dwProcessNb);
	m_wndListGrid.AdjustLayout();
}
/**
 * @brief 装载所有的处理类型
 * @note  
 * @param  CProcessTypes* pProcess 处理类型
 * @return void
 */
void	CDlgProcessType::LoadProcess(CProcessType* pProcess)
{
	// 根据不同处理类型装载不同的数据
	switch(pProcess->m_byProcessType)
	{
	case PROCESS_IMPULSIVE:
		LoadImpulsive((CProcessImpulsive*)pProcess);
		break;
	default:
		break;
	}
	// 依据处理类型不同，显示不同的对话框
	ShowProcessDialog(pProcess->m_byProcessType);
}
/**
 * @brief 装载脉冲处理类型
 * @note  
 * @param  CProcessType* pProcess 处理类型
 * @return void
 */
void	CDlgProcessType::Load(void)
{	
	int				i,nCount;
	CProcessType*	pProcessType=NULL;
	nCount =m_AllProcesses.GetCount();
	m_wndListGrid.RemoveAll();
	for (i=0;i<nCount;i++)
	{
		pProcessType = m_AllProcesses.GetProcessType(i);
		AppendRecord(pProcessType);		
	}
}
/**
 * @brief 将当前窗口的脉冲处理类型属性写入指定对象中
 * @note  将当前属性窗口的脉冲处理类型写入指定的对象中
 * @param  CProcessImpulsive* pProcess，脉冲处理类型
 * @return 
 */
void	CDlgProcessType::ApplyImpulsive(CProcessImpulsive* pProcess)
{
	if(!pProcess)
		return;
	UpdateData(TRUE);
	// 设置类型：高级或者标准
	pProcess->m_ProcessSetup = m_ProcessSetup;
	pProcess->m_byProcessType = (BYTE)m_ctrlProcessType.GetCurSel();	
	// 爆炸机所连的采集站信息
	pProcess->m_dwFDUSN = m_dwFDUSN;
	// 采样长度
	pProcess->m_dwRecordLen =m_dwRecordLen;
	// TB窗口
	pProcess->m_dwTBWindow = m_dwTBWindow;
	// 反射时间
	pProcess->m_dwRefractionDelay = m_dwRefractionDelay;
	// 保存脉冲辅助道信息
	m_dlgImpulsive.Apply(pProcess);

}
/**
 * @brief 生成一个新的处理类型
 * @note  此函数被处理类型Nb、Label网格的操作按钮Add和change响应函数调用。
 用于更新一条数据处理类型
 * @param int nListRow , 处理类型Nb。Label列表框中某一行 
 * @return 
 */
CProcessType*	CDlgProcessType::ApplyRecord(int nListRow)
{
	CBCGPGridRow* pRow = m_wndListGrid.GetRow(nListRow);
	if(!pRow)
		return false;
	COleVariant oVariant;
	CString		strTemp;
	
	UpdateData(TRUE);
	CProcessType *pProcessType =NULL;
	CProcessImpulsive*  pImpulsive = NULL;
	try
	{	// 通过处理类型来生成不同对象		
		switch(m_ctrlProcessType.GetCurSel())
		{
		case PROCESS_IMPULSIVE:
			pImpulsive = new CProcessImpulsive;
			pProcessType = pImpulsive;
			ApplyImpulsive(pImpulsive);			
			break;
	//	case PROCESS_STACKIMPULSIVE:		// ???????????增加对其他处理类型的处理
	//		break;
			
		default:
			return NULL;
			break;
		}
		// Nb
		oVariant=pRow->GetItem (0)->GetValue();	
		oVariant.ChangeType(VT_UI4);
		pProcessType->m_dwProcessNb = oVariant.ulVal;
		// Label
		oVariant = pRow->GetItem(1)->GetValue();
		oVariant.ChangeType(VT_BSTR);
		pProcessType->m_strLabel= oVariant.bstrVal;				
	}
	catch (CException* )
	{
		if(pProcessType)
			delete pProcessType;
		pProcessType = NULL;
		return NULL;
	}
	return pProcessType;
}
/**
 * @brief 单击处理类型NB、Label列表框
 * @note  单击处理类型NB、Label列表框，更新单条显示记录
 * @param  
 * @return 
 */
LRESULT CDlgProcessType::OnGridItemClick(WPARAM wParam, LPARAM lParam)
{
	CGridCtrlOperation::OnGridItemClick(&m_wndEditGrid,&m_wndListGrid);	
	return 0;
}  
/**
 * @brief 双击处理类型NB、Label列表框
 * @note  用户鼠标双击处理类型定义列表框，在对话框上显示当前被双击的处理类型信息，刷新显示界面。
 * @param  
 * @return 
 */
LRESULT  CDlgProcessType::OnGridItemDblClk(WPARAM wParam, LPARAM lParam)
{	
	// 找到被双击的单元格条目指针
	CBCGPGridItem*	pItem = (CBCGPGridItem*)lParam;
	if(!pItem)
		return 0;
	CBCGPGridRow*   pRow = pItem->GetParentRow();
	if(!pRow)
		return 0;
	// 找到父窗口，判断是否为该网格对象
	if(pRow->GetOwnerList()!=&m_wndListGrid)
		return 0;
	CGridCtrlOperation::OnGridItemDblClk(&m_wndEditGrid,&m_wndListGrid);

	// 通过被双击的行获得处理类型Nb
	COleVariant oVariant;
	oVariant = pRow->GetItem(0)->GetValue();
	oVariant.ChangeType(VT_UI4);
	// 从内存中找到被双击的处理类型对象，刷新显示
	CProcessType* pProcess=m_AllProcesses.GetProcessTypeByNb(oVariant.ulVal);
	if(pProcess)
	{
		LoadProcess(pProcess);
	}
	return 0;
}
/**
 * @brief 点击Add按钮
 * @note  
 * @param  
 * @return 
 */
void CDlgProcessType::OnBnClickedBtnadd()
{
	if(m_wndEditGrid.IsColumnEmpty())
	{
		return;
	}
	// 1、检查Nb是否为数字，且与列表框中已有的数字不重复	
	CBCGPGridRow* pRow = m_wndEditGrid.GetRow(0);
	COleVariant oVariant;
	try
	{
		// Nb
		oVariant=pRow->GetItem (0)->GetValue();	
		oVariant.ChangeType(VT_UI4);		
		// 查找Nb是否存在
		if(NULL!=m_wndListGrid.FindRowByData(oVariant.ulVal))
		{
			AfxMessageBox(_T("Error,Nb should be Unique!"));
			return;
		}		
	}
	catch (CException* )
	{
		AfxMessageBox(_T("Error,Nb Can only be number!"));
		return;
	}
	// 2、将Nb添加到表格中
	CGridCtrlOperation::OnBnClickedButtonAddB(&m_wndEditGrid,&m_wndListGrid);

	// 3、将内容添加到内存中
	CProcessType* pProcessType=NULL;
	int nListRow = m_wndListGrid.GetRowCount()-1;
	pProcessType = ApplyRecord(nListRow);
	if(!pProcessType)
	{
		return;
	}
	// 4、将处理类型增加到内存中的处理类型表中
	m_AllProcesses.Add(pProcessType);	
}
/**
 * @brief 点击Change按钮
 * @note  
 * @param  
 * @return 
 */
void CDlgProcessType::OnBnClickedBtnchange()
{
	if(m_wndEditGrid.IsColumnEmpty())
	{
		return;
	}
	// 1、检查Nb是否为数字，且在列表框中已存在	
	CBCGPGridRow* pRow = m_wndEditGrid.GetRow(0);
	COleVariant oVariant;
	int nListRow;
	try
	{
		// Nb
		oVariant=pRow->GetItem (0)->GetValue();	
		oVariant.ChangeType(VT_UI4);	
		// 查找Nb是否存在
		pRow = m_wndListGrid.FindRowByData(oVariant.ulVal);
		if(NULL==pRow)
		{
			AfxMessageBox(_T("Error,Nb does not exist!"));
			return;
		}
	}
	catch (CException* )
	{
		AfxMessageBox(_T("Error,Nb Can only be number!"));
		return;
	}
	// 2、将Nb修改到表格中
	CGridCtrlOperation::OnBnClickedButtonChangeA(&m_wndEditGrid,&m_wndListGrid);
	// 3、将内容添加到内存中
	CProcessType* pProcessType=NULL;
	nListRow = pRow->GetRowId();
	pProcessType = ApplyRecord(nListRow);
	if(!pProcessType)
	{
		return;
	}
	// 4、先查找原来对象并删除原来对象，在更新处理类型表对应行的对象指针
	nListRow = m_AllProcesses.GetProcessIndexByNb(pProcessType->m_dwProcessNb);
	if(nListRow <0)
	{
		delete pProcessType;
		return;
	}
	// 删除原来对象
	delete m_AllProcesses.m_AllProcess[nListRow];
	// 更新对象
	m_AllProcesses.m_AllProcess[nListRow] = pProcessType;
	// m_AllProcesses.Add(pProcessType);
}
/**
 * @brief 点击Delete按钮
 * @note  
 * @param  
 * @return 
 */
void CDlgProcessType::OnBnClickedBtndelete()
{
	// 1、检查Nb是否为数字，且在列表框中已存在	
	CBCGPGridRow* pRow = m_wndEditGrid.GetRow(0);
	COleVariant oVariant;
	try
	{
		// Nb
		oVariant=pRow->GetItem (0)->GetValue();	
		oVariant.ChangeType(VT_UI4);		
		if(NULL==m_wndListGrid.FindRowByData(oVariant.ulVal))
		{
			AfxMessageBox(_T("Error,Nb does not exist!"));
			return;
		}

	}
	catch (CException* )
	{
		AfxMessageBox(_T("Error,Nb Can only be number!"));
		return;
	}
	// 2、删除表格内容
	CGridCtrlOperation::OnBnClickedButtonDeleteA(&m_wndEditGrid,&m_wndListGrid);
	// 3、删除对象
	int nIndex = m_AllProcesses.GetProcessIndexByNb(oVariant.ulVal);
	if(nIndex>=0)
		m_AllProcesses.m_AllProcess.RemoveAt(nIndex);

}
/**
 * @brief 点击Apply按钮
 * @note  
 * @param  
 * @return 
 */
void CDlgProcessType::OnBnClickedBtnapply()
{
	// -----------------------------------------------
	// 1、 先将当前界面上的对象数据保存
	CBCGPGridRow* pRow = m_wndListGrid.GetCurSel();
	if(pRow)
	{
		CProcessType* pProcessType=NULL;
		int nListRow = pRow->GetRowId();
		// 将当前选择行的处理类型属性保存到对象
		pProcessType = ApplyRecord(nListRow);
		if(!pProcessType)
		{
			return;
		}
		// 2、先查找原来对象并删除原来对象，在更新处理类型表对应行的对象指针
		nListRow = m_AllProcesses.GetProcessIndexByNb(pProcessType->m_dwProcessNb);
		if(nListRow <0)
		{
			delete pProcessType;
			AfxMessageBox(_T("Apply Error!Edit and Apply it again!"));
			return;
		}
		// 删除原来对象
		delete m_AllProcesses.m_AllProcess[nListRow];
		// 更新对象
		m_AllProcesses.m_AllProcess[nListRow] = pProcessType;
	}
	
	// -----------------------------------------------
	// 2、将本对话框保存的处理类型写入XML文件
	COperationParseXML OperaXML;
	OperaXML.SaveProcessType(theApp.m_strLocalXMLFile,&m_AllProcesses);
	// 调用主框架的读XML配置文件函数，写入主框架中
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	if(!pFrm)
		return;
	// 删除原来的对象
	pFrm->m_AllProcesses.RemoveAll();
	// 创建新对象
	OperaXML.ParseProcessType(theApp.m_strLocalXMLFile,&pFrm->m_AllProcesses);
}
/**
 * @brief 点击Reset按钮
 * @note  删除内存中对象，从XML文件中重新读取处理类型对象
 * @param  
 * @return 
 */
void CDlgProcessType::OnBnClickedBtnreset()
{	
	COperationParseXML OperaXML;
	m_AllProcesses.RemoveAll();
	// 解析处理类型对象
	OperaXML.ParseProcessType(theApp.m_strLocalXMLFile,&m_AllProcesses);
	Load();
}
