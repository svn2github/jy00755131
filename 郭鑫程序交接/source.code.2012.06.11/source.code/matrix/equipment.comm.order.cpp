
#include "../common/config.h"
#include "../common/typedef.h"
#include "../common/sockimpl.h"
#include "common.functions.h"
#include "equipment.comm.order.h"
#include "matrix.h"

#pragma warning(disable: 4100)

#define __MATRIX_DEBUG__

extern APPLICATION_ENVIRONMENT __application_environment;

int make_package_head(int ip_destination, int port, int cmd, unsigned char* frame_data)
{
	unsigned i;
	unsigned iPos = 0;

	i = 0x44332211;
	memcpy(frame_data + iPos, &i, 4);
	iPos = iPos + 4;
	i = 0x000000;
	memcpy(frame_data + iPos, &i, 3);
	iPos = iPos + 12;
	i = __application_environment.ip_;
	memcpy(frame_data + iPos, &i, 4);
	iPos = iPos + 4;
	i = ip_destination;
	memcpy(frame_data + iPos, &i, 4);
	iPos = iPos + 4;
	i = port;
	memcpy(frame_data + iPos, &i, 2);
	iPos = iPos + 2;
	i = cmd;
	memcpy(frame_data + iPos, &i, 2);
	iPos = iPos + 6;

	return iPos;
}

//注意要初始化删除标记， 等待计数器和ip设置计数器
void make_ip_setting_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size)
{
	buf_size = buf_size;
	unsigned iPos = 0;
	unsigned i = 0;

	iPos = make_package_head(IP_BROADCAST, PORT_IP_SET, CMD_SET, buf);
	i = CMD_SN;
	memcpy(buf + iPos, &i, 1);
	iPos = iPos + 1;
	i = equipment_ptr->serial_no_;
	memcpy(buf + iPos, &i, 4);
	iPos = iPos + 4;
	i = CMD_IP;
	memcpy(buf + iPos, &i, 1);
	iPos = iPos + 1;
	i = equipment_ptr->ip_;
	memcpy(buf + iPos, &i, 4);
	iPos = iPos + 4;

	if (equipment_ptr->type_ == LAUX || equipment_ptr->type_ == SN_MAIN_LAUX)
	{

		for (int j = 0; j<MAX_ROUTE_SIZE; j++)
		{
			i = CMD_ROUTE_SET;
			memcpy(buf + iPos, &i, 1);
			iPos = iPos + 1;
			i = equipment_ptr->route_[j];
			memcpy(buf + iPos, &i, 4);
			iPos = iPos + 4;
		}
	} 

	i = CMD_BOARDCAST_PORT_W;
	memcpy(buf + iPos, &i, 1);
	iPos += 1;
	i = equipment_ptr->boardcast_port_[CURRENT_EQUIPMENT_BOARDCAST_INDEX];
	memcpy(buf + iPos, &i, 2);
	iPos += 2;
	memcpy(buf + iPos, &i, 2);
	iPos += 2;

	i = 0x00;
	memcpy(buf + iPos, &i, 1);

	buf[DELETION_TOKEN_INDEX] = 0;
	buf[WAIT_COUNT_TOKEN_INDEX] = 3;
	buf[PROC_COUNT_TOKEN_INDEX] = 0;
}

unsigned AD_init_18(unsigned char *buf, unsigned iPos, unsigned char *ad_str, int lenth)
{
	for (int i = 0; i<lenth; i+=4){
		buf[iPos] = CMD_18;
		iPos++;
		memcpy(buf + iPos, ad_str + i, 4);
		iPos = iPos + 4;
	}
	return iPos;
}

//正弦函数输出命令
void make_ad_set_sin_function_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr)
{
	unsigned iPos = 0;
	unsigned char ad_init[4] = {0xb2, 0x00, 0x80, 0x00};
	iPos = make_package_head(equipment_ptr->ip_, PORT_AD_SET, CMD_SET, buf);
	iPos = AD_init_18(buf, iPos, ad_init, 4);
	buf[iPos] = 0x00;
}

//停止连续采集命令
void make_ad_set_SDATAC_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr)
{
	unsigned iPos = 0;
	unsigned char ad_init[4] = {0x81, 0x11, 0x00, 0x00};
	iPos = make_package_head(equipment_ptr->ip_, PORT_AD_SET, CMD_SET, buf);
	iPos = AD_init_18(buf, iPos, ad_init, 4);
	buf[iPos] = 0x00;
}

