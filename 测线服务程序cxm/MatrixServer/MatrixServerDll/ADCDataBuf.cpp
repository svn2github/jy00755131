#include "stdafx.h"
#include "MatrixServerDll.h"

// 重置数据存储缓冲区信息
void OnADCDataBufReset(m_oADCDataBufStruct* pADCDataBuf)
{
	ASSERT(pADCDataBuf != NULL);
	// 是否使用中
	pADCDataBuf->m_bInUsed = false;
	if (pADCDataBuf->m_pADCDataBuf != NULL)
	{
		memset(pADCDataBuf->m_pADCDataBuf, 0, pADCDataBuf->m_uiBufLength);
	}
// 	// 每个采集站数据存储帧数
// 	pADCDataBuf->m_uiFrameNb = 0;
// 	// SEGD协议头长度
// 	pADCDataBuf->m_uiSEGDHeaderLen = 0;
// 	// 采集站数据头长度
// 	pADCDataBuf->m_uiDataHeaderLen = 0;
}