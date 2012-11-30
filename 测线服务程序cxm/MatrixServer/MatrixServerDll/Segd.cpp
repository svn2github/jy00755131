#include "stdafx.h"
#include "MatrixServerDll.h"
// 初始化SEGD文件标准头
void OnInitSegdSH(m_oSegdStandardHeaderStruct* pSegdSH)
{
	//说明，字节数，数据
	//四位数文件编号 (0-9999),2,0327
	memcpy(pSegdSH->m_pFileNumberGeneral1, "0327", 4);
	//格式代码,2,8058
	memcpy(pSegdSH->m_pFormatCodeGeneral1, "8058", 4);
	//普通常数,6,000000000005
	memcpy(pSegdSH->m_pGeneralConstantsGeneral1, "000000000005", 12);
	//年份的最后两位数(0-99),1,12
	memcpy(pSegdSH->m_pLastTwoDigitsOfYearGeneral1, "12", 2);
	//普通头段中额外数据块数目,H,2
	memcpy(pSegdSH->m_pNumberAdditionalBlocksGeneral1, "2", 1);
	//儒略日，3 位数(1-366),L+1,187
	memcpy(pSegdSH->m_pJulianDayGeneral1, "187", 3);
	//一天中的小时（0-23）,1,08
	memcpy(pSegdSH->m_pHourOfDayGeneral1, "08", 2);
	//小时中的分钟（0-59）,1,36
	memcpy(pSegdSH->m_pMinuteOfHourGeneral1, "36", 2);
	//分钟中的秒钟（0-59）,1,06
	memcpy(pSegdSH->m_pSecondOfMinuteGeneral1, "06", 2);
	//制造商代码,1,13
	memcpy(pSegdSH->m_pManufacturerCodeGeneral1, "13", 2);
	//制造商序列号,2,0000
	memcpy(pSegdSH->m_pManufacturerSerialNumberGeneral1, "0000", 4);
	//每次扫描字节数,3,000000
	memcpy(pSegdSH->m_pBytesPerScanGeneral1, "000000", 6);
	//基本扫描间隔,1,10
	memcpy(pSegdSH->m_pBaseScanIntervalGeneral1, "10", 2);
	//极性,H,0
	memcpy(pSegdSH->m_pPolarityGeneral1, "0", 1);
	//未使用,L+1,000
	memcpy(pSegdSH->m_pNotUsedGeneral1, "000", 3);
	//记录类型,H,8
	memcpy(pSegdSH->m_pRecordTypeGeneral1, "8", 1);
	//记录长度,L+1,fff
	memcpy(pSegdSH->m_pRecordLengthGeneral1, "fff", 3);
	//每个记录的扫描类型,1,01
	memcpy(pSegdSH->m_pScanTypePerRecordGeneral1, "01", 2);
	//每个记录的地震道组数目,1,16
	memcpy(pSegdSH->m_pNumberChannelSetsPerRecordGeneral1, "16", 2);
	//采样时滞 32 字节扩展名数目,1,00
	memcpy(pSegdSH->m_pNumberSampleSkew32ByteExtensionsGeneral1, "00", 2);
	//扩展头段长度,1,32
	memcpy(pSegdSH->m_pExtendedHeaderLengthGeneral1, "32", 2);
	//外部头段长度,1,32
	memcpy(pSegdSH->m_pExternalHeaderLengthGeneral1, "32", 2);
	
	//扩展文件编号,3,327
	memcpy(pSegdSH->m_pExpandedFileNumberGeneral2, "327", 3);
	//扩展地震道组/ 扫描类型（未使用）,2,0000
	memcpy(pSegdSH->m_pExpandedChannelSetsGeneral2, "0000", 4);
	//扩展头段数据块（未使用）,2,0000
	memcpy(pSegdSH->m_pExtendedHeaderBlocksGeneral2, "0000", 4);
	//外部头段数据块,2,00
	memcpy(pSegdSH->m_pExternalHeaderBlocksGeneral2, "00", 2);
	//未使用,1,00
	memcpy(pSegdSH->m_pNotUsed1General2, "00", 2);
	//SEG-D 版本号,2,0201
	memcpy(pSegdSH->m_pSegdRevisionNumberGeneral2, "0201", 4);
	//普通尾段数据块数目,2,0000
	memcpy(pSegdSH->m_pNumberBlocksGeneralTrailerGeneral2, "0000", 4);
	//扩展记录长度（0-128000 毫秒）,3,4000
	memcpy(pSegdSH->m_pExpandedRecordLengthGeneral2, "4000", 4);
	//未使用,1,00
	memcpy(pSegdSH->m_pNotUsed2General2, "00", 2);
	//普通头段数据块编号,1,02
	memcpy(pSegdSH->m_pGeneralHeaderBlockNumberGeneral2, "02", 2);
	//未使用,13,00000000000000000000000000
	memcpy(pSegdSH->m_pNotUsed3General2, "00000000000000000000000000", 26);
	//扩展文件编号,3,000147
	memcpy(pSegdSH->m_pExpandedFileNumberGeneral3, "000147", 6);
	//震源测线号(0-99999.99),5,0000030000
	memcpy(pSegdSH->m_pSourceLineNumberGeneral3, "0000030000", 10);
	//震源点号(0-99999.99),5,00001e0000
	memcpy(pSegdSH->m_pSourcePointNumberGeneral3, "00001e0000", 10);
	//震源点索引 (1-9),1,01
	memcpy(pSegdSH->m_pSourcePointIndexGeneral3, "01", 2);
	//相位控制（未记录的）,1,00
	memcpy(pSegdSH->m_pPhaseControlGeneral3, "00", 2);
	//可控震源类型（未记录的）,1,00
	memcpy(pSegdSH->m_pVibratorTypeGeneral3, "00", 2);
	//相位角（未记录的）,2,0000
	memcpy(pSegdSH->m_pPhaseAngleGeneral3, "0000", 4);
	//普通头段数据块编号,1,03
	memcpy(pSegdSH->m_pGeneralHeaderBlockNumberGeneral3, "03", 2);
	//震源组号,1,01
	memcpy(pSegdSH->m_pSourceSetNumberGeneral3, "01", 2);
	//未使用,12,000000000000000000000000
	memcpy(pSegdSH->m_pNotUsedGeneral3, "000000000000000000000000", 24);

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

	//采集长度,4,4000
	memcpy(pSegdSH->m_pAcquisitionLengthExtended, "4000", 4);
	//采样率,4,1000
	memcpy(pSegdSH->m_pSampleRateExtended, "1000", 4);
	//记录道总数,4,14
	memcpy(pSegdSH->m_pTotalNumberTracesExtended, "14", 2);
	//辅助道数目,4,02
	memcpy(pSegdSH->m_pNumberAuxesExtended, "02", 2);
	//地震记录道数目,4,12
	memcpy(pSegdSH->m_pNumberSeisTracesExtended, "12", 2);
	//停滞地震记录道数目,4,00
	memcpy(pSegdSH->m_pNumberDeadSeisTracesExtended, "00", 2);
	//活动地震记录道数目,4,12
	memcpy(pSegdSH->m_pNumberLiveSeisTracesExtended, "12", 2);
	//震源类型,4,01
	memcpy(pSegdSH->m_pTypeSourceExtended, "01", 2);
	//记录道中样本数目,4,4001
	memcpy(pSegdSH->m_pNumberSamplesInTraceExtended, "4001", 4);
	//炮号,4,01
	memcpy(pSegdSH->m_pShotNumberExtended, "01", 2);
	//TB 窗口,4,3000.000000
	memcpy(pSegdSH->m_pTBWindowExtended, "3000.000000", 11);
	//测试记录类型,4,00
	memcpy(pSegdSH->m_pTestRecordTypeExtended, "00", 2);
	//线束第一条测线,4,01
	memcpy(pSegdSH->m_pSpreadFirstLineExtended, "01", 2);
	//线束第一个号码,4,34
	memcpy(pSegdSH->m_pSpreadFirstNumberExtended, "34", 2);
	//排列编号,4,01
	memcpy(pSegdSH->m_pSpreadNumberExtended, "01", 2);
	//排列类型,4,01
	memcpy(pSegdSH->m_pSpreadTypeExtended, "01", 2);
	//时断信号,4,00
	memcpy(pSegdSH->m_pTimebreakExtended, "00", 2);
	//井口时间,4,00
	memcpy(pSegdSH->m_pUpholeTimeExtended, "00", 2);
	//爆炸机标识号,4,01
	memcpy(pSegdSH->m_pBlasterIdExtended, "01", 2);
	//爆炸机状态,4,02
	memcpy(pSegdSH->m_pBlasterStatusExtended, "02", 2);
	//折射延迟,4,00
	memcpy(pSegdSH->m_pRefractionDelayExtended, "00", 2);
	//Tb 至 T0 时间,4,00
	memcpy(pSegdSH->m_pTBtoT0TimeExtended, "00", 2);
	//内部时断信号,4,00
	memcpy(pSegdSH->m_pInternalTimeBreakExtended, "00", 2);
	//野外设备中预叠加,4,00
	memcpy(pSegdSH->m_pPrestackWithinFieldUnitsExtended, "00", 2);
	//噪声抑制类型,4,01
	memcpy(pSegdSH->m_pNoiseEliminationTypeExtended, "01", 2);
	//低记录道百分比,4,00
	memcpy(pSegdSH->m_pLowTracePercentageExtended, "00", 2);
	//低记录道值,4,00
	memcpy(pSegdSH->m_pLowTraceValueExtended, "00", 2);
	//窗口数目,4,00
	memcpy(pSegdSH->m_pNumberWindowsOrWindowLengthExtended, "00", 2);
	//历史编辑类型,4,00
	memcpy(pSegdSH->m_pHistoricEditingTypeOrOverlapExtended, "00", 2);
	//噪声记录道百分比,4,00
	memcpy(pSegdSH->m_pNoisyTracePercentageExtended, "00", 2);
	//历史范围,4,00
	memcpy(pSegdSH->m_pHistoricRangeExtended, "00", 2);
	//历史锥形长度 2 的指数,4,00
	memcpy(pSegdSH->m_pHistoricTaperLengthExtended, "00", 2);
	//门槛值不变/ 变化,4,00
	memcpy(pSegdSH->m_pThresholdHoldOrNotExtended, "00", 2);
	//历史门槛值初始值,4,00
	memcpy(pSegdSH->m_pHistoricThresholdInitValueExtended, "00", 2);
	//历史归零长度,4,00
	memcpy(pSegdSH->m_pHistoricZeroingLengthExtended, "00", 2);
	//处理类型,4,01
	memcpy(pSegdSH->m_pTypeProcessExtended, "01", 2);
	//采集类型表,128,00
	memcpy(pSegdSH->m_pAcquisitionTypeTablesExtended, "00", 2);
	//门槛值类型表,128,00
	memcpy(pSegdSH->m_pThresholdTypeTablesExtended, "00", 2);
	//叠加次数,4,01
	memcpy(pSegdSH->m_pStackingFoldExtended, "01", 2);
	//未使用,80,00
	memcpy(pSegdSH->m_pNotUsed1Extended, "02", 2);
	//记录长度,4,4000
	memcpy(pSegdSH->m_pRecordLengthExtended, "4000", 4);
	//自相关峰值时间,4,00
	memcpy(pSegdSH->m_pAutocorrelationPeakTimeExtended, "00", 2);
	//未使用,4,00
	memcpy(pSegdSH->m_pNotUsed2Extended, "00", 2);
	//相关参考信号编号,4,00
	memcpy(pSegdSH->m_pCorrelationPilotNoExtended, "00", 2);
	//参考信号长度,4,00
	memcpy(pSegdSH->m_pPilotLengthExtended, "00", 2);
	//扫描长度,4,00
	memcpy(pSegdSH->m_pSweepLengthExtended, "00", 2);
	//采集号,4,01
	memcpy(pSegdSH->m_pAcquisitionNumberExtended, "01", 2);
	//最多辅助道的最大值,4,3041785.250000
	memcpy(pSegdSH->m_pMaxOfMaxAuxExtended, "3041785.250000", 14);
	//最多地震道最大值,4,-614945.500000
	memcpy(pSegdSH->m_pMaxOfMaxSeisExtended, "-614945.500000", 14);
	//转储叠加次数,4,01
	memcpy(pSegdSH->m_pDumpStackingFoldExtended, "01", 2);
	//磁带标签,16,label
	memcpy(pSegdSH->m_pTapeLabelExtended, "label", 5);
	//磁带号,4,50
	memcpy(pSegdSH->m_pTapeNumberExtended, "50", 2);
	//软件版本,16,4.0
	memcpy(pSegdSH->m_pSoftwareVersionExtended, "4.0", 3);
	//日期,12,05 Jul 2012
	memcpy(pSegdSH->m_pDateExtended, "05 Jul 2012", 11);
	//震源偏东距,8,0
	memcpy(pSegdSH->m_pSourceEastingExtended, "0", 1);
	//震源北向纬度差,8,0
	memcpy(pSegdSH->m_pSourceNorthingExtended, "0", 1);
	//震源高程,4,0.000000
	memcpy(pSegdSH->m_pSourceElevationExtended, "0.000000", 8);
	//所用的滑动扫描模式,4,00
	memcpy(pSegdSH->m_pSlipSweepModeUsedExtended, "00", 2);
	//每个磁带的文件数,4,2000
	memcpy(pSegdSH->m_pFilesPerTapeExtended, "2000", 4);
	//文件计数,4,01
	memcpy(pSegdSH->m_pFileCountExtended, "01", 2);
	//@@@@@未赋值
	char m_pAcquisitionErrorDescriptionExtended[SIZE_BUFFER];//采集错误说明,160,

	//滤波器类型,4,02
	memcpy(pSegdSH->m_pFilterTypeExtended, "02", 2);
	//叠加已转储,4,00
	memcpy(pSegdSH->m_pStackDumpedExtended, "00", 2);
	//叠加符号（当前）,4,01
	memcpy(pSegdSH->m_pStackSignExtended, "01", 2);
	//所用的 PRM 倾斜度校正,4,00
	memcpy(pSegdSH->m_pPRMTiltCorrectionUsedExtended, "00", 2);
	//线束名,64,D5
	memcpy(pSegdSH->m_pSwathNameExtended, "D5", 2);
	//工作模式,4,01
	memcpy(pSegdSH->m_pOperatingModeExtended, "01", 2);
	//保留,4,79
	memcpy(pSegdSH->m_pReserved1Extended, "79", 2);
	//无记录,4,00
	memcpy(pSegdSH->m_pNoLogExtended, "00", 2);
	//监听时间,4,4000
	memcpy(pSegdSH->m_pListeningTimeExtended, "4000", 4);
	//转储类型,4,00
	memcpy(pSegdSH->m_pTypeDumpExtended, "00", 2);
	//保留,4,00
	memcpy(pSegdSH->m_pReserved2Extended, "00", 2);
	//线束 Id,4,01
	memcpy(pSegdSH->m_pSwathIdExtended, "01", 2);
	//,4,00
	memcpy(pSegdSH->m_pSeismicTraceOffsetRemovalDisabledExtended, "00", 2);
	//,8,0003a4ac0c90344f
	memcpy(pSegdSH->m_pGPSTimeAcquisitionTimeBreakExtended, "0003a4ac0c90344f", 16);
	//,8,0000000000000000
	memcpy(pSegdSH->m_pAlignedGPSTimeAcquisitionExtended, "0000000000000000", 16);
	//未使用,132,000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
	memcpy(pSegdSH->m_pNotUsed3Extended, "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", 264);
	
	//@@@@@未赋值
	char* m_pConcatenationOfExternal;//外部头段,m_pExternalHeaderLengthGeneral1*32,*SHOTPRO SP#    1/SL#    3.0/SN#   30.0/SI#1/SEQ# 1/STA:2/CTB:00.000/UH:000.0
}

