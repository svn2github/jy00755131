#pragma once
////////////////////////////////////////////////////////////////
// 该文件主要定义配置文件（XML文件）的文件名称，便于程序维护
// Matrix测线定义
#define    MATRIX_XMLFILE		(_T("Matrix.xml"))
// Start程序配置参数
#define    START_XMLFILE		(_T("MatrixStart.xml"))	
// AdminServer配置程序
#define    ADMINSERVER_XMLFILE		(_T("MatrixAdminServer.XML"))

// Install程序配置文件
#define    INSTALL_XMLFILE		(_T("MatrixInstall.xml"))
// 由Config程序配置的参数文件
#define    CONFIG_XMLFILE		(_T("MatrixConfiguration.xml"))
// Operation程序配置参数
#define    OPERATION_XMLFILE		(_T("MatrixOperation.xml"))
// OperationServer程序配置参数
#define    OPERATIONSERVER_XMLFILE	(_T("MatrixOperationServer.xml"))
// LineInit文件名
#define		LINEINIT_TXTFILE		(_T("LineInit.txt"))


////////////////////////////客户端及服务器程序子目录定义/////////////////////////////////////////////
// 客户端临时保存XML文件的子目录
#define		CLIENTDIR_XMLFILE		(_T("..\\Parameter\\"))
// 服务器端保存XML文件的子目录
#define		SERVERDIR_XMLFILE		(_T("..\\Parameter\\"))

// 客户端保存临时数据的子目录:如测线程序生成的FileChannel.dat等
#define		CLIENTDIR_DATAFILE		(_T("..\\Data\\"))

// 服务器端保存日志的子目录，由Adminserver程序保存日志文件
#define     SERVERDIR_LOGFILE		(_T("..\\Log\\"))