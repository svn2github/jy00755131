#pragma once

#include "FrameTestData.h"
#include "Matrixline.h"

#define MY_MSG WM_USER+100

/**
*@brief 仪器测试数据帧接收网络接口类
*/
class CSocketTestDataFrame : public CSocket
{
public:
	CSocketTestDataFrame();
	virtual ~CSocketTestDataFrame();
	virtual void OnReceive(int nErrorCode);

public: // 属性
	/** 和现场仪器通讯的本机IP地址*/
	CString m_strIPForInstrument;
	/** 用于数据接收的端口号*/
	UINT m_uiPortForTestDataFrame;

	/** 数据帧字节数*/
	int m_iFrameSize;
	/** 数据接收帧计数*/
	DWORD m_irevFrameCount;
	/** 数据帧处理帧计数*/
	DWORD m_iFrameCount;
	/** 帧数据对象*/
	CFrameTestData m_oFrameTestData;
	/** 为采集接收线程ID对象*/
	CSocket m_TestDataSocket;
	DWORD m_oProcTestThreadID;
	UINT	  m_dwMessage;
	byte * OnReceive_buf;
	int okBegin;

public: // 方法
	// 初始化
	BOOL OnInit(CString strIPForInstrument, UINT uiPortForTestDataFrame);
	// 得到网络接收缓冲区收到的帧数量
	DWORD GetFrameCount();
	DWORD GetFrameCount1();
	// 设置接收缓冲区可以接收的帧数量
	BOOL SetBufferSize(int iFrameCount);
	// 得到帧数据
	BOOL GetFrameData();
	BOOL GetFrameData1();
	// 得到帧数据，不解析
	BOOL GetFrameDataNotParse();
	BOOL GetFrameDataNotParse1();

};


