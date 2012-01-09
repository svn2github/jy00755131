// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 MATRIXSERVERDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// MATRIXSERVERDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

#include <list>
#include <hash_map>
#include "Resource.h"
using std::list;
using stdext::hash_map;
using std::string;
#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

// 此类是从 MatrixServerDll.dll 导出的
class MATRIXSERVERDLL_API CMatrixServerDll {
public:
	CMatrixServerDll(void);
	// TODO: 在此添加您的方法。
};

extern MATRIXSERVERDLL_API int nMatrixServerDll;

MATRIXSERVERDLL_API int fnMatrixServerDll(void);
// ini参数文件相对路径
#define INIFilePath			_T("..\\parameter\\MatrixServerDLL.ini")
// 测线网络配置XML文件相对路径
#define CommXMLFilePath		_T("..\\parameter\\MatrixLineApp.XML")
// 日志文件夹
#define LogFolderPath		_T("..\\Log")
// 输出选择:Debug输出则为0，Release输出则为1
#define OutPutSelect		0
// 输出错误日志上限
#define OutPutLogErrorLimit	100
// 日志输出类型
enum{LogType, WarningType, ErrorType, ExpandType};
// 日志输出结构
typedef struct LogOutPut_Struct
{
	// 日志输出资源同步对象
	CRITICAL_SECTION m_oSecLogFile;
	// 日志输出文件指针
	FILE* m_pFile;
	// 日志输出结构
	list<string> m_oLogOutputList;
	// 错误计数
	unsigned int m_uiErrorCount;
}m_oLogOutPutStruct;
// 从INI文件中解析得到的常量
typedef struct ConstVar_Struct
{
	//____常量设置____
	// 一次休眠的时间
	int m_iOneSleepTime;
	// 日志输出线程写日志的延时次数
	int m_iLogOutPutSleepTimes;
	// 心跳线程发送心跳帧延时次数
	int m_iHeartBeatSleepTimes;
	// 首包线程接收首包延时次数
	int m_iHeadFrameSleepTimes;
	// IP地址设置线程延时次数
	int m_iIPSetFrameSleepTimes;
	// 尾包线程延时次数
	int m_iTailFrameSleepTimes;
	// 路由监视线程延时次数
	int m_iMonitorRoutSleepTimes;
	// 等待线程关闭的延时次数
	int m_iCloseThreadSleepTimes;

	// 首包稳定计数
	int m_iHeadFrameStableTimes;
	// IP地址重设次数
	int m_iIPAddrResetTimes;
	// 尾包稳定次数
	int m_iTailFrameStableTimes;
	// 路由监视稳定时间
	int m_iMonitorRoutStableTime;

	// 仪器类型-交叉站
	char m_byInstrumentTypeLAUX;
	// 仪器类型-电源站
	char m_byInstrumentTypeLAUL;
	// 仪器类型-采集站
	char m_byInstrumentTypeFDU;
	// 仪器类型-LCI
	char m_byInstrumentTypeLCI;
	// 方向上方
	int m_iDirectionTop;
	// 方向下方
	int m_iDirectionDown;
	// 方向左方
	int m_iDirectionLeft;
	// 方向右方
	int m_iDirectionRight;
	// 方向正中
	int m_iDirectionCenter;
	// IP地址设置的起始地址
	int m_iIPSetAddrStart;
	// IP地址设置的间隔
	int m_iIPSetAddrInterval;
	// 路由地址设置的起始地址
	int m_iRoutSetAddrStart;
	// 路由地址设置的间隔
	int m_iRoutSetAddrInterval;
	// 设置广播端口起始地址
	int m_iBroadcastPortStart;
	// 设置为广播IP
	int m_iIPBroadcastAddr;

	//____帧格式设置____
	// 帧头长度
	int m_iFrameHeadSize;
	// 同步帧头
	char* m_pFrameHeadCheck;
	// 命令字长度1字节
	int m_iFrameCmdSize1B;
	// 命令包长度1字节
	int m_iFramePacketSize1B;
	// 命令包长度2字节
	int m_iFramePacketSize2B;
	// 命令包长度4字节
	int m_iFramePacketSize4B;
	// ADC数据所占字节数
	int m_iADCDataSize3B;
	// 命令字个数最大值
	int m_iCommandWordMaxNum;
	// 0x18命令数组包含的最大字节数
	int m_iADCSetCommandMaxByte;
	// 发送帧缓冲区初值设定
	char m_cSndFrameBufInit;
	// 接收的网络数据帧帧长度
	int m_iRcvFrameSize;
	// 发送的网络数据帧帧长度
	int m_iSndFrameSize;
	//____服务器与设备命令字设置____
	// 发送设置命令
	unsigned short m_usSendSetCmd;
	// 发送查询命令
	unsigned short m_usSendQueryCmd;
	// 发送ADC采样数据重发命令
	unsigned short m_usSendADCCmd;
	// TB开始采集开关控制命令(TB_L高8位)
	unsigned int m_uiCmdTBCtrl;
	// 串号
	char m_byCmdSn;
	// 首包时间
	char m_byCmdHeadFrameTime;
	// 本地IP地址
	char m_byCmdLocalIPAddr;
	// 本地系统时间
	char m_byCmdLocalSysTime;
	// 本地时间修正高位
	char m_byCmdLocalTimeFixedHigh;
	// 本地时间修正低位
	char m_byCmdLocalTimeFixedLow;
	// 自动数据返回地址
	char m_byCmdADCDataReturnAddr;
	// 自动数据返回端口和命令
	char m_byCmdADCDataReturnPort;
	// 端口递增下限和上限
	char m_byCmdADCDataReturnPortLimit;
	// 设置网络等待端口和命令
	char m_byCmdSetBroadCastPort;
	// 系统硬件状态拷贝
	char m_byCmdFDUErrorCode;
	// TB时刻高位
	char m_byCmdTBHigh;
	// TB时刻低位
	char m_byCmdTbLow;
	// work_ctrl 交叉站方向
	char m_byCmdLAUXRoutOpenQuery;
	// 路由开关
	char m_byCmdLAUXRoutOpenSet;
	// 尾包接收\发送时刻低位
	char m_byCmdTailRecSndTimeLow;
	// 广播命令等待端口匹配
	char m_byCmdBroadCastPortSet;
	// 设置ADC控制命令命令字
	char m_byCmdADCSet;
	// 网络时刻
	char m_byCmdNetTime;
	// 交叉站大线尾包接收时刻
	char m_byCmdLineTailRecTimeLAUX;
	// 交叉站交叉线尾包接收时刻
	char m_byCmdLAUTailRecTimeLAUX;
	// 交叉站故障1
	char m_byCmdLAUXErrorCode1;
	// 交叉站故障2
	char m_byCmdLAUXErrorCode2;
	// 交叉站路由分配
	char m_byCmdLAUXSetRout;
	// 返回路由
	char m_byCmdReturnRout;
	// 命令解析结束命令
	char m_byCmdEnd;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPut;
}m_oConstVarStruct;
// 从XML文件中解析得到的信息
typedef struct InstrumentCommInfo_Struct
{
	/** XMLDOM文件对象*/
	CXMLDOMDocument m_oXMLDOMDocument;
	// 源地址
	unsigned int m_uiSrcIP;
	// LCI的IP地址
	unsigned int m_uiAimIP;
	// LCI接收的端口号
	unsigned int m_uiAimPort;
	// 心跳帧返回端口
	unsigned short m_usHeartBeatReturnPort;
	// 首包接收端口
	unsigned short m_usHeadFramePort;
	// IP地址设置返回端口
	unsigned short m_usIPSetReturnPort;
	// 尾包接收端口
	unsigned short m_usTailFramePort;
	// 仪器设备个数
	unsigned int m_uiInstrumentNum;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPut;
}m_oInstrumentCommInfoStruct;