// 初始化SEGD文件数据道头
void OnInitSegdTH(m_oSegdTraceHeaderStruct* pSegdTH)
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
}
// 保存到Segd文件，成功返回true，失败返回false
bool SaveSegdFile(m_oSegdFileStruct* pSegdFileStruct)
{
	int iCountSeisChannel = 0;

	struct tm *newtime = NULL;
	char tmpbuf[128];
	time_t lt1;
	time(&lt1);
	localtime_s(newtime, &lt1);
	CString strTime = _T("");
	CString strFilePath = _T("");
	CString strPath = _T("");
	FILE* pNewSegdFile;
	string strTemp = "";
	strftime(tmpbuf, 128, "%Y%m%d %I%M%S", newtime);
	strTime.Format(_T("%s"), tmpbuf);
	strPath.Format(_T("%s"), pSegdFileStruct->m_strPath.c_str());
	strTime = strTime + _T(".segd"); 
	strFilePath = strPath + strTime;
	strTemp = (CStringA)strFilePath;
	if((0 != fopen_s(&pNewSegdFile, strTemp.c_str(), "wb+")) == NULL)
	{
		AfxMessageBox(_T("Open new segd failed!\n"));
	}

	ProcessGeneralHeaderBlock1BackNew(pNewSegdFile, pSegdFileStruct);
	ProcessGeneralHeaderBlock2BackNew(pNewSegdFile, pSegdFileStruct);
	ProcessGeneralHeaderBlock3BackNew(pNewSegdFile, pSegdFileStruct);
	for(int i = 1; i< COUNT_SCAN_TYPE_HEADER + 1; i++ )
	{
		ProcessScanTypeHeaderBackNew(pNewSegdFile, i, pSegdFileStruct);
	}
	ProcessExtendedHeaderBackNew(pNewSegdFile, pSegdFileStruct);

	ProcessExternalHeaderBackNew(pNewSegdFile, pSegdFileStruct);

	for(int i = 1; i< pSegdFileStruct->m_oSegdSH.m_iTotalTraceNum + 1; i++)
	{

		iCountSeisChannel++;

		if (iCountSeisChannel > pSegdFileStruct->m_oSegdSH.m_iAcqTraceNum)
		{
			iCountSeisChannel -= pSegdFileStruct->m_oSegdSH.m_iAcqTraceNum;
		}

		ProcessTraceHeaderBackNew(pNewSegdFile, iCountSeisChannel, pSegdFileStruct);
		ProcessTraceHeaderExtensionBlock1BackNew(pNewSegdFile, i, pSegdFileStruct);
		ProcessTraceHeaderExtensionBlock2BackNew(pNewSegdFile, iCountSeisChannel, pSegdFileStruct);
		ProcessTraceHeaderExtensionBlock3BackNew(pNewSegdFile, pSegdFileStruct);
		ProcessTraceHeaderExtensionBlock4BackNew(pNewSegdFile, pSegdFileStruct);
		ProcessTraceHeaderExtensionBlock5BackNew(pNewSegdFile, pSegdFileStruct);
		ProcessTraceHeaderExtensionBlock6BackNew(pNewSegdFile, pSegdFileStruct);
		ProcessTraceHeaderExtensionBlock7BackNew(pNewSegdFile, pSegdFileStruct);

		ProcessTraceDataBackNew(pNewSegdFile, i, pSegdFileStruct);

	}

	if (fclose(pNewSegdFile))
	{
		AfxMessageBox(_T("Close new segd file failed!"));
	}

	return true;
}

