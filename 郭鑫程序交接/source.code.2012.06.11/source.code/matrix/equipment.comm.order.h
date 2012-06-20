#ifndef __MATRIX_EQUIPMENT_COMM_ORDER_H__
#define __MATRIX_EQUIPMENT_COMM_ORDER_H__

typedef void (*MAKE_AD_CMD_PACKAGE_PTR)(EQUIPMENT_ITEM_PTR, unsigned char*, unsigned int, void*);

typedef struct __tagAD_COMMAND_ITEM
{
	MAKE_AD_CMD_PACKAGE_PTR function_ptr_;
	unsigned int elapsed_time_;
	void* data_ptr_;
}AD_COMMAND_ITEM, *AD_COMMAND_ITEM_PTR;

int make_package_head(int ip_destination, int port, int cmd, unsigned char* frame_data);

void make_ip_setting_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size);

void make_ad_set_TB_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr);
void make_ad_set_TB_TO_0_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr);
void make_ad_set_WREG_begin_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr);
void make_ad_set_WREG_offset_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr);
void make_ad_set_WREG_init_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr);
void make_ad_set_OFSCAL_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr);
void make_ad_set_RDATAC_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr);
void make_ad_set_SYNC_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr);
void make_ad_set_RREG_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr);
void make_ad_set_switch7_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr);
void make_ad_set_switch7_tb_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr);
void make_ad_set_switch4_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr);
void make_ad_set_switch4_tb_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr);
void make_ad_set_switch2_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr);
void make_ad_set_switch2_tb_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr);
void make_ad_set_SDATAC_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr);
void make_ad_set_sin_function_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr);
void make_ad_set_return_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, void* void_ptr);

void make_require_delay_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size);
void make_require_delay_package_ptop(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size);

void make_set_delay_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size);

void make_ad_loss_req_package(EQUIPMENT_ITEM_PTR equipment_ptr, unsigned char* buf, unsigned int buf_size, unsigned int port, unsigned offset);

#endif//__MATRIX_EQUIPMENT_COMM_ORDER_H__