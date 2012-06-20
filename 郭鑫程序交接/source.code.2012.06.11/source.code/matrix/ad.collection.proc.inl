/*
*	描述：该文件实现采集设备的AD采集设置功能
*	文件：ad.collection.proc.cpp
*	注释：
*	版本：
*	完成人：
*	参考文献：
*	修改：
*
*/


AD_COMMAND_ITEM __ad_collecion_commands[] = 
{
	{make_ad_set_switch7_tb_package, 200, NULL},
	{make_ad_set_SDATAC_package, 200, NULL},
	{make_ad_set_WREG_begin_package, 200, NULL},
	{make_ad_set_RDATAC_package, 200, NULL},
	{make_ad_set_TB_package, 200, &__heartbeat_data}
};

bool init_ad_collection_proc(){	return init_ad_operation_proc();}
void release_ad_collecion_proc(){	release_ad_operation_proc();}
bool proc_ad_collection(){	return proc_ad_operation(__ad_collecion_commands, sizeof(__ad_collecion_commands)/sizeof(__ad_collecion_commands[0]));}

