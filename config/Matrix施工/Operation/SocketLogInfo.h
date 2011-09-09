#pragma once

// CSocketLogInfo 命令目标
/**
 * @class CSocketLogInfo
 * @brief 接收和发送日志通信类
 *
 * @note 接收和发送日志信息。
 * 
 */
class CSocketLogInfo : public CSocket
{
public:
	CSocketLogInfo();
	virtual ~CSocketLogInfo();
	/// 创建Socket端口
	bool CreateSocket(UINT wPort, LPCTSTR strHostIP);
	/// 接收到数据
	virtual void OnReceive(int nErrorCode);
};