// 与设备通讯命令字内容
typedef struct InstrumentCommand_Struct
{
	// LCI的IP地址
	unsigned int m_uiAimIP;
	// LCI接收的端口号
	unsigned int m_uiAimPort;
	// 源地址
	unsigned int m_uiSrcIP;
	// 目的地址
	unsigned int m_uiDstIP;
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
	unsigned int m_uiSetBroadCastPort;
	// 网络数据错误计数
	char m_byFDUErrorCodeDataCount;
	// 命令错误计数
	char m_byFDUErrorCodeCmdCount;
	// 时间报文状态
	char m_byTimeStatus;
	// 控制状态
	char m_byCtrlStatus;
	// TB时刻高位
	unsigned int m_uiTBHigh;
	// TB时刻低位
	unsigned short m_usTBLow;
	// TB控制，0x05启动TB，0x06则AD采集无需TB，0x00停止AD，ad_ctrl(2)=1则LED灯灭
	unsigned short m_usTBCtrl;
	// work_ctrl控制交叉站接收和发送命令的方向
	// 由高到低位每位分别控制发送口交叉线A、B，大线A、B，接收端交叉线A、B，大线A、B
	// =0为开，=1为关
	char m_byLAUXRoutOpenQuery;
	// 路由开关
	// 由高位到低位分别控制开交叉线A、开交叉线B、开大线A、开大线B
	// =0为开，=1为关
	char m_byLAUXRoutOpenSet;
	// 尾包接收\发送时刻低位//交叉站尾包发送时刻，低14位有效
	unsigned short m_usTailRecSndTimeLow;
	// 广播命令等待端口匹配，必须放在第一个命令字位置，并和0x0a命令中的16位端口匹配才能接收广播命令
	unsigned int m_uiBroadCastPortSet;
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
	char m_byLAUXErrorCodeDataLineACount;
	// 交叉站大线B数据故障
	char m_byLAUXErrorCodeDataLineBCount;
	// 交叉站交叉线A数据故障
	char m_byLAUXErrorCodeDataLAUXLineACount;
	// 交叉站交叉线B数据故障
	char m_byLAUXErrorCodeDataLAUXLineBCount;
	// 交叉站命令口故障
	char m_byLAUXErrorCodeCmdCount;
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
	char* m_pcADCSet;
	// 0x18命令数据个数
	unsigned short m_usADCSetNum;
	/** 仪器类型 1-交叉站；2-电源站；3-采集站；4-LCI*/
	unsigned int m_uiInstrumentType;
}m_oInstrumentCommandStruct;

