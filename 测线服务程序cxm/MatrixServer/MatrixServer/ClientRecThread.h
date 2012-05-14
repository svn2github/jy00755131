#pragma once
#include "ClientRecFrame.h"
#include "ClientSndFrame.h"
#include "ClientCommThread.h"
#include "MatrixDllCall.h"
#include <hash_map>
#include <map>
using std::map;
using std::hash_map;
// 接收来自客户端的帧线程
class CClientRecThread : public CClientCommThread
{
public:
	CClientRecThread(void);
	~CClientRecThread(void);
public:
	// 接收客户端类指针
	CClientRecFrame* m_pClientRecFrame;
	// 发送客户端类指针
	CClientSndFrame* m_pClientSndFrame;
	// DLL函数调用类成员
	CMatrixDllCall* m_pMatrixDllCall;
	// 接收区域——行数
	unsigned int m_uiLineNum;
	// 接收区域——列数
	unsigned int m_uiColumnNum;
	// 连接验证
	bool m_bCheckConnected;
	// 客户端设备位置索引表
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*> m_oInstrumentWholeTableMap;
	// 仪器区域结构体
	typedef struct AreaStruct
	{
		// 线号，从1开始
		unsigned int m_uiLineNb;
		// 区域号，从1开始
		unsigned int m_uiAreaNb;
		bool operator == (const AreaStruct& rhs) const
		{
			return ((m_uiLineNb == rhs.m_uiLineNb) && (m_uiAreaNb == rhs.m_uiAreaNb));
		}
		bool operator < (const AreaStruct& rhs) const
		{
			if (m_uiLineNb == rhs.m_uiLineNb)
			{
				return (m_uiAreaNb < rhs.m_uiAreaNb);
			}
			else
			{
				return (m_uiLineNb < rhs.m_uiLineNb);
			}
		}
	}m_oAreaStruct;
	// 客户端设备更新区域索引表
	map<m_oAreaStruct, m_oAreaStruct> m_oInstrumentUpdateArea;
	// 仪器噪声测试判据
	float m_fInstrumentNoiseLimit;
	// 仪器畸变测试判据
	float m_fInstrumentDistortionLimit;
	// 仪器串音测试判据
	float m_fInstrumentCrosstalkLimit;
	// 仪器增益相位测试判据
	float m_fInstrumentGainPhaseLimit;
	// 仪器共模测试判据
	float m_fInstrumentCMRRLimit;
	// 野外电阻测试判据，下限
	float m_fSensorResistanceLimitMin;
	// 野外电阻测试判据，上限
	float m_fSensorResistanceLimitMax;
	// 野外漏电测试判据
	float m_fSensorLeakageLimit;
	// 野外噪声测试判据
	float m_fSensorNoiseLimit;
	// 野外倾斜度测试判据
	float m_fSensorTiltLimit;
	// 野外倾斜度模型测试判据
	float m_fSensorTiltModelLimit;
	// 地震监测测试数据
	float m_fSeisMonitorLimit;
public:
 	// 处理函数
 	void OnProc(void);
	// 将帧内容加入待处理任务中
	void SaveRecFrameToTask(m_oCommFrameStructPtr ptrFrame);
	// 处理接收命令函数
	void OnProcRecCmd(unsigned short usCmd, char* pChar, unsigned int uiSize);
	// 处理仪器设备表更新
	void OnProcInstrumentTableUpdate(void);
	// 处理上电
	void OnProcSetFieldOn(void);
	// 处理断电
	void OnProcSetFieldOff(void);
	// 查询 SurveyXML 文件信息
	void OnProcQuerySurveyXMLInfo(unsigned short usCmd);
	// 查询 PointCode XML文件信息
	void OnProcQueryPointCodeXMLInfo(unsigned short usCmd);
	// 查询 Sensor XML文件信息
	void OnProcQuerySensorXMLInfo(unsigned short usCmd);
	// 查询 Marker XML文件信息
	void OnProcQueryMarkerXMLInfo(unsigned short usCmd);
	// 查询 Aux XML文件信息
	void OnProcQueryAuxXMLInfo(unsigned short usCmd);
	// 查询 Detour XML文件信息
	void OnProcQueryDetourXMLInfo(unsigned short usCmd);
	// 查询 Mute XML文件信息
	void OnProcQueryMuteXMLInfo(unsigned short usCmd);
	// 查询 BlastMachine XML文件信息
	void OnProcQueryBlastMachineXMLInfo(unsigned short usCmd);
	// 查询 Absolute XML文件信息
	void OnProcQueryAbsoluteXMLInfo(unsigned short usCmd);
	// 查询 Generic XML文件信息
	void OnProcQueryGenericXMLInfo(unsigned short usCmd);
	// 查询 Look XML文件信息
	void OnProcQueryLookXMLInfo(unsigned short usCmd);
	// 查询 InstrumentTestBase XML文件信息
	void OnProcQueryInstrumentTestBaseXMLInfo(unsigned short usCmd);
	// 查询 SensorTestBase XML文件信息
	void OnProcQuerySensorTestBaseXMLInfo(unsigned short usCmd);
	// 查询 InstrumentTestLimit XML文件信息
	void OnProcQueryInstrumentTestLimitXMLInfo(unsigned short usCmd);
	// 查询 SensorTestLimit XML文件信息
	void OnProcQuerySensorTestLimitXMLInfo(unsigned short usCmd);
	// 查询 InstrumentTest XML文件信息
	void OnProcQueryInstrumentTestXMLInfo(unsigned short usCmd);
	// 查询 SensorTest XML文件信息
	void OnProcQuerySensorTestXMLInfo(unsigned short usCmd);
	// 查询 MultipleTest XML文件信息
	void OnProcQueryMultipleTestXMLInfo(unsigned short usCmd);
	// 查询 SeisMonitorTest XML文件信息
	void OnProcQuerySeisMonitorTestXMLInfo(unsigned short usCmd);
	// 查询 LAULeakage XML文件信息
	void OnProcQueryLAULeakageXMLInfo(unsigned short usCmd);
	// 查询 FormLine XML文件信息
	void OnProcQueryFormLineXMLInfo(unsigned short usCmd);
	// 由线号和点号得到区域位置
	void GetAreaFromPoint(int iLineIndex, int iPointIndex, m_oAreaStruct* pAreaStruct);
	// 由区域号得到线号及点号范围
	void GetPointRangeFromArea(int* iLineIndex, int* iPointMinIndex, int* iPointMaxIndex, m_oAreaStruct* pAreaStruct);
	// 判断仪器更新区域是否已加入索引表
	BOOL IfAreaExistInMap(m_oAreaStruct* pAreaStruct, 
		map<m_oAreaStruct, m_oAreaStruct>* pMap);
	// 增加对象到索引表
	void AddAreaToMap(int iLineIndex, int iPointIndex, 
		map<m_oAreaStruct, m_oAreaStruct>* pMap);
	// 处理仪器信息查询
	void OnProcQueryByArea(char* pChar, unsigned int uiSize, 
		unsigned int(CClientRecThread::*ptrFun)(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos));
	// 处理全部信息查询
	void OnProcQueryInfoAll(unsigned int(CClientRecThread::*ptrFun)(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos));
	// 按区域查询仪器信息
	unsigned int QueryByArea(m_oAreaStruct* pArea, unsigned int uiStartPos, 
		unsigned int(CClientRecThread::*ptrFun)(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos));
	// 按区域查询仪器信息
	unsigned int QueryInstrumentInfoByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos);
	// 查询所选区域仪器噪声测试数据和测试结果
	unsigned int QueryInstrNoiseTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos);
	// 查询所选区域仪器失真测试数据和测试结果
	unsigned int QueryInstrDistortionTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos);
	// 查询所选区域仪器串扰测试数据和测试结果
	unsigned int QueryInstrCrosstalkTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos);
	// 查询所选区域仪器共模抑制比测试数据和测试结果
	unsigned int QueryInstrCMRRTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos);
	// 查询所选区域仪器增益相位测试数据和测试结果
	unsigned int QueryInstrGainPhaseTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos);
	// 查询所选区域检波器阻抗测试数据和测试结果
	unsigned int QuerySensorResistanceTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos);
	// 查询所选区域检波器漏电测试数据和测试结果
	unsigned int QuerySensorLeakageTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos);
	// 查询所选区域检波器噪声测试数据和测试结果
	unsigned int QuerySensorNoiseTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos);
	// 查询所选区域检波器倾斜度测试数据和测试结果
	unsigned int QuerySensorTiltTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos);
	// 查询所选区域检波器倾斜度模式测试数据和测试结果
	unsigned int QuerySensorTiltModelTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos);
	// 查询所选区域地震监测测试数据和测试结果
	unsigned int QuerySeisMonitorTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos);

	// 从XML配置文件得到测试数据限制值
	void GetTestDataLimitFromXML(bool bInstrument);
	// 计算测试数据的算术均方根
	double CalTestDataMeanSquare(m_oInstrumentStruct* pInstrument);
};

