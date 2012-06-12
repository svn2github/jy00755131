#include <malloc.h>
#include <memory.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "code.conv.h"

static char BIN_TABLE[16][5] = {
	{"0000"}, //0
	{"0001"}, //1
	{"0010"}, //2
	{"0011"}, //3
	{"0100"}, //4
	{"0101"}, //5
	{"0110"}, //6
	{"0111"}, //7	
	{"1000"}, //8
	{"1001"}, //9
	{"1010"}, //10
	{"1011"}, //11
	{"1100"}, //12
	{"1101"}, //13
	{"1110"}, //14
	{"1111"}, //15
}; 

static char HEX_TABLE[] = {"0123456789ABCDEF"}; 

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
unsigned int ascii_to_bin(const char* ascii_array, int ascii_array_size, char* bin_array)
{
	int i = 0, 
		h = 0, 
		l = 0;
	char* ptr = bin_array;
	if(ascii_array == NULL || ascii_array_size == 0 || ptr == NULL)
		return 0;

	for(i = 0; i < ascii_array_size; ++i){

		h = ((unsigned char)ascii_array[i]) / 16;
		l = ((unsigned char)ascii_array[i]) % 16;
		memcpy(ptr, BIN_TABLE[h], sizeof(char) * 4);
		ptr += 4;
		memcpy(ptr, BIN_TABLE[l], sizeof(char) * 4);
		ptr += 4;
	}

	*ptr = 0;

	return ptr - bin_array;
}


/*
函数名称：unsigned int hex_to_ascii(const char* in_buf, char* out_buf)
功能描述：转化HEX到ASCII
返回值：返回值out_buf长度
参数：
	in_buf	const char*	有效16进制字符串
	out_buf	char*	2进制字符串输出
注：
*/
unsigned int hex_to_ascii(const char* in_buf, char* out_buf)
{
	if(in_buf == NULL || out_buf == NULL) return 0;

	char hex_val = 0,
		*ptr  = NULL,
		*dest = NULL,
		*tmp_buf = NULL;

	unsigned int buf_size = 0;
	int size = strlen(in_buf) + 1;

	tmp_buf = (char*)malloc(size + 2);
	if(tmp_buf == NULL) return 0;

	memset(tmp_buf, 0x0, sizeof(char) * size);
	memcpy(tmp_buf, in_buf, sizeof(char) * size);

    ptr  = tmp_buf;
    dest = tmp_buf;

    while(*ptr) {

        if((*ptr >= '0') && (*ptr <= '9')) *dest++ = (char)((*ptr) - '0');
        if((*ptr >= 'A') && (*ptr <= 'F')) *dest++ = (char)((*ptr) - 'A' + 10);
        if((*ptr >= 'a') && (*ptr <= 'f')) *dest++ = (char)((*ptr) - 'a' + 10);

        ++ptr;
	}

    * dest = '\x80';
    *(dest+1) = '\x80';
 	
    ptr = tmp_buf;

    while( *ptr != '\x80'){

        hex_val  = (char) ((*ptr &  '\x0f') << 4 );
        hex_val |= (char) ((*(ptr+1) & '\x0f'));
		out_buf[buf_size] = hex_val;
        ptr += 2;
		++buf_size;
    }


	free(tmp_buf);
	tmp_buf = NULL;

	return buf_size;
}

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
unsigned bin_to_hex(const char* bin_array, int bin_array_size, char* hex_array)
{
	char *ptr = (char*)bin_array,
		*dest = (char*)hex_array;
	char bin[5] = {0};

	int i = 0, 
		j = 0,
		bin_table_size = sizeof(BIN_TABLE)/sizeof(BIN_TABLE[0]);

	if(bin_array == NULL || bin_array_size == 0)
		return 0;

	while(*ptr){

		if(i > 3){//最多4个字符
			//get a hex 
			for(j = 0; j < bin_table_size; ++j){
				
				if(strcmp(bin, BIN_TABLE[j]) == 0){

					*dest = HEX_TABLE[j];
					if(dest != hex_array){

						if(*(dest - 1) != ' '){
							
							*(++dest) = ' ';
						}
					}					
					++dest;
				}
			}

			i = 0;
			memset(bin, 0x0, sizeof(bin));
		}

		bin[i] = *ptr;
		++i;
		++ptr;
	}

	if(i >= 3){
		//get a hex 
		for(j = 0; j < bin_table_size; ++j){
			
			if(strcmp(bin, BIN_TABLE[j]) == 0){

				*dest = HEX_TABLE[j];
				++dest;
			}
		}
	}

	*dest = NULL;

	return dest - hex_array;
}

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
int hex_to_bin(const char* hex_array, int hex_array_size, char* bin_array)
{
	int i = 0;
	char *ptr = (char*)hex_array,
		*dest = bin_array;

	if(hex_array == NULL || hex_array_size == 0){ 
		return -1;
	}
	else{
		*(ptr + hex_array_size) = NULL;
	}

	while(*ptr){
		
        if((*ptr >= '0') && (*ptr <= '9')){

			i = *ptr - '0';
			memcpy(dest, BIN_TABLE[i], sizeof(char) * 4);
			dest += 4;
		} 
		else if((*ptr >= 'A') && (*ptr <= 'F')){
			
			i = (*ptr - 'A') + 10;

			memcpy(dest, BIN_TABLE[i], sizeof(char) * 4);
			dest += 4;
		}
		else if((*ptr >= 'a') && (*ptr <= 'f')){

			i = (*ptr - 'a') +  + 10;
			memcpy(dest, BIN_TABLE[i], sizeof(char) * 4);
			dest += 4;
		}
 
        ++ptr;
	}

	*dest = NULL;

	return dest - bin_array;
}

