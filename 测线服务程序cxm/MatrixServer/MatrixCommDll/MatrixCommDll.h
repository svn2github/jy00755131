// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 MATRIXCOMMDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// MATRIXCOMMDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

#ifndef _MatrixCommDll_H
#define _MatrixCommDll_H
/*
*	if using C++ Compiler to compile the file, adopting C linkage mode
*/
#ifdef __cplusplus
extern "C" {
#endif

	// according to the control macro, deciding whether export or import functions
#ifdef MATRIXCOMMDLL_EXPORTS
#define MATRIXCOMMDLL_API __declspec(dllexport)
#else
#define MATRIXCOMMDLL_API __declspec(dllimport)
#endif

// 此类是从 MatrixCommDll.dll 导出的
/**
* @class CCommClient
* @brief 从Dll导出的客户端Socket类
*/
class MATRIXCOMMDLL_API CCommClient : public CAsyncSocket
{
public:
	/**
	* @fn CCommServer(void)
	* @detail 构造函数
	*/
	CCommClient();
	/**
	* @fn ~CCommServer(void)
	* @detail 析构函数
	*/
	~CCommClient();
};
/**
* @class CMatrixCommDll
* @brief 从Dll导出的通讯接口类
*/
class MATRIXCOMMDLL_API CMatrixCommDll {
public:
	/**
	* @fn CMatrixCommDll()
	* @detail 构造函数
	*/
	CMatrixCommDll();
	/**
	* @fn ~CMatrixCommDll()
	* @detail 析构函数
	*/
	~CMatrixCommDll();
	// TODO: 在此添加您的方法。
	/**
	* @fn void OnInit(void)
	* @detail 初始化
	* @return void
	*/
	virtual void OnInit(void);
	/**
	* @fn void OnClose(void)
	* @detail 关闭
	* @return void
	*/
	virtual void OnClose(void);
	/**
	* @fn void OnInitSocketLib(void)
	* @detail 初始化套接字库
	* @return void
	*/
	virtual void OnInitSocketLib(void);
	/**
	* @fn void OnCloseSocketLib(void)
	* @detail 释放套接字库
	* @return void
	*/
	virtual void OnCloseSocketLib(void);
public:
	/** 客户端Socket类指针*/
	CCommClient* m_pCommClient;
};
extern MATRIXCOMMDLL_API int nMatrixCommDll;

MATRIXCOMMDLL_API int fnMatrixCommDll(void);

MATRIXCOMMDLL_API CMatrixCommDll* CreateMatrixCommDll(void);
MATRIXCOMMDLL_API void DeleteMatrixCommDll(CMatrixCommDll* pClass);
/**
* @fn CCommClient* CreateCommClient(void)
* @detail 创建客户端
*/
MATRIXCOMMDLL_API CCommClient* CreateCommClient(void);
/**
* @fn void DeleteCommClient(CCommClient* pClass)
* @detail 释放客户端
*/
MATRIXCOMMDLL_API void DeleteCommClient(CCommClient* pClass);
#undef MATRIXCOMMDLL_API
#ifdef __cplusplus
}
#endif

#endif