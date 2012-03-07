#pragma once


#include "ChannelSetupData.h"

class CChannelList
{
public:
	CChannelList(void);
	~CChannelList(void);

public: //属性
	// 测道数组指针
	CChannelSetupData* m_pArrayChannel;
	// 测道总数
	unsigned int m_uiCountAll;
	// 使用中测道数量
	unsigned int m_uiCountUsed;
	// 空闲测道数量
	unsigned int m_uiCountFree;
	// 测道索引表
	CMap<unsigned int, unsigned int, CChannelSetupData*, CChannelSetupData*> m_oChannelMap;

	/*
	unsigned int m_uiNbPointMin;	// 最小测点号
	unsigned int m_uiNbPointMax;	// 最大测点号
	unsigned int m_uiPointChannelCountMax;	// 测点上最大测道数量
	unsigned int m_uiNbLineCount;	// 测线数量
	
	CList<unsigned int, unsigned int> m_olsNbLine;	// 测线号队列
	CMap<unsigned int, unsigned int, unsigned int, unsigned int> m_oNbLineMap;	// 测线号索引表
	*/
public: //方法
	// 初始化
	void OnInit();
	// 关闭
	void OnClose();
	// 重置
	void OnReset();
	// 增加一个测道到索引表
	void AddChannelToMap(unsigned int uiIndex, CChannelSetupData* pChannel);
	// 由IP索引表得到一个测道
	BOOL GetChannelFromMap(unsigned int uiIndex, CChannelSetupData* &pChannel);
	// 测道索引号是否已加入索引表
	BOOL IfIndexExistInMap(unsigned int uiIndex);
	// 得到一个测道
	CChannelSetupData* GetChannelByIndex(unsigned int uiIndex);
	// 得到测道
	CChannelSetupData* GetChannel(unsigned int uiNbLine, unsigned int uiNbPoint, unsigned int uiNbChannel);
	// 加载现场测道数据队列
	void LoadSiteData();

};
