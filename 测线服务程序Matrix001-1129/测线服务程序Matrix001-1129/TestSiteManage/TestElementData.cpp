#include "StdAfx.h"
#include "TestElementData.h"
#include "Matrixline.h"

CTestElementData::CTestElementData()
{
}

CTestElementData::~CTestElementData()
{
}

/**
* 重置
* @param void
* @return void
*/
void CTestElementData::Reset()
{
	m_bInUsed = false;	// 是否使用中	
	m_uiIP = 0;	// IP地址
	m_uiDataStep = 0;	// 数据索引进位基数
	m_uiFrameIndex = 0;	// 帧计数
	m_uiDataIndexPreview = 0;	// 上一次的数据索引
	m_uiDataBegin=-1;			// 数据索引初始偏移量
	IndexPreview=-1;
	m_uiADCSetOperationNb = 0;

	// 重置测试
	ResetTest();
}

/**
* 重置测试
* @param void
* @return void
*/
void CTestElementData::ResetTest()
{
	// 仪器噪声测试值
	m_fInstrumentNoiseValue = 0.0;
	// 仪器噪声测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byInstrumentNoiseCheck = 0;
	// 仪器噪声测试判据
	m_fInstrumentNoiseLimit = 0.0;

	// 仪器畸变测试值
	m_fInstrumentDistortionValue = 0.0;
	// 仪器畸变测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byInstrumentDistortionCheck = 0;
	// 仪器畸变测试判据
	m_fInstrumentDistortionLimit = 0.0;

	// 仪器串音测试值
	m_fInstrumentCrosstalkValue = 0.0;
	// 仪器串音测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byInstrumentCrosstalkCheck = 0;
	// 仪器串音测试判据
	m_fInstrumentCrosstalkLimit = 0.0;

	// 仪器增益测试值
	m_fInstrumentGainValue = 0.0;
	// 仪器增益测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byInstrumentGainCheck = 0;
	// 仪器增益测试判据
	m_fInstrumentGainLimit = 0.0;
	// 仪器相位测试值
	m_fInstrumentPhaseValue = 0.0;
	// 仪器相位测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byInstrumentPhaseCheck = 0;
	// 仪器相位测试判据
	m_fInstrumentPhaseLimit = 0.0;
	// 仪器增益和相位测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byInstrumentGainAndPhaseCheck = 0;

	// 仪器共模测试值
	m_fInstrumentCMRRValue = 0.0;
	// 仪器共模测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byInstrumentCMRRCheck = 0;
	// 仪器共模测试判据
	m_fInstrumentCMRRLimit = 0.0;

	// 仪器测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号
	m_byInstrumentCheck = 0;

	// 野外电阻测试值
	m_fSensorResistanceValue = 0.0;
	// 野外电阻测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_bySensorResistanceCheck = 0;
	/** 野外电阻测试判据，下限*/
	m_fSensorResistanceLimitMin = 0.0;
	/** 野外电阻测试判据，上限*/
	m_fSensorResistanceLimitMax = 0.0;

	// 野外漏电测试值
	m_fSensorLeakageValue = 0.0;
	// 野外漏电测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_bySensorLeakageCheck = 0;
	// 野外漏电测试判据
	m_fSensorLeakageLimit = 0.0;

	// 野外噪声测试值
	m_fSensorNoiseValue = 0.0;
	// 野外噪声测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_bySensorNoiseCheck = 0;
	// 野外噪声测试判据
	m_fSensorNoiseLimit = 0.0;

	// 野外倾斜度测试值
	m_fSensorTiltValue = 0.0;
	// 野外倾斜度测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_bySensorTiltCheck = 0;
	// 野外倾斜度测试判据
	m_fSensorTiltLimit = 0.0;

	// 野外倾斜度模型测试值
	m_fSensorTiltModelValue = 0.0;
	// 野外倾斜度模型测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_bySensorTiltModelCheck = 0;
	// 野外倾斜度模型测试判据
	m_fSensorTiltModelLimit = 0.0;

	// 野外测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_bySensorCheck = 0;

	// 噪声监测测试值
	m_fSensorNoiseMonitorValue = 0.0;
	// 噪声监测测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_bySensorNoiseMonitorCheck = 0;

	// 检波器是否进行了初始测试
	m_byTestOnField = 0;
	// 初始测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
	m_byOnFieldCheck = 0;
	m_uigroupendcount=0;
}

