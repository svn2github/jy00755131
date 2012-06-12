/*@
*	描述：该文件为测线命令行操作类的实现文件
*	继承：CCommandLineInfo
*	文件：line.cmd.line.info.cpp
*	注释：
*	参考文献：
*	版本：1.0.0.0
*	注释：
*	修改：
*/

#include "stdafx.h"
#include "line.h"
#include "line.cmd.line.info.h"

#define PORT_CMD_LINE	__TEXT("port")
#define IP_CMD_LINE		__TEXT("ip")
#define REVBUF_CMD_LINE		__TEXT("revbuf")

command_line_info::command_line_info()
{
	ip_flag_ = false;
	port_flag_ = false;
	ip_ = __TEXT("");
	port_ = 0x0;
	recv_buff_size_ = 0x0;
	revbuf_flag_ = false;
}

command_line_info::~command_line_info()
{

}

void command_line_info::ParseParam( const TCHAR* pszParam, BOOL bFlag, BOOL bLast)
{
	CString arg(pszParam);

	if(bFlag){
		ip_flag_ = false;
		port_flag_ = false;
		revbuf_flag_ = false;
		
		if(arg.CompareNoCase(PORT_CMD_LINE) == 0) port_flag_ = true;
		if(arg.CompareNoCase(IP_CMD_LINE) == 0) ip_flag_ = true;
		if(arg.CompareNoCase(REVBUF_CMD_LINE) == 0) revbuf_flag_ = true;
	}	
	else{
		if(ip_flag_) ip_ = arg;
		if(port_flag_){
			port_ = atoi(arg.GetBuffer());
			arg.ReleaseBuffer();
		}
		if(revbuf_flag_){
			recv_buff_size_ = atoi(arg.GetBuffer());
			arg.ReleaseBuffer();
		}
	}
	
	CCommandLineInfo::ParseParam(pszParam, bFlag, bLast);
}
