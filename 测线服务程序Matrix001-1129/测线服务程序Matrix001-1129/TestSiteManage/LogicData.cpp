#include "StdAfx.h"
#include "LogicData.h"

CLogicData::CLogicData()
{
}

CLogicData::~CLogicData()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CLogicData::OnInit()
{
	// 打开配置文件
	OpenIniXMLFile();

	// 测区设置对象
	m_oSurveySetupData.m_pXMLDOMDocument = &m_oXMLDOMDocument;	// XMLDOM文件对象
	m_oSurveySetupData.m_uiLineCountAll = m_uiLineCountAll;	// 测线数组总数	
	m_oSurveySetupData.m_uiPointCountAll = m_uiPointCountAll;	// 测点数组总数	
	m_oSurveySetupData.m_uiChannelCountAll = m_uiChannelCountAll;	// 测道数组总数
	m_oSurveySetupData.OnInit();

	// 布局设置对象
	m_oLayoutSetupData.m_pXMLDOMDocument = &m_oXMLDOMDocument;	// XMLDOM文件对象
	m_oLayoutSetupData.OnInit();
	// 设置哑点到测点和测道
	SetMuteToCSurveySetupData(true);

	// Look设置对象
	m_oLookSetupData.m_pXMLDOMDocument = &m_oXMLDOMDocument;	// XMLDOM文件对象
	m_oLookSetupData.OnInit();

	// 测试设置对象
	m_oTestSetupData.m_pXMLDOMDocument = &m_oXMLDOMDocument;	// XMLDOM文件对象
	m_oTestSetupData.m_uiTestElementCountAll = m_uiTestElementCountAll;	// // 测试单元数组总数
	m_oTestSetupData.OnInit();

	// 关闭配置文件
	CloseIniXMLFile();
}

/**
* 重置
* @param void
* @return void
*/
void CLogicData::OnReset()
{
	// 测区设置对象
	m_oSurveySetupData.OnReset();
	// 测试设置对象
	m_oTestSetupData.OnReset();
}

/**
* 重新加载
* @param void
* @return void
*/
void CLogicData::OnReload()
{
	// 打开配置文件
	OpenIniXMLFile();

	// 测区设置对象
	m_oSurveySetupData.OnReload();
	// 布局设置对象
	m_oLayoutSetupData.OnReload();
	// Look设置对象
	m_oLookSetupData.OnReload();
	// 测试设置对象
	m_oTestSetupData.OnReload();

	// 关闭配置文件
	CloseIniXMLFile();
}

/**
* 关闭
* @param void
* @return void
*/
void CLogicData::OnClose()
{
	// 测区设置对象
	m_oSurveySetupData.OnClose();
	// 布局设置对象
	m_oLayoutSetupData.OnClose();
	// Look设置对象
	m_oLookSetupData.OnClose();
	// 测试设置对象
	m_oTestSetupData.OnClose();
}

/**
* 打开配置文件
* @param void
* @return void
*/
void CLogicData::OpenIniXMLFile()
{
	// 由FTP服务器得到Matrix设置文件
	GetMatrixIniFileFromFTPServer();

	// 初始化COM库
	CoInitialize(NULL);

	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;

	strOLEObject = "msxml2.domdocument";
	BOOL bData = m_oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);

	oVariant = m_strIniFile;
	bData = m_oXMLDOMDocument.load(oVariant);
}

/**
* 关闭配置文件
* @param void
* @return void
*/
void CLogicData::CloseIniXMLFile()
{
	m_oXMLDOMDocument.DetachDispatch();
	// 释放COM库
	CoUninitialize();
}

/**
* 由FTP服务器得到Matrix设置文件
* @param void
* @return bool true：成功；false：失败
*/
bool CLogicData::GetMatrixIniFileFromFTPServer()
{
	bool bReturn = false;
	BOOL bData;
	int iCount = 0;
	CInternetSession oSession;
	CFtpConnection* pConnection = oSession.GetFtpConnection(m_strIPFTPServer);
	CString strFileSource = "";
	strFileSource = m_strIniFile.Right(m_strIniFile.GetLength() - 2);
	
	while(true)
	{
		bData = pConnection->GetFile(strFileSource, m_strIniFile, FALSE);
		if(TRUE == bData)
		{
			bReturn = true;
			break;
		}
		iCount++;
		if(4 <= iCount)
		{
			break;
		}
		Sleep(50);
	}
	pConnection->Close();
	delete pConnection;
	return bReturn;
}

