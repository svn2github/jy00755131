#include "StdAfx.h"
#include "FileSEGD.h"



/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////CSEGDScanTypeHeader//////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
CSEGDScanTypeHeader::CSEGDScanTypeHeader(void)
{	
	/** 该组通道的组号*/
	m_byChnSetNb=0;
	/** 该组通道的道数*/ 
	m_wChnCount=0;
	/** 通道类型: CHN_NORMAL=0为地震道，CHN_MUTE=1为哑道，CHN_AUXI = 2为辅助道;*/
	m_byChnType=0;
	/** 通道组开始时间，对于辅助道而言为0,；采集道等于折射延迟时间。
	    2ms为增量，因此取值应该是实际开始时间的一半*/
	m_wChnStartingTime=0;
	/* 通道组结束时间，2ms为增量，因此取值应该是通道开始时间加上采样长度的一半。*/
	m_wChnEndTime=0;
	/** 仪器类型：0x00未确定；0x01为FDU；0x1C表示DSU428；0x20，VE464*/
	m_byUnitType=1;
	/** 放大增益,取值：1,2,4,8,16,32,64,其中1对应Sercel的g1设置,4对应为g2*/
	m_dwGain=1;
	/** 垂直叠加(1字节，无符号二进制数)。默认为0。有效的叠加次数，
	    如果故意将道数据置为0，则垂直叠加置为0。如果未叠加，置为1。如
		果数据是叠加的结果(处理或者未处理)，置为有效叠加次数*/
	m_byVerticalStack=0;
}
CSEGDScanTypeHeader::~CSEGDScanTypeHeader(void)
{

}
/// 得到缩小比例系数,Descale Multiplier，位于扫描类型头段的7、8字节
WORD	CSEGDScanTypeHeader::GetDescaleMultiplier()
{
	if(m_byUnitType==2)
	{
		return 0xAF6D;
	}
	else if(m_byUnitType==1)
	{
		if(m_dwGain==1)
			return 0xAF6D;
		if(m_dwGain==4)
			return 0xB76D;
	}
	return 0x8000;
}
/// 3dB 点处的混叠滤波器频率; Alias filter frequency at - 3dB point
WORD    CSEGDScanTypeHeader::GetAliasFilterfrequency(DWORD dwSampleRateUS)
{
	switch(dwSampleRateUS)
	{
	case 250:		// 
		return 1600;
		break;
	case 500:
		return 800;
		break;
	case 1000:
		return 400;
		break;
	case 2000:
		return 200;
		break;
	case 4000:
		return 100;
		break;
	default:
		return 400;
		break;
	}	
	return 400;	
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////CSEGDExtendedHeader///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
CSEGDExtendedHeader::CSEGDExtendedHeader()
{
	/** 记录道总数*/
	m_dwTraceCount=0;
	/** 辅助道总数*/
	m_dwAuxesTraceCount=0;
	/** 无应答地址道总数*/
	//m_dwDeadSeisTraceCount=0;
	/** 有应答地址道总数*/
	m_dwLiveSeisTraceCount=0;

	/**震源类型:0为无震源；1为脉冲，2为可控*/
	m_dwSourceType=1;
	/** 炮号*/
	m_dwShotNb=0;
	/** TB窗口时间*/
	m_fTBWindowS=1.0f;
	/** 采集排列的第一条测线 */
	m_dwSpreadSFL =1;
	/** 排列第一个接收检波器位置号码 */
	m_dwSpreadSFN=0;
	/** 采集排列编号 */
	m_dwSpreadNb=0;
	/** 采集排列类型: 1为普通排列，2为绝对排列 */
	m_dwSpreadType=2; 
	/** 测试记录类型Test record type:
	0 正常记录。1 野外噪声。2 野外脉冲。3 野外串音。4 仪器噪声。
	5 仪器畸变。6 仪器增益/ 相位。	7 仪器串音。	8 仪器共模。
	9 合成。	10 一致性（仅限 CM408）。11 仪器脉冲。12 野外畸变。
	13 仪器重力。14 野外漏电。15 野外电阻。*/
	m_dwTestType=0;
	
	/** 时断信号Timebreak，微秒单位 */
	m_dwConfirmationTBUS=0;
	/** 井口时间,微秒为单位*/
	m_dwUpholetimeUS=0;
	/** 爆炸机标识Blaster id */
	m_dwBlasterID=1;
	/** 爆炸机状态 */
	m_dwBlasterStatus=1;
	

	/**折射延时，毫秒ms*/
	m_dwRefractionDelayMS=0;
	/**TB到T0时间，确认的TB时间减去反射时间,单位微秒*/
	m_lTBToTStartUS = 0;
	/**内部时断信号，ITB 指示; 0为是；1为否*/
	m_dwInternalTimeBreak=1;
	
	
	/** 野外设备中预叠加，0为是，1为否；Prestack within field units*/
	m_dwPrestack=1;
	/**噪声抑制类型: 1 无;2花样叠加;3 历史;4 关闭*/
	m_dwNoiseEliminationType=1;
	/** 低记录道百分比，0~100%; Low trace percentage*/
	m_dwLowTracePercentage=0;
	/** 低记录道值;0-132db; Low trace value */
	m_dwLowTraceValue=0;
	/** 窗口数目1-64; Number of windows (Div.)or Window length (Enhanced Div.) */
	m_dwWindowLength=1;
	/** 历史编辑类型：1为归零；2为剪裁；*/
	m_dwHistoricEditingType=1;
	/** 噪声记录道百分比：0 到100% ； Noisy trace percentage*/
	m_dwNoisyTracePercentage=0;
	/** 历史范围0 到 36 dB;  Historic range */
	m_dwHistoricRange=0;
	/** 历史锥形长度 2 的指数:0 到 8;Historic taper length 2’s	exponent*/
	m_dwExpHistoricTaperLen=0;
	/** 门槛值不变/ 变化1 = 不变;2 = 变化; Threshold Hold/Var*/
	m_dwThresholdHold=1;
	/**历史门槛值初始值0 到 132 dB; Historic threshold Init value*/
	m_dwHistoricThresholdValue=0;
	/**历史归零长度1 到 500 毫秒; Historic zeroing length*/
	m_dwHistoricZeroingLen=1;
	
	
	/**处理类型1 无操作（原始数据）;2 叠加;	3 叠加后相关;4 叠加前相关;5 原始数据，叠加;	6 原始数据，叠加后相关;7 原始数据，叠加前相关*/
	m_dwProcessType=1;
	/** 采集类型表,32个数值，128字节*/
	// m_dwAcquisitionTypeTable[32];
	memset(m_dwAcquisitionTypeTable,0,128);
	/**门槛值类型表32 个数值（128 字节）,Threshold type tables 32 values (128 bytes).*/
	// m_dwThresholdTypeTable[32];
	memset(m_dwThresholdTypeTable,0,128);
	/**叠加次数,1~32; Stacking fold*/
	m_dwStackingFold=1;
	/** 记录长度100 到 128000 毫秒;Record length*/
	m_dwRecordLength=1000;
	/** 自相关峰值时间1 到 128000 毫秒; Autocorrelation peak time */
	m_dwAutoCorrPeakTime=1;
	/** 相关参考信号编号1 到 100000;Correlation Pilot No. 1 to 100000.*/
	m_dwCorrelationPilot=1;
	/** 参考信号长度1000 到 128000 毫秒 ; Pilot length*/
	m_dwPilotLength=1000;
	/** 扫描长度1000 到 128000 毫秒; Sweep length*/
	m_dwSweepLength=1000;
	/** 采集号1 到 32; Acquisition number */
	m_dwAcquisitionNb=1;
	/** 最多辅助道的最大值;Max of max, Aux*/
	m_fMaxAux=0;
	/** 最多地震道最大值IEEE 格式，单精度;Max of max, Seis*/
	m_fMaxSeis=0;
	/** 转储叠加次数1..32;Dump stacking fold*/
	m_dwDumpStackingFold=1;
	/** 磁带标签ASCII 文本，16 个字符*/
	// m_byTapeLabel[16];
	memset(m_byTapeLabel,0,16);
	/** 磁带号1 到 9999 */
	m_dwTapeNb=1;
	/** 软件版本ASCII 文本，16 个字符; Software version ASCII text, 16 characters.*/
	//m_bySoftwareVersion[16];
	strcpy_s((char*)m_bySoftwareVersion,16,"1.0");
	/** 日期ASCII 文本，12 个字符(dd mmm yyyy)*/
	//m_byDate[12];
	strcpy_s((char*)m_bySoftwareVersion,12,"01  01 2011");
	/** 震源偏东距; Source easting*/
	m_dbSourceEasting=0;
	/** 震源北向纬度差; Source northing*/
	m_dbSourceNorthing=0;
	/** 震源高程; Source elevation*/
	m_fSourceElevation=0;
	/** 所用的滑动扫描模式： 0为否;1为是 */
	m_dwSlipSweepMode=0;
	/** 每个磁带的文件数1 到 9999*/
	m_dwFilesPerTape=1;
	/** 文件计数1 到 9999*/
	m_dwFileCount=1;
	/** 采集错误说明ASCII 文本，160 个字符*/
	//m_byAcquisitErrorDesc[160];
	memset(m_byAcquisitErrorDesc,0,160);
	/** 滤波器类型: 1 =0.8最小; 2= 0.8 线性; */
	m_dwFilterType=1;
	/** 叠加已转储: 	0 = No; 1 = Yes.;*/
	m_dwStackIsDumped=0;
	/** 叠加符号（当前） 0 = 否;1 = 正;2 = 负*/
	m_dwStackSign=0;
	/** 所用的 PRM 倾斜度校正0 = 否;1为是; PRM Tilt Correction used*/
	m_dwPRMTiltCorrect=0;
	/** 线束名;ASCII 文本，64个字符; */
	//m_bySwathName[64];
	strcpy_s((char*)m_bySwathName,64,"SW1");
	/**施工模式bit0 = 1 Standard.bit1 = 1 Microseismic.bit2 = 1 Slip-sweep.	bit3 = 1 SQC Dump (VSR).bit4 = 1 Guidance(Navigation).*/
	m_dwOperatingMode=1;
	/** 无记录0 = 否;1 = 是*/
	m_dwNoLog=0;
	/** 监听时间100 到 128000毫秒; Listening time 100 to 128000 ms.*/
	m_dwListeningTime=1000;
	/** 转储类型0 = 正常转储;1 = 原始转储;2 = 附加转储;*/
	m_dwDumpType=0;
	/** 线束 Id; Swath ID*/
	m_dwSwathId=1;
	
	/** Seismic trace offset removal is disabled; 0 = No (i.e.offset is removed);1 = Yes (Microseismic mode); Sercel 4.0版本增加*/
	m_dwSeismicTaceOffsetRemoval=0;
}
CSEGDExtendedHeader::~CSEGDExtendedHeader()
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////CSEGDTraceHeads///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
CSEGDTraceHeads::CSEGDTraceHeads()
{
	/** 地震道组号*/
	m_byChnSetNb=1;
	/** 记录道编号*/
	m_dwTraceNb=1;
	/** 记录道编辑;00:未进行编辑。02:采集前静噪或停滞。03: 由采集系统编辑*/
	m_byTraceEdit=0;

	/** 接收点测线号*/
	m_dwLineNb=1;
	/** 接收点号*/
	m_dwPointNb=0;
	/** 接收点索引*/
	m_byPointIndex=1;
	/** 采样样本数，与*/
	//DWORD dwSampleCount;
	/** 检波器SEGD代码,在Line程序中设置*/
	m_bySensorSEGDCode=0;

	/** 接收点偏东距*/
	m_dbPointEasting=0;
	/** 接收点北向纬度*/
	m_dbPointNorthing=0;
	/** 接收点高程*/
	m_fPointElevation=0;
	/** 传感器类型编号*/
	m_bySensorType=1;

	/** 电阻值上限*/
	m_fResistanceMax=10000;
	/** 电阻值上限*/
	m_fResistanceMin=0;
	/** 电阻值*/
	m_fResistanceVal=0;
	/** 最大陆上检波器倾斜度百分比*/
	m_fTiltMax=0;
	/** 倾斜度值*/
	m_fTiltVal=0;

	/** 电容上限*/
	m_fCapacitanceMax=10;
	/** 电容下限*/
	m_fCapacitanceMin=0;
	/** 电容值，纳法拉*/
	m_fCapacitanceVal=0;
	/** 截止上限*/
	m_fCutoffFreqMax=0;
	/** 截止下限*/
	m_fCutoffFreqMin=0;
	/** 截止值*/
	m_fCutoffFreqVal=0;
	/** 漏电界限*/
	m_fLeakageLimit=100;
	/** 漏电值*/
	m_fLeakageVal=10000;

	/**设备类型, 仪器类型：1为FDU；0x1C表示DSU428 */
	// BYTE	m_byUnitType;由扫描类型头段的变量说明
	/** 设备序列号*/
	m_dwUnitSN=0;
	/** 地震道编号*/
	m_byChannelNb=0;
	/**组件类型*/
	m_byAssemblyType=1;
	/** FDU 或 DSU 组件序列号*/
	m_dwFDUAssemblySN=0;
	m_byLocationInFDUAssembly = 0;
	/** 子单元类型*/
	m_bySubunitType=0x01;
	/** 地震道类型:0 检波器;1 压敏检波器*/
	m_byChannelType=0;
	/** 检波器灵敏度*/

	/** 控制单元类型:0x01 LCI408;0x02 LAUL408;0x03 LAUX408;0x04 LAULS408;0x05 LAUXS408;0x06 LAUL428;0x16 LRU;0x17 LAUR428;0x30 LAUX428;0x31 LCI428*/
	m_byControlUnitType=0x01;
	/** 控制单元序列号*/
	m_dwControlUnitSN=0;
	/** 地震道增益刻度*/
	m_byGainScale=1;
	/** 地震道滤波器*/
	m_byChannelFilter=1;
	/** 地震道数据误差*/
	m_byChannelDataError=0;
	/** 地震道编辑状态,1 停滞;2 采集/ 回传错误;3 噪声编辑*/
	m_byChannelEditedStatus=1;
	/** 地震道采样毫伏转换系数,辅助道为0*/
	m_fChannelConversionFactor=0;
	/** 有噪声叠加次数*/
	m_byNumberOfStacksNoisy=0;
	/** 低叠加次数*/
	m_byNumberOfStacksLow=0;
	/** 地震道类型标识号：1 = 地震道;9 = 辅助道;在扫描类型头段对象中*/

	/** 地震道处理:01=原始数据;	02=辅助道叠加;03=相关，负部分;04=相关，正部分;05=正态相关;06=地震道叠加*/
	m_byChannelProcessType=01;
	/** 记录道最大值*/
	m_fTraceMaxValue=0;
	/** 记录道最大时间,微秒*/
	m_dwTraceMaxtime=0;
	/** 内插次数*/
	m_dwInterpolations=0;
	/**Seismic trace offset value(if offset removal is disabled).*/
	m_dwSeismicTraceOffsetValue=0;
}
CSEGDTraceHeads::~CSEGDTraceHeads()
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////   CSEGDFile   ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief CSEGDFile构造函数
 * @note  CSEGDFile构造函数
 * @param  void
 * @return void
 */
CSEGDFile::CSEGDFile(void)
: m_pFile(NULL)
{
	InitASCIIToBCD();
	m_dwFileNumber = 800;
	m_dwSampleRateUS = 1000;
	m_dwRecordLenMS = 10000;
	m_byRecordType = 8;
	m_dwSourcePointIndex = 1;
	m_dwFleetNb = 1;

	m_byManufacturerCode=13;
	m_wManufacturerSN=0;
	m_byChnSetsCount=16;
	m_wExternalHeaderSize = 1024;

	
	m_strShooterInfo = _T("");
	/** 来自Config程序设置的用户信息*/
	m_strConfigUserInfo = _T("");
	/** 施工窗口的震源注释*/
	m_strSourceComment = _T("");
}
/**
 * @brief CSEGDFile析构函数
 * @note  CSEGDFile析构函数
 * @param  void
 * @return void
 */
CSEGDFile::~CSEGDFile(void)
{
	if(m_pFile)
	{
		m_pFile->Flush();
		m_pFile->Close();
		delete m_pFile;
		m_pFile=NULL;
	}
}
/**
 * @brief 关闭SEGD文件，删除对象
 * @note  关闭SEGD文件，删除对象
 * @param  void
 * @return void
 */
bool CSEGDFile::CloseSEGDFile(void)
{
	try
	{
		if(m_pFile)
		{
			m_pFile->Flush();
			m_pFile->Close();
			delete m_pFile;
			m_pFile=NULL;
		}
	}
	catch (CFileException* )
	{
		return false;
	}
	return true;	
}
/**
 * @brief 十进制数字转化为BCD码
 * @note  十进制数字转化为BCD码
 * @param  void
 * @return void
 */
void    CSEGDFile::InitASCIIToBCD(void)
{	
	BYTE byData[10];
	m_unionType.lData = -0x12345678;
	byData[0]= m_unionType.byData[2];	// 扩展文件编号最高位
	byData[1]= m_unionType.byData[1];	
	byData[2]= m_unionType.byData[0];	// 扩展文件编号最低位
	byData[3]= 0;

	m_byDecToBCD[0] = 0;
	m_byDecToBCD[1] = 0x01;
	m_byDecToBCD[2] = 0x02;
	m_byDecToBCD[3] = 0x03;
	m_byDecToBCD[4] = 0x04;
	m_byDecToBCD[5] = 0x05;
	m_byDecToBCD[6] = 0x06;
	m_byDecToBCD[7] = 0x07;
	m_byDecToBCD[8] = 0x08;
	m_byDecToBCD[9] = 0x09;
}
/**
 * @brief 字节按位反序
 * @note  将字节反序，最低位变成最高位，依次转换。如数是01001001，反序后为10010010
 * @param  unsigned char   ucRegchar
 * @return unsigned char,输出结果
 */
unsigned char   CSEGDFile::ReverseChar(unsigned char   ucRegchar) 
{ 
	unsigned char   ucTemp   = 0; 
	unsigned char   ucResult = 0; 
	int   i; 

	for(i=0;i<8;i++) 
	{
		ucTemp = ucRegchar >> (7-i); 
		ucResult |= (ucTemp & (unsigned char)0x1) << i; 
	} 
	return   ucResult; 
} 
/**
 * @brief 单字节数字转化为BCD码
 * @note  将单字节数字转化为BCD码；需要进行高低位交换，高位在第一个字节，且最高位位于第一个字节的0位上
 * @param  WORD wData,小于或者等于9999
 * @param  unsigned char* byData，返回四个字节的BCD码，高位在前，且最高位位于第一个字节的0位上
 * @return void
 */
BYTE CSEGDFile::ConvertBYTEToBCD(BYTE byData)
{
	BYTE  byTemp,byRet;
	byTemp = byData/10;
	byRet = m_byDecToBCD[byTemp];
	byTemp = byData%10;
	byRet = byRet << 4;
	byRet |= m_byDecToBCD[byTemp];
	// byRet = ReverseChar(byRet);
	return byRet;
}
/**
 * @brief 将不大于9999的数字转化为BCD码
 * @note  将不大于9999的数字转化为BCD码；需要进行高低位交换，高位在第一个字节，且最高位位于第一个字节的0位上
 * @param  WORD wData,小于或者等于9999
 * @param  unsigned char* byOut，输出参数，至少2个字节的空间，保存BCD码，高位在前，且最高位位于第一个字节的0位上
 * @return void
 */
void  CSEGDFile::ConvertWORDToBCD(WORD wData,unsigned char* byOut)
{
	WORD    wTemp;
	BYTE	byData[4];
	if(!byOut || wData>9999)
		return ;
	// 计算最高位
	wTemp = wData/1000;
	byData[0] = m_byDecToBCD[wTemp]; 
	wData -=wTemp*1000;
	wTemp = wData/100;
	byData[1] =  m_byDecToBCD[wTemp];
	wData -=wTemp*100;
	wTemp = wData/10;
	byData[2] =  m_byDecToBCD[wTemp]; 	
	wTemp = wData%10;
	byData[3] =  m_byDecToBCD[wTemp];

	byData[0]=byData[0]<<4;
	byData[0]|=byData[1];
	//byOut[0] = ReverseChar(byData[0]);
	byOut[0] = byData[0];
	
	byData[2]=byData[2]<<4;
	byData[2]|=byData[3];
	// byOut[1] = ReverseChar(byData[2]);
	byOut[1] = byData[2];
	
	//wTemp = MAKEWORD(byData[1],byData[3]);	
	return ;
}

/**
 * @brief 计算儒略日
 * @note  按照儒略日计算年内的序数日，用于填充普通头段1 的12和13字节
          年内序数日的计算公式：

	      年内的序数日 N 可由以下公式得出：N = INT(275*M/9) - K*INT((M+9)/12) + D - 30 

		  此处 M 为月份，D为该月日期，闰年 K = 1，平年 K = 2 N取整数，自1月1日开始取值1，直至12月31日取值365（或闰年取值366）。

		  例 7.f 1978年11月14日。 

		  平年 K=2，M = 11, D = 14, K = 2。得 N = 318

		  例 7.g 1988年4月22日。 

		  闰年 K=1， M = 4， D = 22，K = 1。得 N = 113. 

		  现在让我们考虑逆问题： 年内序数日N已知，要求相应的日期，M为月份，D为该月日期。

		  下面的算法是由比利时民间天文协会的A. Pouplier发现的。 

		  如上所述，令K = 1 若为闰年K = 2 若为平年M = INT(9(K+N)/275+0.98)，

		  若 N < 32，则令 M = 1D = N – INT(275M/9) + K*INT((M+9)/12) + 30 

 * @param  SYSTEMTIME* pSysTime，当前时间
 * @return 两个字节年内的序数日
 */
WORD CSEGDFile::CalcJulianDay(SYSTEMTIME* pSysTime)
{
	WORD   wResult;
	if(!pSysTime)
		return 0;
	int nLeapYear = 2;
	wResult = pSysTime->wYear;
	// 判断是否为闰年，如果为闰年，nLeapYear = 1
	if((wResult%4==0) && (wResult%100!=0))
	{
		nLeapYear = 1;
	}
	if (wResult%400 == 0)
	{
		nLeapYear =1;
	}

	wResult = 275*pSysTime->wMonth/9 - nLeapYear*((pSysTime->wMonth+9)/12)+pSysTime->wDay-30;
	return wResult;

}
/**
 * @brief 转换基本扫描间隔为SEGD要求格式
 * @note  转换基本扫描间隔为SEGD要求格式
 * @param  float fSample
 * @return void
 */
BYTE  CSEGDFile::GetBaseScanInterval()
{
	switch(m_dwSampleRateUS)
	{
	case 250:
		return 4;
		break;
	case 500:
		return 8;
		break;
	case 1000:
		return 10;
		break;
	case 2000:
		return 20;
		break;
	case 4000:
		return 40;
		break;
	default:
		return 10;
		break;
	}
}

/**
 * @brief 将小于1的浮点数转化为16位的表示方法
 * @note 将小于1的浮点数转化为16位的表示方法，输出为两个字节，byOut[0]为高位，byOut[1]为低位
 * @param  
 * @return void
 */
void CSEGDFile::ConvertLessOneFloatToWORD(double fValue,unsigned char* byOut)
{
	int i;
	unsigned long dwTemp=0;
	unsigned long dwRet=0;
	for(i=0;i<16;i++)
	{
		fValue*=2;
		dwRet = dwRet<<1;
		dwTemp = (unsigned long)fValue;
		dwRet |=dwTemp;
		fValue -=dwTemp;		
	}
	byOut[0] = HIBYTE(dwRet);
	byOut[1] = LOBYTE(dwRet);
}

/**
 * @brief 将小于1的小数转化为1个字节的整数形式
 * @note 将小于1的浮点数转化为1个字节的表示方法:T-1,T-2,T-3,T-4,T-5,T-6,T-7,T-8
 * @param  
 * @return BYTE
 */
BYTE  CSEGDFile::ConvertLessOneFloatToBYTE(double fValue)
{
	int i;
	BYTE byTemp=0;
	BYTE byRet=0;
	for(i=0;i<16;i++)
	{
		fValue*=2;
		byRet = byRet<<1;
		byTemp = (BYTE)fValue;
		byRet |=byTemp;
		fValue -=byTemp;		
	}
	return byRet;
}
/**
 * @brief  向文件中写入总头段数据块1
 * @note   按照SEGD2.1的格式写入总头段1，长度为32字节
 * @param  void
 * @return void
 */
void CSEGDFile::WriteGeneralHeader1( )
{
	unsigned char byData[32];
	WORD  wTemp;
	// 1-2字节，文件编号
	if(m_dwFileNumber>9999)
	{
		byData[0] = 0xFF;
		byData[1] = 0xFF;
	}
	else
	{
		ConvertWORDToBCD(WORD(m_dwFileNumber),byData);
	}
	// 3-4字节，浮点格式代码，32位IEEE
	ConvertWORDToBCD(8058,byData+2);
	// 5-10字节，普通常数
	memset(byData+4,0,6);
	byData[9] = 0x05;		// ??????? 428XL对应为5，没有意义
	// 11字节，年份的最后两位数（0-99）
	SYSTEMTIME	sysTime;
	GetLocalTime(&sysTime);
	byData[11] = sysTime.wYear%100;
	byData[10] = ConvertBYTEToBCD(byData[11]);

	// 12H，取值2，表示附加的块数
	// 12L-13字节，Julia日（儒略日），3位数(0-366)
	wTemp = CalcJulianDay(&sysTime);
	wTemp = wTemp + 2000;
	ConvertWORDToBCD(wTemp,byData+11);
	// 14字节，一天中的小时(0-23)
	byData[14] = (BYTE)sysTime.wHour;
    byData[13]=ConvertBYTEToBCD(byData[14]);

	// 15字节，一天中的分钟(0-59)
	byData[15] = (BYTE)sysTime.wMinute;
	byData[14]=ConvertBYTEToBCD(byData[15]);

	// 16字节，一天中的分钟(0-59)
	byData[16] = (BYTE)sysTime.wSecond;
	byData[15]=ConvertBYTEToBCD(byData[16]);

	// 17字节，制造商代码,Sercel公司为13,
	byData[16] = ConvertBYTEToBCD(m_byManufacturerCode);
	// 18-19字节，制造商序列号
	ConvertWORDToBCD(m_wManufacturerSN,byData+17);	
	// 20-22字节，SEGD标准未用
	byData[19] = 0;
	byData[20] = 0;
	byData[21] = 0;
	// 23字节，基本扫描间隔
	byData[22] = GetBaseScanInterval();
	byData[22]=ConvertBYTEToBCD(byData[22]);
	// 24H极性测试，24L-25未用，两个字节均为0
	byData[23] = 0;
	byData[24] = 0;
	// 26H，记录类型，8为正常
	byData[25] = ConvertBYTEToBCD(m_byRecordType)<<4;
	// 26L-27，记录长度，使用扩展记录长度为0xFFF;
	byData[25]|= 0x0F;
	byData[26] = 0xFF;
	// 28字节，每个记录的扫描类型数
	byData[27] = 1;
	// 29字节，每个扫描类型通道组数，16
	byData[28] = ConvertBYTEToBCD(m_byChnSetsCount);
	// 30字节，采样时偏头端，0
	byData[29] = 0;
	// 31字节，扩展头端长度，32*32=1024
	byData[30] = ConvertBYTEToBCD(32);
	// 32字节，外部头端长度，32*32=1024
	if(m_wExternalHeaderSize>99*32)
		byData[31] = 0xFF;
	else
		byData[31] = ConvertBYTEToBCD(BYTE(m_wExternalHeaderSize/32));
	// byData[31] = 0;

	if(m_pFile)
	{
		m_pFile->Write(byData,32);
	}
}
/**
 * @brief 向文件中写入总头段数据块2
 * @note  按照SEGD2.1的格式写入总头段2，长度为32字节
 * @param  
 * @return void
 */
void CSEGDFile::WriteGeneralHeader2()
{
	unsigned char  byData[32];
	WORD           wTemp;
	// 1-3字节，文件编号,4-5字节为扫描类型，未使用为0
	if(m_dwFileNumber<=9999)
	{
		byData[0] = 0x00;
		byData[1] = 0x00;
		byData[2] = 0x00;
		byData[3] = 0x00;
	}
	else
	{	
		m_unionType.dwData = m_dwFileNumber;
		byData[0]= m_unionType.byData[2];	// 扩展文件编号最高位
		byData[1]= m_unionType.byData[1];	
		byData[2]= m_unionType.byData[0];	// 扩展文件编号最低位
		byData[3]= 0;
		
	}
	byData[4] = 0x00;

	// 6-7字节为扩展头段数据块（未使用）
	byData[5] = 0x00;
	byData[6] = 0x00;
	// 8-9字节为外部头段数据块
	if(m_wExternalHeaderSize<=99*32)
	{
		byData[7] = 0x00;
		byData[8] = 0x00;
	}
	else
	{
		wTemp = m_wExternalHeaderSize/32;
		byData[7] = HIBYTE(wTemp);
		byData[8] = LOBYTE(wTemp);
	}	
	// 10字节未使用
	byData[9] = 0x00;
	// 11-12字节，bin,SEGD版本, 按照rev2.1标准写为2.0
	byData[10] = 0x02;
	byData[11] = 0x00;
	// 13-14字节，普通尾段
	byData[12] = 0x00;
	byData[13] = 0x00;
	// 15-17字节，bin扩展记录长度，0-128000ms
	m_unionType.dwData = m_dwRecordLenMS;
	byData[14] = m_unionType.byData[2];	// 最高位
	byData[15] = m_unionType.byData[1];
	byData[16] = m_unionType.byData[0];	// 最低位
	// 18字节，未使用
	byData[17] = 0;
	// 19字节，总头块编号，为2号数据头块
	byData[18] = 2;
	// 20-32未使用
	memset(byData+19,0,13);

	if(m_pFile)
	{
		m_pFile->Write(byData,32);
	}
}
/**
 * @brief 向文件中写入总头段数据块3
 * @note  按照SEGD2.1的格式写入总头段3，长度为32字节
 * @param  DWORD dwFileNumber,double fSourceLine,double fSourceNb,DWORD dwSourcePointIndex,DWORD dwSwathNb
 * @return void
 */
void CSEGDFile::WriteGeneralHeader3()
{
	unsigned char  byData[32];
	// 1-3字节，扩展文件编号
	if(m_dwFileNumber<=9999)
	{
		byData[0] = 0x00;
		byData[1] = 0x00;
		byData[2] = 0x00;
	}
	else
	{	
		m_unionType.dwData = m_dwFileNumber;
		byData[0]= m_unionType.byData[2];	// 扩展文件编号最高位
		byData[1]= m_unionType.byData[1];	
		byData[2]= m_unionType.byData[0];	// 扩展文件编号最低位
	}
	// 4-8字节，震源测线号（0-99999.99），其中4-6为整数部分（3字节，2的补码）,7-8小数部分
	m_unionType.lData = (long)m_fSourceLine;
	byData[3]= m_unionType.byData[2];	// 扩展文件编号最高位
	byData[4]= m_unionType.byData[1];	
	byData[5]= m_unionType.byData[0];	// 扩展文件编号最低位
	ConvertLessOneFloatToWORD(m_fSourceLine-m_unionType.lData,byData+6);
	// 9-13字节，震源点号（0-99999.99），其中9-11为整数部分（3字节，2的补码）,12-13小数部分
	m_unionType.lData = (long)m_fSourceNb;
	byData[8]= m_unionType.byData[2];	// 扩展文件编号最高位
	byData[9]= m_unionType.byData[1];	
	byData[10]= m_unionType.byData[0];	// 扩展文件编号最低位
	ConvertLessOneFloatToWORD(m_fSourceNb-m_unionType.lData,byData+11);
	// 14字节，震源点索引，震源点移动次数(1-9)
	byData[13] = (unsigned char)m_dwSourcePointIndex;		
	// 15字节，相位控制(未记录)
	byData[14]=0;
	// 16字节，可控震源类型(未记录)
	byData[15]=0;
	// 17-18字节，相位角(未记录)
	byData[16]=0;
	byData[17]=0;
	// 19字节，总头段编号
	byData[18] = 3;
	// 20字节，震源组号
	byData[19] = (unsigned char)m_dwFleetNb;		// ??????????????或者炮点的SwathNb
	// 21-32未使用
	memset(byData+20,0,12);

	if(m_pFile)
	{
		m_pFile->Write(byData,32);
	}
}
/**
 * @brief 向文件中写入一个扫描类型头段的地震道组
 * @note  按照SEGD2.1的格式写入一个扫描类型头段的地震道组，长度为32字节;
          根据设置一个扫描类型头段中有多个地震道组。
 * @param  
 * @param  
 * @return void
 */
void CSEGDFile::WriteScanTypeHeader(CSEGDScanTypeHeader* pScanTypeHeader)
{
	unsigned char  byData[32];
	memset(byData,0,32);
	if(!pScanTypeHeader)
	{
		if(m_pFile)
		{
			m_pFile->Write(byData,32);
		}
		return;
	}
	WORD  wTemp;
	// 1字节，扫描类型头段编号，只有一个，取值01
	byData[0] = 0x01;
	// 2字节，地震道通道组号
	byData[1] = ConvertBYTEToBCD(pScanTypeHeader->m_byChnSetNb);
	// 3-4字节，通道组开始时间，2ms为增量，0-131070ms
	byData[2] = HIBYTE(pScanTypeHeader->m_wChnStartingTime);
	byData[3] = LOBYTE(pScanTypeHeader->m_wChnStartingTime);
	// 5-6字节，通道组终止时间，2ms为增量，0-131070ms
	byData[4] = HIBYTE(pScanTypeHeader->m_wChnEndTime);
	byData[5] = LOBYTE(pScanTypeHeader->m_wChnEndTime);
	// 7-8字节，缩小比例乘数	
	wTemp = pScanTypeHeader->GetDescaleMultiplier();
	byData[6] = LOBYTE(wTemp);
	byData[7] = HIBYTE(wTemp);
	// 9-10字节，BCD码，该通道组中通道数,0-9999
	ConvertWORDToBCD(pScanTypeHeader->m_wChnCount,byData+8);
	// 11H为地震道类型标识：1为地震道，9为辅助道;
	// 11L未使用
	if (pScanTypeHeader->m_byChnType==9)
	{
		byData[10]=0x90;
	}
	else
	{
		byData[10] = 0x10;
	}
	// 12H为副扫描指数数目,为0
	// 12L为地震道增益控制方式，固定增益为3
	byData[11] = 0x03;	
	// 13-14字节，BCD码，3db点处的混叠滤波器频率	
	wTemp = pScanTypeHeader->GetAliasFilterfrequency(m_dwSampleRateUS);
	ConvertWORDToBCD(wTemp,byData+12);
	// 15-16字节，BCD码，混叠滤波器斜率			
	wTemp = pScanTypeHeader->GetAliasFilterSlope();
	ConvertWORDToBCD(wTemp,byData+14);	
	// 17-18字节，BCD码，低截滤波器频率
	wTemp = pScanTypeHeader->GetLowcutFilterFrequency();
	ConvertWORDToBCD(wTemp,byData+16);	
	// 19-20字节，BCD码，低截滤波器斜率
	wTemp = pScanTypeHeader->GetLowcutFilterSlope();
	ConvertWORDToBCD(wTemp,byData+18);
	// 21-22字节，第一陷波器斜率
	byData[20] = 0;
	byData[21] = 0;
	// 23-24字节，第二陷波器斜率
	byData[22] = 0;
	byData[23] = 0;
	// 25-26字节，第三陷波器斜率
	byData[24] = 0;
	byData[25] = 0;
	// 27-28字节，扩展地震组道号
	byData[26] = 0;
	byData[27] = 0;
	// 29H,扩展头标记，29L，道头扩展
	byData[28] = 0x07;
	// 30字节，垂直叠加
	byData[29] = pScanTypeHeader->m_byVerticalStack;
	// 31字节，海洋拖缆电缆号，陆地为0
	byData[30] = 0x00;
	// 32字节，阵列组成,组合类型
	byData[31] = 0x01;
	
	if(m_pFile)
	{
		m_pFile->Write(byData,32);
	}
}
/**
 * @brief 向文件中写入扩展头段
 * @note  按照SEGD2.1的格式写入扩展头段，长度为32*32=1024字节;
          扩展头段由厂家决定，与数据处理有密切关系
 * @param  
		
 * @return void
 */
#define   MACRO_Write_BigEndianDWORD(dwValue)  { ReverseDWORDToBigEndian((dwValue),byData+nPos); nPos+=4; }
#define   MACRO_Write_BigEndianFloat(dwValue)  { ReverseFloatToBigEndian((dwValue),byData+nPos); nPos+=4; }

void CSEGDFile::WriteExtendedHeader()
{	
	unsigned char byData[1024];
	memset(byData,0,1024);
	DWORD		  dwTemp;
	int           nPos=0;
	union unDOUBLE
	{
		BYTE  byData[8];
		double dbData;
	}undbTemp;

	// 1-4，采样长度，与普通头段2号的15-17字节相同
	MACRO_Write_BigEndianDWORD( m_dwRecordLenMS );	
	// 5-8，采样率，以微秒为单位
	MACRO_Write_BigEndianDWORD( m_dwSampleRateUS);		
	// 9-12，记录道总数
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwTraceCount);	
	// 13-16，辅助道总数
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwAuxesTraceCount);	
	// 17-20，地震记录道总数
	dwTemp = m_ExtendedHeader.m_dwTraceCount-m_ExtendedHeader.m_dwAuxesTraceCount;	
	MACRO_Write_BigEndianDWORD( dwTemp);
	// 21-24，停滞地震记录道数目: 地震记录道总数 - 活动地震记录道数目
	dwTemp -= m_ExtendedHeader.m_dwLiveSeisTraceCount;
	MACRO_Write_BigEndianDWORD( dwTemp);	
	// 25-28，活动地震记录道数目			
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwLiveSeisTraceCount);	
	// 29-32，震源类型
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwSourceType );	
	// 33-36，记录道中采样数目
	dwTemp =  GetNumberOfSamples();
	MACRO_Write_BigEndianDWORD( dwTemp );
	// 37-40，炮号
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwShotNb);
	// 41-44,TB窗口
	MACRO_Write_BigEndianFloat( m_ExtendedHeader.m_fTBWindowS);	// ????????????是否要变成BigEndian
	// 45-48,测试记录类型
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwTestType);
	// 49-52,第一测线
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwSpreadSFL);


	// 53-56,Spread first number
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwSpreadSFN);		
	// 57-60,排列编号
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwSpreadNb);	
	// 61-64，排列类型
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwSpreadType);	
	// 65-68,时断信号
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwConfirmationTBUS);
	// 69-72,井口信号
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwUpholetimeUS);
	// 73-76,爆炸机标识
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwBlasterID);
	// 77-80,爆炸机状态
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwBlasterStatus);
	// 81-84,折射延时
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwRefractionDelayMS);
	// 85-88,TB到T0时间	
	ReverseDWORDToBigEndian((DWORD)m_ExtendedHeader.m_lTBToTStartUS,byData+nPos);
	nPos+=4;
	// 89-92,内部时断信号，ITB中断
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwInternalTimeBreak);

	// 93-96,野外设备中预叠加
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwPrestack);
	// 97-100,噪声抑制类型: 1 无;2花样叠加;3 历史;4 关闭
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwNoiseEliminationType);	
	// 101-104,低记录道百分比，0~100%; Low trace percentage	
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwLowTracePercentage);	
	// 105-108,低记录道值;0-132db; Low trace value 
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwLowTraceValue);
	// 109-112,窗口数目1-64; Number of windows (Div.)or Window length (Enhanced Div.) 
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwWindowLength);
	// 113-116,历史编辑类型：1为归零；2为剪裁；
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwHistoricEditingType);
	// 117-120,噪声记录道百分比：0 到100% ； Noisy trace percentage
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwNoisyTracePercentage);	
	// 121-124,历史范围0 到 36 dB;  Historic range 
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwHistoricRange);	
	// 125-128,历史锥形长度 2 的指数:0 到 8;Historic taper length 2’s	exponent
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwExpHistoricTaperLen );
	// 129-132 门槛值不变/ 变化1 = 不变;2 = 变化; Threshold Hold/Var
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwThresholdHold );
	// 133-136,历史门槛值初始值0 到 132 dB; Historic threshold Init value
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwHistoricThresholdValue);
	// 137-140,历史归零长度1 到 500 毫秒; Historic zeroing length
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwHistoricZeroingLen);

	// 141-144, 处理类型1 无操作（原始数据）;2 叠加;	3 叠加后相关;4 叠加前相关;5 原始数据，叠加;	6 原始数据，叠加后相关;7 原始数据，叠加前相关
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwProcessType);
	// 145-272 采集类型表,32个数值，128字节*/
	for (dwTemp=0;dwTemp<32;dwTemp++)
	{
		MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwAcquisitionTypeTable[dwTemp]);
	}	
	// 273-400,门槛值类型表32 个数值（128 字节）,Threshold type tables 32 values (128 bytes).*/
	for (dwTemp=0;dwTemp<32;dwTemp++)
	{
		MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwThresholdTypeTable[dwTemp]);
	}	
	// 401-404,叠加次数,1~32; Stacking fold
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwStackingFold);
	// 405-484,未使用
	nPos=485;
	// 485-488,记录长度100 到 128000 毫秒;Record length
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwRecordLength);
	// 489-492,自相关峰值时间1 到 128000 毫秒; Autocorrelation peak time 
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwAutoCorrPeakTime);
	// 493-496,未使用
	nPos = 497;
	// 497-500,相关参考信号编号1 到 100000;Correlation Pilot No. 1 to 100000.
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwCorrelationPilot);
	// 501-504,参考信号长度1000 到 128000 毫秒 ; Pilot length
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwPilotLength);
	// 505-508,扫描长度1000 到 128000 毫秒; Sweep length
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwSweepLength);
	// 509-512,采集号1 到 32; Acquisition number 
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwAcquisitionNb);

	// 513-516,最多辅助道的最大值;Max of max, Aux
	MACRO_Write_BigEndianFloat( m_ExtendedHeader.m_fMaxAux);
	// 517-520,最多地震道最大值IEEE 格式，单精度;Max of max, Seis
	MACRO_Write_BigEndianFloat( m_ExtendedHeader.m_fMaxSeis);
	// 521-524,转储叠加次数1..32;Dump stacking fold
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwDumpStackingFold);
	// 525-540,磁带标签ASCII 文本，16 个字符
	memcpy_s(byData+nPos,16,m_ExtendedHeader.m_byTapeLabel,16);
	nPos+=16;
	// 541-544,磁带号1 到 9999 
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwTapeNb);
	// 545-560,软件版本ASCII 文本，16 个字符; Software version ASCII text, 16 characters.
	memcpy_s(byData+nPos,16,m_ExtendedHeader.m_bySoftwareVersion,16);	
	nPos+=16;
	// 561-572,日期ASCII 文本，12 个字符(dd mmm yyyy)
	memcpy_s(byData+nPos,12,m_ExtendedHeader.m_byDate,12);
	nPos+=12;
	// 573-580,震源偏东距; Source easting	
	undbTemp.dbData = m_ExtendedHeader.m_dbSourceEasting;
	byData[nPos++] = undbTemp.byData[7];
	byData[nPos++] = undbTemp.byData[6];
	byData[nPos++] = undbTemp.byData[5];
	byData[nPos++] = undbTemp.byData[4];
	byData[nPos++] = undbTemp.byData[3];
	byData[nPos++] = undbTemp.byData[2];
	byData[nPos++] = undbTemp.byData[1];
	byData[nPos++] = undbTemp.byData[0];
	// 581-588, 震源北向纬度差; Source northing
	undbTemp.dbData = m_ExtendedHeader.m_dbSourceNorthing;
	byData[nPos++] = undbTemp.byData[7];
	byData[nPos++] = undbTemp.byData[6];
	byData[nPos++] = undbTemp.byData[5];
	byData[nPos++] = undbTemp.byData[4];
	byData[nPos++] = undbTemp.byData[3];
	byData[nPos++] = undbTemp.byData[2];
	byData[nPos++] = undbTemp.byData[1];
	byData[nPos++] = undbTemp.byData[0];
	// 589-592,震源高程; Source elevation
	MACRO_Write_BigEndianFloat( m_ExtendedHeader.m_fSourceElevation);	
	// 593-596,所用的滑动扫描模式： 0为否;1为是 
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwSlipSweepMode);
	// 597-600,每个磁带的文件数1 到 9999
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwFilesPerTape);
	// 601-604,文件计数1 到 9999
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwFileCount);
	// 605-764,采集错误说明ASCII 文本，160 个字符
	memcpy_s(byData+nPos,160,m_ExtendedHeader.m_byAcquisitErrorDesc,160);	
	nPos+=160;
	// 765-768,滤波器类型: 1 =0.8最小; 2= 0.8 线性; 
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwFilterType);
	// 769-772, 叠加已转储: 	0 = No; 1 = Yes.;
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwStackIsDumped);
	// 773-776, 叠加符号（当前） 0 = 否;1 = 正;2 = 负
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwStackSign);
	// 777-780, 所用的 PRM 倾斜度校正0 = 否;1为是; PRM Tilt Correction used
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwPRMTiltCorrect);
	// 781-844, 线束名;ASCII 文本，64个字符; 
	memcpy_s(byData+nPos,64,m_ExtendedHeader.m_bySwathName,64);	
	nPos+=64;
	// 845-848, 施工模式
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwOperatingMode);
	// 849-852, 未用
	nPos = 853;
	// 853-856, 无记录0 = 否;1 = 是
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwNoLog);
	// 857-860,监听时间100 到 128000毫秒; Listening time 100 to 128000 ms.
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwListeningTime);
	// 861-864, 转储类型0 = 正常转储;1 = 原始转储;2 = 附加转储;
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwDumpType);
	// 865-868, 未用
	nPos = 869;
	// 869-872, 线束 Id; Swath ID*/
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwSwathId);

	// 873-876,Seismic trace offset removal is disabled; Sercel 4.0版本增加
	MACRO_Write_BigEndianDWORD( m_ExtendedHeader.m_dwSeismicTaceOffsetRemoval);
	// 877-1024,未使用
	if(m_pFile)
	{
		m_pFile->Write(byData,1024);
	}
}
/**
 * @brief 向文件中写入外部头段
 * @note  写入外部头段，长度为 m_wExternalHeaderSize 字节;
          扩展头段由厂家决定，主要包括来自配置窗口、施工窗口的注释
 * @param  
		
 * @return void
 */
