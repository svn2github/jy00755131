
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CToolbarGraph : public CBCGPToolbarButton  
{
	DECLARE_SERIAL(CToolbarGraph)
public:
	CToolbarGraph(UINT uiID = 0);
	virtual ~CToolbarGraph();

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
	/**模式 0：无；1：合格-不合格；2：不合格-合格-不合格；3：噪声监测判据图*/
	int m_iMode;
	/**标签*/
	CString m_strLabel;

public:	// 方法
};