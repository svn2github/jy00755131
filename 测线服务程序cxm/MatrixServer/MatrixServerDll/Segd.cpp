#include "stdafx.h"
#include "MatrixServerDll.h"
// 初始化SEGD文件标准头
void OnInitSegdSH(m_oSegdStandardHeaderStruct* pSegdSH)
{
	//说明，字节数，数据
	//四位数文件编号 (0-9999),2,0327
	pSegdSH->m_pFileNumberGeneral1 = "0327";
	//格式代码,2,8058
	pSegdSH->m_pFormatCodeGeneral1 = "8058";
	//普通常数,6,000000000005
	pSegdSH->m_pGeneralConstantsGeneral1 = "000000000005";
	//年份的最后两位数(0-99),1,12
	pSegdSH->m_pLastTwoDigitsOfYearGeneral1 = "12";
	//普通头段中额外数据块数目,H,2
	pSegdSH->m_pNumberAdditionalBlocksGeneral1 = "2";
	//儒略日，3 位数(1-366),L+1,187
	pSegdSH->m_pJulianDayGeneral1 = "187";
	//一天中的小时（0-23）,1,08
	pSegdSH->m_pHourOfDayGeneral1 = "08";
	//小时中的分钟（0-59）,1,36
	pSegdSH->m_pMinuteOfHourGeneral1 = "36";
	//分钟中的秒钟（0-59）,1,06
	pSegdSH->m_pSecondOfMinuteGeneral1 = "06";
	//制造商代码,1,13
	pSegdSH->m_pManufacturerCodeGeneral1 = "13";
	//制造商序列号,2,0000
	pSegdSH->m_pManufacturerSerialNumberGeneral1 = "0000";
	//每次扫描字节数,3,000000
	pSegdSH->m_pBytesPerScanGeneral1 = "000000";
	//基本扫描间隔,1,10
	pSegdSH->m_pBaseScanIntervalGeneral1 = "10";
	//极性,H,0
	pSegdSH->m_pPolarityGeneral1 = "0";
	//未使用,L+1,000
	pSegdSH->m_pNotUsedGeneral1 = "000";
	//记录类型,H,8
	pSegdSH->m_pRecordTypeGeneral1 = "8";
	//记录长度,L+1,fff
	pSegdSH->m_pRecordLengthGeneral1 = "fff";
	//每个记录的扫描类型,1,01
	pSegdSH->m_pScanTypePerRecordGeneral1 = "01";
	//每个记录的地震道组数目,1,16
	pSegdSH->m_pNumberChannelSetsPerRecordGeneral1 = "16";
	//采样时滞 32 字节扩展名数目,1,00
	pSegdSH->m_pNumberSampleSkew32ByteExtensionsGeneral1 = "00";
	//扩展头段长度,1,32
	pSegdSH->m_pExtendedHeaderLengthGeneral1 = "32";
	//外部头段长度,1,32
	pSegdSH->m_pExternalHeaderLengthGeneral1 = "32";
	
	//扩展文件编号,3,327
	pSegdSH->m_pExpandedFileNumberGeneral2 = "327";
	//扩展地震道组/ 扫描类型（未使用）,2,0000
	pSegdSH->m_pExpandedChannelSetsGeneral2 = "0000";
	//扩展头段数据块（未使用）,2,0000
	pSegdSH->m_pExtendedHeaderBlocksGeneral2 = "0000";
	//外部头段数据块,2,00
	pSegdSH->m_pExternalHeaderBlocksGeneral2 = "00";
	//未使用,1,00
	pSegdSH->m_pNotUsed1General2 = "00";
	//SEG-D 版本号,2,0201
	pSegdSH->m_pSegdRevisionNumberGeneral2 = "0201";
	//普通尾段数据块数目,2,0000
	pSegdSH->m_pNumberBlocksGeneralTrailerGeneral2 = "0000";
	//扩展记录长度（0-128000 毫秒）,3,4000
	pSegdSH->m_pExpandedRecordLengthGeneral2 = "4000";
	//未使用,1,00
	pSegdSH->m_pNotUsed2General2 = "00";
	//普通头段数据块编号,1,02
	pSegdSH->m_pGeneralHeaderBlockNumberGeneral2 = "02";
	//未使用,13,00000000000000000000000000
	pSegdSH->m_pNotUsed3General2 = "00000000000000000000000000";
	//扩展文件编号,3,000147
	pSegdSH->m_pExpandedFileNumberGeneral3 = "000147";
	//震源测线号(0-99999.99),5,0000030000
	pSegdSH->m_pSourceLineNumberGeneral3 = "0000030000";
	//震源点号(0-99999.99),5,00001e0000
	pSegdSH->m_pSourcePointNumberGeneral3 = "00001e0000";
	//震源点索引 (1-9),1,01
	pSegdSH->m_pSourcePointIndexGeneral3 = "01";
	//相位控制（未记录的）,1,00
	pSegdSH->m_pPhaseControlGeneral3 = "00";
	//可控震源类型（未记录的）,1,00
	pSegdSH->m_pVibratorTypeGeneral3 = "00";
	//相位角（未记录的）,2,0000
	pSegdSH->m_pPhaseAngleGeneral3 = "0000";
	//普通头段数据块编号,1,03
	pSegdSH->m_pGeneralHeaderBlockNumberGeneral3 = "03";
	//震源组号,1,01
	pSegdSH->m_pSourceSetNumberGeneral3 = "01";
	//未使用,12,000000000000000000000000
	pSegdSH->m_pNotUsedGeneral3 = "000000000000000000000000";

	//说明，字节数，数据1，数据2，数据3，此后均同数据3至数据16
	//扫描类型头段,1,01,01,00...
	pSegdSH->m_pScanTypeHeaderScan[0] = "01";
	pSegdSH->m_pScanTypeHeaderScan[1] = "01";
	for (int i = 2; i < 16; i++)
	{
		pSegdSH->m_pScanTypeHeaderScan[i] = "00";
	}
	//地震道组号,1,01,02,00...
	pSegdSH->m_pChannelSetNumberScan[0] = "01";
	pSegdSH->m_pChannelSetNumberScan[1] = "02";
	for (int i = 2; i < 16; i++)
	{
		pSegdSH->m_pChannelSetNumberScan[i] = "00";
	}
	//地震道组起始时间,2,00,00,00...
	for (int i = 0; i < 16; i++)
	{
		pSegdSH->m_pChannelSetStartingTimeScan[i] = "00";
	}
	//地震道组结束时间,2,2000,2000,00...
	pSegdSH->m_pChannelSetEndTimeScan[0] = "2000";
	pSegdSH->m_pChannelSetEndTimeScan[1] = "2000";
	for (int i = 2; i < 16; i++)
	{
		pSegdSH->m_pChannelSetEndTimeScan[i] = "00";
	}
	//缩小比例乘数,2,6db7,6db7,0000...
	pSegdSH->m_pDescaleMultiplierScan[0] = "6db7";
	pSegdSH->m_pDescaleMultiplierScan[1] = "6db7";
	for (int i = 2; i < 16; i++)
	{
		pSegdSH->m_pDescaleMultiplierScan[i] = "0000";
	}
	//此地震道组中地震道数目,2,0002,0012,0000...
	pSegdSH->m_pNumberChannelsOfChannelSetScan[0] = "0002";
	pSegdSH->m_pNumberChannelsOfChannelSetScan[1] = "0012";
	for (int i = 2; i < 16; i++)
	{
		pSegdSH->m_pNumberChannelsOfChannelSetScan[i] = "0000";
	}
	//地震道类型标识,H,9,1,0...
	pSegdSH->m_pChannelTypeIdentificationScan[0] = "9";
	pSegdSH->m_pChannelTypeIdentificationScan[1] = "1";
	for (int i = 2; i < 16; i++)
	{
		pSegdSH->m_pChannelTypeIdentificationScan[i] = "0";
	}
	//未使用,L,0,0,0...
	for (int i = 0; i < 16; i++)
	{
		pSegdSH->m_pNotUsedScan[i] = "0";
	}
	//副扫描指数数目,H,0,0,0...
	for (int i = 0; i < 16; i++)
	{
		pSegdSH->m_pNumberSubscansExponentScan[i] = "0";
	}

	char m_pChannelGainControlMethodScan[16][SIZE_BUFFER];//地震道增益控制方法（固定增益）,L,3,3,0...
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
	pSegdSH->m_pAcquisitionLengthExtended = "4000";
	//采样率,4,1000
	pSegdSH->m_pSampleRateExtended = "1000";
	//记录道总数,4,14
	pSegdSH->m_pTotalNumberTracesExtended = "14";
	//辅助道数目,4,02
	pSegdSH->m_pNumberAuxesExtended = "02";
	//地震记录道数目,4,12
	pSegdSH->m_pNumberSeisTracesExtended = "12";
	//停滞地震记录道数目,4,00
	pSegdSH->m_pNumberDeadSeisTracesExtended = "00";
	//活动地震记录道数目,4,12
	pSegdSH->m_pNumberLiveSeisTracesExtended = "12";
	//震源类型,4,01
	pSegdSH->m_pTypeSourceExtended = "01";
	//记录道中样本数目,4,4001
	pSegdSH->m_pNumberSamplesInTraceExtended = "4001";
	//炮号,4,01
	pSegdSH->m_pShotNumberExtended = "01";
	//TB 窗口,4,3000.000000
	pSegdSH->m_pTBWindowExtended = "3000.000000";
	//测试记录类型,4,00
	pSegdSH->m_pTestRecordTypeExtended = "00";
	//线束第一条测线,4,01
	pSegdSH->m_pSpreadFirstLineExtended = "01";
	//线束第一个号码,4,34
	pSegdSH->m_pSpreadFirstNumberExtended = "34";
	//排列编号,4,01
	pSegdSH->m_pSpreadNumberExtended = "01";
	//排列类型,4,01
	pSegdSH->m_pSpreadTypeExtended = "01";
	//时断信号,4,00
	pSegdSH->m_pTimebreakExtended = "00";
	//井口时间,4,00
	pSegdSH->m_pUpholeTimeExtended = "00";
	//爆炸机标识号,4,01
	pSegdSH->m_pBlasterIdExtended = "01";
	//爆炸机状态,4,02
	pSegdSH->m_pBlasterStatusExtended = "02";
	//折射延迟,4,00
	pSegdSH->m_pRefractionDelayExtended = "00";
	//Tb 至 T0 时间,4,00
	pSegdSH->m_pTBtoT0TimeExtended = "00";
	//内部时断信号,4,00
	pSegdSH->m_pInternalTimeBreakExtended = "00";
	//野外设备中预叠加,4,00
	pSegdSH->m_pPrestackWithinFieldUnitsExtended = "00";
	//噪声抑制类型,4,01
	pSegdSH->m_pNoiseEliminationTypeExtended = "01";
	//低记录道百分比,4,00
	pSegdSH->m_pLowTracePercentageExtended = "00";
	//低记录道值,4,00
	pSegdSH->m_pLowTraceValueExtended = "00";
	//窗口数目,4,00
	pSegdSH->m_pNumberWindowsOrWindowLengthExtended = "00";
	//历史编辑类型,4,00
	pSegdSH->m_pHistoricEditingTypeOrOverlapExtended = "00";
	//噪声记录道百分比,4,00
	pSegdSH->m_pNoisyTracePercentageExtended = "00";
	//历史范围,4,00
	pSegdSH->m_pHistoricRangeExtended = "00";
	//历史锥形长度 2 的指数,4,00
	pSegdSH->m_pHistoricTaperLengthExtended = "00";
	//门槛值不变/ 变化,4,00
	pSegdSH->m_pThresholdHoldOrNotExtended = "00";
	//历史门槛值初始值,4,00
	pSegdSH->m_pHistoricThresholdInitValueExtended = "00";
	//历史归零长度,4,00
	pSegdSH->m_pHistoricZeroingLengthExtended = "00";
	//处理类型,4,01
	pSegdSH->m_pTypeProcessExtended = "01";
	//采集类型表,128,00
	pSegdSH->m_pAcquisitionTypeTablesExtended = "00";
	//门槛值类型表,128,00
	pSegdSH->m_pThresholdTypeTablesExtended = "00";
	//叠加次数,4,01
	pSegdSH->m_pStackingFoldExtended = "01";
	//未使用,80,00
	pSegdSH->m_pNotUsed1Extended = "02";
	//记录长度,4,4000
	pSegdSH->m_pRecordLengthExtended = "4000";
	//自相关峰值时间,4,00
	pSegdSH->m_pAutocorrelationPeakTimeExtended = "00";
	//未使用,4,00
	pSegdSH->m_pNotUsed2Extended = "00";
	//相关参考信号编号,4,00
	pSegdSH->m_pCorrelationPilotNoExtended = "00";
	//参考信号长度,4,00
	pSegdSH->m_pPilotLengthExtended = "00";
	//扫描长度,4,00
	pSegdSH->m_pSweepLengthExtended = "00";
	//采集号,4,01
	pSegdSH->m_pAcquisitionNumberExtended = "01";
	//最多辅助道的最大值,4,3041785.250000
	pSegdSH->m_pMaxOfMaxAuxExtended = "3041785.250000";
	//最多地震道最大值,4,-614945.500000
	pSegdSH->m_pMaxOfMaxSeisExtended = "-614945.500000";
	//转储叠加次数,4,01
	pSegdSH->m_pDumpStackingFoldExtended = "01";
	//磁带标签,16,label
	pSegdSH->m_pTapeLabelExtended = "label";
	//磁带号,4,50
	pSegdSH->m_pTapeNumberExtended = "50";
	//软件版本,16,4.0
	pSegdSH->m_pSoftwareVersionExtended = "4.0";
	//日期,12,05 Jul 2012
	pSegdSH->m_pDateExtended = "05 Jul 2012";
	//震源偏东距,8,0
	pSegdSH->m_pSourceEastingExtended = "0";
	//震源北向纬度差,8,0
	pSegdSH->m_pSourceNorthingExtended = "0";
	//震源高程,4,0.000000
	pSegdSH->m_pSourceElevationExtended = "0.000000";
	//所用的滑动扫描模式,4,00
	pSegdSH->m_pSlipSweepModeUsedExtended = "00";
	//每个磁带的文件数,4,2000
	pSegdSH->m_pFilesPerTapeExtended = "2000";
	//文件计数,4,01
	pSegdSH->m_pFileCountExtended = "01";
	//@@@@@未赋值
	char m_pAcquisitionErrorDescriptionExtended[SIZE_BUFFER];//采集错误说明,160,

	//滤波器类型,4,02
	pSegdSH->m_pFilterTypeExtended = "02";
	//叠加已转储,4,00
	pSegdSH->m_pStackDumpedExtended = "00";
	//叠加符号（当前）,4,01
	pSegdSH->m_pStackSignExtended = "01";
	//所用的 PRM 倾斜度校正,4,00
	pSegdSH->m_pPRMTiltCorrectionUsedExtended = "00";
	//线束名,64,D5
	pSegdSH->m_pSwathNameExtended = "D5";
	//工作模式,4,01
	pSegdSH->m_pOperatingModeExtended = "01";
	//保留,4,79
	pSegdSH->m_pReserved1Extended = "79";
	//无记录,4,00
	pSegdSH->m_pNoLogExtended = "00";
	//监听时间,4,4000
	pSegdSH->m_pListeningTimeExtended = "4000";
	//转储类型,4,00
	pSegdSH->m_pTypeDumpExtended = "00";
	//保留,4,00
	pSegdSH->m_pReserved2Extended = "00";
	//线束 Id,4,01
	pSegdSH->m_pSwathIdExtended = "01";
	//,4,00
	pSegdSH->m_pSeismicTraceOffsetRemovalDisabledExtended = "00";
	//,8,0003a4ac0c90344f
	pSegdSH->m_pGPSTimeAcquisitionTimeBreakExtended = "0003a4ac0c90344f";
	//,8,0000000000000000
	pSegdSH->m_pAlignedGPSTimeAcquisitionExtended = "0000000000000000";
	//未使用,132,000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
	pSegdSH->m_pNotUsed3Extended = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
	
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
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pFileNumberGeneral1.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pFormatCodeGeneral1.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 6, pSegdFileStruct->m_oSegdSH.m_pGeneralConstantsGeneral1.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pLastTwoDigitsOfYearGeneral1.c_str());

	ReadInitToWriteSegdNew_BCD_addHigh(pSegdFileStruct->m_oSegdSH.m_pNumberAdditionalBlocksGeneral1.c_str(), &strHigh);
	ReadInitToWriteSegdNew_BCD_addLow(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pJulianDayGeneral1.c_str(), strHigh);

	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pHourOfDayGeneral1.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pMinuteOfHourGeneral1.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pSecondOfMinuteGeneral1.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pManufacturerCodeGeneral1.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pManufacturerSerialNumberGeneral1.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 3, pSegdFileStruct->m_oSegdSH.m_pBytesPerScanGeneral1.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pBaseScanIntervalGeneral1.c_str());

	ReadInitToWriteSegdNew_BCD_addHigh(pSegdFileStruct->m_oSegdSH.m_pPolarityGeneral1.c_str(), &strHigh);
	ReadInitToWriteSegdNew_BCD_addLow(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pNotUsedGeneral1.c_str(), strHigh);

	ReadInitToWriteSegdNew_BCD_addHigh(pSegdFileStruct->m_oSegdSH.m_pRecordTypeGeneral1.c_str(), &strHigh);
	ReadInitToWriteSegdNew_BCD_addLow(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pRecordLengthGeneral1.c_str(), strHigh);

	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pScanTypePerRecordGeneral1.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pNumberChannelSetsPerRecordGeneral1.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pNumberSampleSkew32ByteExtensionsGeneral1.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pExtendedHeaderLengthGeneral1.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pExternalHeaderLengthGeneral1.c_str());

}