/**
* 设置测点和测道的哑点属性
* @param bool bValue 是否是哑点 true：是；false：否
* @return void
*/
void CLogicData::SetMuteToCSurveySetupData(bool bValue)
{
	POSITION pos = NULL;
	CMuteData* pMuteData = NULL;
	CPointSetupData* pPointSetupData = NULL;
	CChannelSetupData* pChannelSetupData = NULL;
	pos = m_oLayoutSetupData.m_oMuteList.m_olsMute.GetHeadPosition();
	while(NULL != pos)
	{
		pMuteData = m_oLayoutSetupData.m_oMuteList.m_olsMute.GetNext(pos);
		pPointSetupData = m_oSurveySetupData.m_oPointList.GetPoint(pMuteData->m_uiLineName, pMuteData->m_uiPointNb);
		if(NULL != pPointSetupData)
		{
			pPointSetupData->m_bMute = bValue;
			pChannelSetupData = pPointSetupData->m_pChannelHead;
			while(true)
			{
				pChannelSetupData->m_bMute = bValue;
				if(pChannelSetupData == pPointSetupData->m_pChannelTail)
				{
					break;
				}
				pChannelSetupData = pChannelSetupData->m_pChannelRight;
			}
		}
	}
}

/**
* 设置基本测试项目，来自测试设置的测试任务
* @param unsigned int uiTestNb 测试任务号
* @return bool true：成功；false：失败
*/
bool CLogicData::OnSetupTestProjectForTestBaseFromTestSetup(unsigned int uiTestNb)
{
	// 设置测试项目
	if(false == m_oTestSetupData.OnSetupTestProjectForTestBaseFromTestSetup(uiTestNb))
	{
		return false;
	}
	// 清空仪器IP地址队列
	m_oTestSetupData.m_oTestProjectBase.m_olsIP.RemoveAll();

	// 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测
	if((1 == m_oTestSetupData.m_oTestProjectBase.m_uiProjectType) || (3 == m_oTestSetupData.m_oTestProjectBase.m_uiProjectType))
	{
		// 解析排列，得到IP地址队列，辅助道
		if(false == m_oLayoutSetupData.ParseSpreadToIPListByAuxiliaryDescr(&m_oTestSetupData.m_oTestProjectBase.m_olsIP, m_oTestSetupData.m_oTestProjectBase.m_strAuxiliaryDescr))
		{
			return false;
		}
	}

	// 解析排列，得到IP地址队列，采集道
	if(false == m_oSurveySetupData.ParseSpreadToIPListByAbsoluteSpread(&m_oTestSetupData.m_oTestProjectBase.m_olsIP, m_oTestSetupData.m_oTestProjectBase.m_strAbsoluteSpread))
	{
		return false;
	}
	// 判断：没有要测试的测道
	if(0 == m_oTestSetupData.m_oTestProjectBase.m_olsIP.GetCount())
	{
		return false;
	}
	// 设置测试单元，基本测试
	m_oTestSetupData.OnSetupTestElementBase();
	// 设置测试数据缓冲区，基本测试
	m_oTestSetupData.OnSetupTestDataBufferBase();

	return true;
}

/**
* 设置基本测试项目，来自测试视图的测试任务
* @param unsigned int uiTestAim 测试目标
* @param unsigned int uiTestType 测试类型
* @param CString strTestAbsoluteSpread 绝对排列
* @return bool true：成功；false：失败
*/
bool CLogicData::OnSetupTestProjectForTestBaseFromTestView(unsigned int uiTestAim, unsigned int uiTestType, CString strTestAbsoluteSpread)
{
	// 设置基本测试项目，来自测试图形的测试任务
	if(false == m_oTestSetupData.OnSetupTestProjectForTestBaseFromTestView(uiTestAim, uiTestType, strTestAbsoluteSpread))
	{
		return false;
	}
	
	// 清空仪器IP地址队列
	m_oTestSetupData.m_oTestProjectBase.m_olsIP.RemoveAll();

	// 解析排列，得到IP地址队列，采集道
	if(false == m_oSurveySetupData.ParseSpreadToIPListByAbsoluteSpread(&m_oTestSetupData.m_oTestProjectBase.m_olsIP, m_oTestSetupData.m_oTestProjectBase.m_strAbsoluteSpread))
	{
		return false;
	}
	// 判断：没有要测试的测道
	if(0 == m_oTestSetupData.m_oTestProjectBase.m_olsIP.GetCount())
	{
		return false;
	}
	// 设置测试单元，基本测试
	m_oTestSetupData.OnSetupTestElementBase();
	// 设置测试数据缓冲区，基本测试
	m_oTestSetupData.OnSetupTestDataBufferBase();

	return true;
}

