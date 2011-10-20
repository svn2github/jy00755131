#pragma once
#include "afxwin.h"
#include "InstrumentList.h"

// CTabEepromUpdata 对话框

class CTabEepromUpdata : public CDialog
{
	DECLARE_DYNAMIC(CTabEepromUpdata)

public:
	CTabEepromUpdata(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTabEepromUpdata();

// 对话框数据
	enum { IDD = IDD_DIALOG_EEPROM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
private:
	// 每帧读取的最大字节数
	UINT m_uiEditReadMax;
	// 读取字节总数
	UINT m_uiEditReadTotal;
	// 读取EEPROM的首地址
	UINT m_uiEditReadAddr;
	// 每帧写入的最大字节数
	UINT m_uiEditWriteMax;
	// 写入的字节总数
	UINT m_uiEditWriteTotal;
	// 写入EEPROM的首地址
	UINT m_uiEditWriteAddr;
	// 未参与更新的设备列表控制变量
	CListBox m_ctrlListInstrument;
	// 参与更新的设备列表控制变量
	CListBox m_ctrlListUpdata;
	// 更新失败的设备列表控制变量
	CListBox m_ctrlListFail;
	// 更新成功的设备列表控制变量
	CListBox m_ctrlListSuccess;
	// 未参与更新的设备列表个数
	CStatic m_ctrlStaticInstrumentNum;
	// 参与更新的设备列表个数
	CStatic m_ctrlStaticUpdataNum;
	// 更新失败的设备列表个数
	CStatic m_ctrlStaticFailNum;
	// 更新成功的设备列表个数
	CStatic m_ctrlStaticSuccessNum;
	// 将设备添加到参与更新的设备列表
	CButton m_ctrlBtnMoveRightOne;
	// 将设备全部添加到参与更新的设备列表
	CButton m_ctrlBtnMoveRightAll;
	// 将设备添加到未参与更新的设备列表
	CButton m_ctrlBtnMoveLeftOne;
	// 将设备全部添加到未参与更新的设备列表
	CButton m_ctrlBtnMoveLeftAll;
	// 广播写入EEPROM的Check控件的控制变量
	CButton m_ctrlCheckBroadCastWrite;
	// 连续工作Check按钮的控制变量
	CButton m_ctrlCheckContinueWork;
	// 操作信息控制变量
	CListBox m_ctrlListMessage;
	// 更新设备列表按钮控制变量
	CButton m_ctrlBtnUpdataInstrumentList;
	// 选择要写入的文件按钮控制变量
	CButton m_ctrlBtnLoadFile;
	// 写入EEPROM按钮控制变量
	CButton m_ctrlBtnWriteEeprom;
	// 读取EEPROM按钮控制变量
	CButton m_ctrlBtnReadEeprom;
	// 开始校验按钮控制变量
	CButton m_ctrlBtnStartCheck;
	// 重置按钮控制变量
	CButton m_ctrlBtnReset;
public:
	CInstrumentList* m_pInstrumentList;
private:
	// 重置
	void OnReset(void);
public:
	afx_msg void OnBnClickedButtonUpdataInstrumentlist();
	afx_msg void OnBnClickedButtonMoverightOne();
	afx_msg void OnBnClickedButtonMoverightAll();
	afx_msg void OnBnClickedButtonMoveleftOne();
	afx_msg void OnBnClickedButtonMoveleftAll();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonLoadfile();
	afx_msg void OnBnClickedButtonWriteEeprom();
	afx_msg void OnBnClickedButtonReadEeprom();
	afx_msg void OnBnClickedButtonStartCheck();
};
