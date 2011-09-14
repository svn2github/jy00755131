
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CToolbarGraphButton : public CBCGPToolbarButton  
{
	DECLARE_SERIAL(CToolbarGraphButton)
public:
	CToolbarGraphButton(UINT uiID = 0);
	virtual ~CToolbarGraphButton();

	virtual void OnDraw (CDC* pDC, const CRect& rectClient, 
							CBCGPToolBarImages* pImages, 
							BOOL bHorz = TRUE, BOOL bCustomizeMode = FALSE,
							BOOL bHighlight = FALSE,
							BOOL bDrawBorder = TRUE, 
							BOOL bGrayDisabledButtons = TRUE);   	

	virtual BOOL IsEditable () const	
	{
		return FALSE;
	}

	virtual SIZE OnCalculateSize(CDC*pDC,const CSize&sizeDefault,BOOL bHorz);

protected:

public:	// 属性
	/**宽度*/
	int m_iWidth;
	/**模式 0：未选中；1：选中*/
	int m_iMode;
	/**标签*/
	CString m_strLabel;

public:	// 方法
};