/**
* 设置基本测试项目，处理新加检波器的初始测试
* @param void
* @return bool true：成功；false：失败
*/
bool CLogicData::OnSetupTestProjectForSensorTestOnField()
{
	// 判断：不需要测试
	if((0 == m_oLookSetupData.m_uiTestResistance) && (0 == m_oLookSetupData.m_uiTestTilt) && (0 == m_oLookSetupData.m_uiTestLeakage))
	{
		return false;
	}
	// 设置基本测试项目，处理新加检波器的初始测试
	if(false == m_oTestSetupData.OnSetupTestProjectForSensorTestOnField(m_oLookSetupData.m_uiTestResistance, m_oLookSetupData.m_uiTestTilt, m_oLookSetupData.m_uiTestLeakage))
	{
		return false;
	}

	// 清空仪器IP地址队列
	m_oTestSetupData.m_oTestProjectBase.m_olsIP.RemoveAll();

	// 得到IP地址队列，参加新加检波器的初始测试
	if(false == m_oSurveySetupData.GetIPListForSensorTestOnField(&m_oTestSetupData.m_oTestProjectBase.m_olsIP))
	{
		return false;
	}
	// 判断：没有要测试的测道
	if(0 == m_oTestSetupData.m_oTestProjectBase.m_olsIP.GetCount())
	{
		return false;
	}
	// 设置测试单元，基本测试
	m_oTestSetupData.OnSetupTestElementBase();
	// 设置测试数据缓冲区，基本测试
	m_oTestSetupData.OnSetupTestDataBufferBase();

	return true;
}

/**
* 设置噪声监测测试项目，来自测试设置的测试任务
* @param void
* @return bool true：成功；false：失败
*/
bool CLogicData::OnSetupTestProjectForTestNoiseFromTestSetup()
{
	// 设置测试项目
	if(false == m_oTestSetupData.OnSetupTestProjectForTestNoise())
	{
		return false;
	}

	m_oTestSetupData.m_oTestProjectNoise.m_olsIP.RemoveAll();
	// 解析排列，得到IP地址队列，采集道
	if(false == m_oSurveySetupData.ParseSpreadToIPListByAbsoluteSpread(&m_oTestSetupData.m_oTestProjectNoise.m_olsIP, m_oTestSetupData.m_oTestProjectNoise.m_strAbsoluteSpread))
	{
		return false;
	}
	// 判断：没有要测试的测道
	if(0 == m_oTestSetupData.m_oTestProjectNoise.m_olsIP.GetCount())
	{
		return false;
	}
	// 设置测试单元，噪声监测
	m_oTestSetupData.OnSetupTestElementNoise();
	// 设置测试数据缓冲区，噪声监测
	m_oTestSetupData.OnSetupTestDataBufferNoise();

	return true;
}

/**
* 设置噪声监测测试项目，来自噪声监测视图的测试任务
* @param CString strTestAbsoluteSpread 绝对排列
* @return bool true：成功；false：失败
*/
bool CLogicData::OnSetupTestProjectForTestNoiseFromTestView(CString strTestAbsoluteSpread)
{
	// 设置测试项目
	if(false == m_oTestSetupData.OnSetupTestProjectForTestNoise())
	{
		return false;
	}

	m_oTestSetupData.m_oTestProjectNoise.m_strAbsoluteSpread = strTestAbsoluteSpread;
	m_oTestSetupData.m_oTestProjectNoise.m_olsIP.RemoveAll();
	// 解析排列，得到IP地址队列，采集道
	if(false == m_oSurveySetupData.ParseSpreadToIPListByAbsoluteSpread(&m_oTestSetupData.m_oTestProjectNoise.m_olsIP, m_oTestSetupData.m_oTestProjectNoise.m_strAbsoluteSpread))
	{
		return false;
	}
	// 判断：没有要测试的测道
	if(0 == m_oTestSetupData.m_oTestProjectNoise.m_olsIP.GetCount())
	{
		return false;
	}
	// 设置测试单元，噪声监测
	m_oTestSetupData.OnSetupTestElementNoise();
	// 设置测试数据缓冲区，噪声监测
	m_oTestSetupData.OnSetupTestDataBufferNoise();

	return true;
}

