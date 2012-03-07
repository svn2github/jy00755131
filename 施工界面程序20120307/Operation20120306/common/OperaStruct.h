#pragma once
/******************************************************************************
  
    功    能：定义施工客户端和施工服务程序需要使用的数据结构

    修改历史：

*******************************************************************************/
#include "FDUConfig.h"
//////////////////////////////////////////////////////////////////////////
// 定义 FDU 最大数量
#define  FDU_MAXINUM	   20000


//////////////////////////////////////////////////////////////////////////
// 定义施工客户端与服务器命令码
#define  OPERATION_CMD_REQUEST	0x1000	// 请求放炮
#define  OPERATION_CMD_CONFIRM	0x1001	// 确认放炮
#define  OPERATION_CMD_SHOT		0x1002	// 发送炮号对象、震源对象
#define  OPERATION_CMD_PROCESS	0x1003	// 发送处理方法对象
#define  OPERATION_CMD_SPREAD	0x1004	// 发送采集排列编号、标签、类型等
#define  OPERATION_CMD_CHANNEL	0x1005	// 发送采集排列的所有采集道	
#define  OPERATION_CMD_FDUCONF	0x1006	// 发送采集站的配置：采样率、放大器、滤波器等参数

#define  OPERATION_CMD_TBUPHOLE	0x1008	// 爆炸机参数：确认TB、井口时间
#define  OPERATION_CMD_ITBERR	0x1009	// 服务器向客户发送 ITB 错误

#define  OPERATION_CMD_SRCRDY	0x1010	// 爆炸机控制器准备好
#define  OPERATION_CMD_SHOTEND	0x1011	// 结束放炮，由服务器通知客户端此次放炮结束

#define  OPERATION_CMD_RCVING	0x1012  // 采集开始, 采集开始结束、处理开始结束和输出开始结束的状态命令编号必须连在一起
#define  OPERATION_CMD_RCVED	0x1013	// 采集结束
#define  OPERATION_CMD_PROCING	0x1014	// 处理开始
#define  OPERATION_CMD_PROCED	0x1015	// 处理结束
#define  OPERATION_CMD_OUTING	0x1016	// 输出开始
#define  OPERATION_CMD_OUTED	0x1017	// 输出结束

// 施工客户端发送给服务器，控制放炮进程的命令码
#define  OPERATION_CMD_AGAIN	0x1030	// Again：停止当前的放炮状态，恢复到初始状态重新放这一炮。施工客户端和服务器之间保持联系
#define  OPERATION_CMD_CANCEL   0x1031	// cancel取消此次放炮，删除激活管理对象，施工服务器线程处于空闲
#define  OPERATION_CMD_START	0x1032  // 开始放炮,点名
#define  OPERATION_CMD_SHOTGO	0x1033	// 开始放炮，准备接收数据GO
#define  OPERATION_CMD_PROCGO	0x1034	// 采集结束，准备处理数据



#define  OPERATION_ACK_NAME		0x1100	// 点名结果，由服务器发送给客户端

///////////////////////////////////////////////////////////////////////////
// 定义炮点Break Point属性
#define SHOTPOINT_BREAK_NO		0
#define SHOTPOINT_BREAK_YES		1
// 定义Break Point选择字符串
#define SHOTPOINT_STRING_NO		_T("NO")
#define SHOTPOINT_STRING_YES	_T("YES")

//////////////////////////////////////////////////////////////////////////
// 定义爆炸机控制器类型
#define  SHOTTYPE_NULL			0x0000
#define  SHOTTYPE_EXPLO			0x0001			
#define  SHOTTYPE_VIBRO			0x0002

//////////////////////////////////////////////////////////////////////////
// 排列类型
#define  SPREADTYPE_ABSOLUTE	2		// 绝对排列
#define  SPREADTYPE_GENERIC		1		// 相对排列,普通排列

