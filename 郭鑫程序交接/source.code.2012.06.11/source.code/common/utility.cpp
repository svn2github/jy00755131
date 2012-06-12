/*@
*	描述：该文件为公用工具函数实现文件
*	文件：utility.cpp
*	注释：公用工具函数
*	参考文献：
*	版本：1.1.2011.06.22
*	注释：
*	修改：
*
*		1.1.2011.06.22	加入interlock_add函数实现add原子操作
*	
*/

#include "config.h"
#include "utility.h"
#include <crtdbg.h>

/*
void OUTPUT_DEBUG_STRING(log* lg, const char* info) 
{
	lg->output_log(info); 
};

void OUTPUT_DEBUG_STRING(HINSTANCE inst, log* lg, unsigned int id, ...)
{
	int size = 0;
	TCHAR buf[MAX_STRING_SIZE] = {0};
	TCHAR format[MAX_STRING_SIZE] = {0};
	va_list ap;

	size = get_string(inst, id, format, sizeof(format));
	format[size] = 0x0;

	va_start(ap, id);
	_vstprintf_s(buf, format, ap);
	va_end(ap);

	lg->output_log(buf);
}
*/
int get_string(HINSTANCE inst, UINT id, LPTSTR buf, unsigned size)
{
	return LoadString(inst, id, buf, size);
}

bool get_uint_value(HINSTANCE inst, UINT id, unsigned* result)
{	
	bool is_ok = true;
	int len = 0;
	TCHAR buf[MAX_STRING_SIZE] = {0};
	TCHAR* stop_buf_ptr = NULL;

	len = LoadString(inst, id, buf, sizeof(buf)/sizeof(buf[0]));
	if(len == 0){
		*result = 0x0;
		return is_ok = false;
	}
	else {
		buf[len] = 0x0;
#if defined(_UNICODE) || defined(UNICODE)
		*result = wcstoul(buf, &stop_buf_ptr, 10);	
#else
		*result = strtoul(buf, &stop_buf_ptr, 10);
#endif /*defined(_UNICODE) || defined(UNICODE)*/
	}

	return is_ok;
}

bool get_ushort_value(HINSTANCE inst, UINT id, unsigned short* result)
{
	unsigned tmp_result = 0x0;
	bool is_ok = get_uint_value(inst, id, &tmp_result);

	if(is_ok) *result = static_cast<unsigned short>(tmp_result);

	return is_ok;
}

int show_message_box(HINSTANCE inst, HWND window_handle, UINT text_id,UINT type)
{
	TCHAR buf[MAX_STRING_SIZE] = {0};

	get_string(inst, text_id, buf, sizeof(buf)/sizeof(buf[0]));

	return MessageBox(window_handle, buf, __TEXT(""), type);
}

void convert_ushort_littlend_to_bigend(unsigned short const* const littlend, unsigned short* bigend)
{
	((unsigned char*)bigend)[0] = ((unsigned char*)littlend)[1];
	((unsigned char*)bigend)[1] = ((unsigned char*)littlend)[0];
}

void convert_ushort_bigend_to_littlend(unsigned short const* const bigend, unsigned short* littlend)
{
	((unsigned char*)littlend)[0] = ((unsigned char*)bigend)[1];
	((unsigned char*)littlend)[1] = ((unsigned char*)bigend)[0];
}

void convert_ulong_littlend_to_bigend(unsigned long const* const littlend, unsigned long* bigend)
{
	((unsigned char*)bigend)[0] = ((unsigned char*)littlend)[3];
	((unsigned char*)bigend)[1] = ((unsigned char*)littlend)[2];
	((unsigned char*)bigend)[2] = ((unsigned char*)littlend)[1];
	((unsigned char*)bigend)[3] = ((unsigned char*)littlend)[0];
}

void convert_ulong_bigend_to_littlend(unsigned long const* const bigend, unsigned long* littlend)
{
	((unsigned char*)littlend)[0] = ((unsigned char*)bigend)[3];
	((unsigned char*)littlend)[1] = ((unsigned char*)bigend)[2];
	((unsigned char*)littlend)[2] = ((unsigned char*)bigend)[1];
	((unsigned char*)littlend)[3] = ((unsigned char*)bigend)[0];
}

