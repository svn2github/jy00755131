#ifndef __MATRIX_AD_OPERATION_PROC_H__
#define __MATRIX_AD_OPERATION_PROC_H__

enum __enuAD_OPERATION_PROC_ERROR
{
};

bool init_ad_operation_proc();
void release_ad_operation_proc();
bool proc_ad_operation(AD_COMMAND_ITEM_PTR, unsigned int);

#endif//__MATRIX_AD_OPERATION_PROC_H__