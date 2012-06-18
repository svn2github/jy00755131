/**
@brief	该文件为测线全部数据存放访问类的实现文件
@details	
@file	data.base.helper.h
@note	
@see	
@version	
@author	kuoxin	
@bug
*/
#pragma once

#include "data.base.h"

namespace matrix_data{
class data_base_helper
{
public:
	typedef data_base::sensor sensor;
	typedef data_base::sensor_vector sensor_vector;
	typedef data_base::instrument_limit instrument_limit;
	typedef data_base::sensor_limit sensor_limit;
	typedef data_base::instrument_limit_vector instrument_limit_vector;
	typedef data_base::sensor_limit_vector sensor_limit_vector;
	typedef data_base::device_info_map device_info_map;
	typedef device_info device_info;
	typedef data_base::point_nb_to_sn_map point_nb_to_sn_map;
	typedef data_base::sensor_type sensor_type;
	typedef data_base::sensor_type_vector sensor_type_vector;

	typedef const data_base::sensor const_sensor;
	typedef const data_base::sensor_vector const_sensor_vector;
	typedef const data_base::instrument_limit const_instrument_limit;
	typedef const data_base::sensor_limit const_sensor_limit;
	typedef const data_base::instrument_limit_vector const_instrument_limit_vector;
	typedef const data_base::sensor_limit_vector const_sensor_limit_vector;
	typedef const data_base::device_info_map const_device_info_map;
	typedef const device_info const_device_info;
	typedef const data_base::point_nb_to_sn_map const_point_nb_to_sn_map;
	typedef const data_base::sensor_type const_sensor_type;
	typedef const data_base::sensor_type_vector const_sensor_type_vector;
	
	typedef const data_base::sensor& const_sensor_reference;
	typedef const data_base::sensor_vector& const_sensor_vector_reference;
	typedef const data_base::instrument_limit& const_instrument_limit_reference;
	typedef const data_base::sensor_limit& const_sensor_limit_reference;
	typedef const data_base::instrument_limit_vector& const_instrument_limit_vector_reference;
	typedef const data_base::sensor_limit_vector& const_sensor_limit_vector_reference;
	typedef const data_base::device_info_map& const_device_info_map_reference;
	typedef const device_info& const_device_info_reference;
	typedef const data_base::point_nb_to_sn_map& const_point_nb_to_sn_map_reference;
	typedef const data_base::sensor_type& const_sensor_type_reference;
	typedef const data_base::sensor_type_vector& const_sensor_type_vector_reference;
public:
	virtual ~data_base_helper(void);
private:
	data_base_helper():data_base_ptr_(NULL){
		InitializeCriticalSection(&instrument_limit_lock_);
		InitializeCriticalSection(&sensor_limit_lock_);
		InitializeCriticalSection(&devices_info_lock_);
		InitializeCriticalSection(&point_nb_to_sn_map_lock_);
		InitializeCriticalSection(&sensor_type_lock_);
		create_data_base();
	};
	data_base_helper (data_base_helper const& dhp){};
	data_base_helper& operator=(data_base_helper const& dhp){ return *this;};
public:
	static data_base_helper* create_instance();
	static void destroy_instance();
	data_base* set_data_base(data_base* data_base_ptr){
		data_base* ptr = data_base_ptr_;
		data_base_ptr_ = data_base_ptr;
		return ptr;
	}
	data_base* get_data_base(){	return data_base_ptr_;}
protected://create_data_base & release_data_base 成对调用
	void* create_data_base(){
		if(data_base_ptr_ == NULL)	{	data_base_ptr_ = new data_base; 	}
		return data_base_ptr_;
	}
	void release_data_base(){
		if(data_base_ptr_ != NULL){		delete data_base_ptr_;		data_base_ptr_ = NULL;	}
	}

public: // element accessed
	void instrument_limit_lock() const{EnterCriticalSection(const_cast<LPCRITICAL_SECTION>(&instrument_limit_lock_));};
	void instrument_limit_unlock() const{LeaveCriticalSection(const_cast<LPCRITICAL_SECTION>(&instrument_limit_lock_));};
	instrument_limit_vector const& get_instrument_limit_vector() const{ return data_base_ptr_->instrument_limits_;};

	void sensor_limit_lock() const{EnterCriticalSection(const_cast<LPCRITICAL_SECTION>(&sensor_limit_lock_));};
	void sensor_limit_unlock() const{LeaveCriticalSection(const_cast<LPCRITICAL_SECTION>(&sensor_limit_lock_));};
	sensor_limit_vector const& get_sensor_limit_vector() const{ return data_base_ptr_->sensor_limits_;};

	void devices_info_lock() const{EnterCriticalSection(const_cast<LPCRITICAL_SECTION>(&devices_info_lock_));};
	void devices_info_unlock()const{LeaveCriticalSection(const_cast<LPCRITICAL_SECTION>(&devices_info_lock_));};
	device_info_map& get_devices_info(){return data_base_ptr_->devices_info_;};

