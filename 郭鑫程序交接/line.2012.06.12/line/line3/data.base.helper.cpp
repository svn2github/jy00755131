/**
@brief	该文件为测线全部数据存放访问类的实现文件
@details	
@file	data.base.helper.cpp
@note	
@see	
@version	
@author	kuoxin	
@bug
*/

#include "StdAfx.h"
#include "data.base.helper.h"
namespace matrix_data{

data_base_helper* data_base_helper::instance_ = NULL;
data_base_helper::~data_base_helper(void)
{
	DeleteCriticalSection(&instrument_limit_lock_);
	DeleteCriticalSection(&sensor_limit_lock_);
	DeleteCriticalSection(&devices_info_lock_);
	DeleteCriticalSection(&point_nb_to_sn_map_lock_);
	DeleteCriticalSection(&sensor_type_lock_);
	release_data_base();
};

void data_base_helper::destroy_instance()
{
	if(instance_ != NULL){
		delete instance_;
		instance_ = NULL;
	}
}

data_base_helper* data_base_helper::create_instance()
{
	if(instance_ == NULL)	instance_ = new data_base_helper;
	return instance_;
}

TCHAR* data_base_helper::get_device_type_string(unsigned int sn)
{
	static TCHAR  string[MAX_PATH];
	switch(sn & 0x3){
	case 0x1:
		StrCpy(string, __TEXT("1:AUX"));
		break;
	case 0x2:
		StrCpy(string, __TEXT("2:AUL"));
		break;
	case 0x3:
		StrCpy(string, __TEXT("3:FDU"));
		break;
	}
	return string;
} 

TCHAR* data_base_helper::get_device_test_result_by_test_type(const_device_info& di, int type)
{
	float test_value = 0x0f;
	static TCHAR string[MAX_PATH]=__TEXT("No test");
	switch(type){
	case 4:
		test_value = di.m_fNoiseValue;
		break;
	case 5:
		test_value = di.m_fDistortionValue;
		break;
	case 6:
		test_value = di.m_fPhaseValue;
		break;
	case 7:
		test_value = di.m_fCrosstalkValue;
		break;
	case 8:
		test_value = di.m_fCMRRValue;
		break;
	}
	if(test_value != 0x0f)  _stprintf(string, __TEXT("%.2f"), test_value);
	return string;
}

TCHAR* data_base_helper::get_sensor_limit_string_by_test_type(int type)
{
	static TCHAR string[MAX_PATH] = {""};
	switch(type){
	case 15:
		StrCpy(string, __TEXT("SENSOR RESISTANCE"));
		break;
	case 14:
		StrCpy(string, __TEXT("SENSOR LEAKAGE"));
		break;
	case 1:
		StrCpy(string, __TEXT("SENSOR NOISE"));
		break;
	case 20:
		StrCpy(string, __TEXT("SENSOR TILT"));
		break;
	case 21:
		StrCpy(string, __TEXT("SENSOR TILT MODEL"));
		break;
	case 22:
		StrCpy(string, __TEXT("SENSOR SEISMONITOR"));
		break;
	}
	return string;
}

int data_base_helper::get_sensor_limit_type_by_string(TCHAR const* string)
{
	int type = 0x0;
	if(StrCmp(string, __TEXT("SENSOR RESISTANCE")) == 0 )	type = 15;
	else if(StrCmp(string, __TEXT("SENSOR LEAKAGE")) == 0 )	type = 14;
	else if(StrCmp(string, __TEXT("SENSOR NOISE")) == 0 )	type = 1;
	else if(StrCmp(string, __TEXT("SENSOR TILT")) == 0 )	type = 20;
	else if(StrCmp(string, __TEXT("SENSOR TILT MODEL")) == 0 )	type = 21;
	else if(StrCmp(string, __TEXT("SENSOR SEISMONITOR")) == 0 )	type = 22;
	return type;
}

TCHAR* data_base_helper::get_sensor_limit_info_by_type(unsigned int type)
{
	static TCHAR string[MAX_PATH] = {""};
	sensor_limit_vector::iterator itr, itr2;
	for(itr = data_base_ptr_->sensor_limits_.begin(); itr != data_base_ptr_->sensor_limits_.end(); ++itr){
		if(itr->m_uiTestType == type){
			if(itr->m_uiTestType == 15){
				itr2 = itr;
				++itr2;
				_stprintf(string, __TEXT("%.2f-%.2f %s"), itr->m_fLimit, itr2->m_fLimit, itr->m_pcUnit);
				break;
			}
			else{
				_stprintf(string, __TEXT("%.2f%s"), itr->m_fLimit, itr->m_pcUnit);
				break;
			}
		}
	}
	return string;
}

float data_base_helper::get_sensor_limit_value_by_type(unsigned int type)
{
	float value = 0x0;
	sensor_limit_vector::iterator itr;

	for(itr = data_base_ptr_->sensor_limits_.begin(); itr != data_base_ptr_->sensor_limits_.end(); ++itr){
		if(itr->m_uiTestType == type){
			value = itr->m_fLimit;
		}
	}
	return value;
}

TCHAR* data_base_helper::get_test_result_string(byte check, float value, TCHAR const* unit)
{
	static TCHAR result[MAX_PATH] = __TEXT("");
	switch(check)
	{
	case 0:	// 0：未测试
		StrCpy(result, __TEXT("No test"));
		break;
	case 1:	// 1：测试合格
	case 2:	// 2：测试不合格
		_stprintf(result, __TEXT("%3.2f%s"), value, unit);
		break;
	case 3:	// 3：无信号
		StrCpy(result, __TEXT("No signal"));
		break;
	case 4:	// 4：未判读
		StrCpy(result, __TEXT("No Judge"));
		break;
	}
	return result;
}

TCHAR* data_base_helper::get_test_result_string(byte check)
{
	static TCHAR result[MAX_PATH]= __TEXT("");
	switch(check)
	{
	case 0:	// 0：未测试
		StrCpy(result, __TEXT("No test"));
		break;
	case 1:	// 1：测试合格
		StrCpy(result, __TEXT("OK"));
		break;
	case 2:	// 2：测试不合格
		StrCpy(result, __TEXT("Fail"));
		break;
	case 3:	// 3：无信号
		StrCpy(result, __TEXT("No signal"));
		break;
	case 4:	// 4：未判读
		StrCpy(result, __TEXT("No Judge"));
		break;
	}
	return result;
}

TCHAR* data_base_helper::get_sensor_test_unit_by_test_type(const_device_info& di, unsigned int test_type)
{
	TCHAR *tmp_ptr= NULL,
		*tmp_ptr2 = NULL;
	sensor_limit_vector& vect = data_base_ptr_->sensor_limits_;
	sensor_limit_vector::iterator itr;
	sensor_vector::const_iterator  s_itr = di.sensors_.begin();

	for(itr = vect.begin(); itr != vect.end(); ++itr){
		if(itr->m_uiTestType == test_type){
			tmp_ptr = itr->m_pcUnit;
			break;
		}
	}
	//get the value
	switch(test_type){
	case 15:	// 检波器电阻。 Resistance
		tmp_ptr2 = get_test_result_string(s_itr->resistance_check_, s_itr->resistance_, tmp_ptr);
		break;
	case 14:	// 检波器漏电。 Leakage
		tmp_ptr2 = get_test_result_string(s_itr->leakage_check_, s_itr->leakage_, tmp_ptr);
		break;
	case 1:	// 检波器噪声。 Noise
		tmp_ptr2 = get_test_result_string(s_itr->noise_check_, s_itr->noise_, tmp_ptr);
		break;
	case 20:	// 检波器倾斜度。 Tilt
		tmp_ptr2 = get_test_result_string(s_itr->tilt_check_, s_itr->tilt_, tmp_ptr);	
		break;
	case 21:	// 检波器倾斜度模型。Tilt Model
		tmp_ptr2 = get_test_result_string(s_itr->tilt_model_check_, s_itr->tilt_model_, tmp_ptr);
		break;
	case 22:	// 检波器倾斜度模型。Tilt Model
		tmp_ptr2 = get_test_result_string(s_itr->noise_monitor_check_, s_itr->noise_monitor_, tmp_ptr);
		break;
	case 40:	// 所有检波器测试
		tmp_ptr2 = get_test_result_string(s_itr->check_all_);
		break;
	}
	return tmp_ptr2;
}

/**
* @fn得到测试结果
* @param unsigned int type 测试类型
* @param float limit 噪声监测判据
* @return byte 测试结果
*/
byte data_base_helper::get_test_result_by_type(device_info& di, unsigned int type, float limit)
{
	sensor& si = *(di.sensors_.begin());
	byte check = 0;
	switch(type){
	case 15:	// 检波器电阻。 Resistance
		check = si.resistance_check_;
		break;
	case 14:	// 检波器漏电。 Leakage
		check = si.leakage_check_;
		break;
	case 1:	// 检波器噪声。 Noise
		check = si.noise_check_;
		break;
	case 20:	// 检波器倾斜度。 Tilt
		check = si.tilt_check_;
		break;
	case 21:	// 检波器倾斜度模型。Tilt Model
		check = si.tilt_model_check_;
		break;
	case 22:	// 检波器噪声监测
		// 判断：4：未判读
		if(4 == si.noise_monitor_check_){
			si.noise_monitor_check_ = 2;	// 2：测试不合格
			if(si.noise_monitor_ < limit)
				si.noise_monitor_check_ = 1;	// 1：测试合格
		}
		check = si.noise_monitor_check_;
		break;
	case 40:	// 所有检波器测试
		check = si.check_all_;
		break;
	}
	return check;

}

};