/**
* 清除被删除仪器对应测道的仪器SN号
* @param unsigned int uiIP 仪器IP地址
* @return void
*/
void CLogicData::ClearSNInChannelWhenInstrumentDelete(unsigned int uiIP)
{
	CChannelSetupData* pChannel = NULL;
	if(TRUE == m_oSurveySetupData.m_oChannelList.GetChannelFromMap(uiIP, pChannel))
	{
		pChannel->m_uiSN = 0;
	}
}

/**
* 处理配置文件更改	SurveySetup的ApplyAll
* @param void
* @return void
*/
void CLogicData::OnSetupDataChangeForSurveySetupApplyAll()
{
	// 打开配置文件
	OpenIniXMLFile();
	// 测区设置对象
	m_oSurveySetupData.OnReload();
	// 关闭配置文件
	CloseIniXMLFile();
}

/**
* 处理配置文件更改	SurveySetup的ApplySensor
* @param void
* @return void
*/
void CLogicData::OnSetupDataChangeForSurveySetupApplySensor()
{
	// 打开配置文件
	OpenIniXMLFile();
	// 处理配置文件更改	SurveySetup的ApplySensor
	m_oSurveySetupData.m_oSensorList.OnSetupDataChangeForSurveySetupApplySensor();
	// 关闭配置文件
	CloseIniXMLFile();	
}

/**
* 处理配置文件更改	LayoutSetup的ApplyBlast
* @param void
* @return void
*/
void CLogicData::OnSetupDataChangeForLayoutSetupApplyBlast()
{
	// 打开配置文件
	OpenIniXMLFile();
	// 处理配置文件更改	LayoutSetup的ApplySensor
	m_oLayoutSetupData.m_oBlastMachineList.OnReload();
	// 关闭配置文件
	CloseIniXMLFile();	
}

/**
* 处理配置文件更改	LayoutSetup的ApplyAux
* @param void
* @return void
*/
void CLogicData::OnSetupDataChangeForLayoutSetupApplyAux()
{
	// 打开配置文件
	OpenIniXMLFile();
	// 处理配置文件更改	LayoutSetup的ApplySensor
	m_oLayoutSetupData.m_oAuxList.OnReload();
	// 关闭配置文件
	CloseIniXMLFile();	
}

/**
* 处理配置文件更改	LayoutSetup的ApplyMarkers
* @param void
* @return void
*/
void CLogicData::OnSetupDataChangeForLayoutSetupApplyMarkers()
{
	// 打开配置文件
	OpenIniXMLFile();
	// 处理配置文件更改	LayoutSetup的ApplySensor
	m_oLayoutSetupData.m_oMarkerList.OnReload();
	// 关闭配置文件
	CloseIniXMLFile();	
}

/**
* 处理配置文件更改	LayoutSetup的ApplyDetour
* @param void
* @return void
*/
void CLogicData::OnSetupDataChangeForLayoutSetupApplyDetour()
{
	// 打开配置文件
	OpenIniXMLFile();
	// 处理配置文件更改	LayoutSetup的ApplySensor
	m_oLayoutSetupData.m_oDetourList.OnReload();
	// 关闭配置文件
	CloseIniXMLFile();	
}

/**
* 处理配置文件更改	LayoutSetup的ApplyDetour
* @param void
* @return void
*/
void CLogicData::OnSetupDataChangeForLayoutSetupApplyMute()
{
	// 清除设在测点和测道上的哑点标记
	SetMuteToCSurveySetupData(false);	// 设置哑点到测点和测道
	// 打开配置文件
	OpenIniXMLFile();
	// 处理配置文件更改	LayoutSetup的ApplySensor
	m_oLayoutSetupData.m_oMuteList.OnReload();
	// 关闭配置文件
	CloseIniXMLFile();
	// 重新设置测点和测道上的哑点标记
	SetMuteToCSurveySetupData(true);	// 设置哑点到测点和测道
}

/**
* 处理配置文件更改	LookSetup的Apply
* @param void
* @return void
*/
void CLogicData::OnSetupDataChangeForLookSetupApply()
{
	// 打开配置文件
	OpenIniXMLFile();
	// Look设置对象
	m_oLookSetupData.OnReload();
	// 关闭配置文件
	CloseIniXMLFile();
}

