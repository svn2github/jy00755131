#pragma once
#include "Parameter.h"
#include "SetupGridCtrl.h"
// CDlgProcessTypeSetup 对话框

class CDlgProcessTypeSetup : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgProcessTypeSetup)

public:
	CDlgProcessTypeSetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgProcessTypeSetup();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROCESSSETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioStandard();
	afx_msg void OnBnClickedRadioAdvanced();
	afx_msg void OnCbnSelchangeComboProcesstypeselect();
	afx_msg void OnBnClickedButtonAuxAdd();
	afx_msg void OnBnClickedButtonAuxChange();
	afx_msg void OnBnClickedButtonAuxDelete();
	afx_msg void OnBnClickedButtonAcqAdd();
	afx_msg void OnBnClickedButtonAcqChange();
	afx_msg void OnBnClickedButtonAcqDelete();
	afx_msg void OnBnClickedButtonProAdd();
	afx_msg void OnBnClickedButtonProChange();
	afx_msg void OnBnClickedButtonProDelete();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnDestroy();
public:
	// 处理类型选项
	int m_radioProcessSetup;
	// 处理类型选择
	int m_cbindexProcessType;
	// Plug选项的数值变量
	int m_cbindexPlug;
	// 设备类型选项的数值变量
	int m_cbindexBoxType;
	// 串号的数值变量
	int m_editindexSerialNb;
	// 记录时间数值变量
	int m_editindexRecordLength;
	// TB时间窗口的数值变量
	int m_editindexTBWindow;
	// PeakTime的数值变量
	int m_editindexPeakTime;
	// Raw的布尔型变量
	BOOL m_bCheckRaw;
	// CorrelWith的字符串变量
	CString m_strCorrelWith;
	// AuxesFromDSD的布尔型变量
	BOOL m_bAuxesFromDSD;
	// 折射延迟的数值变量
	int m_editIndexRefractionDelay;
	// 记录长度
	int m_editIndexListeningTime;
	// PreStack的布尔型变量
	BOOL m_bPreStack;
public:
	CGridCtrlList		m_wndAuxListGrid;
	CGridCtrlEdit		m_wndAuxEditGrid;
	CGridCtrlList		m_wndAcqListGrid;
	CGridCtrlEdit		m_wndAcqEditGrid;
	CGridCtrlList		m_wndProTypeListGrid;
	CGridCtrlEdit		m_wndProTypeEditGrid;
	// ProType控件显示
	typedef struct ProTypeControls
	{
		// IDC_STATIC_TBWINDOW
		bool m_bTbWindow;
		// IDC_STATIC_PEAKTIME
		bool m_bPeakTime;
		// IDC_STATIC_REFRACTIONDELAY
		bool m_bRefractionDelay;
		// IDC_STATIC_LISTENINGTIME
		bool m_bListeningTime;
		// IDC_STATIC_RAW
		bool m_bRaw;
		// IDC_STATIC_PRESTACK
		bool m_bPreStack;
	}m_oProTypeControls;
	m_oProTypeControls m_oProTypeControlsShow;
	// Aux控件显示
	typedef struct AuxControls
	{
		// IDC_STATIC_AUX_COR
		bool m_bAuxCor;
		// IDC_CHECK_AUX
		bool m_bAuxCheck;
	}m_oAuxControls;
	m_oAuxControls m_oAuxControlsShow;
	// Acq控件显示
	typedef struct AcqControls
	{
		// IDC_STATIC_ACQGROUP
		bool m_bAcqGroup;
		// IDC_STATIC_ACQ
		bool m_bAcq;
		// IDC_STATIC_ACQNB
		bool m_bAcqNb;
		// IDC_STATIC_ACQTYPE
		bool m_bAcqType;
		// IDC_STATIC_ACQSIGNSTACK
		bool m_bAcqSignStack;
		// IDC_STATIC_ACQOUTPUT
		bool m_bAcqOutput;
		// IDC_STATIC_ACQ_GRID_EDIT
		bool m_bAcqGridEdit;
		// IDC_STATIC_ACQ_GRID
		bool m_bAcqGridList;
		// IDC_BUTTON_ACQ_ADD,IDC_BUTTON_ACQ_CHANGE,IDC_BUTTON_ACQ_DELETE
		bool m_bAcqEdit;
	}m_oAcqControls;
	m_oAcqControls m_oAcqControlsShow;
	// 显示控件
	void OnShowControls(void);
	// 依据左边界设置控件位置和尺寸
	void OnSetControlsLocationByLeftPos(int ID, int iLeftPos);
	// 依据右边界设置控件位置和尺寸
	void OnSetControlsLocationByRightPos(int ID, int iRightPos);
	// 显示处理类型控件
	void OnShowProTypeControls(void);
	// 显示Aux控件
	void OnShowAuxControls(void);
	// 显示Acq控件
	void OnShowAcqControls(void);
	// 显示处理类型窗口
	void OnShowProcessTypeWindow(int iProcessType);
	// 显示Aux窗口
	void OnShowAuxWindow(void);
	// 显示Aux窗口
	void OnShowAcqWindow(void);
	// 显示Process Type窗口
	void OnShowProTypeWindow(void);
};