void CSEGDFile::WriteExternalHeader()
{
	char* byData=new char[m_wExternalHeaderSize];
	memset(byData,0,m_wExternalHeaderSize);
	// 放炮或者导航系统信息
	CT2A	pstr1(m_strShooterInfo);
	size_t  nLen1=strlen(pstr1)+2;
	strcpy_s(byData,m_wExternalHeaderSize,pstr1);
	// 来自Config程序设置的用户信息
	CT2A	pstr2(m_strConfigUserInfo);
	size_t  nLen2=strlen(pstr2)+2;
	strcpy_s(byData+nLen1,m_wExternalHeaderSize-nLen1,pstr2);
	// 施工注释信息
	CT2A	pstr3(m_strSourceComment);
	strcpy_s(byData+nLen1+nLen2,m_wExternalHeaderSize-nLen1-nLen2,pstr3);

	if(m_pFile)
	{
		m_pFile->Write(byData,m_wExternalHeaderSize);
	}
}

/**
 * @brief 向文件中写道头数据
 * @note  每道数据前面的有20字节的多路解编道头
 * @param void
 * @return void
 */
void CSEGDFile::WriteDemuxTraceHeader(CSEGDTraceHeads*	pTrace)
{
	unsigned char byData[20];
	WORD    wTemp;
	// 1-2字节，文件编号
	if(m_dwFileNumber>9999)
	{
		byData[0] = 0xFF;
		byData[1] = 0xFF;
	}
	else
	{
		ConvertWORDToBCD(WORD(m_dwFileNumber),byData);
	}
	// 3Byte扫描类型编号
	byData[2] = 0x01;
	// 4字节，地震道组号
	byData[3] = ConvertBYTEToBCD(pTrace->m_byChnSetNb);
	// 5-6字节，记录道编号
	if(pTrace->m_dwTraceNb>9999)
	{
		byData[4] = 0xFF;
		byData[5] = 0xFF;
	}
	else
	{
		wTemp = (WORD)pTrace->m_dwTraceNb;
		ConvertWORDToBCD(wTemp,byData+4);
	}	
	// 7-9字节，第一个计时字，即折射延迟
	m_unionType.lData = m_ExtendedHeader.m_dwRefractionDelayMS;		// ???????????
	byData[6] = m_unionType.byData[2];
	byData[7] = m_unionType.byData[1];
	byData[8] = m_unionType.byData[0];
	// 10字节，道头扩展的数量，为7
	byData[9] = 7;
	// 11字节，采样时滞/采样扭曲
	byData[10] = 0;
	// 12字节，记录道编辑
	byData[11] = pTrace->m_byTraceEdit;
	// 13-15字节，时断信号窗口
	wTemp = (WORD)m_ExtendedHeader.m_fTBWindowS;
	byData[12] = HIBYTE(wTemp);
	byData[13] = LOBYTE(wTemp);
	byData[14] = ConvertLessOneFloatToBYTE(m_ExtendedHeader.m_fTBWindowS - wTemp);
	// 16-17字节，扩展地震道组号
	byData[15] = 0;
	byData[16] = 0;
	// 18-20字节，扩展文件编号
	m_unionType.dwData = m_dwFileNumber;
	byData[17] = m_unionType.byData[2];	// 扩展文件编号最高位
	byData[18] = m_unionType.byData[1];	
	byData[19] = m_unionType.byData[0];	// 扩展文件编号最低位
	
	if(m_pFile)
	{
		m_pFile->Write(byData,20);
	}
}

