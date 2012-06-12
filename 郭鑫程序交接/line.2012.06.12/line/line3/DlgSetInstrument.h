#pragma once
#include "afxwin.h"


// CDlgSetInstrument 对话框

class CDlgSetInstrument : public CDialog
{
	DECLARE_DYNAMIC(CDlgSetInstrument)

public:
	CDlgSetInstrument(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSetInstrument();

// 对话框数据
	enum { IDD = IDD_DIALOG_SETINSTRUMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonAddPrevious();
	afx_msg void OnBnClickedButtonAddNext();
	afx_msg void OnBnClickedButtonChange();
	afx_msg void OnBnClickedButtonDelete();
	// 仪器列序列号
	int m_iSelectUnitColumnIndex;
	// 仪器行序列号
	int m_iSelectUnitRowIndex;
	// 当前仪器类型，1为交叉站，2为电源站，3为采集站
	unsigned int m_uiInstrumentType;
	// 仪器设备类型选项控制变量
	CComboBox m_ctrlInstrumentType;
	// 得到当前待操作仪器的仪器类型
	unsigned int GetCurInstrumentType(void);
	// 在文件中设置仪器
	void OnSetInstrumentToFile(unsigned int uiInstrumentType, unsigned int uiOpt);
	// 修改仪器类型
	void OnChangeInstrumentType(unsigned int uiInstrumentType);
	// 仪器列数（单条测线最大仪器数）
	unsigned int m_uiColumnCount;
	// 测线条数
	unsigned int m_uiRowCount;
	// 删除仪器
	void OnDeleteInstrument(void);
	// 在此仪器之前加入仪器
	void OnAddInstrumentPrevious(unsigned int uiInstrumentType);
	// 在此仪器之后加入仪器
	void OnAddInstrumentNext(unsigned int uiInstrumentType);
	// 在每行的左侧增加仪器位置
	void OnAddInstrumentPosLeft(CFile* pFile);
	// 在每行的右侧增加仪器位置
	void OnAddInstrumentPosRight(CFile* pFile);
	// 在交叉站左侧添加仪器
	void AddInstrumentOnLAUXLeft(CFile * pfile, unsigned char* pInstrumentAdd);
	// 在交叉站右侧添加仪器
	void AddInstrumentOnLAUXRight(CFile * pfile, unsigned char* pInstrumentAdd);
	// 删除每行左侧空余的仪器位置
	void OnDeleteInstrumentPosLeft(CFile* pFile);
	// 删除每行右侧空余的仪器位置
	void OnDeleteInstrumentPosRight(CFile* pFile);
	// 检查仪器左侧空余位置
	BOOL OnCheckInstrumentPosLeft(CFile* pFile);
	// 检查仪器右侧空余位置
	BOOL OnCheckInstrumentPosRight(CFile* pFile);
};
