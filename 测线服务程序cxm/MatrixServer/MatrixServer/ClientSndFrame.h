#pragma once
#include "ClientCommFrame.h"
#include "ClientSocket.h"
#include <map>
using std::map;
/**
* @class CClientSndFrame
* @brief 与客户端通讯发送帧类
*/
class CClientSndFrame : public CClientCommFrame
{
public:
	CClientSndFrame(void);
	~CClientSndFrame(void);
public:
	/** 客户端Socket类指针*/
	CClientSocket* m_pClientSocket;
	/**
	* @brief 已发送帧索引关键字
	*/
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
		/** 帧命令字，表明帧的功能*/
		unsigned short m_usCmd;
		/** 服务端时间戳*/
		unsigned int m_uiServerTimeStep;
		/** 包流水号*/
		unsigned int m_uiPacketIndex;
	}m_oSndFrameKey;
	/** 已发送帧索引*/
	map<m_oSndFrameKey, m_oCommFrameStructPtr> m_oSndFrameMap;
public:
	// 生成应答帧
	void MakeReturnFrame(m_oCommFrameStructPtr ptrFrame);
	// 生成设置帧
	void MakeSetFrame(unsigned short usCmd, char* pChar, unsigned int uiSize);
	// 生成发送帧
	void MakeSendFrame(m_oCommFrameStructPtr ptrFrame);
	// 重发过期帧
	bool OnReSendFrame(void);
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