//打开模拟开关2命令,tb模式
void make_ad_set_switch2_tb_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr)
{
	unsigned iPos = 0;
	unsigned char ad_init[4] = {0xa3, 0x00, 0x30, 0x40};
	iPos = make_package_head(equipment_ptr->ip_, PORT_AD_SET, CMD_SET, buf);
	iPos = AD_init_18(buf, iPos, ad_init, 4);
	buf[iPos] = 0x00;
}

//打开模拟开关2命令，立即模式
void make_ad_set_switch2_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr)
{
	unsigned iPos = 0;
	unsigned char ad_init[4] = {0xa3, 0x00, 0x30, 0x00};
	iPos = make_package_head(equipment_ptr->ip_, PORT_AD_SET, CMD_SET, buf);
	iPos = AD_init_18(buf, iPos, ad_init, 4);
	buf[iPos] = 0x00;
}

//打开模拟开关命令4,tb模式
void make_ad_set_switch4_tb_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr)
{
	unsigned iPos = 0;
	unsigned char ad_init[4] = {0xa3, 0x00, 0xf0, 0x40};
	iPos = make_package_head(equipment_ptr->ip_, PORT_AD_SET, CMD_SET, buf);
	iPos = AD_init_18(buf, iPos, ad_init, 4);
	buf[iPos] = 0x00;
}

//打开模拟开关命令4,立即模式
void make_ad_set_switch4_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr)
{
	unsigned iPos = 0;
	unsigned char ad_init[4] = {0xa3, 0x00, 0xf0, 0x00};
	iPos = make_package_head(equipment_ptr->ip_, PORT_AD_SET, CMD_SET, buf);
	iPos = AD_init_18(buf, iPos, ad_init, 4);
	buf[iPos] = 0x00;
}

//打开模拟开关命令7,tb模式
void make_ad_set_switch7_tb_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr)
{
	unsigned iPos = 0;
	unsigned char ad_init[4] = {0xa3, 0x05, 0xf8, 0x40};
	iPos = make_package_head(equipment_ptr->ip_, PORT_AD_SET, CMD_SET, buf);
	iPos = AD_init_18(buf, iPos, ad_init, 4);
	buf[iPos] = 0x00;
}

//打开模拟开关命令7,立即模式
void make_ad_set_switch7_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr)
{
	unsigned iPos = 0;
	unsigned char ad_init[4] = {0xa3, 0x05, 0xf8, 0x00};
	iPos = make_package_head(equipment_ptr->ip_, PORT_AD_SET, CMD_SET, buf);
	iPos = AD_init_18(buf, iPos, ad_init, 4);
	buf[iPos] = 0x00;
}

//读寄存器全部
void make_ad_set_RREG_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr)
{
	unsigned iPos = 0;
	unsigned char ad_init[4] = {0x82, 0x20, 0x0a, 0x00};
	iPos = make_package_head(equipment_ptr->ip_, PORT_AD_SET, CMD_SET, buf);
	iPos = AD_init_18(buf, iPos, ad_init, 4);
	buf[iPos] = 0x00;
}

//同步
void make_ad_set_SYNC_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr)
{
	unsigned iPos = 0;
	unsigned char ad_init[4] = {0x81, 0x04, 0x00, 0x00};
	iPos = make_package_head(equipment_ptr->ip_, PORT_AD_SET, CMD_SET, buf);
	iPos = AD_init_18(buf, iPos, ad_init, 4);
	buf[iPos] = 0x00;
}

//连续读
void make_ad_set_RDATAC_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr)
{
	unsigned iPos = 0;
	unsigned char ad_init[4] = {0x81, 0x10, 0x00, 0x00};
	iPos = make_package_head(equipment_ptr->ip_, PORT_AD_SET, CMD_SET, buf);
	iPos = AD_init_18(buf, iPos, ad_init, 4);
	buf[iPos] = 0x00;
}

//偏移校正
void make_ad_set_OFSCAL_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr)
{
	unsigned iPos = 0;
	unsigned char ad_init[4] = {0x81, 0x60, 0x00, 0x00};
	iPos = make_package_head(equipment_ptr->ip_, PORT_AD_SET, CMD_SET, buf);
	iPos = AD_init_18(buf, iPos, ad_init, 4);
	buf[iPos] = 0x00;
}

