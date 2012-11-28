// Draw3DGraph_Test4Dlg.h : 头文件
//

#pragma once
#include "nigraph3d.h"
#include "afxwin.h"
#include <fstream>
#include <string>
using std::string;
using std::ifstream;
using std::ios;
using std::streamoff;
// 采样数据最大值
#define SampleAmpMax		10000.0
// 采样数据最大值
#define SampleAmpMin		-10000.0
// 列数
#define ColumnNum	20
// 定时器序号
#define TimerID				1
// 定时器延时
#define TimerDelay			1000
// 定义帧内时间显示字节数
#define FrameTimeBytesNume	13
// 数据显示字节数
#define DataBytesNum		10
// 行差值数据个数
#define InterpolateRowNum		9
// 列差值数据个数
#define InterpolateColumnNum	9
// CDraw3DGraph_Test4Dlg 对话框
class CDraw3DGraph_Test4Dlg : public CDialog
{
// 构造
public:
	CDraw3DGraph_Test4Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DRAW3DGRAPH_TEST4_DIALOG };

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
	// 绘图控件控制变量
	NI::CNiGraph3D m_ctrlGraph3D;
	// 开始显示控制变量
	CButton m_ctrlBtnStart;
	// 停止显示控制变量
	CButton m_ctrlBtnStop;
	// 选择文件按键控制变量
	CButton m_ctrlBtnSelectFile;
	// X轴数据
	CNiReal64Vector m_xRow;
	// Y轴数据
	CNiReal64Vector m_yColumn;
	// 采样数据
	CNiReal64Matrix m_zAmpData;
	// 差值计算后X轴数据
	CNiReal64Vector m_xCurveRow;
	// 差值计算后Y轴数据
	CNiReal64Vector m_yCurveColumn;
	// 差值计算后Z轴数据
	CNiReal64Matrix m_zCurveAmpData;
	// X轴方向衍生数据
	CNiReal64Vector m_dtRow;
	// Y轴方向衍生数据
	CNiReal64Vector m_dtColumn;
	// X轴方向采样数据临时缓冲
	CNiReal64Vector m_zAmpRowTemp;
	// Y轴方向采样数据临时缓冲
	CNiReal64Vector m_zAmpColumnTemp;
	// 坐标轴
	CNiAxis3D m_Axis3D;
	// 打开文件路径
	CString m_strFilePath;
	// 读取文件
	ifstream m_fin;
	// 读文件指针偏移量
	streamoff m_iOff;
	// 道数
	unsigned int m_uiTraceNume;
	// 打开文件标志位
	bool m_bOpenFile;
	// 时间计数
	unsigned int m_uiTimeCount;
	// 选择要打开的文件
	CString SelectOpenFile(void);
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnSelectfile();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
};
