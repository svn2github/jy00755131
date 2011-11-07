#pragma once
#include "afxwin.h"
#include "InstrumentList.h"
#include "EepromSocket.h"
#include "Parameter.h"
#include "afxcmn.h"

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
	// 读取EEPROM时间间隔
	UINT m_uiEditReadTimer;
	// 写EEPROM时间间隔
	UINT m_uiEditWriteTimer;
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
	// 目标IP地址
	CIPAddressCtrl m_ctrlAimIP;
	// 目标端口控制变量
	CEdit m_ctrlAimPort;
	// 写入EEPROM的数据个数
	unsigned int m_uiWriteEepromNum;
	// 写入EEPROM的缓冲区
	unsigned char m_ucWriteEepromBuf[EEPROMCapacity];
	// 发送缓冲区
	unsigned char m_ucSendBuf[SndFrameSize];
	// 本机地址
	unsigned int m_uiIPSource;
	// 目标端口
	unsigned int m_uiAimPort;
	// 目标IP地址
	CString m_csAimIP;
	// 向单个仪器写入EEPROM数据的帧数
	unsigned int m_uiWriteFrameNum;
public:
	// CInstrumentList类指针
	CInstrumentList* m_pInstrumentList;
	// EepromSocket类成员对象
	CEepromSocket m_oEepromSocket;
private:
	// 重置
	void OnReset(void);
	// 载入程序文件
	BOOL LoadFile(CString strPath);
	// 创建EEPROM的Socket
	void OnCreateEepromSocket(void);
	// 写EEPROM
	void OnWriteEeprom(void);
	// 读EEPROM
	void OnReadEeprom(void);
	// 生成发送帧帧格式
	void OnMakeSendFrame(unsigned int uiInstrumentIP, unsigned int uiStartAddr, unsigned char* ucBuf, unsigned int uiLength, unsigned int uiOptCmd);
	// 发送0x18命令
	int ADCCommand_18(int iPos, byte* cADCSet, unsigned int uiLength);
	// 生成0x18命令查询帧
	void OnMakeCmd_18(unsigned int uiInstrumentIP);
	// 校验一个仪器的EEPROM
	void OnCheckEepromOne(unsigned int uiInstrumentSN, unsigned int uiInstrumentIP);
	// 在列表控件中寻找是否有该仪器
	void OnFindInListBox(CListBox* pListBox, unsigned int uiInstrumentSN, unsigned int uiInstrumentIP, unsigned int uiOpt);
public:
	// 关闭
	void OnClose(void);
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
	afx_msg void OnBnClickedCheckBroadcastWrite();
	afx_msg void OnBnClickedCheckContinuework();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	// 写入EEPROM的帧计数
	unsigned int m_uiWriteFrameCount;
	// 设备IP地址
	unsigned int m_uiInstrumentIP;
	// 连续工作控件选择
	int m_iCheckContinueWork;
	// 已更新设备的个数
	int m_iUpdataInstrumentCount;
	// 读取EEPROM的帧计数
	unsigned int m_uiReadFrameCount;
	// 向单个仪器读取EEPROM数据的帧数
	unsigned int m_uiReadFrameNum;
	// 设备SN
	unsigned int m_uiSN;
};