void ProcessGeneralHeaderBlock2BackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct)
{
	ReadInitToWriteSegdNew_BIN(pFile, 3, pSegdFileStruct->m_oSegdSH.m_pExpandedFileNumberGeneral2.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pExpandedChannelSetsGeneral2.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pExtendedHeaderBlocksGeneral2.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pExternalHeaderBlocksGeneral2.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pNotUsed1General2.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pSegdRevisionNumberGeneral2.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pNumberBlocksGeneralTrailerGeneral2.c_str());

	if (pSegdFileStruct->m_oSegdSH.m_uiSampleTime == 0)
	{
		ReadInitToWriteSegdNew_BIN(pFile, 3, pSegdFileStruct->m_oSegdSH.m_pExpandedRecordLengthGeneral2.c_str());
	}
	else
	{
		ReadParameterToSegd_BIN(pFile, 3, pSegdFileStruct->m_oSegdSH.m_uiSampleTime);
	}

	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pNotUsed2General2.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pGeneralHeaderBlockNumberGeneral2.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 13, pSegdFileStruct->m_oSegdSH.m_pNotUsed3General2.c_str());
}

void ProcessGeneralHeaderBlock3BackNew(FILE* pFile,m_oSegdFileStruct* pSegdFileStruct)
{
	ReadInitToWriteSegdNew_BCD(pFile, 3, pSegdFileStruct->m_oSegdSH.m_pExpandedFileNumberGeneral3.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 5, pSegdFileStruct->m_oSegdSH.m_pSourceLineNumberGeneral3.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 5, pSegdFileStruct->m_oSegdSH.m_pSourcePointNumberGeneral3.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pSourcePointIndexGeneral3.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pPhaseControlGeneral3.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pVibratorTypeGeneral3.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pPhaseAngleGeneral3.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pGeneralHeaderBlockNumberGeneral3.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pSourceSetNumberGeneral3.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 12, pSegdFileStruct->m_oSegdSH.m_pNotUsedGeneral3.c_str());
}

