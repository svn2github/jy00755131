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