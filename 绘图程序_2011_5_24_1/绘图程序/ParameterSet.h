#pragma once
#include "afxcmn.h"


// CParameterSet 对话框

class CParameterSet : public CDialog
{
	DECLARE_DYNAMIC(CParameterSet)

public:
	CParameterSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CParameterSet();

// 对话框数据
	enum { IDD = IDD_DIALOG_PARAMETER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonSelectxmlpath();
	// 目标IP地址
	CString m_csIPAddrAim;
	// 发送端口
	CString m_csSendPort;
	// 接收端口
	CString m_csRcvPort;
	// 图形显示类型
	CString m_csShowGraphType;
	// 配置文件路径
	CString m_csXMLPath;
	// XMLDOM文件对象
	CXMLDOMDocument m_oXMLDOMDocument;
	// 图形显示的方向
	unsigned int m_uiShowDirection;
	// 载入配置文件
	void LoadXmlFile(void);
	// 保存配置文件
	void SaveXmlFile(void);
	// 打开XML配置文件
	BOOL OpenXmlFile(void);
	// 选择XML文件路径
	void OnSelectXMLPath(void);
	// 关闭程序配置文件
	void CloseXmlFile(void);
	// 载入图形显示设置
	bool LoadGraphShowSetup(void);
	// 保存图形显示设置
	bool SaveGraphShowSetup(void);
	// 保存XML文件路径
	bool SaveXmlFilePath(void);
};
