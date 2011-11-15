#pragma once
#include "OScopeCtrl.h"
#include "ThreadManage.h"
#include "Parameter.h"
#include "GraphShowDlg.h"
#include "LogFile.h"
#include "InstrumentGraph.h"
#include <afxmt.h>

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

private:
	// 打开
	void OnOpen(void);
	// 停止
	void OnStop(void);
	// 测网建立标志位
	BOOL m_bMatrixNetwork;
	// 尾包时刻查询过程计数
	unsigned int m_uiTailTimeCount;
	// 尾包监测计数
	unsigned int m_uiTailFrameTimerCount;
	// 本机IP地址
	unsigned int m_uiIPSource;
public:
	// 保存数据到文件的存储路径
	CString m_csSaveFilePath;
	// 线程控制类的对象
	CThreadManage m_oThreadManage;
	// 是否开启心跳的BOOL型变量
	BOOL m_bHeartBeat;
	// ADC数据自动存储文件数据点个数
	unsigned int m_uiADCFileLength;
	// 图形化显示ADC数据Dlg
	CGraphShowDlg m_dlgADCGraphShow[InstrumentNum];
	// 定义临界区变量
	CCriticalSection m_Sec_Sample;
	// 测网数据发送端口
	unsigned int m_uiSendPort;
	// 日志类指针
	CLogFile* m_pLogFile;
	// 绘图类变量
	CInstrumentGraph m_oInstrumentGraph;
public:
	// 初始化
	void OnInit(void);
	// 关闭
	void OnClose();
	// 求采样数据的平均值
	double SampleDataAverage(double* dSampleData, unsigned int uiLength);
	// 求采样数据的最大值
	double SampleDataMax(double* dSampleData, unsigned int uiLength);
	// 求采样数据的最小值
	double SampleDataMin(double* dSampleData, unsigned int uiLength);
	// 求采样数据的均方根
	double SampleDataRms(double* dSampleData, unsigned int uiLength);
	// 发送数据采集命令帧
	void OnSendCmdFrame(void);
	// 使能button控件
	void OnEnableButtons(void);
	// 禁用Button控件
	void OnDisableButtons(void);
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
	// 填充套接字地址结构
	void OnSetSocketAddr(sockaddr_in* addr, u_short hostshort, ULONG Saddr);
	// 计算绘图序号
	unsigned int CalLocationGraph(unsigned int uiLocation);
	// 绘制设备连接图
	void OnDrawInstrumentGraph(void);
public:
	afx_msg void OnBnClickedCheckMatrixnetwork();
	afx_msg void OnBnClickedButtotStartsample();
	afx_msg void OnBnClickedButtonStopsample();
	afx_msg void OnBnClickedButtonSavefilepath();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar); 
	afx_msg void OnBnClickedCheckHeartbeat();
	afx_msg void OnBnClickedButtonSetadc();
	afx_msg void OnBnClickedButtonTimecal();
	afx_msg void OnBnClickedButtonSetbyhand();
	afx_msg LRESULT OnTailTimeSocket(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSysTimeSocket(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnADCSetSocket(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTailFrameSocket(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnIPSetSocket(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHeadFrameSocket(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnADCGraphSocket(WPARAM wParam, LPARAM lParam);
	// 创建图形化显示ADC数据Dlg
	afx_msg LRESULT OnShowADCGraph(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonGraphexe();
};
