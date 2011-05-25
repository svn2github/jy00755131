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
	EnableColumnAutoSize(TRUE);
	EnableDragHeaderItems(FALSE);
	//SetReadOnly ();
	//SetSingleSel(TRUE);
	//SetWholeRowSel(TRUE);
	EnableDragSelection(FALSE);

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
	EnableColumnAutoSize(TRUE);
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
* @note  来自测线程序   
* @param CBCGPGridCtrl* pGridCtrlEdit 输入行
* @param CBCGPGridCtrl* pGridCtrlList 列表
* @param CList<int, int>* ListNb Nb号队列指针
* @return void
*/
void CGridCtrlOperation::OnBnClickedButtonAddA(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList)
{
	CBCGPGridRow* pRowEdit = NULL;	// 输入行
	CBCGPGridRow* pRowNew = NULL;	// 新行
	int iRowID = 0;
	CList<int, int>  ListNb;
	// 得到索引队列，增加操作
	if(false == GetIndexListForAdd(pGridCtrlEdit, &ListNb))
	{
		return;
	}

	//int iSelectRowIndex = -1;	
	//pRowNew = pGridCtrlList->GetCurSel();
	//if(pRowNew)
	//	iSelectRowIndex = pRowNew->GetRowId();
	// 得到输入行
	pRowEdit = pGridCtrlEdit->GetRow(0);
	COleVariant oVariant;
	int i;
	while(FALSE == ListNb.IsEmpty())
	{
		// 得到索引号
		int iNb = ListNb.RemoveHead();
		// 索引号在列表中已经存在
		if(NULL != pGridCtrlList->FindRowByData(iNb))
		{
			continue;
		}
		pRowNew = pGridCtrlList->CreateRow(pGridCtrlList->GetColumnCount());
		pRowNew->SetData(iNb);
		pRowNew->GetItem(0)->SetValue(iNb);
		for(i = 1; i < pGridCtrlList->GetColumnCount(); i++)
		{
			oVariant = pRowEdit->GetItem(i)->GetValue();
			pRowNew->GetItem(i)->SetValue(oVariant);
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
* 得到索引队列，修改操作、删除操作、颠倒操作
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
		if(strPart == "")
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
		if(strPart == "")
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
		int iRowIndex = ListNb.RemoveHead();
		pRowChange = pGridCtrlList->GetRow(iRowIndex);
		for(int i = 1; i < pGridCtrlList->GetColumnCount(); i++)
		{
			COleVariant oVariant = pRowEdit->GetItem(i)->GetValue();
			if(oVariant.vt == VT_BSTR)
			{
				CString strData = oVariant;
				if(strData != "/")
				{
					pRowChange->GetItem(i)->SetValue(oVariant);
				}
			}
			else
			{
				pRowChange->GetItem(i)->SetValue(oVariant);
			}			
		}
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