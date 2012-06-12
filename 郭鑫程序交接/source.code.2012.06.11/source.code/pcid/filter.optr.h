#ifndef __FILTER_OPTR_H__
#define __FILTER_OPTR_H__

/*setting file for ip & port*/
bool read_filter(APPLICATION_ENVIRONMENT_PTR);
bool write_filter(APPLICATION_ENVIRONMENT_PTR);

bool init_filter(APPLICATION_ENVIRONMENT_PTR);

#ifdef __DEBUG_IT__
void test_filter_driver(APPLICATION_ENVIRONMENT_PTR);
bool filter_it(unsigned long, unsigned short);
#else
bool filter_it(const unsigned char*, unsigned);
#endif //__DEBUG_IT__

bool check_crc(const unsigned char*, unsigned, unsigned short);

#endif //__FILTER_OPTR_H__