//////////////////////////////////////////////////////////////////////////
// 定义排列应答类型，定义的参数大小顺序不能够颠倒，在IsAllAck函数中应用该属性
#define	 SHOTSPREAD_ACKED_NULL			0		// 无应答，该参数值只能是0或者第一个，不能够修改
#define	 SHOTSPREAD_ACKED_CONFIGCMD		1		// 放炮前发送硬件配置命令时应答：广播端口、滤波器等
#define	 SHOTSPREAD_ACKED_SAMPLECMD		2		// 放炮前发送采样配置命令时应答：TB、TE
// 以下定义在施工程序中用到基础数据结构
////////////////////////////////////////////////////////////////////////////
#define VP_TODO			0
#define VP_READY		1
#define VP_DOING		2
#define VP_SUSPENDED	3
#define VP_DONE			4
//////////////////////////////////////////////////////////////////////////
// 定义震源状态
#define SHOTSOURCE_NEXT			1				// 要用此震源进行的下一次放炮
#define SHOTSOURCE_READY		2				// 要用此震源进行的放炮。系统已做好发送点火命令的准备。
#define SHOTSOURCE_FOCUS		3				// 此震源有组合中心，且采集正在进行之中。
#define SHOTSOURCE_UNUSED		0				// 只有当您手工分配给它一个VP 后才使用的震源。
///////////////////////////////////////////////////////////////////////////
// 定义可控震源施工
#define VIBROMOVING_SEQUENTIAL  0
#define VIBROMOVING_RANDOMLY    1
// 定义选择字符串
#define VIBROMOVING_STRING_SEQ  _T("SEQUIENTIAL")
#define VIBROMOVING_STRING_RND  _T("RANDOM")

//////////////////////////////////////////////////////////////////////////
// 定义采集道的类型,辅助道的定义“9”与SEGD协议一致，在SEGD中其他采集道均为1
#define CHN_NORMAL		0				// 正常采集道
#define CHN_MUTE		1				// 哑道
#define CHN_AUXI		9				// 辅助道

////////////////////////////////////////////////////////////////////////////
// 数据处理对象类型
#define PROCESS_IMPULSIVE		0		// 脉冲
#define PROCESS_STACKIMPULSIVE	1		// 脉冲叠加
#define PROCESS_CORRELBEFORE	2		// 叠加前相关
#define PROCESS_CORRELAFTER		3		// 叠加后相关
#define PROCESS_STACK			4		// 可控震源地震叠加
//////////////////////////////////////////////////////////////////////////
// 处理设置选项
#define PROCESS_STANDARD		0		// 标准
#define PROCESS_ADVANCE			1		// 高级

/*
// 定义放炮表中每一炮的状态
enum    VPSTATE    : unsigned char{ VP_ToDO =0, VP_Ready, VP_Doing, VP_Suspended , VP_Done};
//////////////////////////////////////////////////////////////////////////
// 定义施工模式
enum   OperatingMode: unsigned char{ Mode_Standard,  Mode_SlipSweep, Mode_SQCDump, Mode_MicroSeismic,Mode_Guidance};
////////////////////////////////////////////////////////////////////////////
// 施工过程控制方式
enum   OperatingAuto: unsigned char{ Continuous, Discontinuous, Manual  };
///////////////////////////////////////////////////////////////////////////
// 定义可控震源施工
enum  VibroMoving: unsigned char { Sequential,  Randomly };
//////////////////////////////////////////////////////////////////////////
// 定义震源状态
enum  SourceState: unsigned char{ SHOTSOURCE_NEXT, SHOTSOURCE_READY, SHOTSOURCE_FOCUS, SHOTSOURCE_UNUSED};
*/
///////////////////////////////////////////////////////////////////////////
// 处理设置
//enum  ProcessSetup: unsigned char{ Process_Standard,  Process_Advance  };
//enum  ProcessType : unsigned char{ Process_IMPULSIVE, Process_STACKIMPULSIVE, Process_CORRELBEFORE, Process_CORRELAFTER, Process_STACK };

///////////////////////////////////////////////////////////////////////////
/**
 * @class CShotPoint
 * @brief 炮点对象
 *
 * 定义一个炮点对象属性，是放炮表中一条记录的数据结构。
 */
class CShotPoint
{
public:
	CShotPoint();
	~CShotPoint(void);
public:
	/** 炮点状态 */
	unsigned char  m_VPState;
	/** 该炮完成后是否暂停 */ 
	unsigned char  m_byBreakPoint;		 
	/** 炮号 */
	unsigned long  m_dwShotNb;			 
	/** 放炮的地震测线组的身份号 */
	unsigned long  m_dwSwathNb;
	/** 放炮次数 */
	unsigned long  m_dwSourcePointIndex;
	/** 震源点测线号，可在测线 L 和测线 L+1 之间以 0.1 步幅调节 */
	float          m_fSourceLine;        
	/** 震源点接收道位置号，可在接收道位置 RP 和 RP+1 之间以步幅 0.1 调节 */
	float          m_fSourceNb;          
	/** 采集排列的第一条测线 */
	unsigned long  m_dwSpreadSFL;
	/** 排列第一个接收检波器位置号码 */
	unsigned long  m_dwSpreadSFN;
	/** 采集排列编号 */
	unsigned long  m_dwSpreadNb;		 
	/** 超级排列编号 */
	unsigned long  m_dwSuperSpread;	     
	/** 处理类型编号 */
	unsigned long  m_dwProcessNb;
	/** 注释，写入SEGD文件 */
	CString        m_szComments;
};

