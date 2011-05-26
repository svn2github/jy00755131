// HardwareTestDlg.h : 头文件
//

#pragma once
#include "OScopeCtrl.h"
#include <vector>
#include "resource.h"
#include "ThreadManage.h"
#include "Parameter.h"
#include "SysTimeSocket.h"
#include <afxmt.h>

using namespace std;

// CHardwareTestDlg 对话框

class CHardwareTestDlg : public CDialog
{
// 构造
public:
	CHardwareTestDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CHardwareTestDlg();

// 对话框数据
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	// 图形模板指针
	COScopeCtrl m_OScopeCtrl[GraphViewNum];
	// 图形对应的picture控件ID指针
	int m_iGraphViewId[GraphViewId];
	// 采样开始标志位
	BOOL m_bStartSample;
	// 设置的采样时间
	double m_dSampleTime;
	// 采集到的数据
	vector <double> m_dSampleData[GraphViewNum];
	// 选择仪器对象
	int m_bSelectObject[GraphViewId];
	// 选择仪器对象1
	int m_bSelectObject1;
	// 选择仪器对象2
	int m_bSelectObject2;
	// 选择仪器对象3
	int m_bSelectObject3;
	// 选择仪器对象4
	int m_bSelectObject4;
	// 选择仪器对象5
	int m_bSelectObject5;
	// 选择仪器对象6
	int m_bSelectObject6;
	// 选择仪器对象7
	int m_bSelectObject7;
	// 选择仪器对象8
	int m_bSelectObject8;
	// 选中的仪器对象名称
	char** m_cSelectObjectName;
	// 保存采样数据文件的路径
	char cSaveToFilePath[MAX_PATH];
	CString strFilePath;
	// 横向滚动条的最大和最小值
	UINT MAX_RANGE; 
	UINT MIN_RANGE; 

	// 采样停止标志位
	BOOL m_bStopSample;
	// 仪器选择
	UINT32 m_uiSampleNb;
	// 计算仪器最大的采样长度
	unsigned int m_iMaxLength;
	// 采样数据处理结果显示所需控件ID
	int m_iMathId[GraphViewId][MathValueIdNum];
	// 选择仪器对象做噪声监测
	int m_bSelectObjectNoise[GraphViewId];
	// 选择仪器对象1做噪声监测
	int m_bSelectObjectNoise1;
	// 选择仪器对象2做噪声监测
	int m_bSelectObjectNoise2;
	// 选择仪器对象3做噪声监测
	int m_bSelectObjectNoise3;
	// 选择仪器对象4做噪声监测
	int m_bSelectObjectNoise4;
	// 选择仪器对象5做噪声监测
	int m_bSelectObjectNoise5;
	// 选择仪器对象6做噪声监测
	int m_bSelectObjectNoise6;
	// 选择仪器对象7做噪声监测
	int m_bSelectObjectNoise7;
	// 选择仪器对象8做噪声监测
	int m_bSelectObjectNoise8;
	// 噪声采集开始标志位
	BOOL m_bStartNoise;
	// 噪声仪器选择
	UINT32 m_uiNoiseNb;
	CThreadManage m_oThreadManage;
	// 定义临界区全局变量
	CCriticalSection m_Sec;
	// 尾包时刻查询帧
	byte m_pCollectSysTimeSendData[128];
	// 查询采集站本地时间Socket
	CSysTimeSocket* m_SysTimeSocket;
public:
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

	// 接收采样数据
	void ReceiveSampleData(byte* pSampleData);
	// 创建获取采集站本地时间端口
	void OnCreateCollectSysTimeBroadcastSocket(void);

public:
	afx_msg void OnBnClickedButtotStartSample();
	afx_msg void OnBnClickedButtonStopSample();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar); 
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonStartNoise();
	afx_msg void OnBnClickedButtonStopNoise();
	afx_msg void OnBnClickedButtonADCInit();
	afx_msg void OnBnClickedButtonADCOffest();
	afx_msg void OnBnClickedButtonADCFullScale();
	afx_msg void OnBnClickedButtonADCStart();
	afx_msg void OnBnClickedButtonADCStop();
	afx_msg void OnClose();
};
