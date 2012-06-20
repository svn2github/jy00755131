/*
*	描述：该文件实现采集设备的AD零漂设置功能
*	文件：ad.offset.proc.cpp
*	注释：
*	版本：
*	完成人：
*	参考文献：
*	修改：
*
*/

AD_COMMAND_ITEM __ad_zero_offset_commands[] = 
{
	{make_ad_set_switch7_package, 200, NULL},
	{make_ad_set_SDATAC_package, 200, NULL},
	{make_ad_set_SDATAC_package, 200, NULL},
	{make_ad_set_WREG_offset_package, 200, NULL},
	{make_ad_set_RREG_package, 200, NULL},
	{make_ad_set_SDATAC_package, 200, NULL},
	{make_ad_set_SYNC_package, 200, NULL},
	{make_ad_set_RDATAC_package, 500, NULL},
	{make_ad_set_SDATAC_package, 200, NULL},
	{make_ad_set_SDATAC_package, 200, NULL},
	{make_ad_set_OFSCAL_package, 200, NULL},
	{make_ad_set_RDATAC_package, 500, NULL},
	{make_ad_set_SDATAC_package, 200, NULL},
	{make_ad_set_SDATAC_package, 200, NULL},
	{make_ad_set_RREG_package, 200, NULL}
};

bool init_ad_offset_proc(){	return init_ad_operation_proc();}
void release_ad_offset_proc(){	release_ad_operation_proc();}
bool proc_ad_offset(){	return proc_ad_operation(__ad_zero_offset_commands, sizeof(__ad_zero_offset_commands)/sizeof(__ad_zero_offset_commands[0]));}

