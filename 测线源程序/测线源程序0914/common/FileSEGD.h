#pragma once

#ifdef PROGRAM_OPERATIONSERVER
#include "OperaStruct.h"
#include "FDUConfig.h"
#include "..\operationserver\OperaThread.h"
#include "ChannelList.h"
// class COperaThread;
#endif

/**
 * @class CSEGDScanTypeHeader
 * @brief 扫描类型头段数据
 *
 * SEGD格式的扫描类型头段数据结构。
 */
class CSEGDScanTypeHeader
{
public:
	CSEGDScanTypeHeader(void);
	~CSEGDScanTypeHeader(void);
	/** 该组通道的组号*/
	BYTE	m_byChnSetNb;
	/** 该组通道的道数*/ 
	WORD	m_wChnCount;
	/** 通道类型: 1为地震道(含哑道），9为辅助道;*/
	BYTE	m_byChnType;
	/** 通道组开始时间，对于辅助道而言为0,；采集道等于折射延迟时间。
	    2ms为增量，因此取值应该是实际开始时间的一半*/
	WORD	m_wChnStartingTime;
	/* 通道组结束时间，2ms为增量，因此取值应该是通道开始时间加上采样长度的一半。*/
	WORD	m_wChnEndTime;
	/** 仪器类型：0x00未确定；0x01为FDU；0x1C表示DSU428；0x20，VE464*/
	BYTE	m_byUnitType;
	/** 放大增益,取值：1,2,4,8,16,32,64,其中1对应Sercel的g1设置,4对应为g2*/
	DWORD	m_dwGain;
	/** 垂直叠加(1字节，无符号二进制数)。默认为0。有效的叠加次数，
	    如果故意将道数据置为0，则垂直叠加置为0。如果未叠加，置为1。如
		果数据是叠加的结果(处理或者未处理)，置为有效叠加次数*/
	BYTE	m_byVerticalStack;
public:
	/// 得到缩小比例系数,Descale Multiplier，位于扫描类型头段的7、8字节
	WORD	GetDescaleMultiplier();
	/// 3dB 点处的混叠滤波器频率; Alias filter frequency at - 3dB point
	WORD    GetAliasFilterfrequency(DWORD dwSampleRateUS);
	/// 混叠滤波器斜率;Alias filter slope
	WORD    GetAliasFilterSlope()			{ return 370;};
	/// 低截滤波器频率; Low-cut filter frequency
	WORD    GetLowcutFilterFrequency()		{ return 0;};
	/// 低截滤波器斜率,Low-cut filter slope
	WORD    GetLowcutFilterSlope()			{ return 0;};
};
/**
 * @class CSEGDExtendedHeader
 * @brief 扩展头段数据结构
 *
 * SEGD格式的扩展头段数据结构。
 */
class CSEGDExtendedHeader
{
public:
	CSEGDExtendedHeader();
	~CSEGDExtendedHeader();
	// 属性
public:
	
	/** 记录道总数*/
	DWORD	m_dwTraceCount;
	/** 辅助道总数*/
	DWORD	m_dwAuxesTraceCount;
	/** 无应答地址道总数*/
	// DWORD	m_dwDeadSeisTraceCount;
	/** 有应答地址道总数*/
	DWORD	m_dwLiveSeisTraceCount;

	/**震源类型:0为无震源；1为脉冲，2为可控*/
	DWORD   m_dwSourceType;
	/** 炮号*/
	DWORD	m_dwShotNb;
	/** TB窗口时间,秒为单位*/
	float   m_fTBWindowS;
	/** 采集排列的第一条测线 */
	DWORD	m_dwSpreadSFL;
	/** 排列第一个接收检波器位置号码 */
	DWORD	m_dwSpreadSFN;
	/** 采集排列编号 */
	DWORD	m_dwSpreadNb;
	/** 采集排列类型: 1为普通排列，2为绝对排列 */
	DWORD   m_dwSpreadType; 
	/** 测试记录类型Test record type:
	0 正常记录。1 野外噪声。2 野外脉冲。3 野外串音。4 仪器噪声。
	5 仪器畸变。6 仪器增益/ 相位。	7 仪器串音。	8 仪器共模。
	9 合成。	10 一致性（仅限 CM408）。11 仪器脉冲。12 野外畸变。
	13 仪器重力。14 野外漏电。15 野外电阻。*/
	DWORD	m_dwTestType;
	
