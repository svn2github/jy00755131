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
	string m_pFileNumberGeneral1;//四位数文件编号 (0-9999),2,0327
	string m_pFormatCodeGeneral1;//格式代码,2,8058
	string m_pGeneralConstantsGeneral1;//普通常数,6,000000000005
	string m_pLastTwoDigitsOfYearGeneral1;//年份的最后两位数(0-99),1,12
	string m_pNumberAdditionalBlocksGeneral1;//普通头段中额外数据块数目,H,2
	string m_pJulianDayGeneral1;//儒略日，3 位数(1-366),L+1,187
	string m_pHourOfDayGeneral1;//一天中的小时（0-23）,1,08
	string m_pMinuteOfHourGeneral1;//小时中的分钟（0-59）,1,36
	string m_pSecondOfMinuteGeneral1;//分钟中的秒钟（0-59）,1,06
	string m_pManufacturerCodeGeneral1;//制造商代码,1,13
	string m_pManufacturerSerialNumberGeneral1;//制造商序列号,2,0000
	string m_pBytesPerScanGeneral1;//每次扫描字节数,3,000000
	string m_pBaseScanIntervalGeneral1;//基本扫描间隔,1,10
	string m_pPolarityGeneral1;//极性,H,0
	string m_pNotUsedGeneral1;//未使用,L+1,000
	string m_pRecordTypeGeneral1;//记录类型,H,8
	string m_pRecordLengthGeneral1;//记录长度,L+1,fff
	string m_pScanTypePerRecordGeneral1;//每个记录的扫描类型,1,01
	string m_pNumberChannelSetsPerRecordGeneral1;//每个记录的地震道组数目,1,16
	string m_pNumberSampleSkew32ByteExtensionsGeneral1;//采样时滞 32 字节扩展名数目,1,00
	string m_pExtendedHeaderLengthGeneral1;//扩展头段长度,1,32
	string m_pExternalHeaderLengthGeneral1;//外部头段长度,1,32

	string m_pExpandedFileNumberGeneral2;//扩展文件编号,3,327
	string m_pExpandedChannelSetsGeneral2;//扩展地震道组/ 扫描类型（未使用）,2,0000
	string m_pExtendedHeaderBlocksGeneral2;//扩展头段数据块（未使用）,2,0000
	string m_pExternalHeaderBlocksGeneral2;//外部头段数据块,2,00
	string m_pNotUsed1General2;//未使用,1,00
	string m_pSegdRevisionNumberGeneral2;//SEG-D 版本号,2,0201
	string m_pNumberBlocksGeneralTrailerGeneral2;//普通尾段数据块数目,2,0000
	string m_pExpandedRecordLengthGeneral2;//扩展记录长度（0-128000 毫秒）,3,4000
	string m_pNotUsed2General2;//未使用,1,00
	string m_pGeneralHeaderBlockNumberGeneral2;//普通头段数据块编号,1,02
	string m_pNotUsed3General2;//未使用,13,00000000000000000000000000

	string m_pExpandedFileNumberGeneral3;//扩展文件编号,3,000147
	string m_pSourceLineNumberGeneral3;//震源测线号(0-99999.99),5,0000030000
	string m_pSourcePointNumberGeneral3;//震源点号(0-99999.99),5,00001e0000
	string m_pSourcePointIndexGeneral3;//震源点索引 (1-9),1,01
	string m_pPhaseControlGeneral3;//相位控制（未记录的）,1,00
	string m_pVibratorTypeGeneral3;//可控震源类型（未记录的）,1,00
	string m_pPhaseAngleGeneral3;//相位角（未记录的）,2,0000
	string m_pGeneralHeaderBlockNumberGeneral3;//普通头段数据块编号,1,03
	string m_pSourceSetNumberGeneral3;//震源组号,1,01
	string m_pNotUsedGeneral3;//未使用,12,000000000000000000000000

	//说明，字节数，数据1，数据2，数据3，此后均同数据3至数据16
	string m_pScanTypeHeaderScan[16];//扫描类型头段,1,01,01,00...
	string m_pChannelSetNumberScan[16];//地震道组号,1,01,02,00...
	string m_pChannelSetStartingTimeScan[16];//地震道组起始时间,2,00,00,00...
	string m_pChannelSetEndTimeScan[16];//地震道组结束时间,2,2000,2000,00...
	string m_pDescaleMultiplierScan[16];//缩小比例乘数,2,6db7,6db7,0000...
	string m_pNumberChannelsOfChannelSetScan[16];//此地震道组中地震道数目,2,0002,0012,0000...
	string m_pChannelTypeIdentificationScan[16];//地震道类型标识,H,9,1,0...
	string m_pNotUsedScan[16];//未使用,L,0,0,0...
	string m_pNumberSubscansExponentScan[16];//副扫描指数数目,H,0,0,00...
	string m_pChannelGainControlMethodScan[16];//地震道增益控制方法（固定增益）,L,3,3,00...
	string m_pAliasFilterFrequency3dBPointScan[16];//3dB 点处的混叠滤波器频率,2,0400,0400,0000...
	string m_pAliasFilterSlopeScan[16];//混叠滤波器斜率,2,0370,0370,0000...
	string m_pLowCutFilterFrequencyScan[16];//低截滤波器频率,2,0000,0000,0000...
	string m_pLowCutFilterSlopeScan[16];//低截滤波器斜率,2,0000,0000,0000...
	string m_pFirstNotchFrequencyScan[16];//第一陷波频率,2,0000,0000,0000...
	string m_pSecondNotchFrequencyScan[16];//第二陷波频率,2,0000,0000,0000...
	string m_pThirdNotchFrequencyScan[16];//第三陷波频率,2,0000,0000,0000...
	string m_pExtendedChannelSetNumberScan[16];//扩展地震道组号,2,0000,0000,0000...
	string m_pExtendedHeaderFlagScan[16];//扩展头段标记,H,0,0,0...
	string m_pTraceHeaderExtensionsScan[16];//道头扩展,L,7,7,0...
	string m_pVerticalStackScan[16];//垂直叠加,1,01,01,00...
	string m_pStreamerCableNumberScan[16];//拖缆电缆号,1,00,00,00...
	string m_pArrayFormingScan[16];//阵列组成（无阵列组成）,1,01,01,00...

	string m_pAcquisitionLengthExtended;//采集长度,4,4000
	string m_pSampleRateExtended;//采样率,4,1000
	string m_pTotalNumberTracesExtended;//记录道总数,4,14
	string m_pNumberAuxesExtended;//辅助道数目,4,02
	string m_pNumberSeisTracesExtended;//地震记录道数目,4,12
	string m_pNumberDeadSeisTracesExtended;//停滞地震记录道数目,4,00
	string m_pNumberLiveSeisTracesExtended;//活动地震记录道数目,4,12
	string m_pTypeSourceExtended;//震源类型,4,01
	string m_pNumberSamplesInTraceExtended;//记录道中样本数目,4,4001
	string m_pShotNumberExtended;//炮号,4,01
	string m_pTBWindowExtended;//TB 窗口,4,3000.000000
	string m_pTestRecordTypeExtended;//测试记录类型,4,00
	string m_pSpreadFirstLineExtended;//线束第一条测线,4,01
	string m_pSpreadFirstNumberExtended;//线束第一个号码,4,34
	string m_pSpreadNumberExtended;//排列编号,4,01
	string m_pSpreadTypeExtended;//排列类型,4,01
	string m_pTimebreakExtended;//时断信号,4,00
	string m_pUpholeTimeExtended;//井口时间,4,00
	string m_pBlasterIdExtended;//爆炸机标识号,4,01
	string m_pBlasterStatusExtended;//爆炸机状态,4,02
	string m_pRefractionDelayExtended;//折射延迟,4,00
	string m_pTBtoT0TimeExtended;//Tb 至 T0 时间,4,00
	string m_pInternalTimeBreakExtended;//内部时断信号,4,00
	string m_pPrestackWithinFieldUnitsExtended;//野外设备中预叠加,4,00
	string m_pNoiseEliminationTypeExtended;//噪声抑制类型,4,01
	string m_pLowTracePercentageExtended;//低记录道百分比,4,00
	string m_pLowTraceValueExtended;//低记录道值,4,00
	string m_pNumberWindowsOrWindowLengthExtended;//窗口数目,4,00
	string m_pHistoricEditingTypeOrOverlapExtended;//历史编辑类型,4,00
	string m_pNoisyTracePercentageExtended;//噪声记录道百分比,4,00
	string m_pHistoricRangeExtended;//历史范围,4,00
	string m_pHistoricTaperLengthExtended;//历史锥形长度 2 的指数,4,00
	string m_pThresholdHoldOrNotExtended;//门槛值不变/ 变化,4,00
	string m_pHistoricThresholdInitValueExtended;//历史门槛值初始值,4,00
	string m_pHistoricZeroingLengthExtended;//历史归零长度,4,00
	string m_pTypeProcessExtended;//处理类型,4,01
	string m_pAcquisitionTypeTablesExtended;//采集类型表,128,00
	string m_pThresholdTypeTablesExtended;//门槛值类型表,128,00
	string m_pStackingFoldExtended;//叠加次数,4,01
	string m_pNotUsed1Extended;//未使用,80,00
	string m_pRecordLengthExtended;//记录长度,4,4000
	string m_pAutocorrelationPeakTimeExtended;//自相关峰值时间,4,00
	string m_pNotUsed2Extended;//未使用,4,00
	string m_pCorrelationPilotNoExtended;//相关参考信号编号,4,00
	string m_pPilotLengthExtended;//参考信号长度,4,00
	string m_pSweepLengthExtended;//扫描长度,4,00
	string m_pAcquisitionNumberExtended;//采集号,4,01
	string m_pMaxOfMaxAuxExtended;//最多辅助道的最大值,4,3041785.250000
	string m_pMaxOfMaxSeisExtended;//最多地震道最大值,4,-614945.500000
	string m_pDumpStackingFoldExtended;//转储叠加次数,4,01
	string m_pTapeLabelExtended;//磁带标签,16,label
	string m_pTapeNumberExtended;//磁带号,4,50
	string m_pSoftwareVersionExtended;//软件版本,16,4.0
	string m_pDateExtended;//日期,12,05 Jul 2012
	string m_pSourceEastingExtended;//震源偏东距,8,0
	string m_pSourceNorthingExtended;//震源北向纬度差,8,0
	string m_pSourceElevationExtended;//震源高程,4,0.000000
	string m_pSlipSweepModeUsedExtended;//所用的滑动扫描模式,4,00
	string m_pFilesPerTapeExtended;//每个磁带的文件数,4,2000
	string m_pFileCountExtended;//文件计数,4,01
	string m_pAcquisitionErrorDescriptionExtended;//采集错误说明,160,
	string m_pFilterTypeExtended;//滤波器类型,4,02
	string m_pStackDumpedExtended;//叠加已转储,4,00
	string m_pStackSignExtended;//叠加符号（当前）,4,01
	string m_pPRMTiltCorrectionUsedExtended;//所用的 PRM 倾斜度校正,4,00
	string m_pSwathNameExtended;//线束名,64,D5
	string m_pOperatingModeExtended;//工作模式,4,01
	string m_pReserved1Extended;//保留,4,79
	string m_pNoLogExtended;//无记录,4,00
	string m_pListeningTimeExtended;//监听时间,4,4000
	string m_pTypeDumpExtended;//转储类型,4,00
	string m_pReserved2Extended;//保留,4,00
	string m_pSwathIdExtended;//线束 Id,4,01
	string m_pSeismicTraceOffsetRemovalDisabledExtended;//,4,00
	string m_pGPSTimeAcquisitionTimeBreakExtended;//,8,0003a4ac0c90344f
	string m_pAlignedGPSTimeAcquisitionExtended;//,8,0000000000000000
	string m_pNotUsed3Extended;//未使用,132,000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000

	char* m_pConcatenationOfExternal;//外部头段,m_pExternalHeaderLengthGeneral1*32,*SHOTPRO SP#    1/SL#    3.0/SN#   30.0/SI#1/SEQ# 1/STA:2/CTB:00.000/UH:000.0
}m_oSegdStandardHeaderStruct;