void convert_float_littlend_to_bigend(float const* const littlend, float* bigend)
{
	((unsigned char*)bigend)[0] = ((unsigned char*)littlend)[3];
	((unsigned char*)bigend)[1] = ((unsigned char*)littlend)[2];
	((unsigned char*)bigend)[2] = ((unsigned char*)littlend)[1];
	((unsigned char*)bigend)[3] = ((unsigned char*)littlend)[0];
}

void convert_float_bigend_to_littlend(float const* const bigend, float* littlend)
{
	((unsigned char*)littlend)[0] = ((unsigned char*)bigend)[3];
	((unsigned char*)littlend)[1] = ((unsigned char*)bigend)[2];
	((unsigned char*)littlend)[2] = ((unsigned char*)bigend)[1];
	((unsigned char*)littlend)[3] = ((unsigned char*)bigend)[0];
}

void convert_double_littlend_to_bigend(double const* const littlend, double* bigend)
{
	((unsigned char*)bigend)[0] = ((unsigned char*)littlend)[7];
	((unsigned char*)bigend)[1] = ((unsigned char*)littlend)[6];
	((unsigned char*)bigend)[2] = ((unsigned char*)littlend)[5];
	((unsigned char*)bigend)[3] = ((unsigned char*)littlend)[4];
	((unsigned char*)bigend)[4] = ((unsigned char*)littlend)[3];
	((unsigned char*)bigend)[5] = ((unsigned char*)littlend)[2];
	((unsigned char*)bigend)[6] = ((unsigned char*)littlend)[1];
	((unsigned char*)bigend)[7] = ((unsigned char*)littlend)[0];
}

void convert_double_bigend_to_littlend(double const* const bigend, double* littlend)
{
	((unsigned char*)littlend)[0] = ((unsigned char*)bigend)[7];
	((unsigned char*)littlend)[1] = ((unsigned char*)bigend)[6];
	((unsigned char*)littlend)[2] = ((unsigned char*)bigend)[5];
	((unsigned char*)littlend)[3] = ((unsigned char*)bigend)[4];
	((unsigned char*)littlend)[4] = ((unsigned char*)bigend)[3];
	((unsigned char*)littlend)[5] = ((unsigned char*)bigend)[2];
	((unsigned char*)littlend)[6] = ((unsigned char*)bigend)[1];
	((unsigned char*)littlend)[7] = ((unsigned char*)bigend)[0];
}

void convert_uint_littlend_to_bigend(unsigned int const* const littlend, unsigned int* bigend)
{
	((unsigned char*)bigend)[0] = ((unsigned char*)littlend)[3];
	((unsigned char*)bigend)[1] = ((unsigned char*)littlend)[2];
	((unsigned char*)bigend)[2] = ((unsigned char*)littlend)[1];
	((unsigned char*)bigend)[3] = ((unsigned char*)littlend)[0];
}

void convert_uint_bigend_to_littlend(unsigned int const* const bigend, unsigned int* littlend)
{
	((unsigned char*)littlend)[0] = ((unsigned char*)bigend)[3];
	((unsigned char*)littlend)[1] = ((unsigned char*)bigend)[2];
	((unsigned char*)littlend)[2] = ((unsigned char*)bigend)[1];
	((unsigned char*)littlend)[3] = ((unsigned char*)bigend)[0];
}

void convert_uchar_array_to_ushort_littlend(unsigned char const* const uchar_ary, unsigned short* littlend)
{
	((unsigned char*)littlend)[1] = uchar_ary[0];
	((unsigned char*)littlend)[0] = uchar_ary[1];
}

void convert_ushort_littlend_to_uchar_array(unsigned short littlend, unsigned char* uchar_ary)
{
	uchar_ary[0] = ((unsigned char*)&littlend)[1];
	uchar_ary[1] = ((unsigned char*)&littlend)[0];
}

