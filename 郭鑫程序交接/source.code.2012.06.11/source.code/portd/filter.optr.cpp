#include "../common/config.h"
#include "../common/crc16.h"
#include "pcid.h"

#include <hash_map>
#include "filter.optr.h"

enum __enuFILTER_TYPE
{
	FILTER_NULL	=	0x00,
	FILTER_IP	=	0X01,
	FILTER_PORT	=	0X02,
};

typedef struct __tagFILTER_DATA
{
	unsigned char data_[256];
}FILTER_DATA, *FILTER_DATA_PTR;

typedef std::pair<ULONGLONG, FILTER_DATA> data_Pair;
typedef stdext::hash_map<ULONGLONG, FILTER_DATA> filter_data_map;

//	long		long
//	port		 ip
filter_data_map __filter_data;
unsigned int __filter_type = FILTER_NULL;


#ifdef __DEBUG_IT__
ULARGE_INTEGER __debug_key[MAX_STRING_SIZE * 2] = {0};
int __debug_key_size = 0x0;

void test_filter_driver(APPLICATION_ENVIRONMENT_PTR env_ptr)
{

	init_filter(env_ptr);

	for(int i = 0; i < __debug_key_size; ++i)		
		if(!filter_it(__debug_key[i].LowPart, __debug_key[i].HighPart))
			OUTPUT_DEBUG_STRING("Have a error, testing filter map!");		

}

#endif 

bool filter_it(
#ifdef __DEBUG_IT__
				unsigned long ip, unsigned short port
#else
				const unsigned char* buf, unsigned buf_size
#endif
			   )
{
	bool is_ok = false;
	filter_data_map::iterator itr;
	ULARGE_INTEGER value64 = {0};

	buf_size = buf_size;

#ifdef __DEBUG_IT__
	value64.LowPart = ip;
	value64.HighPart = port;
#else
	if(__filter_type == FILTER_NULL) return is_ok = true;

	if((__filter_type & FILTER_IP) == FILTER_IP)
		memcpy(&value64.LowPart, buf + DEST_IP_BEGIN_POS, sizeof(unsigned long));
	else
		value64.LowPart = 0x0;

	if((__filter_type & FILTER_PORT) == FILTER_PORT)
		memcpy(&value64.HighPart, buf + DEST_PORT_BEGIN_POS, sizeof(unsigned short));
	else
		value64.HighPart = 0x0;

#endif //__DEBUG_IT__

	itr = __filter_data.find(value64.QuadPart);
	if(itr != __filter_data.end())	is_ok = true;

	return is_ok;
}

bool init_filter(APPLICATION_ENVIRONMENT_PTR env_ptr)
{
	unsigned long *port_ptr = env_ptr->port_filter_;
	unsigned long *ip_ptr = env_ptr->ip_filter_;
	data_Pair d_pair;
	ULARGE_INTEGER value64 = {0};

	memset(&d_pair.second, 0x0, sizeof(d_pair.second));

	if(__filter_data.size() > 0) __filter_data.clear();

	if(!port_ptr && !ip_ptr){

		read_filter(env_ptr);

		port_ptr = env_ptr->port_filter_;
		ip_ptr = env_ptr->ip_filter_;
	}

	if(ip_ptr && port_ptr){

		__filter_type = FILTER_IP | FILTER_PORT;
		
		while(*ip_ptr){

			port_ptr = env_ptr->port_filter_;
			while(*port_ptr){

				value64.LowPart = *ip_ptr;
				value64.HighPart = *port_ptr;
#ifdef __DEBUG_IT__
				__debug_key[__debug_key_size++].QuadPart = value64.QuadPart;
#endif
				d_pair.first = value64.QuadPart;
				__filter_data.insert(d_pair);				
				++port_ptr;
			}
			
			++ip_ptr;
		}

	}
	else if(ip_ptr && !port_ptr){

		__filter_type = FILTER_IP;

		while(*ip_ptr){

			value64.LowPart = *ip_ptr;
			value64.HighPart = 0x0;
			d_pair.first = value64.QuadPart;
#ifdef __DEBUG_IT__
			__debug_key[__debug_key_size++].QuadPart = value64.QuadPart;
#endif
			__filter_data.insert(d_pair);
			++ip_ptr;
		}
		
	}
	else if(!ip_ptr && port_ptr){

		__filter_type = FILTER_PORT;

		while(*port_ptr){

			value64.LowPart = 0x0;
			value64.HighPart = *port_ptr;
			d_pair.first = value64.QuadPart;
#ifdef __DEBUG_IT__
			__debug_key[__debug_key_size++].QuadPart = value64.QuadPart;
#endif
			__filter_data.insert(d_pair);				
			++port_ptr;
		}
	}
	else{

		__filter_type = FILTER_NULL;
	}

	return true;
}

bool check_crc(const unsigned char* buf, unsigned buf_size, unsigned short crc16)
{
	bool is_ok = true;
	unsigned short result = 0x0;
	
	result = get_crc_16_private(buf, buf_size);
	if(result != crc16) is_ok = false;
	return is_ok;
}

unsigned get_line(FILE* file_ptr, char* buf, int buf_size)
{
	if(buf_size == 0) return 0;

	char* p = buf;
	while(--buf_size && (fread(p, sizeof(char), 1, file_ptr) > 0)){
		if(*p++ == '\n'){

			*--p = 0x0;
			break;
		}
	}

	return p - buf;
}

