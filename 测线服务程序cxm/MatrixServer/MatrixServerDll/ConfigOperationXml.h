#ifndef _ConfigOptXml_H
#define _ConfigOptXml_H
#include <list>
#include <map>
using std::list;
using std::map;
using std::string;
/**
* @struct Delay_Struct
* @brief Delay SETUP结构体
*/
typedef struct Delay_Struct
{
	/** VP时间滑动值*/
	int m_iVPSlipTime;
	/** VP时间延时最大值*/
	int m_iVPTimeMax;
	/** VP时间延时最小值*/
	int m_iVPTimeMin;
	/** Acq时间滑动值*/
	int m_iAcqSlipTime;
	/** Acq时间延时最大值*/
	int m_iAcqTimeMax;
	/** Acq时间延时最小值*/
	int m_iAcqTimeMin;
}m_oDelayStruct;

/**
* @struct SourceShot_Struct
* @brief 炮表结构体
*/
typedef struct SourceShot_Struct
{
	/** 炮点状态*/
	unsigned int m_uiVPStatus;
	/** 炮号*/
	unsigned int m_uiShotID;
	/** 中断标志位*/
	unsigned int m_uiBreakPoint;
	/** 震源点测线号*/
	float m_fSourceLine;
	/** 震源点接收道位置号*/
	float m_fSourceReceiver;
	/** 排列中的最低测线号*/
	unsigned int m_uiSFL;
	/** 排列第一个接收检波器位置号码*/
	unsigned int m_uiSFN;
	/** 排列类型（绝对排列和相对排列）*/
	unsigned int m_uiSpreadType;
	/** 超级排列（不使用则显示为空,值为-1）*/
	int m_iSuperSpread;
	/** 数据处理类型*/
	unsigned int m_uiProcessType;
	/** 注释信息长度*/
	unsigned short m_usCommentsSize;
	/** 注释*/
	char* m_pcComments;
}m_oSourceShotStruct;

/**
* @struct SourceExplo_Struct
* @brief Explo震源结构体
*/
typedef struct SourceExplo_Struct
{
	/** 震源状态*/
	unsigned int m_uiSourceStatus;
	/** 震源序号*/
	unsigned int m_uiSourceNb;
	/** 长度*/
	unsigned short m_usLabelSize;
	/** 标签*/
	char* m_pcLabel;
	/** 震源控制器的身份号*/
	unsigned int m_uiShooterSN;
	/** 完成一次放炮后要使用的递增步幅*/
	int m_iIncrNb;
	/** 注释信息长度*/
	unsigned short m_usCommentsSize;
	/** 注释*/
	char* m_pcComments;
}m_oSourceExploStruct;

/**
* @struct SourceVibro_Struct
* @brief Vibro震源结构体
*/
typedef struct SourceVibro_Struct
{
	/** 震源状态*/
	unsigned int m_uiSourceStatus;
	/** 震源序号*/
	unsigned int m_uiSourceNb;
	/** 长度*/
	unsigned short m_usLabelSize;
	/** 标签*/
	char* m_pcLabel;
	/** 可控震源组的身份号*/
	unsigned int m_uiFleetNb;
	/** moving下拉框选择*/
	unsigned int m_uiMoving;
	/** 递增步幅*/
	int m_iStep;
	/** Work by Acq*/
	unsigned int m_uiWorkByAcq;
	/** Cluster 编号确定要同时使用的两个震源组*/
	unsigned int m_uiCluster;
	/** 注释信息长度*/
	unsigned short m_usCommentsSize;
	/** 注释*/
	char* m_pcComments;
}m_oSourceVibroStruct;

