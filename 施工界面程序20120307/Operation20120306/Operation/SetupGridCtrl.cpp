// SetupGridCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "Operation.h"
#include "SetupGridCtrl.h"


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
// CGridCtrlEdit
IMPLEMENT_DYNAMIC(CGridCtrlEdit, CBCGPGridCtrl)

CGridCtrlEdit::CGridCtrlEdit()
{

}

CGridCtrlEdit::~CGridCtrlEdit()
{
	RemoveAll();
}


BEGIN_MESSAGE_MAP(CGridCtrlEdit, CBCGPGridCtrl)
	
END_MESSAGE_MAP()



// CGridCtrlEdit 消息处理程序
void CGridCtrlEdit::SetRowHeight()
{
	CBCGPGridCtrl::SetRowHeight();
	m_nRowHeight += 5;
	//m_nLargeRowHeight = m_nRowHeight;
	
}
/**
 * @brief 创建列表网格
 * @note  
 * @param  
 * @return 
 */
void	CGridCtrlEdit::CreateGrid(CRect& rc,CWnd* pParent)
{
	Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rc, pParent, (UINT)-2);
	// 禁止列排序
	EnableHeader (FALSE, BCGP_GRID_HEADER_HIDE_ITEMS);
	ShowVertScrollBar(FALSE);
	EnableColumnAutoSize(FALSE);
	EnableDragHeaderItems(FALSE);
	//SetReadOnly ();
	//SetSingleSel(TRUE);
	//SetWholeRowSel(TRUE);
	EnableDragSelection(FALSE);

}
/**
 * @brief 判断是否有单元格为空
 * @note  
 * @param  
 * @return 
 */
