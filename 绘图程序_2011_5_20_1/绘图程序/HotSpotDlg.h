#pragma once

class ImageMapHandler;
// CHotSpotDlg 对话框

class CHotSpotDlg : public CDialog
{
	DECLARE_DYNAMIC(CHotSpotDlg)

public:
	CHotSpotDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHotSpotDlg();

// 对话框数据
	enum { IDD = IDD_HOTSPOTPARAM };
	CListCtrl	m_ParamList;
protected:
	virtual BOOL OnInitDialog();

	// Implementation
public:
	virtual void SetData(ImageMapHandler *data);

	// Member Variables
private:
	ImageMapHandler *hotSpotData;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
