#ifndef __CODE_CONV_H__
#define __CODE_CONV_H__

/*
函数名称：unsigned int ascii_to_bin(const char* ascii_array, int ascii_array_size, char* bin_array)
功能描述：转化ASCII到BIN
返回值：返回值bin_array长度
参数：
	ascii_array			const char*		ascii字符串
	ascii_array_size	int				ascii字符串长度
	bin_array			char*			二进制字符串
注：
	bin_array必须预先分配足够大
i.e
	int size = 0;
	char ascii[]={"FLAW"};
	char bin[256] = {0};
	
	size = ascii_to_bin(ascii, strlen(ascii), bin);
*/
unsigned int ascii_to_bin(const char* ascii_array, int ascii_array_size, char* bin_array);


/*
函数名称：unsigned int hex_to_ascii(const char* in_buf, char* out_buf)
功能描述：转化HEX到ASCII
返回值：返回值out_buf长度
参数：
	in_buf	const char*	有效16进制字符串
	out_buf	char*	2进制字符串输出
注：
*/
unsigned int hex_to_ascii(const char* in_buf, char* out_buf);


/*
函数名称：int bin_to_hex(const char* bin_array, int bin_array_size, char* hex_array)
功能描述：从2进制字符串转化为16进制
返回值：返回值当前创建的hex_array长度
参数：
	bin_array		const char*		有效2进制字符串
	bin_array_size	int				2进制字符串长度
	hex_array		char*			有效16进制字符串缓冲指针，该字符串必须提前分配足够的内存
注：
i.e:
	int size = 0;
	char bin_array[] = {"1001111101000000"};
	char hex_array[MAX_BUFF] = {0};
	size = bin_to_hex(bin_array, sizeof(bin_array) - 1, hex_array);
	printf("BIN: %s\n", hex_array);

*/
unsigned bin_to_hex(const char* bin_array, int bin_array_size, char* hex_array);

/*
函数名称：int hex_to_bin(const char* hex_array,	int hex_array_size,	char* bin_array)
功能描述：从16进制字符串转化为2进制
返回值：返回值当前创建的bin_array长度
参数：
   hex_array		const char* 有效16进制字符串
   hex_array_size	int			16进制字符串长度
   bin_array		char*		转化成2进制字符串，该字符串必须提前分配足够的内存
注：
i.e:
	int size = 0;
	char hex_array[] = {"9F 40"};
	char bin_array[MAX_BUFF] = {0};
	size = hex_to_bin( hex_array, 5, bin_array);
	printf("BIN: %s\n", bin_array);

*/
int hex_to_bin(const char* hex_array, int hex_array_size, char* bin_array);

/*
函数名称：unsigned short bin_to_unsigned_short_private(const char* bin_array)
功能描述：通过输入CRC校验码的二进制形式
返回值：CRC16编码的unsigned short 形式
参数：
	bin_array	cnost char*	传入需要校验的二进制字符串指针
注：
	这个函数存在很大的限制，只能用于本模块内部使用，不可外部调用
*/
unsigned short bin_to_unsigned_short_private(const char* bin_array);


/*
函数名称：unsigned int bin_to_ansi(const unsigned char* binary_buf, int binary_buf_size, char* ansi_buf, int ansi_buf_size)
功能描述：二进制形式转化到ASCII 16进制格式输出
返回值：返回大小
参数：
	bin_buf		const unsigned char*	传入二进制字符串指针
	ansi_buf	char*					ASCII码缓冲指针
注：
	必须保证足够的ansi_buf缓冲
*/
int bin_to_ansi(const unsigned char* binary_buf, int binary_buf_size, char* ansi_buf, int ansi_buf_size);


#endif /*__CODE_CONV_H__*/