// 仪器属性结构体
typedef struct Instrument_Struct 
{
	/** 在仪器数组中的位置*/
	unsigned int m_uiIndex;
	/** 仪器是否使用中*/
	bool m_bInUsed;
	/** 仪器设备号*/
	unsigned int m_uiSN;
	/** 仪器类型 1-交叉站；2-电源站；3-采集站；4-LCI*/
	unsigned int m_uiInstrumentType;
	/** 仪器IP地址*/
	unsigned int m_uiIP;
	/** 仪器路由IP地址*/
	unsigned int m_uiRoutIP;
	/** 路由方向 1-上；2-下；3-左；4右*/
	unsigned int m_uiRoutDirection;
	/** 路由地址 交叉线方向 上方*/
	unsigned int m_uiRoutIPTop;
	/** 路由地址 交叉线方向 下方*/
	unsigned int m_uiRoutIPDown;
	/** 路由地址 测线线方向 左方*/
	unsigned int m_uiRoutIPLeft;
	/** 路由地址 测线线方向 右方*/
	unsigned int m_uiRoutIPRight;
	/** 路由开关*/
	byte m_byLAUXRoutOpenSet;
	/** 链接的仪器 上方*/
	Instrument_Struct* m_pInstrumentTop;
	/** 链接的仪器 下方*/
	Instrument_Struct* m_pInstrumentDown;
	/** 链接的仪器 左方*/
	Instrument_Struct* m_pInstrumentLeft;
	/** 链接的仪器 右方*/
	Instrument_Struct* m_pInstrumentRight;
	/** 首包时刻*/
	unsigned int m_uiTimeHeadFrame;
	/** 尾包计数*/
	int m_iTailFrameCount;
	/** 仪器时延*/
	unsigned int m_uiTimeDelay;
	/** 仪器本地系统时间*/
	unsigned int m_uiTimeSystem;
	/** 仪器网络时间*/
	unsigned int m_uiNetTime;
	/** 仪器网络状态*/
	unsigned int m_uiNetState;
	/** 仪器参数备用1*/
	unsigned int m_uiNetOrder;
	/** 16bits时间，接收时刻低位*/
	unsigned short m_usReceiveTime;	
	/** 16bits时间，发送时刻低位*/
	unsigned short m_usSendTime;
	/** 16bits 测线方向左面尾包接收时刻*/
	unsigned short m_usLineLeftReceiveTime;	
	/** 16bits 测线方向右面尾包接收时刻*/
	unsigned short m_usLineRightReceiveTime;	
	/** 16bits 交叉线方向上面尾包接收时刻*/
	unsigned short m_usCrossTopReceiveTime;	
	/** 16bits 交叉线方下面尾包接收时刻*/
	unsigned short m_usCrossDownReceiveTime;	
	/** 采集站或电源站的广播端口*/
	unsigned int m_uiBroadCastPort;
 	/** 测线*/
	int m_iLineNb;
	/** 测点号*/
	int m_iPointIndex;
	/** 标记点号*/
	unsigned int m_uiPointNb;
	// 	/** 测道*/
	// 	unsigned int m_uiChannelNb;
	/** 是否跳过道*/
	bool m_bJumpedChannel;
	/** 是否连接检波器*/
	bool m_bSensor;
	/** 是否辅助道*/
	bool m_bAux;
	/** 是否连接爆炸机*/
	bool m_bBlastMachine;
	/** 是否迂回道*/
	bool m_bDetour;
	/** 是否迂回道低端标记点*/
	bool m_bDetourMarkerLow;
	/** 是否迂回道高端标记点*/
	bool m_bDetourMarkerHigh;
	/** 是否迂回道停止标记*/
	unsigned int m_uiStopMarking;
	/** 是否标记点*/
	bool m_bMarker;
	// 首包位置稳定次数
	int m_iHeadFrameStableNum;
	/** 第几次设置IP地址*/
	int m_iIPSetCount;
	/** IP地址设置是否成功*/
	bool m_bIPSetOK;
	/** 第几次尾包时刻查询*/
	int m_iTailTimeQueryCount;
	/** 尾包时刻查询是否成功*/
	bool m_bTailTimeQueryOK;
	/** 本地时间修正高位*/
	unsigned int m_uiTimeHigh;
	/** 本地时间修正低位*/
	unsigned int m_uiTimeLow;
	/** 第几次设置仪器时间*/
	int m_iTimeSetCount;
	/** 仪器时间设置是否成功*/
	bool m_bTimeSetOK;
}m_oInstrumentStruct;

