#pragma once
#include "OScopeCtrl.h"
#include <vector>
#include "ThreadManage.h"
#include "Parameter.h"

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
	COScopeCtrl m_OScopeCtrl[GraphViewNum];
	// 图形对应的picture控件ID指针
	int m_iGraphViewId[GraphViewId];
	// 采样数据处理结果显示所需控件ID
	int m_iMathId[GraphViewId][MathValueIdNum];
	// 选择仪器对象
	int m_iSelectObject[GraphViewId];
	// 选择仪器对象做噪声监测
	int m_iSelectObjectNoise[GraphViewId];
	// 采集到的数据
	vector <double> m_dSampleData[GraphViewNum];
	// 选中的仪器对象名称
	char* m_cSelectObjectName[GraphViewId];
	// 线程控制类的对象
	CThreadManage m_oThreadManage;
protected:
	// 保存数据到文件的存储路径
	CString m_csSaveFilePath;
	// 横向滚动条的最大值
	unsigned int m_uiScrollBarMax;
	// 横向滚动条的最小值
	unsigned int m_uiScrollBarMin;
protected:
	// 创建图形显示模板
	void CreateGraph(int iGraphViewNum, int* iGraphViewId, COScopeCtrl* oScopeCtrl);
	// 设置采样取值范围
	void SetRange(double dLower, double dUpper, int nDecimalPlaces, int iGraphViewNum, COScopeCtrl* oScopeCtrl);
	// 设置Y轴标签
	void SetYUnits(CString string, int iGraphViewNum, COScopeCtrl* oScopeCtrl);
	// 设置X轴标签
	void SetXUnits(CString string, int iGraphViewNum, COScopeCtrl* oScopeCtrl);
	// 设置背景颜色
	void SetBackgroundColor(COLORREF color, int iGraphViewNum, COScopeCtrl* oScopeCtrl);
	// 设置网格颜色
	void SetGridColor(COLORREF color, int iGraphViewNum, COScopeCtrl* oScopeCtrl);
	// 设置画点颜色
	void SetPlotColor(COLORREF color, int iGraphViewNum, COScopeCtrl* oScopeCtrl);
public:
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
	afx_msg void OnBnClickedButtonStartnoise();
	afx_msg void OnBnClickedButtonStopnoise();
	afx_msg void OnBnClickedButtonSavefilepath();
	afx_msg void OnBnClickedButtonReset();

	// 关闭
	void OnClose();
	// 采样开始标志位
	BOOL m_bStartSample;
	// 设置的采样时间
	double m_dSampleTime;
	// 选择仪器对象1
	int m_iSelectObject1;
	// 选择仪器对象2
	int m_iSelectObject2;
	// 选择仪器对象3
	int m_iSelectObject3;
	// 选择仪器对象4
	int m_iSelectObject4;
	// 选择仪器对象1做噪声监测
	int m_iSelectObjectNoise1;
	// 选择仪器对象2做噪声监测
	int m_iSelectObjectNoise2;
	// 选择仪器对象3做噪声监测
	int m_iSelectObjectNoise3;
	// 选择仪器对象4做噪声监测
	int m_iSelectObjectNoise4;
	// 噪声采集开始标志位
	BOOL m_bStartNoise;
	// 仪器选择
	unsigned int m_uiSampleNb;
	// 噪声仪器选择
	unsigned int m_uiNoiseNb;
	// 计算仪器最大的采样长度
	unsigned int m_iMaxLength;
	// 将采集到的数据保存到文件中
	void OnSaveToFile(int iMathValueIdNum, int iGraphViewNum, char** cppSelectObjectName, vector <double>* dSampleData);
	// 计算仪器最大的采样长度
	unsigned int OnSampleDataMaxLength(int iGraphViewNum, vector<double>* dSampleData);
	// 求采样数据的平均值
	double SampleDataAverage(vector<double> dSampleData);
	// 求采样数据的最大值
	double SampleDataMax(vector<double> dSampleData);
	// 求采样数据的最小值
	double SampleDataMin(vector<double> dSampleData);
	// 求采样数据的均方根
	double SampleDataRms(vector<double> dSampleData);
	// 发送数据采集命令帧
	void OnSendCmdFrame(unsigned int uiTestNb);
	// 采样停止标志位
	BOOL m_bStopSample;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar); 
	// 设备断开连接图标
	HICON m_iconDisconnected;
	// 设备连接图标
	HICON m_iconConnected;
	afx_msg void OnBnClickedCheckHeartbeat();
	// 是否开启心跳的BOOL型变量
	BOOL m_bHeartBeat;
};