/**
 * @brief 向文件中写道头扩展数据块1
 * @note  
 * @param 	
 long lLineNb,接收点测线号
 long lPointNb,接收点点号
 BYTE byPointIndex,接收点索引
 DWORD dwSampleCount,每个记录道采样样本数目
 BYTE bySensorSEGDCode
 * @return void
 */
void CSEGDFile::WriteTraceHeaderExtension1(CSEGDTraceHeads*	pTrace)
{
	unsigned char byData[32];
	// 1-3字节，接收点测线号
	m_unionType.lData = pTrace->m_dwLineNb;
	byData[0] = m_unionType.byData[2];
	byData[1] = m_unionType.byData[1];
	byData[2] = m_unionType.byData[0];
	// 4-6字节，接收点点号
	m_unionType.lData = pTrace->m_dwPointNb;
	byData[3] = m_unionType.byData[2];
	byData[4] = m_unionType.byData[1];
	byData[5] = m_unionType.byData[0];
	// 7字节，接收点索引
	byData[6] = pTrace->m_byPointIndex;
	// 8-10字节，每个记录道采样样本数目
	m_unionType.dwData = GetNumberOfSamples();
	byData[7] = m_unionType.byData[2];
	byData[8] = m_unionType.byData[1];
	byData[9] = m_unionType.byData[0];
	// 11-15字节，扩展接收点测线号;当接收点测线号大于0xFFFFFF时，使用扩展接收点
	// 16-20字节，扩展接收点点号
	memset(byData+10,0,10);
	// 21字节，检波器的SEGD代码
	byData[20] = pTrace->m_bySensorSEGDCode;
	// 22-32未使用
	memset(byData+21,0,10);
	if(m_pFile)
	{
		m_pFile->Write(byData,32);
	}
}
/**
 * @brief 向文件中写道头扩展数据块2
 * @note  
 * @param 	
 double dbFromeEast,接收点偏东距
 double dbNorthLatitudeDifference,接收点北向纬度差
 float fElevation,接收点高程
 BYTE bySensorType,检波器类型编号
 DWORD dwTraceNb,扩展记录道编号
 * @return void
 */