typedef struct Segd_Trace_Header_Struct
{
	//说明，字节数，采集道数据，辅助道数据
	string m_pFileNumberTrace;//四位数文件号(0-9999),2,0327,0327
	string m_pScanTypeNumberTrace;//扫描类型编号,1,01,01
	string m_pChannelSetNumberTrace;//地震道组号,1,02,01
	string m_pTraceNumberTrace;//记录道编号,2,0001,0001
	string m_pFirstTimingWordTrace;//第一个计时字,3,00,00
	string m_pTraceHeaderExtensionTrace;//道头扩展,1,07,07
	string m_pSampleSkewTrace;//采样时滞,1,00,00
	string m_pTraceEditTrace;//记录道编辑,1,00,00
	string m_pTimeBreakWindowTrace;//时断信号窗口,3,00,00
	string m_pExtendedChannelSetNumberTrace;//扩展地震道组号,2,0000,0000
	string m_pExtendedFileNumberTrace;//扩展文件编号,3,000147,000147

	string m_pReceiverLineNumberTrace1;//接收点测线号,3,01,00
	string m_pReceiverPointNumberTrace1;//接收点号,3,34,01
	string m_pReceiverPointIndexTrace1;//接收点索引,1,01,01
	string m_pNumberSamplesPerTraceTrace1;//每个记录道样本数目,3,4001,4001
	string m_pExtendedReceiverLineNumberTrace1;//扩展接收点测线号,5,0000000000,0000000000
	string m_pExtendedReceiverPointNumberTrace1;//扩展接收点号,5,0000000000,0000000000
	string m_pSensorSEGDCodeTrace1;//检波器 SEGD 代码,1,02,00
	string m_pNotUsedTrace1;//未使用,11,0000000000000000000000,0000000000000000000000

	string m_pReceiverPointEastingTrace2;//接收点偏东距,8,0,0
	string m_pReceiverPointNorthingTrace2;//接收点北向纬度差,8,0,0
	string m_pReceiverPointElevationTrace2;//接收点高程,4,0.000000,0.000000
	string m_pSensorTypeNumberTrace2;//检波器类型编号（1 到 9）,1,01,00
	string m_pNotUsedTrace2;//未使用,3,000000,000000
	string m_pDSDIdentificationNoTrace2;//未使用,4,00,00
	string m_pExtendedTraceNoTrace2;//扩展记录道编号,4,01,01

	string m_pResistanceLowLimitTrace3;//电阻下限,4,300.000000,0.000000
	string m_pResistanceHighLimitTrace3;//电阻上限,4,500.000000,0.000000
	string m_pResistanceResistanceValueTrace3;//电阻值,4,1904.939941,0.000000
	string m_pTiltLimitTrace3;//倾斜度界限,4,15.000000,0.000000
	string m_pTiltValueTrace3;//倾斜度值,4,N-,0.000000
	string m_pResistanceErrorTrace3;//电阻误差,1,01,00
	string m_pTiltErrorTrace3;//倾斜度误差,1,00,00
	string m_pNotUsedTrace3;//未使用,10,00000000000000000000,00000000000000000000

	string m_pCapacitanceLowLimitTrace4;//电容下限,4,N-,0.000000
	string m_pCapacitanceHighLimitTrace4;//电容上限,4,N-,0.000000
	string m_pCapacitanceValueTrace4;//电容值,4,N-,0.000000
	string m_pCutOffLowLimitTrace4;//截止下限,4,N-,0.000000
	string m_pCutOffHighLimitsTrace4;//截止上限,4,N-,0.000000
	string m_pCutOffValueTrace4;//截止值,4,N-,0.000000
	string m_pCapacitanceErrorTrace4;//电容误差,1,00,00
	string m_pCutOffErrorTrace4;//截止误差,1,00,00
	string m_pNotUsedTrace4;//未使用,6,000000000000,000000000000

	string m_pLeakageLimitTrace5;//漏电界限,4,5.000000,0.000000
	string m_pLeakageValueTrace5;//漏电值,4,5.000000,0.000000
	string m_pNotUsed1Trace5;//未使用,16,00000000000000000000000000000000,00000000000000000000000000000000
	string m_pLeakageErrorTrace5;//漏电误差,1,00,00
	string m_pNotUsed2Trace5;//未使用,7,00000000000000,00000000000000

	string m_pUnitTypeTrace6;//设备类型,1,01,01
	string m_pUnitSerialNumberTrace6;//设备序列号,3,6104422,4819236
	string m_pChannelNumberTrace6;//地震道编号,1,01,01
	string m_pSpare1Trace6;//备用,3,000000,000000
	string m_pAssemblyTypeTrace6;//组件类型,1,03,04
	string m_pFDUorDSUAssemblySerialNumberTrace6;//FDU 或 DSU 组件序列号,3,1791352,4819236
	string m_pLocationInFDUorDSUAssemblyTrace6;//在 FDU 或 DSU组件中的位置,1,02,01
	string m_pSpare2Trace6;//备用,3,000000,000000
	string m_pSubunitTypeTrace6;//FDU 或 DSU 设备类型,1,01,21
	string m_pChannelTypeTrace6;//地震道类型,1,00,00
	string m_pSpare3Trace6;//备用,2,0000,0000
	string m_pSensorSensitivityTrace6;//检波器灵敏度,4,4294967296.000000,4294967296.000000
	string m_pNotUsedTrace6;//未使用,8,0000000000000000,0000000000000000

	string m_pControlUnitTypeTrace7;//控制单元类型,1,02,49
	string m_pControlUnitSerialNumberTrace7;//控制单元序列号,3,17693,2442269
	string m_pChannelGainScaleTrace7;//地震道增益刻度,1,02,02
	string m_pChannelFilterTrace7;//地震道滤波器,1,02,02
	string m_pChannelDataErrorTrace7;//地震道数据误差：超过范围,1,00,00
	string m_pChannelEditedStatusTrace7;//地震道编辑状态,1,00,00
	string m_pChannelSampleTomVConversionFactorTrace7;//地震道采样毫伏转换系数,4,0.000067,0.000000
	string m_pNumberOfStacksNoisyTrace7;//有噪声叠加次数,1,00,00
	string m_pNumberOfStacksLowTrace7;//低叠加次数,1,00,00
	string m_pChannelTypeIdTrace7;//地震道类型标识号,1,01,09
	string m_pChannelProcessTrace7;//地震道处理,1,01,01
	string m_pTraceMaxValueTrace7;//记录道最大值,4,0.000000,0.000000
	string m_pTraceMaxTimeTrace7;//记录道最大时间,4,00,00
	string m_pNumberInterpolationsTrace7;//内插次数,4,00,00
	string m_pSeismicTraceOffsetValueTrace7;//未使用,4,00,00	
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