#include "stdafx.h"
#include "MatrixServerDll.h"

// 重置数据存储缓冲区信息
void OnADCDataBufReset(m_oADCDataBufStruct* pADCDataBuf)
{
	ASSERT(pADCDataBuf != NULL);
	// 是否使用中
	pADCDataBuf->m_bInUsed = false;
	// 数据存储帧序号，从0开始
	pADCDataBuf->m_uiFrameNb = 0;
	// 采样仪器SN
	pADCDataBuf->m_uiIP = 0;
	// 帧的本地时间
	pADCDataBuf->m_uiSysTime = 0;
}