void ProcessScanTypeHeaderBackNew(FILE* pFile, int iChannel, m_oSegdFileStruct* pSegdFileStruct)
{

	string strHigh = "";
	int iNumTrace = 0;
	CString strNumTrace = _T("");

	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pScanTypeHeaderScan[iChannel-1].c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pChannelSetNumberScan[iChannel-1].c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pChannelSetStartingTimeScan[iChannel-1].c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pChannelSetEndTimeScan[iChannel-1].c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pDescaleMultiplierScan[iChannel-1].c_str());
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
		ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pNumberChannelsOfChannelSetScan[iChannel-1].c_str());
	}
	else
	{
		ReadParameterToSegd_BCD(pFile, 2, iNumTrace);
	}

	ReadInitToWriteSegdNew_BCD_addHigh(pSegdFileStruct->m_oSegdSH.m_pChannelTypeIdentificationScan[iChannel-1].c_str(), &strHigh);
	ReadInitToWriteSegdNew_BCD_addLow(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pNotUsedScan[iChannel-1].c_str(), strHigh);
	ReadInitToWriteSegdNew_BCD_addHigh(pSegdFileStruct->m_oSegdSH.m_pNumberSubscansExponentScan[iChannel-1].c_str(), &strHigh);
	ReadInitToWriteSegdNew_BCD_addLow(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pChannelGainControlMethodScan[iChannel-1].c_str(), strHigh);

	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pAliasFilterFrequency3dBPointScan[iChannel-1].c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pAliasFilterSlopeScan[iChannel-1].c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pLowCutFilterFrequencyScan[iChannel-1].c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pLowCutFilterSlopeScan[iChannel-1].c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pFirstNotchFrequencyScan[iChannel-1].c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pSecondNotchFrequencyScan[iChannel-1].c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pThirdNotchFrequencyScan[iChannel-1].c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 2, pSegdFileStruct->m_oSegdSH.m_pExtendedChannelSetNumberScan[iChannel-1].c_str());

	ReadInitToWriteSegdNew_BCD_addHigh(pSegdFileStruct->m_oSegdSH.m_pExtendedHeaderFlagScan[iChannel-1].c_str(), &strHigh);
	ReadInitToWriteSegdNew_BCD_addLow(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pTraceHeaderExtensionsScan[iChannel-1].c_str(), strHigh);

	ReadInitToWriteSegdNew_BIN(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pVerticalStackScan[iChannel-1].c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pStreamerCableNumberScan[iChannel-1].c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pArrayFormingScan[iChannel-1].c_str());
}

void ProcessExtendedHeaderBackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct)
{

	if (pSegdFileStruct->m_oSegdSH.m_uiSampleTime == 0)
	{
		pSegdFileStruct->m_oSegdSH.m_uiSampleTime = ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pAcquisitionLengthExtended.c_str());
	}
	else
	{
		ReadParameterToSegd_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_uiSampleTime);
	}

	if (pSegdFileStruct->m_oSegdSH.m_uiSampleRate == 0)
	{
		pSegdFileStruct->m_oSegdSH.m_uiSampleRate = ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pSampleRateExtended.c_str());
	}
	else
	{
		ReadParameterToSegd_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_uiSampleRate);
	}


	if (pSegdFileStruct->m_oSegdSH.m_iTotalTraceNum == 0)
	{
		pSegdFileStruct->m_oSegdSH.m_iTotalTraceNum = ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pTotalNumberTracesExtended.c_str());
	}
	else
	{
		ReadParameterToSegd_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_iTotalTraceNum);
	}

	if (pSegdFileStruct->m_oSegdSH.m_iAuxTraceNum == 0)
	{
		pSegdFileStruct->m_oSegdSH.m_iAuxTraceNum = ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pNumberAuxesExtended.c_str());
	}
	else
	{
		ReadParameterToSegd_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_iAuxTraceNum);
	}

	if (pSegdFileStruct->m_oSegdSH.m_iAcqTraceNum == 0)
	{
		pSegdFileStruct->m_oSegdSH.m_iAcqTraceNum = ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pNumberSeisTracesExtended.c_str());
	} 
	else
	{
		ReadParameterToSegd_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_iAcqTraceNum);
	}

	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pNumberDeadSeisTracesExtended.c_str());


	if (pSegdFileStruct->m_oSegdSH.m_iAcqTraceNum == 0)
	{
		ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pNumberLiveSeisTracesExtended.c_str());
	}
	else
	{
		ReadParameterToSegd_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_iAcqTraceNum);
	}

	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pTypeSourceExtended.c_str());


	if (pSegdFileStruct->m_oSegdSH.m_iSampleLength == 0)
	{
		pSegdFileStruct->m_oSegdSH.m_iSampleLength = ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pNumberSamplesInTraceExtended.c_str());
	}
	else
	{
		ReadParameterToSegd_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_iSampleLength);
	}


	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pShotNumberExtended.c_str());
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pTBWindowExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pTestRecordTypeExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pSpreadFirstLineExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pSpreadFirstNumberExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pSpreadNumberExtended.c_str());

	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pSpreadTypeExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pTimebreakExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pUpholeTimeExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pBlasterIdExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pBlasterStatusExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pRefractionDelayExtended.c_str());

	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pTBtoT0TimeExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pInternalTimeBreakExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pPrestackWithinFieldUnitsExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pNoiseEliminationTypeExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pLowTracePercentageExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pLowTraceValueExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pNumberWindowsOrWindowLengthExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pHistoricEditingTypeOrOverlapExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pNoisyTracePercentageExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pHistoricRangeExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pHistoricTaperLengthExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pThresholdHoldOrNotExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pHistoricThresholdInitValueExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pHistoricZeroingLengthExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pTypeProcessExtended.c_str());    //36*4=144
	ReadInitToWriteSegdNew_BIN(pFile, 128, pSegdFileStruct->m_oSegdSH.m_pAcquisitionTypeTablesExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 128, pSegdFileStruct->m_oSegdSH.m_pThresholdTypeTablesExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pStackingFoldExtended.c_str());
	ReadInitToWriteSegdNew_ASC(pFile, 80, pSegdFileStruct->m_oSegdSH.m_pNotUsed1Extended.c_str());

	if (pSegdFileStruct->m_oSegdSH.m_uiSampleTime == 0)
	{
		ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pRecordLengthExtended.c_str());
	}
	else
	{
		ReadParameterToSegd_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_uiSampleTime);
	}

	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pAutocorrelationPeakTimeExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pNotUsed2Extended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pCorrelationPilotNoExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pPilotLengthExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pSweepLengthExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pAcquisitionNumberExtended.c_str());
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pMaxOfMaxAuxExtended.c_str());
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pMaxOfMaxSeisExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pDumpStackingFoldExtended.c_str());
	ReadInitToWriteSegdNew_ASC(pFile, 16, pSegdFileStruct->m_oSegdSH.m_pTapeLabelExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pTapeNumberExtended.c_str());
	ReadInitToWriteSegdNew_ASC(pFile, 16, pSegdFileStruct->m_oSegdSH.m_pSoftwareVersionExtended.c_str());
	ReadInitToWriteSegdNew_ASC(pFile, 12, pSegdFileStruct->m_oSegdSH.m_pDateExtended.c_str());
	ReadInitToWriteSegdNew_DOUBLE(pFile, 8, pSegdFileStruct->m_oSegdSH.m_pSourceEastingExtended.c_str());
	ReadInitToWriteSegdNew_DOUBLE(pFile, 8, pSegdFileStruct->m_oSegdSH.m_pSourceNorthingExtended.c_str());
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pSourceElevationExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pSlipSweepModeUsedExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pFilesPerTapeExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pFileCountExtended.c_str());
	ReadInitToWriteSegdNew_ASC(pFile, 160, pSegdFileStruct->m_oSegdSH.m_pAcquisitionErrorDescriptionExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pFilterTypeExtended.c_str());

	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pStackDumpedExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pStackSignExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pPRMTiltCorrectionUsedExtended.c_str());
	ReadInitToWriteSegdNew_ASC(pFile, 64, pSegdFileStruct->m_oSegdSH.m_pSwathNameExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pOperatingModeExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pReserved1Extended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pNoLogExtended.c_str());

	if (pSegdFileStruct->m_oSegdSH.m_uiSampleTime == 0)
	{
		ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pListeningTimeExtended.c_str());
	}
	else
	{
		ReadParameterToSegd_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_uiSampleTime);
	}

	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pTypeDumpExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pReserved2Extended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pSwathIdExtended.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, pSegdFileStruct->m_oSegdSH.m_pSeismicTraceOffsetRemovalDisabledExtended.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 8, pSegdFileStruct->m_oSegdSH.m_pGPSTimeAcquisitionTimeBreakExtended.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 8, pSegdFileStruct->m_oSegdSH.m_pAlignedGPSTimeAcquisitionExtended.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 132, pSegdFileStruct->m_oSegdSH.m_pNotUsed3Extended.c_str());
}

void ProcessExternalHeaderBackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct)
{
	int iLenth = 0;
	int iLenthExternalHeader = ReadInitToWriteSegdNew_BCD(pFile, 1, pSegdFileStruct->m_oSegdSH.m_pExternalHeaderLengthGeneral1.c_str());
	iLenth = iLenthExternalHeader * 32;
	ReadInitToWriteSegdNew_ASC(pFile, iLenth, pSegdFileStruct->m_oSegdSH.m_pConcatenationOfExternal);
}

void ProcessTraceHeaderBackNew(FILE* pFile, int iChannel, m_oSegdFileStruct* pSegdFileStruct)
{

	ReadInitToWriteSegdNew_BCD(pFile, 2, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pFileNumberTrace.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pScanTypeNumberTrace.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pChannelSetNumberTrace.c_str());

	if (iChannel == 0)
	{
		ReadInitToWriteSegdNew_BCD(pFile, 2, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pTraceNumberTrace.c_str());
	}
	else
	{
		ReadParameterToSegd_BCD(pFile, 2, iChannel);
	}

	ReadInitToWriteSegdNew_BIN(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pFirstTimingWordTrace.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pTraceHeaderExtensionTrace.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pSampleSkewTrace.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pTraceEditTrace.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pTimeBreakWindowTrace.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 2, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pExtendedChannelSetNumberTrace.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pExtendedFileNumberTrace.c_str());
}