/**
* @struct ProcessRecord_Struct
* @brief ProcessRecord结构体
*/
typedef struct ProcessRecord_Struct
{
	/** Plug选项*/
	unsigned int m_uiPlug;
	/** BoxType选项*/
	unsigned int m_uiBoxType;
	/** SN串号*/
	unsigned int m_uiSN;
	/** 在脉冲模式中记录长度*/
	unsigned int m_uiRecordLength;
	/** 折射延迟*/
	unsigned int m_uiRefractionDelay;
	/** TB窗口时间*/
	unsigned int m_uiTBWindow;
	/** 在可控震源模式中记录长度*/
	unsigned int m_uiListeningTime;
	/** PeakTime*/
	unsigned int m_uiPeakTime;
	/** Raw*/
	unsigned int m_uiRaw;
	/** PreStack*/
	unsigned int m_uiPreStack;
}m_oProcessRecordStruct;

/**
* @struct ProcessAux_Struct
* @brief ProcessAux结构体
*/
typedef struct ProcessAux_Struct
{
	/** 辅助道序号*/
	unsigned int m_uiAuxNb;
	/** 处理方法信息长度*/
	unsigned short m_usAuxProcessingSize;
	/** 处理方法*/
	char* m_pcAuxProcessing;
}m_oProcessAuxStruct;

/**
* @struct ProcessAcq_Struct
* @brief ProcessAcq结构体
*/
typedef struct ProcessAcq_Struct
{
	/** ACQ状态*/
	unsigned int m_uiAcqStatus;
	/** 每次独立采集的有序数*/
	unsigned int m_uiAcqNb;
	/** 采集的类型*/
	unsigned int m_uiAcqType;
	/** 叠加方式*/
	unsigned int m_uiSignStack;
	/** 输出模式选择*/
	unsigned int m_uiOutPut;
}m_oProcessAcqStruct;

/**
* @struct ProcessType_Struct
* @brief ProcessType结构体
*/
typedef struct ProcessType_Struct
{
	/** 序号*/
	unsigned int m_uiProNb;
	/** 标签信息长度*/
	unsigned short m_usLabelSize;
	/** 标签*/
	char* m_pcLabel;
}m_oProcessTypeStruct;

/**
* @struct OperationComment_Struct
* @brief OperationComment结构体
*/
typedef struct OperationComment_Struct
{
	/** 序号*/
	unsigned int m_uiNb;
	/** 标签信息长度*/
	unsigned short m_usLabelSize;
	/** 标签*/
	char* m_pcLabel;
	/** 注释信息长度*/
	unsigned short m_usCommentsSize;
	/** 注释*/
	char* m_pcComments;
}m_oOperationCommentStruct;

/**
* @struct OptSetupData_Struct
* @brief 施工客户端信息结构体
*/
typedef struct OptSetupData_Struct
{
	/**	Delay*/
	m_oDelayStruct m_oDelay;
	/** 炮点队列*/
	list<m_oSourceShotStruct> m_olsSourceShot;
	/** Explo震源结构体*/
	list<m_oSourceExploStruct> m_olsExploStruct;
	/** Vibro震源结构体*/
	list<m_oSourceVibroStruct> m_olsVibroStruct;
	/** 处理类型*/
	m_oProcessRecordStruct m_oProcessRecord;
	/** AppendAuxesFromDsd*/
	unsigned int m_uiAppendAux;
	/** Correl with*/
	int m_iCorrelWith;
	/** 迂回道队列*/
	list<m_oProcessAuxStruct> m_olsProcessAuxStruct;
	/** Acq队列*/
	list<m_oProcessAcqStruct> m_olsProcessAcqStruct;
	/** 处理类型队列*/
	list<m_oProcessTypeStruct> m_olsProcessTypeStruct;
	/** 注释队列*/
	list<m_oOperationCommentStruct> m_olsComment;
	/** 资源同步对象*/
	CRITICAL_SECTION m_oSecCommInfo;
	/** XMLDOM文件对象*/
	CXMLDOMDocument m_oXMLDOMDocument;
	/** 施工XML配置文件路径*/
	string m_strOptXMLFilePath;
}m_oOptSetupDataStruct;
#endif