	/** 时断信号Timebreak，微秒单位 */
	DWORD	m_dwConfirmationTBUS;
	/** 井口时间,微秒为单位*/
	DWORD	m_dwUpholetimeUS;
	/** 爆炸机标识Blaster id */
	DWORD	m_dwBlasterID;
	/** 爆炸机状态 */
	DWORD	m_dwBlasterStatus;
	

	/** 折射延时，毫秒ms*/
	DWORD	m_dwRefractionDelayMS;
	/** TB到T0时间，确认的TB时间减去折射时间,单位微秒*/
	long	m_lTBToTStartUS;
	/** 内部时断信号，ITB 指示; 0为是；1为否*/
	DWORD	m_dwInternalTimeBreak;
	
	
	/** 野外设备中预叠加，0为是，1为否；Prestack within field units*/
	DWORD	m_dwPrestack;
	/**噪声抑制类型: 1 无;2花样叠加;3 历史;4 关闭*/
	DWORD	m_dwNoiseEliminationType;
	/** 低记录道百分比，0~100%; Low trace percentage*/
	DWORD	m_dwLowTracePercentage;
	/** 低记录道值;0-132db; Low trace value */
	DWORD	m_dwLowTraceValue;
	/** 窗口数目1-64; Number of windows (Div.)or Window length (Enhanced Div.) */
	DWORD	m_dwWindowLength;
	/** 历史编辑类型：1为归零；2为剪裁；*/
	DWORD	m_dwHistoricEditingType;
	/** 噪声记录道百分比：0 到100% ； Noisy trace percentage*/
	DWORD	m_dwNoisyTracePercentage;
	/** 历史范围0 到 36 dB;  Historic range */
	DWORD   m_dwHistoricRange;
	/** 历史锥形长度 2 的指数:0 到 8;Historic taper length 2’s	exponent*/
	DWORD   m_dwExpHistoricTaperLen;
	/** 门槛值不变/ 变化1 = 不变;2 = 变化; Threshold Hold/Var*/
	DWORD   m_dwThresholdHold;
	/**历史门槛值初始值0 到 132 dB; Historic threshold Init value*/
	DWORD	m_dwHistoricThresholdValue;
	/**历史归零长度1 到 500 毫秒; Historic zeroing length*/
	DWORD	m_dwHistoricZeroingLen;
	
	
	/**处理类型1 无操作（原始数据）;2 叠加;	3 叠加后相关;4 叠加前相关;5 原始数据，叠加;	6 原始数据，叠加后相关;7 原始数据，叠加前相关*/
	DWORD   m_dwProcessType;
	/** 采集类型表,32个数值，128字节*/
	DWORD	m_dwAcquisitionTypeTable[32];
	/**门槛值类型表32 个数值（128 字节）,Threshold type tables 32 values (128 bytes).*/
	DWORD   m_dwThresholdTypeTable[32];
	/**叠加次数,1~32; Stacking fold*/
	DWORD   m_dwStackingFold;
	/** 记录长度100 到 128000 毫秒;Record length*/
	DWORD	m_dwRecordLength;
	/** 自相关峰值时间1 到 128000 毫秒; Autocorrelation peak time */
	DWORD	m_dwAutoCorrPeakTime;
	/** 相关参考信号编号1 到 100000;Correlation Pilot No. 1 to 100000.*/
	DWORD	m_dwCorrelationPilot;
	/** 参考信号长度1000 到 128000 毫秒 ; Pilot length*/
	DWORD	m_dwPilotLength;
	/** 扫描长度1000 到 128000 毫秒; Sweep length*/
	DWORD   m_dwSweepLength;
	/** 采集号1 到 32; Acquisition number */
	DWORD	m_dwAcquisitionNb;