/**
* 处理配置文件更改	TestSetup的ApplyInstrument
* @param void
* @return void
*/
void CLogicData::OnSetupDataChangeForTestSetupApplyInstrument()
{
	// 打开配置文件
	OpenIniXMLFile();
	// 重新加载仪器测试配置文件
	m_oTestSetupData.m_oTestProjectList.OnReloadTestSetupDataForApplyInstrument();
	// 关闭配置文件
	CloseIniXMLFile();
}

/**
* 处理配置文件更改	TestSetup的ApplySensor
* @param void
* @return void
*/
void CLogicData::OnSetupDataChangeForTestSetupApplySensor()
{
	// 打开配置文件
	OpenIniXMLFile();
	// 重新加载仪器测试配置文件
	m_oTestSetupData.m_oTestProjectList.OnReloadTestSetupDataForApplySensor();
	// 关闭配置文件
	CloseIniXMLFile();
}

/**
* 处理配置文件更改	TestSetup的ApplyMultiple
* @param void
* @return void
*/
void CLogicData::OnSetupDataChangeForTestSetupApplyMultiple()
{
	// 打开配置文件
	OpenIniXMLFile();
	// 重新加载仪器测试配置文件
	m_oTestSetupData.m_oTestProjectList.OnReloadTestSetupDataForApplyMultiple();
	// 关闭配置文件
	CloseIniXMLFile();
}

/**
* 处理配置文件更改	TestSetup的ApplySeisMonitor
* @param void
* @return void
*/
void CLogicData::OnSetupDataChangeForTestSetupApplySeisMonitor()
{
	// 打开配置文件
	OpenIniXMLFile();
	// 重新加载仪器测试配置文件
	m_oTestSetupData.m_oTestProjectList.OnReloadTestSetupDataForApplySeisMonitor();
	// 关闭配置文件
	CloseIniXMLFile();
}

/**
* 处理配置文件更改	TestSetup的ApplyInstrumentTestBase
* @param void
* @return void
*/
void CLogicData::OnSetupDataChangeForTestSetupApplyInstrumentTestBase()
{
	// 打开配置文件
	OpenIniXMLFile();
	// 重新加载测试配置文件
	m_oTestSetupData.m_oTestBaseListInstrument.OnReloadTestSetupDataForApplyTestBase();
	// 关闭配置文件
	CloseIniXMLFile();
}

/**
* 处理配置文件更改	TestSetup的ApplySensorTestBase
* @param void
* @return void
*/
void CLogicData::OnSetupDataChangeForTestSetupApplySensorTestBase()
{
	// 打开配置文件
	OpenIniXMLFile();
	// 重新加载测试配置文件
	m_oTestSetupData.m_oTestBaseListSensor.OnReloadTestSetupDataForApplyTestBase();
	// 关闭配置文件
	CloseIniXMLFile();
}


/**
* 处理配置文件更改	TestSetup的ApplyInstrumentTestLimit
* @param void
* @return void
*/
void CLogicData::OnSetupDataChangeForTestSetupApplyInstrumentTestLimit()
{
	// 打开配置文件
	OpenIniXMLFile();
	// 重新加载测试配置文件
	m_oTestSetupData.m_oTestLimitListInstrument.OnReloadTestSetupDataForApplyTestLimit();
	// 关闭配置文件
	CloseIniXMLFile();
}

/**
* 处理配置文件更改	TestSetup的ApplySensorTestLimit
* @param void
* @return void
*/
void CLogicData::OnSetupDataChangeForTestSetupApplySensorTestLimit()
{
	// 打开配置文件
	OpenIniXMLFile();
	// 重新加载测试配置文件
	m_oTestSetupData.m_oTestLimitListSensor.OnReloadTestSetupDataForApplyTestLimit();
	// 关闭配置文件
	CloseIniXMLFile();
}

