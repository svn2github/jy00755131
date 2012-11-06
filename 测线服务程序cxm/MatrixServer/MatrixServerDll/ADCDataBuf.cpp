#include "stdafx.h"
#include "MatrixServerDll.h"

// 重置数据存储缓冲区信息
void OnADCDataBufReset(m_oADCDataBufStruct* pADCDataBuf)
{
	ASSERT(pADCDataBuf != NULL);
	m_oSegdDataHeaderStruct* pSegdDataHeader = NULL;
	// 是否使用中
	pADCDataBuf->m_bInUsed = false;
	/** 每个站存储的数据点数*/
	pADCDataBuf->m_uiSavePointNum = 0;
	// 采样时间
	pADCDataBuf->m_uiSampleTime = 0;
	/** 判断是否写入SEGD文件标志位*/
	pADCDataBuf->m_bSaveInSegd = false;
	/** 参与施工的采集站个数*/
	pADCDataBuf->m_uiAcqTraceNum = 0;
	/** 辅助道数目*/
	pADCDataBuf->m_uiAuxTraceNum = 0;
	/** 施工任务索引表指针，关键字为IP，内容为参与施工的仪器结构体*/
	while(pADCDataBuf->m_olsSegdDataHeader.size() != 0)
	{
		pSegdDataHeader = *pADCDataBuf->m_olsSegdDataHeader.begin();
		delete pSegdDataHeader;
		pSegdDataHeader = NULL;
		pADCDataBuf->m_olsSegdDataHeader.pop_front();
	}
	/** 施工炮号*/
	pADCDataBuf->m_uiOptNo = 0;
	/** 采样率*/
	pADCDataBuf->m_iSampleRate = 0;
	/** 施工数据存储文件路径*/
	pADCDataBuf->m_SaveFilePath = "";
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