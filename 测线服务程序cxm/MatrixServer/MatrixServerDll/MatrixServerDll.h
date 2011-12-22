// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 MATRIXSERVERDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// MATRIXSERVERDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

#include <list>
using std::list;
using std::string;
#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#endif

#define MatrixServerDll_API extern "C" __declspec(dllexport)
// 此类是从 MatrixServerDll.dll 导出的
class MATRIXSERVERDLL_API CMatrixServerDll {
public:
	CMatrixServerDll(void);
	// TODO: 在此添加您的方法。
};

extern MATRIXSERVERDLL_API int nMatrixServerDll;

MATRIXSERVERDLL_API int fnMatrixServerDll(void);
// 从INI文件中解析得到的常量
typedef struct ConstVar_Struct
{
	//____帧格式设置____
	// 帧头长度
	int m_oFrameHeadSize;
	// 同步帧头
	byte* m_pFrameHeadCheck;
	// 命令字长度1字节
	int m_oFrameCmdSize1B;
	// 命令包长度1字节
	int m_oFramePacketSize1B;
	// 命令包长度2字节
	int m_oFramePacketSize2B;
	// 命令包长度4字节
	int m_oFramePacketSize4B;
	// ADC数据所占字节数
	int m_oADCDataSize3B;
	// 命令字个数最大值
	int m_oCommandWordMaxNum;
	// 0x18命令数组包含的最大字节数
	int m_oADCSetCommandMaxByte;
	// 发送帧缓冲区初值设定
	byte m_oSndFrameBufInit;
	// 接收的网络数据帧帧长度
	int m_oRcvFrameSize;
	// 发送的网络数据帧帧长度
	int m_oSndFrameSize;
	//____服务器与设备命令字设置____
	// 发送设置命令
	unsigned short m_oSendSetCmd;
	// 发送查询命令
	unsigned short m_oSendQueryCmd;
	// 发送ADC采样数据重发命令
	unsigned short m_oSendADCCmd;
	// TB开始采集开关控制命令(TB_L高8位)
	unsigned int m_oCmdTBCtrl;
	// 串号
	byte m_oCmdSn;
	// 首包时间
	byte m_oCmdHeadFrameTime;
	// 本地IP地址
	byte m_oCmdLocalIPAddr;
	// 本地系统时间
	byte m_oCmdLocalSysTime;
	// 本地时间修正高位
	byte m_oCmdLocalTimeFixedHigh;
	// 本地时间修正低位
	byte m_oCmdLocalTimeFixedLow;
	// 自动数据返回地址
	byte m_oCmdADCDataReturnAddr;
	// 自动数据返回端口和命令
	byte m_oCmdADCDataReturnPort;
	// 端口递增下限和上限
	byte m_oCmdADCDataReturnPortLimit;
	// 设置网络等待端口和命令
	byte m_oCmdSetBroadCastPort;
	// 系统硬件状态拷贝
	byte m_oCmdFDUErrorCode;
	// TB时刻高位
	byte m_oCmdTBHigh;
	// TB时刻低位
	byte m_oCmdTbLow;
	// work_ctrl 交叉站方向
	byte m_oCmdLAUXRoutOpenQuery;
	// 路由开关
	byte m_oCmdLAUXRoutOpenSet;
	// 尾包接收\发送时刻低位
	byte m_oCmdTailRecSndTimeLow;
	// 广播命令等待端口匹配
	byte m_oCmdBroadCastPortSet;
	// 设置ADC控制命令命令字
	byte m_oCmdADCSet;
	// 网络时刻
	byte m_oCmdNetTime;
	// 交叉站大线尾包接收时刻
	byte m_oCmdLineTailRecTimeLAUX;
	// 交叉站交叉线尾包接收时刻
	byte m_oCmdLAUTailRecTimeLAUX;
	// 交叉站故障1
	byte m_oCmdLAUXErrorCode1;
	// 交叉站故障2
	byte m_oCmdLAUXErrorCode2;
	// 交叉站路由分配
	byte m_oCmdLAUXSetRout;
	// 返回路由
	byte m_oCmdReturnRout;
	// 命令解析结束命令
	byte m_oCmdEnd;
}m_oConstVarStruct;
// 从XML文件中解析得到的信息
typedef struct InstrumentCommInfo_Struct
{
	/** XMLDOM文件对象*/
	CXMLDOMDocument m_oXMLDOMDocument;
	// 源地址
	unsigned int m_uiSrcIP;
	// 目的地址
	unsigned int m_uiDstIP;
	// 目标IP地址端口号
	unsigned int m_uiAimPort;
	// 心跳帧返回端口
	unsigned short m_usHeartBeatReturnPort;
	// 首包接收端口
	unsigned short m_usHeadFramePort;
	// IP地址设置返回端口
	unsigned short m_usIPSetReturnPort;
	// 仪器设备个数
	unsigned int m_uiInstrumentNum;
}m_oInstrumentCommInfoStruct;

