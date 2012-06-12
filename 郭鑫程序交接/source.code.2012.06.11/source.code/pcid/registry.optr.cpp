#include "../common/config.h"
#include "pcid.h"
#include "registry.optr.h"

extern APPLICATION_ENVIRONMENT __application_environment;

bool open_registry_private(PHKEY key_handler, DWORD* disposition)
{
	LONG ret_code = 0x0;
	char buf[MAX_STRING_SIZE] = {0};

	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_PRODUCT_INFORMATION_STRING, buf, sizeof(buf));
	ret_code = RegCreateKeyEx(HKEY_CURRENT_USER, buf, NULL, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS, NULL, key_handler, disposition); 

	if(ret_code != ERROR_SUCCESS) return false;
	return true;
}

int read_registry_private(APPLICATION_ENVIRONMENT_PTR env_ptr, HKEY key_handler)
{
	DWORD type = 0x0;
	LONG ret_code = 0x0;
	DWORD size = sizeof(env_ptr->log_ip_);
	char buf[MAX_STRING_SIZE] = {0};

	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_LISTEN_PORT_STRING, buf, sizeof(buf));
	ret_code = RegQueryValueEx(key_handler, buf, NULL, &type, (BYTE*)&env_ptr->listen_port_, &size);
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_REMOTE_PORT_STRING, buf, sizeof(buf));
	size = sizeof(env_ptr->log_ip_); 
	ret_code = RegQueryValueEx(key_handler, buf, NULL, &type, (BYTE*)&env_ptr->remote_port_, &size);
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	size = sizeof(env_ptr->log_ip_); 
	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_RECV_BUFFER_SIZE_STRING, buf, sizeof(buf));
	ret_code = RegQueryValueEx(key_handler, buf, NULL, &type, (BYTE*)&env_ptr->recv_buff_size_, &size);
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	size = sizeof(env_ptr->log_ip_); 
	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_SEND_BUFFER_SIZE_STRING, buf, sizeof(buf));
	ret_code = RegQueryValueEx(key_handler, buf, NULL, &type, (BYTE*)&env_ptr->send_buff_size_, &size);
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	size = sizeof(env_ptr->log_ip_); 
	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_RECV_THREAD_WAITING_TIME_STRING, buf, sizeof(buf));
	ret_code = RegQueryValueEx(key_handler, buf, NULL, &type, (BYTE*)&env_ptr->recv_thread_waiting_time_, &size);
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	size = sizeof(env_ptr->log_ip_); 
	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_INPUT_WAITING_TIME_STRING, buf, sizeof(buf));
	ret_code = RegQueryValueEx(key_handler, buf, NULL, &type, (BYTE*)&env_ptr->parallel_port_input_waiting_time_, &size);
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	size = sizeof(env_ptr->log_ip_); 
	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_INPUT_BUFFER_SIZE_STRING, buf, sizeof(buf));
	ret_code = RegQueryValueEx(key_handler, buf, NULL, &type, (BYTE*)&env_ptr->parallel_port_data_input_buffer_size_, &size);
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	size = sizeof(env_ptr->log_ip_); 
	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_ENABLE_TO_CHECK_CRC, buf, sizeof(buf));
	ret_code = RegQueryValueEx(key_handler, buf, NULL, &type, (BYTE*)&env_ptr->enable_to_check_crc_, &size);
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	size = sizeof(env_ptr->log_ip_); 
	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_ENABLE_TO_TRANSFER_DATA, buf, sizeof(buf));
	ret_code = RegQueryValueEx(key_handler, buf, NULL, &type, (BYTE*)&env_ptr->enable_to_transfer_data_, &size);
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	size = sizeof(env_ptr->log_ip_); 
	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_INPUT_BUFFERS_NUM_STRING, buf, sizeof(buf));
	ret_code = RegQueryValueEx(key_handler, buf, NULL, &type, (BYTE*)&env_ptr->parallel_port_data_input_buffer_num_, &size);
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	size = sizeof(env_ptr->log_ip_); 
	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_OUTPUT_BUFFER_SIZE_STRING, buf, sizeof(buf));
	ret_code = RegQueryValueEx(key_handler, buf, NULL, &type, (BYTE*)&env_ptr->parallel_port_output_buffer_size_, &size);
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	size = sizeof(env_ptr->log_ip_); 
	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_LOG_PORT_STRING, buf, sizeof(buf));
	ret_code = RegQueryValueEx(key_handler, buf, NULL, &type, (BYTE*)&env_ptr->log_port_, &size);
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	size = sizeof(env_ptr->log_ip_); 
	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_LOG_IP_STRING, buf, sizeof(buf));
	ret_code = RegQueryValueEx(key_handler, buf, NULL, &type, (BYTE*)&env_ptr->log_ip_, &size);
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	size = sizeof(env_ptr->log_ip_); 
	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_REMOTE_IP_STRING, buf, sizeof(buf));
	ret_code = RegQueryValueEx(key_handler, buf, NULL, &type, (BYTE*)&env_ptr->remote_ip_, &size);
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	size = sizeof(env_ptr->log_ip_); 
	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_IGNORED_PORT_STRING, buf, sizeof(buf));
	ret_code = RegQueryValueEx(key_handler, buf, NULL, &type, (BYTE*)&env_ptr->ignored_port_, &size);
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	size = sizeof(env_ptr->log_ip_); 
	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_BE_ALL_INFO_OUTPUTTED_STRING, buf, sizeof(buf));
	ret_code = RegQueryValueEx(key_handler, buf, NULL, &type, (BYTE*)&env_ptr->be_all_info_outputted_, &size);
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	return ERROR_SUCCESS;
}