void CSEGDFile::WriteTraceHeaderExtension2(CSEGDTraceHeads*	pTrace)
{
	unsigned char byData[32];
	union unDOUBLE
	{
		BYTE  byData[8];
		double dbData;
	}undbTemp;
	
	// 1-8字节，double类型，接收点偏东距
	undbTemp.dbData = pTrace->m_dbPointEasting;
	byData[0] = undbTemp.byData[7];
	byData[1] = undbTemp.byData[6];
	byData[2] = undbTemp.byData[5];
	byData[3] = undbTemp.byData[4];
	byData[4] = undbTemp.byData[3];
	byData[5] = undbTemp.byData[2];
	byData[6] = undbTemp.byData[1];
	byData[7] = undbTemp.byData[0];
	// 9-16字节，double类型，接收点北向纬度差
	undbTemp.dbData = pTrace->m_dbPointNorthing;
	byData[8] = undbTemp.byData[7];
	byData[9] = undbTemp.byData[6];
	byData[10] = undbTemp.byData[5];
	byData[11] = undbTemp.byData[4];
	byData[12] = undbTemp.byData[3];
	byData[13] = undbTemp.byData[2];
	byData[14] = undbTemp.byData[1];
	byData[15] = undbTemp.byData[0];
	// 17-20字节，float类型，接收点高程
	m_unionType.fData = pTrace->m_fPointElevation;
	byData[16] = m_unionType.byData[3];
	byData[17] = m_unionType.byData[2];
	byData[18] = m_unionType.byData[1];
	byData[19] = m_unionType.byData[0];
	// 21字节，检波器类型编号
	byData[20] = pTrace->m_bySensorType;
	// 22-28字节，未使用
	memset(byData+21,0,6);
	// 29-32字节，扩展记录道编号
	if(pTrace->m_dwTraceNb>9999)
	{
		m_unionType.dwData = pTrace->m_dwTraceNb;
		byData[28] = m_unionType.byData[3];
		byData[29] = m_unionType.byData[2];
		byData[30] = m_unionType.byData[1];
		byData[31] = m_unionType.byData[0];
	}
	else
	{
		memset(byData+28,0,4);
	}
	
	if(m_pFile)
	{
		m_pFile->Write(byData,32);
	}
}
/**
 * @brief 向文件中写道头扩展数据块3
 * @note  
 * @param 	float fResistanceMax,
 float fResistanceMin,
 float fResistanceVal,
 float fTiltMax,
 float fTiltVal,
 BYTE byResistanceError,
 BYTE byTiltError	
 * @return void
 */
void CSEGDFile::WriteTraceHeaderExtension3(CSEGDTraceHeads*	pTrace)
{
	unsigned char byData[32];
	// 1-4 字节，电阻下限
	m_unionType.fData = pTrace->m_fResistanceMin;
	byData[0] = m_unionType.byData[3];
	byData[1] = m_unionType.byData[2];
	byData[2] = m_unionType.byData[1];
	byData[3] = m_unionType.byData[0];
	// 5-8 字节，电阻上限
	m_unionType.fData = pTrace->m_fResistanceMax;
	byData[4] = m_unionType.byData[3];
	byData[5] = m_unionType.byData[2];
	byData[6] = m_unionType.byData[1];
	byData[7] = m_unionType.byData[0];
	// 9-12 字节，电阻值(欧姆)
	m_unionType.fData = pTrace->m_fResistanceVal;
	byData[8] = m_unionType.byData[3];
	byData[9] = m_unionType.byData[2];
	byData[10] = m_unionType.byData[1];
	byData[11] = m_unionType.byData[0];
	// 13-16字节，倾斜度界限
	m_unionType.fData = pTrace->m_fTiltMax;
	byData[12] = m_unionType.byData[3];
	byData[13] = m_unionType.byData[2];
	byData[14] = m_unionType.byData[1];
	byData[15] = m_unionType.byData[0];
	// 17-20字节，倾斜度值(对于FDU地震道为%，对于DSU地震道为度)
	m_unionType.fData = pTrace->m_fTiltVal;
	byData[16] = m_unionType.byData[3];
	byData[17] = m_unionType.byData[2];
	byData[18] = m_unionType.byData[1];
	byData[19] = m_unionType.byData[0];
	// 21字节，电阻误差:0为否，1为是
	if(pTrace->m_fResistanceVal<pTrace->m_fResistanceMin ||pTrace->m_fResistanceVal>pTrace->m_fResistanceMax)
		byData[20] = 1;
	else
		byData[20] = 0;
	// 22字节，倾斜度误差:0为否，1为是
	if(pTrace->m_fTiltVal>pTrace->m_fTiltMax)
		byData[21] = 1;
	else
		byData[21] = 0;
	// 23-32未用
	memset(byData+22,0,10);
	if(m_pFile)
	{
		m_pFile->Write(byData,32);
	}
}
/**
 * @brief 向文件中写道头扩展数据块4
 * @note  
 * @param 		
 * @return void
 */
