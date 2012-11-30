#ifndef _SEGD_H
#define _SEGD_H
#define COUNT_SCAN_TYPE_HEADER		16
#define SIZE_BUFFER					256
/**
* @struct Segd_Standard_Header_Struct
* @brief Segd文件标准头结构体
*/
typedef struct Segd_Standard_Header_Struct
{
	/** 采样率*/
	unsigned int m_uiSampleRate;
	/** 采样长度*/
	int m_iSampleLength;
	/** 采样时间*/
	unsigned int m_uiSampleTime;
	/** 辅助道数目*/
	int m_iAuxTraceNum;
	/* 采集道数目*/
	int m_iAcqTraceNum;
	/** 总道数*/
	int m_iTotalTraceNum;
	/** 炮号*/
	unsigned int m_uiShotNo;

	//说明，字节数，数据
	char m_pFileNumberGeneral1[SIZE_BUFFER];//四位数文件编号 (0-9999),2,0327
	char m_pFormatCodeGeneral1[SIZE_BUFFER];//格式代码,2,8058
	char m_pGeneralConstantsGeneral1[SIZE_BUFFER];//普通常数,6,000000000005
	char m_pLastTwoDigitsOfYearGeneral1[SIZE_BUFFER];//年份的最后两位数(0-99),1,12
	char m_pNumberAdditionalBlocksGeneral1[SIZE_BUFFER];//普通头段中额外数据块数目,H,2
	char m_pJulianDayGeneral1[SIZE_BUFFER];//儒略日，3 位数(1-366),L+1,187
	char m_pHourOfDayGeneral1[SIZE_BUFFER];//一天中的小时（0-23）,1,08
	char m_pMinuteOfHourGeneral1[SIZE_BUFFER];//小时中的分钟（0-59）,1,36
	char m_pSecondOfMinuteGeneral1[SIZE_BUFFER];//分钟中的秒钟（0-59）,1,06
	char m_pManufacturerCodeGeneral1[SIZE_BUFFER];//制造商代码,1,13
	char m_pManufacturerSerialNumberGeneral1[SIZE_BUFFER];//制造商序列号,2,0000
	char m_pBytesPerScanGeneral1[SIZE_BUFFER];//每次扫描字节数,3,000000
	char m_pBaseScanIntervalGeneral1[SIZE_BUFFER];//基本扫描间隔,1,10
	char m_pPolarityGeneral1[SIZE_BUFFER];//极性,H,0
	char m_pNotUsedGeneral1[SIZE_BUFFER];//未使用,L+1,000
	char m_pRecordTypeGeneral1[SIZE_BUFFER];//记录类型,H,8
	char m_pRecordLengthGeneral1[SIZE_BUFFER];//记录长度,L+1,fff
	char m_pScanTypePerRecordGeneral1[SIZE_BUFFER];//每个记录的扫描类型,1,01
	char m_pNumberChannelSetsPerRecordGeneral1[SIZE_BUFFER];//每个记录的地震道组数目,1,16
	char m_pNumberSampleSkew32ByteExtensionsGeneral1[SIZE_BUFFER];//采样时滞 32 字节扩展名数目,1,00
	char m_pExtendedHeaderLengthGeneral1[SIZE_BUFFER];//扩展头段长度,1,32
	char m_pExternalHeaderLengthGeneral1[SIZE_BUFFER];//外部头段长度,1,32

	char m_pExpandedFileNumberGeneral2[SIZE_BUFFER];//扩展文件编号,3,327
	char m_pExpandedChannelSetsGeneral2[SIZE_BUFFER];//扩展地震道组/ 扫描类型（未使用）,2,0000
	char m_pExtendedHeaderBlocksGeneral2[SIZE_BUFFER];//扩展头段数据块（未使用）,2,0000
	char m_pExternalHeaderBlocksGeneral2[SIZE_BUFFER];//外部头段数据块,2,00
	char m_pNotUsed1General2[SIZE_BUFFER];//未使用,1,00
	char m_pSegdRevisionNumberGeneral2[SIZE_BUFFER];//SEG-D 版本号,2,0201
	char m_pNumberBlocksGeneralTrailerGeneral2[SIZE_BUFFER];//普通尾段数据块数目,2,0000
	char m_pExpandedRecordLengthGeneral2[SIZE_BUFFER];//扩展记录长度（0-128000 毫秒）,3,4000
	char m_pNotUsed2General2[SIZE_BUFFER];//未使用,1,00
	char m_pGeneralHeaderBlockNumberGeneral2[SIZE_BUFFER];//普通头段数据块编号,1,02
	char m_pNotUsed3General2[SIZE_BUFFER];//未使用,13,00000000000000000000000000

	char m_pExpandedFileNumberGeneral3[SIZE_BUFFER];//扩展文件编号,3,000147
	char m_pSourceLineNumberGeneral3[SIZE_BUFFER];//震源测线号(0-99999.99),5,0000030000
	char m_pSourcePointNumberGeneral3[SIZE_BUFFER];//震源点号(0-99999.99),5,00001e0000
	char m_pSourcePointIndexGeneral3[SIZE_BUFFER];//震源点索引 (1-9),1,01
	char m_pPhaseControlGeneral3[SIZE_BUFFER];//相位控制（未记录的）,1,00
	char m_pVibratorTypeGeneral3[SIZE_BUFFER];//可控震源类型（未记录的）,1,00
	char m_pPhaseAngleGeneral3[SIZE_BUFFER];//相位角（未记录的）,2,0000
	char m_pGeneralHeaderBlockNumberGeneral3[SIZE_BUFFER];//普通头段数据块编号,1,03
	char m_pSourceSetNumberGeneral3[SIZE_BUFFER];//震源组号,1,01
	char m_pNotUsedGeneral3[SIZE_BUFFER];//未使用,12,000000000000000000000000

	//说明，字节数，数据1，数据2，数据3，此后均同数据3至数据16
	char m_pScanTypeHeaderScan[16][SIZE_BUFFER];//扫描类型头段,1,01,01,00...
	char m_pChannelSetNumberScan[16][SIZE_BUFFER];//地震道组号,1,01,02,00...
	char m_pChannelSetStartingTimeScan[16][SIZE_BUFFER];//地震道组起始时间,2,00,00,00...
	char m_pChannelSetEndTimeScan[16][SIZE_BUFFER];//地震道组结束时间,2,2000,2000,00...
	char m_pDescaleMultiplierScan[16][SIZE_BUFFER];//缩小比例乘数,2,6db7,6db7,0000...
	char m_pNumberChannelsOfChannelSetScan[16][SIZE_BUFFER];//此地震道组中地震道数目,2,0002,0012,0000...
	char m_pChannelTypeIdentificationScan[16][SIZE_BUFFER];//地震道类型标识,H,9,1,0...
	char m_pNotUsedScan[16][SIZE_BUFFER];//未使用,L,0,0,0...
	char m_pNumberSubscansExponentScan[16][SIZE_BUFFER];//副扫描指数数目,H,0,0,00...
	char m_pChannelGainControlMethodScan[16][SIZE_BUFFER];//地震道增益控制方法（固定增益）,L,3,3,00...
	char m_pAliasFilterFrequency3dBPointScan[16][SIZE_BUFFER];//3dB 点处的混叠滤波器频率,2,0400,0400,0000...
	char m_pAliasFilterSlopeScan[16][SIZE_BUFFER];//混叠滤波器斜率,2,0370,0370,0000...
	char m_pLowCutFilterFrequencyScan[16][SIZE_BUFFER];//低截滤波器频率,2,0000,0000,0000...
	char m_pLowCutFilterSlopeScan[16][SIZE_BUFFER];//低截滤波器斜率,2,0000,0000,0000...
	char m_pFirstNotchFrequencyScan[16][SIZE_BUFFER];//第一陷波频率,2,0000,0000,0000...
	char m_pSecondNotchFrequencyScan[16][SIZE_BUFFER];//第二陷波频率,2,0000,0000,0000...
	char m_pThirdNotchFrequencyScan[16][SIZE_BUFFER];//第三陷波频率,2,0000,0000,0000...
	char m_pExtendedChannelSetNumberScan[16][SIZE_BUFFER];//扩展地震道组号,2,0000,0000,0000...
	char m_pExtendedHeaderFlagScan[16][SIZE_BUFFER];//扩展头段标记,H,0,0,0...
	char m_pTraceHeaderExtensionsScan[16][SIZE_BUFFER];//道头扩展,L,7,7,0...
	char m_pVerticalStackScan[16][SIZE_BUFFER];//垂直叠加,1,01,01,00...
	char m_pStreamerCableNumberScan[16][SIZE_BUFFER];//拖缆电缆号,1,00,00,00...
	char m_pArrayFormingScan[16][SIZE_BUFFER];//阵列组成（无阵列组成）,1,01,01,00...

	char m_pAcquisitionLengthExtended[SIZE_BUFFER];//采集长度,4,4000
	char m_pSampleRateExtended[SIZE_BUFFER];//采样率,4,1000
	char m_pTotalNumberTracesExtended[SIZE_BUFFER];//记录道总数,4,14
	char m_pNumberAuxesExtended[SIZE_BUFFER];//辅助道数目,4,02
	char m_pNumberSeisTracesExtended[SIZE_BUFFER];//地震记录道数目,4,12
	char m_pNumberDeadSeisTracesExtended[SIZE_BUFFER];//停滞地震记录道数目,4,00
	char m_pNumberLiveSeisTracesExtended[SIZE_BUFFER];//活动地震记录道数目,4,12
	char m_pTypeSourceExtended[SIZE_BUFFER];//震源类型,4,01
	char m_pNumberSamplesInTraceExtended[SIZE_BUFFER];//记录道中样本数目,4,4001
	char m_pShotNumberExtended[SIZE_BUFFER];//炮号,4,01
	char m_pTBWindowExtended[SIZE_BUFFER];//TB 窗口,4,3000.000000
	char m_pTestRecordTypeExtended[SIZE_BUFFER];//测试记录类型,4,00
	char m_pSpreadFirstLineExtended[SIZE_BUFFER];//线束第一条测线,4,01
	char m_pSpreadFirstNumberExtended[SIZE_BUFFER];//线束第一个号码,4,34
	char m_pSpreadNumberExtended[SIZE_BUFFER];//排列编号,4,01
	char m_pSpreadTypeExtended[SIZE_BUFFER];//排列类型,4,01
	char m_pTimebreakExtended[SIZE_BUFFER];//时断信号,4,00
	char m_pUpholeTimeExtended[SIZE_BUFFER];//井口时间,4,00
	char m_pBlasterIdExtended[SIZE_BUFFER];//爆炸机标识号,4,01
	char m_pBlasterStatusExtended[SIZE_BUFFER];//爆炸机状态,4,02
	char m_pRefractionDelayExtended[SIZE_BUFFER];//折射延迟,4,00
	char m_pTBtoT0TimeExtended[SIZE_BUFFER];//Tb 至 T0 时间,4,00
	char m_pInternalTimeBreakExtended[SIZE_BUFFER];//内部时断信号,4,00
	char m_pPrestackWithinFieldUnitsExtended[SIZE_BUFFER];//野外设备中预叠加,4,00
	char m_pNoiseEliminationTypeExtended[SIZE_BUFFER];//噪声抑制类型,4,01
	char m_pLowTracePercentageExtended[SIZE_BUFFER];//低记录道百分比,4,00
	char m_pLowTraceValueExtended[SIZE_BUFFER];//低记录道值,4,00
	char m_pNumberWindowsOrWindowLengthExtended[SIZE_BUFFER];//窗口数目,4,00
	char m_pHistoricEditingTypeOrOverlapExtended[SIZE_BUFFER];//历史编辑类型,4,00
	char m_pNoisyTracePercentageExtended[SIZE_BUFFER];//噪声记录道百分比,4,00
	char m_pHistoricRangeExtended[SIZE_BUFFER];//历史范围,4,00
	char m_pHistoricTaperLengthExtended[SIZE_BUFFER];//历史锥形长度 2 的指数,4,00
	char m_pThresholdHoldOrNotExtended[SIZE_BUFFER];//门槛值不变/ 变化,4,00
	char m_pHistoricThresholdInitValueExtended[SIZE_BUFFER];//历史门槛值初始值,4,00
	char m_pHistoricZeroingLengthExtended[SIZE_BUFFER];//历史归零长度,4,00
	char m_pTypeProcessExtended[SIZE_BUFFER];//处理类型,4,01
	char m_pAcquisitionTypeTablesExtended[SIZE_BUFFER];//采集类型表,128,00
	char m_pThresholdTypeTablesExtended[SIZE_BUFFER];//门槛值类型表,128,00
	char m_pStackingFoldExtended[SIZE_BUFFER];//叠加次数,4,01
	char m_pNotUsed1Extended[SIZE_BUFFER];//未使用,80,00
	char m_pRecordLengthExtended[SIZE_BUFFER];//记录长度,4,4000
	char m_pAutocorrelationPeakTimeExtended[SIZE_BUFFER];//自相关峰值时间,4,00
	char m_pNotUsed2Extended[SIZE_BUFFER];//未使用,4,00
	char m_pCorrelationPilotNoExtended[SIZE_BUFFER];//相关参考信号编号,4,00
	char m_pPilotLengthExtended[SIZE_BUFFER];//参考信号长度,4,00
	char m_pSweepLengthExtended[SIZE_BUFFER];//扫描长度,4,00
	char m_pAcquisitionNumberExtended[SIZE_BUFFER];//采集号,4,01
	char m_pMaxOfMaxAuxExtended[SIZE_BUFFER];//最多辅助道的最大值,4,3041785.250000
	char m_pMaxOfMaxSeisExtended[SIZE_BUFFER];//最多地震道最大值,4,-614945.500000
	char m_pDumpStackingFoldExtended[SIZE_BUFFER];//转储叠加次数,4,01
	char m_pTapeLabelExtended[SIZE_BUFFER];//磁带标签,16,label
	char m_pTapeNumberExtended[SIZE_BUFFER];//磁带号,4,50
	char m_pSoftwareVersionExtended[SIZE_BUFFER];//软件版本,16,4.0
	char m_pDateExtended[SIZE_BUFFER];//日期,12,05 Jul 2012
	char m_pSourceEastingExtended[SIZE_BUFFER];//震源偏东距,8,0
	char m_pSourceNorthingExtended[SIZE_BUFFER];//震源北向纬度差,8,0
	char m_pSourceElevationExtended[SIZE_BUFFER];//震源高程,4,0.000000
	char m_pSlipSweepModeUsedExtended[SIZE_BUFFER];//所用的滑动扫描模式,4,00
	char m_pFilesPerTapeExtended[SIZE_BUFFER];//每个磁带的文件数,4,2000
	char m_pFileCountExtended[SIZE_BUFFER];//文件计数,4,01
	char m_pAcquisitionErrorDescriptionExtended[SIZE_BUFFER];//采集错误说明,160,
	char m_pFilterTypeExtended[SIZE_BUFFER];//滤波器类型,4,02
	char m_pStackDumpedExtended[SIZE_BUFFER];//叠加已转储,4,00
	char m_pStackSignExtended[SIZE_BUFFER];//叠加符号（当前）,4,01
	char m_pPRMTiltCorrectionUsedExtended[SIZE_BUFFER];//所用的 PRM 倾斜度校正,4,00
	char m_pSwathNameExtended[SIZE_BUFFER];//线束名,64,D5
	char m_pOperatingModeExtended[SIZE_BUFFER];//工作模式,4,01
	char m_pReserved1Extended[SIZE_BUFFER];//保留,4,79
	char m_pNoLogExtended[SIZE_BUFFER];//无记录,4,00
	char m_pListeningTimeExtended[SIZE_BUFFER];//监听时间,4,4000
	char m_pTypeDumpExtended[SIZE_BUFFER];//转储类型,4,00
	char m_pReserved2Extended[SIZE_BUFFER];//保留,4,00
	char m_pSwathIdExtended[SIZE_BUFFER];//线束 Id,4,01
	char m_pSeismicTraceOffsetRemovalDisabledExtended[SIZE_BUFFER];//,4,00
	char m_pGPSTimeAcquisitionTimeBreakExtended[SIZE_BUFFER];//,8,0003a4ac0c90344f
	char m_pAlignedGPSTimeAcquisitionExtended[SIZE_BUFFER];//,8,0000000000000000
	char m_pNotUsed3Extended[SIZE_BUFFER];//未使用,132,000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000

	char* m_pConcatenationOfExternal;//外部头段,m_pExternalHeaderLengthGeneral1*32,*SHOTPRO SP#    1/SL#    3.0/SN#   30.0/SI#1/SEQ# 1/STA:2/CTB:00.000/UH:000.0
}m_oSegdStandardHeaderStruct;