bool   CGridCtrlEdit::IsColumnEmpty(void)
{
	COleVariant  oVT;
	int i,nCount = GetColumnCount();
	for(i=0;i<nCount;i++)
	{
		oVT = GetRow(0)->GetItem(i)->GetValue();
		if(oVT.vt == VT_EMPTY)
		{
			return true;
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
// CGridCtrlList

IMPLEMENT_DYNAMIC(CGridCtrlList, CBCGPGridCtrl)

CGridCtrlList::CGridCtrlList()
{

}

CGridCtrlList::~CGridCtrlList()
{
	RemoveAll();
}


BEGIN_MESSAGE_MAP(CGridCtrlList, CBCGPGridCtrl)
	ON_WM_LBUTTONDOWN()	
END_MESSAGE_MAP()

//ON_REGISTERED_MESSAGE(BCGM_GRID_ITEM_DBLCLICK,OnGridItemDblClk)

// CGridCtrlList 消息处理程序
void CGridCtrlList::SetRowHeight()
{
	CBCGPGridCtrl::SetRowHeight();
	m_nRowHeight += 5;
	// m_nLargeRowHeight = m_nRowHeight;
}


void CGridCtrlList::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CWnd* pWnd = GetParent();
	pWnd->PostMessage(WM_SETUPGRID_CLICK, 0,0);

	CBCGPGridCtrl::OnLButtonDown(nFlags, point);
}

/**
 * @brief 创建列表网格
 * @note  
 * @param  
 * @return 
 */
void	CGridCtrlList::CreateGrid(CRect& rc,CWnd* pParent)
{
	Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER , rc, pParent, (UINT)-1);
	// 禁止列排序
	EnableHeader (FALSE, BCGP_GRID_HEADER_HIDE_ITEMS);		
	EnableColumnAutoSize(FALSE);
	EnableDragHeaderItems(FALSE);
	SetReadOnly ();
	SetSingleSel(TRUE);
	SetWholeRowSel(TRUE);
	EnableDragSelection(FALSE);
	SetScrollBarsStyle(CBCGPScrollBar::BCGP_SBSTYLE_DEFAULT);
	ShowVertScrollBar();

	BCGP_GRID_COLOR_DATA clrData;
	clrData = GetColorTheme();
	//clrData.m_clrBackground = globalData.clrWindow; //::GetSysColor(COLOR_3DFACE);
	clrData.m_EvenColors.m_clrBackground = RGB(255,255,255);
	clrData.m_OddColors.m_clrBackground = RGB(250,253,253);

	SetColorTheme(clrData);
	AdjustLayout();
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief 单击列表框中某一条目
 * @note  
 * @param  CGridCtrlEdit* pGridEdit
 * @param  CGridCtrlList* pGridList
 * @return 
 */
void  CGridCtrlOperation::OnGridItemClick(CGridCtrlEdit* pGridEdit,CGridCtrlList* pGridList)
{
	if(!pGridList || !pGridEdit)
		return;
	CBCGPGridRow* pRowList = pGridList->GetCurSel();
	if(!pRowList)
		return ;
	CBCGPGridRow* pRowEdit = pGridEdit->GetRow(0);
	if(!pRowEdit)
		return ;
	COleVariant oVariant = pRowList->GetItem(0)->GetValue();
	oVariant.ChangeType(VT_BSTR);
	pRowEdit->GetItem(0)->SetValue(oVariant);	
	
}
/**
 * @brief 双击列表框中某一条目
 * @note  
 * @param  CGridCtrlEdit* pGridEdit
 * @param  CGridCtrlList* pGridList
 * @return 
 */
void  CGridCtrlOperation::OnGridItemDblClk(CGridCtrlEdit* pGridEdit,CGridCtrlList* pGridList)
{	
	if(!pGridList || !pGridEdit)
		return;
	if(pGridEdit->GetColumnCount()!=pGridList->GetColumnCount())
		return;
	CBCGPGridRow* pRowList = pGridList->GetCurSel();
	if(!pRowList)
		return ;
	CBCGPGridRow* pRowEdit = pGridEdit->GetRow(0);
	if(!pRowEdit)
		return ;

	int i,nCount;
	nCount = pGridList->GetColumnCount();
	COleVariant oVariant = pRowList->GetItem(0)->GetValue();
	oVariant.ChangeType(VT_BSTR);
	pRowEdit->GetItem(0)->SetValue(oVariant);
	for(i=1;i<nCount;i++)
	{
		oVariant=pRowList->GetItem(i)->GetValue();
		pRowEdit->GetItem(i)->SetValue(oVariant);	
	}	
}

/**
* 得到索引队列，增加操作
* @param CBCGPGridCtrl* pGridCtrlEdit 输入行
* @param CList<int, int>* ListNb Nb号队列指针
* @return bool
*/
bool CGridCtrlOperation::GetIndexListForAdd(CBCGPGridCtrl* pGridCtrlEdit, CList<int, int>* pListNb)
{	
	pListNb->RemoveAll();	// 清空索引队列

	CString strNb = _T("");	// 索引号
	int iNbStart = 0;
	int iNbEnd = 0;
	int iNbStep = 1;

	CBCGPGridRow* pRowEdit = pGridCtrlEdit->GetRow(0);	// 输入行
	strNb = pRowEdit->GetItem(0)->GetValue();	// 索引号
	if(strNb == _T(""))
	{		
		return false;
	}

	int iPos;
	CString strPart;

	// 得到步长
	iPos = strNb.Find('/');
	if(iPos > 0)
	{
		try
		{
			strPart = strNb.Right(strNb.GetLength() - (iPos + 1));
			iNbStep = CXMLDOMTool::ConvertStringToInt(strPart);
			strNb = strNb.Left(iPos);
		}
		catch (CException* )
		{
			//AfxMessageBox("Please Input Step.", MB_OK | MB_ICONSTOP);
			return false;
		}
	}

	// 得到起始索引号
	iPos = strNb.Find('-');
	if(iPos >= 0)
	{
		try
		{
			// 索引号起点
			strPart = strNb.Left(iPos);
			iNbStart = CXMLDOMTool::ConvertStringToInt(strPart);
		}
		catch (CException* )
		{
			//AfxMessageBox("Please Input Start Index.", MB_OK | MB_ICONSTOP);
			return false;
		}
		try
		{
			// 索引号终点
			strPart = strNb.Right(strNb.GetLength() - (iPos + 1));
			iNbEnd = CXMLDOMTool::ConvertStringToInt(strPart);
		}
		catch (CException* )
		{
			//AfxMessageBox("Please Input Stop Index.", MB_OK | MB_ICONSTOP);
			return false;
		}		

	}
	else
	{
		try
		{
			// 索引号起点
			iNbStart = CXMLDOMTool::ConvertStringToInt(strNb);
			// 索引号终点
			iNbEnd = iNbStart;
		}
		catch (CException* )
		{
			//AfxMessageBox("Please Input Index.", MB_OK | MB_ICONSTOP);
			return false;
		}
	}
	// 增量
	if(iNbStep > 0)
	{
		// 确保终点不小于起点
		if(iNbStart > iNbEnd)
		{
			int iData = iNbStart;
			iNbStart = iNbEnd;
			iNbEnd = iData;
		}
		for(int i = iNbStart; i <= iNbEnd; i = i + iNbStep)
		{
			pListNb->AddTail(i);
		}
	}
	// 减量
	if(iNbStep < 0)
	{
		if(iNbStart < iNbEnd)
		{
			int iData = iNbStart;
			iNbStart = iNbEnd;
			iNbEnd = iData;
		}
		for(int i = iNbStart; i >= iNbEnd; i = i + iNbStep)
		{
			pListNb->AddTail(i);
		}
	}

	return true;
}
/**
* @brief 响应鼠标单击按钮；Add
* @note  该函数总是向列表框的后面追加记录。
* @param CBCGPGridCtrl* pGridCtrlEdit 输入行
* @param CBCGPGridCtrl* pGridCtrlList 列表
* @param CList<int, int>* ListNb Nb号队列指针
* @return void
*/
void CGridCtrlOperation::OnBnClickedButtonAddA(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList)
{	
	int				i;
	int				iNb;
	COleVariant		oEditVariant;
	CBCGPGridRow*	pRowEdit = NULL;	// 输入行
	CBCGPGridRow*	pRowNew = NULL;	// 新行
	int				iRowID = 0;
	CList<int, int>  ListNb;
	// 得到索引队列，增加操作
	if(false == GetIndexListForAdd(pGridCtrlEdit, &ListNb))
	{
		return;
	}

	pRowEdit = pGridCtrlEdit->GetRow(0);
	
	while(FALSE == ListNb.IsEmpty())
	{
		// 得到索引号
		iNb = ListNb.RemoveHead();
		// 索引号在列表中已经存在
		if(NULL != pGridCtrlList->FindRowByData(iNb))
		{
			continue;
		}
		pRowNew = pGridCtrlList->CreateRow(pGridCtrlList->GetColumnCount());
		pRowNew->SetData(iNb);
		oEditVariant.vt = VT_UI4;
		oEditVariant.ulVal = iNb;
		oEditVariant.ChangeType(VT_BSTR);
		pRowNew->GetItem(0)->SetValue(oEditVariant);
		for(i = 1; i < pGridCtrlList->GetColumnCount(); i++)
		{						
			oEditVariant= pRowEdit->GetItem(i)->GetValue();			
			pRowNew->GetItem(i)->SetValue(oEditVariant);
		}
		pGridCtrlList->AddRow(pRowNew, FALSE);

		/*
		if(iSelectRowIndex == -1)	// 无选中行
		{
			pGridCtrlList->AddRow(pRowNew, FALSE);
		}
		else
		{
			pGridCtrlList->InsertRowAfter(iSelectRowIndex + iRowID, pRowNew, FALSE);
			iRowID++;
		}*/
	}
	pGridCtrlList->AdjustLayout();
}
/**
* @brief 响应鼠标单击按钮；Add
* @note  该函数与 OnBnClickedButtonAddA不同在于，A不能够对除Nb之外的文本框采用 V/i的形式；
而 本函数OnBnClickedButtonAddB调用GetValueForTextField函数，能够对每一列进行V/i形式的解析。

该函数总是向列表框的后面追加记录。

* @param CBCGPGridCtrl* pGridCtrlEdit 输入行
* @param CBCGPGridCtrl* pGridCtrlList 列表
* @param CList<int, int>* ListNb Nb号队列指针
* @return void
*/
void CGridCtrlOperation::OnBnClickedButtonAddB(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList)
{	
	int				i;
	int				iNb;
	COleVariant		oEditVariant,oListVariant;
	CBCGPGridRow*	pRowEdit = NULL;	// 输入行
	CBCGPGridRow*	pRowNew = NULL;	// 新行
	int				iRowID = 0;
	CList<int, int>  ListNb;
	// 得到索引队列，增加操作
	if(false == GetIndexListForAdd(pGridCtrlEdit, &ListNb))
	{
		return;
	}	
	
	pRowEdit = pGridCtrlEdit->GetRow(0);
	int nIndex=0;
	while(FALSE == ListNb.IsEmpty())
	{
		// 得到索引号
		iNb = ListNb.RemoveHead();
		// 索引号在列表中已经存在
		if(NULL != pGridCtrlList->FindRowByData(iNb))
		{
			continue;
		}
		pRowNew = pGridCtrlList->CreateRow(pGridCtrlList->GetColumnCount());
		pRowNew->SetData(iNb);
		oEditVariant.vt = VT_UI4;
		oEditVariant.ulVal = iNb;
		oEditVariant.ChangeType(VT_BSTR);
		pRowNew->GetItem(0)->SetValue(oEditVariant);
		for(i = 1; i < pGridCtrlList->GetColumnCount(); i++)
		{						
			oEditVariant= pRowEdit->GetItem(i)->GetValue();
			oListVariant= pRowNew->GetItem(i)->GetValue();
			oListVariant = GetValueForTextField(&oEditVariant,&oListVariant,nIndex);
			oListVariant.ChangeType(VT_BSTR);
			pRowNew->GetItem(i)->SetValue(oListVariant);
		}
		pGridCtrlList->AddRow(pRowNew, FALSE);
		nIndex++;
		
	}
	pGridCtrlList->AdjustLayout();
}
/**
* @brief 得到索引队列，修改操作、删除操作、颠倒操作
* @param CBCGPGridCtrl* pGridCtrlEdit 输入行
* @param CBCGPGridCtrl* pGridCtrlList 列表
* @param CList<int, int>* pListNb Nb号队列指针
* @return bool true：成功；false：失败
*/
bool CGridCtrlOperation::GetIndexListForChangeOrDeleteOrReverse(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList, CList<int, int>* pListNb)
{	
	pListNb->RemoveAll();	// 清空索引队列

	CString strNb =_T("");	// 索引号
	int iNbStart = 0;
	int iNbEnd = 0;
	int iRowIndexStart = -1;
	int iRowIndexEnd = -1;
	int iRowIndexStep = 1;

	CBCGPGridRow* pRowEdit = pGridCtrlEdit->GetRow(0);	// 输入行
	strNb = pRowEdit->GetItem(0)->GetValue();	// 索引号
	if(strNb == _T(""))
	{		
		return false;
	}
	
	if(strNb ==_T(""))
	{
		//AfxMessageBox("Please Input Index.", MB_OK | MB_ICONSTOP);
		return false;
	}

	int iPos;
	CString strPart;

	// 得到步长
	iPos = strNb.Find('/');
	if(iPos > 0)
	{
		try
		{
			strPart = strNb.Right(strNb.GetLength() - (iPos + 1));
			iRowIndexStep = CXMLDOMTool::ConvertStringToInt(strPart);
			strNb = strNb.Left(iPos);
		}
		catch (CException* )
		{
			//AfxMessageBox("Please Input Step.", MB_OK | MB_ICONSTOP);
			return false;
		}
	}

	// 得到起始索引号
	iPos = strNb.Find('-');
	if(iPos >= 0)
	{
		// 索引号起点
		strPart = strNb.Left(iPos);
		if(strPart == _T(""))
		{
			iRowIndexStart = 0;
		}
		else
		{
			try
			{
				iNbStart = CXMLDOMTool::ConvertStringToInt(strPart);
				CBCGPGridRow* pRow = pGridCtrlList->FindRowByData(iNbStart);
				if(NULL == pRow)
				{
					//AfxMessageBox("Start Index Not Found.", MB_OK | MB_ICONSTOP);
					return false;
				}
				iRowIndexStart = pRow->GetRowId();
			}
			catch (CException* )
			{
				//AfxMessageBox("Please Input Start Index.", MB_OK | MB_ICONSTOP);
				return false;
			}
		}		
		// 索引号终点
		strPart = strNb.Right(strNb.GetLength() - (iPos + 1));
		if(strPart == _T(""))
		{
			iRowIndexEnd = pGridCtrlList->GetRowCount() - 1;
		}
		else
		{
			try
			{
				iNbEnd = CXMLDOMTool::ConvertStringToInt(strPart);
				CBCGPGridRow* pRow = pGridCtrlList->FindRowByData(iNbEnd);
				if(NULL == pRow)
				{
					//AfxMessageBox("Stop Index Not Found.", MB_OK | MB_ICONSTOP);
					return false;
				}
				iRowIndexEnd = pRow->GetRowId();
			}
			catch (CException* )
			{
				//AfxMessageBox("Please Input Stop Index.", MB_OK | MB_ICONSTOP);
				return false;
			}
		}		
	}
	else
	{
		try
		{
			// 索引号起点
			iNbStart = CXMLDOMTool::ConvertStringToInt(strNb);
			CBCGPGridRow* pRow = pGridCtrlList->FindRowByData(iNbStart);
			if(NULL == pRow)
			{
				//AfxMessageBox("Start Index Not Found.", MB_OK | MB_ICONSTOP);
				return false;
			}
			iRowIndexStart = pRow->GetRowId();
			// 索引号终点
			iRowIndexEnd = iRowIndexStart;
		}
		catch (CException* )
		{
			//AfxMessageBox("Please Input Start Index.", MB_OK | MB_ICONSTOP);
			return false;
		}
	}

	// 确保终点不小于起点
	if(iRowIndexStart > iRowIndexEnd)
	{
		int iData = iRowIndexStart;
		iRowIndexStart = iRowIndexEnd;
		iRowIndexEnd = iData;
	}

	for(int i = iRowIndexStart; i <= iRowIndexEnd; i = i + iRowIndexStep)
	{
		pListNb->AddTail(i);
	}

	return true;
}
/**
 * @brief 解析文本框的格式
 * @note	在文本框中
						/		表示列表框中当前值不会被更改。
						V		表示数值 V 将被应用到指定一行或一组行。
						V/i		或 V/+i （或 V/-i）表示从 V 开始的步进
								（或步减）值将被应用到指定一组行上。
								（+ 符号为可选）。
						/i		或 /+i （或 /-i）表示步进（或步减）
								将被应用到指定一组行的已存在数值上。  
 * @param  COleVariant* pEditVariant	编辑框的内容
 * @param  COleVariant* pListVariant	文本框的内容
 * @param  int nIndex					循环次数，如V/i的格式时，用于计算 V + nIndex*i
 * @return 
 */
COleVariant  CGridCtrlOperation::GetValueForTextField(COleVariant* pEditVariant,COleVariant* pListVariant,int nIndex)
{
	int				nPos;
	CString			strData;
	COleVariant		oResultVariant;	
	try
	{	// 格式  V ,不是字符串
		if(pEditVariant->vt!=VT_BSTR)
		{
			return *pEditVariant;
		}

		strData = *pEditVariant;
		// 格式  /
		if(!strData.CompareNoCase(_T("/")))
		{
			return *pListVariant;
		}
		// 格式  V
		nPos = strData.Find('/');
		if(nPos<0)
		{
			return *pEditVariant;
		}
		else if(nPos ==0)	// 解析格式 /i
		{
			strData.Remove('/');
			oResultVariant = strData;
			oResultVariant.ChangeType(VT_R4);
			pListVariant->ChangeType(VT_R4);
			oResultVariant.fltVal = pListVariant->fltVal + nIndex * oResultVariant.fltVal;
			return oResultVariant;
		}
		else	// 解析格式 V/i
		{
			float  fValue,fStep;			
			strData.Replace(_T("/"),_T(" "));
			_stscanf_s(LPCTSTR(strData),_T("%f%f"),&fValue,&fStep);			
			oResultVariant = fValue + nIndex * fStep;
			return oResultVariant;
		}
	}
	catch (CException* )
	{
		oResultVariant.vt = VT_EMPTY;
		
	}
	return oResultVariant;

	
}
/**
* 响应鼠标单击按钮；Delete
* @param CBCGPGridCtrl* pGridCtrlEdit 输入行
* @param CBCGPGridCtrl* pGridCtrlList 列表
* @return void
*/
void CGridCtrlOperation::OnBnClickedButtonDeleteA(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList)
{
	CList<int, int> ListNb;
	// 得到索引队列，修改操作、删除操作、颠倒操作
	if(false == GetIndexListForChangeOrDeleteOrReverse(pGridCtrlEdit, pGridCtrlList, &ListNb))
	{
		return;
	}
	while(FALSE == ListNb.IsEmpty())
	{
		// 得到行索引号
		int iRowIndex = ListNb.RemoveTail();
		pGridCtrlList->RemoveRow(iRowIndex);

	}
	pGridCtrlList->AdjustLayout();
}

/**
* 响应鼠标单击按钮；Change
* @param CBCGPGridCtrl* pGridCtrlEdit 输入行
* @param CBCGPGridCtrl* pGridCtrlList 列表
* @return void
*/
void CGridCtrlOperation::OnBnClickedButtonChangeA(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList)
{
	int				i,iRowIndex;
	COleVariant		oEditVariant ;
	//COleVariant		oListVariant ;
	CString			strData;
	CBCGPGridRow* pRowEdit = NULL;	// 输入行
	CBCGPGridRow* pRowChange = NULL;	// 修改行
	CList<int, int> ListNb;
	// 得到索引队列，修改操作、删除操作、颠倒操作
	if(false == GetIndexListForChangeOrDeleteOrReverse(pGridCtrlEdit, pGridCtrlList, &ListNb))
	{
		return;
	}
	// 得到输入行
	pRowEdit = pGridCtrlEdit->GetRow(0);


	while(FALSE == ListNb.IsEmpty())
	{
		// 得到行索引号
		iRowIndex = ListNb.RemoveHead();
		pRowChange = pGridCtrlList->GetRow(iRowIndex);
		for(i = 1; i < pGridCtrlList->GetColumnCount(); i++)
		{
			oEditVariant= pRowEdit->GetItem(i)->GetValue();			
			if(oEditVariant.vt == VT_BSTR)
			{
				strData = oEditVariant;
				if(strData != _T("/"))
				{
					pRowChange->GetItem(i)->SetValue(oEditVariant);
				}
			}
			else
			{
				pRowChange->GetItem(i)->SetValue(oEditVariant);
			}			
		}
	
	}
	pGridCtrlList->AdjustLayout();
}
/**
* @brief 响应鼠标单击按钮；Change
* @note	与 OnBnClickedButtonChangeA函数不同，该函数对每一列调用GetValueForTextField进行解析。
* 支持V/i的格式输入。
* @param CBCGPGridCtrl* pGridCtrlEdit 输入行
* @param CBCGPGridCtrl* pGridCtrlList 列表
* @return void
*/
void CGridCtrlOperation::OnBnClickedButtonChangeB(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList)
{
	int				nIndex;  // 计数器，计算当前更新的行数
	int				i,iRowIndex;
	COleVariant		oEditVariant ;
	COleVariant		oListVariant ;
	CString			strData;
	CBCGPGridRow* pRowEdit = NULL;	// 输入行
	CBCGPGridRow* pRowChange = NULL;	// 修改行
	CList<int, int> ListNb;
	// 得到索引队列，修改操作、删除操作、颠倒操作
	if(false == GetIndexListForChangeOrDeleteOrReverse(pGridCtrlEdit, pGridCtrlList, &ListNb))
	{
		return;
	}
	// 得到输入行
	pRowEdit = pGridCtrlEdit->GetRow(0);

	nIndex = 0;
	while(FALSE == ListNb.IsEmpty())
	{
		// 得到行索引号
		iRowIndex = ListNb.RemoveHead();
		pRowChange = pGridCtrlList->GetRow(iRowIndex);
		for(i = 1; i < pGridCtrlList->GetColumnCount(); i++)
		{
			oEditVariant= pRowEdit->GetItem(i)->GetValue();
			oListVariant= pRowChange->GetItem(i)->GetValue();
			oListVariant = GetValueForTextField(&oEditVariant,&oListVariant,nIndex);
			if (oEditVariant.vt == VT_BSTR)
			{
				oListVariant.ChangeType(VT_BSTR);
			}			
			pRowChange->GetItem(i)->SetValue(oListVariant);			
		}
		nIndex++;
	}
	pGridCtrlList->AdjustLayout();
}
/**
* 响应鼠标单击按钮；Reverse
* @param CBCGPGridCtrl* pGridCtrlEdit 输入行
* @param CBCGPGridCtrl* pGridCtrlList 列表
* @param CList<int, int>* plsNb Nb号队列指针
* @return void
*/
void CGridCtrlOperation::OnBnClickedButtonReverseA(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList)
{
	CList<int, int> ListNb;
	CList<int, int>* plsNb = &ListNb;
	// 得到索引队列，修改操作、删除操作、颠倒操作
	if(false == GetIndexListForChangeOrDeleteOrReverse(pGridCtrlEdit, pGridCtrlList, plsNb))
	{
		return;
	}

	CBCGPGridRow* pRow = NULL;	// 行
	CBCGPGridRow* pRowMax = NULL;	// 排序行	
	CBCGPGridRow* pRowNew = NULL;	// 新行

	CList<int, int> olsNb;
	int iNbStart = -1;
	int iNbEnd = -1;
	int iRowIndex = -1;
	int i, j;

	POSITION posMax;
	POSITION pos;
	int iNbMax;
	int iNb;

	// 得到Nb队列
	for(i = 0; i < plsNb->GetCount(); i++)
	{
		pos = plsNb->FindIndex(i);
		iRowIndex = plsNb->GetAt(pos);
		iNb = pGridCtrlList->GetRow(iRowIndex)->GetData();
		olsNb.AddTail(iNb);
		if(0 == i)
		{
			iNbStart = iNb;
		}
		if((plsNb->GetCount() - 1) == i)
		{
			iNbEnd = iNb;
		}
	}

	// 对Nb排序，由小到大
	for(i = 0; i < olsNb.GetCount(); i++)
	{
		posMax = olsNb.FindIndex(i);
		iNbMax = olsNb.GetAt(posMax);
		for(j = i + 1; j < olsNb.GetCount(); j++)
		{
			pos = olsNb.FindIndex(j);
			iNb = olsNb.GetAt(pos);

			if(iNbStart < iNbEnd)
			{
				if(iNbMax > iNb)
				{
					iNbMax = iNb;
					posMax = pos;
				}
			}
			else
			{
				if(iNbMax < iNb)
				{
					iNbMax = iNb;
					posMax = pos;
				}
			}

		}
		olsNb.RemoveAt(posMax);
		olsNb.AddHead(iNbMax);
	}

	for(i = 0; i < plsNb->GetCount(); i++)
	{
		pos = plsNb->FindIndex(i);
		iRowIndex = plsNb->GetAt(pos);
		pRow = pGridCtrlList->GetRow(iRowIndex);

		posMax = olsNb.FindIndex(i);
		iNbMax = olsNb.GetAt(posMax);
		pRowMax = pGridCtrlList->FindRowByData(iNbMax);

		pRowNew = pGridCtrlList->CreateRow(pGridCtrlList->GetColumnCount());
		pGridCtrlList->AddRow(pRowNew, FALSE);

		pRowNew->SetData(pRow->GetData());
		pRowNew->GetItem(0)->SetValue(pRow->GetItem(0)->GetValue());
		for(int j = 1; j < pGridCtrlList->GetColumnCount(); j++)
		{
			COleVariant oVariant = pRow->GetItem(j)->GetValue();
			pRowNew->GetItem(j)->SetValue(oVariant);
		}

		pRow->SetData(pRowMax->GetData());
		pRow->GetItem(0)->SetValue(pRowMax->GetItem(0)->GetValue());
		for(int j = 1; j < pGridCtrlList->GetColumnCount(); j++)
		{
			COleVariant oVariant = pRowMax->GetItem(j)->GetValue();
			pRow->GetItem(j)->SetValue(oVariant);
		}

		pRowMax->SetData(pRowNew->GetData());
		pRowMax->GetItem(0)->SetValue(pRowNew->GetItem(0)->GetValue());
		for(int j = 1; j < pGridCtrlList->GetColumnCount(); j++)
		{
			COleVariant oVariant = pRowNew->GetItem(j)->GetValue();
			pRowMax->GetItem(j)->SetValue(oVariant);
		}
		pGridCtrlList->RemoveRow((pGridCtrlList->GetRowCount() - 1), FALSE);
	}
	pGridCtrlList->AdjustLayout();
}