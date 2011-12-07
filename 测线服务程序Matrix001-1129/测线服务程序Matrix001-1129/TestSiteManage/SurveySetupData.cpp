#include "StdAfx.h"
#include "SurveySetupData.h"

CSurveySetupData::CSurveySetupData()
{
}

CSurveySetupData::~CSurveySetupData()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CSurveySetupData::OnInit()
{
	// 测线数量
	m_uiLineCount = 0;
	// 测线头指针
	m_pLineHead = NULL;
	// 测线尾指针
	m_pLineTail = NULL;

	// 测点数量
	m_uiPointCount = 0;
	// 测点头指针
	m_pPointHead = NULL;
	// 测点尾指针
	m_pPointTail = NULL;

	// 测道数量
	m_uiChannelCount = 0;
	// 测道头指针
	m_pChannelHead = NULL;
	// 测道尾指针
	m_pChannelTail = NULL;

	// 测线数组总数
	m_oLineList.m_uiCountAll = m_uiLineCountAll;
	// 测线队列
	m_oLineList.OnInit();

	// 测点数组总数
	m_oPointList.m_uiCountAll = m_uiPointCountAll;
	// 测点队列
	m_oPointList.OnInit();

	// 测道数组总数
	m_oChannelList.m_uiCountAll = m_uiChannelCountAll;
	// 测道队列
	m_oChannelList.OnInit();

	// 检波器列表对象
	m_oSensorList.m_pXMLDOMDocument = m_pXMLDOMDocument;	// XMLDOM文件对象
	m_oSensorList.OnInit();

	// 点代码列表对象
	m_oPointCodeList.m_pXMLDOMDocument = m_pXMLDOMDocument;	// XMLDOM文件对象
	m_oPointCodeList.m_pSensorList = &m_oSensorList;	// 检波器列表对象
	m_oPointCodeList.OnInit();

	// 解析配置文件
	ParseXML();
	// 链接
	Link();
}

/**
* 重置
* @param void
* @return void
*/
void CSurveySetupData::OnReset()
{
	// 测道队列
	m_oChannelList.OnReset();
}

/**
* 重新加载
* @param void
* @return void
*/
void CSurveySetupData::OnReload()
{
	// 测线数量
	m_uiLineCount = 0;
	// 测线头指针
	m_pLineHead = NULL;
	// 测线尾指针
	m_pLineTail = NULL;

	// 测点数量
	m_uiPointCount = 0;
	// 测点头指针
	m_pPointHead = NULL;
	// 测点尾指针
	m_pPointTail = NULL;

	// 测道数量
	m_uiChannelCount = 0;
	// 测道头指针
	m_pChannelHead = NULL;
	// 测道尾指针
	m_pChannelTail = NULL;

	// 测线队列
	m_oLineList.OnReload();

	// 测点队列
	m_oPointList.OnReload();

	// 测道队列
	m_oChannelList.OnReload();

	// 检波器列表对象
	m_oSensorList.OnReload();

	// 点代码列表对象
	m_oPointCodeList.OnReload();

	// 解析配置文件
	ParseXML();
	// 链接
	Link();
}

/**
* 关闭
* @param void
* @return void
*/
void CSurveySetupData::OnClose()
{
	// 测线队列
	m_oLineList.OnClose();

	// 测点队列
	m_oPointList.OnClose();

	// 测道队列
	m_oChannelList.OnClose();

	// 检波器列表对象
	m_oSensorList.OnClose();

	// 点代码列表对象
	m_oPointCodeList.OnClose();
}

/**
* 解析配置文件
* @param void
* @return void
*/
void CSurveySetupData::ParseXML()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	// 找到检波器设置区
	strKey = "SurverySetup";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到测线总数
	strKey = "Count";
	m_uiLineCount = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CLineSetupData* pLineSetupData = NULL;
	for(unsigned int i = 0; i < m_uiLineCount; i++)
	{
		// 得到XML元素
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);

		// 得到一个空闲测线
		pLineSetupData = m_oLineList.GetFreeLine();
		pLineSetupData->ParseXML(&oElement);
		// 加入测线索引表
		m_oLineList.AddLineToMap(pLineSetupData->m_uiNbLine, pLineSetupData);
		// 解析测线
		ParseLine(pLineSetupData);
	}
}

