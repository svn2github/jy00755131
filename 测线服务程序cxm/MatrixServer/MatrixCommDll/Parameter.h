
/**
* @brief 常量设置
*/
/** 仪器SN位数*/
#define InstrumentBytes						4
/** 仪器表网格大小，仪器SN为0则表示无仪器*/
#define InstrumentTableWindowSize			169
/** 测试结果位数*/
#define TestResultBytes						1
/** 网络传输的数据位数*/
#define TestDataBytes						4
/** 数据表网格大小*/
#define DataTableWindowSize					270
/** C/S接收缓冲区大小*/
#define CommRecBufferSize					8192000
/** C/S发送缓冲区大小*/
#define CommSndBufferSize					8192000
/** C/S通讯端口*/
#define ServerClientPort					0x8330
/** 最多连接的客户端个数*/
#define ListenClientMaxNum					15
/** 网络接收缓冲区大小*/
#define ServerRecBufSize					8192000
/** 最大接收帧结构体数*/
#define FrameStructNumMax					5800
/** 线程关闭的等待时间*/
#define CloseClientCommThreadTimes			800
/** 线程执行的间隔时间*/
#define ClientCommThreadWaitTime			50
/** 线程执行的间隔次数*/
#define ClientCommThreadWaitTimes			10
/** 发送帧延时等待次数*/
#define SndFrameWaitTimes					5
/** 发送帧最多发送次数*/
#define SndFrameMaxNum						5
/** 客户端验证时间次数*/
#define ClientCheckCountNum					50
/** 客户端活跃时间间隔次数*/
#define ClientActiveCountNum				50
/** 数据处理缓冲区大小（20000站*（线号+点号+仪器指针地址））*/
#define ProcBufferSize						240000
/** 两次FieldOn时间间隔*/
#define FieldOnWaitForTime					120000
/**
* @brief 命令字和通讯协议/n
* 查询某个区域客户端只需发送行号和区域号/n
* 服务端则在区域号和行号后再加入仪器属性（SN（4个字节)后跟IP（4个字节）)或数值（数值(4个字节)后跟测试结果（1个字节））/n
* 查询仪器列表某个区域（4个字节的行号和4个字节的区域序号）
*/

/** 应答命令位（区别应答，进行与操作判别）*/
#define CmdReturnBit						(1 << 15)
/** 命令类型为设置命令*/
#define CmdTypeSet							0x01
/** 命令类型为应答*/
#define CmdTypeReturn						0x02
/** TCP/IP帧长限制*/
#define FrameSizeLimit						1400
/** 帧头长度*/
#define FrameHeadSize						4
/** 帧长长度*/
#define FrameLengthSize						2
/** 帧头信息长度*/
#define FrameHeadInfoSize					27
/** 帧尾长度*/
#define FrameTailSize						1
/** 帧内容限制*/
#define FrameInfoSizeLimit					(FrameSizeLimit-FrameHeadSize-FrameLengthSize-FrameHeadInfoSize-FrameTailSize)
/** 帧头*/
#define FrameHead1							(char)(0xaa)
/** 帧头*/
#define FrameHead2							(char)(0xa5)
/** 帧头*/
#define FrameHead3							0x5a
/** 帧头*/
#define FrameHead4							0x55
/** 帧尾*/
#define FrameTail							(char)(0xff)
/** 帧命令执行结果-等待执行*/
#define CmdResultWait						0x00
/** 帧命令执行结果-执行成功*/
#define CmdResultSuccess					0x01
/** 帧命令执行结果-执行失败*/
#define CmdResultFail						0x02
/** 帧命令执行结果-需要延时执行*/
#define CmdResultDelay						0x03
/** 帧命令执行结果-该命令已经执行*/
#define CmdResultDone						0x04
/** 测试结果-合格*/
#define TestResultQualified					0x01
/** 测试结果-不合格*/
#define TestResultFailure					0x02
/** 测试结果-无数据*/
#define TestResultNull						0x03
/** 客户端向服务端发送验证码*/
#define CommCheck							"天津海泰光电科技有限公司"

/**
* @brief 服务端与客户端通讯帧结构\n
* 业务逻辑：先找到帧头，帧头校验，成功则继续，失败则重新找帧头；读取帧内\n
* 容总长度，验证其合理性，不合理则重新找帧头；能找到帧尾则继续，否则先不\n
* 处理；验证帧尾，正确则对帧内容进行业务处理，错误则重新找帧头。
*/
typedef struct CommFrame_Struct
{
	// 	/** 帧头*/
	// 	char m_cFrameHead[4];
	// 	/** 从帧长之后到帧尾（包含帧尾）为帧内容总长度，需要小于帧长限制*/
	// 	unsigned short m_usFrameLength;
	/** 帧命令字、时间戳、包流水号均相同，则认为是同一帧*/
	/** 帧命令类型，为1则为命令，为3则为应答*/
	char m_cCmdType;
	/** 帧命令字，表明帧的功能*/
	unsigned short m_usCmd;
	/** 服务端时间戳*/
	unsigned int m_uiServerTimeStep;
	/** 客户端时间戳*/
	unsigned int m_uiClientTimeStep;
	/** 包流水号*/
	unsigned int m_uiPacketIndex;
	/** 命令流水号*/
	unsigned int m_uiCmdIndex;
	/** 总帧数*/
	unsigned int m_uiFrameNum;
	/** 帧序号（从1开始）*/
	unsigned int m_uiFrameIndex;
	/** 帧命令执行结果*/
	char m_cResult;
	/** 帧内容长度 = 帧内容总长度 – 2 – 4 – 4 – 4 - 1；*/
	/** FieldOn命令会应答4个字节表明客户端FieldOn需要等待的时间，
	以秒为单位为0则执行，否则等待)。*/
	unsigned short m_usFrameInfoSize;
	/** 帧内容指针，如果为查询命令则帧内容的前两个字节分别定义行号和区域号*/
	char m_pcFrameInfo[FrameInfoSizeLimit];
	// 	/** 帧尾*/
	// 	char m_cFrameTail;
	/** 超时计数*/
	unsigned int m_uiTimeOutCount;
	/** 发送次数计数*/
	unsigned int m_uiSndCount;
}m_oCommFrameStruct, *m_oCommFrameStructPtr;
