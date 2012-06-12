// TabWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "line.h"
#include "TabPage.h"


// CTabWnd

IMPLEMENT_DYNAMIC(CTabPage, CStatic)

CTabPage::CTabPage()
{

}

CTabPage::~CTabPage()
{
}

BEGIN_MESSAGE_MAP(CTabPage, CStatic)
END_MESSAGE_MAP()



// CTabWnd 消息处理程序

/**
* 响应鼠标单击列表
* @param CBCGPGridCtrl* pGridCtrlEdit 输入行
* @param CBCGPGridCtrl* pGridCtrlList 列表
* @return void
*/
void CTabPage::OnGridCtrlListLButtonDownA(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList)
{
	CBCGPGridRow* pRow = pGridCtrlList->GetCurSel();
	if(NULL != pRow)
	{
		COleVariant oVariant = pRow->GetItem(0)->GetValue();
		oVariant.ChangeType(VT_BSTR);
		pGridCtrlEdit->GetRow(0)->GetItem(0)->SetValue(oVariant);
	}
}

/**
* 响应鼠标单击双击列表
* @param CBCGPGridCtrl* pGridCtrlEdit 输入行
* @param CBCGPGridCtrl* pGridCtrlList 列表
* @return void
*/
void CTabPage::OnGridCtrlListDblClkA(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList)
{
	CBCGPGridRow* pRow = pGridCtrlList->GetCurSel();
	if(NULL != pRow)
	{
		COleVariant oVariant = pRow->GetItem(0)->GetValue();
		oVariant.ChangeType(VT_BSTR);
		pGridCtrlEdit->GetRow(0)->GetItem(0)->SetValue(oVariant);
		for(int i = 1; i < pGridCtrlList->GetColumnCount(); i++)
		{
			oVariant = pRow->GetItem(i)->GetValue();
			pGridCtrlEdit->GetRow(0)->GetItem(i)->SetValue(oVariant);
		}
	}
}

/**
* 响应鼠标单击按钮；Add
* @param CBCGPGridCtrl* pGridCtrlEdit 输入行
* @param CBCGPGridCtrl* pGridCtrlList 列表
* @param CList<int, int>* plsNb Nb号队列指针
* @return void
*/
void CTabPage::OnBnClickedButtonAddA(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList, CList<int, int>* plsNb)
{
	CBCGPGridRow* pRowEdit = NULL;	// 输入行
	CBCGPGridRow* pRowNew = NULL;	// 新行
	int iRowID = 0;

	// 得到索引队列，增加操作
	if(false == GetIndexListForAdd(pGridCtrlEdit, plsNb))
	{
		return;
	}
	// 得到列表当前选中行索引
	int iSelectRowIndex = GetGridListSelectRowID(pGridCtrlList);
	// 得到输入行
	pRowEdit = pGridCtrlEdit->GetRow(0);

	while(FALSE == plsNb->IsEmpty())
	{
		// 得到索引号
		int iNb = plsNb->RemoveHead();
		// 索引号在列表中已经存在
		if(NULL != pGridCtrlList->FindRowByData(iNb))
		{
			continue;
		}
		pRowNew = pGridCtrlList->CreateRow(pGridCtrlList->GetColumnCount());
		pRowNew->SetData(iNb);
		pRowNew->GetItem(0)->SetValue(iNb);
		for(int i = 1; i < pGridCtrlList->GetColumnCount(); i++)
		{
			COleVariant oVariant = pRowEdit->GetItem(i)->GetValue();
			pRowNew->GetItem(i)->SetValue(oVariant);
		}
		if(iSelectRowIndex == -1)	// 无选中行
		{
			pGridCtrlList->AddRow(pRowNew, FALSE);
		}
		else
		{
			pGridCtrlList->InsertRowAfter(iSelectRowIndex + iRowID, pRowNew, FALSE);
			iRowID++;
		}
	}
	pGridCtrlList->AdjustLayout();
}