/**
* 解析测线
* @param CLineSetupData* pLineSetupData 测线设置数据指针
* @return void
*/
void CSurveySetupData::ParseLine(CLineSetupData* pLineSetupData)
{
	// 解析测线的测点定义
	ParseLineReceiverSection(pLineSetupData);
}

/**
* 解析测点
* @param CPointSetupData* pPointSetupData 测点设置数据指针
* @return void
*/
void CSurveySetupData::ParsePoint(CPointSetupData* pPointSetupData)
{
	// 得到测点对应的点代码
	CPointCodeData* pPointCodeData = m_oPointCodeList.GetPointCode(pPointSetupData->m_iPointCode);
	if(NULL != pPointCodeData)
	{
		// 测道
		CChannelSetupData* pChannelSetupData = NULL;
		// 检波器
		CSensorData* pSensorData = NULL;
		for(int i = 0; i < pPointCodeData->m_iSensorCount; i++)
		{
			// 得到一个空闲测道
			pChannelSetupData = m_oChannelList.GetFreeChannel();
			// 测线号
			pChannelSetupData->m_uiNbLine = pPointSetupData->m_uiNbLine;
			// 测点号
			pChannelSetupData->m_uiNbPoint = pPointSetupData->m_uiNbPoint;	
			// 测点索引号
			pChannelSetupData->m_uiIndexPoint = pPointSetupData->m_uiIndexPoint;
			// 测道号
			pChannelSetupData->m_uiNbChannel = i + 1;
			// 根据索引号，得到检波器
			pSensorData = pPointCodeData->GetSensorData(i);
			// 检波器号
			pChannelSetupData->m_uiSensorNb = pSensorData->m_uiNb;
			// SegdCode
			pChannelSetupData->m_uiSegdCode = pSensorData->m_uiSegdCode;
			// 根据测线号、测点号、测道号，生成IP地址
			pChannelSetupData->CreateIP();
			// 加入测道索引表
			m_oChannelList.AddChannelToMap(pChannelSetupData->m_uiIP, pChannelSetupData);
			// 解析测道
			ParseChannel(pChannelSetupData);				
		}
	}
}

/**
* 解析测道
* @param CChannelSetupData* pChannelSetupData 测道设置数据指针
* @return void
*/
void CSurveySetupData::ParseChannel(CChannelSetupData* pChannelSetupData)
{
	// 检波器号为0
	if(0 == pChannelSetupData->m_uiSensorNb)
	{
		// 是否跳过道
		pChannelSetupData->m_bJumped = true;
	}
}

/**
* 解析测线的测点定义
* @param CLineSetupData* pLineSetupData 测线设置数据指针
* @return void
*/
void CSurveySetupData::ParseLineReceiverSection(CLineSetupData* pLineSetupData)
{
	CString strReceiverSection;
	strReceiverSection = pLineSetupData->m_strReceiverSection;
	// 转换为小写
	strReceiverSection.MakeLower();

	CString strPart;	// 线段字符串
	POSITION pos;	// 位置
	int iPos, iPosStart, iPosStartEnd;	// 位置
	int iCount, i;
	CList<int, int> lsPos;	// 位置队列

	lsPos.AddTail(-1);
	iPosStart = 0;
	while(true)
	{
		// 查找分割标记","
		iPos = strReceiverSection.Find(',', iPosStart);
		if(-1 == iPos)
		{
			break;
		}
		lsPos.AddTail(iPos);
		iPosStart = iPos + 1;
	}
	lsPos.AddTail(strReceiverSection.GetAllocLength());

	// 得到标记点 "," 数量
	iCount = lsPos.GetCount();
	for(i = 0; i < iCount - 1; i++)
	{
		pos = lsPos.FindIndex(i);
		iPosStart = lsPos.GetAt(pos) + 1;
		pos = lsPos.FindIndex(i + 1);
		iPosStartEnd = lsPos.GetAt(pos);
		// 得到线段字符串
		strPart = strReceiverSection.Mid(iPosStart, iPosStartEnd - iPosStart);
		// 解析测线段
		ParseLineReceiverSectionPart(pLineSetupData, strPart);
	}
}