void CSEGDFile::WriteTraceHeaderExtension4(CSEGDTraceHeads*	pTrace)
{
	unsigned char byData[32];
	memset(byData,0xFF,32);
	// 1-4 字节，电容下限
	m_unionType.fData = pTrace->m_fCapacitanceMin;
	byData[0] = m_unionType.byData[3];
	byData[1] = m_unionType.byData[2];
	byData[2] = m_unionType.byData[1];
	byData[3] = m_unionType.byData[0];
	// 5-8 字节，电容上限
	m_unionType.fData = pTrace->m_fCapacitanceMax;
	byData[4] = m_unionType.byData[3];
	byData[5] = m_unionType.byData[2];
	byData[6] = m_unionType.byData[1];
	byData[7] = m_unionType.byData[0];
	// 9-12 字节，电容值(纳法拉)
	m_unionType.fData = pTrace->m_fCapacitanceVal;
	byData[8] = m_unionType.byData[3];
	byData[9] = m_unionType.byData[2];
	byData[10] = m_unionType.byData[1];
	byData[11] = m_unionType.byData[0];
	// 13-16 字节，截止下限
	m_unionType.fData = pTrace->m_fCutoffFreqMin;
	byData[12] = m_unionType.byData[3];
	byData[13] = m_unionType.byData[2];
	byData[14] = m_unionType.byData[1];
	byData[15] = m_unionType.byData[0];
	// 17-20 字节，截止上限
	m_unionType.fData = pTrace->m_fCutoffFreqMax;
	byData[16] = m_unionType.byData[3];
	byData[17] = m_unionType.byData[2];
	byData[18] = m_unionType.byData[1];
	byData[19] = m_unionType.byData[0];
	// 21-24 字节，截止值(赫兹)
	m_unionType.fData = pTrace->m_fCutoffFreqVal;
	byData[20] = m_unionType.byData[3];
	byData[21] = m_unionType.byData[2];
	byData[22] = m_unionType.byData[1];
	byData[23] = m_unionType.byData[0];
	// 25字节，电容误差:0为否，1为是
	if(pTrace->m_fCapacitanceVal<pTrace->m_fCapacitanceMin || pTrace->m_fCapacitanceVal>pTrace->m_fCapacitanceMax)
		byData[24] = 1;
	else
		byData[24] = 0;
	// 26字节，截止误差:0为否，1为是
	if(pTrace->m_fCutoffFreqVal<pTrace->m_fCutoffFreqMin || pTrace->m_fCutoffFreqVal>pTrace->m_fCutoffFreqMax)
		byData[25] = 1;
	else
		byData[25] = 0;
	// 27-32未用
	// memset(byData+26,0,6);
	if(m_pFile)
	{
		m_pFile->Write(byData,32);
	}
}
/**
 * @brief 向文件中写道头扩展数据块5
 * @note  
 * @param 		
 * @return void
 */
void CSEGDFile::WriteTraceHeaderExtension5(CSEGDTraceHeads*	pTrace)
{
	unsigned char byData[32];
	memset(byData,0xFF,32);
	// 1-4 字节，漏电界限
	m_unionType.fData = pTrace->m_fLeakageLimit;
	byData[0] = m_unionType.byData[3];
	byData[1] = m_unionType.byData[2];
	byData[2] = m_unionType.byData[1];
	byData[3] = m_unionType.byData[0];
	// 5-8 字节，漏电值（千欧）
	m_unionType.fData = pTrace->m_fLeakageVal;
	byData[4] = m_unionType.byData[3];
	byData[5] = m_unionType.byData[2];
	byData[6] = m_unionType.byData[1];
	byData[7] = m_unionType.byData[0];
	// 9-24字节未使用
	// 25字节，漏电误差
	if(pTrace->m_fLeakageVal<pTrace->m_fLeakageLimit)
		byData[24] = 1;
	else
		byData[24] = 0;
	// 26-32未用
	// memset(byData+25,0,7);
	if(m_pFile)
	{
		m_pFile->Write(byData,32);
	}
}
/**
 * @brief 向文件中写道头扩展数据块6
 * @note  
 * @param 		
 * @return void
 */
void CSEGDFile::WriteTraceHeaderExtension6(CSEGDTraceHeads*	pTrace,CSEGDScanTypeHeader* pScanTypeHeader)
{
	unsigned char byData[32];
	//memset(byData,0xFF,32);
	if(!pTrace || !pScanTypeHeader)
		return;
	// 1,设备类型
	byData[0] = pScanTypeHeader->m_byUnitType;	
	// 2-4,设备序列号
	m_unionType.dwData = pTrace->m_dwUnitSN;
	byData[1] = m_unionType.byData[2];
	byData[2] = m_unionType.byData[1];
	byData[3] = m_unionType.byData[0];
	// 5,地震道号
	byData[4] = pTrace->m_byChannelNb;
	// 6-8,备用
	byData[5] = 0;
	byData[6] = 0;
	byData[7] = 0;
	// 9，组件类型
	byData[8] = pTrace->m_byAssemblyType;
	// 10-12，FDU或者DSU组件序列号
	m_unionType.dwData = pTrace->m_dwFDUAssemblySN;
	byData[9] = m_unionType.byData[2];
	byData[10] = m_unionType.byData[1];
	byData[11] = m_unionType.byData[0];
	// 13，在FDU组件中位置
	byData[12] = pTrace->m_byLocationInFDUAssembly;
	// 14-16,备用
	byData[13] = 0;
	byData[14] = 0;
	byData[15] = 0;
	// 17，FDU/DSU设备类型
	byData[16] = pTrace->m_bySubunitType;
	// 18，地震道类型
	byData[17] = pTrace->m_byChannelType;
	// 19-20,备用
	byData[18] = 0;
	byData[19] = 0;
	// 21-24,检波器灵敏度
	memset(byData+20,0xFF,4);
	// 25-32,未使用
	memset(byData+24,0x0,8);
	if(m_pFile)
	{
		m_pFile->Write(byData,32);
	}
}
/**
 * @brief 向文件中写道头扩展数据块7
 * @note  
 * @param 		
 * @return void
 */
void CSEGDFile::WriteTraceHeaderExtension7(CSEGDTraceHeads*	pTrace,CSEGDScanTypeHeader* pScanTypeHeader)
{
	unsigned char byData[32];
	//memset(byData,0xFF,32);
	// 1,控制单元类型
	byData[0] = pTrace->m_byControlUnitType;
	// 2-4,控制单元序列号
	m_unionType.dwData = pTrace->m_dwControlUnitSN;
	byData[1] = m_unionType.byData[2];
	byData[2] = m_unionType.byData[1];
	byData[3] = m_unionType.byData[0];
	// 5，地震道增益刻度
	byData[4] = (BYTE)pScanTypeHeader->m_dwGain;
	// 6，地震道滤波器
	byData[5] = pTrace->m_byChannelFilter;
	// 7，地震道数据错误,
	byData[6] = pTrace->m_byChannelDataError;
	// 8,地震道编辑状态
	byData[7] = pTrace->m_byChannelEditedStatus;
	// 9-12，浮点数，地震道采样毫伏转换系数
	ReverseFloatToBigEndian(pTrace->m_fChannelConversionFactor,byData+8);
	// 13,有噪声叠加次数
	byData[12] = pTrace->m_byNumberOfStacksNoisy;
	// 14, 低叠加次数
	byData[13] = pTrace->m_byNumberOfStacksLow;
	// 15,地震道类型标识号：1 = 地震道;9 = 辅助道;在扫描类型头段对象中
	/*if(pScanTypeHeader->m_byChnType==CHN_AUXI)
		byData[14] = 0x09;
	else
		byData[14] = 0x01;*/
	byData[14] = pScanTypeHeader->m_byChnType;
	// 16,地震道处理:01=原始数据;	02=辅助道叠加;03=相关，负部分;04=相关，正部分;05=正态相关;06=地震道叠加
	byData[15] = pTrace->m_byChannelProcessType;
	// 17-20, 记录道最大值
	ReverseFloatToBigEndian(pTrace->m_fTraceMaxValue,byData+16);
	// 21-24,记录道最大时间,微秒
	ReverseDWORDToBigEndian(pTrace->m_dwTraceMaxtime,byData+20);
	// 25-28,内插次数	
	ReverseDWORDToBigEndian(pTrace->m_dwInterpolations,byData+24);
	// 29-32,Seismic trace offset value(if offset removal is disabled).
	ReverseDWORDToBigEndian(pTrace->m_dwSeismicTraceOffsetValue,byData+24);
	if(m_pFile)
	{
		m_pFile->Write(byData,32);
	}
}
/**
 * @brief 颠倒浮点数高低位
 * @note  颠倒浮点数高低位，高位在前，低位在后
 * @param 		
 * @return void
 */
void CSEGDFile::ReverseFloatToBigEndian(float fValue,BYTE* byData)
{
	if(!byData)
		return;
	m_unionType.fData = fValue;
	byData[0] = m_unionType.byData[3];
	byData[1] = m_unionType.byData[2];
	byData[2] = m_unionType.byData[1];
	byData[3] = m_unionType.byData[0];

}
/**
 * @brief 颠倒整数高低位
 * @note  颠倒整数高低位，高位在前，低位在后
 * @param 		
 * @return void
 */
void CSEGDFile::ReverseDWORDToBigEndian(DWORD dwValue,BYTE* byData)
{
	if(!byData)
		return;
	m_unionType.dwData = dwValue;
	byData[0] = m_unionType.byData[3];
	byData[1] = m_unionType.byData[2];
	byData[2] = m_unionType.byData[1];
	byData[3] = m_unionType.byData[0];

}
/**
 * @brief 将三个字节的整数转化为四个字节的浮点数格式，并高低字节互换
 * @note  将三个字节的整数转化为四个字节的浮点数格式，颠倒浮点数高低位，高位在前，低位在后
 * @param 		
 * @return void
 */
void CSEGDFile::Convert3ByteIntTo4ByteAntiFlt(BYTE* lpInData,BYTE* lpOutData)
{
	if(!lpInData || !lpOutData)
		return;
	
	DWORD	dwTemp=0;
	// 高位
	dwTemp = lpInData[2];
	dwTemp = dwTemp<<8;
	dwTemp = dwTemp+ lpInData[1];
	dwTemp = dwTemp<<8;
	// 最低位
	dwTemp = dwTemp+ lpInData[0];
	m_unionType.fData = (float)dwTemp;
	lpOutData[0] = m_unionType.byData[3];
	lpOutData[1] = m_unionType.byData[2];
	lpOutData[2] = m_unionType.byData[1];
	lpOutData[3] = m_unionType.byData[0];

}

/**
 * @brief 写SEGD文件
 * @note  该函数写完整的SEGD文件
 * @param 		
 * @return void
 */
void CSEGDFile::SaveSEGDHead(CSEGDScanTypeHeader* pScanTypeHeader)
{
	DWORD	dwTemp;
	// 普通头段1
	WriteGeneralHeader1();
	// 普通头段2
	WriteGeneralHeader2();
	// 普通头段3
	WriteGeneralHeader3();	
	// 扫描类型头段
	for (dwTemp=0;dwTemp<m_byChnSetsCount;dwTemp++)
	{
		WriteScanTypeHeader(pScanTypeHeader+dwTemp);
	}
	// 扩展头段
	WriteExtendedHeader();
	// 外部头段
	WriteExternalHeader();

}

/**
 * @brief 存储某一道的道头属性数据
 * @note  存储某一道的道头属性数据
 * @param 		
 * @return void
 */