int write_registry_private(APPLICATION_ENVIRONMENT_PTR env_ptr, HKEY key_handler)
{
	LONG ret_code = 0x0;
	char buf[MAX_STRING_SIZE] = {0};

	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_LISTEN_PORT_STRING, buf, sizeof(buf));
	ret_code = RegSetValueEx(key_handler, buf, NULL, REG_DWORD, (BYTE*)&env_ptr->listen_port_, 
		sizeof(env_ptr->listen_port_));
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_REMOTE_PORT_STRING, buf, sizeof(buf));
	ret_code = RegSetValueEx(key_handler, buf, NULL, REG_DWORD, (BYTE*)&env_ptr->remote_port_, 
		sizeof(env_ptr->remote_port_));
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_RECV_BUFFER_SIZE_STRING, buf, sizeof(buf));
	ret_code = RegSetValueEx(key_handler, buf, NULL, REG_DWORD, (BYTE*)&env_ptr->recv_buff_size_, 
		sizeof(env_ptr->recv_buff_size_));
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_SEND_BUFFER_SIZE_STRING, buf, sizeof(buf));
	ret_code = RegSetValueEx(key_handler, buf, NULL, REG_DWORD, (BYTE*)&env_ptr->send_buff_size_, 
		sizeof(env_ptr->send_buff_size_));
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_RECV_THREAD_WAITING_TIME_STRING, buf, sizeof(buf));
	ret_code = RegSetValueEx(key_handler, buf, NULL, REG_DWORD, (BYTE*)&env_ptr->recv_thread_waiting_time_, 
		sizeof(env_ptr->recv_thread_waiting_time_));
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_INPUT_WAITING_TIME_STRING, buf, sizeof(buf));
	ret_code = RegSetValueEx(key_handler, buf, NULL, REG_DWORD, (BYTE*)&env_ptr->parallel_port_input_waiting_time_, 
		sizeof(env_ptr->parallel_port_input_waiting_time_));
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_INPUT_BUFFER_SIZE_STRING, buf, sizeof(buf));
	ret_code = RegSetValueEx(key_handler, buf, NULL, REG_DWORD, (BYTE*)&env_ptr->parallel_port_data_input_buffer_size_, 
		sizeof(env_ptr->parallel_port_data_input_buffer_size_));
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_INPUT_BUFFERS_NUM_STRING, buf, sizeof(buf));
	ret_code = RegSetValueEx(key_handler, buf, NULL, REG_DWORD, (BYTE*)&env_ptr->parallel_port_data_input_buffer_num_, 
		sizeof(env_ptr->parallel_port_data_input_buffer_num_));
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_OUTPUT_BUFFER_SIZE_STRING, buf, sizeof(buf));
	ret_code = RegSetValueEx(key_handler, buf, NULL, REG_DWORD, (BYTE*)&env_ptr->parallel_port_output_buffer_size_, 
		sizeof(env_ptr->parallel_port_output_buffer_size_));
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_LOG_PORT_STRING, buf, sizeof(buf));
	ret_code = RegSetValueEx(key_handler, buf, NULL, REG_DWORD, (BYTE*)&env_ptr->log_port_, 
		sizeof(env_ptr->log_port_));
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_ENABLE_TO_CHECK_CRC, buf, sizeof(buf));
	ret_code = RegSetValueEx(key_handler, buf, NULL, REG_DWORD, (BYTE*)&env_ptr->enable_to_check_crc_, 
		sizeof(env_ptr->enable_to_check_crc_));
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_ENABLE_TO_TRANSFER_DATA, buf, sizeof(buf));
	ret_code = RegSetValueEx(key_handler, buf, NULL, REG_DWORD, (BYTE*)&env_ptr->enable_to_transfer_data_, 
		sizeof(env_ptr->enable_to_transfer_data_));
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_LOG_IP_STRING, buf, sizeof(buf));
	ret_code = RegSetValueEx(key_handler, buf, NULL, REG_SZ, (BYTE*)&env_ptr->log_ip_, 
		strlen(env_ptr->log_ip_) + 1);
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_REMOTE_IP_STRING, buf, sizeof(buf));
	ret_code = RegSetValueEx(key_handler, buf, NULL, REG_SZ, (BYTE*)&env_ptr->remote_ip_, 
		strlen(env_ptr->remote_ip_) + 1);
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_BE_ALL_INFO_OUTPUTTED_STRING, buf, sizeof(buf));
	ret_code = RegSetValueEx(key_handler, buf, NULL, REG_DWORD, (BYTE*)&env_ptr->be_all_info_outputted_, 
		sizeof(env_ptr->be_all_info_outputted_));
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	memset(buf, 0x0, sizeof(buf));
	get_string(IDS_IGNORED_PORT_STRING, buf, sizeof(buf));
	ret_code = RegSetValueEx(key_handler, buf, NULL, REG_DWORD, (BYTE*)&env_ptr->ignored_port_, 
		sizeof(env_ptr->ignored_port_));
	if(ret_code != ERROR_SUCCESS) return GetLastError();

	RegFlushKey(key_handler);

	return ERROR_SUCCESS;
}