/**
* 响应鼠标单击按钮；Change
* @param CBCGPGridCtrl* pGridCtrlEdit 输入行
* @param CBCGPGridCtrl* pGridCtrlList 列表
* @param CList<int, int>* plsNb Nb号队列指针
* @return void
*/
void CTabPage::OnBnClickedButtonChangeA(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList, CList<int, int>* plsNb)
{
	CBCGPGridRow* pRowEdit = NULL;	// 输入行
	CBCGPGridRow* pRowChange = NULL;	// 修改行

	// 得到索引队列，修改操作、删除操作、颠倒操作
	if(false == GetIndexListForChangeOrDeleteOrReverse(pGridCtrlEdit, pGridCtrlList, plsNb))
	{
		return;
	}
	// 得到输入行
	pRowEdit = pGridCtrlEdit->GetRow(0);

	while(FALSE == plsNb->IsEmpty())
	{
		// 得到行索引号
		int iRowIndex = plsNb->RemoveHead();
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
* 响应鼠标单击按钮；Delete
* @param CBCGPGridCtrl* pGridCtrlEdit 输入行
* @param CBCGPGridCtrl* pGridCtrlList 列表
* @param CList<int, int>* plsNb Nb号队列指针
* @return void
*/
void CTabPage::OnBnClickedButtonDeleteA(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList, CList<int, int>* plsNb)
{
	// 得到索引队列，修改操作、删除操作、颠倒操作
	if(false == GetIndexListForChangeOrDeleteOrReverse(pGridCtrlEdit, pGridCtrlList, plsNb))
	{
		return;
	}
	while(FALSE == plsNb->IsEmpty())
	{
		// 得到行索引号
		int iRowIndex = plsNb->RemoveTail();
		pGridCtrlList->RemoveRow(iRowIndex);

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
void CTabPage::OnBnClickedButtonReverseA(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList, CList<int, int>* plsNb)
{
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

/**
* 响应鼠标单击按钮；Clear
* @param CBCGPGridCtrl* pGridCtrlList 列表
* @return void
*/
void CTabPage::OnBnClickedButtonClearA(CBCGPGridCtrl* pGridCtrlList)
{
	pGridCtrlList->RemoveAll();	
	pGridCtrlList->AdjustLayout();
}

/**
* 响应鼠标单击按钮；Add
* @param CBCGPGridCtrl* pGridCtrlEdit 输入行
* @param CBCGPGridCtrl* pGridCtrlList 列表
* @return void
*/
void CTabPage::OnBnClickedButtonAddB(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList)
{
	CBCGPGridRow* pRowEdit = NULL;	// 输入行
	CBCGPGridRow* pRowNew = NULL;	// 新行
	int iRowID = 0;

	// 得到列表当前选中行索引
	int iSelectRowIndex = GetGridListSelectRowID(pGridCtrlList);
	// 得到输入行
	pRowEdit = pGridCtrlEdit->GetRow(0);

	pRowNew = pGridCtrlList->CreateRow(pGridCtrlList->GetColumnCount());
	for(int i = 0; i < pGridCtrlList->GetColumnCount(); i++)
	{
		COleVariant oVariant = pRowEdit->GetItem(i)->GetValue();
		pRowNew->GetItem(i)->SetValue(oVariant);
	}
	if(iSelectRowIndex == -1)	// 无选中行
	{
		pGridCtrlList->AddRow(pRowNew, FALSE);
	}
	else
	{
		pGridCtrlList->InsertRowAfter(iSelectRowIndex + iRowID, pRowNew, FALSE);
	}
	pGridCtrlList->AdjustLayout();
}

/**
* 响应鼠标单击按钮；Change
* @param CBCGPGridCtrl* pGridCtrlEdit 输入行
* @param CBCGPGridCtrl* pGridCtrlList 列表
* @return void
*/
void CTabPage::OnBnClickedButtonChangeB(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList)
{
	CBCGPGridRow* pRowEdit = NULL;	// 输入行
	CBCGPGridRow* pRowChange = NULL;	// 修改行

	// 得到列表当前选中行索引
	int iSelectRowIndex = GetGridListSelectRowID(pGridCtrlList);
	if(iSelectRowIndex == -1)	// 无选中行
	{
		AfxMessageBox("Please Select A Record To Change.", MB_OK | MB_ICONSTOP);
		return;
	}

	// 得到输入行
	pRowEdit = pGridCtrlEdit->GetRow(0);
	// 得到要修改的行
	pRowChange = pGridCtrlList->GetRow(iSelectRowIndex);
	for(int i = 0; i < pGridCtrlList->GetColumnCount(); i++)
	{
		COleVariant oVariant = pRowEdit->GetItem(i)->GetValue();
		pRowChange->GetItem(i)->SetValue(oVariant);			
	}
	pGridCtrlList->AdjustLayout();
}

/**
* 响应鼠标单击按钮；Delete
* @param CBCGPGridCtrl* pGridCtrlList 列表
* @return void
*/
void CTabPage::OnBnClickedButtonDeleteB(CBCGPGridCtrl* pGridCtrlList)
{
	// 得到列表当前选中行索引
	int iSelectRowIndex = GetGridListSelectRowID(pGridCtrlList);
	if(iSelectRowIndex == -1)	// 无选中行
	{
		AfxMessageBox("Please Select A Record To Delete.", MB_OK | MB_ICONSTOP);
		return;
	}
	pGridCtrlList->RemoveRow(iSelectRowIndex);
	pGridCtrlList->AdjustLayout();
}

/**
* 得到索引框输入的索引字符串
* @param CBCGPGridCtrl* pGridCtrlEdit 输入行
* @return CString 索引字符串
*/
CString CTabPage::GetInputIndexString(CBCGPGridCtrl* pGridCtrlEdit)
{
	CBCGPGridRow* pRowEdit = pGridCtrlEdit->GetRow(0);	// 输入行
	CString strNb = pRowEdit->GetItem(0)->GetValue();	// 索引号
	return strNb;
}

/**
* 得到索引队列，增加操作
* @param CBCGPGridCtrl* pGridCtrlEdit 输入行
* @param CList<int, int>* plsNb Nb号队列指针
* @return bool true：成功；false：失败
*/
bool CTabPage::GetIndexListForAdd(CBCGPGridCtrl* pGridCtrlEdit, CList<int, int>* plsNb)
{	
	plsNb->RemoveAll();	// 清空索引队列

	CString strNb = "";	// 索引号
	int iNbStart = 0;
	int iNbEnd = 0;
	int iNbStep = 1;

	// 得到输入的索引字符串
	strNb = GetInputIndexString(pGridCtrlEdit);	// 索引号
	if(strNb == "")
	{
		AfxMessageBox("Please Input Index.", MB_OK | MB_ICONSTOP);
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
		catch (CException* e)
		{
			AfxMessageBox("Please Input Step.", MB_OK | MB_ICONSTOP);
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
		catch (CException* e)
		{
			AfxMessageBox("Please Input Start Index.", MB_OK | MB_ICONSTOP);
			return false;
		}
		try
		{
			// 索引号终点
			strPart = strNb.Right(strNb.GetLength() - (iPos + 1));
			iNbEnd = CXMLDOMTool::ConvertStringToInt(strPart);
		}
		catch (CException* e)
		{
			AfxMessageBox("Please Input Stop Index.", MB_OK | MB_ICONSTOP);
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
		catch (CException* e)
		{
			AfxMessageBox("Please Input Index.", MB_OK | MB_ICONSTOP);
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
			plsNb->AddTail(i);
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
			plsNb->AddTail(i);
		}
	}

	return true;
}

/**
* 得到索引队列，修改操作、删除操作、颠倒操作
* @param CBCGPGridCtrl* pGridCtrlEdit 输入行
* @param CBCGPGridCtrl* pGridCtrlList 列表
* @param CList<int, int>* plsNb Nb号队列指针
* @return bool true：成功；false：失败
*/
bool CTabPage::GetIndexListForChangeOrDeleteOrReverse(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList, CList<int, int>* plsNb)
{	
	plsNb->RemoveAll();	// 清空索引队列

	CString strNb = "";	// 索引号
	int iNbStart = 0;
	int iNbEnd = 0;
	int iRowIndexStart = -1;
	int iRowIndexEnd = -1;
	int iRowIndexStep = 1;

	// 得到输入的索引字符串
	strNb = GetInputIndexString(pGridCtrlEdit);	// 索引号
	if(strNb == "")
	{
		AfxMessageBox("Please Input Index.", MB_OK | MB_ICONSTOP);
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
		catch (CException* e)
		{
			AfxMessageBox("Please Input Step.", MB_OK | MB_ICONSTOP);
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
					AfxMessageBox("Start Index Not Found.", MB_OK | MB_ICONSTOP);
					return false;
				}
				iRowIndexStart = pRow->GetRowId();
			}
			catch (CException* e)
			{
				AfxMessageBox("Please Input Start Index.", MB_OK | MB_ICONSTOP);
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
					AfxMessageBox("Stop Index Not Found.", MB_OK | MB_ICONSTOP);
					return false;
				}
				iRowIndexEnd = pRow->GetRowId();
			}
			catch (CException* e)
			{
				AfxMessageBox("Please Input Stop Index.", MB_OK | MB_ICONSTOP);
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
				AfxMessageBox("Start Index Not Found.", MB_OK | MB_ICONSTOP);
				return false;
			}
			iRowIndexStart = pRow->GetRowId();
			// 索引号终点
			iRowIndexEnd = iRowIndexStart;
		}
		catch (CException* e)
		{
			AfxMessageBox("Please Input Start Index.", MB_OK | MB_ICONSTOP);
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
		plsNb->AddTail(i);
	}

	return true;
}

/**
* 得到列表当前选中行索引
* @param CBCGPGridCtrl* pGridCtrlList 列表
* @return int 前选中行索引 -1：无选中行
*/
// 
int CTabPage::GetGridListSelectRowID(CBCGPGridCtrl* pGridCtrlList)
{
	int iSelectRowIndex = -1;
	CBCGPGridRow* pRow = NULL;	// 队列行

	// 得到选中行
	pRow = pGridCtrlList->GetCurSel();
	if(NULL != pRow)
	{
		iSelectRowIndex = pRow->GetRowId();
	}
	return iSelectRowIndex;
}