/**
* 解析测线段
* @param CLineSetupData* pLineSetupData 测线设置数据指针
* @param CString strPart 测线段定义字符串
* @return void
*/
void CSurveySetupData::ParseLineReceiverSectionPart(CLineSetupData* pLineSetupData, CString strPart)
{
	int iPointCode;	// 点代码
	int iPos;	// 位置
	int iStep = 1;	// 步长
	int iStart = 1;	// 起始测点
	int iEnd = 1;	// 终止测点
	CString strData;

	// 得到步长
	iPos = strPart.Find('/', 0);
	// 无步长定义
	if(-1 == iPos)
	{
		iStep = 1;	// 步长
	}
	else	// 有步长定义
	{
		iPos++;
		strData = strPart.Mid(iPos, strPart.GetLength() - iPos);
		// 步长
		iStep = CXMLDOMTool::ConvertStringToInt(strData);
		strPart = strPart.Mid(0, iPos - 1);
	}

	// 得到点代码
	iPos = strPart.Find('p', 0);
	iPos++;
	strData = strPart.Mid(iPos, strPart.GetLength() - iPos);
	// 点代码
	iPointCode = CXMLDOMTool::ConvertStringToInt(strData);
	strPart = strPart.Mid(0, iPos - 1);

	// 得到起始点
	iPos = strPart.Find('-', 0);
	// 无分隔符
	if(-1 == iPos)
	{
		// 起始测点
		iStart = CXMLDOMTool::ConvertStringToInt(strPart);
		// 终止测点
		iEnd = iStart;
	}
	else	// 有分隔符
	{
		strData = strPart.Mid(0, iPos);
		// 起始测点
		iStart = CXMLDOMTool::ConvertStringToInt(strData);

		iPos++;
		strData = strPart.Mid(iPos, strPart.GetLength() - iPos);
		// 终止测点
		iEnd = CXMLDOMTool::ConvertStringToInt(strData);
	}

	CPointSetupData* pPointSetupData = NULL;
	for(int i = iStart; i <= iEnd; i = i + iStep)
	{
		// 得到一个空闲测点
		pPointSetupData = m_oPointList.GetFreePoint();
		// 测线号
		pPointSetupData->m_uiNbLine = pLineSetupData->m_uiNbLine;
		// 测点号
		pPointSetupData->m_uiNbPoint = i;
		// 点代码
		pPointSetupData->m_iPointCode = iPointCode;
		// 测点索引号
		pPointSetupData->CreateIndexPoint();
		// 加入测点索引表
		m_oPointList.AddPointToMap(pPointSetupData->m_uiIndexPoint, pPointSetupData);
		// 解析测点
		ParsePoint(pPointSetupData);
	}
}

/**
* 链接地形设置数据
* @param void
* @return void
*/
void CSurveySetupData::Link()
{
	// 链接测线
	LinkLine();
	// 链接测点
	LinkPoint();
	// 链接测道
	LinkChannel();
}

/**
* 链接测线
* @param void
* @return void
*/
void CSurveySetupData::LinkLine()
{
	// 测线数量
	m_uiLineCount = m_oLineList.m_uiCountUsed;
	// 测线头指针
	m_pLineHead = m_oLineList.GetLineByIndex(0);
	// 测线尾指针
	m_pLineTail = m_oLineList.GetLineByIndex(m_uiLineCount - 1);

	// 测线指针
	CLineSetupData* pLineSetupData;
	// 下一个测线指针
	CLineSetupData* pLineSetupDataNext;
	for(unsigned int i = 0; i < m_uiLineCount - 1; i++)
	{
		pLineSetupData = m_oLineList.GetLineByIndex(i);
		pLineSetupDataNext = m_oLineList.GetLineByIndex(i + 1);
		// 链接上下
		pLineSetupData->m_pLinelDown = pLineSetupDataNext;
		pLineSetupDataNext->m_pLinelTop = pLineSetupData;
	}
}

