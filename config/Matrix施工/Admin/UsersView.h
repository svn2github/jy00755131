#pragma once
#include "afxwin.h"
#include "afxcmn.h"



// CUsersView 窗体视图

class CUsersView : public CBCGPFormView
{
	DECLARE_DYNCREATE(CUsersView)

protected:
	CUsersView();           // 动态创建所使用的受保护的构造函数
	virtual ~CUsersView();

public:
	enum { IDD = IDD_USERSVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	void    LoadSingleUserInfo(CUserInfo* pUserInfo);
	void    SaveSingleUserInfo(int  nItem,CUserInfo* pUserInfo);
	void    EnableModify(BOOL bEnable);
private:
	// 前一个被选择的用户编号
	int		m_nLastSelectedItem;
	// 图像列表
	CImageList		m_ImageList;
public:
	// 自动删除
	BOOL m_bAutoDelete;
	CBCGPComboBox m_ctrlUserRole;
	// 密码失效模式
	int m_nExpiredMode;
	CBCGPDateTimeCtrl	m_ctrlExpiryDate;
	CBCGPListCtrl		m_ctrlUsersList;
public:
	// 初始化
	virtual void OnInitialUpdate();
	// 加载所有用户信息
	void LoadAllUserInfo(void);
	afx_msg void OnNMClickListusers(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListusers(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnUserModify();
	afx_msg void OnBnClickedBtnsave();
	afx_msg void OnUserDelete();
	afx_msg void OnUserNew();
};