///////////////////////////////////////////////////////////////////////////
/**
 * @class CShotSource
 * @brief 震源对象
 *
 * 定义震源对象属性。
 */
class CShotSource
{
public:
	CShotSource(void);
	virtual ~CShotSource(void);
	/** 爆炸机控制器类型：Explo or Vibro */
	unsigned long m_dwSourceType;
	/** 震源状态 */
	unsigned char m_bySourceState;	
	/** 震源编号 */
	DWORD m_dwSourceNb;
	/** 震源标签 */
	CString m_strLabel;
	/** 震源控制器身份号 */
	DWORD m_dwShooterNb;				
	/** 震源描述 */
	CString m_strComment;
	/** 完成一次放炮后要使用的递增步幅，可以为负值 */
	long m_lStep;
	/** 震源放炮次数 */
	unsigned long  m_dwSourceIndex;
	/** 下一个炮号VP Nb */
	unsigned long  m_dwReadyVPNb;
	/** 是否准备好 */
	BOOL	   m_bReady;
};

///////////////////////////////////////////////////////////////////////////
/**
 * @class CShotSourceExplo
 * @brief 脉冲震源对象
 *
 * 定义脉冲震源对象属性。其中基类的m_lStep 参数对应于 IncrNB属性值。
 */
class CShotSourceExplo :  public  CShotSource
{
public:
	CShotSourceExplo(void);
	~CShotSourceExplo(void);	
};

///////////////////////////////////////////////////////////////////////////
/**
 * @class CShotSourceVibro
 * @brief 可控震源对象
 *
 * 定义可控震源对象属性。ShooterNb对应于FleetNb
 */
class CShotSourceVibro :  public  CShotSource
{
public:
	CShotSourceVibro(void);
	~CShotSourceVibro(void);	
	/** 震源移动方式 */
	unsigned char	m_byMoving; 
	/** 是否需要依据采样来移动震源 */
	BOOL			m_bWorkByAcq; 
	/** 震源分组，用于多组同时爆炸*/
	DWORD			m_dwClusterNb;
};
////////////////////////////////////////////////////////////////////////////////////////
/**
 * @class CAuxChannel
 * @brief 辅助道数据结构
 *
 * 描述一个辅助道的数据结构。
 */
class CAuxChannel
{
public:
	CAuxChannel(void){};
	~CAuxChannel(void);

public: //属性
	/** 辅助道Nb */
	unsigned int	m_dwNb;
	/** 辅助道说明  */
	CString			m_strLabel;
	/** 辅助道所用设备的类型 */
	unsigned int	m_dwBoxType;
	/** 辅助道所用设备的序列号 */
	unsigned int	m_dwSN;
	/** 辅助道所占地震道道号 */
	unsigned int	m_dwChannelNb;
	/** 辅助道放大增益 */
	unsigned int	m_dwGain;
	/** DPG机箱 */
	unsigned int	m_dwDPGNb;
	/** 辅助道备注 */
	CString			m_strComments;
	/** 辅助道IP地址 */
	unsigned int	m_dwIP;	
};
////////////////////////////////////////////////////////////////////////////////////////
/**
 * @class CAuxChannels
 * @brief 辅助道数组
 *
 * 利用动态数组管理所有的辅助道对象。
 */
class CAuxChannels
{
public:
	CAuxChannels(void){};
	~CAuxChannels(void);
public:
	CArray<CAuxChannel*,CAuxChannel*>	m_arrAuxChannel;
public:
	/** 删除所有辅助道对象*/
	void   RemoveAll(void);
	/** 计算辅助道对象总数*/
	INT_PTR GetCount(void)	{ return m_arrAuxChannel.GetCount(); };
	/** 增加辅助道对象*/
	INT_PTR Add(CAuxChannel* pAuxChannel){ return m_arrAuxChannel.Add(pAuxChannel); };	
	/** 通过索引获得辅助道对象，索引从0开始 */
	CAuxChannel* GetAuxChannel(int iIndex);
	/** 通过辅助道Nb编号获得辅助道对象 */
	CAuxChannel* GetAuxChannelByNb(DWORD dwNb);
};