/**
* 重置，为一次新的测试请求
* @param void
* @return void
*/
void CTestElementData::ResetForNewTestRequest()
{
	m_uiDataStep = 0;	// 数据索引进位基数
	m_uiFrameIndex = 0;	// 帧计数
	m_uiDataIndexPreview = 0;	// 上一次的数据索引
	m_uiDataBegin=-1;			// 数据索引初始偏移量
	IndexPreview=-1;

	// 重置测试
	ResetTest();
}

float CTestElementData::MeanSquare(byte* pData,unsigned int uiSamplingPointCount)
{
	int uiData,iNoiseGain;
	double iAVG=0,Value2=0,m_AVGNoiseValue,itmp;
	//iNoiseGain=NoiseGain;
	iNoiseGain=1;
	char charTemp[128];
	CString strOutput = _T("");
	for(int j = 0; j < uiSamplingPointCount; j++){
		memcpy(&uiData, &pData[(j) * 4], 4);
		if(uiData>0x800000)
			uiData=(0xffffff-uiData);
		//sprintf_s(charTemp,"%d \t",uiData);
		//strOutput += charTemp;
		if(uiData>0x4000)
			uiData=uiData;
		iAVG+=(double)uiData*iNoiseGain;
	}
	//TRACE1("strOutput=%s\r\n",strOutput);
	iAVG=iAVG/uiSamplingPointCount;
	for(int i = 0; i < uiSamplingPointCount; i++){
		memcpy(&uiData, &pData[(i) * 4], 4);
		if(uiData>0x800000)
			uiData=(0xffffff-uiData);
		itmp=(double)uiData*iNoiseGain;
		Value2+=(itmp-iAVG)*(itmp-iAVG);
	}
	Value2=Value2/(uiSamplingPointCount-1);
	if(Value2==0)
		Value2=1.0;
	m_AVGNoiseValue=pow(Value2,0.5);
	return (float)m_AVGNoiseValue;
}
/**
* 计算得到测试结果
* @param unsigned int uiTestType 测试类型
* @param byte* pData 测试数据
* @param unsigned int uiSamplingPointCount 采样点数量 单位个
* @return void
*/
void CTestElementData::CalculateTestResult(unsigned int uiTestType, byte* pData, unsigned int uiSamplingPointCount,float m_AVGNoiseValue)
{
	int uiData = 0;
	//memcpy(&uiData, &pData[(FrameADCount) * 4], 4);	
	memcpy(&uiData, &pData[(uiSamplingPointCount-1) * 4], 4);	
	if(uiTestType==22)
		m_AVGNoiseValue=MeanSquare(pData,uiSamplingPointCount);
	else
		m_AVGNoiseValue=MeanSquare(pData+(FrameADCount)*4,FrameADCount);
	//uiData=m_AVGNoiseValue;
	if(uiData>0x800000){
		uiData=-(0xffffff-uiData);
	}
	switch(uiTestType)
	{
		// 仪器测试
		// 仪器测试，仪器噪声
	case 4:
		if(0x7FFFFF == uiData)
		//if(0x7fffff == uiData)
		{
			m_byInstrumentNoiseCheck = 3;
		}
		else
		{
			m_byInstrumentNoiseCheck = 4;
		}
		//转换成uV
		m_fInstrumentNoiseValue =(float)(m_AVGNoiseValue*5/16.777216);
		//TRACE1("m_AVGNoiseValue=%f\r\n",m_AVGNoiseValue);
		//m_fInstrumentNoiseValue =(float)uiData*NoiseGain*5/16.777216;
		break;
		// 	仪器测试，仪器畸变
	case 5:
		if(0x7fffff == uiData)
		{
			m_byInstrumentDistortionCheck = 3;
		}
		else
		{
			m_byInstrumentDistortionCheck = 4;
		}
		m_fInstrumentDistortionValue = (float)uiData;
		break;
		// 仪器测试，仪器串音
	case 7:	
		if(0x7fffff == uiData)
		{
			m_byInstrumentCrosstalkCheck = 3;
		}
		else
		{
			m_byInstrumentCrosstalkCheck = 4;
		}
		m_fInstrumentCrosstalkValue = (float)uiData;
		break;
		// 仪器测试，仪器增益和相位
	case 6:
		if(0x7fffff == uiData)
		{
			m_byInstrumentGainCheck = 3;
			m_byInstrumentPhaseCheck = 3;
			m_byInstrumentGainAndPhaseCheck = 3;
		}
		else
		{
			m_byInstrumentGainCheck = 4;
			m_byInstrumentPhaseCheck = 4;
			m_byInstrumentGainAndPhaseCheck = 4;
		}
		// 仪器增益测试值
		m_fInstrumentGainValue = (float)uiData;
		// 仪器相位测试值
		m_fInstrumentPhaseValue = (float)uiData;
		break;
		// 仪器测试，仪器共模
	case 8:
		if(0x7fffff == uiData)
		{
			m_byInstrumentCMRRCheck = 3;
		}
		else
		{
			m_byInstrumentCMRRCheck = 4;
		}
		m_fInstrumentCMRRValue = (float)uiData;
		break;
		// 检波器测试
		// 检波器测试，野外电阻
	case 15:
		if(0x7fffff == uiData)
		{
			m_bySensorResistanceCheck = 3;
		}
		else
		{
			m_bySensorResistanceCheck = 4;
		}
		m_fSensorResistanceValue = (float)uiData;
		break;
		// 检波器测试，野外漏电
	case 14:
		if(0x7fffff == uiData)
		{
			m_bySensorLeakageCheck = 3;
		}
		else
		{
			m_bySensorLeakageCheck = 4;
		}
		m_fSensorLeakageValue = (float)uiData;
		break;
		// 检波器测试，野外噪声
	case 1:
		if(0x7fffff == uiData)
		{
			m_bySensorNoiseCheck = 3;
		}
		else
		{
			m_bySensorNoiseCheck = 4;
		}
		m_fSensorNoiseValue = (float)uiData;
		break;
		// 检波器测试，野外倾斜度
	case 20:
		if(0x7fffff == uiData)
		{
			m_bySensorTiltCheck = 3;
		}
		else
		{
			m_bySensorTiltCheck = 4;
		}
		m_fSensorTiltValue = (float)uiData;
		break;
		// 检波器测试，野外倾斜度模式
	case 21:
		if(0x7fffff == uiData)
		{
			m_bySensorTiltModelCheck = 3;
		}
		else
		{
			m_bySensorTiltModelCheck = 4;
		}
		m_fSensorTiltModelValue = (float)uiData;
		break;
		// 检波器测试，噪声监测
	case 22:
		if(0x7fffff == uiData)
		{
			m_bySensorNoiseMonitorCheck = 3;
		}
		else
		{
			m_bySensorNoiseMonitorCheck = 4;
		}
		
/*		//还原值
		if(uiData<0)
			uiData=abs(uiData);
		if(uiData==0)
			uiData++;
		//m_fSensorNoiseMonitorValue = 20.0 * (float)log(((double)uiData) / 16777216.0);
		//折成-102db
		//m_fSensorNoiseMonitorValue = 170 * (float)log(((double)uiData/16777216.0));
		m_fSensorNoiseMonitorValue = 20 * (float)log(((double)uiData*5*30/16777216.0) );
		//TRACE2("MrValue =%d %f ",uiData, m_fSensorNoiseMonitorValue);
		
		//m_fSensorNoiseMonitorValue =(float)uiData-100;
		
*/		m_fSensorNoiseMonitorValue=20 * (float)log10(m_AVGNoiseValue*5/16777216.0 );
		break;
	}
}