void CSEGDFile::SaveSEGDTraceHead(CSEGDTraceHeads*	pTrace,CSEGDScanTypeHeader* pScanTypeHeader)
{
	// 多路解编道头
	WriteDemuxTraceHeader(pTrace);
	// 道头扩展
	WriteTraceHeaderExtension1(pTrace);
	WriteTraceHeaderExtension2(pTrace);
	WriteTraceHeaderExtension3(pTrace);
	WriteTraceHeaderExtension4(pTrace);
	WriteTraceHeaderExtension5(pTrace);
	WriteTraceHeaderExtension6(pTrace,pScanTypeHeader);
	WriteTraceHeaderExtension7(pTrace,pScanTypeHeader);
}
// =============================只有施工服务程序使用到下列函数======================================
#ifdef PROGRAM_OPERATIONSERVER
void	CSEGDFile::SetExtendedHeader(COperaThread* pThread,CChannelList* pChannelList)
{
	if(!pThread || !pChannelList)
		return;
	CShotClient* pShotClient = pThread->GetShotClient();
	/** 记录道总数*/
	m_ExtendedHeader.m_dwTraceCount=pShotClient->m_pSpread->GetCount();
	/** 辅助道总数*/
	m_ExtendedHeader.m_dwAuxesTraceCount=pShotClient->m_pSpread->GetAuxiCount();
	/** 有应答地址道总数*/
	m_ExtendedHeader.m_dwLiveSeisTraceCount=pShotClient->m_pSpread->TotalOfAcked();
	/** 无应答地址道总数*/
	// m_dwDeadSeisTraceCount=0;	

	/**震源类型:0为无震源；1为脉冲，2为可控*/
	m_ExtendedHeader.m_dwSourceType=pShotClient->m_pShotSource->m_dwSourceType;
	/** 炮号*/
	m_ExtendedHeader.m_dwShotNb=pShotClient->m_pShotPoint->m_dwShotNb;
	/** TB窗口时间*/
	m_ExtendedHeader.m_fTBWindowS=pShotClient->m_pProcessType->m_dwTBWindow/1000.0f;
	/** 采集排列的第一条测线 */
	m_ExtendedHeader.m_dwSpreadSFL =pShotClient->m_pShotPoint->m_dwSpreadSFL;
	/** 排列第一个接收检波器位置号码 */
	m_ExtendedHeader.m_dwSpreadSFN=pShotClient->m_pShotPoint->m_dwSpreadSFN;
	/** 采集排列编号 */
	m_ExtendedHeader.m_dwSpreadNb=pShotClient->m_pSpread->m_dwNb;
	/** 采集排列类型: 1为普通排列，2为绝对排列 */
	m_ExtendedHeader.m_dwSpreadType=pShotClient->m_pSpread->m_bySpreadType; 
	/** 测试记录类型Test record type:
	0 正常记录。1 野外噪声。2 野外脉冲。3 野外串音。4 仪器噪声。
	5 仪器畸变。6 仪器增益/ 相位。	7 仪器串音。	8 仪器共模。
	9 合成。	10 一致性（仅限 CM408）。11 仪器脉冲。12 野外畸变。
	13 仪器重力。14 野外漏电。15 野外电阻。*/
	m_ExtendedHeader.m_dwTestType=0;
	
	/** 时断信号Timebreak，微秒单位 */
	m_ExtendedHeader.m_dwConfirmationTBUS=DWORD(pThread->m_pShotCtrl->m_fConfirmedTB*1000);
	/** 井口时间,微秒为单位*/
	m_ExtendedHeader.m_dwUpholetimeUS = DWORD(pThread->m_pShotCtrl->m_fUpholeTime*1000);
	/** 爆炸机标识Blaster id */
	m_ExtendedHeader.m_dwBlasterID=1;		// ????????爆炸机道上位机的协议中解析，只有部分爆炸机协议中有该字段			
	/** 爆炸机状态 */
	m_ExtendedHeader.m_dwBlasterStatus=pThread->m_pShotCtrl->m_byBlasterState;
	

	/** 折射延时，毫秒ms*/
	if(pShotClient->m_pProcessType->m_byProcessType == PROCESS_IMPULSIVE)
		m_ExtendedHeader.m_dwRefractionDelayMS=((CProcessImpulsive*)(pShotClient->m_pProcessType))->m_dwRefractionDelay;
	else
		m_ExtendedHeader.m_dwRefractionDelayMS = 0;
	/** TB到T0时间，确认的TB时间减去反射时间,单位微秒*/
	m_ExtendedHeader.m_lTBToTStartUS = DWORD(pThread->m_pShotCtrl->m_fConfirmedTB*1000-m_ExtendedHeader.m_dwRefractionDelayMS*1000);// ???????????????????
	/** 内部时断信号，ITB 指示; 0为是；1为否*/
	if(pThread->m_pShotCtrl->m_fConfirmedTB<pShotClient->m_pProcessType->m_dwTBWindow)
		m_ExtendedHeader.m_dwInternalTimeBreak = 1;								// ???????????????????
	else
		m_ExtendedHeader.m_dwInternalTimeBreak = 0;
	
	/** 野外设备中预叠加，0为是，1为否；Prestack within field units*/
	m_ExtendedHeader.m_dwPrestack=1;											// ???????????????以下变量均需修改
	/**噪声抑制类型: 1 无;2花样叠加;3 历史;4 关闭*/
	m_ExtendedHeader.m_dwNoiseEliminationType=1;
	/** 低记录道百分比，0~100%; Low trace percentage*/
	m_ExtendedHeader.m_dwLowTracePercentage=0;
	/** 低记录道值;0-132db; Low trace value */
	m_ExtendedHeader.m_dwLowTraceValue=0;
	/** 窗口数目1-64; Number of windows (Div.)or Window length (Enhanced Div.) */
	m_ExtendedHeader.m_dwWindowLength=1;
	/** 历史编辑类型：1为归零；2为剪裁；*/
	m_ExtendedHeader.m_dwHistoricEditingType=1;
	/** 噪声记录道百分比：0 到100% ； Noisy trace percentage*/
	m_ExtendedHeader.m_dwNoisyTracePercentage=0;
	/** 历史范围0 到 36 dB;  Historic range */
	m_ExtendedHeader.m_dwHistoricRange=0;
	/** 历史锥形长度 2 的指数:0 到 8;Historic taper length 2’s	exponent*/
	m_ExtendedHeader.m_dwExpHistoricTaperLen=0;
	/** 门槛值不变/ 变化1 = 不变;2 = 变化; Threshold Hold/Var*/
	m_ExtendedHeader.m_dwThresholdHold=1;
	/**历史门槛值初始值0 到 132 dB; Historic threshold Init value*/
	m_ExtendedHeader.m_dwHistoricThresholdValue=0;
	/**历史归零长度1 到 500 毫秒; Historic zeroing length*/
	m_ExtendedHeader.m_dwHistoricZeroingLen=1;								// ??????????????????????????到此为止
	
	
	/**处理类型1 无操作（原始数据）;2 叠加;	3 叠加后相关;4 叠加前相关;5 原始数据，叠加;	6 原始数据，叠加后相关;7 原始数据，叠加前相关*/
	m_ExtendedHeader.m_dwProcessType=1;
	/** 采集类型表,32个数值，128字节*/
	// m_dwAcquisitionTypeTable[32];
	memset(m_ExtendedHeader.m_dwAcquisitionTypeTable,0,128);
	/**门槛值类型表32 个数值（128 字节）,Threshold type tables 32 values (128 bytes).*/
	// m_dwThresholdTypeTable[32];
	memset(m_ExtendedHeader.m_dwThresholdTypeTable,0,128);
	/**叠加次数,1~32; Stacking fold*/
	m_ExtendedHeader.m_dwStackingFold=1;
	/** 记录长度100 到 128000 毫秒;Record length*/
	m_ExtendedHeader.m_dwRecordLength=pShotClient->m_pProcessType->m_dwRecordLen;
	/** 自相关峰值时间1 到 128000 毫秒; Autocorrelation peak time */
	m_ExtendedHeader.m_dwAutoCorrPeakTime=1;
	/** 相关参考信号编号1 到 100000;Correlation Pilot No. 1 to 100000.*/
	m_ExtendedHeader.m_dwCorrelationPilot=1;
	/** 参考信号长度1000 到 128000 毫秒 ; Pilot length*/
	m_ExtendedHeader.m_dwPilotLength=1000;
	/** 扫描长度1000 到 128000 毫秒; Sweep length*/
	m_ExtendedHeader.m_dwSweepLength=1000;
	/** 采集号1 到 32; Acquisition number */
	m_ExtendedHeader.m_dwAcquisitionNb=1;
	/** 最多辅助道的最大值;Max of max, Aux*/
	m_ExtendedHeader.m_fMaxAux=0;
	/** 最多地震道最大值IEEE 格式，单精度;Max of max, Seis*/
	m_ExtendedHeader.m_fMaxSeis=0;
	/** 转储叠加次数1..32;Dump stacking fold*/
	m_ExtendedHeader.m_dwDumpStackingFold=1;
	/** 磁带标签ASCII 文本，16 个字符*/
	// m_byTapeLabel[16];
	memset(m_ExtendedHeader.m_byTapeLabel,0,16);
	/** 磁带号1 到 9999 */
	m_ExtendedHeader.m_dwTapeNb=1;
	/** 软件版本ASCII 文本，16 个字符; Software version ASCII text, 16 characters.*/
	//m_bySoftwareVersion[16];
	strcpy_s((char*)m_ExtendedHeader.m_bySoftwareVersion,16,"1.0");					// ??????????????软件版本
	/** 日期ASCII 文本，12 个字符(dd mmm yyyy)*/
	//m_byDate[12];
	strcpy_s((char*)m_ExtendedHeader.m_bySoftwareVersion,12,"01  01 2011");
	/** 震源偏东距; Source easting*/
	m_ExtendedHeader.m_dbSourceEasting=0;
	/** 震源北向纬度差; Source northing*/
	m_ExtendedHeader.m_dbSourceNorthing=0;
	/** 震源高程; Source elevation*/
	m_ExtendedHeader.m_fSourceElevation=0;
	/** 所用的滑动扫描模式： 0为否;1为是 */
	m_ExtendedHeader.m_dwSlipSweepMode=0;
	/** 每个磁带的文件数1 到 9999*/
	m_ExtendedHeader.m_dwFilesPerTape=1;
	/** 文件计数1 到 9999*/
	m_ExtendedHeader.m_dwFileCount=1;
	/** 采集错误说明ASCII 文本，160 个字符*/
	//m_byAcquisitErrorDesc[160];
	memset(m_ExtendedHeader.m_byAcquisitErrorDesc,0,160);
	/** 滤波器类型: 1 =0.8最小; 2= 0.8 线性; */
	m_ExtendedHeader.m_dwFilterType=1;
	/** 叠加已转储: 	0 = No; 1 = Yes.;*/
	m_ExtendedHeader.m_dwStackIsDumped=0;
	/** 叠加符号（当前） 0 = 否;1 = 正;2 = 负*/
	m_ExtendedHeader.m_dwStackSign=0;
	/** 所用的 PRM 倾斜度校正0 = 否;1为是; PRM Tilt Correction used*/
	m_ExtendedHeader.m_dwPRMTiltCorrect=0;
	/** 线束名;ASCII 文本，64个字符; */
	//m_bySwathName[64];
	strcpy_s((char*)m_ExtendedHeader.m_bySwathName,64,"SW1");
	/**施工模式bit0 = 1 Standard.bit1 = 1 Microseismic.bit2 = 1 Slip-sweep.	bit3 = 1 SQC Dump (VSR).bit4 = 1 Guidance(Navigation).*/
	m_ExtendedHeader.m_dwOperatingMode=1;
	/** 无记录0 = 否;1 = 是*/
	m_ExtendedHeader.m_dwNoLog=0;
	/** 监听时间100 到 128000毫秒; Listening time 100 to 128000 ms.*/
	m_ExtendedHeader.m_dwListeningTime=1000;
	/** 转储类型0 = 正常转储;1 = 原始转储;2 = 附加转储;*/
	m_ExtendedHeader.m_dwDumpType=0;
	/** 线束 Id; Swath ID*/
	m_ExtendedHeader.m_dwSwathId=1;
	
	/** Seismic trace offset removal is disabled; 0 = No (i.e.offset is removed);1 = Yes (Microseismic mode); Sercel 4.0版本增加*/
	m_ExtendedHeader.m_dwSeismicTaceOffsetRemoval=0;
	
}
/**
 * @brief 得到辅助道扫描类型组的索引位置数组
 * @note  从采集排列中找出所有的辅助道，并记录在采集排列数组中位置，写入arr*Scantype变量中
 当前仅定义四组扫描类型头段：辅助道+增益g1、辅助道+增益g2、采集道+增益g1、采集道+增益g2
 * @param 		
 * @return void
 */