	/** 最多辅助道的最大值;Max of max, Aux*/
	float	m_fMaxAux;
	/** 最多地震道最大值IEEE 格式，单精度;Max of max, Seis*/
	float	m_fMaxSeis;
	/** 转储叠加次数1..32;Dump stacking fold*/
	DWORD	m_dwDumpStackingFold;
	/** 磁带标签ASCII 文本，16 个字符*/
	BYTE	m_byTapeLabel[16];
	/** 磁带号1 到 9999 */
	DWORD	m_dwTapeNb;
	/** 软件版本ASCII 文本，16 个字符; Software version ASCII text, 16 characters.*/
	BYTE	m_bySoftwareVersion[16];
	/** 日期ASCII 文本，12 个字符(dd mmm yyyy)*/
	BYTE    m_byDate[12];
	/** 震源偏东距; Source easting*/
	double	m_dbSourceEasting;
	/** 震源北向纬度差; Source northing*/
	double  m_dbSourceNorthing;
	/** 震源高程; Source elevation*/
	float   m_fSourceElevation;

	/** 所用的滑动扫描模式： 0为否;1为是 */
	DWORD   m_dwSlipSweepMode;
	/** 每个磁带的文件数1 到 9999*/
	DWORD   m_dwFilesPerTape;
	/** 文件计数1 到 9999*/
	DWORD   m_dwFileCount;
	/** 采集错误说明ASCII 文本，160 个字符*/
	BYTE	m_byAcquisitErrorDesc[160];
	/** 滤波器类型: 1 =0.8最小; 2= 0.8 线性; */
	DWORD   m_dwFilterType;
	/** 叠加已转储: 	0 = No; 1 = Yes.;*/
	DWORD   m_dwStackIsDumped;
	/** 叠加符号（当前） 0 = 否;1 = 正;2 = 负*/
	DWORD   m_dwStackSign;
	/** 所用的 PRM 倾斜度校正0 = 否;1为是; PRM Tilt Correction used*/
	DWORD	m_dwPRMTiltCorrect;
	/** 线束名;ASCII 文本，64个字符; */
	BYTE	m_bySwathName[64];

	/**施工模式bit0 = 1 Standard.bit1 = 1 Microseismic.bit2 = 1 Slip-sweep.	bit3 = 1 SQC Dump (VSR).bit4 = 1 Guidance(Navigation).*/
	DWORD	m_dwOperatingMode;
	/** 无记录0 = 否;1 = 是*/
	DWORD	m_dwNoLog;
	/** 监听时间100 到 128000毫秒; Listening time 100 to 128000 ms.*/
	DWORD	m_dwListeningTime;
	/** 转储类型0 = 正常转储;1 = 原始转储;2 = 附加转储;*/
	DWORD   m_dwDumpType;
	/** 线束 Id; Swath ID*/
	DWORD	m_dwSwathId;
	
	/** Seismic trace offset removal is disabled; 0 = No (i.e.offset is removed);1 = Yes (Microseismic mode); Sercel 4.0版本增加*/
	DWORD   m_dwSeismicTaceOffsetRemoval;

	/**GPS time of acquisition Time Break, expressed as the	number of microseconds
	since January 6, 1980 at 0:00 a.m. UTC (signed integer).*/

};
/**
 * @class CSEGDTraceData
 * @brief 读写SEGD文件
 *
 * 读写SEGD格式的文件。
 */
class CSEGDTraceHeads
{
public:
	CSEGDTraceHeads();
	~CSEGDTraceHeads();
public:
	/** 地震道组号*/
	BYTE		m_byChnSetNb;
	/** 记录道编号*/
	DWORD		m_dwTraceNb;
	/** 记录道编辑;00:未进行编辑。02:采集前静噪或停滞。03: 由采集系统编辑*/
	BYTE		m_byTraceEdit;