void convert_uchar_array_to_uint_littlend(unsigned char const* const uchar_ary, unsigned int* littlend, unsigned int uchar_ary_size)
{
	if(uchar_ary_size == 4){
		((unsigned char*)littlend)[0] = uchar_ary[3];
		((unsigned char*)littlend)[1] = uchar_ary[2];
		((unsigned char*)littlend)[2] = uchar_ary[1];
		((unsigned char*)littlend)[3] = uchar_ary[0];
	}
	else if(uchar_ary_size == 3){
		((unsigned char*)littlend)[3] = 0x0;
		((unsigned char*)littlend)[0] = uchar_ary[2];
		((unsigned char*)littlend)[1] = uchar_ary[1];
		((unsigned char*)littlend)[2] = uchar_ary[0];
	}
	else if(uchar_ary_size == 2){

		((unsigned char*)littlend)[3] = 0x0;
		((unsigned char*)littlend)[0] = uchar_ary[1];
		((unsigned char*)littlend)[1] = uchar_ary[0];
		((unsigned char*)littlend)[2] = 0x0;
	}
	else if(uchar_ary_size == 1){
		((unsigned char*)littlend)[1] = 0x0;
		((unsigned char*)littlend)[0] = uchar_ary[0];
		((unsigned char*)littlend)[2] = 0x0;
		((unsigned char*)littlend)[3] = 0x0;	
	}
}

void convert_uint_littlend_to_uchar_array(unsigned int littlend, unsigned char* uchar_ary, unsigned int uchar_ary_size)
{
	if(uchar_ary_size == 4){
		uchar_ary[0] = ((unsigned char*)&littlend)[3];
		uchar_ary[1] = ((unsigned char*)&littlend)[2];
		uchar_ary[2] = ((unsigned char*)&littlend)[1];
		uchar_ary[3] = ((unsigned char*)&littlend)[0];
	}
	else if(uchar_ary_size == 3){
		uchar_ary[0] = ((unsigned char*)&littlend)[2];
		uchar_ary[1] = ((unsigned char*)&littlend)[1];
		uchar_ary[2] = ((unsigned char*)&littlend)[0];
	}
	else if(uchar_ary_size == 2){
		uchar_ary[0] = ((unsigned char*)&littlend)[1];
		uchar_ary[1] = ((unsigned char*)&littlend)[0];
	}
	else if(uchar_ary_size == 1){
		uchar_ary[0] = ((unsigned char*)&littlend)[0];
	}
}

void convert_uchar_array_to_float_littlend(__in__ unsigned char const* const uchar_ary, __out__ float* littlend, __in__ unsigned int uchar_ary_size)
{
	if(uchar_ary_size == 4){
		((unsigned char*)littlend)[0] = uchar_ary[3];
		((unsigned char*)littlend)[1] = uchar_ary[2];
		((unsigned char*)littlend)[2] = uchar_ary[1];
		((unsigned char*)littlend)[3] = uchar_ary[0];
	}
	else if(uchar_ary_size == 3){
		((unsigned char*)littlend)[3] = 0x0;
		((unsigned char*)littlend)[0] = uchar_ary[2];
		((unsigned char*)littlend)[1] = uchar_ary[1];
		((unsigned char*)littlend)[2] = uchar_ary[0];
	}
	else if(uchar_ary_size == 2){

		((unsigned char*)littlend)[3] = 0x0;
		((unsigned char*)littlend)[0] = uchar_ary[1];
		((unsigned char*)littlend)[1] = uchar_ary[0];
		((unsigned char*)littlend)[2] = 0x0;
	}
	else if(uchar_ary_size == 1){
		((unsigned char*)littlend)[1] = 0x0;
		((unsigned char*)littlend)[0] = uchar_ary[0];
		((unsigned char*)littlend)[2] = 0x0;
		((unsigned char*)littlend)[3] = 0x0;	
	}

}

void convert_float_littlend_to_uchar_array(__in__ float littlend, __out__ unsigned char* uchar_ary, __in__ unsigned int uchar_ary_size)
{
	if(uchar_ary_size == 4){
		uchar_ary[0] = ((unsigned char*)&littlend)[3];
		uchar_ary[1] = ((unsigned char*)&littlend)[2];
		uchar_ary[2] = ((unsigned char*)&littlend)[1];
		uchar_ary[3] = ((unsigned char*)&littlend)[0];
	}
	else if(uchar_ary_size == 3){
		uchar_ary[0] = ((unsigned char*)&littlend)[2];
		uchar_ary[1] = ((unsigned char*)&littlend)[1];
		uchar_ary[2] = ((unsigned char*)&littlend)[0];
	}
	else if(uchar_ary_size == 2){
		uchar_ary[0] = ((unsigned char*)&littlend)[1];
		uchar_ary[1] = ((unsigned char*)&littlend)[0];
	}
	else if(uchar_ary_size == 1){
		uchar_ary[0] = ((unsigned char*)&littlend)[0];
	}
}