/**
* 判读测试结果
* @param unsigned int uiTestType 测试类型
* @return void
*/
void CTestElementData::JudgeTestResult(unsigned int uiTestType)
{
	switch(uiTestType)
	{
		// 仪器测试
		// 仪器测试，仪器噪声
	case 4:
		if(4 == m_byInstrumentNoiseCheck)
		{
			m_byInstrumentNoiseCheck = 1;		// by zl 3.31
			//正负1/2m_fInstrumentNoiseLimit有效
			if(m_fInstrumentNoiseValue>m_fInstrumentNoiseLimit || m_fInstrumentNoiseValue<-m_fInstrumentNoiseLimit)
				m_byInstrumentNoiseCheck = 2;
		}
		break;
		// 	仪器测试，仪器畸变
	case 5:
		if(4 == m_byInstrumentDistortionCheck)
		{
			m_byInstrumentDistortionCheck = 1;
			if(m_fInstrumentDistortionValue>m_fInstrumentDistortionLimit || m_fInstrumentDistortionValue<-m_fInstrumentDistortionLimit)
				m_byInstrumentDistortionCheck = 2;
		}
		break;
		// 仪器测试，仪器串音
	case 7:	
		if(4 == m_byInstrumentCrosstalkCheck)
		{
			m_byInstrumentCrosstalkCheck = 1;
			if(m_fInstrumentCrosstalkValue>m_fInstrumentCrosstalkLimit || m_fInstrumentCrosstalkValue<-m_fInstrumentCrosstalkLimit)
				m_byInstrumentCrosstalkCheck = 2;
		}
		break;
		// 仪器测试，仪器增益和相位
	case 6:
		if(4 == m_byInstrumentGainCheck)
		{
			m_byInstrumentGainCheck = 1;
			if(m_fInstrumentGainValue>m_fInstrumentGainLimit || m_fInstrumentGainValue<-m_fInstrumentGainLimit)
				m_byInstrumentGainCheck = 2;
		}
		if(4 == m_byInstrumentPhaseCheck)
		{
			m_byInstrumentPhaseCheck = 1;
			if(m_fInstrumentGainValue>m_fInstrumentPhaseLimit || m_fInstrumentGainValue<-m_fInstrumentPhaseLimit)
				m_byInstrumentPhaseCheck = 2;
		}
		if((1 == m_byInstrumentGainCheck) && (1 == m_byInstrumentPhaseCheck))
		{
			m_byInstrumentGainAndPhaseCheck = 1;
		}
		if((2 == m_byInstrumentGainCheck) || (2 == m_byInstrumentPhaseCheck))
		{
			m_byInstrumentGainAndPhaseCheck = 2;
		}
		break;
		// 仪器测试，仪器共模
	case 8:
		if(4 == m_byInstrumentCMRRCheck)
		{
			m_byInstrumentCMRRCheck = 1;
			if(m_fInstrumentCMRRValue>m_fInstrumentCMRRLimit || m_fInstrumentCMRRValue<-m_fInstrumentCMRRLimit)
				m_byInstrumentCMRRCheck = 2;
		}
		break;
		// 检波器测试
		// 检波器测试，野外电阻
	case 15:
		if(4 == m_bySensorResistanceCheck)
		{
			m_bySensorResistanceCheck = 1;
			if(m_fSensorResistanceValue>m_fSensorResistanceLimitMin || m_fSensorResistanceValue<-m_fSensorResistanceLimitMin)
				m_bySensorResistanceCheck = 2;
		}
		break;
		// 检波器测试，野外漏电
	case 14:
		if(4 == m_bySensorLeakageCheck)
		{
			m_bySensorLeakageCheck = 1;
			if(m_fSensorLeakageValue>m_fSensorLeakageLimit || m_fSensorLeakageValue<-m_fSensorLeakageLimit)
				m_bySensorLeakageCheck = 2;
		}
		break;
		// 检波器测试，野外噪声
	case 1:
		if(4 == m_bySensorNoiseCheck)
		{
			m_bySensorNoiseCheck = 1;
			if(m_fSensorNoiseValue>m_fSensorNoiseLimit || m_fSensorNoiseValue<-m_fSensorNoiseLimit)
				m_bySensorNoiseCheck = 2;
		}
		break;
		// 检波器测试，野外倾斜度
	case 20:
		if(4 == m_bySensorTiltCheck)
		{
			m_bySensorTiltCheck = 1;
			if(m_fSensorTiltValue>m_fSensorTiltLimit || m_fSensorTiltValue<-m_fSensorTiltLimit)
				m_bySensorTiltCheck = 2;
		}
		break;
		// 检波器测试，野外倾斜度模型
	case 21:
		if(4 == m_bySensorTiltModelCheck)
		{
			m_bySensorTiltModelCheck = 1;
			if(m_fSensorTiltModelValue>m_fSensorTiltModelLimit || m_fSensorTiltModelValue<-m_fSensorTiltModelLimit)
				m_bySensorTiltModelCheck = 2;
		}
		break;
	}
}

