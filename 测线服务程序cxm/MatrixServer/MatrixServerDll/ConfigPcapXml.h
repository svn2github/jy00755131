#ifndef _ConfigPcapXml_H
#define _ConfigPcapXml_H
/**
* @struct PcapIPSetupData_Struct
* @brief 从XML文件中解析得到IP地址设置数据
*/
typedef struct PcapIPSetupData_Struct
{
	/** 下行设备IP地址*/
	string m_strLowIP;
	/** 上行设备IP地址*/
	string m_strHighIP;
}m_oPcapIPSetupDataStruct;
/**
* @struct PcapPortSetupData_Struct
* @brief 从XML文件中解析得到端口设置数据
*/ 
typedef struct PcapPortSetupData_Struct
{
	/** 下行帧源端口*/
	unsigned short m_usDownStreamPort;
	/** 上行帧源端口*/
	unsigned short m_usUpStreamPort;
}m_oPcapPortSetupDataStruct;
/**
* @struct PcapParameterSetupData_Struct
* @brief 从XML文件解析得到的参数数据
*/ 
typedef struct PcapParamSetupData_Struct
{
	/** Pcap程序路径*/
	string m_strPath;
	/** 网卡序号*/
	unsigned short m_usNetCardID;
	/** Pcap接收缓冲大小*/
	unsigned int m_uiPcapBufSize;
	/** 下行发送缓冲大小*/
	unsigned int m_uiDownStreamSndBufSize;
	/** 上行发送缓冲大小*/
	unsigned int m_uiUpStreamSndBufSize;
	/** Pcap数据包最大长度*/
	unsigned int m_uiPcapMaxPackageSize;
	/** Pcap超时时间*/
	unsigned int m_uiPcapTimeOut;
	/** Pcap发送等待时间*/
	unsigned int m_uiPcapSndWaitTime;
	/** Pcap接收等待时间*/
	unsigned int m_uiPcapRcvWaitTime;
	/** Pcap程序接收发送队列大小*/
	unsigned int m_uiPcapQueueSize;
}m_oPcapParamSetupDataStruct;
/**
* @struct NetPcapSetupData_Struct
* @brief 从XML文件中解析得到的NetPcap程序参数信息
*/
typedef struct NetPcapSetupData_Struct
{
	/** 资源同步对象*/
	CRITICAL_SECTION m_oSecCommInfo;
	/** XMLDOM文件对象*/
	CXMLDOMDocument m_oXMLDOMDocument;
	/** NetPcap的XML配置文件路径*/
	string m_strPcapXMLFilePath;
	/** 从XML文件中解析得到IP地址设置数据*/
	m_oPcapIPSetupDataStruct m_oXMLIPSetupData;
	/** 从XML文件中解析得到端口设置数据*/
	m_oPcapPortSetupDataStruct m_oXMLPortSetupData;
	/** 从XML文件解析得到的参数数据*/
	m_oPcapParamSetupDataStruct m_oPcapParamSetupData;
}m_oNetPcapSetupDataStruct;
#endif