/**
* 链接测点
* @param void
* @return void
*/
void CSurveySetupData::LinkPoint()
{
	// 测点数量
	m_uiPointCount = m_oPointList.m_uiCountUsed;
	// 测点头指针
	m_pPointHead = m_oPointList.GetPointByIndex(0);
	// 测点尾指针
	m_pPointTail = m_oPointList.GetPointByIndex(m_uiPointCount - 1);

	// 测点指针
	CPointSetupData* pPointSetupData;
	// 下一个测点指针
	CPointSetupData* pPointSetupDataNext;

	// 测线指针
	CLineSetupData* pLineSetupData = m_pLineHead;
	// 测线头指针
	pLineSetupData->m_pPointHead = m_pPointHead;

	for(unsigned int i = 0; i < m_uiPointCount - 1; i++)
	{
		pPointSetupData = m_oPointList.GetPointByIndex(i);
		pPointSetupDataNext = m_oPointList.GetPointByIndex(i + 1);
		// 测点和测线在同一条测线
		if(pPointSetupData->m_uiNbLine == pLineSetupData->m_uiNbLine)
		{
			// 测点数量
			pLineSetupData->m_uiPointCount++;
		}
		// 前后测点在同一条测线
		if(pPointSetupData->m_uiNbLine == pPointSetupDataNext->m_uiNbLine)
		{
			// 链接左右
			pPointSetupData->m_pPointRight = pPointSetupDataNext;
			pPointSetupDataNext->m_pPointLeft = pPointSetupData;
		}
		else	// 不在同一条测线上
		{
			// 测线的测点尾指针
			pLineSetupData->m_pPointTail = pPointSetupData;
			// 链接测点上下
			pLineSetupData->m_pPointHead->m_pPointlDown = pPointSetupDataNext;
			pPointSetupDataNext->m_pPointlTop = pLineSetupData->m_pPointHead;
			// 下一条测线
			pLineSetupData = pLineSetupData->m_pLinelDown;
			pLineSetupData->m_pPointHead = pPointSetupDataNext;
		}
	}
	// 测点数量
	pLineSetupData->m_uiPointCount++;
	// 测线的测点尾指针
	pLineSetupData->m_pPointTail = pPointSetupDataNext;
}

/**
* 链接测道
* @param void
* @return void
*/
void CSurveySetupData::LinkChannel()
{
	// 测道数量
	m_uiChannelCount = m_oChannelList.m_uiCountUsed;
	// 测道头指针
	m_pChannelHead = m_oChannelList.GetChannelByIndex(0);
	// 测道尾指针
	m_pChannelTail = m_oChannelList.GetChannelByIndex(m_uiChannelCount - 1);

	// 测道指针
	CChannelSetupData* pChannelSetupData;
	// 下一个测道指针
	CChannelSetupData* pChannelSetupDataNext;

	// 测线指针
	CLineSetupData* pLineSetupData = m_pLineHead;
	// 测线头指针
	pLineSetupData->m_pChannelHead = m_pChannelHead;

	// 测点指针
	CPointSetupData* pPointSetupData = m_pPointHead;
	// 测点头指针
	pPointSetupData->m_pChannelHead = m_pChannelHead;

	for(unsigned int i = 0; i < m_uiChannelCount - 1; i++)
	{
		pChannelSetupData = m_oChannelList.GetChannelByIndex(i);
		pChannelSetupDataNext = m_oChannelList.GetChannelByIndex(i + 1);
		// 同一条测线
		if(pChannelSetupData->m_uiNbLine == pLineSetupData->m_uiNbLine)
		{
			// 测道数量
			pLineSetupData->m_uiChannelCount++;
		}
		// 同一条测点
		if(pChannelSetupData->m_uiNbPoint == pPointSetupData->m_uiNbPoint)
		{
			// 测道数量
			pPointSetupData->m_uiChannelCount++;
		}
		// 前后测道在同一条测线
		if(pChannelSetupData->m_uiNbLine == pChannelSetupDataNext->m_uiNbLine)
		{
			// 链接左右
			pChannelSetupData->m_pChannelRight = pChannelSetupDataNext;
			pChannelSetupDataNext->m_pChannelLeft = pChannelSetupData;
		}
		else	// 不在同一条测线上
		{
			// 测线的测道尾指针
			pLineSetupData->m_pChannelTail = pChannelSetupData;
			// 链接测道上下
			pLineSetupData->m_pChannelHead->m_pChannelDown = pChannelSetupDataNext;
			pChannelSetupDataNext->m_pChannelTop = pLineSetupData->m_pChannelHead;
			// 下一条测线
			pLineSetupData = pLineSetupData->m_pLinelDown;
			pLineSetupData->m_pChannelHead = pChannelSetupDataNext;
		}
		// 前后测道不在同一条测点
		if(pChannelSetupData->m_uiNbPoint != pChannelSetupDataNext->m_uiNbPoint)
		{
			// 测点的测道尾指针
			pPointSetupData->m_pChannelTail = pChannelSetupData;
			// 下一条测点
			pPointSetupData = m_oPointList.GetPointByIndex(pPointSetupData->m_uiIndex + 1);
			pPointSetupData->m_pChannelHead = pChannelSetupDataNext;
		}
	}
	// 测道数量
	pLineSetupData->m_uiChannelCount++;
	// 测线的测道尾指针
	pLineSetupData->m_pChannelTail = pChannelSetupDataNext;

	// 测道数量
	pPointSetupData->m_uiChannelCount++;
	// 测点的测道尾指针
	pPointSetupData->m_pChannelTail = pChannelSetupDataNext;
}