void convert_uchar_array_to_double_littlend(unsigned char const* const uchar_ary, double* littlend)
{
		((unsigned char*)littlend)[0] = uchar_ary[7];
		((unsigned char*)littlend)[1] = uchar_ary[6];
		((unsigned char*)littlend)[2] = uchar_ary[5];
		((unsigned char*)littlend)[3] = uchar_ary[4];
		((unsigned char*)littlend)[4] = uchar_ary[3];
		((unsigned char*)littlend)[5] = uchar_ary[2];
		((unsigned char*)littlend)[6] = uchar_ary[1];
		((unsigned char*)littlend)[7] = uchar_ary[0];
}
void convert_double_littlend_to_uchar_array(double littlend, unsigned char* uchar_ary)
{
		uchar_ary[0] = ((unsigned char*)&littlend)[0];
		uchar_ary[1] = ((unsigned char*)&littlend)[1];
		uchar_ary[2] = ((unsigned char*)&littlend)[2];
		uchar_ary[3] = ((unsigned char*)&littlend)[3];
		uchar_ary[4] = ((unsigned char*)&littlend)[4];
		uchar_ary[5] = ((unsigned char*)&littlend)[5];
		uchar_ary[6] = ((unsigned char*)&littlend)[6];
		uchar_ary[7] = ((unsigned char*)&littlend)[7];
}

void convert_uchar_array_to_ulonglong_littlend(__in__ unsigned char const* const uchar_ary, __out__ ULONGLONG* littlend, __in__ unsigned int uchar_ary_size)
{
	if(uchar_ary_size == 8){
		((unsigned char*)littlend)[0] = uchar_ary[7];
		((unsigned char*)littlend)[1] = uchar_ary[6];
		((unsigned char*)littlend)[2] = uchar_ary[5];
		((unsigned char*)littlend)[3] = uchar_ary[4];
		((unsigned char*)littlend)[4] = uchar_ary[3];
		((unsigned char*)littlend)[5] = uchar_ary[2];
		((unsigned char*)littlend)[6] = uchar_ary[1];
		((unsigned char*)littlend)[7] = uchar_ary[0];
	}
	else if(uchar_ary_size == 7){
		((unsigned char*)littlend)[0] = uchar_ary[6];
		((unsigned char*)littlend)[1] = uchar_ary[5];
		((unsigned char*)littlend)[2] = uchar_ary[4];
		((unsigned char*)littlend)[3] = uchar_ary[3];
		((unsigned char*)littlend)[4] = uchar_ary[2];
		((unsigned char*)littlend)[5] = uchar_ary[1];
		((unsigned char*)littlend)[6] = uchar_ary[0];
		((unsigned char*)littlend)[7] = 0x0;
	}
	else if(uchar_ary_size == 6){
		((unsigned char*)littlend)[0] = uchar_ary[5];
		((unsigned char*)littlend)[1] = uchar_ary[4];
		((unsigned char*)littlend)[2] = uchar_ary[3];
		((unsigned char*)littlend)[3] = uchar_ary[2];
		((unsigned char*)littlend)[4] = uchar_ary[1];
		((unsigned char*)littlend)[5] = uchar_ary[0];
		((unsigned char*)littlend)[6] = 0x0;
		((unsigned char*)littlend)[7] = 0x0;
	}
	else if(uchar_ary_size == 5){
		((unsigned char*)littlend)[0] = uchar_ary[4];
		((unsigned char*)littlend)[1] = uchar_ary[3];
		((unsigned char*)littlend)[2] = uchar_ary[2];
		((unsigned char*)littlend)[3] = uchar_ary[1];
		((unsigned char*)littlend)[4] = uchar_ary[0];
		((unsigned char*)littlend)[5] = 0x0;
		((unsigned char*)littlend)[6] = 0x0;
		((unsigned char*)littlend)[7] = 0x0;
	}
	else if(uchar_ary_size == 4){
		((unsigned char*)littlend)[0] = uchar_ary[3];
		((unsigned char*)littlend)[1] = uchar_ary[2];
		((unsigned char*)littlend)[2] = uchar_ary[1];
		((unsigned char*)littlend)[3] = uchar_ary[0];
		((unsigned char*)littlend)[4] = 0x0;
		((unsigned char*)littlend)[5] = 0x0;
		((unsigned char*)littlend)[6] = 0x0;
		((unsigned char*)littlend)[7] = 0x0;
	}
	else if(uchar_ary_size == 3){
		((unsigned char*)littlend)[0] = uchar_ary[2];
		((unsigned char*)littlend)[1] = uchar_ary[1];
		((unsigned char*)littlend)[2] = uchar_ary[0];
		((unsigned char*)littlend)[3] = 0x0;
		((unsigned char*)littlend)[4] = 0x0;
		((unsigned char*)littlend)[5] = 0x0;
		((unsigned char*)littlend)[6] = 0x0;
		((unsigned char*)littlend)[7] = 0x0;
	}
	else if(uchar_ary_size == 2){
		((unsigned char*)littlend)[0] = uchar_ary[1];
		((unsigned char*)littlend)[1] = uchar_ary[0];
		((unsigned char*)littlend)[2] = 0x0;
		((unsigned char*)littlend)[3] = 0x0;
		((unsigned char*)littlend)[4] = 0x0;
		((unsigned char*)littlend)[5] = 0x0;
		((unsigned char*)littlend)[6] = 0x0;
		((unsigned char*)littlend)[7] = 0x0;
	}
	else if(uchar_ary_size == 1){

		((unsigned char*)littlend)[0] = uchar_ary[0];
		((unsigned char*)littlend)[1] = 0x0;
		((unsigned char*)littlend)[2] = 0x0;
		((unsigned char*)littlend)[3] = 0x0;
		((unsigned char*)littlend)[4] = 0x0;
		((unsigned char*)littlend)[5] = 0x0;
		((unsigned char*)littlend)[6] = 0x0;
		((unsigned char*)littlend)[7] = 0x0;
	}
}

