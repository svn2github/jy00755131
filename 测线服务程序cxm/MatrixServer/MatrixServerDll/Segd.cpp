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
	memcpy(pSegdSH->m_pRecordLengthGeneral1, "01", 2);
	
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