void ProcessGeneralHeaderBlock1BackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct)
{

	string strHigh = "";
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pFileNumberGeneral1);
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pFormatCodeGeneral1);
	ReadInitToWriteSegdNew_BCD(pFile, 6, pSegdFileStruct->m_oSegdSH.m_pGeneralConstantsGeneral1);
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pLastTwoDigitsOfYearGeneral1);

	ReadInitToWriteSegdNew_BCD_addHigh(pSegdFileStruct->m_oSegdSH.m_pNumberAdditionalBlocksGeneral1, &strHigh);
	ReadInitToWriteSegdNew_BCD_addLow(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pJulianDayGeneral1, strHigh);

	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pHourOfDayGeneral1);
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pMinuteOfHourGeneral1);
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pSecondOfMinuteGeneral1);
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pManufacturerCodeGeneral1);
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pManufacturerSerialNumberGeneral1);
	ReadInitToWriteSegdNew_BCD(pFile, 3, pSegdFileStruct->m_oSegdSH.m_pBytesPerScanGeneral1);
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pBaseScanIntervalGeneral1);

	ReadInitToWriteSegdNew_BCD_addHigh(pSegdFileStruct->m_oSegdSH.m_pPolarityGeneral1, &strHigh);
	ReadInitToWriteSegdNew_BCD_addLow(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pNotUsedGeneral1, strHigh);

	ReadInitToWriteSegdNew_BCD_addHigh(pSegdFileStruct->m_oSegdSH.m_pRecordTypeGeneral1, &strHigh);
	ReadInitToWriteSegdNew_BCD_addLow(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pRecordLengthGeneral1, strHigh);

	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pScanTypePerRecordGeneral1);
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pNumberChannelSetsPerRecordGeneral1);
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pNumberSampleSkew32ByteExtensionsGeneral1);
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pExtendedHeaderLengthGeneral1);
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pExternalHeaderLengthGeneral1);

}