/**
* 根据仪器IP地址，在测道队列中得到测道
* @param unsigned int uiIP 仪器IP地址
* @return CChannelSetupData* 测道指针 NULL：无对应测道
*/
CChannelSetupData* CSurveySetupData::GetChannel(unsigned int uiIP)
{
	CChannelSetupData* pChannel = NULL;
	if(TRUE == m_oChannelList.GetChannelFromMap(uiIP, pChannel))
	{
		return pChannel;
	}
	return NULL;
}

/**
* 根据测线、测点、测道，在测道队列中得到测道
* @param unsigned int uiNbLine 测线号
* @param unsigned int uiNbPoint 测点号
* @param unsigned int uiNbChannel 测道号
* @return CChannelSetupData* 测道指针 NULL：无对应测道
*/
CChannelSetupData* CSurveySetupData::GetChannel(unsigned int uiNbLine, unsigned int uiNbPoint, unsigned int uiNbChannel)
{
	return m_oChannelList.GetChannel(uiNbLine, uiNbPoint, uiNbChannel);
}

/**
* 根据测点索引号，得到测点
* @param unsigned int uiIndexPoint 测点索引号
* @return CPointSetupData* 测点指针 NULL：无对应测点
*/
// 得到测点
CPointSetupData* CSurveySetupData::GetPoint(unsigned int uiIndexPoint)
{
	CPointSetupData* pPoint = NULL;
	if(TRUE == m_oPointList.GetPointFromMap(uiIndexPoint, pPoint))
	{
		return pPoint;
	}
	return NULL;
}

/**
* 根据测线、测点，在测点队列中得到测点
* @param unsigned int uiNbLine 测线号
* @param unsigned int uiNbPoint 测点号
* @return CPointSetupData* 测点指针 NULL：无对应测点
*/
CPointSetupData* CSurveySetupData::GetPoint(unsigned int uiNbLine, unsigned int uiNbPoint)
{
	return m_oPointList.GetPoint(uiNbLine, uiNbPoint);
}

/**
* 根据测线号，得到测线
* @param unsigned int uiNbLine 测线号
* @return CLineSetupData* 测线指针 NULL：无对应测线
*/
CLineSetupData* CSurveySetupData::GetLine(unsigned int uiNbLine)
{
	return m_oLineList.GetLine(uiNbLine);
}

/**
* 根据增量和方向，得到测点的相邻点
* @param CPointSetupData* pPointSetupData 测点指针
* @param unsigned int uiIncrement 增量
* @param unsigned int uiDirection 方向 3：左；4：右
* @return CPointSetupData* 测点指针 NULL：无对应测点
*/
CPointSetupData* CSurveySetupData::GetPointByIncrementAndDirection(CPointSetupData* pPointSetupData, unsigned int uiIncrement, unsigned int uiDirection)
{
	if(NULL == pPointSetupData)
	{
		return NULL;
	}

	CPointSetupData* pPoint = pPointSetupData;
	unsigned int iCount = 0;
	// 判断方向
	switch(uiDirection)
	{
	case 3:	// 左
		{
			while(true)
			{
				pPoint = pPoint->m_pPointLeft;
				if(NULL == pPoint)
				{
					return NULL;
				}
				iCount++;
				if(iCount == uiIncrement)
				{
					return pPoint;
				}
			}
			break;
		}
	case 4:	// 右
		{
			while(true)
			{
				pPoint = pPoint->m_pPointRight;
				if(NULL == pPoint)
				{
					return NULL;
				}
				iCount++;
				if(iCount == uiIncrement)
				{
					return pPoint;
				}
			}
			break;
		}
	}
	return NULL;
}

