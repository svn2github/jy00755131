
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CToolbarImage : public CBCGPToolbarButton  
{
	DECLARE_SERIAL(CToolbarImage)
public:
	CToolbarImage();
	CToolbarImage(UINT uiID, UINT uiBitmapID);
	virtual ~CToolbarImage();

	virtual void OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* pImages, 
						 BOOL bHorz = TRUE, BOOL bCustomizeMode = FALSE,
						 BOOL bHighlight = FALSE,
						 BOOL bDrawBorder = TRUE, 
						 BOOL bGrayDisabledButtons = TRUE);   	

	virtual BOOL IsEditable () const	
	{
		return FALSE;
	}

	virtual SIZE OnCalculateSize(CDC*pDC,const CSize&sizeDefault,BOOL bHorz);
	virtual void CopyFrom (const CBCGPToolbarButton& src);

public:
	unsigned int m_uiBitmapID;
	CBitmap	m_bmpMyImage;
};