// 仪器队列
typedef struct InstrumentList_Struct
{
	// 仪器队列资源同步对象
	CRITICAL_SECTION m_oSecInstrumentList;
	/** 仪器数组指针*/
	m_oInstrumentStruct* m_pArrayInstrument;
	/** 空闲仪器队列*/
	list<m_oInstrumentStruct*> m_olsInstrumentFree;
	/** 设置IP地址的仪器队列*/
	hash_map<unsigned int, m_oInstrumentStruct*> m_oIPSetMap;
	// 仪器SN索引表
	hash_map<unsigned int, m_oInstrumentStruct*> m_oSNInstrumentMap;
	// 仪器IP地址索引表
	hash_map<unsigned int, m_oInstrumentStruct*> m_oIPInstrumentMap;
	/** 仪器总数*/
	unsigned int m_uiCountAll;
	/** 空闲仪器数量*/
	unsigned int m_uiCountFree;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPut;
}m_oInstrumentListStruct;
// 路由属性结构体
typedef struct Rout_Struct
{
	/** 在路由数组中的位置*/
	unsigned int m_uiIndex;
	/** 路由是否使用中*/
	bool m_bInUsed;
	/** 路由IP地址*/
	unsigned int m_uiRoutIP;
	/** 路由方向 1-上；2-下；3-左；4右*/
	unsigned int m_uiRoutDirection;
	/** 路由头仪器*/
	Instrument_Struct* m_pHead;
	/** 路由尾仪器*/
	Instrument_Struct* m_pTail;
	/** 路由时刻*/
	unsigned int m_uiRoutTime;
	/** 上次时统处理时刻*/
	unsigned int m_uiDelayProcTime;
}m_oRoutStruct;
// 路由队列结构体
typedef struct RoutList_Struct
{
	// 路由队列资源同步对象
	CRITICAL_SECTION m_oSecRoutList;
	/** 路由数组指针*/
	m_oRoutStruct* m_pArrayRout;
	/** 空闲路由队列*/
	list<m_oRoutStruct*> m_olsRoutFree;
	// 仪器路由地址索引表
	hash_map<unsigned int, m_oRoutStruct*> m_oRoutMap;
	/** 路由总数*/
	unsigned int m_uiCountAll;
	/** 空闲路由数量*/
	unsigned int m_uiCountFree;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPut;
}m_oRoutListStruct;
// 心跳
typedef struct HeartBeatFrame_Struct
{
	// 心跳帧资源同步对象
	CRITICAL_SECTION m_oSecHeartBeat;
	// 发送帧缓冲区
	char* m_pcSndFrameData;
	// 心跳命令字集合
	char* m_pcCommandWord;
	// 心跳命令字个数
	unsigned short m_usCommandWordNum;
	// 心跳帧命令
	m_oInstrumentCommandStruct* m_pCommandStruct;
	// 心跳Socket套接字
	SOCKET m_oHeartBeatSocket;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPut;
}m_oHeartBeatFrameStruct;