void ProcessTraceHeaderExtensionBlock1BackNew(FILE* pFile, int iChannel, m_oSegdFileStruct* pSegdFileStruct)//ichannel 为点号，1-98
{

	ReadParameterToSegd_BIN(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_iLineIndex);

	if (iChannel == 0)
	{
		ReadInitToWriteSegdNew_BIN(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pReceiverPointNumberTrace1.c_str());
	}
	else
	{
		ReadParameterToSegd_BIN(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_iPointIndex);
	}

	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pReceiverPointIndexTrace1.c_str());

	if (pSegdFileStruct->m_oSegdSH.m_iSampleLength == 0)
	{
		ReadInitToWriteSegdNew_BIN(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pNumberSamplesPerTraceTrace1.c_str());
	} 
	else
	{
		ReadParameterToSegd_BIN(pFile, 3, pSegdFileStruct->m_oSegdSH.m_iSampleLength);
	}

	ReadInitToWriteSegdNew_BCD(pFile, 5, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pExtendedReceiverLineNumberTrace1.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 5, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pExtendedReceiverPointNumberTrace1.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pSensorSEGDCodeTrace1.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 11, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pNotUsedTrace1.c_str());
}

void ProcessTraceHeaderExtensionBlock2BackNew(FILE* pFile, int iChannel, m_oSegdFileStruct* pSegdFileStruct)
{

	ReadInitToWriteSegdNew_DOUBLE(pFile, 8, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pReceiverPointEastingTrace2.c_str());
	ReadInitToWriteSegdNew_DOUBLE(pFile, 8, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pReceiverPointNorthingTrace2.c_str());
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pReceiverPointElevationTrace2.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pSensorTypeNumberTrace2.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pNotUsedTrace2.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pDSDIdentificationNoTrace2.c_str());

	if (iChannel == 0)
	{
		ReadInitToWriteSegdNew_BIN(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pExtendedTraceNoTrace2.c_str());
	}
	else
	{
		ReadParameterToSegd_BIN(pFile, 4, iChannel);
	}

}

