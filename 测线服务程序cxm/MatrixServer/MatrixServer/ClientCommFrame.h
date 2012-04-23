#pragma once
#include "CommProtocol.h"
#include <list>
#include <map>
using std::list;
using std::map;
class CClientCommFrame
{
public:
	CClientCommFrame(void);
	~CClientCommFrame(void);
public:
	// 空闲的帧结构体数
	unsigned int m_uiCountFree;
	// 帧结构体数组
	m_oCommFrameStruct m_oCommFrameArray[FrameStructNumMax];
	// 队列资源同步对象
	CRITICAL_SECTION m_oSecClientFrame;
	// 空闲帧结构体队列
	list<m_oCommFrameStructPtr> m_olsCommFrameFree;
	// 等待处理帧结构体
	list<m_oCommFrameStructPtr> m_olsCommWorkFrame;
	// 包的流水号
	unsigned int m_uiPacketIndex;
	// 命令流水号
	unsigned int m_uiCmdIndex;
	// 已发送帧索引关键字
	typedef struct SndFrameKey
	{
		SndFrameKey(unsigned short usCmd, unsigned int uiTimeStep, unsigned int uiPacketIndex)
		{
			this->m_usCmd = usCmd;
			this->m_uiServerTimeStep = uiTimeStep;
			this->m_uiPacketIndex = uiPacketIndex;
		}
		~SndFrameKey()
		{
		}
		bool operator < (const SndFrameKey& rhs) const
		{
			if (m_usCmd == rhs.m_usCmd)
			{
				if (m_uiServerTimeStep == rhs.m_uiServerTimeStep)
				{
					return(m_uiPacketIndex < rhs.m_uiPacketIndex);
				} 
				else
				{
					return(m_uiServerTimeStep < rhs.m_uiServerTimeStep);
				}
			}
			else
			{
				return (m_usCmd < rhs.m_usCmd);
			}
		}
		// 帧命令字，表明帧的功能
		unsigned short m_usCmd;
		// 服务端时间戳
		unsigned int m_uiServerTimeStep;
		// 包流水号
		unsigned int m_uiPacketIndex;
	}m_oSndFrameKey;
	// 已发送帧索引
	map<m_oSndFrameKey, m_oCommFrameStructPtr> m_oSndFrameMap;
public:
	// 重置接收帧结构体
	void OnResetFrameStruct(m_oCommFrameStructPtr pFrameStruct);
	// 初始化
	void OnInit(void);
	// 关闭
	void OnClose(void);
	// 得到一个空闲接收帧结构体
	m_oCommFrameStructPtr GetFreeFrameStruct(void);
	// 增加一个空闲接收帧结构体
	void AddFreeFrameStruct(m_oCommFrameStructPtr pFrameStruct);
	// 判断索引号是否已加入索引表
	BOOL IfFramePtrExistInSndMap(unsigned short m_usCmd, unsigned int m_uiServerTimeStep, 
		unsigned int m_uiPacketIndex, map<m_oSndFrameKey, m_oCommFrameStructPtr>* pMap);
	// 增加对象到索引表
	void AddFramePtrToSndMap(unsigned short m_usCmd, unsigned int m_uiServerTimeStep, 
		unsigned int m_uiPacketIndex, m_oCommFrameStructPtr pFrameStruct, 
		map<m_oSndFrameKey, m_oCommFrameStructPtr>* pMap);
	// 根据输入索引号，由索引表得到指针
	m_oCommFrameStructPtr GetFramePtrFromSndMap(unsigned short m_usCmd, unsigned int m_uiServerTimeStep, 
		unsigned int m_uiPacketIndex, map<m_oSndFrameKey, m_oCommFrameStructPtr>* pMap);
	// 从索引表删除索引号指向的仪器指针
	BOOL DeleteFramePtrFromSndMap(unsigned short m_usCmd, unsigned int m_uiServerTimeStep, 
		unsigned int m_uiPacketIndex, map<m_oSndFrameKey, m_oCommFrameStructPtr>* pMap);
};

