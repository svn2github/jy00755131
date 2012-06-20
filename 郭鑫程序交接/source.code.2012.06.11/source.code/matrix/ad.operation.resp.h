#ifndef __MATRIX_AD_OPERATION_RESP_H__
#define __MATRIX_AD_OPERATION_RESP_H__

enum __enuAD_OPERATION_RESP_ERROR
{
	AD_SETTING_RECV_SIZE_ERROR = 0x5001,
	AD_SETTING_FORMAT_ERROR,
	AD_SETTING_SEND_ERROR,
};

bool init_ad_operation_resp();
void release_ad_operation_resp();
bool insert_into_ad_operation_list(EQUIPMENT_ITEM_PTR, MAKE_AD_CMD_PACKAGE_PTR, void*);
void start_ad_operation(unsigned int elapsed_time);
void stop_ad_operation();
bool be_ad_operation_completed();
bool is_ad_operation_running();

#endif//__MATRIX_AD_OPERATION_RESP_H__