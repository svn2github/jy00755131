#pragma once
#include <afxmt.h>
class CSaveFile
{
public:
	CSaveFile(void);
	~CSaveFile(void);
	// 显示收到和发送帧
	CString m_csEditShow;
	// 保存接收的数据到文件
	CString m_csSaveReceiveFile;
	// 定义临界区全局变量
	CCriticalSection m_Sec;
	// 通讯文件的存储路径
	CString m_csSaveFilePath;
	// 自动保存文件的大小
	int m_iSaveSize;
	// 初始化
	void OnInit(void);
	// 保存通讯数据开始标志位
	bool m_bStartSave;

	// 重置
	void OnReset(void);
	// 保存到文件中
	void OnSaveToFile(void);
	// 保存接收数据
	void OnSaveReceiveData(unsigned char* buf, int iRecLength);

	// 保存文件
//	FILE* m_file;
	CFile m_file;
};
