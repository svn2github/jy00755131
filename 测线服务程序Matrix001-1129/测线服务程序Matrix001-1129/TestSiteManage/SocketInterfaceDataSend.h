#pragma once

#include "FrameInterface.h"

/**
*@brief 服务器端数据帧发送网络接口类
*/
class CSocketInterfaceDataSend : public CSocket
{
public:
	CSocketInterfaceDataSend();
	virtual ~CSocketInterfaceDataSend();
public: // 属性

	/** 和内部通讯的本机IP地址*/
	CString m_strIPForInterface;
	/** 用于命令帧的端口号*/
	UINT m_uiPortForCommandSend;
	/** 客户端目标IP地址*/
	CString m_strIPAim;
	/** 客户端目标IP端口号*/
	UINT m_uiPortAim;

	/** 帧字节数*/
	int m_iFrameSize;
	/** 发送帧数据对象*/
	CFrameInterface m_oFrameInterface;

public: // 方法
	// 初始化
	BOOL OnInit(CString strIPForInterface, UINT uiPortForCommandSend, UINT uiPortAim);
	// 设置缓冲区大小
	BOOL SetBufferSize(int iBufferSize);
	// 发送数据帧
	int SendDataFrame();
	// 生成测试数据帧
	void CreateOutputDataFrameTest(unsigned int uiTestType, unsigned int uiDataSize, unsigned short usDataCount, byte* pData);
};


