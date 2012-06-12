/*
*	描述：提供公用函数
*	文件：utility.h
*	注释：
*	参考文献：
*	版本：
*	完成人：
*	example:
*/

#ifndef __COMMON_UTILITY_H__
#define __COMMON_UTILITY_H__

#include "log.h" 

//void OUTPUT_DEBUG_STRING(log* lg, const char* info);
//void OUTPUT_DEBUG_STRING(HINSTANCE inst, log* lg, unsigned int id, ...);
int get_string(HINSTANCE inst, UINT id, LPTSTR buf, unsigned size);
bool get_uint_value(HINSTANCE inst, UINT id, unsigned* result);
bool get_ushort_value(HINSTANCE inst, UINT id, unsigned short* result);
int show_message_box(HINSTANCE inst, HWND window_handle, UINT text_id,UINT type);

void convert_ushort_littlend_to_bigend(unsigned short const* const littlend, unsigned short* bigend);
void convert_ushort_bigend_to_littlend(unsigned short const* const bigend, unsigned short* littlend);
void convert_ulong_littlend_to_bigend(unsigned long const* const littlend, unsigned long* bigend);
void convert_ulong_bigend_to_littlend(unsigned long const* const bigend, unsigned long* littlend);
void convert_float_littlend_to_bigend(float const* const littlend, float* bigend);
void convert_float_bigend_to_littlend(float const* const bigend, float* littlend);
void convert_double_littlend_to_bigend(double const* const littlend, double* bigend);
void convert_double_bigend_to_littlend(double const* const bigend, double* littlend);
void convert_uint_littlend_to_bigend(unsigned int const* const littlend, unsigned int* bigend);
void convert_uint_bigend_to_littlend(unsigned int const* const bigend, unsigned int* littlend);

void convert_uchar_array_to_ushort_littlend(unsigned char const* const uchar_ary, unsigned short* littlend);
void convert_ushort_littlend_to_uchar_array(unsigned short littlend, unsigned char* uchar_ary);

void convert_uchar_array_to_uint_littlend(unsigned char const* const uchar_ary, unsigned int* littlend, unsigned int uchar_ary_size);
void convert_uint_littlend_to_uchar_array(unsigned int littlend, unsigned char* uchar_ary, unsigned int uchar_ary_size);

void convert_uchar_array_to_double_littlend(unsigned char const* const uchar_ary, double* littlend);
void convert_double_littlend_to_uchar_array(double littlend, unsigned char* uchar_ary);

void convert_uchar_to_bcd(__in__ unsigned char uchar_value, __out__ unsigned char* bcd_value);//bcd_value'size = 1
void convert_ushort_to_bcd(__in__ unsigned short ushort_value, __out__ unsigned short* bcd_value);

void convert_bcd_to_uchar(__in__ unsigned char bcd_value, __out__ unsigned char* uchar_value);//bcd_value'size = 1
void convert_bcd_to_ushort(__in__ unsigned short bcd_value, __out__ unsigned short* ushort_value);

void convert_uchar_array_to_float_littlend(__in__ unsigned char const* const uchar_ary, __out__ float* littlend, __in__ unsigned int uchar_ary_size);
void convert_float_littlend_to_uchar_array(__in__ float littlend, __out__ unsigned char* uchar_ary, __in__ unsigned int uchar_ary_size);

void convert_uchar_array_to_ulonglong_littlend(__in__ unsigned char const* const uchar_ary, __out__ ULONGLONG* littlend, __in__ unsigned int uchar_ary_size);
void convert_ulonglong_littlend_to_uchar_array(__in__ ULONGLONG littlend, __out__ unsigned char* uchar_ary, __in__ unsigned int uchar_ary_size);

LONG interlock_add(__in_out__ LONG volatile* atomic_value, LONG add_value);

#endif /*__COMMON_UTILITY_H__*/