void ProcessGeneralHeaderBlock2BackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct)
{
	ReadInitToWriteSegdNew_BIN(pFile, 3, pSegdFileStruct->m_oSegdSH.m_pExpandedFileNumberGeneral2);
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pExpandedChannelSetsGeneral2);
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pExtendedHeaderBlocksGeneral2);
	ReadInitToWriteSegdNew_BIN(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pExternalHeaderBlocksGeneral2);
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pNotUsed1General2);
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pSegdRevisionNumberGeneral2);
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pNumberBlocksGeneralTrailerGeneral2);

	if (pSegdFileStruct->m_oSegdSH.m_uiSampleTime == 0)
	{
		ReadInitToWriteSegdNew_BIN(pFile, 3, pSegdFileStruct->m_oSegdSH.m_pExpandedRecordLengthGeneral2);
	}
	else
	{
		ReadParameterToSegd_BIN(pFile, 3, pSegdFileStruct->m_oSegdSH.m_uiSampleTime);
	}

	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pNotUsed2General2);
	ReadInitToWriteSegdNew_BIN(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pGeneralHeaderBlockNumberGeneral2);
	ReadInitToWriteSegdNew_BCD(pFile, 13, pSegdFileStruct->m_oSegdSH.m_pNotUsed3General2);
}

void ProcessGeneralHeaderBlock3BackNew(FILE* pFile,m_oSegdFileStruct* pSegdFileStruct)
{
	ReadInitToWriteSegdNew_BCD(pFile, 3, pSegdFileStruct->m_oSegdSH.m_pExpandedFileNumberGeneral3);
	ReadInitToWriteSegdNew_BCD(pFile, 5, pSegdFileStruct->m_oSegdSH.m_pSourceLineNumberGeneral3);
	ReadInitToWriteSegdNew_BCD(pFile, 5, pSegdFileStruct->m_oSegdSH.m_pSourcePointNumberGeneral3);
	ReadInitToWriteSegdNew_BIN(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pSourcePointIndexGeneral3);
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pPhaseControlGeneral3);
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pVibratorTypeGeneral3);
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pPhaseAngleGeneral3);
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pGeneralHeaderBlockNumberGeneral3);
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pSourceSetNumberGeneral3);
	ReadInitToWriteSegdNew_BCD(pFile, 12, pSegdFileStruct->m_oSegdSH.m_pNotUsedGeneral3);
}

