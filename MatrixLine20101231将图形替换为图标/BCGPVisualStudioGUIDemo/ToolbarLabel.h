
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CToolbarLabel : public CBCGPToolbarButton  
{
	DECLARE_SERIAL(CToolbarLabel)
public:
	CToolbarLabel(UINT uiID = 0, LPCTSTR lpszText = NULL);
	virtual ~CToolbarLabel();
	virtual void OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* pImages, BOOL bHorz = TRUE, BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE);   	
	virtual BOOL IsEditable () const	
	{
		return FALSE;
	}
	virtual SIZE OnCalculateSize(CDC*pDC,const CSize&sizeDefault,BOOL bHorz);
};
