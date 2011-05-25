// BLASTER_SHOTPRODlg.h : 头文件
//

#pragma once
#include "RecSocket428XL.h"
#include "RecSocketShotPro.h"

#define RecPort428XL		0x8220
#define RecPortShotPro		0x8221
#define IPAddress428XL		"192.168.1.56"
#define IPAddressShotPro	"192.168.1.56"
// CBLASTER_SHOTPRODlg 对话框
class CBLASTER_SHOTPRODlg : public CDialog
{
// 构造
public:
	CBLASTER_SHOTPRODlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BLASTER_SHOTPRO_DIALOG };

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
	afx_msg void OnBnClickedButtonSend428xl();
	afx_msg void OnBnClickedButtonSendShotpro();
	// 428XL端炮号SP
	UINT m_uiSP428XL;
	// 爆炸机端炮号SP
	UINT m_uiSPShotPro;
	// 428XL端震源点测线SL
	double m_dSL428XL;
	// 爆炸机端震源点测线SL
	double m_dSLShotPro;
	// 428XL端震源点号SN
	double m_dSN428XL;
	// 爆炸机端震源点号SN
	double m_dSNShotPro;
	// 428XL端震源点索引SI
	UINT m_uiSI428XL;
	// 爆炸机端震源点索引SI
	UINT m_uiSIShotPro;
	// 428XL端爆炸机号SEQ
	UINT m_uiSEQ428XL;
	// 爆炸机端爆炸机号SEQ
	UINT m_uiSEQShotPro;
	// 爆炸机端爆炸机状态STA
	UINT m_uiSTAShotPro;
	// 爆炸机端确认的时断信号CTB
	double m_dCTBShotPro;
	// 爆炸机端的井口时间UH
	double m_dUHShotPro;
	// 428XL端发送缓冲区
	unsigned char m_ucSendBuf428XL[128];
	// 爆炸机端发送缓冲区
	unsigned char m_ucSendBufShotPro[256];
	// 428XL端接收端口
	CRecSocket428XL m_oRecSocket428XL;
	// 爆炸机端接收端口
	CRecSocketShotPro m_oRecSocketShotPro;
	// 生成428XL端发送帧
	void MakeFrame428XL(void);
	// 生成爆炸机端发送帧
	void MakeFrameShotPro(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
