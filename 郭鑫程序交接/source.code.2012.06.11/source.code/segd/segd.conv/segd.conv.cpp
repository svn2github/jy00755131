/**
@brief	这个程序主要对于segd文件的输出。
@details	
		segd.conv.exe --data-file [args.conf] --verbose --help --version
			--data-file		传入配置文件名，要求带路径
			--verbose	输出调试信息
			--help		显示帮助信息
			--version	显示版本信息
@file	segd.conv.cpp
@note	该程序将被其他程序进行调用，并且在使用时，它将导出
@see	需要参考SPS文件格式和Sercel 428XL用户手册第二册V2.0中的SEG-D文件格式
@version	1.1.34.87.2011.11.04
@author	kuoxin	
@bug
	1.1.1.5.2011.08.31		完成基本功能
	1.1.22.63.2011.10.18	修改程序中有关错误输出问题
	1.1.23.66.2011.10.18	修改程序中的memory leak, data races等问题(当前使用Intel Inspector 2011XE)
	1.1.30.80.2011.10.26	将segd.api.dll工作移入程序中,将不再使用该dll,程序性能提高150-200ms
							修改程序中写入SEGD辅助道数据是少写了12888个字节错误,当前程序使用了overlapped I/O方式,
							写入SEGD数据方式,并没有配合FILE_FLAG_NO_BUFFERING的使用,因此性能较使用简单写入
							(不使用FlushFileBuffers, 直接由OS来提供写入方式)还要慢(830K数据,要慢3倍,该测试
							在intel Amplifier XE 2011测试得到)
*/


#include "segd.conv.h"

static const char_t __app_version[] = __TEXT("Rev: 1.2.46.125.2012.04.26");

int wmain( int argc, wchar_t *argv[ ]){	return main_private(argc, argv, __app_version);}

