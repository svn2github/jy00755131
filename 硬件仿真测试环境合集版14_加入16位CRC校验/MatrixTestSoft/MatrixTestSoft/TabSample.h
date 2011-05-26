#pragma once
#include "OScopeCtrl.h"
#include <vector>
#include "ThreadManage.h"
#include "Parameter.h"
#include "ADCSet.h"
#include "GraphShowDlg.h"
#include <afxmt.h>
//#include "HeartBeatSocket.h"
using namespace std;
// CGraphView 对话框

class CTabSample : public CDialog
{
	DECLARE_DYNAMIC(CTabSample)

public:
	CTabSample(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTabSample();

// 对话框数据
	enum { IDD = IDD_DIALOG_SAMPLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 初始化
	void OnInit(void);

public:
	// 图形模板
//	COScopeCtrl m_OScopeCtrl[GraphViewNum];
	// 图形对应的picture控件ID指针
//	int m_iGraphViewId[GraphViewId];
	// 选择仪器对象
	int m_iSelectObject[GraphViewNum];
	// 选择仪器对象做噪声监测
	int m_iSelectObjectNoise[GraphViewNum];
	// 选中的仪器对象名称
	char m_cSelectObjectName[GraphViewNum][RcvFrameSize];
	// 采样数据处理结果显示所需控件ID
//	int m_iMathId[GraphViewId][MathValueIdNum];
	// 线程控制类的对象
	CThreadManage m_oThreadManage;
	// 心跳Socket
//	CHeartBeatSocket m_HeartBeatSocket;
protected:
	// 保存数据到文件的存储路径
	CString m_csSaveFilePath;
protected:
	// 打开
	void OnOpen(void);
	// 停止
	void OnStop(void);
	// 测网建立标志位
	BOOL m_bMatrixNetwork;

public:
	afx_msg void OnBnClickedCheckMatrixnetwork();
	afx_msg void OnBnClickedButtotStartsample();
	afx_msg void OnBnClickedButtonStopsample();
	afx_msg void OnBnClickedButtonSavefilepath();
	afx_msg void OnBnClickedButtonReset();

	// ADC设置类对象
	CADCSet m_oADCSet;
	// 关闭
	void OnClose();
	// 采样开始标志位
	BOOL m_bStartSample;
	// 求采样数据的平均值
	double SampleDataAverage(vector<double> dSampleData);
	// 求采样数据的最大值
	double SampleDataMax(vector<double> dSampleData);
	// 求采样数据的最小值
	double SampleDataMin(vector<double> dSampleData);
	// 求采样数据的均方根
	double SampleDataRms(vector<double> dSampleData);
	// 计算仪器最大的采样长度
	unsigned int OnSampleDataMaxLength(int iGraphViewNum, vector<double>* dSampleData);
	// 发送数据采集命令帧
	void OnSendCmdFrame(int* pSelectObject);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar); 
	// 采集站设备断开连接图标
	HICON m_iconFDUDisconnected;
	// 采集站设备连接图标
	HICON m_iconFDUConnected;
	// 交叉站设备断开连接图标
	HICON m_iconLAUXDisconnected;
	// 交叉站设备连接图标
	HICON m_iconLAUXConnected;
	afx_msg void OnBnClickedCheckHeartbeat();
	// 是否开启心跳的BOOL型变量
	BOOL m_bHeartBeat;
	afx_msg void OnBnClickedButtonSetadc();
	// ADC参数设置
	void OnADCSet(unsigned int uiIPAim);
	// ADC数据采集停止
	void OnADCSampleStop(unsigned int uiIPAim);
	afx_msg void OnBnClickedButtonTimecal();
	// 未收到尾包计数
	unsigned int m_uiTailRecCount;
	// 采集站设备Button控件ID
	int m_iButtonIDFDU[GraphViewNum];
	// 采集站设备仪器选择Check控件ID
	int m_iCheckIDInstrumentFDU[GraphViewNum];
	// 采集站设备噪声选择Check控件ID
	int m_iCheckIDNoiseFDU[GraphViewNum];
	// ADC数据自动存储文件数据点个数
	unsigned int m_uiADCFileLength;
	// 使能button控件
	void OnEnableButtons(void);
	// 禁用Button控件
	void OnDisableButtons(void);
	// 图形化显示ADC数据Dlg
	CGraphShowDlg m_dlgADCData;
	afx_msg void OnBnClickedButtonFdu1();
	afx_msg void OnBnClickedButtonFdu2();
	afx_msg void OnBnClickedButtonFdu3();
	afx_msg void OnBnClickedButtonFdu4();
	afx_msg void OnBnClickedButtonFdu5();
	afx_msg void OnBnClickedButtonFdu6();
	afx_msg void OnBnClickedButtonFdu7();
	afx_msg void OnBnClickedButtonFdu8();
	afx_msg void OnBnClickedButtonFdu9();
	afx_msg void OnBnClickedButtonFdu10();
	afx_msg void OnBnClickedButtonFdu11();
	afx_msg void OnBnClickedButtonFdu12();
	afx_msg void OnBnClickedButtonFdu13();
	afx_msg void OnBnClickedButtonFdu14();
	afx_msg void OnBnClickedButtonFdu15();
	afx_msg void OnBnClickedButtonFdu16();
	afx_msg void OnBnClickedButtonFdu17();
	afx_msg void OnBnClickedButtonFdu18();
	afx_msg void OnBnClickedButtonFdu19();
	afx_msg void OnBnClickedButtonFdu20();
	afx_msg void OnBnClickedButtonFdu21();
	afx_msg void OnBnClickedButtonFdu22();
	afx_msg void OnBnClickedButtonFdu23();
	afx_msg void OnBnClickedButtonFdu24();
	afx_msg void OnBnClickedButtonFdu25();
	afx_msg void OnBnClickedButtonFdu26();
	afx_msg void OnBnClickedButtonFdu27();
	afx_msg void OnBnClickedButtonFdu28();
	afx_msg void OnBnClickedButtonFdu29();
	afx_msg void OnBnClickedButtonFdu30();
	afx_msg void OnBnClickedButtonFdu31();
	afx_msg void OnBnClickedButtonFdu32();
	afx_msg void OnBnClickedButtonFdu33();
	afx_msg void OnBnClickedButtonFdu34();
	afx_msg void OnBnClickedButtonFdu35();
	afx_msg void OnBnClickedButtonFdu36();
	afx_msg void OnBnClickedButtonFdu37();
	afx_msg void OnBnClickedButtonFdu38();
	afx_msg void OnBnClickedButtonFdu39();
	afx_msg void OnBnClickedButtonFdu40();
	afx_msg void OnBnClickedButtonFdu41();
	afx_msg void OnBnClickedButtonFdu42();
	afx_msg void OnBnClickedButtonFdu43();
	afx_msg void OnBnClickedButtonFdu44();
	afx_msg void OnBnClickedButtonFdu45();
	afx_msg void OnBnClickedButtonFdu46();
	afx_msg void OnBnClickedButtonFdu47();
	afx_msg void OnBnClickedButtonFdu48();
	// 创建图形化显示ADC数据Dlg
	void OnShowADCGraph(unsigned int uiADCGraphNb);
	// ADC零漂矫正
	void OnADCZeroDrift(unsigned int uiIPAim);
	// 定义临界区变量
	CCriticalSection m_Sec;
	// 测网数据发送端口
	unsigned int m_uiSendPort;
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
};