	/** 接收点测线号*/
	DWORD		m_dwLineNb;
	/** 接收点号*/
	DWORD		m_dwPointNb;
	/** 接收点索引*/
	BYTE		m_byPointIndex;
	/** 采样样本数，与扩展字段相同*/
	//DWORD dwSampleCount;
	/** 检波器SEGD代码,在Line程序中设置*/
	BYTE m_bySensorSEGDCode;

	/** 接收点偏东距*/
	double		m_dbPointEasting;
	/** 接收点北向纬度*/
	double		m_dbPointNorthing;
	/** 接收点高程*/
	float		m_fPointElevation;
	/** 传感器类型编号,取值1-9*/
	BYTE		m_bySensorType;

	/** 电阻值上限*/
	float		m_fResistanceMax;
	/** 电阻值上限*/
	float		m_fResistanceMin;
	/** 电阻值*/
	float		m_fResistanceVal;
	/** 最大陆上检波器倾斜度百分比*/
	float		m_fTiltMax;
	/** 倾斜度值*/
	float		m_fTiltVal;
	
	/** 电容上限*/
	float		m_fCapacitanceMax;
	/** 电容下限*/
	float		m_fCapacitanceMin;
	/** 电容值，纳法拉*/
	float		m_fCapacitanceVal;
	/** 截止上限*/
	float		m_fCutoffFreqMax;
	/** 截止下限*/
	float		m_fCutoffFreqMin;
	/** 截止值*/
	float		m_fCutoffFreqVal;
	/** 漏电界限*/
	float		m_fLeakageLimit;
	/** 漏电值*/
	float		m_fLeakageVal;
	
	/**设备类型, 仪器类型：1为FDU；0x1C表示DSU428 */
	// BYTE	m_byUnitType;由扫描类型头段的变量说明
	/** 设备序列号*/
	DWORD		m_dwUnitSN;
	/** 地震道编号*/
	BYTE		m_byChannelNb;
	/** 组件类型*/
	BYTE		m_byAssemblyType;
	/** FDU 或 DSU 组件序列号*/
	DWORD		m_dwFDUAssemblySN;
	/**在 FDU 或 DSU组件中的位置 */
	BYTE		m_byLocationInFDUAssembly;
	/** 子单元类型*/
	BYTE		m_bySubunitType;
	/** 地震道类型:0 检波器;1 压敏检波器*/
	BYTE		m_byChannelType;
	/** 检波器灵敏度*/

	/** 控制单元类型:0x01 LCI408;0x02 LAUL408;0x03 LAUX408;0x04 LAULS408;0x05 LAUXS408;0x06 LAUL428;0x16 LRU;0x17 LAUR428;0x30 LAUX428;0x31 LCI428*/
	BYTE		m_byControlUnitType;
	/** 控制单元序列号*/
	DWORD		m_dwControlUnitSN;
	/** 地震道增益刻度*/
	BYTE		m_byGainScale;
	/** 地震道滤波器*/
	BYTE		m_byChannelFilter;
	/** 地震道数据误差*/
	BYTE		m_byChannelDataError;
	/** 地震道编辑状态,1 停滞;2 采集/ 回传错误;3 噪声编辑*/
	BYTE		m_byChannelEditedStatus;
	/** 地震道采样毫伏转换系数,辅助道为0*/
	float		m_fChannelConversionFactor;
	/** 有噪声叠加次数*/
	BYTE		m_byNumberOfStacksNoisy;
	/** 低叠加次数*/
	BYTE		m_byNumberOfStacksLow;
	/** 地震道类型标识号：1 = 地震道;9 = 辅助道;在扫描类型头段对象中*/
	