// 线程处理标志位结构
typedef struct ThreadProcFlag_Struct
{
	// 标志位的资源同步对象
	CRITICAL_SECTION m_oSecFlag;
	// 心跳处理线程停止标志位
	bool m_bProcHeartBeatStop;
	// 首包处理线程停止标志位
	bool m_bProcHeadFrameStop;
	// 仪器IP地址设置线程停止标志位
	bool m_bProcIPSetStop;
}m_oThreadProcFlagStruct;

// 与设备通讯命令字内容
typedef struct InstrumentCommand_Struct
{
	// 源地址
	unsigned int m_uiSrcIP;
	// 目的地址
	unsigned int m_uiDstIP;
	// 目标IP地址端口号
	unsigned int m_uiAimPort;
	// 应答端口
	unsigned short m_usReturnPort;
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	unsigned short m_usCommand;
	// SN，低8位为仪器类型，0x01为交叉站，0x02为电源站，0x03为采集站
	unsigned int m_uiSN;
	// 首包时刻
	unsigned int m_uiTimeHeadFrame;
	// 仪器IP地址
	unsigned int m_uiInstrumentIP;
	// 本地时间
	unsigned int m_uiSysTime;
	// 本地时间修正高位
	unsigned int m_uiLocalTimeFixedHigh;
	// 本地时间修正低位
	unsigned short m_usLocalTimeFixedLow;
	// 自动数据返回地址
	unsigned int m_uiADCDataReturnAddr;
	// 自动数据返回端口
	unsigned short m_usADCDataReturnPort;
	// 自动数据返回命令，ad_cmd(7)=1，端口递增；=0，端口不变
	unsigned short m_usADCDataReturnCmd;
	// 当返回命令高位为1时，返回端口自动加1，到高限时返回低限端口
	// 端口递增下限
	unsigned short m_usADCDataReturnPortLimitLow;
	// 端口递增上限
	unsigned short m_usADCDataReturnPortLimitHigh;
	// 设置网络等待端口，指设置接收上位机广播命令的端口
	unsigned short m_usSetBroadCastPort;
	// 网络数据错误计数
	byte m_byFDUErrorCodeDataCount;
	// 命令错误计数
	byte m_byFDUErrorCodeCmdCount;
	// 时间报文状态
	byte m_byTimeStatus;
	// 控制状态
	byte m_byCtrlStatus;
	// TB时刻高位
	unsigned int m_uiTBHigh;
	// TB时刻低位
	unsigned short m_usTBLow;
	// TB控制，0x05启动TB，0x06则AD采集无需TB，0x00停止AD，ad_ctrl(2)=1则LED灯灭
	unsigned short m_usTBCtrl;
	// work_ctrl控制交叉站接收和发送命令的方向
	// 由高到低位每位分别控制发送口交叉线A、B，大线A、B，接收端交叉线A、B，大线A、B
	// =0为开，=1为关
	byte m_byLAUXRoutOpenQuery;
	// 路由开关
	// 由高位到低位分别控制开交叉线A、开交叉线B、开大线A、开大线B
	// =0为开，=1为关
	byte m_byLAUXRoutOpenSet;
	// 尾包接收\发送时刻低位//交叉站尾包发送时刻，低14位有效
	unsigned short m_usTailRecSndTimeLow;
	// 广播命令等待端口匹配，必须放在第一个命令字位置，并和0x0a命令中的16位端口匹配才能接收广播命令
	unsigned short m_usBroadCastPortSet;
	// 网络时刻
	unsigned int m_uiNetTime;
	// 交叉站大线A尾包接收时刻
	unsigned short m_usLAUXTailRecTimeLineA;
	// 交叉站大线B尾包接收时刻
	unsigned short m_usLAUXTailRecTimeLineB;
	// 交叉站交叉线A尾包接收时刻
	unsigned short m_usLAUXTailRecTimeLAUXLineA;
	// 交叉站交叉线B尾包接收时刻
	unsigned short m_usLAUXTailRecTimeLAUXLineB;
	// 交叉站大线A数据故障
	byte m_byLAUXErrorCodeDataLineACount;
	// 交叉站大线B数据故障
	byte m_byLAUXErrorCodeDataLineBCount;
	// 交叉站交叉线A数据故障
	byte m_byLAUXErrorCodeDataLAUXLineACount;
	// 交叉站交叉线B数据故障
	byte m_byLAUXErrorCodeDataLAUXLineBCount;
	// 交叉站命令口故障
	byte m_byLAUXErrorCodeCmdCount;
	// 交叉站上方路由IP
	unsigned int m_uiRoutIPTop;
	// 交叉站下方路由IP
	unsigned int m_uiRoutIPDown;
	// 交叉站左方路由IP
	unsigned int m_uiRoutIPLeft;
	// 交叉站右方路由IP
	unsigned int m_uiRoutIPRight;
	// 路由IP地址
	unsigned int m_uiRoutIP;
	// 0x18命令数组
	byte* m_pbyADCSet;
	// 0x18命令数据个数
	unsigned short m_usADCSetNum;
}m_oInstrumentCommandStruct;
// 心跳
typedef struct HeartBeatFrame_Struct
{
	// 心跳帧资源同步对象
	CRITICAL_SECTION m_oSecHeartBeat;
	// 发送帧缓冲区
	byte* m_pbySndFrameData;
	// 心跳命令字集合
	byte* m_pbyCommandWord;
	// 心跳命令字个数
	unsigned short m_usCommandWordNum;
	// 心跳帧命令
	m_oInstrumentCommandStruct m_oCommandStruct;
	// 心跳Socket套接字
	SOCKET m_oHeartBeatSocket;
}m_oHeartBeatFrameStruct;
// 首包
typedef struct HeadFrame_Struct
{
	// 首包帧资源同步对象
	CRITICAL_SECTION m_oSecHeadFrame;
	// 网络端口接收缓冲区大小
	unsigned int m_uiRcvBufferSize;
	// 接收帧缓冲区
	byte* m_pbyRcvFrameData;
	// 首包帧命令
	m_oInstrumentCommandStruct m_oCommandStruct;
	// 首包Socket套接字
	SOCKET m_oHeadFrameSocket;
}m_oHeadFrameStruct;
// IP地址设置
typedef struct IPSetFrame_Struct
{
	// IP地址设置帧资源同步对象
	CRITICAL_SECTION m_oSecIPSetFrame;
	// 网络端口发送缓冲区大小
	unsigned int m_uiSndBufferSize;
	// 发送帧缓冲区
	byte* m_pbySndFrameData;
	// IP地址设置命令字集合
	byte* m_pbyCommandWord;
	// IP地址设置命令字个数
	unsigned short m_usCommandWordNum;
	// IP地址设置帧命令
	m_oInstrumentCommandStruct m_oCommandStructSet;
	// 网络端口接收缓冲区大小
	unsigned int m_uiRcvBufferSize;
	// 接收帧缓冲区
	byte* m_pbyRcvFrameData;
	// IP地址设置应答帧命令
	m_oInstrumentCommandStruct m_oCommandStructReturn;
	// IP地址设置Socket套接字
	SOCKET m_oIPSetFrameSocket;
}m_oIPSetFrameStruct;
// 日志输出结构
typedef struct LogOutPut_Struct
{
	// 日志输出资源同步对象
	CRITICAL_SECTION m_oSecLogFile;
	// 日志输出文件指针
	FILE* m_pFile;
	// 日志输出结构
	list<string> m_oLogOutputList;
}m_oLogOutPutStruct;
// 环境结构体
typedef struct Environment_Struct
{
	// 从INI文件得到的常量信息
	m_oConstVarStruct* m_pConstVar;
	// 从XML文件得到的与仪器通讯信息结构
	m_oInstrumentCommInfoStruct* m_pInstrumentCommInfo;
	// 心跳帧结构
	m_oHeartBeatFrameStruct* m_pHeartBeatFrame;
	// 首包帧结构
	m_oHeadFrameStruct* m_pHeadFrame;
	// IP地址设置帧结构
	m_oIPSetFrameStruct* m_pIPSetFrame;
	// 线程处理标志位结构
	m_oThreadProcFlagStruct* m_pThreadProcFlag;
	// 日志输出结构
	m_oLogOutPutStruct* m_pLogOutPut;
}m_oEnvironmentStruct;


