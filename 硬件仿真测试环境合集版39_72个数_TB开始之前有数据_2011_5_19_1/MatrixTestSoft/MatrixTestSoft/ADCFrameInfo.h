#pragma once
#include "Parameter.h"
class CADCFrameInfo
{
public:
	CADCFrameInfo(void);
	~CADCFrameInfo(void);
public:
	// 文件保存路径
	CString m_csSaveFilePath;
	// 保存文件的文件指针
	FILE * m_pFileSave;
public:
	// 打开文件
	void OnOpenFile(void);
	// 保存文件
	void OnSaveFile(unsigned int uiInstrumentNb, unsigned int uiDataPointNum, unsigned int uiSysTime);
	// 关闭文件
	void OnCloseFile(void);
};