// 设置测试单元队列判据
void CLogicData::SetTestElementListLimit(unsigned int uiType)
{
	switch(uiType)
	{
		// 仪器测试
		// 设置测试单元队列判据，仪器测试，仪器噪声
	case 4:
		SetTestElementListLimitInstrumentNoise();
		break;
		// 	设置测试单元队列判据，仪器测试，仪器畸变
	case 5:
		SetTestElementListLimitInstrumentDistortion();
		break;
		// 设置测试单元队列判据，仪器测试，仪器串音
	case 7:
		SetTestElementListLimitInstrumentCrosstalk();
		break;
		// 设置测试单元队列判据，仪器测试，仪器增益和相位
	case 6:	
		SetTestElementListLimitInstrumentGainAndPhase();
		break;
		// 设置测试单元队列判据，仪器测试，仪器共模
	case 8:
		SetTestElementListLimitInstrumentCMRR();
		break;
		// 检波器测试
		// 设置测试单元队列判据，检波器测试，野外电阻
	case 15:
		SetTestElementListLimitISensorResistance();
		break;
		// 设置测试单元队列判据，检波器测试，野外漏电
	case 14:
		SetTestElementListLimitISensorLeakage();
		break;
		// 设置测试单元队列判据，检波器测试，野外噪声
	case 1:
		SetTestElementListLimitISensorNoise();
		break;
		// 设置测试单元队列判据，检波器测试，野外倾斜度
	case 20:	
		SetTestElementListLimitISensorTilt();
		break;
		// 设置测试单元队列判据，检波器测试，野外倾斜度模式
	case 21:
		SetTestElementListLimitISensorTiltModel();
		break;
	}
}

/**
* 设置测试单元队列判据，仪器测试，仪器噪声
* @param void
* @return void
*/
void CLogicData::SetTestElementListLimitInstrumentNoise()
{
	// 仪器噪声测试判据
	float fLimit = m_oTestSetupData.m_oTestLimitListInstrument.GetTestLimitValueByIndex(400);
	for(unsigned int i = 0; i < m_oTestSetupData.m_oTestElementListBase.m_uiCountUsed; i++)
	{
		// 单元在单元数组中的位置
		m_oTestSetupData.m_oTestElementListBase.m_pArrayTestElementData[i].m_fInstrumentNoiseLimit = fLimit;
	}
}

/**
* 设置测试单元队列判据，仪器测试，仪器畸变
* @param void
* @return void
*/
void CLogicData::SetTestElementListLimitInstrumentDistortion()
{
	// 仪器畸变测试判据
	float fLimit = m_oTestSetupData.m_oTestLimitListInstrument.GetTestLimitValueByIndex(500);
	for(unsigned int i = 0; i < m_oTestSetupData.m_oTestElementListBase.m_uiCountUsed; i++)
	{
		// 单元在单元数组中的位置
		m_oTestSetupData.m_oTestElementListBase.m_pArrayTestElementData[i].m_fInstrumentDistortionLimit = fLimit;
	}
}

/**
* 设置测试单元队列判据，仪器测试，仪器串音
* @param void
* @return void
*/
void CLogicData::SetTestElementListLimitInstrumentCrosstalk()
{
	// 仪器串音测试判据
	float fLimit = m_oTestSetupData.m_oTestLimitListInstrument.GetTestLimitValueByIndex(700);
	for(unsigned int i = 0; i < m_oTestSetupData.m_oTestElementListBase.m_uiCountUsed; i++)
	{
		// 单元在单元数组中的位置
		m_oTestSetupData.m_oTestElementListBase.m_pArrayTestElementData[i].m_fInstrumentCrosstalkLimit = fLimit;
	}
}

/**
* 设置测试单元队列判据，仪器测试，仪器增益和相位
* @param void
* @return void
*/
void CLogicData::SetTestElementListLimitInstrumentGainAndPhase()
{
	// 仪器增益测试判据
	float fLimitGain = m_oTestSetupData.m_oTestLimitListInstrument.GetTestLimitValueByIndex(600);
	// 仪器相位测试判据
	float fLimitPhase = m_oTestSetupData.m_oTestLimitListInstrument.GetTestLimitValueByIndex(610);
	for(unsigned int i = 0; i < m_oTestSetupData.m_oTestElementListBase.m_uiCountUsed; i++)
	{
		// 单元在单元数组中的位置
		m_oTestSetupData.m_oTestElementListBase.m_pArrayTestElementData[i].m_fInstrumentGainLimit = fLimitGain;
		m_oTestSetupData.m_oTestElementListBase.m_pArrayTestElementData[i].m_fInstrumentPhaseLimit = fLimitPhase;
	}
}

/**
* 设置测试单元队列判据，仪器测试，仪器共模
* @param void
* @return void
*/
void CLogicData::SetTestElementListLimitInstrumentCMRR()
{
	float fLimit = m_oTestSetupData.m_oTestLimitListInstrument.GetTestLimitValueByIndex(800);
	for(unsigned int i = 0; i < m_oTestSetupData.m_oTestElementListBase.m_uiCountUsed; i++)
	{
		// 单元在单元数组中的位置
		m_oTestSetupData.m_oTestElementListBase.m_pArrayTestElementData[i].m_fInstrumentCMRRLimit = fLimit;
	}
}

