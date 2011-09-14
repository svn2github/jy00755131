#pragma once

#include "ChannelSetupData.h"

/**
*@brief 测道数据队列类
*/
class CChannelList
{
public:
	CChannelList();
	~CChannelList();

public: //属性
	/** 测道数组指针*/
	CChannelSetupData* m_pArrayChannel;
	/** 测道数组指针，为现场数据输出准备*/
	CChannelSetupData* m_pArrayChannelOutput;
	/** 测道总数*/
	unsigned int m_uiCountAll;
	/** 使用中测道数量*/
	unsigned int m_uiCountUsed;
	/** 空闲测道数量*/
	unsigned int m_uiCountFree;
	/** 测道索引表*/
	CMap<unsigned int, unsigned int, CChannelSetupData*, CChannelSetupData*> m_oChannelMap;

public: //方法
	// 初始化
	void OnInit();
	// 重置
	void OnReset();
	// 重新加载
	void OnReload();
	// 关闭
	void OnClose();

	// 得到一个空闲测道
	CChannelSetupData* GetFreeChannel();
	// 增加一个测道到索引表
	void AddChannelToMap(unsigned int uiIndex, CChannelSetupData* pChannel);
	// 由索引表得到一个测道
	BOOL GetChannelFromMap(unsigned int uiIndex, CChannelSetupData* &pChannel);
	// 测道索引号是否已加入索引表
	BOOL IfIndexExistInMap(unsigned int uiIndex);
	// 得到一个测道
	CChannelSetupData* GetChannelByIndex(unsigned int uiIndex);
	// 得到测道
	CChannelSetupData* GetChannel(unsigned int uiNbLine, unsigned int uiNbPoint, unsigned int uiNbChannel);
	// 复制测道数组到输出数组
	void CopyChannelArrayForOutput();
};