void ProcessTraceHeaderExtensionBlock3BackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct)
{

	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pResistanceLowLimitTrace3.c_str());
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pResistanceHighLimitTrace3.c_str());
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pResistanceResistanceValueTrace3.c_str());
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pTiltLimitTrace3.c_str());
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pTiltValueTrace3.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pResistanceErrorTrace3.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pTiltErrorTrace3.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 10, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pNotUsedTrace3.c_str());
}

void ProcessTraceHeaderExtensionBlock4BackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct)
{

	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pCapacitanceLowLimitTrace4.c_str());
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pCapacitanceHighLimitTrace4.c_str());
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pCapacitanceValueTrace4.c_str());
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pCutOffLowLimitTrace4.c_str());
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pCutOffHighLimitsTrace4.c_str());
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pCutOffValueTrace4.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pCapacitanceErrorTrace4.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pCutOffErrorTrace4.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 6, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pNotUsedTrace4.c_str());
}

void ProcessTraceHeaderExtensionBlock5BackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct)
{

	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pLeakageLimitTrace5.c_str());
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pLeakageValueTrace5.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 16, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pNotUsed1Trace5.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pLeakageErrorTrace5.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 7, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pNotUsed2Trace5.c_str());

}

void ProcessTraceHeaderExtensionBlock6BackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct)
{

	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pUnitTypeTrace6.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pUnitSerialNumberTrace6.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pChannelNumberTrace6.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pSpare1Trace6.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pAssemblyTypeTrace6.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pFDUorDSUAssemblySerialNumberTrace6.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pLocationInFDUorDSUAssemblyTrace6.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pSpare2Trace6.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pSubunitTypeTrace6.c_str());

	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pChannelTypeTrace6.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 2, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pSpare3Trace6.c_str());
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pSensorSensitivityTrace6.c_str());
	ReadInitToWriteSegdNew_BCD(pFile, 8, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pNotUsedTrace6.c_str());

}