void ProcessScanTypeHeaderBackNew(FILE* pFile, int iChannel, m_oSegdFileStruct* pSegdFileStruct)
{

	string strHigh = "";
	int iNumTrace = 0;
	CString strNumTrace = _T("");

	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pScanTypeHeaderScan[iChannel-1]);
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pChannelSetNumberScan[iChannel-1]);
	ReadInitToWriteSegdNew_BIN(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pChannelSetStartingTimeScan[iChannel-1]);
	ReadInitToWriteSegdNew_BIN(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pChannelSetEndTimeScan[iChannel-1]);
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pDescaleMultiplierScan[iChannel-1]);
	if (iChannel == 1)
	{
		iNumTrace = pSegdFileStruct->m_oSegdSH.m_iAuxTraceNum;
	}
	else if(iChannel == 2)
	{
		iNumTrace = pSegdFileStruct->m_oSegdSH.m_iAcqTraceNum;
	}
	else
	{
		iNumTrace = 0;
	}

	if (iNumTrace == 0)
	{
		ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pNumberChannelsOfChannelSetScan[iChannel-1]);
	}
	else
	{
		ReadParameterToSegd_BCD(pFile, 2, iNumTrace);
	}

	ReadInitToWriteSegdNew_BCD_addHigh(pSegdFileStruct->m_oSegdSH.m_pChannelTypeIdentificationScan[iChannel-1], &strHigh);
	ReadInitToWriteSegdNew_BCD_addLow(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pNotUsedScan[iChannel-1], strHigh);
	ReadInitToWriteSegdNew_BCD_addHigh(pSegdFileStruct->m_oSegdSH.m_pNumberSubscansExponentScan[iChannel-1], &strHigh);
	ReadInitToWriteSegdNew_BCD_addLow(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pChannelGainControlMethodScan[iChannel-1], strHigh);

	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pAliasFilterFrequency3dBPointScan[iChannel-1]);
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pAliasFilterSlopeScan[iChannel-1]);
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pLowCutFilterFrequencyScan[iChannel-1]);
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pLowCutFilterSlopeScan[iChannel-1]);
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pFirstNotchFrequencyScan[iChannel-1]);
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pSecondNotchFrequencyScan[iChannel-1]);
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pThirdNotchFrequencyScan[iChannel-1]);
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pExtendedChannelSetNumberScan[iChannel-1]);

	ReadInitToWriteSegdNew_BCD_addHigh(pSegdFileStruct->m_oSegdSH.m_pExtendedHeaderFlagScan[iChannel-1], &strHigh);
	ReadInitToWriteSegdNew_BCD_addLow(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pTraceHeaderExtensionsScan[iChannel-1], strHigh);

	ReadInitToWriteSegdNew_BIN(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pVerticalStackScan[iChannel-1]);
	ReadInitToWriteSegdNew_BIN(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pStreamerCableNumberScan[iChannel-1]);
	ReadInitToWriteSegdNew_BIN(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pArrayFormingScan[iChannel-1]);
}

void ProcessExtendedHeaderBackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct)
{

	if (pSegdFileStruct->m_oSegdSH.m_uiSampleTime == 0)
	{
		pSegdFileStruct->m_oSegdSH.m_uiSampleTime = ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pAcquisitionLengthExtended);
	}
	else
	{
		ReadParameterToSegd_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_uiSampleTime);
	}

	if (pSegdFileStruct->m_oSegdSH.m_uiSampleRate == 0)
	{
		pSegdFileStruct->m_oSegdSH.m_uiSampleRate = ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pSampleRateExtended);
	}
	else
	{
		ReadParameterToSegd_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_uiSampleRate);
	}


	if (pSegdFileStruct->m_oSegdSH.m_iTotalTraceNum == 0)
	{
		pSegdFileStruct->m_oSegdSH.m_iTotalTraceNum = ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pTotalNumberTracesExtended);
	}
	else
	{
		ReadParameterToSegd_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_iTotalTraceNum);
	}

	if (pSegdFileStruct->m_oSegdSH.m_iAuxTraceNum == 0)
	{
		pSegdFileStruct->m_oSegdSH.m_iAuxTraceNum = ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pNumberAuxesExtended);
	}
	else
	{
		ReadParameterToSegd_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_iAuxTraceNum);
	}

	if (pSegdFileStruct->m_oSegdSH.m_iAcqTraceNum == 0)
	{
		pSegdFileStruct->m_oSegdSH.m_iAcqTraceNum = ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pNumberSeisTracesExtended);
	} 
	else
	{
		ReadParameterToSegd_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_iAcqTraceNum);
	}

	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pNumberDeadSeisTracesExtended);


	if (pSegdFileStruct->m_oSegdSH.m_iAcqTraceNum == 0)
	{
		ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pNumberLiveSeisTracesExtended);
	}
	else
	{
		ReadParameterToSegd_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_iAcqTraceNum);
	}

	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pTypeSourceExtended);


	if (pSegdFileStruct->m_oSegdSH.m_iSampleLength == 0)
	{
		pSegdFileStruct->m_oSegdSH.m_iSampleLength = ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pNumberSamplesInTraceExtended);
	}
	else
	{
		ReadParameterToSegd_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_iSampleLength);
	}


	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pShotNumberExtended);
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pTBWindowExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pTestRecordTypeExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pSpreadFirstLineExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pSpreadFirstNumberExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pSpreadNumberExtended);

	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pSpreadTypeExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pTimebreakExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pUpholeTimeExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pBlasterIdExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pBlasterStatusExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pRefractionDelayExtended);

	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pTBtoT0TimeExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pInternalTimeBreakExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pPrestackWithinFieldUnitsExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pNoiseEliminationTypeExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pLowTracePercentageExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pLowTraceValueExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pNumberWindowsOrWindowLengthExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pHistoricEditingTypeOrOverlapExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pNoisyTracePercentageExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pHistoricRangeExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pHistoricTaperLengthExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pThresholdHoldOrNotExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pHistoricThresholdInitValueExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pHistoricZeroingLengthExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pTypeProcessExtended);    //36*4=144
	ReadInitToWriteSegdNew_BIN(pFile, 128, pSegdFileStruct->m_oSegdSH.m_pAcquisitionTypeTablesExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 128, pSegdFileStruct->m_oSegdSH.m_pThresholdTypeTablesExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pStackingFoldExtended);
	ReadInitToWriteSegdNew_ASC(pFile, 80, pSegdFileStruct->m_oSegdSH.m_pNotUsed1Extended);

	if (pSegdFileStruct->m_oSegdSH.m_uiSampleTime == 0)
	{
		ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pRecordLengthExtended);
	}
	else
	{
		ReadParameterToSegd_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_uiSampleTime);
	}

	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pAutocorrelationPeakTimeExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pNotUsed2Extended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pCorrelationPilotNoExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pPilotLengthExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pSweepLengthExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pAcquisitionNumberExtended);
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pMaxOfMaxAuxExtended);
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pMaxOfMaxSeisExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pDumpStackingFoldExtended);
	ReadInitToWriteSegdNew_ASC(pFile, 16, pSegdFileStruct->m_oSegdSH.m_pTapeLabelExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pTapeNumberExtended);
	ReadInitToWriteSegdNew_ASC(pFile, 16, pSegdFileStruct->m_oSegdSH.m_pSoftwareVersionExtended);
	ReadInitToWriteSegdNew_ASC(pFile, 12, pSegdFileStruct->m_oSegdSH.m_pDateExtended);
	ReadInitToWriteSegdNew_DOUBLE(pFile, 8, pSegdFileStruct->m_oSegdSH.m_pSourceEastingExtended);
	ReadInitToWriteSegdNew_DOUBLE(pFile, 8, pSegdFileStruct->m_oSegdSH.m_pSourceNorthingExtended);
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pSourceElevationExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pSlipSweepModeUsedExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pFilesPerTapeExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pFileCountExtended);
	ReadInitToWriteSegdNew_ASC(pFile, 160, pSegdFileStruct->m_oSegdSH.m_pAcquisitionErrorDescriptionExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pFilterTypeExtended);

	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pStackDumpedExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pStackSignExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pPRMTiltCorrectionUsedExtended);
	ReadInitToWriteSegdNew_ASC(pFile, 64, pSegdFileStruct->m_oSegdSH.m_pSwathNameExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pOperatingModeExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pReserved1Extended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pNoLogExtended);

	if (pSegdFileStruct->m_oSegdSH.m_uiSampleTime == 0)
	{
		ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pListeningTimeExtended);
	}
	else
	{
		ReadParameterToSegd_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_uiSampleTime);
	}

	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pTypeDumpExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pReserved2Extended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pSwathIdExtended);
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pSeismicTraceOffsetRemovalDisabledExtended);
	ReadInitToWriteSegdNew_BCD(pFile, 8, pSegdFileStruct->m_oSegdSH.m_pGPSTimeAcquisitionTimeBreakExtended);
	ReadInitToWriteSegdNew_BCD(pFile, 8, pSegdFileStruct->m_oSegdSH.m_pAlignedGPSTimeAcquisitionExtended);
	ReadInitToWriteSegdNew_BCD(pFile, 132, pSegdFileStruct->m_oSegdSH.m_pNotUsed3Extended);
}

void ProcessExternalHeaderBackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct)
{
	int iLenth = 0;
	int iLenthExternalHeader = ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pExternalHeaderLengthGeneral1);
	iLenth = iLenthExternalHeader * 32;
	ReadInitToWriteSegdNew_ASC(pFile, iLenth, pSegdFileStruct->m_oSegdSH.m_pConcatenationOfExternal);
}

void ProcessTraceHeaderBackNew(FILE* pFile, int iChannel, m_oSegdFileStruct* pSegdFileStruct)
{

	ReadInitToWriteSegdNew_BCD(pFile, 2, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pFileNumberTrace);
	ReadInitToWriteSegdNew_BCD(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pScanTypeNumberTrace);
	ReadInitToWriteSegdNew_BCD(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pChannelSetNumberTrace);

	if (iChannel == 0)
	{
		ReadInitToWriteSegdNew_BCD(pFile, 2, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pTraceNumberTrace);
	}
	else
	{
		ReadParameterToSegd_BCD(pFile, 2, iChannel);
	}

	ReadInitToWriteSegdNew_BIN(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pFirstTimingWordTrace);
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pTraceHeaderExtensionTrace);
	ReadInitToWriteSegdNew_BCD(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pSampleSkewTrace);
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pTraceEditTrace);
	ReadInitToWriteSegdNew_BIN(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pTimeBreakWindowTrace);
	ReadInitToWriteSegdNew_BCD(pFile, 2, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pExtendedChannelSetNumberTrace);
	ReadInitToWriteSegdNew_BCD(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pExtendedFileNumberTrace);
}

void ProcessTraceHeaderExtensionBlock1BackNew(FILE* pFile, int iChannel, m_oSegdFileStruct* pSegdFileStruct)//ichannel 为点号，1-98
{

	ReadParameterToSegd_BIN(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_iLineIndex);

	if (iChannel == 0)
	{
		ReadInitToWriteSegdNew_BIN(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pReceiverPointNumberTrace1);
	}
	else
	{
		ReadParameterToSegd_BIN(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_iPointIndex);
	}

	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pReceiverPointIndexTrace1);

	if (pSegdFileStruct->m_oSegdSH.m_iSampleLength == 0)
	{
		ReadInitToWriteSegdNew_BIN(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pNumberSamplesPerTraceTrace1);
	} 
	else
	{
		ReadParameterToSegd_BIN(pFile, 3, pSegdFileStruct->m_oSegdSH.m_iSampleLength);
	}

	ReadInitToWriteSegdNew_BCD(pFile, 5, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pExtendedReceiverLineNumberTrace1);
	ReadInitToWriteSegdNew_BCD(pFile, 5, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pExtendedReceiverPointNumberTrace1);
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pSensorSEGDCodeTrace1);
	ReadInitToWriteSegdNew_BCD(pFile, 11, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pNotUsedTrace1);
}

void ProcessTraceHeaderExtensionBlock2BackNew(FILE* pFile, int iChannel, m_oSegdFileStruct* pSegdFileStruct)
{

	ReadInitToWriteSegdNew_DOUBLE(pFile, 8, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pReceiverPointEastingTrace2);
	ReadInitToWriteSegdNew_DOUBLE(pFile, 8, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pReceiverPointNorthingTrace2);
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pReceiverPointElevationTrace2);
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pSensorTypeNumberTrace2);
	ReadInitToWriteSegdNew_BCD(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pNotUsedTrace2);
	ReadInitToWriteSegdNew_BIN(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pDSDIdentificationNoTrace2);

	if (iChannel == 0)
	{
		ReadInitToWriteSegdNew_BIN(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pExtendedTraceNoTrace2);
	}
	else
	{
		ReadParameterToSegd_BIN(pFile, 4, iChannel);
	}

}

void ProcessTraceHeaderExtensionBlock3BackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct)
{

	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pResistanceLowLimitTrace3);
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pResistanceHighLimitTrace3);
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pResistanceResistanceValueTrace3);
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pTiltLimitTrace3);
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pTiltValueTrace3);
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pResistanceErrorTrace3);
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pTiltErrorTrace3);
	ReadInitToWriteSegdNew_BCD(pFile, 10, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pNotUsedTrace3);
}

