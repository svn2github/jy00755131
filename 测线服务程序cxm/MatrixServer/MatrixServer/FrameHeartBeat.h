#pragma once
#include "Parameter.h"
/**
*@brief 仪器心跳帧类
*/
class CFrameHeartBeat
{
public:
	CFrameHeartBeat();
	~CFrameHeartBeat();

public: // 属性
	/** 帧数据*/
	byte m_pFrameData[SndFrameSize];
public: // 方法
	// 重置对象
	void MakeHeartBeatFrame();
};
