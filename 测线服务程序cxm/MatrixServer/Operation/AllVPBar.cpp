#include "StdAfx.h"
#include "AllVPBar.h"


CAllVPBar::CAllVPBar(void)
{
}


CAllVPBar::~CAllVPBar(void)
{
}


// 设置表头
bool CAllVPBar::SetGridHead(void)
{
	if(!m_GridView.GetSafeHwnd())		
		return false;
	m_GridView.InsertColumn (0, _T(""), 50);
	m_GridView.InsertColumn (1, _T("Shot#"), 80);
	m_GridView.InsertColumn (2, _T("SourcePointLine"), 150);
	m_GridView.InsertColumn (3, _T("SourcePointNb"), 150);
	m_GridView.InsertColumn (4, _T("SourcePointIndex"), 150);
	m_GridView.InsertColumn (5, _T("Sfn"), 100);
	m_GridView.InsertColumn (6, _T("Sfl"), 100);
	m_GridView.InsertColumn (7, _T("Spread#"), 100);
	m_GridView.InsertColumn (8, _T("SuperSpread#"), 100);
	m_GridView.InsertColumn (9, _T("ProcessType"), 100);
	m_GridView.InsertColumn (10, _T("Comment"), 100);
	m_GridView.InsertColumn (11, _T("Swath"), 100);
	return true;
}


// 载入全部炮点信息
void CAllVPBar::LoadShotPoints(void)
{
	for (int i =0; i<35; i++)
	{
		LoadShotPoint();
	}
}


// 载入单条炮点信息
void CAllVPBar::LoadShotPoint(void)
{
	CBCGPGridRow* pRow = m_GridView.CreateRow (12);
	// pRow->GetItem (0)->SetValue (pVP->m_VPState);
	pRow->GetItem (0)->SetValue(1);
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
	pRow->GetItem (11)->SetValue (2);
	m_GridView.AddRow (pRow, FALSE );
}
