#include <list>
#include <map>
using std::list;
using std::map;
/**
* @struct Delay_Struct
* @brief Delay SETUP结构体
*/
typedef struct Delay_Struct
{
	/** Acq时间延时最小值*/
	int m_iAcqTimeMin;
	/** Acq时间延时最大值*/
	int m_iAcqTimeMax;
	/** Acq时间滑动值*/
	int m_iAcqSlipTime;
	/** VP时间延时最小值*/
	int m_iVPTimeMin;
	/** VP时间延时最大值*/
	int m_iVPTimeMax;
	/** VP时间滑动值*/
	int m_iVPSlipTime;
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
	bool m_bBreakPoint;
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
	/** 超级排列（不适用则显示为空,值为-1）*/
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
	bool m_bWorkByAcq;
	/** Cluster 编号确定要同时使用的两个震源组*/
	unsigned int m_uiCluster;
	/** 注释信息长度*/
	unsigned short m_usCommentsSize;
	/** 注释*/
	char* m_pcComments;
}m_oSourceVibroStruct;
/**
* @struct SourceType_Struct
* @brief 震源类型结构体
*/
typedef struct SourceType_Struct
{

}m_oSourceTypeStruct;
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
* @struct Process_Struct
* @brief Process结构体
*/
typedef struct Process_Struct
{
}m_oProcessStruct;

/**
* @struct OperationComment_Struct
* @brief OperationComment结构体
*/
typedef struct OperationComment_Struct
{
	/** 序号*/
	unsigned int m_uiNb;
	/** 注释信息长度*/
	unsigned short m_usCommentsSize;
	/** 注释*/
	char* m_pcComments;
}m_oOperationCommentStruct;