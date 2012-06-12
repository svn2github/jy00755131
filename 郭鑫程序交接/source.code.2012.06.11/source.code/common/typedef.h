#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

/*
当前用况为： 只有主交叉线上存在交叉站，即只有一条交叉线存在

当前使用的ip分配方式为：
	1.主交叉站为192.0.0.0 这是默认分配
	2.交叉站来说，默认起始分配为 192.B.XXX.XXX 将交叉站信息存放在B位置存放信息如下：
		a. 所在大线信息 第6位存放
		b. 所在交叉线信息 高2位存放 
				交叉线A（主交叉线上方）00
				交叉线B（主交叉线下方）11		
	3.采集站和电源站 默认起始分配为 192.xxx.c.d 将使用c, d 来访方式信息
		c高两位存放大线A（交叉站左） 00
		c高两位存放大线B（交叉站右） 11

路由分配方式为
	1. 主交叉站	默认方式为 ROUTE_BIG_LINE_BASE_NUMBER + ROUTE_XXXX
	2. 交叉站		默认使用全局变量来记录当前线号 * ROUTE_BIG_LINE_BASE_NUMBER + ROUTE_XXXX
*/

#include <hash_map>
#include <list>

#define LCI_IP_ADDRESS	__TEXT("192.0.0.0")

#define	WM_THE_END_PACKAGE_RECV_MESSAGE	WM_USER + 100
#define WM_HEARTBEAT_PACKAGE_MESSAGE  WM_USER + 110
#define	WM_THE_END_PACKAGE_TIME_DELAY_RECV_MESSAGE	WM_USER + 120


enum __enu_CONST_TYPE_TOKEN
{
	ROUTE_UP = 0x0,						//上方路由
	ROUTE_DOWN = 0x3,					//下方路由
	ROUTE_RIGHT = 0x2,					//右边路由
	ROUTE_LEFT = 0x1,					//左边路由
	MAX_EQUIPMENT_SIZE = 20000,			//最大支持设备格式
	MAX_ROUTE_SIZE = 4,					//路由方向
	MAX_OUTPUT_PACKAGE_SIZE = 128,		//下行数据包大小
	MAX_IP_SETTING_ARRAY_SIZE = 1024,	//IP设置响应队列大小
	MAX_BIG_LINE_SIZE = 20,				//大线数量
	MAX_INPUT_PACKAGE_SIZE = 256,		//上行数据包大小

	HEARTBEAT_EVENT_ID	= 0x8000,		//心跳定时器标记
	HEARTBEAT_TIMEOUT_VALUE = 200,	//心跳定时器超时时间

	SN_MAIN_LAUX = 0x04001001,		//主交叉站
	LAUX = 0x01,				//交叉站
	LAUL = 0x02,				//电源站
	FDU = 0x03,					//采集站
	IP_SOURCE = 0xC0000000,     //主机IP
	IP_BROADCAST = 0xFFFFFFFF,  //广播IP
	PORT_IP_SET = 0x9004,      //IP设置应答监听端口
	CMD_SET = 0x0001,          //设置命令
	CMD_SEARCH = 0x0002,       //查询命令
	CMD_SN = 0x01,
	CMD_IP = 0x03,

	IP_MAIN_LAUX = 0x000000C0,      //主交叉站IP

	DESTINATION_PORT = 0x9003,  //发送目标端口

	CMD_FST_TIME = 0x02,       //首包时刻命令
	CMD_ROUTE_SET = 0x1f,      //路由设置命令
	CMD_ROUTE_REC = 0x3f,      //路由接收命令
	CMD_END = 0x00,            //包结束命令
	CMD_SYSTEM_TIME = 0x04,    //系统时刻命令

	IP_SETTING_RESP_EVENT_ID	= 0x9000,	//ip设置定时器标记
	IP_SETTING_RESP_TIMEOUT_VALUE = 1000,	//ip设置定时器超时时间

	DELETION_TOKEN_INDEX = 0x4,	//0：不能删除， 1：删除并添加ip索引表 ，-1：表示失效
	WAIT_COUNT_TOKEN_INDEX = 0x5,
	PROC_COUNT_TOKEN_INDEX = 0x6,

	CHECK_THE_FIRST_PACKAGE_EVENT_ID	= 0xa000,	//检查首包退化定时器标记
	CHECK_THE_FIRST_PACKAGE_TIMEOUT_VALUE = 1000,	//检查首包退化定时器超时时间

	ADD_IP_SETTING_LIST_CONDITION	= 0x6,
	ROUTE_DIRECTION_VALUE_MASK = 0x3,

	ROUTE_BIG_LINE_BASE_NUMBER = 1000,
	THE_FIRST_PACKAGE_INC_COUNTER = 0x2,
	BASE_BOARDCAST_PORT = 0x100,
	BIG_LINE_A_BOARDCAST_PORT = 0x10,
	BIG_LINE_B_BOARDCAST_PORT = 0x20,