//初始化中写寄存器
void make_ad_set_WREG_init_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr)
{
	unsigned iPos = 0;
	unsigned char ad_init[16] = {0x8d, 0x40, 0x0a, 0x00, AD_DATA_RATE, 0x08, 0x32, 0x03, 0x6f, 0x0c, 0xff, 0x7d, 0x52, 0x40, 0x00, 0x00};
	iPos = make_package_head(equipment_ptr->ip_, PORT_AD_SET, CMD_SET, buf);
	iPos = AD_init_18(buf, iPos, ad_init, 16);
	buf[iPos] = 0x0;
}

//零漂校正中写寄存器
void make_ad_set_WREG_offset_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr)
{
	unsigned iPos = 0;
	unsigned char ad_init[16] = {0x8d, 0x40, 0x0a, 0x00, AD_DATA_RATE, 0x18, 0x32, 0x03, 0x6f, 0x0c, 0xff, 0x7d, 0x52, 0x40, 0x00, 0x00};
	iPos = make_package_head(equipment_ptr->ip_, PORT_AD_SET, CMD_SET, buf);
	iPos = AD_init_18(buf, iPos, ad_init, 16);
	buf[iPos] = 0x00;
}

//连续采集中写寄存器
void make_ad_set_WREG_begin_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr)
{
	unsigned iPos = 0;
	unsigned char ad_init[8] = {0x87, 0x40, 0x04, 0x00, AD_DATA_RATE, 0x08, 0x32, 0x03};
	iPos = make_package_head(equipment_ptr->ip_, PORT_AD_SET, CMD_SET, buf);
	iPos = AD_init_18(buf, iPos, ad_init, 8);
	buf[iPos] = 0x00;

}

//TB清0
void make_ad_set_TB_TO_0_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr)
{
	unsigned iPos = 0;
	unsigned i = 0;
	iPos = make_package_head(equipment_ptr->ip_, PORT_AD_SET, CMD_SET, buf);
	i = CMD_TB_HIGH;
	memcpy(buf + iPos, &i, 1);
	iPos = iPos + 1;
	i = 0x00000000;
	memcpy(buf + iPos, &i, 4);
	iPos = iPos + 4;
	i = CMD_TB_LOW;
	memcpy(buf + iPos, &i, 1);
	iPos = iPos + 1;
	i = 0x00000000;
	memcpy(buf + iPos, &i, 4);
	iPos = iPos + 4;
	i = 0x00;
	memcpy(buf + iPos, &i, 1);

}

//TB设置
void make_ad_set_TB_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr)
{
	unsigned iPos = 0;
	unsigned i = 0;

	THE_HEART_BEAT_PACKAGE_INFO_PTR heart_beat_pack_info = (THE_HEART_BEAT_PACKAGE_INFO_PTR)void_ptr;

	iPos = make_package_head(equipment_ptr->ip_, PORT_AD_SET, CMD_SET, buf);
	i = CMD_TB_HIGH;
	memcpy(buf + iPos, &i, 1);
	iPos = iPos + 1;
	i = heart_beat_pack_info->time_ + TB_HIGH_DELAY;  
	memcpy(buf + iPos, &i, 4);
	iPos = iPos + 4;
	i = CMD_TB_LOW;
	memcpy(buf + iPos, &i, 1);
	iPos = iPos + 1;
	i = TB_LOW;
	memcpy(buf + iPos, &i, 2);
	iPos = iPos + 2;
	i = TB_SWITCH;
	memcpy(buf + iPos, &i, 2);
	iPos = iPos + 2;
	i = 0x00;
	memcpy(buf + iPos, &i, 1);
#ifdef __MATRIX_DEBUG__
	output_log(IDS_STRING177, equipment_ptr->ip_, heart_beat_pack_info->time_ + TB_HIGH_DELAY);
#endif //__MATRIX_DEBUG__
}