void close_registry_private(HKEY key_handler)
{
	RegCloseKey(key_handler);
}

int read_registry(APPLICATION_ENVIRONMENT_PTR env_ptr)
{
	HKEY reg_key = NULL;
	DWORD disposition = NULL;

	if(!open_registry_private(&reg_key, &disposition)) return GetLastError();

	if(disposition == REG_CREATED_NEW_KEY){

		get_uint_value(IDS_LISTEN_PORT, &__application_environment.listen_port_);
		get_uint_value(IDS_INPUT_BUFFERS_NUM, &__application_environment.parallel_port_data_input_buffer_num_);
		get_uint_value(IDS_INPUT_BUFFER_SIZE, &__application_environment.parallel_port_data_input_buffer_size_);
		get_uint_value(IDS_INPUT_WAITING_TIME, &__application_environment.parallel_port_input_waiting_time_);

		get_uint_value(IDS_OUTPUT_BUFFER_SIZE, &__application_environment.parallel_port_output_buffer_size_);

		get_uint_value(IDS_RECV_BUFFER_SIZE, &__application_environment.recv_buff_size_);
		get_uint_value(IDS_RECV_WAITING_TIME, &__application_environment.recv_thread_waiting_time_);
		get_uint_value(IDS_REMOTE_PORT, &__application_environment.remote_port_);
		get_uint_value(IDS_SEND_BUFFER_SIZE, &__application_environment.send_buff_size_);
		get_string(IDS_REMOTE_IP, __application_environment.remote_ip_, sizeof(__application_environment.remote_ip_));
		get_string(IDS_LOG_IP, __application_environment.log_ip_, sizeof(__application_environment.log_ip_));
		get_uint_value(IDS_LOG_PORT, &__application_environment.log_port_);
		get_uint_value(IDS_DISABLE_TO_CHECK_CRC, &__application_environment.enable_to_check_crc_);
		get_uint_value(IDS_DISABLE_TO_TRANSFER_DATA, &__application_environment.enable_to_transfer_data_);

		get_uint_value(IDS_IGNORED_PORT, &__application_environment.ignored_port_);
		get_uint_value(IDS_BE_ALL_INFO_OUTPUTTED, &__application_environment.be_all_info_outputted_);

		write_registry_private(env_ptr, reg_key);
	}
	else if(disposition == REG_OPENED_EXISTING_KEY) read_registry_private(env_ptr, reg_key);

	close_registry_private(reg_key);

	return 0;
}

int write_registry(APPLICATION_ENVIRONMENT_PTR env_ptr)
{
	HKEY reg_key = NULL;
	DWORD disposition = NULL;

	if(!open_registry_private(&reg_key, &disposition)) return GetLastError();

	write_registry_private(env_ptr, reg_key);
	close_registry_private(reg_key);

	return 0;
}




