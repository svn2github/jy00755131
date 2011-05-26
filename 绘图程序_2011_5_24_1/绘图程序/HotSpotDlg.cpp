// HotSpotDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "chartdir.h"
#include "绘图程序.h"
#include "ChartViewer.h"
#include "HotSpotDlg.h"


// CHotSpotDlg 对话框

IMPLEMENT_DYNAMIC(CHotSpotDlg, CDialog)

CHotSpotDlg::CHotSpotDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHotSpotDlg::IDD, pParent)
{
	hotSpotData = 0;
}

CHotSpotDlg::~CHotSpotDlg()
{
}

void CHotSpotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHotSpotDlg)
	DDX_Control(pDX, IDC_PARAMLIST, m_ParamList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHotSpotDlg, CDialog)
END_MESSAGE_MAP()


// CHotSpotDlg 消息处理程序
/////////////////////////////////////////////////////////////////////////////
// CHotSpotDlg implementation

void CHotSpotDlg::SetData(ImageMapHandler *data)
{
	// set the data for the dialog
	hotSpotData = data;
}

BOOL CHotSpotDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//
	// fill the list control with data from the hot spot
	//

	m_ParamList.InsertColumn(0, _T("Param"), LVCFMT_LEFT, 75);
	m_ParamList.InsertColumn(1, _T("Value"), LVCFMT_LEFT, 240);

	if (0!= hotSpotData)
	{
		int i = 0;
		const char *key;
		while (0 != (key = hotSpotData->getKey(i)))
		{
			//remember to use UTF8toTCHAR for possible conversion, as ChartDirector
			//uses UTF8, while MFC uses TCHAR
			m_ParamList.InsertItem(i, UTF8toTCHAR(key));
			m_ParamList.SetItemText(i, 1, UTF8toTCHAR(hotSpotData->getValue(i)));
			++i;
		}
	}

	return TRUE;
}