	//用于设备数据结构中route_的下标索引值
	CROSS_LINE_A_INDEX = 0x2, 
	CROSS_LINE_B_INDEX = 0x3,
	BIG_LINE_A_INDEX = 0X0,
	BIG_LINE_B_INDEX = 0X1,
	//end
	MAX_BOARDCAST_PORT_SIZE = 0x3,
	CURRENT_EQUIPMENT_BOARDCAST_INDEX = 0x0,
	BIG_LINE_A_BOARDCAST_PORT_INDEX = 0x1,
	BIG_LINE_B_BOARDCAST_PORT_INDEX = 0x2,

	CMD_BOARDCAST_PORT_W = 0x0a, //广播等待端口设置
	CMD_BOARDCAST_PORT_R = 0x17, //广播等待端口匹配

	THE_END_PACKAGE_INC_COUNTER = 0x2,
	THE_END_PACKAGE_DES_COUNTER = 0x1,

	CHECK_THE_END_PACKAGE_EVENT_ID	= 0xb000,	//检查首包退化定时器标记
	CHECK_THE_END_PACKAGE_TIMEOUT_VALUE = 3000,	//检查首包退化定时器超时时间

	PORT_REQUIRE_DELAY = 0x9001,  //尾包时刻查询监听端口
	CMD_REQUIRE_DELAY_LAUX = 0x1b,//交叉站尾包时刻查询命令
	CMD_REQUIRE_DELAY_FDU = 0x16, //采集站尾包时刻查询命令
	CMD_NET_TIME = 0x19,          //网络时刻
	PORT_SET_DELAY = 0x9002,      //设置时延监听端口
	CMD_MODIFY_TIME_HIGH = 0x05, //本地时间修正高位
	CMD_MODIFY_TIME_LOW = 0x06,   //本地时间修正低位

	THE_FIRST_PACKAGE_OPTR_COMPELETED_COUNTER = 0x6,
	TIME_DELAY_SETTING_CONDITION = 0x6,

	TIME_DELAY_SETTING_RESP_EVENT_ID	= 0x9040,	//设置定时器标记
	TIME_DELAY_SETTING_RESP_TIMEOUT_VALUE = 500,	//设置定时器超时时间

	MAX_CROSS_LINE_SIZE	=	0x1, //指定当前交叉线数量

	CMD_18 = 0x18,                //18命令字
	PORT_AD_SET = 0x6000,
	CMD_TB_HIGH = 0x0c,          
	CMD_TB_LOW = 0x0d,
	TB_HIGH_DELAY = 0x5000,       //tb高位的时延
	TB_LOW = 250,                 //tb低位
	TB_SWITCH = 0x01,             //tb开关

	CMD_AD_IP_RETURN = 0x07,
	CMD_AD_PORT_RETURN = 0x08,
	CMD_AD_PORT_RANGE = 0x09,

	//	AD_PORT_CMD = 0x0003,    //端口固定
	AD_PORT_CMD = 0x8003,    //端口递增

	AD_PORT_UPPER = 0x5008,
	AD_PORT_LOWER = 0x5001,
	CMD_AD = 0x0003,
	//AD_DATA_RATE = 0x42,		//采样率为250
	//  AD_DATA_RATE = 0x4a,     //采样率为500
	  AD_DATA_RATE = 0x52,     //采样率为1000
	//  AD_DATA_RATE = 0x5a,     //采样率为2000
	//  AD_DATA_RATE = 0x62,     //采样率为4000

	AD_SETTING_PROC_EVENT_ID = 0x9080,
	AD_SETTING_PROC_TIMEOUT_VALUE = 200,

	AD_COLLECTION_RECV_EVENT_ID = 0x90a0,
	AD_COLLECTION_RECV_TIMEOUT_VALUE = 100,

	AD_STOP_COLLECTION_PROC_EVENT_ID = 0x9090,
};

typedef struct __tagEQUIPMENT_ITEM
{
	int type_;
	int route_[MAX_ROUTE_SIZE];				//当前设备路由，主要用于交叉站
	unsigned the_first_package_time_;		//首包时刻
	int the_first_package_count_;		//首包计数器
	unsigned serial_no_;					//序列号
	int the_end_package_count_;		//尾包时刻
	unsigned the_end_package_recv_time_;	//尾包接受时刻
	unsigned the_end_package_send_time_;	//尾包发送时刻
	unsigned ip_;							//设备ip地址
	bool is_valid_ip_;						//设备ip地址是否生效true生效
	bool is_valid_the_end_package_time_delay_resp_; //是否收到有效尾包时延
	__tagEQUIPMENT_ITEM *up_, *down_, *left_, *right_; //物理连接的上下左右设备指针

	unsigned boardcast_port_[MAX_BOARDCAST_PORT_SIZE];	//广播端口
														//0: 表示当前设备的广播端口
														//1: 表示当前设备如果是交叉站左边链接采集站和电源站设备的广播端口
														//2: 表示当前设备如果是交叉站右边链接采集站和电源站设备的广播端口
	unsigned the_end_package_delay_time_high_;	//尾包延时
	unsigned the_end_package_delay_time_low_;	//尾包延时
	bool is_valid_equipment_;//是否为有效设备, true -- 有效, false -- 无效, 该设置主要表明该设备是否完成时延设置,可以作为正常使用于AD中	
//	unsigned int offset_; //AD采集的偏移量
//	bool is_valid_the_end_package_time_delay_;//当前尾包时延是否有效初始化状态为false--无效, 当时延设置成功后,设置为true--有效
}EQUIPMENT_ITEM, *EQUIPMENT_ITEM_PTR;

