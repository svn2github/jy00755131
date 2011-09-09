#pragma once



// CSetupScalabilityView 窗体视图

class CSetupScalabilityView : public CBCGPDialog
{
	DECLARE_DYNCREATE(CSetupScalabilityView)

public:
	CSetupScalabilityView();           // 动态创建所使用的受保护的构造函数
	virtual ~CSetupScalabilityView();

public:
	enum { IDD = IDD_SETUPSCALABILITYVIEW };

/*
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
*/
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};


