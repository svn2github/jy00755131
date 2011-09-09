#pragma once

// CSocketConfig 命令目标
/**
 * @class CSocketConfig
 * @brief 接收系统运行进程信息的SOCKET类
 *
 * @note 接收各个服务程序发送系统运行中数据流的进程信息。
通过该端口还发送config程序运行日志信息。
 * 
 */
class CSocketConfig : public CSocket
{
public:
	CSocketConfig();
	virtual ~CSocketConfig();
	/// 创建Socket端口
	bool CreateSocket(UINT wPort, LPCTSTR strHostIP);
	/// 接收到数据
	virtual void OnReceive(int nErrorCode);
};