//首包设置ip地址响应队列数据结构
typedef struct __tagIP_SETTING_INFO
{
	EQUIPMENT_ITEM_PTR equipment_item_;
	unsigned char pack_[MAX_OUTPUT_PACKAGE_SIZE]; //发送数据包内容
	__tagIP_SETTING_INFO* next_;
}IP_SETTING_INFO, *IP_SETTING_INFO_PTR;

//每条交叉线上的存储数据结构,该数据结构用于尾包处理,
//初次初始化在
typedef struct __tagCROSS_LING_ARRAY_ITEM
{
	EQUIPMENT_ITEM_PTR the_first_station_item_up_; //CROSS LINE A 第一个设备位置
	EQUIPMENT_ITEM_PTR the_end_station_item_up_; //CROSS LINE A最后一个设备位置
	EQUIPMENT_ITEM_PTR the_first_station_item_down_;//CROSS LINE B第一个设备位置
	EQUIPMENT_ITEM_PTR the_end_station_item_down_;//CROSS LINE B最后一个设备位置
	bool be_the_upper_line_time_delay_setting_completed_;
	bool be_the_following_line_time_delay_setting_completed_;
}CROSS_LING_ARRAY_ITEM, *CROSS_LING_ARRAY_ITEM_PTR;

//每条大线上的存储数据结构
typedef struct __tagLING_ARRAY_ITEM
{
	EQUIPMENT_ITEM_PTR the_first_station_item_left_; //左边第一个设备位置
	EQUIPMENT_ITEM_PTR the_end_station_item_left_; //左边最后一个设备位置
	EQUIPMENT_ITEM_PTR the_first_station_item_right_;//右边第一个设备位置
	EQUIPMENT_ITEM_PTR the_end_station_item_right_;//右边最后一个设备位置
	bool be_the_left_line_time_delay_setting_completed_;
	bool be_the_right_line_time_delay_setting_completed_;
}LING_ARRAY_ITEM, *LING_ARRAY_ITEM_PTR;

typedef struct __tagCURRENT_LINE_TIME_DELAY_DATA
{
	bool is_completed_; //是否完成当前线路时统
	unsigned char cross_station_boardcast_pack_[MAX_OUTPUT_PACKAGE_SIZE]; //交叉站广播端口设置数据包
	unsigned char time_delay_pack_[MAX_OUTPUT_PACKAGE_SIZE];//发送时延查询数据包
}CURRENT_LINE_TIME_DELAY_DATA, *CURRENT_LINE_TIME_DELAY_DATA_PTR;

typedef struct __tagTHE_HEART_BEAT_PACKAGE_INFO
{
	int ip_;
	int time_;
	int cmd_word_;
	int net_time_;
}THE_HEART_BEAT_PACKAGE_INFO, *THE_HEART_BEAT_PACKAGE_INFO_PTR;

enum __enuTHE_TAIL_PACKAGE_PROC_STATUS 
{
	stop_the_tail_package_proc = 0x00,
	start_the_tail_package_proc = 0x80,
	start_the_tail_package_time_delay_req = 0x100,
	start_the_tail_package_time_delay_setting = 0x200,
};

typedef struct __tagTHE_END_PACKAGE_PROC_RUNNING
{
	enum __enuTHE_TAIL_PACKAGE_PROC_STATUS the_end_package_status_;
	EQUIPMENT_ITEM_PTR equipment_ptr_;
	int route_;
}THE_END_PACKAGE_PROC_RUNNING, THE_END_PACKAGE_PROC_RUNNING_PTR;

typedef stdext::hash_map<unsigned, EQUIPMENT_ITEM_PTR>	stdext_hash_map;
typedef stdext::hash_map<unsigned, EQUIPMENT_ITEM_PTR>::iterator stdext_hash_map_iterator;
typedef std::list<IP_SETTING_INFO_PTR> ip_setting_list;
typedef std::list<IP_SETTING_INFO_PTR>::iterator ip_setting_list_iterator;

#endif //__TYPEDEF_H__