void ProcessTraceHeaderExtensionBlock4BackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct)
{

	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pCapacitanceLowLimitTrace4);
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pCapacitanceHighLimitTrace4);
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pCapacitanceValueTrace4);
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pCutOffLowLimitTrace4);
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pCutOffHighLimitsTrace4);
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pCutOffValueTrace4);
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pCapacitanceErrorTrace4);
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pCutOffErrorTrace4);
	ReadInitToWriteSegdNew_BCD(pFile, 6, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pNotUsedTrace4);
}

void ProcessTraceHeaderExtensionBlock5BackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct)
{

	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pLeakageLimitTrace5);
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pLeakageValueTrace5);
	ReadInitToWriteSegdNew_BCD(pFile, 16, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pNotUsed1Trace5);
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pLeakageErrorTrace5);
	ReadInitToWriteSegdNew_BCD(pFile, 7, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pNotUsed2Trace5);

}

void ProcessTraceHeaderExtensionBlock6BackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct)
{

	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pUnitTypeTrace6);
	ReadInitToWriteSegdNew_BIN(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pUnitSerialNumberTrace6);
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pChannelNumberTrace6);
	ReadInitToWriteSegdNew_BCD(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pSpare1Trace6);
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pAssemblyTypeTrace6);
	ReadInitToWriteSegdNew_BIN(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pFDUorDSUAssemblySerialNumberTrace6);
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pLocationInFDUorDSUAssemblyTrace6);
	ReadInitToWriteSegdNew_BCD(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pSpare2Trace6);
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pSubunitTypeTrace6);

	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pChannelTypeTrace6);
	ReadInitToWriteSegdNew_BCD(pFile, 2, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pSpare3Trace6);
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pSensorSensitivityTrace6);
	ReadInitToWriteSegdNew_BCD(pFile, 8, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pNotUsedTrace6);

}

void ProcessTraceHeaderExtensionBlock7BackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct)
{

	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pControlUnitTypeTrace7);
	ReadInitToWriteSegdNew_BIN(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pControlUnitSerialNumberTrace7);
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pChannelGainScaleTrace7);
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pChannelFilterTrace7);

	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pChannelDataErrorTrace7);
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pChannelEditedStatusTrace7);
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pChannelSampleTomVConversionFactorTrace7);
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pNumberOfStacksNoisyTrace7);
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pNumberOfStacksLowTrace7);
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pChannelTypeIdTrace7);
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pChannelProcessTrace7);
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pTraceMaxValueTrace7);
	ReadInitToWriteSegdNew_BIN(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pTraceMaxTimeTrace7);
	ReadInitToWriteSegdNew_BIN(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pNumberInterpolationsTrace7);
	ReadInitToWriteSegdNew_BIN(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pSeismicTraceOffsetValueTrace7);

	pSegdFileStruct->m_pSegdDHList->pop_front();

}

void ProcessTraceDataBackNew(FILE* pFile, int iInstrNo, m_oSegdFileStruct* pSegdFileStruct)
{
	for(int i = 0; i<pSegdFileStruct->m_oSegdSH.m_iSampleLength; i++)
	{
		ReadArrayToWriteSegdData_FLOAT(pFile, iInstrNo, i, pSegdFileStruct);
	}
}

void ReadArrayToWriteSegdData_FLOAT(FILE* pFile, int iInstrNo, int iDataNo, m_oSegdFileStruct* pSegdFileStruct)
{
	int iResultNew = 0;
	int iTemp = 0;
	CString strTemp = _T("");
	float fTemp = 0;

	char charByte[SIZE_BUFFER];
	char charFinal[SIZE_BUFFER];
	unsigned char charTemp[SIZE_BUFFER];
	memset(charTemp, 0, sizeof(unsigned char)*SIZE_BUFFER);
	memset(charByte, 0, sizeof(char)*SIZE_BUFFER);
	memset(charFinal, 0, sizeof(char)*SIZE_BUFFER);


	memmove(charTemp, &pSegdFileStruct->m_pSegdDataBuf[(iInstrNo-1)*pSegdFileStruct->m_oSegdSH.m_iSampleLength*3 + 3*iDataNo], 3);
	memmove(&iTemp, charTemp, 3);
	if (iTemp - 0x800000 > 0) 
	{
		iTemp = iTemp - 16777216;
	}
	fTemp = (float)(iTemp);

	//float转4byte char，且4byte位置倒置
	memcpy(charByte, (char*)&fTemp, 4);
	for (int i = 0; i<4; i++)
	{
		charFinal[i] = charByte[4-i-1];
	}

	iResultNew = fseek(pFile, 0, SEEK_CUR);
	fwrite(charFinal, sizeof(char), 4, pFile);

}

int ReadInitToWriteSegdNew_BCD(FILE* pFile, int iCountByte, char* pParameter)
{
	int iResultNew = 0;
	int iTemp = 0;
	CString strTemp = _T("");
	int iLenth = 0;

	char charByte[SIZE_BUFFER];
	memset(charByte, 0, sizeof(char)*SIZE_BUFFER);

	strTemp = CString(pParameter);
	for (int i = 0; i<iCountByte; i++)
	{
		_stscanf_s(strTemp.Mid(i*2, 2), _T("%02x"), &iTemp);
		charByte[i] = (char)iTemp;
	}
	iLenth = _ttoi(strTemp);

	iResultNew = fseek(pFile, 0, SEEK_CUR);
	fwrite(charByte, sizeof(char), iCountByte, pFile);

	return iLenth;
}

int ReadInitToWriteSegdNew_BIN(FILE* pFile, int iCountByte, char* pParameter)
{
	int iResultNew = 0;
	int iTemp = 0;
	CString strTemp = _T("");
	int iReturn = 0;
	char charByte[SIZE_BUFFER];
	memset(charByte, 0, sizeof(char)*SIZE_BUFFER);
	iReturn = _ttoi(CString(pParameter));
	if(iCountByte == 1)
	{
		strTemp.Format(_T("%02x"), iReturn);
	}
	else if(iCountByte == 2)
	{
		strTemp.Format(_T("%04x"), iReturn);
	}
	else if(iCountByte == 3)
	{
		strTemp.Format(_T("%06x"), iReturn);
	}
	else if(iCountByte == 4)
	{
		strTemp.Format(_T("%08x"), iReturn);
	}

	for(int i = 0; i< iCountByte; i++)
	{
		_stscanf_s(strTemp.Mid(i*2, 2), _T("%02x"), &iTemp);
		charByte[i] = (char)iTemp;
	}

	iResultNew = fseek(pFile, 0, SEEK_CUR);
	fwrite(charByte, sizeof(char), iCountByte, pFile);

	return iReturn;
}