void convert_ulonglong_littlend_to_uchar_array(__in__ ULONGLONG littlend, __out__ unsigned char* uchar_ary, __in__ unsigned int uchar_ary_size)
{
	if(uchar_ary_size == 8){
		uchar_ary[0] = ((unsigned char*)&littlend)[0];
		uchar_ary[1] = ((unsigned char*)&littlend)[1];
		uchar_ary[2] = ((unsigned char*)&littlend)[2];
		uchar_ary[3] = ((unsigned char*)&littlend)[3];
		uchar_ary[4] = ((unsigned char*)&littlend)[4];
		uchar_ary[5] = ((unsigned char*)&littlend)[5];
		uchar_ary[6] = ((unsigned char*)&littlend)[6];
		uchar_ary[7] = ((unsigned char*)&littlend)[7];
	}
	else if(uchar_ary_size == 7){
		uchar_ary[0] = ((unsigned char*)&littlend)[0];
		uchar_ary[1] = ((unsigned char*)&littlend)[1];
		uchar_ary[2] = ((unsigned char*)&littlend)[2];
		uchar_ary[3] = ((unsigned char*)&littlend)[3];
		uchar_ary[4] = ((unsigned char*)&littlend)[4];
		uchar_ary[5] = ((unsigned char*)&littlend)[5];
		uchar_ary[6] = ((unsigned char*)&littlend)[6];

	}
	else if(uchar_ary_size == 6){
		uchar_ary[0] = ((unsigned char*)&littlend)[0];
		uchar_ary[1] = ((unsigned char*)&littlend)[1];
		uchar_ary[2] = ((unsigned char*)&littlend)[2];
		uchar_ary[3] = ((unsigned char*)&littlend)[3];
		uchar_ary[4] = ((unsigned char*)&littlend)[4];
		uchar_ary[5] = ((unsigned char*)&littlend)[5];
	}
	else if(uchar_ary_size == 5){
		uchar_ary[0] = ((unsigned char*)&littlend)[0];
		uchar_ary[1] = ((unsigned char*)&littlend)[1];
		uchar_ary[2] = ((unsigned char*)&littlend)[2];
		uchar_ary[3] = ((unsigned char*)&littlend)[3];
		uchar_ary[4] = ((unsigned char*)&littlend)[4];
	}
	else if(uchar_ary_size == 4){
		uchar_ary[0] = ((unsigned char*)&littlend)[0];
		uchar_ary[1] = ((unsigned char*)&littlend)[1];
		uchar_ary[2] = ((unsigned char*)&littlend)[2];
		uchar_ary[3] = ((unsigned char*)&littlend)[3];
	}
	else if(uchar_ary_size == 3){
		uchar_ary[0] = ((unsigned char*)&littlend)[0];
		uchar_ary[1] = ((unsigned char*)&littlend)[1];
		uchar_ary[2] = ((unsigned char*)&littlend)[2];
	}
	else if(uchar_ary_size == 2){
		uchar_ary[0] = ((unsigned char*)&littlend)[0];
		uchar_ary[1] = ((unsigned char*)&littlend)[1];
	}
	else if(uchar_ary_size == 1){
		uchar_ary[0] = ((unsigned char*)&littlend)[0];
	}
}

