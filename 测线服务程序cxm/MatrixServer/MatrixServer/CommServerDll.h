#pragma once
#include "MatrixServerDllCall.h"
#include "..\MatrixCommDll\MatrixCommDll.h"
class CCommServerDll
{
public:
	CCommServerDll(void);
	~CCommServerDll(void);
public:
	CMatrixServerDllCall* m_pMatrixServer;
public:
	/** 接收区域——行数*/
	unsigned int m_uiLineNum;
	/** 接收区域——列数*/
	unsigned int m_uiColumnNum;
	/** 仪器噪声测试判据*/
	float m_fInstrumentNoiseLimit;
	/** 仪器畸变测试判据*/
	float m_fInstrumentDistortionLimit;
	/** 仪器串音测试判据*/
	float m_fInstrumentCrosstalkLimit;
	/** 仪器增益相位测试判据*/
	float m_fInstrumentGainPhaseLimit;
	/** 仪器共模测试判据*/
	float m_fInstrumentCMRRLimit;
	/** 野外电阻测试判据，下限*/
	float m_fSensorResistanceLimitMin;
	/** 野外电阻测试判据，上限*/
	float m_fSensorResistanceLimitMax;
	/** 野外漏电测试判据*/
	float m_fSensorLeakageLimit;
	/** 野外噪声测试判据*/
	float m_fSensorNoiseLimit;
	/** 野外倾斜度测试判据*/
	float m_fSensorTiltLimit;
	/** 野外倾斜度模型测试判据*/
	float m_fSensorTiltModelLimit;
	/** 地震监测测试数据*/
	float m_fSeisMonitorLimit;
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


	// 按区域查询仪器信息
	unsigned int QueryInstrumentInfoBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域仪器噪声测试数据和测试结果
	unsigned int QueryInstrNoiseTestBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域仪器失真测试数据和测试结果
	unsigned int QueryInstrDistortionTestBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域仪器串扰测试数据和测试结果
	unsigned int QueryInstrCrosstalkTestBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域仪器共模抑制比测试数据和测试结果
	unsigned int QueryInstrCMRRTestBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域仪器增益相位测试数据和测试结果
	unsigned int QueryInstrGainPhaseTestBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域检波器阻抗测试数据和测试结果
	unsigned int QuerySensorResistanceTestBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域检波器漏电测试数据和测试结果
	unsigned int QuerySensorLeakageTestBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域检波器噪声测试数据和测试结果
	unsigned int QuerySensorNoiseTestBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域检波器倾斜度测试数据和测试结果
	unsigned int QuerySensorTiltTestBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域检波器倾斜度模式测试数据和测试结果
	unsigned int QuerySensorTiltModelTestBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);
	// 查询所选区域地震监测测试数据和测试结果
	unsigned int QuerySeisMonitorTestBySN(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread);

	// 计算测试数据的算术均方根
	float CalTestDataMeanSquare(m_oInstrumentStruct* pInstrument);

	// 处理仪器设备表更新
	void OnProcInstrumentTableUpdate(CCommRecThread* pRecThread);

	// 处理仪器信息查询
	void OnProcQueryBySN(CCommRecThread* pRecThread, unsigned short usCmd, char* pChar, 
		unsigned int uiSize, unsigned int(CCommServerDll::*ptrFun)(m_oInstrumentStruct* pInstrument, 
		unsigned int uiStartPos, CCommRecThread* pRecThread));
	// 处理全部信息查询
	void OnProcQueryInfoAll(CCommRecThread* pRecThread, unsigned short usCmd,
		unsigned int(CCommServerDll::*ptrFun)(m_oInstrumentStruct* pInstrument, 
		unsigned int uiStartPos, CCommRecThread* pRecThread));
	// 按区域查询仪器信息
	unsigned int QueryBySN(CCommRecThread* pRecThread, unsigned int uiSN, 
		unsigned int uiSendPos, unsigned int(CCommServerDll::*ptrFun)(m_oInstrumentStruct* pInstrument, 
		unsigned int uiStartPos, CCommRecThread* pRecThread));
};

