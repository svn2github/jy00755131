#pragma once

/**
*@brief 仪器测试数据帧类
*/
class CFrameTestData
{
public:
	CFrameTestData();
	~CFrameTestData();

public: // 属性
	// 帧大小*/
	int m_iFrameSize;
	/** 帧数据*/
	byte m_pFrameData[256];
	/** 源IP地址*/
	unsigned int m_uiIPSource;
	/** 目标IP地址*/
	unsigned int m_uiIPAim;
	/** 目标端口号*/
	unsigned short m_usPortAim;
	/** 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发*/
	unsigned short m_usCommand;

	/** 数据索引*/
	unsigned short m_usDataIndex;
	/** 测试数据*/
	byte* m_pData;

public: // 方法
	// 重置对象
	void Reset();
	// 解析数据帧
	bool ParseFrame();
};