// 设置测试单元队列判据，检波器测试，野外电阻
void CLogicData::SetTestElementListLimitISensorResistance()
{
	bool bValue = false;
	float fLimit = 0.0;
	for(unsigned int i = 0; i < m_oTestSetupData.m_oTestElementListBase.m_uiCountUsed; i++)
	{
		bValue = m_oSurveySetupData.GetTestLimitValue(1500, m_oTestSetupData.m_oTestElementListBase.m_pArrayTestElementData[i].m_uiIP, fLimit);
		if(true == bValue)
		{
			m_oTestSetupData.m_oTestElementListBase.m_pArrayTestElementData[i].m_fSensorResistanceLimitMin = fLimit;
		}
		else
		{
			fLimit = m_oTestSetupData.m_oTestLimitListSensor.GetTestLimitValueByIndex(1500);
			m_oTestSetupData.m_oTestElementListBase.m_pArrayTestElementData[i].m_fSensorResistanceLimitMin = fLimit;
		}

		bValue = m_oSurveySetupData.GetTestLimitValue(1501, m_oTestSetupData.m_oTestElementListBase.m_pArrayTestElementData[i].m_uiIP, fLimit);
		if(true == bValue)
		{
			m_oTestSetupData.m_oTestElementListBase.m_pArrayTestElementData[i].m_fSensorResistanceLimitMax = fLimit;
		}
		else
		{
			fLimit = m_oTestSetupData.m_oTestLimitListSensor.GetTestLimitValueByIndex(1501);
			m_oTestSetupData.m_oTestElementListBase.m_pArrayTestElementData[i].m_fSensorResistanceLimitMax = fLimit;
		}
	}
}

// 设置测试单元队列判据，检波器测试，野外漏电
void CLogicData::SetTestElementListLimitISensorLeakage()
{
	bool bValue = false;
	float fLimit = 0.0;
	for(unsigned int i = 0; i < m_oTestSetupData.m_oTestElementListBase.m_uiCountUsed; i++)
	{
		bValue = m_oSurveySetupData.GetTestLimitValue(1400, m_oTestSetupData.m_oTestElementListBase.m_pArrayTestElementData[i].m_uiIP, fLimit);
		if(true == bValue)
		{
			m_oTestSetupData.m_oTestElementListBase.m_pArrayTestElementData[i].m_fSensorLeakageLimit = fLimit;
		}
		else
		{
			fLimit = m_oTestSetupData.m_oTestLimitListSensor.GetTestLimitValueByIndex(1400);
			m_oTestSetupData.m_oTestElementListBase.m_pArrayTestElementData[i].m_fSensorLeakageLimit = fLimit;
		}
	}
}

// 设置测试单元队列判据，检波器测试，野外噪声
void CLogicData::SetTestElementListLimitISensorNoise()
{
	bool bValue = false;
	float fLimit = 0.0;
	for(unsigned int i = 0; i < m_oTestSetupData.m_oTestElementListBase.m_uiCountUsed; i++)
	{
		bValue = m_oSurveySetupData.GetTestLimitValue(100, m_oTestSetupData.m_oTestElementListBase.m_pArrayTestElementData[i].m_uiIP, fLimit);
		if(true == bValue)
		{
			m_oTestSetupData.m_oTestElementListBase.m_pArrayTestElementData[i].m_fSensorNoiseLimit = fLimit;
		}
		else
		{
			fLimit = m_oTestSetupData.m_oTestLimitListSensor.GetTestLimitValueByIndex(100);
			m_oTestSetupData.m_oTestElementListBase.m_pArrayTestElementData[i].m_fSensorNoiseLimit = fLimit;
		}
	}
}