void convert_uchar_to_bcd(__in__ unsigned char uchar_value, __out__ unsigned char* bcd_value)
{
	_ASSERT(bcd_value != NULL && uchar_value <= 99);

	*bcd_value = static_cast<unsigned char>(uchar_value / 10);
	*bcd_value <<= 4;
	*bcd_value |= static_cast<unsigned char>(uchar_value % 10);
}

//void convert_ushort_to_bcd_uchar(__in__ unsigned short ushort_value, __out__ unsigned char* bcd_value)
//{
//	_ASSERT(bcd_value != NULL && ushort_value <= 9999);
//
//	unsigned short tmp = 0x0;
//
//	bcd_value[0] = tmp = static_cast<unsigned short>(ushort_value/1000);
//	bcd_value[0] <<= 4;
//
//	tmp = (ushort_value - tmp * 1000) / 100;
//	bcd_value[0] |= tmp;
//
//	tmp = ushort_value - static_cast<unsigned short>(ushort_value / 100) * 100;
//	bcd_value[1] = (tmp / 10);
//	bcd_value[1] <<= 4;
//	bcd_value[1] |= (ushort_value % 10);
//}
//
void convert_ushort_to_bcd(__in__ unsigned short ushort_value, __out__ unsigned short* bcd_value)
{
	_ASSERT(bcd_value != NULL && ushort_value <= 9999);

	unsigned short tmp = 0x0;

	*bcd_value = tmp = static_cast<unsigned short>(ushort_value/1000);
	*bcd_value <<= 4;

	tmp = (ushort_value - tmp * 1000) / 100;
	*bcd_value |= tmp;
	*bcd_value <<= 4;

	tmp = ushort_value - static_cast<unsigned short>(ushort_value / 100) * 100;
	*bcd_value |= (tmp / 10);
	*bcd_value <<= 4;
	*bcd_value |= (ushort_value % 10);
}

void convert_bcd_to_uchar(__in__ unsigned char bcd_value, __out__ unsigned char* uchar_value)
{
	_ASSERT(uchar_value != NULL && bcd_value <= 0x99);

	*uchar_value = (bcd_value >> 4) * 10;
	*uchar_value += (bcd_value & 0xf);
}

//void convert_bcd_uchar_to_ushort(__in__ unsigned char* bcd_value, __out__ unsigned short* ushort_value)
//{
//	_ASSERT(bcd_value != NULL && ushort_value != NULL && *bcd_value <= 0x9999);
//
//	*ushort_value = static_cast<unsigned char>(bcd_value[0] >> 4) * 1000;
//	*ushort_value += static_cast<unsigned char>(bcd_value[0] & 0xf) * 100;
//
//	*ushort_value += static_cast<unsigned char>(bcd_value[1] >> 4) * 10;
//	*ushort_value += static_cast<unsigned char>(bcd_value[1] & 0xf);
//}

void convert_bcd_to_ushort(__in__ unsigned short bcd_value, __out__ unsigned short* ushort_value)
{
	_ASSERT(ushort_value != NULL && bcd_value <= 0x9999);

	*ushort_value = static_cast<unsigned short>(bcd_value >> 12) * 1000;
	*ushort_value += static_cast<unsigned short>((bcd_value & 0xfff) >> 8) * 100;
	*ushort_value += static_cast<unsigned short>((bcd_value & 0xff) >> 4) * 10;
	*ushort_value += static_cast<unsigned short>(bcd_value & 0xf);
}

LONG interlock_add(__in_out__ LONG volatile* atomic_value, LONG add_value)
{
	LONG result = 0x0;

	while(true){

		result = *atomic_value;
		if(result == _InterlockedCompareExchange(atomic_value, result + add_value, result)) break;
	}

	return result += add_value;
}


