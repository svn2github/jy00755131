// GraphView.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixTestSoft.h"
#include "TabSample.h"


// CGraphView 对话框

IMPLEMENT_DYNAMIC(CTabSample, CDialog)

CTabSample::CTabSample(CWnd* pParent /*=NULL*/)
	: CDialog(CTabSample::IDD, pParent)
{

}

CTabSample::~CTabSample()
{
}

void CTabSample::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabSample, CDialog)
END_MESSAGE_MAP()


// CGraphView 消息处理程序
