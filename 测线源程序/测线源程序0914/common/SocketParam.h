#pragma once

/////////////////////////用户管理//////////////////////////////////////////////////////
// AdminServer程序中管理用户信息的端口
#define  ADMINSERVER_USERINFOPORT	0xE000
// Start程序中与AdminServer程序通信的端口，主要包括用户登录
#define  START_USERANDLOGPORT		0xE080
// Admin程序管理用户的端口：注册新用户、删除用户等
#define  ADMIN_USERINFOPORT			0xE090

////////////////////////管理日志信息///////////////////////////////////////////////////
// AdminServer程序中管理日志信息的端口
#define  ADMINSERVER_LOGPORT		0xE100

//////////////////////config接收系统运行进程端口//////////////////////////////////////
// Config接收施工状态的端口：REV、PROC等
#define  CONFIG_SYSPROGRESS			0xE200

//////////////////////Install接收系统运行进程端口//////////////////////////////////////
// Install程序发送日志
#define  INSTALL_SYSPROGRESS		0xE300

////////////////////////Operation程序//////////////////////////////////////////////////
// Operation程序中管理日志信息的端口
#define  OPERATION_LOGPORT			0xE101
// Operation程序施工客户端socket，用于与服务器通信
#define  OPERATION_CLIENTPORT		0xAAFF


///////////////////////operationServer施工服务器程序端口定义///////////////////////////////////////
// operationServer服务器端socket端口，用于与客户端通信
#define  OPERATION_SVRPORT			0xAA00

// 用于与采集排列通信的端口, 实际占用端口为 SOCKET_FDUPORT + m_dwThreadID
#define  SOCKET_FDUPORT				0xA000
// 8个循环端口的起始值, 与线程ID计算出最小端口: 0xB000 + (ID<<8) + i(0~7) 
#define  SOCKET_REV8PORT			0xB000

// 定义施工服务器端与爆炸机控制器所连采集站通信的端口
#define  OPERATION_FDUBLASTERPORT	0xD000
// 定义施工服务器接受系统时刻的端口: 由测线程序定期发送主交叉站尾包或者由施工服务器主动查询
#define  OPERATION_LOCALTIMEPORT	0xC000


///////////////////////Log程序//////////////////////////////////////////////////////////////////////
// Log程序发送日志
#define  LOG_SYSPROGRESS			0xE400


// 由FTP服务器得到Matrix设置文件
extern bool GetMatrixXMLFromFTPServer(LPCTSTR strFTPServerIP,LPCTSTR strFTPFile,LPCTSTR strLocalFile);
// 保存Matrix设置文件到FTP服务器
extern bool PutMatrixXMLToFTPServer(LPCTSTR strFTPServerIP,LPCTSTR strFTPFile,LPCTSTR strLocalFile);