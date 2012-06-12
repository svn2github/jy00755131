/*@
*	描述：该文件为测线commandline操作类的实现文件
*	继承：CCommandLineInfo
*	文件：line.cmd.line.info.h
*	注释：
*	参考文献：
*	版本：1.0.0.0
*	注释：
*	修改：
*/

#ifndef __LINE_COMMAND_LINE_INFO__
#define __LINE_COMMAND_LINE_INFO__

class command_line_info : public CCommandLineInfo
{
public:
	command_line_info();
	~command_line_info();
	void ParseParam( const TCHAR* pszParam, BOOL bFlag, BOOL bLast);

public:
	CString ip_; //!< 服务器ip地址
	unsigned short port_; //!< 服务器端口
	unsigned int recv_buff_size_; //!< 接受缓冲大小

private:
	bool ip_flag_;
	bool port_flag_;
	bool revbuf_flag_;
};

#endif /*__LINE_COMMAND_LINE_INFO__*/