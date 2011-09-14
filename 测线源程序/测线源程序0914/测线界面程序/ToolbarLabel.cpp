
#include "stdafx.h"
#include "ToolbarLabel.h"
#include "ToolbarLabel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CToolbarLabel, CBCGPToolbarButton, 1)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToolbarLabel::CToolbarLabel (UINT uiID, LPCTSTR lpszText)
{
	if (lpszText != NULL)	
	{		
		m_strText = lpszText;	
	}
	m_bText = TRUE;	
	m_nID = uiID;
}

CToolbarLabel::~CToolbarLabel()
{

}

void CToolbarLabel::OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* pImages, BOOL bHorz, BOOL bCustomizeMode, BOOL bHighlight,	BOOL bDrawBorder, BOOL bGrayDisabledButtons)
{
	UINT nStyle = m_nStyle;	
	m_nStyle &= ~TBBS_DISABLED;   	
	CBCGPToolbarButton::OnDraw (pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);   	
	m_nStyle = nStyle;
}

SIZE CToolbarLabel::OnCalculateSize (CDC* pDC, const CSize& sizeDefault, BOOL bHorz)
{
	m_iImage = -1;
	return CBCGPToolbarButton::OnCalculateSize (pDC, sizeDefault, bHorz);
}