// 设置测试单元队列判据，检波器测试，野外倾斜度
void CLogicData::SetTestElementListLimitISensorTilt()
{
	bool bValue = false;
	float fLimit = 0.0;
	for(unsigned int i = 0; i < m_oTestSetupData.m_oTestElementListBase.m_uiCountUsed; i++)
	{
		bValue = m_oSurveySetupData.GetTestLimitValue(2000, m_oTestSetupData.m_oTestElementListBase.m_pArrayTestElementData[i].m_uiIP, fLimit);
		if(true == bValue)
		{
			m_oTestSetupData.m_oTestElementListBase.m_pArrayTestElementData[i].m_fSensorTiltLimit = fLimit;
		}
		else
		{
			fLimit = m_oTestSetupData.m_oTestLimitListSensor.GetTestLimitValueByIndex(2000);
			m_oTestSetupData.m_oTestElementListBase.m_pArrayTestElementData[i].m_fSensorTiltLimit = fLimit;
		}
	}
}

// 设置测试单元队列判据，检波器测试，野外倾斜度模式
void CLogicData::SetTestElementListLimitISensorTiltModel()
{
	float fLimit = 0.0;
	fLimit = m_oTestSetupData.m_oTestLimitListSensor.GetTestLimitValueByIndex(2100);
	for(unsigned int i = 0; i < m_oTestSetupData.m_oTestElementListBase.m_uiCountUsed; i++)
	{
		m_oTestSetupData.m_oTestElementListBase.m_pArrayTestElementData[i].m_fSensorTiltModelLimit = fLimit;
	}
}

/**
* 设置测试结果，基本测试
* @param int iTestType
* @param CTestElementList* pTestElementList
* @param bool bProcSensorTestOnField
* @return void
*/
void CLogicData::SetTestResultBase(int iTestType, CTestElementList* pTestElementList, bool bProcSensorTestOnField)
{
	CTestElementData* pTestElementData = NULL;
	CChannelSetupData* pChannel = NULL;
	for(unsigned int i = 0; i < pTestElementList->m_uiCountUsed; i++)
	{
		pTestElementData = pTestElementList->GetTestElementDataByIndex(i);
		pChannel = m_oSurveySetupData.GetChannel(pTestElementData->m_uiIP);
		if(NULL != pChannel)
		{
			switch(iTestType)
			{
				// 检波器测试
				// 野外电阻
			case 15:
				pChannel->m_fResistanceValue = pTestElementData->m_fSensorResistanceValue;
				pChannel->m_byResistanceCheck = pTestElementData->m_bySensorResistanceCheck;
				pChannel->m_fResistanceLimitMax = pTestElementData->m_fSensorResistanceLimitMax;
				pChannel->m_fResistanceLimitMin = pTestElementData->m_fSensorResistanceLimitMin;
				break;
				// 野外漏电
			case 14:
				pChannel->m_fLeakageValue = pTestElementData->m_fSensorLeakageValue;
				pChannel->m_byLeakageCheck = pTestElementData->m_bySensorLeakageCheck;
				pChannel->m_fLeakageLimit = pTestElementData->m_fSensorLeakageLimit;
				break;
				// 野外噪声
			case 1:
				pChannel->m_fNoiseValue = pTestElementData->m_fSensorNoiseValue;
				pChannel->m_byNoiseCheck = pTestElementData->m_bySensorNoiseCheck;
				pChannel->m_fNoiseLimit = pTestElementData->m_fSensorNoiseLimit;
				break;
				// 野外倾斜度
			case 20:
				pChannel->m_fTiltValue = pTestElementData->m_fSensorTiltValue;
				pChannel->m_byTiltCheck = pTestElementData->m_bySensorTiltCheck;
				pChannel->m_fTiltLimit = pTestElementData->m_fSensorTiltLimit;
				break;
				// 野外倾斜度模式
			case 21:
				pChannel->m_fTiltModelValue = pTestElementData->m_fSensorTiltModelValue;
				pChannel->m_byTiltModelCheck = pTestElementData->m_bySensorTiltModelCheck;
				pChannel->m_fTiltModelLimit = pTestElementData->m_fSensorTiltModelLimit;
				break;
			}
			pChannel->CheckResultAll();
			pTestElementData->m_bySensorCheck = pChannel->m_byCheck;

			// 判断：处理新加检波器的初始测试
			if(true == bProcSensorTestOnField)
			{
				pChannel->CheckResultOnField(m_oLookSetupData.m_uiTestResistance, m_oLookSetupData.m_uiTestTilt, m_oLookSetupData.m_uiTestLeakage);
				pChannel->m_byTestOnField = 1;

				// 检波器是否进行了初始测试
				pTestElementData->m_byTestOnField = pChannel->m_byTestOnField;
				// 初始测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读
				pTestElementData->m_byOnFieldCheck = pChannel->m_byOnFieldCheck;
			}
		}
	}
}