	/** 地震道处理:01=原始数据;	02=辅助道叠加;03=相关，负部分;04=相关，正部分;05=正态相关;06=地震道叠加*/
	BYTE		m_byChannelProcessType;
	/** 记录道最大值*/
	float		m_fTraceMaxValue;
	/** 记录道最大时间,微秒*/
	DWORD		m_dwTraceMaxtime;
	/** 内插次数*/
	DWORD		m_dwInterpolations;
	/**Seismic trace offset value(if offset removal is disabled).*/
	DWORD		m_dwSeismicTraceOffsetValue;
};
 /**
 * @class CSEGDFile
 * @brief 读写SEGD文件
 *
 * 读写SEGD格式的文件。
 */
class CSEGDFile
{
public:
	CSEGDFile(void);
	~CSEGDFile(void);
// 公有属性
public:
	/** 文件编号 */
	DWORD			m_dwFileNumber;	
	/** 制造商代码，默认13*/
	BYTE			m_byManufacturerCode;
	/** 制造商序列号，默认0*/
	WORD			m_wManufacturerSN;
	/** 每个记录的地震道组数量。默认16,修改后对应增加CSEGDScanTypeHeader数组个数*/
	BYTE			m_byChnSetsCount;
	/** 外部头段大小，在Config程序配置中读取*/
	WORD			m_wExternalHeaderSize;

	/** 基本扫描间隔，采样率，以微秒为单位，一般取值：250,500,1000,2000,4000 */
	DWORD			m_dwSampleRateUS;
	/** 采样长度，以ms为单位，取值0-128000ms */
	DWORD			m_dwRecordLenMS;
	/** 采样类型,取值：8为正常采样；2为测试采样 */
	BYTE			m_byRecordType;
	/** 震源测线号，小数点后两位 */
	double			m_fSourceLine;
	/** 震源点号，小数点后两位 */
	double			m_fSourceNb;
	/** 震源点索引 */
	DWORD			m_dwSourcePointIndex;
	/** 震源组号 */
	DWORD			m_dwFleetNb;
	
	/** 16个扫描类型头段 */
	// CSEGDScanTypeHeader	m_ScanTypeHeader[16];
	
	/** 扩展头段数据 */
	CSEGDExtendedHeader	m_ExtendedHeader;

	/** 放炮或者导航系统信息 */
	CString			m_strShooterInfo;
	/** 来自Config程序设置的用户信息*/
	CString			m_strConfigUserInfo;
	/** 施工窗口的震源注释*/
	CString         m_strSourceComment;
	/***/
	/***/
	/***/
	/***/
	/***/
// 私有属性
private:

	union DATATYPE
	{
		unsigned char   byData[4];
		unsigned long   dwData;
		         long   lData;
				 float  fData;

	}m_unionType;
	/** 数字码转化为BCD码的索引表,包括0,1,2,3,4,5,6,7,8,9 */
	unsigned char m_byDecToBCD[10];
	
	/** SEGD文件指针 */
	CFile*	m_pFile;
// 私有函数
private:

	// 计算儒略日
	WORD CalcJulianDay(SYSTEMTIME* pSysTime);
	/// 删除文件指针
	bool CloseSEGDFile(void);
	/// 初始化ASCII到BCD码映射表
	void    InitASCIIToBCD(void);
	/// 将字节反序，最低位变成最高位，依次转换
	unsigned char   ReverseChar(unsigned char   ucRegchar);
	/// 将两个字节的十进制数据转化为BCD码格式
	void  ConvertWORDToBCD(WORD wData,unsigned char* byOut);
	/// 将1个字节的十进制数据转化为BCD码格式
	BYTE  ConvertBYTEToBCD(BYTE byData);
	// 将小于0的小数转化为两个字节的整数形式
	void  ConvertLessOneFloatToWORD(double fValue,unsigned char* byOut);
	// 将小于0的小数转化为1个字节的整数形式
	BYTE  ConvertLessOneFloatToBYTE(double fValue);
	/// 基本扫描间隔
	BYTE  GetBaseScanInterval();