//返回参数设置
void make_ad_set_return_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr)
{
	unsigned iPos = 0;
	unsigned i = 0;
	iPos = make_package_head(equipment_ptr->ip_, PORT_AD_SET, CMD_SET, buf);
	i = CMD_AD_IP_RETURN;
	memcpy(buf + iPos, &i, 1);
	iPos = iPos + 1;
	i = __application_environment.ip_;  
	memcpy(buf + iPos, &i, 4);
	iPos = iPos + 4;
	i = CMD_AD_PORT_RETURN;
	memcpy(buf + iPos, &i, 1);
	iPos = iPos + 1;
	i = AD_PORT_LOWER;
	memcpy(buf + iPos, &i, 2);
	iPos = iPos + 2;
	i = AD_PORT_CMD;
	memcpy(buf + iPos, &i, 2);
	iPos = iPos + 2;
	i = CMD_AD_PORT_RANGE;
	memcpy(buf + iPos, &i, 1);
	iPos = iPos + 1;
	i = AD_PORT_LOWER;
	memcpy(buf + iPos, &i, 2);
	iPos = iPos + 2;
	i = AD_PORT_UPPER;
	memcpy(buf + iPos, &i, 2);
	iPos = iPos + 2;	
	i = 0x00;
	memcpy(buf + iPos, &i, 1);   
}

void make_require_delay_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size)
{
	buf_size = buf_size;
	unsigned iPos = 0;
	unsigned i = 0;

	iPos = make_package_head(IP_BROADCAST, PORT_REQUIRE_DELAY, CMD_SEARCH, buf);
	i = CMD_BOARDCAST_PORT_R;
	memcpy(buf + iPos, &i, 1);
	iPos = iPos + 1;

	i = equipment_ptr->boardcast_port_[CURRENT_EQUIPMENT_BOARDCAST_INDEX];
	memcpy(buf + iPos, &i, 2);
	iPos += 2;
	memcpy(buf + iPos, &i, 2);
	iPos += 2;

	if (equipment_ptr->type_ == FDU)
	{
		i = CMD_REQUIRE_DELAY_FDU;
	}
	else if (equipment_ptr->type_ == LAUX)
	{
		i = CMD_REQUIRE_DELAY_LAUX;
	}

	memcpy(buf + iPos, &i, 1);
	iPos = iPos + 1;
	i = 0x00000000;
	memcpy(buf + iPos, &i, 4);
	iPos = iPos + 4;
	i = 0x00;
	memcpy(buf + iPos, &i, 1);
}

void make_require_delay_package_ptop(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size)
{
	buf_size = buf_size;
	unsigned iPos = 0;
	unsigned i = 0;

	iPos = make_package_head(equipment_ptr->ip_, PORT_REQUIRE_DELAY, CMD_SEARCH, buf);
	i = CMD_REQUIRE_DELAY_LAUX;
	memcpy(buf + iPos, &i, 1);
	iPos = iPos + 1;
	i = 0x00000000;
	memcpy(buf + iPos, &i, 4);
	iPos = iPos + 4;
	i = 0x00;
	memcpy(buf + iPos, &i, 1);
}

void make_set_delay_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size)
{
	buf_size = buf_size;
	unsigned iPos = 0;
	unsigned i = 0;
	iPos = make_package_head(equipment_ptr->ip_, PORT_SET_DELAY, CMD_SET, buf);
	i = CMD_MODIFY_TIME_HIGH;
	memcpy(buf + iPos, &i, 1);
	iPos = iPos + 1;
	i = 0x00000000;
	memcpy(buf + iPos, &i, 4);
	iPos = iPos + 4;
	i = CMD_MODIFY_TIME_LOW;
	memcpy(buf + iPos, &i, 1);
	iPos = iPos + 1;
	i = equipment_ptr->the_end_package_delay_time_low_ & 0x3fff;
	memcpy(buf + iPos, &i, 4);
	iPos = iPos + 4;
	i = 0x00;
	memcpy(buf + iPos, &i, 1);

	buf[DELETION_TOKEN_INDEX] = 0;
	buf[WAIT_COUNT_TOKEN_INDEX] = 3;
	buf[PROC_COUNT_TOKEN_INDEX] = 0;
}

void make_ad_loss_req_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, unsigned int port, unsigned offset)
{
	unsigned iPos = 0;
	unsigned i = 0;
	iPos = make_package_head(equipment_ptr->ip_, port, CMD_AD, buf);
	i = offset;
	memcpy(buf + iPos, &i, 2);
	iPos = iPos + 2;
	i = 0x0;
	memcpy(buf + iPos, &i, 4);
}