// 首包
typedef struct HeadFrame_Struct
{
	// 首包帧资源同步对象
	CRITICAL_SECTION m_oSecHeadFrame;
	// 网络端口接收缓冲区大小
	unsigned int m_uiRcvBufferSize;
	// 接收帧缓冲区
	char* m_pcRcvFrameData;
	// 首包帧命令
	m_oInstrumentCommandStruct* m_pCommandStruct;
	// 首包Socket套接字
	SOCKET m_oHeadFrameSocket;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPut;
}m_oHeadFrameStruct;

// IP地址设置
typedef struct IPSetFrame_Struct
{
	// IP地址设置帧资源同步对象
	CRITICAL_SECTION m_oSecIPSetFrame;
	// 网络端口发送缓冲区大小
	unsigned int m_uiSndBufferSize;
	// 发送帧缓冲区
	char* m_pcSndFrameData;
	// IP地址设置命令字集合
	char* m_pcCommandWord;
	// IP地址设置命令字个数
	unsigned short m_usCommandWordNum;
	// IP地址设置帧命令
	m_oInstrumentCommandStruct* m_pCommandStructSet;
	// 网络端口接收缓冲区大小
	unsigned int m_uiRcvBufferSize;
	// 接收帧缓冲区
	char* m_pcRcvFrameData;
	// IP地址设置应答帧命令
	m_oInstrumentCommandStruct* m_pCommandStructReturn;
	// IP地址设置Socket套接字
	SOCKET m_oIPSetFrameSocket;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPut;
}m_oIPSetFrameStruct;
// 尾包
typedef struct TailFrame_Struct
{
	// 首包帧资源同步对象
	CRITICAL_SECTION m_oSecTailFrame;
	// 网络端口接收缓冲区大小
	unsigned int m_uiRcvBufferSize;
	// 接收帧缓冲区
	char* m_pcRcvFrameData;
	// 尾包帧命令
	m_oInstrumentCommandStruct* m_pCommandStruct;
	// 尾包Socket套接字
	SOCKET m_oTailFrameSocket;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPut;
}m_oTailFrameStruct;
// 线程结构体
typedef struct Thread_Struct
{
	// 线程句柄
	HANDLE m_hThread;
	// 线程号
	DWORD m_dwThreadID;
	//是否工作状态
	bool m_bWork;
	// 是否关闭线程
	bool m_bClose;
	// 线程结束事件
	HANDLE m_hThreadClose;
	// 常量指针
	m_oConstVarStruct* m_pConstVar;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPut;
}m_oThreadStruct;
// 日志输出线程
typedef struct LogOutPutThread_Struct
{
	// 线程结构体指针
	m_oThreadStruct* m_pThread; 
}m_oLogOutPutThreadStruct;
// 心跳线程
typedef struct HeartBeatThread_Struct
{
	// 线程结构体指针
	m_oThreadStruct* m_pThread;
	// 心跳帧指针
	m_oHeartBeatFrameStruct* m_pHeartBeatFrame;
}m_oHeartBeatThreadStruct;
// 首包线程
typedef struct HeadFrameThread_Struct
{
	// 线程结构体指针
	m_oThreadStruct* m_pThread;
	// 首包帧指针
	m_oHeadFrameStruct* m_pHeadFrame;
	// 仪器队列结构体指针
	m_oInstrumentListStruct* m_pInstrumentList;
	// 路由队列结构体指针
	m_oRoutListStruct* m_pRoutList;
}m_oHeadFrameThreadStruct;
// IP地址设置线程
typedef struct IPSetFrameThread_Struct
{
	// 线程结构体指针
	m_oThreadStruct* m_pThread;
	// IP地址设置帧指针
	m_oIPSetFrameStruct* m_pIPSetFrame;
	// 仪器队列结构体指针
	m_oInstrumentListStruct* m_pInstrumentList;
}m_oIPSetFrameThreadStruct;
// 尾包线程
typedef struct TailFrameThread_Struct
{
	// 线程结构体指针
	m_oThreadStruct* m_pThread;
	// 尾包帧指针
	m_oTailFrameStruct* m_pTailFrame;
	// 仪器队列结构体指针
	m_oInstrumentListStruct* m_pInstrumentList;
	// 路由队列结构体指针
	m_oRoutListStruct* m_pRoutList;
}m_oTailFrameThreadStruct;
// 路由监视线程
typedef struct MonitorRoutThread_Struct
{
	// 线程结构体指针
	m_oThreadStruct* m_pThread;
	// 仪器队列结构体指针
	m_oInstrumentListStruct* m_pInstrumentList;
	// 路由队列结构体指针
	m_oRoutListStruct* m_pRoutList;
}m_oMonitorRoutThreadStruct;
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
	// 尾包帧结构
	m_oTailFrameStruct* m_pTailFrame;
	// 日志输出结构
	m_oLogOutPutStruct* m_pLogOutPut;
	// 仪器队列结构
	m_oInstrumentListStruct* m_pInstrumentList;
	// 路由对列结构
	m_oRoutListStruct* m_pRoutList;
	// 日志输出线程
	m_oLogOutPutThreadStruct* m_pLogOutPutThread;
	// 心跳线程
	m_oHeartBeatThreadStruct* m_pHeartBeatThread;
	// 首包接收线程
	m_oHeadFrameThreadStruct* m_pHeadFrameThread;
	// IP地址设置线程
	m_oIPSetFrameThreadStruct* m_pIPSetFrameThread;
	// 尾包接收线程
	m_oTailFrameThreadStruct* m_pTailFrameThread;
	// 路由监视线程
	m_oMonitorRoutThreadStruct* m_pMonitorRoutThread;
}m_oEnvironmentStruct;