// 创建实例，并返回实例指针
typedef m_oEnvironmentStruct* (*Create_Instance)(void);
// 释放实例资源
typedef void (*Free_Instance)(m_oEnvironmentStruct* pEnv);
// 校验与设备通讯帧的同步码
typedef bool (*Instrument_CheckFrameHead)(byte* pFrameData);
// 生成帧的同步码
typedef void (*Instrument_MakeFrameHead)(byte* pFrameData);
// 重置帧内容解析变量
typedef void (*Instrument_ResetFramePacket)(m_oInstrumentCommandStruct* pCommand);
// 解析与设备通讯接收帧内容
typedef bool (*Instrument_ParseFrame)(m_oInstrumentCommandStruct* pCommand, byte* pFrameData);
// 生成与设备通讯帧
typedef void (*Instrument_MakeFrame)(m_oInstrumentCommandStruct* pCommand, byte* pFrameData);
// 创建CSocket接收端口并绑定端口和IP地址,必须先执行初始化该模块后才能创建该模块端口
typedef SOCKET (*Instrument_CreateSocket)(unsigned short usPort, unsigned int uiIP);
// 设置广播模式
typedef void (*Instrument_SetSocketBroadCast)(SOCKET oSocket);
// 设置端口接收缓冲区接收帧数量
typedef void (*Set_RcvBufferSize)(SOCKET oSocket, int iFrameCount);
// 设置端口发送缓冲区发送帧数量
typedef void (*Set_SndBufferSize)(SOCKET oSocket, int iFrameCount);
// 得到网络接收缓冲区收到的帧数量
typedef DWORD (*Get_FrameCount)(SOCKET oSocket);
// 得到帧数据
typedef bool (*Get_FrameData)(SOCKET oSocket, byte* pFrameData);
// 发送帧
typedef void (*Send_Frame)(SOCKET oSocket, byte* pFrameData, unsigned short usPort, unsigned int uiIP);

