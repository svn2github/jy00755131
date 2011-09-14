
#include "stdafx.h"
#include "ToolbarImage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CToolbarImage, CBCGPToolbarButton, 1)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToolbarImage::CToolbarImage()
{
	m_bText = FALSE;	
	m_bImage = TRUE;
	m_nID = 0;
	m_uiBitmapID = 0;
}

CToolbarImage::CToolbarImage (UINT uiID, UINT uiBitmapID)
{
	m_bText = FALSE;	
	m_bImage = TRUE;
	m_nID = uiID;
	m_uiBitmapID = uiBitmapID;
}

CToolbarImage::~CToolbarImage()
{

}

void CToolbarImage::OnDraw (CDC* pDC, const CRect& rect, 
							 CBCGPToolBarImages* /*pImages*/, 
							 BOOL /*bHorz*/, BOOL /*bCustomizeMode*/,						
							 BOOL /*bHighlight*/,						
							 BOOL /*bDrawBorder*/, 
							 BOOL /*bGrayDisabledButtons*/)
{
	pDC->DrawState (rect.TopLeft (), rect.Size (), &m_bmpMyImage, DST_BITMAP); 
}		 


SIZE CToolbarImage::OnCalculateSize(CDC* /*pDC*/, const CSize& /*sizeDefault*/, BOOL /*bHorz*/)
{
	BITMAP bm;
	m_bmpMyImage.GetBitmap (&bm);
	CSize size (bm.bmWidth, bm.bmHeight);
	return size;
}

void CToolbarImage::CopyFrom (const CBCGPToolbarButton& src)
{
	CBCGPToolbarButton::CopyFrom (src);
	m_uiBitmapID = ((CToolbarImage*)&src)->m_uiBitmapID;
	m_bmpMyImage.LoadBitmap(m_uiBitmapID);
}