///////////////////////////////////////////////////////////////////////////
/**
 * @class tagSHOTSPREAD
 * @brief 采集道数据结构
 *
 * @note描述一个采集道的数据结构，如采集道线号、点号、道号、IP地址、增益、类型等。
 * @note 对于哑道处理：不向哑道发送点名信息，直接将哑道的应答标志m_lAckedType置为TRUE；
 在每次向采集排列检查是否需要重发数据时，将哑道对应的缓冲区清零，置接收数据标志. 
 * @note 哑道由施工客户程序从测线程序生成的测网二进制属性文件中读取，并下发给服务器；
 辅助道由服务器程序从测线的XML配置文件中直接读取，在接受施工客户端发送采集排列标签帧时
 写入到采集排列数组的首部。
 * @note 对于辅助道而言，线号保存辅助道的编号，点号保存SN，道号仍然保存道号
 */
typedef struct tagSHOTSPREAD
{
	tagSHOTSPREAD(){
		m_dwLineNb =0;
		m_dwPointNb = 0;	
		m_dwChannelNb=0;
		m_dwIP = 0;
		m_dwGain = 1;
		m_lAckedType =0;
		m_byType = CHN_NORMAL;
		
	}
	/** 测线号; 对于辅助道而言，线号保存辅助道的编号*/
	DWORD			m_dwLineNb;
	/** 测点号; 对于辅助道而言，点号保存SN*/
	DWORD			m_dwPointNb;
	/** 测道号; 对于辅助道而言，道号仍然保存道号*/
	DWORD			m_dwChannelNb;
	/** 采集道IP地址 */
	DWORD			m_dwIP;	
	/** 采集道增益,取值：1,2,4,8,16,32,64;其中1对应Sercel的g1设置,4对应为g2; 写SEGD时也要按照增益分为不同扫描类型头段*/
	DWORD			m_dwGain;	
	/** 点名是否有应答：根据发送的命令类型不同，应答值不同 ，为0表示没有应答*/
	long			m_lAckedType;
	/** 采集道类型: 采集道、哑道、辅助道 */
	unsigned char	m_byType;

}SHOTSPREAD; 
///////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @class CShotSpread
 * @brief 采集排列数据结构
 *
 * 描述一个采集排列的数据结构，保存多个采集道结构。
 */
class CShotSpread
{
public:
	CShotSpread(void);
	virtual ~CShotSpread(void);

	/** 采集排列编号 */
	unsigned long					m_dwNb;
	/** 采集排列标签 */
	CString							m_strLabel;
	/** 采集排列类型:2=绝对排列、1=普通排列 */
	unsigned char					m_bySpreadType;
	/** 采集排列数组 */
	CArray<SHOTSPREAD,SHOTSPREAD&>	m_arrSpread;
private:
	/** 辅助道个数，辅助道位于m_arrSpread数组的前端，由客户端发送到施工服务器 */
	DWORD							m_dwAuxiCount;
public:	
	// 设置辅助道总数
	void  SetAuxiCount(DWORD dwCount){ m_dwAuxiCount = dwCount; };
	// 得到辅助道总数
	DWORD GetAuxiCount(void)         { return m_dwAuxiCount;    };
	// 得到采集排列的总数
	INT_PTR GetCount(void)			{ return m_arrSpread.GetCount();};
	// 得到辅助道总数
	// unsigned long GetAuxiChannelCount(void);
	// 查找指定IP地址的采集站，返回在数组中索引号
	int Find(unsigned long dwIP);
	//int Append(unsigned long dwIP, DWORD dwGain);
	// 添加一个采集站，包括IP地址和增益	
	int Append(SHOTSPREAD&  ShotSpread);
	// 检查应答标志，判断是否全部有应答
	bool IsAllAcked(long bAckedValue);
	// 检查应答标志，计算出有应答的采集站个数(不含辅助道)
	unsigned int TotalOfAcked(void);
	// 设置对应IP地址的FDU应答标志
	bool SetAck(DWORD dwIP,long lAckedType);
	// 释放所有的采集道对象
	void ReleaseAll(void);
};

