// OperationServerDlg.h : 头文件
//
#pragma once
#include "Resource.h"
#include "afxwin.h"
#include "operathread.h"
#include "sockettailframe.h"
#include "ShotController.h"
#include "..\common\OperationLog.h"
#include "..\common\FDUConfig.h"

class CSocketShotClient;
//////////////////////////////////////////////////////////////////////////
// 线程最大数量为4个 
#define  OPERATION_THREAD_MAXIUM	4

//////////////////////////////////////////////////////////////////////////
// COperationServerDlg 对话框
class COperationServerDlg : public CDialog
{
// 构造
public:
	COperationServerDlg(CWnd* pParent = NULL);	// 标准构造函数
// 对话框数据
	enum { IDD = IDD_OPERATIONSERVER_DIALOG };
	
// 属性
public:
	/** 辅助道 */
	// CAuxChannels	m_AuxChannels;
	/** 线程管理对象数组 */
	COperaThread*	m_pOperaThead[OPERATION_THREAD_MAXIUM];	
	/** 与客户端通信的socket端口 */
	CSocketShotClient*	m_pClientSocket;
	/** 与爆炸机所连的采集站通信socket端口 */
	CSocketShotBlaster* m_pBlasterSocket;
	
	/** 尾包时刻 */
	CSocketTailFrame m_sockTailFrame;	
	/** 显示日志信息 */
	CListBox m_ListMsg;
    /** 日志记录管理 */
	// COperationLog m_OperationLog;

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
	virtual void OnCancel();
private:
	// 创建与客户端通信的SOCKET
	// bool CreateClientSocket(UINT dwPort, LPCTSTR strHostIP);	
	// 解析采集排列的数据包
	bool AnalysisCmd_FDUConfig(unsigned char* pBuf);
	bool AnalysisCmd_SpreadLabel(unsigned char* pBuf);
	int  AnalysisCmd_SpreadChannel(unsigned char* pBuf);
	int  AnalysisCmd_ShotPointAndSource(unsigned char* pBuf);
	int  AnalysisCmd_ProcessType(unsigned char* pBuf);

public:	
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnrev();
	
	void AnalysisProtocolOfBlasterSock(void);
	void AnalysisProtocolOfClientSock(void);
	int  FindIdleThread();

public:	
	// void WriteLogMsg(LPCTSTR strMsg,int nType=1);
	void WriteLogMsg(LPCTSTR strPosition,LPCTSTR strDesc,WORD wType,BOOL bShowed);
};