// 创建实例，并返回实例指针
typedef m_oEnvironmentStruct* (*Create_Instance)(void);
// 释放实例资源
typedef void (*Free_Instance)(m_oEnvironmentStruct* pEnv);
// 校验与设备通讯帧的同步码
typedef bool (*Instrument_CheckFrameHead)(char* pFrameData);
// 生成帧的同步码
typedef void (*Instrument_MakeFrameHead)(char* pFrameData);
// 重置帧内容解析变量
typedef void (*Instrument_ResetFramePacket)(m_oInstrumentCommandStruct* pCommand);
// 解析与设备通讯接收帧内容
typedef bool (*Instrument_ParseFrame)(m_oInstrumentCommandStruct* pCommand, char* pFrameData);
// 生成与设备通讯帧
typedef void (*Instrument_MakeFrame)(m_oInstrumentCommandStruct* pCommand, char* pFrameData);
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
typedef bool (*Get_FrameData)(SOCKET oSocket, char* pFrameData);
// 发送帧
typedef void (*Send_Frame)(SOCKET oSocket, char* pFrameData, unsigned short usPort, unsigned int uiIP);

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
typedef void (*On_Init)(m_oEnvironmentStruct* pEnv);
typedef void (*On_Work)(m_oEnvironmentStruct* pEnv);
typedef void (*On_Stop)(m_oEnvironmentStruct* pEnv);
typedef void (*On_Close)(m_oEnvironmentStruct* pEnv);
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