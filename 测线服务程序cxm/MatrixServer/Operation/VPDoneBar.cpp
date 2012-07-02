#include "StdAfx.h"
#include "VPDoneBar.h"
#include "Operation.h"

CVPDoneBar::CVPDoneBar(void)
{
	for (int i=0; i<ActiveSourceNumLimit; i++)
	{
		m_iActiveSourceID[i] = ID_VPSHOT_FROM + i + ActiveSourceNumLimit * 2;
	}
}


CVPDoneBar::~CVPDoneBar(void)
{
}


// 设置表头
bool CVPDoneBar::SetGridHead(void)
{
	if(!m_GridView.GetSafeHwnd())		
		return false;
	m_GridView.InsertColumn (0, _T("口"), 50);
	m_GridView.InsertColumn (1, _T("Shot#"), 80);
	m_GridView.SetHeaderAlign (1, HDF_LEFT);
	m_GridView.SetColumnAlign (1, HDF_RIGHT);
	m_GridView.InsertColumn (2, _T("Breakpoint"), 150);
	m_GridView.SetHeaderAlign (2, HDF_LEFT);
	m_GridView.SetColumnAlign (2, HDF_RIGHT);
	m_GridView.InsertColumn (3, _T("SourcePointLine"), 150);
	m_GridView.SetHeaderAlign (3, HDF_LEFT);
	m_GridView.SetColumnAlign (3, HDF_RIGHT);
	m_GridView.InsertColumn (4, _T("SourcePointNb"), 150);
	m_GridView.SetHeaderAlign (4, HDF_LEFT);
	m_GridView.SetColumnAlign (4, HDF_RIGHT);
	m_GridView.InsertColumn (5, _T("SourcePointIndex"), 150);
	m_GridView.SetHeaderAlign (5, HDF_LEFT);
	m_GridView.SetColumnAlign (5, HDF_RIGHT);
	m_GridView.InsertColumn (6, _T("Sfn"), 100);
	m_GridView.SetHeaderAlign (6, HDF_LEFT);
	m_GridView.SetColumnAlign (6, HDF_RIGHT);
	m_GridView.InsertColumn (7, _T("Sfl"), 100);
	m_GridView.SetHeaderAlign (7, HDF_LEFT);
	m_GridView.SetColumnAlign (7, HDF_RIGHT);
	m_GridView.InsertColumn (8, _T("Spread#"), 100);
	m_GridView.SetHeaderAlign (8, HDF_LEFT);
	m_GridView.SetColumnAlign (8, HDF_RIGHT);
	m_GridView.InsertColumn (9, _T("SuperSpread#"), 100);
	m_GridView.SetHeaderAlign (9, HDF_LEFT);
	m_GridView.SetColumnAlign (9, HDF_RIGHT);
	m_GridView.InsertColumn (10, _T("ProcessType"), 100);
	m_GridView.SetHeaderAlign (10, HDF_LEFT);
	m_GridView.SetColumnAlign (10, HDF_RIGHT);
	m_GridView.InsertColumn (11, _T("Comment"), 100);
	m_GridView.SetHeaderAlign (11, HDF_LEFT);
	m_GridView.SetColumnAlign (11, HDF_RIGHT);
	m_GridView.InsertColumn (12, _T("Swath"), 100);
	m_GridView.SetHeaderAlign (12, HDF_LEFT);
	m_GridView.SetColumnAlign (12, HDF_RIGHT);
	return true;
}


// 载入全部炮点信息
void CVPDoneBar::LoadShotPoints(void)
{
	m_GridView.RemoveAll();
	for (int i =0; i<15; i++)
	{
		LoadShotPoint();
	}
}


// 载入单条炮点信息
void CVPDoneBar::LoadShotPoint(void)
{
	CBCGPGridRow* pRow = m_GridView.CreateRow (13);
	// pRow->GetItem (0)->SetValue (pVP->m_VPState);
	pRow->GetItem (0)->SetImage(1);
	pRow->GetItem (1)->SetValue (1);
	pRow->GetItem (2)->SetValue (1);
	pRow->GetItem (3)->SetValue (1);
	pRow->GetItem (4)->SetValue (1);

	pRow->GetItem (5)->SetValue (1);
	pRow->GetItem (6)->SetValue (1);
	pRow->GetItem (7)->SetValue (1);

	pRow->GetItem (8)->SetValue (1);
	pRow->GetItem (9)->SetValue (1);
	pRow->GetItem (10)->SetValue (1);
	pRow->GetItem (11)->SetValue (1);
	pRow->GetItem (12)->SetValue (2);
	m_GridView.AddRow (pRow, FALSE );
}
