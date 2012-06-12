/*
*	描述：该文件主要提供CRC16的校验码生成和检验功能的导出函数说明文件
*	文件：crc16.h
*	注释：具体实现请参考CRC16.c	 
*	参考文献：http://www.lammertbies.nl/comm/info/crc-calculation.html
*/
#ifndef __CRC_16_H__
#define __CRC_16_H__

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */

/*
函数名称：unsigned short get_crc_16_private(const char* buf, int buf_size)
功能描述：通过输入指定长度的字符串生成CRC校验码
返回值：CRC16编码
参数：
	buf		cnost char*			传入字符串指针
	buf_size	int				字符串长度
注：
*/
unsigned short get_crc_16_private(const unsigned char* buf, int buf_size);


#ifdef __cplusplus
	}
#endif /* __cplusplus */


#endif //__CRC_16_H__