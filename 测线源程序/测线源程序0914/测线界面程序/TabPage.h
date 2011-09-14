#pragma once


/**
CTabPage Tab属性页类
*/
class CTabPage : public CStatic
{
	DECLARE_DYNAMIC(CTabPage)

public:
	CTabPage();
	virtual ~CTabPage();

protected:
	DECLARE_MESSAGE_MAP()

public:	// 属性

public:	// 方法
	// 响应鼠标单击列表
	virtual void OnGridCtrlListLButtonDownA(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList);
	// 响应鼠标单击双击列表
	virtual void OnGridCtrlListDblClkA(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList);

	// 响应鼠标单击按钮；Add
	virtual void OnBnClickedButtonAddA(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList, CList<int, int>* plsNb);
	// 响应鼠标单击按钮；Change
	virtual void OnBnClickedButtonChangeA(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList, CList<int, int>* plsNb);
	// 响应鼠标单击按钮；Delete
	virtual void OnBnClickedButtonDeleteA(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList, CList<int, int>* plsNb);
	// 响应鼠标单击按钮；Reverse
	virtual void OnBnClickedButtonReverseA(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList, CList<int, int>* plsNb);
	// 响应鼠标单击按钮；Clear
	virtual void OnBnClickedButtonClearA(CBCGPGridCtrl* pGridCtrlList);

	// 响应鼠标单击按钮；Add
	virtual void OnBnClickedButtonAddB(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList);
	// 响应鼠标单击按钮；Change
	virtual void OnBnClickedButtonChangeB(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList);
	// 响应鼠标单击按钮；Delete
	virtual void OnBnClickedButtonDeleteB(CBCGPGridCtrl* pGridCtrlList);

	// 得到输入的索引字符串
	virtual CString GetInputIndexString(CBCGPGridCtrl* pGridCtrlEdit);
	// 得到索引队列，增加操作
	virtual bool GetIndexListForAdd(CBCGPGridCtrl* pGridCtrlEdit, CList<int, int>* plsNb);
	// 得到索引队列，修改操作、删除操作、颠倒操作
	virtual bool GetIndexListForChangeOrDeleteOrReverse(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList, CList<int, int>* plsNb);
	// 得到列表当前选中行索引
	virtual int GetGridListSelectRowID(CBCGPGridCtrl* pGridCtrlList);
};