	void point_nb_to_sn_map_lock() const{EnterCriticalSection(const_cast<LPCRITICAL_SECTION>(&point_nb_to_sn_map_lock_));};
	void point_nb_to_sn_map_unlock()const{LeaveCriticalSection(const_cast<LPCRITICAL_SECTION>(&point_nb_to_sn_map_lock_));};
	point_nb_to_sn_map& get_point_nb_to_sn_map(){return data_base_ptr_->point_nb_to_sn_map_;};

	void sensor_type_lock(){EnterCriticalSection(&sensor_type_lock_);};
	void sensor_type_unlock(){LeaveCriticalSection(&sensor_type_lock_);};
	sensor_type_vector& get_sensor_type_vector(){return data_base_ptr_->sensor_type_vector_;};

	bool add_instrument_limit(instrument_limit const& il){
		instrument_limit_lock();
		data_base_ptr_->instrument_limits_.push_back(il); 
		instrument_limit_unlock();
		return true; 
	};
	bool delete_instrument_limit(instrument_limit const& il){
		bool is_ok = false;
		instrument_limit_vector::iterator itr;
		instrument_limit_lock();
		for(itr = data_base_ptr_->instrument_limits_.begin(); itr != data_base_ptr_->instrument_limits_.end(); ++itr){
			if(*itr == il){ 
				is_ok = true; 
				data_base_ptr_->instrument_limits_.erase(itr);
				break;}
		}
		instrument_limit_unlock();
		return is_ok;
	};
	bool modify_instrument_limit(instrument_limit const& il){
		bool is_ok = false;
		instrument_limit_vector::iterator itr;
		instrument_limit_lock();
		for(itr = data_base_ptr_->instrument_limits_.begin(); itr != data_base_ptr_->instrument_limits_.end(); ++itr){
			if(itr->m_uiNb == il.m_uiNb){
				*itr = il;	is_ok = true; 	break;
			}
		}
		instrument_limit_unlock();
		return is_ok;
	}
	bool find_instrument_limit_by_name(instrument_limit& il){
		bool is_ok = false;
		instrument_limit_vector::iterator itr;
		instrument_limit_lock();
		for(itr = data_base_ptr_->instrument_limits_.begin(); itr != data_base_ptr_->instrument_limits_.end(); ++itr){
			if(_tcsicmp(itr->m_pcDescr, il.m_pcDescr)==0){		il = *itr;	is_ok = true; 	break;	}
		}
		instrument_limit_unlock();
		return is_ok;
	}
	bool add_sensor_limit(sensor_limit const& sl){ 
		sensor_limit_lock();
		data_base_ptr_->sensor_limits_.push_back(sl); 
		sensor_limit_unlock();
		return true;	
	};
	bool delete_sensor_limit(sensor_limit const& sl){	
		bool is_ok = false;
		sensor_limit_vector::iterator itr;
		sensor_limit_lock();
		for(itr = data_base_ptr_->sensor_limits_.begin(); itr != data_base_ptr_->sensor_limits_.end(); ++itr){
			if(*itr == sl){ 
				data_base_ptr_->sensor_limits_.erase(itr);
				is_ok = true; break;
			}
		}
		sensor_limit_unlock();
		return is_ok;
	};
	bool modify_sensor_limit(sensor_limit const& sl){	
		bool is_ok = false;
		sensor_limit_vector::iterator itr;
		sensor_limit_lock();
		for(itr = data_base_ptr_->sensor_limits_.begin(); itr != data_base_ptr_->sensor_limits_.end(); ++itr){
			if(itr->m_uiNb == sl.m_uiNb){ *itr = sl; is_ok = true; break;}
		}
		sensor_limit_unlock();
		return is_ok;
	};
	bool add_device_info(device_info const& di){
		data_base::ip_to_sn_map::iterator itr;
		devices_info_lock();
		data_base_ptr_->devices_info_[di.sn_] = di;
		itr = data_base_ptr_->ip_to_sn_map_.find(di.ip_);
		if(itr != data_base_ptr_->ip_to_sn_map_.end()) data_base_ptr_->ip_to_sn_map_.erase(itr);
		data_base_ptr_->ip_to_sn_map_[di.ip_] = di.sn_;
		devices_info_unlock();
		return true; 
	};
	bool delete_device_info(device_info const& di){	return delete_device_info(di.sn_);	};
	bool delete_device_info(int sn){
		bool is_ok = false;
		device_info_map::iterator itr;
		devices_info_lock();
		itr = data_base_ptr_->devices_info_.find(sn);
		if(itr != data_base_ptr_->devices_info_.end()){
			data_base_ptr_->ip_to_sn_map_.erase(itr->second.ip_);
			data_base_ptr_->devices_info_.erase(itr);
			is_ok = true;
		}
		devices_info_unlock();
		return is_ok;
	};
	bool find_device(device_info& di)
	{
		bool is_ok = false;
		device_info_map::iterator itr;
		devices_info_lock();
		itr = data_base_ptr_->devices_info_.find(di.sn_);
		if(itr != data_base_ptr_->devices_info_.end()){
			di = itr->second;
			is_ok = true;
		}
		devices_info_unlock();
		return is_ok;
	} 
	bool find_device(int ip, device_info& di)
	{
		bool is_ok = false;
		data_base::ip_to_sn_map::iterator ip_itr;
		device_info_map::iterator itr;
		devices_info_lock();
		ip_itr = data_base_ptr_->ip_to_sn_map_.find(ip);
		if(ip_itr != data_base_ptr_->ip_to_sn_map_.end()){
			di.sn_ = ip_itr->second;
			itr = data_base_ptr_->devices_info_.find(di.sn_);
			if(itr != data_base_ptr_->devices_info_.end()){
				di = itr->second;
				is_ok = true;
			}
		}
		devices_info_unlock();
		return is_ok;	
	}
	void add_point_nb_to_sn_info(int point_nb, int sn)
	{
		point_nb_to_sn_map_lock();
		data_base_ptr_->point_nb_to_sn_map_[point_nb] = sn;
		point_nb_to_sn_map_unlock();
	}
	unsigned int find_point_nb_and_return_sn(int point_nb)
	{
		unsigned int sn = 0x0;
		point_nb_to_sn_map::iterator itr;
		point_nb_to_sn_map_lock();
		itr = data_base_ptr_->point_nb_to_sn_map_.find(point_nb);
		if(itr != data_base_ptr_->point_nb_to_sn_map_.end()){
			sn = itr->second;
		}
		point_nb_to_sn_map_unlock();
		return sn;
	} 
	bool delete_point_nb_to_sn_info(int point_nb)
	{
		bool is_ok = false;
		point_nb_to_sn_map::iterator itr;
		point_nb_to_sn_map_lock();
		itr = data_base_ptr_->point_nb_to_sn_map_.find(point_nb);
		if(itr != data_base_ptr_->point_nb_to_sn_map_.end()){
			data_base_ptr_->point_nb_to_sn_map_.erase(itr);
			is_ok = true;
		}
		point_nb_to_sn_map_unlock();
		return is_ok;
	}
	void add_sensor_type(sensor_type const& st)
	{
		sensor_type_lock();
		data_base_ptr_->sensor_type_vector_.push_back(st);
		sensor_type_unlock();
	};
	bool delete_sensor_type(int nb)
	{
		bool is_ok = false;
		sensor_type_vector::iterator itr;
		sensor_type_lock();
		for(itr = data_base_ptr_->sensor_type_vector_.begin(); itr != data_base_ptr_->sensor_type_vector_.end(); ++itr){
			if(itr->nb_ == nb){
				data_base_ptr_->sensor_type_vector_.erase(itr);
				is_ok = true;
				break;
			}
		}
		sensor_type_unlock();
		return is_ok;
	};
	bool find_sensor_type(sensor_type& st)
	{
		bool is_ok = false;
		sensor_type_vector::iterator itr;
		sensor_type_lock();
		for(itr = data_base_ptr_->sensor_type_vector_.begin(); itr != data_base_ptr_->sensor_type_vector_.end(); ++itr){
			if(itr->nb_ == st.nb_){
				st = *itr;
				is_ok = true;
				break;
			}
		}
		sensor_type_unlock();
		return is_ok;
	}
	unsigned int get_line_number(){return data_base_ptr_->line_number_;};
	unsigned int get_section_number(){return data_base_ptr_->section_number_;};
	void set_line_number(unsigned int line_number){data_base_ptr_->line_number_ = line_number;};
	void set_section_number(unsigned int section_number){data_base_ptr_->section_number_ = section_number;};
	static TCHAR* get_device_type_string(unsigned int sn);
	static TCHAR* get_device_test_result_by_test_type(const_device_info& di, int type);
	static TCHAR* get_sensor_limit_string_by_test_type(int type);
	static int get_sensor_limit_type_by_string(TCHAR const* string);
	TCHAR* get_sensor_limit_info_by_type(unsigned int type); 
	TCHAR* get_sensor_test_unit_by_test_type(const_device_info& di, unsigned int test_type);
	void set_point_nb_to_sn_map(int line_num = 0, int section_num = 0){
		int tmp_line = line_num == 0 ? data_base_ptr_->line_number_ * 2 - 1: line_num * 2 - 1;
		int tmp_section = section_num == 0 ? data_base_ptr_->section_number_ : section_num;
		for(int i = 0; i < tmp_line; ++i){
			for(int j = 1 - tmp_section; j < tmp_section; ++j){
				add_point_nb_to_sn_info(j*1000 + i + 1, 0);
			}
		}
	};
	float get_sensor_limit_value_by_type(unsigned int type);
	byte get_test_result_by_type(device_info& di, unsigned int type, float limit);
protected:
	TCHAR* get_test_result_string(byte check, float value, TCHAR const* unit);
	TCHAR* get_test_result_string(byte check);
	
private:
	static data_base_helper* instance_;//!<data_base_helper实例
	data_base* data_base_ptr_;//!< 数据存放类
	CRITICAL_SECTION sensor_limit_lock_;
	CRITICAL_SECTION instrument_limit_lock_;
	CRITICAL_SECTION devices_info_lock_;
	CRITICAL_SECTION point_nb_to_sn_map_lock_;
	CRITICAL_SECTION sensor_type_lock_;
};
};