typedef struct Segd_Trace_Header_Struct
{
	//说明，字节数，采集道数据，辅助道数据
	char m_pFileNumberTrace[SIZE_BUFFER];//四位数文件号(0-9999),2,0327,0327
	char m_pScanTypeNumberTrace[SIZE_BUFFER];//扫描类型编号,1,01,01
	char m_pChannelSetNumberTrace[SIZE_BUFFER];//地震道组号,1,02,01
	char m_pTraceNumberTrace[SIZE_BUFFER];//记录道编号,2,0001,0001
	char m_pFirstTimingWordTrace[SIZE_BUFFER];//第一个计时字,3,00,00
	char m_pTraceHeaderExtensionTrace[SIZE_BUFFER];//道头扩展,1,07,07
	char m_pSampleSkewTrace[SIZE_BUFFER];//采样时滞,1,00,00
	char m_pTraceEditTrace[SIZE_BUFFER];//记录道编辑,1,00,00
	char m_pTimeBreakWindowTrace[SIZE_BUFFER];//时断信号窗口,3,00,00
	char m_pExtendedChannelSetNumberTrace[SIZE_BUFFER];//扩展地震道组号,2,0000,0000
	char m_pExtendedFileNumberTrace[SIZE_BUFFER];//扩展文件编号,3,000147,000147

	char m_pReceiverLineNumberTrace1[SIZE_BUFFER];//接收点测线号,3,01,00
	char m_pReceiverPointNumberTrace1[SIZE_BUFFER];//接收点号,3,34,01
	char m_pReceiverPointIndexTrace1[SIZE_BUFFER];//接收点索引,1,01,01
	char m_pNumberSamplesPerTraceTrace1[SIZE_BUFFER];//每个记录道样本数目,3,4001,4001
	char m_pExtendedReceiverLineNumberTrace1[SIZE_BUFFER];//扩展接收点测线号,5,0000000000,0000000000
	char m_pExtendedReceiverPointNumberTrace1[SIZE_BUFFER];//扩展接收点号,5,0000000000,0000000000
	char m_pSensorSEGDCodeTrace1[SIZE_BUFFER];//检波器 SEGD 代码,1,02,00
	char m_pNotUsedTrace1[SIZE_BUFFER];//未使用,11,0000000000000000000000,0000000000000000000000

	char m_pReceiverPointEastingTrace2[SIZE_BUFFER];//接收点偏东距,8,0,0
	char m_pReceiverPointNorthingTrace2[SIZE_BUFFER];//接收点北向纬度差,8,0,0
	char m_pReceiverPointElevationTrace2[SIZE_BUFFER];//接收点高程,4,0.000000,0.000000
	char m_pSensorTypeNumberTrace2[SIZE_BUFFER];//检波器类型编号（1 到 9）,1,01,00
	char m_pNotUsedTrace2[SIZE_BUFFER];//未使用,3,000000,000000
	char m_pDSDIdentificationNoTrace2[SIZE_BUFFER];//未使用,4,00,00
	char m_pExtendedTraceNoTrace2[SIZE_BUFFER];//扩展记录道编号,4,01,01

	char m_pResistanceLowLimitTrace3[SIZE_BUFFER];//电阻下限,4,300.000000,0.000000
	char m_pResistanceHighLimitTrace3[SIZE_BUFFER];//电阻上限,4,500.000000,0.000000
	char m_pResistanceResistanceValueTrace3[SIZE_BUFFER];//电阻值,4,1904.939941,0.000000
	char m_pTiltLimitTrace3[SIZE_BUFFER];//倾斜度界限,4,15.000000,0.000000
	char m_pTiltValueTrace3[SIZE_BUFFER];//倾斜度值,4,N-,0.000000
	char m_pResistanceErrorTrace3[SIZE_BUFFER];//电阻误差,1,01,00
	char m_pTiltErrorTrace3[SIZE_BUFFER];//倾斜度误差,1,00,00
	char m_pNotUsedTrace3[SIZE_BUFFER];//未使用,10,00000000000000000000,00000000000000000000

	char m_pCapacitanceLowLimitTrace4[SIZE_BUFFER];//电容下限,4,N-,0.000000
	char m_pCapacitanceHighLimitTrace4[SIZE_BUFFER];//电容上限,4,N-,0.000000
	char m_pCapacitanceValueTrace4[SIZE_BUFFER];//电容值,4,N-,0.000000
	char m_pCutOffLowLimitTrace4[SIZE_BUFFER];//截止下限,4,N-,0.000000
	char m_pCutOffHighLimitsTrace4[SIZE_BUFFER];//截止上限,4,N-,0.000000
	char m_pCutOffValueTrace4[SIZE_BUFFER];//截止值,4,N-,0.000000
	char m_pCapacitanceErrorTrace4[SIZE_BUFFER];//电容误差,1,00,00
	char m_pCutOffErrorTrace4[SIZE_BUFFER];//截止误差,1,00,00
	char m_pNotUsedTrace4[SIZE_BUFFER];//未使用,6,000000000000,000000000000

	char m_pLeakageLimitTrace5[SIZE_BUFFER];//漏电界限,4,5.000000,0.000000
	char m_pLeakageValueTrace5[SIZE_BUFFER];//漏电值,4,5.000000,0.000000
	char m_pNotUsed1Trace5[SIZE_BUFFER];//未使用,16,00000000000000000000000000000000,00000000000000000000000000000000
	char m_pLeakageErrorTrace5[SIZE_BUFFER];//漏电误差,1,00,00
	char m_pNotUsed2Trace5[SIZE_BUFFER];//未使用,7,00000000000000,00000000000000

	char m_pUnitTypeTrace6[SIZE_BUFFER];//设备类型,1,01,01
	char m_pUnitSerialNumberTrace6[SIZE_BUFFER];//设备序列号,3,6104422,4819236
	char m_pChannelNumberTrace6[SIZE_BUFFER];//地震道编号,1,01,01
	char m_pSpare1Trace6[SIZE_BUFFER];//备用,3,000000,000000
	char m_pAssemblyTypeTrace6[SIZE_BUFFER];//组件类型,1,03,04
	char m_pFDUorDSUAssemblySerialNumberTrace6[SIZE_BUFFER];//FDU 或 DSU 组件序列号,3,1791352,4819236
	char m_pLocationInFDUorDSUAssemblyTrace6[SIZE_BUFFER];//在 FDU 或 DSU组件中的位置,1,02,01
	char m_pSpare2Trace6[SIZE_BUFFER];//备用,3,000000,000000
	char m_pSubunitTypeTrace6[SIZE_BUFFER];//FDU 或 DSU 设备类型,1,01,21
	char m_pChannelTypeTrace6[SIZE_BUFFER];//地震道类型,1,00,00
	char m_pSpare3Trace6[SIZE_BUFFER];//备用,2,0000,0000
	char m_pSensorSensitivityTrace6[SIZE_BUFFER];//检波器灵敏度,4,4294967296.000000,4294967296.000000
	char m_pNotUsedTrace6[SIZE_BUFFER];//未使用,8,0000000000000000,0000000000000000

	char m_pControlUnitTypeTrace7[SIZE_BUFFER];//控制单元类型,1,02,49
	char m_pControlUnitSerialNumberTrace7[SIZE_BUFFER];//控制单元序列号,3,17693,2442269
	char m_pChannelGainScaleTrace7[SIZE_BUFFER];//地震道增益刻度,1,02,02
	char m_pChannelFilterTrace7[SIZE_BUFFER];//地震道滤波器,1,02,02
	char m_pChannelDataErrorTrace7[SIZE_BUFFER];//地震道数据误差：超过范围,1,00,00
	char m_pChannelEditedStatusTrace7[SIZE_BUFFER];//地震道编辑状态,1,00,00
	char m_pChannelSampleTomVConversionFactorTrace7[SIZE_BUFFER];//地震道采样毫伏转换系数,4,0.000067,0.000000
	char m_pNumberOfStacksNoisyTrace7[SIZE_BUFFER];//有噪声叠加次数,1,00,00
	char m_pNumberOfStacksLowTrace7[SIZE_BUFFER];//低叠加次数,1,00,00
	char m_pChannelTypeIdTrace7[SIZE_BUFFER];//地震道类型标识号,1,01,09
	char m_pChannelProcessTrace7[SIZE_BUFFER];//地震道处理,1,01,01
	char m_pTraceMaxValueTrace7[SIZE_BUFFER];//记录道最大值,4,0.000000,0.000000
	char m_pTraceMaxTimeTrace7[SIZE_BUFFER];//记录道最大时间,4,00,00
	char m_pNumberInterpolationsTrace7[SIZE_BUFFER];//内插次数,4,00,00
	char m_pSeismicTraceOffsetValueTrace7[SIZE_BUFFER];//未使用,4,00,00	
}m_oSegdTraceHeaderStruct;
/**
* @struct Segd_Data_Header_Struct
* @brief Segd文件数据头结构体
*/
typedef struct Segd_Data_Header_Struct
{
	/** 接收站点号*/
	int m_iPointIndex;
	/** 接收站线号*/
	int m_iLineIndex;
	/** 位置序号*/
	unsigned int m_uiLocation;
	/** 数据道头*/
	m_oSegdTraceHeaderStruct m_oSegdTH;
}m_oSegdDataHeaderStruct;
/**
* @struct Segd_File_Struct
* @brief Segd文件存储结构体
*/
typedef struct Segd_File_Struct
{
	/** Segd文件保存路径*/
	string m_strPath;
	/** Segd标准头文件*/
	m_oSegdStandardHeaderStruct m_oSegdSH;
	/** Segd数据头队列*/
	list<m_oSegdDataHeaderStruct*>* m_pSegdDHList;
	/** Segd数据缓冲区指针，辅助道数据放后面*/
	char* m_pSegdDataBuf;
	/** Segd数据缓冲区长度*/
	unsigned int m_uiSegdDataBufLength;
}m_oSegdFileStruct;
#endif