/**
* 解析采集道绝对排列，得到IP地址队列
* @param CList<unsigned int, unsigned int>* plsIP IP地址队列指针
* @param CString strAbsoluteSpread 绝对排列
* @return bool true：成功；false：失败
*/
bool CSurveySetupData::ParseSpreadToIPListByAbsoluteSpread(CList<unsigned int, unsigned int>* plsIP, CString strAbsoluteSpread)
{
	// 解析绝对排列定义
	// 判断：没有定义测道
	if(strAbsoluteSpread == "")
	{
		return true;
	}
	// 判断：所有测道加入队列
	if(strAbsoluteSpread == "all")
	{
		for(unsigned int  i = 0; i < m_oChannelList.m_uiCountUsed; i++)
		{
			// 判断：测道不是跳过道和哑道
			if((false == m_oChannelList.m_pArrayChannel[i].m_bJumped) && ((false == m_oChannelList.m_pArrayChannel[i].m_bMute)))
			{
				// 判断：只添加已经连接到现场的测道IP地址
				if(0 < m_oChannelList.m_pArrayChannel[i].m_uiSN)
				{
					if(NULL == plsIP->Find(m_oChannelList.m_pArrayChannel[i].m_uiIP, 0))
					{
						plsIP->AddTail(m_oChannelList.m_pArrayChannel[i].m_uiIP);
					}
				}
			}
		}
	}
	else
	{
		CString strPart;	// 线段字符串                                        
		POSITION pos;	// 位置                                                  
		int iPos, iPosStart, iPosStartEnd;	// 位置                              
		int iCount, i;                                                           
		CList<int, int> lsPos;	// 位置队列                                      

		lsPos.AddTail(-1);                                                       
		iPosStart = 0;                                                           
		while(true)                                                              
		{                                                                        
			// 查找分割标记";"                                                   
			iPos = strAbsoluteSpread.Find(';', iPosStart);                       
			if(-1 == iPos)                                                       
			{                                                                    
				break;                                                           
			}                                                                    
			lsPos.AddTail(iPos);                                                 
			iPosStart = iPos + 1;                                                
		}                                                                        
		lsPos.AddTail(strAbsoluteSpread.GetAllocLength());                       

		// 得到标记点 ";" 数量                                                   
		iCount = lsPos.GetCount();                                               
		for(i = 0; i < iCount - 1; i++)                                          
		{                                                                        
			pos = lsPos.FindIndex(i);                                            
			iPosStart = lsPos.GetAt(pos) + 1;                                    
			pos = lsPos.FindIndex(i + 1);                                        
			iPosStartEnd = lsPos.GetAt(pos);                                     
			// 得到线段字符串                                                    
			strPart = strAbsoluteSpread.Mid(iPosStart, iPosStartEnd - iPosStart);
			ParseSpreadToIPListByAbsoluteSpreadPart(plsIP, strPart);             
		}                                                                        
	}
	return true;
}

/**
* 解析采集道排列定义，得到IP地址队列
* @param CList<unsigned int, unsigned int>* plsIP IP地址队列指针
* @param CString strPart 排列定义
* @return bool true：成功；false：失败
*/
bool CSurveySetupData::ParseSpreadToIPListByAbsoluteSpreadPart(CList<unsigned int, unsigned int>* plsIP, CString strPart)
{
	unsigned int uiNbLine;	// 测线号
	unsigned int uiNbPointStart;	// 起始测点号
	unsigned int uiNbPointEnd;	// 终止测点号
	CLineSetupData* pLineSetupData = NULL;	// 测线
	CPointSetupData* pPointSetupDataStart = NULL;	// 起始测点
	CPointSetupData* pPointSetupDataEnd = NULL;	// 终止测点

	int iPos = 0;	// 位置
	CString strData;

	// 得到测线号
	iPos = strPart.Find(':', 0);
	strData = strPart.Mid(0, iPos);	// 测线号
	uiNbLine = CXMLDOMTool::ConvertStringToInt(strData);
	pLineSetupData = GetLine(uiNbLine);
	if(NULL == pLineSetupData)
	{
		return true;
	}

	iPos = iPos++;
	strPart = strPart.Mid(iPos, strPart.GetLength() - iPos);
	// 得到起始点
	iPos = strPart.Find('-', 0);
	// 无分隔符
	if(-1 == iPos)
	{
		// 起始测点
		uiNbPointStart = CXMLDOMTool::ConvertStringToInt(strPart);
		// 终止测点
		uiNbPointEnd = uiNbPointStart;
	}
	else	// 有分隔符
	{
		strData = strPart.Mid(0, iPos);
		// 起始测点
		uiNbPointStart = CXMLDOMTool::ConvertStringToInt(strData);

		iPos++;
		strData = strPart.Mid(iPos, strPart.GetLength() - iPos);
		// 终止测点
		uiNbPointEnd = CXMLDOMTool::ConvertStringToInt(strData);
	}
	// 得到测线
	CLineSetupData* GetLine(unsigned int uiNbLine);

	// 得到起始测点，根据测线号、测点号
	pPointSetupDataStart = GetPoint(uiNbLine, uiNbPointStart);
	if(NULL == pPointSetupDataStart)
	{
		pPointSetupDataStart = pLineSetupData->m_pPointHead;
	}
	// 得到终止测点，根据测线号、测点号
	pPointSetupDataEnd = GetPoint(uiNbLine, uiNbPointEnd);
	if(NULL == pPointSetupDataEnd)
	{
		pPointSetupDataEnd = pLineSetupData->m_pPointTail;
	}
	
	int iChannelIndexStart = pPointSetupDataStart->m_pChannelHead->m_uiIndex;
	int iChannelIndexEnd = pPointSetupDataEnd->m_pChannelTail->m_uiIndex;

	for(int i = iChannelIndexStart; i <= iChannelIndexEnd; i++)
	{
		// 判断：测道不是跳过道和哑道
		if((false == m_oChannelList.m_pArrayChannel[i].m_bJumped) && ((false == m_oChannelList.m_pArrayChannel[i].m_bMute)))
		{
			// 判断：只添加已经连接到现场的测道IP地址
			if(0 < m_oChannelList.m_pArrayChannel[i].m_uiSN)
			{
				if(NULL == plsIP->Find(m_oChannelList.m_pArrayChannel[i].m_uiIP, 0))
				{
					plsIP->AddTail(m_oChannelList.m_pArrayChannel[i].m_uiIP);
				}
			}
		}		
	}
	return true;
}

