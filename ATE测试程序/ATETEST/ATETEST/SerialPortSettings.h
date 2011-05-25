#pragma once
#include "afxwin.h"


// CSerialPortSettings 对话框

class CSerialPortSettings : public CDialog
{
	DECLARE_DYNAMIC(CSerialPortSettings)

public:
	CSerialPortSettings(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSerialPortSettings();

// 对话框数据
	enum { IDD = IDD_DIALOG_SERIALPORT_SETTINGS };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 控件IDC_COMBO_SERIALPORT_DEVICE的变量
	CComboBox m_comboSerialPortDevice;

	// 控件IDC_COMBO_SERIALPORT_COM的变量
	CComboBox m_comboSerialPortCom;

	// 控件IDC_COMBO_SERIALPORT_BAUD的变量
	CComboBox m_comboSerialPortBaud;
	// // 控件IDC_COMBO_SERIALPORT_PARITY的变量
	CComboBox m_comboSerialPortParity;
	// // 控件IDC_COMBO_SERIALPORT_BYTESIZE的变量
	CComboBox m_comboSerialPortBytesize;
	// // 控件IDC_COMBO_SERIALPORT_STOPBITS的变量
	CComboBox m_comboSerialPortStopbits;
};