bool read_filter(APPLICATION_ENVIRONMENT_PTR env_ptr)
{
	char tmp_str[MAX_STRING_SIZE] = {0};
	char ip_token[17] = {0};
	char port_token[17] = {0};
	char* c_ptr = NULL;
	int token = 0; // 1 : ip, 2: port
	int index = 0;
	unsigned long* tmp = NULL;
	int len = MAX_STRING_SIZE;
	errno_t err = NULL;
	FILE* file_ptr = NULL;

	len = get_string(IDS_IP_PORT_FILTER, tmp_str, sizeof(tmp_str));
	tmp_str[len] = 0x0;

	len = get_string(IDS_STRING148, ip_token, sizeof(ip_token));
	ip_token[len] = 0x0;

	len = get_string(IDS_STRING149, port_token, sizeof(port_token));
	port_token[len] = 0x0;

	err = fopen_s(&file_ptr, tmp_str, "rt");
	if(err != 0){

		OUTPUT_DEBUG_STRING(IDS_STRING150, err);
		if(env_ptr->ip_filter_){

			free(env_ptr->ip_filter_);
			env_ptr->ip_filter_ = NULL;
		}

		if(env_ptr->port_filter_){

			free(env_ptr->port_filter_);
			env_ptr->port_filter_ = NULL;
		}
	}

	while(!feof(file_ptr)){
		
		memset(tmp_str, 0x0, sizeof(tmp_str));
		get_line(file_ptr, tmp_str, sizeof(tmp_str));
		switch(token){
			case 0:
TOKEN:
				index = 0;
				len = MAX_STRING_SIZE;
				if(_stricmp(tmp_str, ip_token) == 0){			

					if(env_ptr->ip_filter_){

						free(env_ptr->ip_filter_);
						env_ptr->ip_filter_ = NULL;
					}

					env_ptr->ip_filter_ = (unsigned long*)malloc(sizeof(unsigned long) * len);
					memset(env_ptr->ip_filter_, 0x0, len * sizeof(unsigned long));
					token = 1;
				}
				else if(_stricmp(tmp_str, port_token) == 0){
					token = 2;
					if(env_ptr->port_filter_){

						free(env_ptr->port_filter_);
						env_ptr->port_filter_ = NULL;
					}

					env_ptr->port_filter_ = (unsigned long*)malloc(sizeof(unsigned long) * len);
					memset(env_ptr->port_filter_, 0x0, len * sizeof(unsigned long));

				}
				else{
					token = 0;
				}
				break;
			case 1:
				if(index == len){

					len *= 2;
					tmp = (unsigned long*)malloc(sizeof(unsigned long) * len);
					memset(tmp, 0x0, len * sizeof(unsigned long));
					memcpy(tmp, env_ptr->ip_filter_, index * sizeof(unsigned long));
					free(env_ptr->ip_filter_);
					env_ptr->ip_filter_ = tmp;
				}

				env_ptr->ip_filter_[index] = inet_addr(tmp_str);
				if(env_ptr->ip_filter_[index] == INADDR_NONE){

					token = 0;
					env_ptr->ip_filter_[index] = 0x0;
					goto TOKEN;
				}
				else
					++index;

				break;
			case 2:
				if(index == len){

					len *= 2;
					tmp = (unsigned long*)malloc(sizeof(unsigned long) * len);
					memset(tmp, 0x0, len * sizeof(unsigned long));
					memcpy(tmp, env_ptr->port_filter_, index * sizeof(unsigned long));
					free(env_ptr->port_filter_);
					env_ptr->port_filter_ = tmp;
				}

				env_ptr->port_filter_[index] = strtoul(tmp_str, &c_ptr, 10);
				if(env_ptr->port_filter_[index] > 0)	++index;

				break;
			default:
				break;
		}		
	}

	fclose(file_ptr);

	//check if ip_filter || port_filter is NULL
	if(env_ptr->ip_filter_ && env_ptr->ip_filter_[0] == 0){

		free(env_ptr->ip_filter_);
		env_ptr->ip_filter_ = NULL;
	}

	if(env_ptr->port_filter_ && env_ptr->port_filter_[0] == 0){

		free(env_ptr->port_filter_);
		env_ptr->port_filter_ = NULL;
	}


	return true;
}

bool write_filter(APPLICATION_ENVIRONMENT_PTR env_ptr)
{
	char tmp_str[MAX_STRING_SIZE] = {0};
	char ip_token[17] = {0};
	char port_token[17] = {0};
	int index = 0, len = 0;
	FILE* file_ptr = NULL; 
	errno_t err = NULL;

	len = get_string(IDS_IP_PORT_FILTER, tmp_str, sizeof(tmp_str));
	tmp_str[len] = 0x0;

	len = get_string(IDS_STRING148, ip_token, sizeof(ip_token));
	ip_token[len] = 0x0;

	len = get_string(IDS_STRING149, port_token, sizeof(port_token));
	port_token[len] = 0x0;

	err = fopen_s(&file_ptr, tmp_str, "w");
	if(err != 0){

		OUTPUT_DEBUG_STRING(IDS_STRING153, err);
		return false;
	}

	fprintf_s(file_ptr, "%s\n", ip_token);
	if(env_ptr->ip_filter_){

		index = 0;
		while(env_ptr->ip_filter_[index]){
			
			fprintf_s(file_ptr, "%s\n", inet_ntoa(*(IN_ADDR*)&(env_ptr->ip_filter_[index])));
			++index;
		}
	}

	fprintf_s(file_ptr, "%s\n", port_token);
	if(env_ptr->port_filter_){

		index = 0;
		while(env_ptr->port_filter_[index]){
			
			fprintf_s(file_ptr, "%d\n", env_ptr->port_filter_[index]);
			++index;
		}
	}

	fflush(file_ptr);
	fclose(file_ptr);

	return true;
}