/**
* 得到IP地址队列，参加新加检波器的初始测试
* @param CList<unsigned int, unsigned int>* plsIP IP地址队列指针
* @return bool true：成功；false：失败
*/
bool CSurveySetupData::GetIPListForSensorTestOnField(CList<unsigned int, unsigned int>* plsIP)
{
	for(unsigned int  i = 0; i < m_oChannelList.m_uiCountUsed; i++)
	{
		// 判断：测道不是跳过道和哑道
		if((false == m_oChannelList.m_pArrayChannel[i].m_bJumped) && ((false == m_oChannelList.m_pArrayChannel[i].m_bMute)))
		{
			// 判断：只添加已经连接到现场的测道IP地址，且未进行检波器的初始测试
			if((0 < m_oChannelList.m_pArrayChannel[i].m_uiSN) && ((0 == m_oChannelList.m_pArrayChannel[i].m_byTestOnField)))
			{
				if(NULL == plsIP->Find(m_oChannelList.m_pArrayChannel[i].m_uiIP, 0))
				{
					plsIP->AddTail(m_oChannelList.m_pArrayChannel[i].m_uiIP);
				}
			}
		}
	}
	return true;
}

/**
* 根据判据号和IP地址，得到测试判据
* @param unsigned int uiTestLimitIndex 判据号
* @param unsigned int uiIP IP地址
* @return float 测试判据；引用方式返回
* @return bool true：成功；false：失败
*/
bool CSurveySetupData::GetTestLimitValue(unsigned int uiTestLimitIndex, unsigned int uiIP, float& fLimit)
{
	bool bReturn = false;
	fLimit = 0.0;
	CChannelSetupData* pSurveySetupData = NULL;
	pSurveySetupData = GetChannel(uiIP);
	if(NULL != pSurveySetupData)
	{
		CSensorData* pSensorData = NULL;
		pSensorData = m_oSensorList.GetSensor(pSurveySetupData->m_uiSensorNb);
		if(NULL != pSensorData)
		{
			switch(uiTestLimitIndex)
			{
				// 电阻最小值
			case 1500:
				fLimit = pSensorData->m_fContinuityMin;
				bReturn = true;
				break;
				// 电阻最大值
			case 1501:
				fLimit = pSensorData->m_fContinuityMax;
				bReturn = true;
				break;
				// 漏电
			case 1400:
				fLimit = pSensorData->m_fLeaKage;
				bReturn = true;
				break;
				// 噪声
			case 100:
				fLimit = pSensorData->m_fNoise;
				bReturn = true;
				break;
				// 倾斜度
			case 2000:
				fLimit = pSensorData->m_fTilt;
				bReturn = true;
				break;
			}
		}
	}
	return bReturn;	
}