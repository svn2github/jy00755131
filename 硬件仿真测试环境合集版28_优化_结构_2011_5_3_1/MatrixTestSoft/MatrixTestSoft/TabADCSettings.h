#pragma once
#include "afxwin.h"
#include "resource.h"

// CTabADCSettings 对话框

class CTabADCSettings : public CDialog
{
	DECLARE_DYNAMIC(CTabADCSettings)

public:
	CTabADCSettings(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTabADCSettings();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADCSETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	// ADC同步设置选项控制变量
	CComboBox m_comboSync;
	// ADC模式设置选项控制变量
	CComboBox m_comboMode;
	// ADC采样率设置选项控制变量
	CComboBox m_comboSps;
	// ADCFIR相位设置选项控制变量
	CComboBox m_comboPhs;
	// ADC数字滤波器设置选项控制变量
	CComboBox m_comboFilter;
	// ADC多路器设置选项控制变量
	CComboBox m_comboMux;
	// ADC CHOP设置选项控制变量
	CComboBox m_comboChop;
	// ADC PGA增益设置选项控制变量
	CComboBox m_comboPga;
	// HPF低位控制变量
	CEdit m_editHpfLow;
	// HPF高位控制变量
	CEdit m_editHpfHigh;
	// ADC同步设置选项
	unsigned int m_uiADCSync;
	// ADC模式设置选项
	unsigned int m_uiADCMode;
	// ADC采样率设置选项
	unsigned int m_uiADCSps;
	// ADCFIR相位设置选项
	unsigned int m_uiADCPhs;
	// ADC数字滤波器设置选项
	unsigned int m_uiADCFilter;
	// ADC多路器设置选项
	unsigned int m_uiADCMux;
	// ADC CHOP设置选项
	unsigned int m_uiADCChop;
	// ADC PGA增益设置选项
	unsigned int m_uiADCPga;
	// HPF低位控制量
	byte m_ucHpfLow;
	// HPF高位控制量
	byte m_ucHpfHigh;
};