void	CSEGDFile::GetScanTypeOfSpread(CShotSpread* pSpread,CDWordArray& arrAuxiG1Scantype,
			CDWordArray& arrAuxiG2Scantype,CDWordArray& arrSeisG1Scantype,CDWordArray& arrSeisG2Scantype)
{
	if(!pSpread)
		return;
	int	i;
	int nCount = pSpread->GetCount();
	for (i=0;i<nCount;i++)
	{
		if(pSpread->m_arrSpread[i].m_byType==CHN_AUXI)
		{
			 if(pSpread->m_arrSpread[i].m_dwGain==1)
				 arrAuxiG1Scantype.Add(i);
			 else if( pSpread->m_arrSpread[i].m_dwGain==4)
				 arrAuxiG2Scantype.Add(i);
		}
		else
		{
			if(pSpread->m_arrSpread[i].m_dwGain==1)
				arrSeisG1Scantype.Add(i);
			else if( pSpread->m_arrSpread[i].m_dwGain==4)
				arrSeisG2Scantype.Add(i);
		}
	}
}
void    CSEGDFile::WriteAuxiTraceHeader(int nChnSetNb,int nTraceIndexInSpread,CSEGDScanTypeHeader* pScanTypeHeader,COperaThread* pThread,CChannelList* pChannelList)
{
	CSEGDTraceHeads		TraceHeads;
	CShotSpread*	pSpread = pThread->GetShotClient()->m_pSpread;
	/** 地震道组号*/
	TraceHeads.m_byChnSetNb=nChnSetNb;
	/** 记录道编号*/
	TraceHeads.m_dwTraceNb=nTraceIndexInSpread;
	/** 记录道编辑;00:未进行编辑。02:采集前静噪或停滞。03: 由采集系统编辑*/
	TraceHeads.m_byTraceEdit=0;

	/** 接收点测线号*/
	TraceHeads.m_dwLineNb=0;
	/** 接收点号*/
	TraceHeads.m_dwPointNb=0;
	/** 接收点索引*/
	TraceHeads.m_byPointIndex=BYTE(pSpread->m_arrSpread[nTraceIndexInSpread].m_dwChannelNb);
	
	/** 检波器SEGD代码,在Line程序中设置*/
	TraceHeads.m_bySensorSEGDCode=0;

	/** 接收点偏东距*/
	TraceHeads.m_dbPointEasting=0;
	/** 接收点北向纬度*/
	TraceHeads.m_dbPointNorthing=0;
	/** 接收点高程*/
	TraceHeads.m_fPointElevation=0;
	/** 传感器类型编号*/
	TraceHeads.m_bySensorType=1;

	/** 电阻值上限*/
	TraceHeads.m_fResistanceMax=0;
	/** 电阻值上限*/
	TraceHeads.m_fResistanceMin=0;
	/** 电阻值*/
	TraceHeads.m_fResistanceVal=0;
	/** 最大陆上检波器倾斜度百分比*/
	TraceHeads.m_fTiltMax=0;
	/** 倾斜度值*/
	TraceHeads.m_fTiltVal=0;

	/** 电容上限*/
	TraceHeads.m_fCapacitanceMax=0;
	/** 电容下限*/
	TraceHeads.m_fCapacitanceMin=0;
	/** 电容值，纳法拉*/
	TraceHeads.m_fCapacitanceVal=0;
	/** 截止上限*/
	TraceHeads.m_fCutoffFreqMax=0;
	/** 截止下限*/
	TraceHeads.m_fCutoffFreqMin=0;
	/** 截止值*/
	TraceHeads.m_fCutoffFreqVal=0;
	/** 漏电界限*/
	TraceHeads.m_fLeakageLimit=0;
	/** 漏电值*/
	TraceHeads.m_fLeakageVal=0;

	/**设备类型, 仪器类型：1为FDU；0x1C表示DSU428 */
	// BYTE	m_byUnitType;由扫描类型头段的变量说明
	/** 设备序列号*/
	TraceHeads.m_dwUnitSN=pSpread->m_arrSpread[nTraceIndexInSpread].m_dwPointNb;
	/** 地震道编号*/
	TraceHeads.m_byChannelNb=BYTE(pSpread->m_arrSpread[nTraceIndexInSpread].m_dwChannelNb);
	/**组件类型*/
	TraceHeads.m_byAssemblyType=1;
	/** FDU 或 DSU 组件序列号*/
	TraceHeads.m_dwFDUAssemblySN=0;
	TraceHeads.m_byLocationInFDUAssembly = 0;
	/** 子单元类型*/
	TraceHeads.m_bySubunitType=0x01;
	/** 地震道类型:0 检波器;1 压敏检波器*/
	TraceHeads.m_byChannelType=0;
	/** 检波器灵敏度*/

	/** 控制单元类型:0x01 LCI408;0x02 LAUL408;0x03 LAUX408;0x04 LAULS408;0x05 LAUXS408;0x06 LAUL428;0x16 LRU;0x17 LAUR428;0x30 LAUX428;0x31 LCI428*/
	TraceHeads.m_byControlUnitType=0x31;
	/** 控制单元序列号*/
	TraceHeads.m_dwControlUnitSN=0;
	/** 地震道增益刻度*/
	TraceHeads.m_byGainScale=1;
	/** 地震道滤波器*/
	TraceHeads.m_byChannelFilter=1;
	/** 地震道数据误差*/
	TraceHeads.m_byChannelDataError=0;
	/** 地震道编辑状态,1 停滞;2 采集/ 回传错误;3 噪声编辑*/
	TraceHeads.m_byChannelEditedStatus=1;
	/** 地震道采样毫伏转换系数,辅助道为0*/
	TraceHeads.m_fChannelConversionFactor=0;
	/** 有噪声叠加次数*/
	TraceHeads.m_byNumberOfStacksNoisy=0;
	/** 低叠加次数*/
	TraceHeads.m_byNumberOfStacksLow=0;
	/** 地震道类型标识号：1 = 地震道;9 = 辅助道;在扫描类型头段对象中*/

	/** 地震道处理:01=原始数据;	02=辅助道叠加;03=相关，负部分;04=相关，正部分;05=正态相关;06=地震道叠加*/
	TraceHeads.m_byChannelProcessType=01;
	/** 记录道最大值*/
	TraceHeads.m_fTraceMaxValue=0;
	/** 记录道最大时间,微秒*/
	TraceHeads.m_dwTraceMaxtime=0;
	/** 内插次数*/
	TraceHeads.m_dwInterpolations=0;
	/**Seismic trace offset value(if offset removal is disabled).*/
	TraceHeads.m_dwSeismicTraceOffsetValue=0;

	if(m_pFile)
	{
		SaveSEGDTraceHead(&TraceHeads,pScanTypeHeader);
	}
}
void    CSEGDFile::WriteSeisTraceHeader(int nChnSetNb,int nTraceIndexInSpread,CSEGDScanTypeHeader* pScanTypeHeader,COperaThread* pThread,CChannelList* pChannelList)
{
	CSEGDTraceHeads		TraceHeads;
	CShotSpread*	pSpread = pThread->GetShotClient()->m_pSpread;
	/** 地震道组号*/
	TraceHeads.m_byChnSetNb=nChnSetNb;
	/** 记录道编号*/
	TraceHeads.m_dwTraceNb=nTraceIndexInSpread;
	/** 记录道编辑;00:未进行编辑。02:采集前静噪或停滞。03: 由采集系统编辑*/
	TraceHeads.m_byTraceEdit=0;

	/** 接收点测线号*/
	TraceHeads.m_dwLineNb=pSpread->m_arrSpread[nTraceIndexInSpread].m_dwLineNb;
	/** 接收点号*/
	TraceHeads.m_dwPointNb=pSpread->m_arrSpread[nTraceIndexInSpread].m_dwPointNb;
	/** 接收点索引*/
	TraceHeads.m_byPointIndex=BYTE(pSpread->m_arrSpread[nTraceIndexInSpread].m_dwChannelNb);

	CChannelSetupData*	pChnData=pChannelList->GetChannel(TraceHeads.m_dwLineNb,TraceHeads.m_dwPointNb,TraceHeads.m_byPointIndex);
	if(pChnData)
	{
		/** 检波器SEGD代码,在Line程序中设置*/
		TraceHeads.m_bySensorSEGDCode=pChnData->m_uiSegdCode;

		/** 接收点偏东距*/
		TraceHeads.m_dbPointEasting=0;
		/** 接收点北向纬度*/
		TraceHeads.m_dbPointNorthing=0;
		/** 接收点高程*/
		TraceHeads.m_fPointElevation=0;
		/** 传感器类型编号*/
		TraceHeads.m_bySensorType=pChnData->m_uiSensorNb;

		/** 电阻值上限*/
		TraceHeads.m_fResistanceMax=pChnData->m_fResistanceLimitMax;
		/** 电阻值上限*/
		TraceHeads.m_fResistanceMin=pChnData->m_fResistanceLimitMin;
		/** 电阻值*/
		TraceHeads.m_fResistanceVal=pChnData->m_fResistanceValue;
		/** 最大陆上检波器倾斜度百分比*/
		TraceHeads.m_fTiltMax=pChnData->m_fTiltLimit;
		/** 倾斜度值*/
		TraceHeads.m_fTiltVal=pChnData->m_fTiltValue;

		/** 电容上限*/
		TraceHeads.m_fCapacitanceMax=0;
		/** 电容下限*/
		TraceHeads.m_fCapacitanceMin=0;
		/** 电容值，纳法拉*/
		TraceHeads.m_fCapacitanceVal=0;
		/** 截止上限*/
		TraceHeads.m_fCutoffFreqMax=0;
		/** 截止下限*/
		TraceHeads.m_fCutoffFreqMin=0;
		/** 截止值*/
		TraceHeads.m_fCutoffFreqVal=0;
		/** 漏电界限*/
		TraceHeads.m_fLeakageLimit=pChnData->m_fLeakageLimit;
		/** 漏电值*/
		TraceHeads.m_fLeakageVal=pChnData->m_fLeakageValue;

		/**设备类型, 仪器类型：1为FDU；0x1C表示DSU428 */
		// BYTE	m_byUnitType;由扫描类型头段的变量说明
		/** 设备序列号*/
		TraceHeads.m_dwUnitSN=pChnData->m_uiSN;
		/** 地震道编号*/
		TraceHeads.m_byChannelNb= BYTE(pChnData->m_uiNbChannel);
		/**组件类型*/
		TraceHeads.m_byAssemblyType=1;
		/** FDU 或 DSU 组件序列号*/
		TraceHeads.m_dwFDUAssemblySN=0;
		TraceHeads.m_byLocationInFDUAssembly = 0;
		/** 子单元类型*/
		TraceHeads.m_bySubunitType=0x01;
		/** 地震道类型:0 检波器;1 压敏检波器*/
		TraceHeads.m_byChannelType=0;
		/** 检波器灵敏度*/

		/** 控制单元类型:0x01 LCI408;0x02 LAUL408;0x03 LAUX408;0x04 LAULS408;0x05 LAUXS408;0x06 LAUL428;0x16 LRU;0x17 LAUR428;0x30 LAUX428;0x31 LCI428*/
		TraceHeads.m_byControlUnitType=0x31;
		/** 控制单元序列号*/
		TraceHeads.m_dwControlUnitSN=0;
		/** 地震道增益刻度*/
		if(pSpread->m_arrSpread[nTraceIndexInSpread].m_dwGain ==1)
			TraceHeads.m_byGainScale=1;
		else
			TraceHeads.m_byGainScale=2;
		/** 地震道滤波器*/
		TraceHeads.m_byChannelFilter=1;
		/** 地震道数据误差*/
		TraceHeads.m_byChannelDataError=0;
		/** 地震道编辑状态,1 停滞;2 采集/ 回传错误;3 噪声编辑*/
		TraceHeads.m_byChannelEditedStatus=1;
		/** 地震道采样毫伏转换系数,辅助道为0*/
		TraceHeads.m_fChannelConversionFactor=0;
		/** 有噪声叠加次数*/
		TraceHeads.m_byNumberOfStacksNoisy=0;
		/** 低叠加次数*/
		TraceHeads.m_byNumberOfStacksLow=0;
		/** 地震道类型标识号：1 = 地震道;9 = 辅助道;在扫描类型头段对象中*/

		/** 地震道处理:01=原始数据;	02=辅助道叠加;03=相关，负部分;04=相关，正部分;05=正态相关;06=地震道叠加*/
		TraceHeads.m_byChannelProcessType=01;
		/** 记录道最大值*/
		TraceHeads.m_fTraceMaxValue=0;
		/** 记录道最大时间,微秒*/
		TraceHeads.m_dwTraceMaxtime=0;
		/** 内插次数*/
		TraceHeads.m_dwInterpolations=0;
		/**Seismic trace offset value(if offset removal is disabled).*/
		TraceHeads.m_dwSeismicTraceOffsetValue=0;
	}
	else
	{
		/** 检波器SEGD代码,在Line程序中设置*/
		TraceHeads.m_bySensorSEGDCode=0;

		/** 接收点偏东距*/
		TraceHeads.m_dbPointEasting=0;
		/** 接收点北向纬度*/
		TraceHeads.m_dbPointNorthing=0;
		/** 接收点高程*/
		TraceHeads.m_fPointElevation=0;
		/** 传感器类型编号*/
		TraceHeads.m_bySensorType=1;

		/** 电阻值上限*/
		TraceHeads.m_fResistanceMax=0;
		/** 电阻值上限*/
		TraceHeads.m_fResistanceMin=0;
		/** 电阻值*/
		TraceHeads.m_fResistanceVal=0;
		/** 最大陆上检波器倾斜度百分比*/
		TraceHeads.m_fTiltMax=0;
		/** 倾斜度值*/
		TraceHeads.m_fTiltVal=0;

		/** 电容上限*/
		TraceHeads.m_fCapacitanceMax=0;
		/** 电容下限*/
		TraceHeads.m_fCapacitanceMin=0;
		/** 电容值，纳法拉*/
		TraceHeads.m_fCapacitanceVal=0;
		/** 截止上限*/
		TraceHeads.m_fCutoffFreqMax=0;
		/** 截止下限*/
		TraceHeads.m_fCutoffFreqMin=0;
		/** 截止值*/
		TraceHeads.m_fCutoffFreqVal=0;
		/** 漏电界限*/
		TraceHeads.m_fLeakageLimit=0;
		/** 漏电值*/
		TraceHeads.m_fLeakageVal=0;

		/**设备类型, 仪器类型：1为FDU；0x1C表示DSU428 */
		// BYTE	m_byUnitType;由扫描类型头段的变量说明
		/** 设备序列号*/
		TraceHeads.m_dwUnitSN=0;
		/** 地震道编号*/
		TraceHeads.m_byChannelNb=BYTE(pSpread->m_arrSpread[nTraceIndexInSpread].m_dwChannelNb);
		/**组件类型*/
		TraceHeads.m_byAssemblyType=1;
		/** FDU 或 DSU 组件序列号*/
		TraceHeads.m_dwFDUAssemblySN=0;
		TraceHeads.m_byLocationInFDUAssembly = 0;
		/** 子单元类型*/
		TraceHeads.m_bySubunitType=0x01;
		/** 地震道类型:0 检波器;1 压敏检波器*/
		TraceHeads.m_byChannelType=0;
		/** 检波器灵敏度*/

		/** 控制单元类型:0x01 LCI408;0x02 LAUL408;0x03 LAUX408;0x04 LAULS408;0x05 LAUXS408;0x06 LAUL428;0x16 LRU;0x17 LAUR428;0x30 LAUX428;0x31 LCI428*/
		TraceHeads.m_byControlUnitType=0x31;
		/** 控制单元序列号*/
		TraceHeads.m_dwControlUnitSN=0;
		/** 地震道增益刻度*/
		TraceHeads.m_byGainScale=1;
		/** 地震道滤波器*/
		TraceHeads.m_byChannelFilter=1;
		/** 地震道数据误差*/
		TraceHeads.m_byChannelDataError=0;
		/** 地震道编辑状态,1 停滞;2 采集/ 回传错误;3 噪声编辑*/
		TraceHeads.m_byChannelEditedStatus=1;
		/** 地震道采样毫伏转换系数,辅助道为0*/
		TraceHeads.m_fChannelConversionFactor=0;
		/** 有噪声叠加次数*/
		TraceHeads.m_byNumberOfStacksNoisy=0;
		/** 低叠加次数*/
		TraceHeads.m_byNumberOfStacksLow=0;
		/** 地震道类型标识号：1 = 地震道;9 = 辅助道;在扫描类型头段对象中*/

		/** 地震道处理:01=原始数据;	02=辅助道叠加;03=相关，负部分;04=相关，正部分;05=正态相关;06=地震道叠加*/
		TraceHeads.m_byChannelProcessType=01;
		/** 记录道最大值*/
		TraceHeads.m_fTraceMaxValue=0;
		/** 记录道最大时间,微秒*/
		TraceHeads.m_dwTraceMaxtime=0;
		/** 内插次数*/
		TraceHeads.m_dwInterpolations=0;
		/**Seismic trace offset value(if offset removal is disabled).*/
		TraceHeads.m_dwSeismicTraceOffsetValue=0;
	}
	if(m_pFile)
	{
		SaveSEGDTraceHead(&TraceHeads,pScanTypeHeader);
	}
}
/**
 * @brief 写入某一道的一次发送的采样数据
 * @note  某一采集道每次采样为24bits的整数，完成74次采样后，获得74*3=222Bytes的数据，放入一个上行数据包中传送给主机。
 该函数将222Bytes的数据格式由每三个字节的整数转化为浮点数，并写入SEGD文件中。
 * @param BYTE* lpInData,采样数据
  * @param int nSize，缓冲区字节大小，一般是3的倍数，缺省为222		
 * @return void
 */
void CSEGDFile::WriteTraceDataOfAFrame(BYTE* lpInData,int nSize)
{
	BYTE   byData[4];
	int i,nCount = nSize/3;
	if(!m_pFile)
		return;
	for (i=0;i<nCount;i++)
	{
		Convert3ByteIntTo4ByteAntiFlt(lpInData+3*i,byData);
		m_pFile->Write(byData,4);		
	}	
}
/**
 * @brief 将某一道数据的多路编排方式转换为SEGD2.1的反多路编排方式
 * @note  
 * @param int nTraceIndexInSpread,
 * @param int nTraceCountOfSpread,
 * @param CFile*	pFileData,	
 * @return void
 */