void ReadInitToWriteSegdNew_BCD_addLow(FILE* pFile, int iCountByte, char* pParameter, string strHigh)
{
	int iResultNew = 0;
	CString strTemp = _T("");
	int iTemp = 0;
	CString cstrHigh = _T("");

	char charByte[SIZE_BUFFER];
	memset(charByte, 0, sizeof(char)*SIZE_BUFFER);

	cstrHigh.Format(_T("%s"), strHigh);

	strTemp = cstrHigh + CString(pParameter);

	for (int i = 0; i<iCountByte; i++)
	{
		_stscanf_s(strTemp.Mid(i*2, 2), _T("%02x"), &iTemp);
		charByte[i] = (char)iTemp;
	}

	iResultNew = fseek(pFile, 0, SEEK_CUR);
	fwrite(charByte, sizeof(char), iCountByte, pFile);
}

void ReadInitToWriteSegdNew_BCD_addHigh(char* pParameter, string* pstr)
{
	(*pstr).assign(pParameter, strlen(pParameter));
}

void ReadInitToWriteSegdNew_FLOAT(FILE* pFile, int iCountByte, char* pParameter)
{
	int iResultNew = 0;
	CString strTemp = _T("");
	float fTemp = 0;

	char charByte[SIZE_BUFFER];
	char charFinal[SIZE_BUFFER];
	memset(charByte, 0, sizeof(char)*SIZE_BUFFER);
	memset(charFinal, 0, sizeof(char)*SIZE_BUFFER);

	if (CString(pParameter) == "N+")
	{
		charFinal[0] = 0x7f;
		charFinal[1] = 0xff;
		charFinal[2] = 0xff;
		charFinal[3] = 0xff;
	} 
	else if (CString(pParameter) == "N-")
	{
		charFinal[0] = 0xff;
		charFinal[1] = 0xff;
		charFinal[2] = 0xff;
		charFinal[3] = 0xff;
	}
	else
	{
		fTemp = (float)_ttof(CString(pParameter));
		memcpy(charByte, (char*)&fTemp, iCountByte);
		for (int i = 0; i<iCountByte; i++)
		{
			charFinal[i] = charByte[iCountByte-i-1];
		}
	}
	iResultNew = fseek(pFile, 0, SEEK_CUR);
	fwrite(charFinal, sizeof(char), iCountByte, pFile);

}

void ReadInitToWriteSegdNew_DOUBLE(FILE* pFile, int iCountByte, char* pParameter)
{
	int iResultNew = 0;
	CString strTemp = _T("");
	double dTemp = 0;

	char charByte[SIZE_BUFFER];
	char charFinal[SIZE_BUFFER];
	memset(charByte, 0, sizeof(char)*SIZE_BUFFER);
	memset(charFinal, 0, sizeof(char)*SIZE_BUFFER);

	dTemp = _ttof(CString(pParameter));
	memcpy(charByte, (char*)&dTemp, iCountByte);
	for (int i = 0; i<iCountByte; i++) 
	{
		charFinal[i] = charByte[iCountByte-i-1];
	}

	iResultNew = fseek(pFile, 0, SEEK_CUR);
	fwrite(charFinal, sizeof(char), iCountByte, pFile);
}

void ReadInitToWriteSegdNew_ASC(FILE* pFile, int iCountByte, char* pParameter)
{
	int iResultNew = 0;
	CString strTemp = _T("");

	char charByte[1024];
	memset(charByte, 0, sizeof(char)*1024);
	if (iCountByte > 0)
	{

		iResultNew = fseek(pFile, 0, SEEK_CUR);
		fwrite(pParameter, sizeof(char), iCountByte, pFile);
	}
}

void ReadParameterToSegd_BIN(FILE* pFile, int iCountByte, int iValue)
{

	int iResultNew = 0;
	int iTemp = 0;
	CString strTemp = _T("");

	char charByte[SIZE_BUFFER];
	memset(charByte, 0, sizeof(char)*SIZE_BUFFER);

	if(iCountByte == 1)
	{
		strTemp.Format(_T("%02x"), iValue);
	}
	else if(iCountByte == 2)
	{
		strTemp.Format(_T("%04x"), iValue);
	}
	else if(iCountByte == 3)
	{
		strTemp.Format(_T("%06x"), iValue);
	}
	else if(iCountByte == 4)
	{
		strTemp.Format(_T("%08x"), iValue);
	}

	for(int i = 0; i< iCountByte; i++)
	{
		_stscanf_s(strTemp.Mid(i*2, 2), _T("%02x"), &iTemp);
		charByte[i] = (char)iTemp;
	}

	iResultNew = fseek(pFile, 0, SEEK_CUR);
	fwrite(charByte, sizeof(char), iCountByte, pFile);

}

void ReadParameterToSegd_BCD(FILE* pFile, int iCountByte, int iValue)
{

	int iResultNew = 0;
	CString strTemp = _T("");
	int iTemp = 0;
	CString strZero = _T("");
	char charByte[SIZE_BUFFER];
	memset(charByte, 0, sizeof(char)*SIZE_BUFFER);

	strTemp.Format(_T("%d"), iValue);
	for (int i = 0; i<iCountByte *2 - (int)_tcslen(strTemp); i++)
	{
		strZero += _T("0");
	}
	strTemp = strZero + strTemp;

	for (int i = 0; i<iCountByte; i++)
	{
		_stscanf_s(strTemp.Mid(i*2, 2), _T("%02x"), &iTemp);
		charByte[i] = (char)iTemp;
	}
	iResultNew = fseek(pFile, 0, SEEK_CUR);
	fwrite(charByte, sizeof(char), iCountByte, pFile);
}