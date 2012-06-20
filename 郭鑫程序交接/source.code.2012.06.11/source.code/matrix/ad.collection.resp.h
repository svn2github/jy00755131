#ifndef __MATRIX_AD_COLLECTION_RESP_H__
#define __MATRIX_AD_COLLECTION_RESP_H__

enum __enuAD_COLLECTION_RESP_ERROR
{
	ALLOC_MEMORY_ERROR = 0x7001,
	CREATE_AD_DATA_FILE_ERROR,
	CREATE_AD_DATA_ERROR_FILE_ERROR,
	AD_DATA_FORMAT_ERROR,
	AD_DATA_RECV_SIZE_ERROR,
	ALLOCATE_MEMORY_SIZE_TOO_SMALL,
	NOT_FIND_IP_ADDR,
	NOT_ENOUGH_AD_COMMAND_BUFFER,
};

bool init_ad_collection_resp(unsigned int pipe_size/*当前需要使用等待的端口数量*/, unsigned short base_port/*端口基数以此上进行增加*/);
void release_ad_collection_resp();
void proc_ad_collection_resp();

#endif//__MATRIX_AD_COLLECTION_RESP_H__