/*
函数名称：unsigned short bin_to_unsigned_short_private(const char* bin_array)
功能描述：通过输入CRC校验码的二进制形式
返回值：CRC16编码的unsigned short 形式
参数：
	bin_array	cnost char*	传入需要校验的二进制字符串指针
注：
	这个函数存在很大的限制，只能用于本模块内部使用，不可外部调用
*/
unsigned short bin_to_unsigned_short_private(const char* bin_array)
{
	unsigned short	value = 0, 
					tmp_val = 0;

	unsigned	size = 0,
				i = 0;

	int bin_array_size = 0;
	char hex_array[8] = {0};
	double xx = 16;

	bin_array_size = strlen(bin_array);

	if(bin_array == NULL || bin_array_size == 0)
		return 0;

	size = bin_to_hex(bin_array, bin_array_size, hex_array);
	hex_array[size] = 0;

    for(i = 0; i < size; ++i) {

		if(hex_array[i] == ' ') continue;

        if((hex_array[i] >= '0') && (hex_array[i] <= '9')) 
			tmp_val = (hex_array[i] - '0');
        else if((hex_array[i] >= 'A') && (hex_array[i] <= 'F')) 
			tmp_val = (hex_array[i] - 'A' + 10);
        else if((hex_array[i] >= 'a') && (hex_array[i] <= 'f')) 
			tmp_val = (hex_array[i] - 'a' + 10);

		switch(i){

			case 0:
				value += static_cast<unsigned short>(pow(xx, 3)) * tmp_val; 
				break;
			case 1:
				value += static_cast<unsigned short>(pow(xx, 2)) * tmp_val;
				break;
			case 3:
				value += 16 * tmp_val;
				break;
			case 4:
				value += tmp_val;
				break;
			default:
				break;
		}
	}

	return value;
}

unsigned short ascii_to_hex_value(char* buf, int buf_size)
{
	unsigned short value = 0,
		tmp_val = 0;

	for(int i = 0; i < buf_size; ++i) {

		if(buf[i] == ' ') continue;

		if((buf[i] >= '0') && (buf[i] <= '9')) 
			tmp_val = (buf[i] - '0');
		else if((buf[i] >= 'A') && (buf[i] <= 'F')) 
			tmp_val = (buf[i] - 'A' + 10);
		else if((buf[i] >= 'a') && (buf[i] <= 'f')) 
			tmp_val = (buf[i] - 'a' + 10);

		switch(i){

			case 0:
				value += 16 * tmp_val;
				break;
			case 1:
				value += tmp_val;
				break;
			default:
				break;
		}
	}
	return value;
};

int bin_to_ansi(const unsigned char* binary_buf, int binary_buf_size, char* ansi_buf, int ansi_buf_size)
{
	int i = 0;
	char ch = 0x0;
	char* ptr = ansi_buf;
	int size = 0;

	ansi_buf_size = ansi_buf_size;
	for(; i < binary_buf_size; ++i, ++ptr){

		ch = binary_buf[i] >> 4;
		if(ch >= 0 && ch <= 9)	*ptr = 0x30 + ch;
		else if(ch >= 0xa && ch <= 0xf)	*ptr = 'A' + ch - 0xa;
		else	*ptr = 0x30;

		ch = binary_buf[i] & 0xf;
		if(ch >= 0 && ch <= 9)	*++ptr = 0x30 + ch;
		else if(ch >= 0xa && ch <= 0xf)	*++ptr = 'A' + ch - 0xa;
		else	*++ptr = 0x30;

		*++ptr = ' ';
		size += 3;
	}

	return size;
}