// 创建常量信息结构体
typedef void (*Create_ConstVar)(m_oEnvironmentStruct* pEnv);
// 初始化常量信息结构体
typedef void (*Init_ConstVar)(m_oEnvironmentStruct* pEnv);
// 释放常量信息结构体
typedef void (*Free_ConstVar)(m_oEnvironmentStruct* pEnv);
// 创建仪器通讯信息结构体
typedef void (*Instrument_CreateCommInfo)(m_oEnvironmentStruct* pEnv);
// 初始化仪器通讯信息结构体
typedef void (*Instrument_InitCommInfo)(m_oEnvironmentStruct* pEnv);
// 释放仪器通讯信息结构体
typedef void (*Instrument_FreeCommInfo)(m_oEnvironmentStruct* pEnv);
// 创建心跳帧信息结构体
typedef void (*Instrument_CreateHeartBeat)(m_oEnvironmentStruct* pEnv);
// 初始化心跳
typedef void (*Instrument_InitHeartBeat)(m_oEnvironmentStruct* pEnv);
// 释放心跳帧信息结构体
typedef void (*Instrument_FreeHeartBeat)(m_oEnvironmentStruct* pEnv);
// 创建首包帧信息结构体
typedef void (*Instrument_CreateHeadFrame)(m_oEnvironmentStruct* pEnv);
// 初始化首包
typedef void (*Instrument_InitHeadFrame)(m_oEnvironmentStruct* pEnv);
// 释放首包帧信息结构体
typedef void (*Instrument_FreeHeadFrame)(m_oEnvironmentStruct* pEnv);
// 创建IP地址设置帧信息结构体
typedef void (*Instrument_CreateIPSetFrame)(m_oEnvironmentStruct* pEnv);
// 初始化IP地址设置
typedef void (*Instrument_InitIPSetFrame)(m_oEnvironmentStruct* pEnv);
// 释放IP地址设置帧信息结构体
typedef void (*Instrument_FreeIPSetFrame)(m_oEnvironmentStruct* pEnv);
// 创建线程处理标志位信息结构体
typedef void (*Create_ThreadProcFlag)(m_oEnvironmentStruct* pEnv);
// 初始化线程处理标志位
typedef void (*Init_ThreadProcFlag)(m_oEnvironmentStruct* pEnv);
// 释放线程处理标志位信息结构体
typedef void (*Free_ThreadProcFlag)(m_oEnvironmentStruct* pEnv);
// 创建并设置心跳端口
typedef void (*CreateAndSet_HeartBeatSocket)(m_oEnvironmentStruct* pEnv);
// 创建并设置首包端口
typedef void (*CreateAndSet_HeadFrameSocket)(m_oEnvironmentStruct* pEnv);
// 创建并设置IP地址设置端口
typedef void (*CreateAndSet_IPSetFrameSocket)(m_oEnvironmentStruct* pEnv);

// 初始化实例
// 包括：
// 创建并初始化仪器通讯信息结构体
// 创建并初始化心跳帧信息结构体
// 创建并初始化首包帧信息结构体
// 创建并初始化IP地址设置帧信息结构体
// 创建并初始化线程处理标志位信息结构体
// 创建并设置心跳端口
// 创建并设置首包端口
// 创建并设置IP地址设置端口
typedef void (*Init_Instance)(m_oEnvironmentStruct* pEnv);

// 解析首包帧
typedef bool (*Instrument_ParseHeadFrame)(m_oEnvironmentStruct* pEnv);
// 解析IP地址设置应答帧
typedef bool (*Instrument_ParseIPSetReturnFrame)(m_oEnvironmentStruct* pEnv);
// 生成心跳帧
typedef void (*Instrument_MakeHeartBeatFrame)(m_oEnvironmentStruct* pEnv);
// 生成IP地址设置帧
typedef void (*Instrument_MakeIPSetFrame)(m_oEnvironmentStruct* pEnv);

// 将日志信息加入到日志文件输出队列
typedef void (*Add_LogFileOutPutList)(int iType, unsigned int uiErrorCodeNb, string strFuncName, string strLogInfo);