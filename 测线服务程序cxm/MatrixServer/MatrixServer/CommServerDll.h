#pragma once
#include "MatrixDllCall.h"
#include "..\MatrixCommDll\MatrixCommDll.h"
class CCommServerDll
{
public:
	CCommServerDll(void);
	~CCommServerDll(void);
public:
	CMatrixDllCall* m_pMatrixDllCall;
public:
		/**
	* @fn void OnProcSetFieldOn(CCommRecThread* pRecThread)
	* @detail 处理上电
	* @param[in] CCommRecThread*
	* @return void
	*/
	void OnProcSetFieldOn(CCommRecThread* pRecThread);
	/**
	* @fn void OnProcSetFieldOff(CCommRecThread* pRecThread)
	* @detail 处理断电
	* @param[in] CCommRecThread*
	* @return void
	*/
	void OnProcSetFieldOff(CCommRecThread* pRecThread);
};