	// 颠倒浮点数高低位，高位在前，低位在后
	void ReverseFloatToBigEndian(float fValue,BYTE* byData);
	// 颠倒整数高低位，高位在前，低位在后
	void ReverseDWORDToBigEndian(DWORD dwValue,BYTE* byData);
	// 将三个字节的整数转化为四个字节的浮点数格式，并高低字节互换
	void Convert3ByteIntTo4ByteAntiFlt(BYTE* lpInData,BYTE* lpOutData);
	
	//////////////////////////////////////////////////////////////////////////
	/// 计算采样样本
	DWORD	GetNumberOfSamples() { return m_dwRecordLenMS*1000/m_dwSampleRateUS+1;};
// 公有函数
public:
	/// 写总头块1
	void WriteGeneralHeader1();
	/// 写总头块2
	void WriteGeneralHeader2();
	/// 写总头块3
	void WriteGeneralHeader3();
	// 写扫描类型头段，
	void WriteScanTypeHeader(CSEGDScanTypeHeader* pScanTypeHeader);
	// 写扩展头段数据
	void WriteExtendedHeader();
	// 写外部头段
	void WriteExternalHeader();

	// 写道头数据
	void WriteDemuxTraceHeader(CSEGDTraceHeads*	pTrace);
	// 写道头扩展1
	void WriteTraceHeaderExtension1(CSEGDTraceHeads*	pTrace);
	// 写道头扩展2
	void WriteTraceHeaderExtension2(CSEGDTraceHeads*	pTrace);
	// 写道头扩展3
	void WriteTraceHeaderExtension3(CSEGDTraceHeads*	pTrace);
	// 写道头扩展4
	void WriteTraceHeaderExtension4(CSEGDTraceHeads*	pTrace);
	// 写道头扩展5
	void WriteTraceHeaderExtension5(CSEGDTraceHeads*	pTrace);
	// 写道头扩展6
	void WriteTraceHeaderExtension6(CSEGDTraceHeads*	pTrace,CSEGDScanTypeHeader* pScanTypeHeader);
	// 写道头扩展7
	void WriteTraceHeaderExtension7(CSEGDTraceHeads*	pTrace,CSEGDScanTypeHeader* pScanTypeHeader);
	
	//
// 公有函数
public:
	// 存储SEGD 的文件头部分
	void SaveSEGDHead(CSEGDScanTypeHeader* pScanTypeHeader);
	// 存储S某一道道头部分数据
	void SaveSEGDTraceHead(CSEGDTraceHeads*	pTrace,CSEGDScanTypeHeader* pScanTypeHeader);

#ifdef PROGRAM_OPERATIONSERVER
	// 写扩展头段属性
	void	SetExtendedHeader(COperaThread* pThread,CChannelList* pChannelList);
	// 计算各扫描类型中采集道在采集排列中索引
	void	GetScanTypeOfSpread(CShotSpread* pSpread,CDWordArray& arrAuxiG1Scantype,
		CDWordArray& arrAuxiG2Scantype,CDWordArray& arrSeisG1Scantype,CDWordArray& arrSeisG2Scantype);
	// 写某一道的属性
	void    WriteAuxiTraceHeader(int nChnSetNb,int nTraceIndexInSpread,CSEGDScanTypeHeader* pScanTypeHeader,COperaThread* pThread,CChannelList* pChannelList);
	// 写某一道的属性
	void    WriteSeisTraceHeader(int nChnSetNb,int nTraceIndexInSpread,CSEGDScanTypeHeader* pScanTypeHeader,COperaThread* pThread,CChannelList* pChannelList);
	// 将某一道一帧内采样数据写入SEGD文件中
	void WriteTraceDataOfAFrame(BYTE* lpInData,int nSize);
	// 写某一道数据
	void	WriteTraceData(int nTraceIndexInSpread,int nTraceCountOfSpread,CFile*	pFileData);
	// 存储SEGD Rev1.0版本的文件
	bool SaveSEGDRev21(CFile* pFileData,COperaThread* pThread,CChannelList* pChannelList);
#endif
};
