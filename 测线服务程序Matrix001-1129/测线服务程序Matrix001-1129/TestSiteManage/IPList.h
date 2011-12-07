#pragma once

/**
*@brief 仪器IP地址队列类
*/
class CIPList
{
public:
	CIPList();
	~CIPList();

public: //属性
	/** 空闲IP队列*/
	CList<unsigned int, unsigned int> m_olsIPFree;
	/** IP总数*/
	unsigned int m_uiCountAll;
	/** 空闲IP数量*/
	unsigned int m_uiCountFree;

public: //方法
	// 初始化
	void OnInit();
	// 关闭
	void OnClose();
	// 重置
	void OnReset();

	// 得到一个空闲仪器IP地址
	unsigned int GetFreeInstrumentIP();
	// 增加一个空闲仪器IP地址
	void AddFreeInstrumentIP(unsigned int uiIP);

	// 得到一个空闲交叉站IP地址
	unsigned int GetFreeCrossIP();
	// 增加一个空闲交叉站IP地址
	void AddFreeCrossIP(unsigned int uiIP);

	// 得到一个空闲电源站IP地址
	unsigned int GetFreePowerIP();
	// 增加一个空闲电源站IP地址
	void AddFreePowerIP(unsigned int uiIP);

	// 得到一个空闲迂回道IP地址
	unsigned int GetDetourIP();
	// 增加一个空闲迂回道IP地址
	void AddDetourIP(unsigned int uiIP);

	// 得到一个采集站IP地址
	static unsigned int GetCollectIP(unsigned int uiLineNb, unsigned int uiPointNb, unsigned int uiChannelNb);
	// 得到一个爆炸机IP地址
	static unsigned int GetBlastMachineIP(unsigned int uiNb);
	//得到一个辅助道IP地址
	static unsigned int GetAuxIP(unsigned int uiNb);

	// 字符串转换为无符号整数
	static unsigned int ConvertStringToUnsignedInt(CString strData);
	// 数字IP地址转换为字符串IP地址
	static CString CIPList::IPToString(UINT uiIP);
	// 字符串IP地址转换为数字IP地址
	static UINT CIPList::StringToIP(CString strIP);

	// 得到仪器IP地址的类型	1-固定IP地址；2-浮动IP地址
	static int GetInstrumentIPType(unsigned int uiIP);
};