void	CSEGDFile::WriteTraceData(int nTraceIndexInSpread,int nTraceCountOfSpread,CFile*	pFileData)
{
	if(!pFileData)
		return;
	int   i;
	int   nNumberOfSamples = GetNumberOfSamples();
	ULONGLONG	  nFileLen = pFileData->GetLength();
	int   nFilePos;
	BYTE	pBuf[SAMPFRM_DATALEN];
	for(i=0;i<nNumberOfSamples;i++)
	{
		nFilePos = nTraceIndexInSpread +i*nTraceCountOfSpread;
		nFilePos *=SAMPFRM_DATALEN;
		if(nFilePos+222>=nFileLen)
			break;
		pFileData->Seek(nFilePos,CFile::begin);
		pFileData->Read(pBuf,SAMPFRM_DATALEN);
		WriteTraceDataOfAFrame(pBuf,SAMPFRM_DATALEN);
	}
}
/**
 * @brief 按照1.0版本的格式写SEGD 文件
 * @note  该函数写完整的SEGD文件，按照1.0版本的格式写数据文件
 * @param 		
 * @return void
 */
bool CSEGDFile::SaveSEGDRev21(CFile* pFileData,COperaThread* pThread,CChannelList* pChannelList)
{
	if(!pFileData || !pThread || !pChannelList)
		return false;  
	CShotClient*	pShotClient = pThread->GetShotClient();
	if(!pShotClient)
		return false;
	
	// --------------------------先创建文件-------------------------------------------------------------
	CString		strFileName;
	pShotClient->m_strSEGDFileName.Format(_T("%d.SEGD"),pShotClient->m_dwFileNumber);		// 原码文件名
	strFileName = pShotClient->m_strNormalFilePath +_T("\\")+pShotClient->m_strSEGDFileName;	// 文件路径
	CFile		SegdFile;
	// 创建新数据文件，先存储数据原码
	if(!SegdFile.Open(strFileName,CFile::modeCreate|CFile::modeWrite))
	{
		// 创建文件错误,通知主机
		TRACE1("Failed to Create file %s !",strFileName);
		return false;
	}
	m_pFile = &SegdFile;

	// -----------------------------------------------------------------------------------------------
	/** 文件编号 */
	m_dwFileNumber = pShotClient->m_dwFileNumber;	
	/** 制造商代码，默认13*/
	m_byManufacturerCode=13;
	/** 制造商序列号，默认0*/
	m_wManufacturerSN =0;
	/** 每个记录的地震道组数量。默认16,修改后对应增加CSEGDScanTypeHeader数组个数*/
	m_byChnSetsCount = 16;
	/** 外部头段大小，在Config程序配置中读取*/
	m_wExternalHeaderSize = 1024;						// ????????????????? 从Config程序中读取

	/** 基本扫描间隔，采样率，以微秒为单位，一般取值：250,500,1000,2000,4000 */
	m_dwSampleRateUS = pShotClient->m_pFDUConfig->GetSampleRateByUS();
	/** 采样长度，以ms为单位，取值0-128000ms */
	m_dwRecordLenMS = pShotClient->m_pProcessType->m_dwRecordLen;
	/** 采样类型,取值：8为正常采样；2为测试采样 */
	m_byRecordType = 8;
	/** 震源测线号，小数点后两位 */
	m_fSourceLine = pShotClient->m_pShotPoint->m_fSourceLine;
	/** 震源点号，小数点后两位 */
	m_fSourceNb = pShotClient->m_pShotPoint->m_fSourceNb;
	/** 震源点索引 */
	m_dwSourcePointIndex = pShotClient->m_pShotPoint->m_dwSourcePointIndex;	// ?????????????????
	/** 震源组号 */
	m_dwFleetNb = pShotClient->m_pShotSource->m_dwShooterNb;	// ?????????????????
	
	// -----------------------------------------------------------------------------------------------
	// 对采集排列的采集道进行分组，定义不同的扫描类型头段。
	// 当前仅定义四组扫描类型头段：辅助道+增益g1、辅助道+增益g2、采集道+增益g1、采集道+增益g2
	CDWordArray		arrAuxiG1ScanType;
	CDWordArray		arrAuxiG2ScanType;
	CDWordArray		arrSeisG1ScanType;
	CDWordArray		arrSeisG2ScanType;
	//GetAuxiGain1ScanType(pShotClient->m_pSpread,arrAuxiG1ScanType);
	//GetAuxiGain2ScanType(pShotClient->m_pSpread,arrAuxiG2ScanType);
	//GetSeisGain1ScanType(pShotClient->m_pSpread,arrSeisG1ScanType);
	//GetSeisGain2ScanType(pShotClient->m_pSpread,arrSeisG2ScanType);
	GetScanTypeOfSpread(pShotClient->m_pSpread,arrAuxiG1ScanType,arrAuxiG2ScanType,arrSeisG1ScanType,arrSeisG2ScanType);
	// -----------------------------------------------------------------------------------------------
	CSEGDScanTypeHeader* pScanTypeHeader =new CSEGDScanTypeHeader[m_byChnSetsCount];
	// 创建扫描类型头段1: 辅助道+增益G1	
	// 地震道组号
	pScanTypeHeader[0].m_byChnSetNb =1;
	/** 该组通道的道数*/ 
	pScanTypeHeader[0].m_wChnCount=arrAuxiG1ScanType.GetCount();
	/** 通道类型: 1为地震道(含哑道），9为辅助道;*/
	pScanTypeHeader[0].m_byChnType=9;
	/** 通道组开始时间，对于辅助道而言为0,；采集道等于折射延迟时间。 2ms为增量，因此取值应该是实际开始时间的一半*/
	pScanTypeHeader[0].m_wChnStartingTime = 0;											// ?????????????????????????
	/* 通道组结束时间，2ms为增量，因此取值应该是通道开始时间加上采样长度的一半。*/
	pScanTypeHeader[0].m_wChnEndTime = WORD(pShotClient->m_pProcessType->m_dwRecordLen/2);	// ?????????????????????????
	/** 仪器类型：0x00未确定；0x01为FDU；0x1C表示DSU428；0x20，VE464*/
	pScanTypeHeader[0].m_byUnitType = 0x01;												// ?????????????????????????
	/** 放大增益,取值：1,2,4,8,16,32,64,其中1对应Sercel的g1设置,4对应为g2*/
	pScanTypeHeader[0].m_dwGain = 1;													// ?????????????????????????
	/** 垂直叠加(1字节，无符号二进制数)。默认为0。有效的叠加次数，
	   如果故意将道数据置为0，则垂直叠加置为0。如果未叠加，置为1。如
		果数据是叠加的结果(处理或者未处理)，置为有效叠加次数*/
	pScanTypeHeader[0].m_byVerticalStack = 1;
	// -----------------------------------------------------------------------------------------------
	// 创建扫描类型头段2: 辅助道+增益G2	
	// 地震道组号
	pScanTypeHeader[1].m_byChnSetNb =2;
	/** 该组通道的道数*/ 
	pScanTypeHeader[1].m_wChnCount=arrAuxiG2ScanType.GetCount();
	/** 通道类型: 1为地震道(含哑道），9为辅助道;*/
	pScanTypeHeader[1].m_byChnType=9;
	/** 通道组开始时间，对于辅助道而言为0,；采集道等于折射延迟时间。 2ms为增量，因此取值应该是实际开始时间的一半*/
	pScanTypeHeader[1].m_wChnStartingTime = 0;										// ?????????????????????????
	/* 通道组结束时间，2ms为增量，因此取值应该是通道开始时间加上采样长度的一半。*/
	pScanTypeHeader[1].m_wChnEndTime = WORD(pShotClient->m_pProcessType->m_dwRecordLen/2);	// ?????????????????????????
	/** 仪器类型：0x00未确定；0x01为FDU；0x1C表示DSU428；0x20，VE464*/
	pScanTypeHeader[1].m_byUnitType = 0x01;											// ?????????????????????????
	/** 放大增益,取值：1,2,4,8,16,32,64,其中1对应Sercel的g1设置,4对应为g2*/
	pScanTypeHeader[1].m_dwGain = 4;												// ?????????????????????????
	/** 垂直叠加(1字节，无符号二进制数)。默认为0。有效的叠加次数，
	   如果故意将道数据置为0，则垂直叠加置为0。如果未叠加，置为1。如
		果数据是叠加的结果(处理或者未处理)，置为有效叠加次数*/
	pScanTypeHeader[1].m_byVerticalStack = 1;
	// -----------------------------------------------------------------------------------------------
	// 创建扫描类型头段3: 地震道+增益G1	
	// 地震道组号
	pScanTypeHeader[2].m_byChnSetNb =3;
	/** 该组通道的道数*/ 
	pScanTypeHeader[2].m_wChnCount=arrSeisG1ScanType.GetCount();
	/** 通道类型: 1为地震道(含哑道），9为辅助道;*/
	pScanTypeHeader[2].m_byChnType=1;
	/** 通道组开始时间，对于辅助道而言为0,；采集道等于折射延迟时间。 2ms为增量，因此取值应该是实际开始时间的一半*/
	pScanTypeHeader[2].m_wChnStartingTime = 0;										// ?????????????????????????
	/* 通道组结束时间，2ms为增量，因此取值应该是通道开始时间加上采样长度的一半。*/
	pScanTypeHeader[2].m_wChnEndTime = WORD(pShotClient->m_pProcessType->m_dwRecordLen/2);	// ?????????????????????????
	/** 仪器类型：0x00未确定；0x01为FDU；0x1C表示DSU428；0x20，VE464*/
	pScanTypeHeader[2].m_byUnitType = 0x01;											// ?????????????????????????
	/** 放大增益,取值：1,2,4,8,16,32,64,其中1对应Sercel的g1设置,4对应为g2*/
	pScanTypeHeader[2].m_dwGain = 1;												// ?????????????????????????
	/** 垂直叠加(1字节，无符号二进制数)。默认为0。有效的叠加次数，
	   如果故意将道数据置为0，则垂直叠加置为0。如果未叠加，置为1。如
		果数据是叠加的结果(处理或者未处理)，置为有效叠加次数*/
	pScanTypeHeader[2].m_byVerticalStack = 1;
	// -----------------------------------------------------------------------------------------------
	// 创建扫描类型头段4: 地震道+增益G2	
	// 地震道组号
	pScanTypeHeader[3].m_byChnSetNb = 4;
	/** 该组通道的道数*/ 
	pScanTypeHeader[3].m_wChnCount=arrSeisG2ScanType.GetCount();
	/** 通道类型: 1为地震道(含哑道），9为辅助道;*/
	pScanTypeHeader[3].m_byChnType=1;
	/** 通道组开始时间，对于辅助道而言为0,；采集道等于折射延迟时间。 2ms为增量，因此取值应该是实际开始时间的一半*/
	pScanTypeHeader[3].m_wChnStartingTime = 0;										// ?????????????????????????
	/* 通道组结束时间，2ms为增量，因此取值应该是通道开始时间加上采样长度的一半。*/
	pScanTypeHeader[3].m_wChnEndTime = WORD(pShotClient->m_pProcessType->m_dwRecordLen/2);	// ?????????????????????????
	/** 仪器类型：0x00未确定；0x01为FDU；0x1C表示DSU428；0x20，VE464*/
	pScanTypeHeader[3].m_byUnitType = 0x01;											// ?????????????????????????
	/** 放大增益,取值：1,2,4,8,16,32,64,其中1对应Sercel的g1设置,4对应为g2*/
	pScanTypeHeader[3].m_dwGain = 4;												// ?????????????????????????
	/** 垂直叠加(1字节，无符号二进制数)。默认为0。有效的叠加次数，
	   如果故意将道数据置为0，则垂直叠加置为0。如果未叠加，置为1。如
		果数据是叠加的结果(处理或者未处理)，置为有效叠加次数*/
	pScanTypeHeader[3].m_byVerticalStack = 1;
	// ----------------------------------------------------------------------------------------------------
	// 扩展头段
	SetExtendedHeader(pThread,pChannelList);
	// ----------------------------------------------------------------------------------------------------
	// 外部头段
	/** 放炮或者导航系统信息 */
	// m_strShooterInfo;								// ?????????????????????????
	/** 来自Config程序设置的用户信息*/
	m_strConfigUserInfo;								// ?????????????????????????
	/** 施工窗口的震源注释*/
	m_strSourceComment = pShotClient->m_pShotSource->m_strComment;
	//-----------------------------------------------------------------------------------------------------
	// 保存整个头段数据
	SaveSEGDHead(pScanTypeHeader);

	//------------------------------------------------------------------------------------------------------
	// 保存各道的数据
	INT_PTR		i;
	INT_PTR		nCount;
	// 第一个扫描类型：辅助道，增益为G1
	nCount = arrAuxiG1ScanType.GetCount();
	for(i=0;i<nCount;i++)
	{
		WriteAuxiTraceHeader(1,arrAuxiG1ScanType[i],pScanTypeHeader,pThread,pChannelList);
		WriteTraceData(arrAuxiG1ScanType[i],m_ExtendedHeader.m_dwTraceCount,pFileData);
	}
	// 第二个扫描类型：辅助道，增益为G2
	nCount = arrAuxiG2ScanType.GetCount();
	for(i=0;i<nCount;i++)
	{
		WriteAuxiTraceHeader(2,arrAuxiG2ScanType[i],pScanTypeHeader+1,pThread,pChannelList);
		WriteTraceData(arrAuxiG2ScanType[i],m_ExtendedHeader.m_dwTraceCount,pFileData);
	}
	// 第三个扫描类型：地震道，增益为G1
	nCount = arrSeisG1ScanType.GetCount();
	for(i=0;i<nCount;i++)
	{
		WriteAuxiTraceHeader(3,arrSeisG1ScanType[i],pScanTypeHeader+2,pThread,pChannelList);
		WriteTraceData(arrSeisG1ScanType[i],m_ExtendedHeader.m_dwTraceCount,pFileData);
	}
	// 第四个扫描类型：地震道，增益为G2
	nCount = arrSeisG2ScanType.GetCount();
	for(i=0;i<nCount;i++)
	{
		WriteAuxiTraceHeader(4,arrSeisG2ScanType[i],pScanTypeHeader+3,pThread,pChannelList);
		WriteTraceData(arrSeisG2ScanType[i],m_ExtendedHeader.m_dwTraceCount,pFileData);
	}
	// 关闭文件
	SegdFile.Flush();
	SegdFile.Close();
	m_pFile = NULL;
	return true;	
}
#endif