///////////////////////////////////////////////////////////////////////////
/**
 * @class tagAuxiChannel
 * @brief 辅助道数据处理结构
 *
 * 描述一个数据处理类型中对辅助道的处理算法。
 */
typedef struct tagAuxiChannel
{
	/** 辅助道处理描述的编号 */
	DWORD    m_dwNb;
	/** 辅助道处理方法描述 */
	TCHAR	 m_szProcessing[128];
}AUXICHANNEL;
///////////////////////////////////////////////////////////////////////////
/**
 * @class CProcessType
 * @brief 数据处理基类
 *
 * 描述一个数据处理类型属性，是所有数据处理类型的基类。
 */
class CProcessType
{
public:
	CProcessType(void);
	virtual ~CProcessType(void);
public:
	
	/** 处理类型 */
	BYTE			m_byProcessType;
	/** 数据处理类型编号 */
	DWORD			m_dwProcessNb;
	/** 数据类型说明 */
	CString		    m_strLabel;
	
	/** 处理设置：标准或者高级 */
	BYTE			m_ProcessSetup;
	/** 爆炸机控制器所连采集站SN号 */
	DWORD			m_dwFDUSN;
	/** 数据记录长度，单位ms，默认值为1000ms */
	DWORD		    m_dwRecordLen;
	/** 数据记录长度，单位ms，默认值为1000ms；对应可控震源是listening time参数 */
	DWORD		    m_dwTBWindow;
	/** 辅助道记录 */
	CArray<AUXICHANNEL,AUXICHANNEL&>	m_arrAuxiChannel;

public:
	// 返回记录长度
	// DWORD GetRecordLength()	{return m_dwRecordLen;};

	virtual DWORD GetObjectByteSize();
	virtual int  SaveToBuf(unsigned char* pBuf);
	virtual int  ReadFromBuf(unsigned char* pBuf);
	//virtual bool Copy(CProcessType* pProcessSrc);
};
///////////////////////////////////////////////////////////////////////////
/**
 * @class CProcessImpulsive
 * @brief 脉冲数据处理类型
 *
 * 描述一个脉冲数据处理类型属性。
 */
class CProcessImpulsive :
	public CProcessType
{
public:
	CProcessImpulsive(void);
	~CProcessImpulsive(void);
public:
	/** 折射延时，默认为0ms */
	DWORD			m_dwRefractionDelay;	
public:
	// 返回数据采集记录的时间长度
	// DWORD GetRecordLength()	{return m_dwRecordLen;};
	// 计算对象属性总的字节大小
	virtual DWORD GetObjectByteSize();
	// 将对象保存到缓冲区，用于客户端与服务器之间数据交换
	virtual int  SaveToBuf(unsigned char* pBuf);
	// 从缓冲区中读出对象，用于客户端与服务器之间数据交换
	virtual int  ReadFromBuf(unsigned char* pBuf);

};

///////////////////////////////////////////////////////////////////////////
/**
 * @class CShotClient
 * @brief  放炮施工客户端管理对象
 *
 * 描述一次放炮任务的属性，如存储施工客户端IP地址、端口；需要放炮的炮号、震源、处理等。
 */
class CShotClient
{
public:
	CShotClient(void);
	~CShotClient(void);

public:
	/** 客户端IP地址 */
	DWORD	m_dwClientIP;
	/** 客户端端口 */
	WORD	m_wClientPort;

	/** 爆炸机控制器 */
//	CShotController*  m_pShotCtrl;
	/** 震源 */
	CShotSource*	  m_pShotSource;
	/** 炮点 */
	CShotPoint*		  m_pShotPoint;
	/** 采集排列 */
	CShotSpread*	  m_pSpread;
	/** 处理类型 */
	CProcessType*	  m_pProcessType;
	/** 采集站配置信息 */
	CFDUConfig*		  m_pFDUConfig;
	/** 正常采集数据存盘路径，对应于CDiskRecordConfig的变量 */
	CString			  m_strNormalFilePath;
	/** 测试数据存盘路径，对应于CDiskRecordConfig的变量 */
	CString			  m_strTestFilePath;
	/** 采集数据文件编号，对应SEGD格式中文件编号 */
	DWORD			  m_dwFileNumber;
	/** 采集数据文件名 */
	CString			  m_strFileName;
	/** 处理的SEGD文件名*/
	CString			  m_strSEGDFileName;
};

