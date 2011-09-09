#pragma once

//////////////////////////////////////////////////////////////////////////
// 配置文件更新自定义消息参数
#define     MSG_XMLFILE_NEW				(_T("MSG_XMLFileNew"))

extern      UINT NEAR WM_NEWXMLFILE;		// 在SocketGlobalFunction.cpp文件中初始化定义

////////////////////////////////////////////////////////////////////////////////////////////////////
// 程序编号定义，用于日志中记录日志来源，配置文件更新通知
#define		MATRIX_START				1
#define		MATRIX_ADMIN				2
#define		MATRIX_ADMINSERVER			3
#define		MATRIX_INSTALL				4
#define		MATRIX_CONFIG				5
#define		MATRIX_LINE					6
#define		MATRIX_LINESERVER			7
#define		MATRIX_OPERATION			8
#define		MATRIX_OPERATIONSERVER		9

////////////////////////////////////////////////////////////////////////////
// Line程序配置文件修改
#define		MATRIX_LINE_ALL				0		// 整个文件更新
#define		MATRIX_LINE_AUXI			1		// 辅助道
#define		MATRIX_LINE_SPREAD          2		// 采集排列



//////////////////////////////////////////////////////////////////////////
// Config程序配置文件通知属性定义
#define		MATRIX_CONFIG_ALL			0		// 整个文件更新
#define		MATRIX_CONFIG_CREWSETUP		1		// CrewSetup更新
#define		MATRIX_CONFIG_USERINFO		2		// Userinfo更新
#define		MATRIX_CONFIG_SEGDSETUP		3		// SEGDSetup更新
#define		MATRIX_CONFIG_DISKRECORD	4		// DiskRecord更新

//////////////////////////////////////////////////////////////////////////
// Install
#define		MATRIX_INSTALL_ALL			0

//////////////////////////////////////////////////////////////////////////
// operation程序配置文件
#define     MATRIX_OPERATION_ALL	    0