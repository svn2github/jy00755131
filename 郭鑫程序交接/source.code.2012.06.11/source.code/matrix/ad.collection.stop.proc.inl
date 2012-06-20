/*
*	描述：该文件实现采集设备的AD采集结束设置功能
*	文件：ad.collection.stop.proc.cpp
*	注释：
*	版本：
*	完成人：
*	参考文献：
*	修改：
*
*/

AD_COMMAND_ITEM __ad_end_commands[] = 
{
	{make_ad_set_SDATAC_package, 200, NULL},
	{make_ad_set_SDATAC_package, 200, NULL},
	{make_ad_set_SDATAC_package, 200, NULL}
};

bool init_ad_end_proc(){	return init_ad_operation_proc();}
void release_ad_end_proc(){	release_ad_operation_proc();}
bool proc_ad_end(){	__be_ad_collection_stopping = true; return proc_ad_operation(__ad_end_commands, sizeof(__ad_end_commands)/sizeof(__ad_end_commands[0]));}

