#pragma once
#include "MatrixOptDllCall.h"
#include "..\MatrixCommDll\MatrixCommDll.h"
class CCommOptDll
{
public:
	CCommOptDll(void);
	~CCommOptDll(void);
public:
	CMatrixOptDllCall* m_pMatrixOpt;
	/** 施工客户端信息*/
	m_oOptSetupDataStruct* m_pOptSetupData;
public:
	/** 初始化*/
	void OnInit(CString strPath);
	/** 载入配置文件*/
	void LoadOptAppSetupData();
	/** 保存配置文件*/
	void SaveOptAppSetupData();
	/** 关闭*/
	void OnClose();
public:
	/**
	* @fn void OnProcSetDelayOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 OperationDelay XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetDelayOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetSourceShotOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 炮表 XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetSourceShotOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetExploOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 Explo震源类型 XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetExploOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetVibroOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 Vibro震源类型 XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetVibroOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetProcessRecordOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 ProcessRecord XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetProcessRecordOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetProcessAuxOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 ProcessAux XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetProcessAuxOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetProcessAcqOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 ProcessAcq XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetProcessAcqOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetProcessTypeOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 ProcessType XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetProcessTypeOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
	/**
	* @fn void OnProcSetCommentsOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true)
	* @detail 设置 注释 XML文件信息
	* @param[in] pChar 缓冲区指针
	* @param[in] uiSize 缓冲区大小
	* @return void
	*/
	void OnProcSetCommentsOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave = true);
		/**
	* @fn void OnProcQueryDelayOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 OperationDelay XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryDelayOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQuerySourceShotOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 炮表 XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQuerySourceShotOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryExploOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 Explo震源类型 XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryExploOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryVibroOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 Vibro震源类型 XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryVibroOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryProcessRecordOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 ProcessRecord XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryProcessRecordOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryProcessAuxOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 ProcessAux XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryProcessAuxOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryProcessAcqOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 ProcessAcq XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryProcessAcqOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryProcessTypeOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 ProcessType XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryProcessTypeOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
	/**
	* @fn void OnProcQueryCommentsOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
	* @detail 查询 注释 XML文件信息
	* @param[in] pRecThread 客户端接收线程指针
	* @param[in] usCmd 命令字
	* @return void
	*/
	void OnProcQueryCommentsOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd);
};