void ProcessTraceHeaderExtensionBlock7BackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct)
{

	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pControlUnitTypeTrace7.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 3, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pControlUnitSerialNumberTrace7.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pChannelGainScaleTrace7.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pChannelFilterTrace7.c_str());

	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pChannelDataErrorTrace7.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pChannelEditedStatusTrace7.c_str());
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pChannelSampleTomVConversionFactorTrace7.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pNumberOfStacksNoisyTrace7.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pNumberOfStacksLowTrace7.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pChannelTypeIdTrace7.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 1, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pChannelProcessTrace7.c_str());
	ReadInitToWriteSegdNew_FLOAT(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pTraceMaxValueTrace7.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pTraceMaxTimeTrace7.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pNumberInterpolationsTrace7.c_str());
	ReadInitToWriteSegdNew_BIN(pFile, 4, (*pSegdFileStruct->m_pSegdDHList->begin())->m_oSegdTH.m_pSeismicTraceOffsetValueTrace7.c_str());

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

int ReadInitToWriteSegdNew_BCD(FILE* pFile, int iCountByte, const char* pParameter)
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

int ReadInitToWriteSegdNew_BIN(FILE* pFile, int iCountByte, const char* pParameter)
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

void ReadInitToWriteSegdNew_BCD_addLow(FILE* pFile, int iCountByte, const char* pParameter, string strHigh)
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

void ReadInitToWriteSegdNew_BCD_addHigh(const char* pParameter, string* pstr)
{
	(*pstr).assign(pParameter, strlen(pParameter));
}

void ReadInitToWriteSegdNew_FLOAT(FILE* pFile, int iCountByte, const char* pParameter)
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

void ReadInitToWriteSegdNew_DOUBLE(FILE* pFile, int iCountByte, const char* pParameter)
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

void ReadInitToWriteSegdNew_ASC(FILE* pFile, int iCountByte, const char* pParameter)
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