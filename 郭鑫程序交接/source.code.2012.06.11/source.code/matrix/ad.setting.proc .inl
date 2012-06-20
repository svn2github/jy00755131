/*
*	描述：该文件实现采集设备的AD设置功能
*	文件：ad.setting.proc.cpp
*	注释：
*	版本：
*	完成人：
*	参考文献：
*	修改：
*
*/

AD_COMMAND_ITEM __ad_setting_commands[] = 
{
	{make_ad_set_TB_TO_0_package, 200, NULL},
	{make_ad_set_sin_function_package, 200, NULL},
	{make_ad_set_SDATAC_package, 200, NULL},
	{make_ad_set_switch2_package, 200, NULL},
	{make_ad_set_switch2_tb_package, 200, NULL},
	{make_ad_set_switch4_package, 200, NULL},
	{make_ad_set_switch4_tb_package, 200, NULL},
	{make_ad_set_SDATAC_package, 200, NULL},
	{make_ad_set_WREG_init_package, 200, NULL},
	{make_ad_set_RREG_package, 200, NULL},
	{make_ad_set_return_package, 200, NULL}
};

bool init_ad_setting_proc(){	return init_ad_operation_proc();}
void release_ad_setting_proc(){	release_ad_operation_proc();}
bool proc_ad_setting(){	return proc_ad_operation(__ad_setting_commands, sizeof(__ad_setting_commands)/sizeof(__ad_setting_commands[0]));}