/**
* 得到测试结果
* @param unsigned int uiTestType 测试类型
* @param byte* pData 测试数据
* @return int 数据占用字节数 
*/
int CTestElementData::GetTestResult(unsigned int uiTestType, byte* pData)
{
	int iPosition = 0;
	memcpy(&pData[iPosition], &m_uiIP, 4);
	iPosition = iPosition + 4;
	switch(uiTestType)
	{
		// 仪器测试
		// 仪器测试，仪器噪声
	case 4:
		memcpy(&pData[iPosition], &m_fInstrumentNoiseValue, 4);
		iPosition = iPosition + 4;
		memcpy(&pData[iPosition], &m_byInstrumentNoiseCheck, 1);
		iPosition = iPosition + 1;
		memcpy(&pData[iPosition], &m_byInstrumentCheck, 1);
		iPosition = iPosition + 1;
		break;
		// 	仪器测试，仪器畸变
	case 5:
		memcpy(&pData[iPosition], &m_fInstrumentDistortionValue, 4);
		iPosition = iPosition + 4;
		memcpy(&pData[iPosition], &m_byInstrumentDistortionCheck, 1);
		iPosition = iPosition + 1;
		memcpy(&pData[iPosition], &m_byInstrumentCheck, 1);
		iPosition = iPosition + 1;
		break;
	// 仪器测试，仪器串音
	case 7:		
		memcpy(&pData[iPosition], &m_fInstrumentCrosstalkValue, 4);
		iPosition = iPosition + 4;
		memcpy(&pData[iPosition], &m_byInstrumentCrosstalkCheck, 1);
		iPosition = iPosition + 1;
		memcpy(&pData[iPosition], &m_byInstrumentCheck, 1);
		iPosition = iPosition + 1;
		break;
		// 仪器测试，仪器增益和相位
	case 6:
		memcpy(&pData[iPosition], &m_fInstrumentGainValue, 4);
		iPosition = iPosition + 4;
		memcpy(&pData[iPosition], &m_byInstrumentGainCheck, 1);
		iPosition = iPosition + 1;
		memcpy(&pData[iPosition], &m_fInstrumentPhaseValue, 4);
		iPosition = iPosition + 4;
		memcpy(&pData[iPosition], &m_byInstrumentPhaseCheck, 1);
		iPosition = iPosition + 1;
		memcpy(&pData[iPosition], &m_byInstrumentGainAndPhaseCheck, 1);
		iPosition = iPosition + 1;
		memcpy(&pData[iPosition], &m_byInstrumentCheck, 1);
		iPosition = iPosition + 1;
		break;
		// 仪器测试，仪器共模
	case 8:
		memcpy(&pData[iPosition], &m_fInstrumentCMRRValue, 4);
		iPosition = iPosition + 4;
		memcpy(&pData[iPosition], &m_byInstrumentCMRRCheck, 1);
		iPosition = iPosition + 1;
		memcpy(&pData[iPosition], &m_byInstrumentCheck, 1);
		iPosition = iPosition + 1;
		break;
		// 检波器测试
		// 检波器测试，野外电阻
	case 15:
		memcpy(&pData[iPosition], &m_fSensorResistanceValue, 4);
		iPosition = iPosition + 4;
		memcpy(&pData[iPosition], &m_bySensorResistanceCheck, 1);
		iPosition = iPosition + 1;
		memcpy(&pData[iPosition], &m_bySensorCheck, 1);
		iPosition = iPosition + 1;
		break;
		// 检波器测试，野外漏电
	case 14:
		memcpy(&pData[iPosition], &m_fSensorLeakageValue, 4);
		iPosition = iPosition + 4;
		memcpy(&pData[iPosition], &m_bySensorLeakageCheck, 1);
		iPosition = iPosition + 1;
		memcpy(&pData[iPosition], &m_bySensorCheck, 1);
		iPosition = iPosition + 1;
		break;
		// 检波器测试，野外噪声
	case 1:
		memcpy(&pData[iPosition], &m_fSensorNoiseValue, 4);
		iPosition = iPosition + 4;
		memcpy(&pData[iPosition], &m_bySensorNoiseCheck, 1);
		iPosition = iPosition + 1;
		memcpy(&pData[iPosition], &m_bySensorCheck, 1);
		iPosition = iPosition + 1;
		break;
		// 检波器测试，野外倾斜度
	case 20:
		memcpy(&pData[iPosition], &m_fSensorTiltValue, 4);
		iPosition = iPosition + 4;
		memcpy(&pData[iPosition], &m_bySensorTiltCheck, 1);
		iPosition = iPosition + 1;
		memcpy(&pData[iPosition], &m_bySensorCheck, 1);
		iPosition = iPosition + 1;
		break;
		// 检波器测试，野外倾斜度模式
	case 21:		
		memcpy(&pData[iPosition], &m_fSensorTiltModelValue, 4);
		iPosition = iPosition + 4;
		memcpy(&pData[iPosition], &m_bySensorTiltModelCheck, 1);
		iPosition = iPosition + 1;
		memcpy(&pData[iPosition], &m_bySensorCheck, 1);
		iPosition = iPosition + 1;
		break;
		// 检波器测试，野外噪声监测
	case 22:		
		memcpy(&pData[iPosition], &m_fSensorNoiseMonitorValue, 4);
		iPosition = iPosition + 4;
		memcpy(&pData[iPosition], &m_bySensorNoiseMonitorCheck, 1);
		iPosition = iPosition + 1;
		break;
		// 检波器测试，新加检波器的初始测试
	case 45:		
		memcpy(&pData[iPosition], &m_byTestOnField, 1);
		iPosition = iPosition + 1;
		memcpy(&pData[iPosition], &m_byOnFieldCheck, 1);
		iPosition = iPosition + 1;
		break;
	}
	return iPosition;
}