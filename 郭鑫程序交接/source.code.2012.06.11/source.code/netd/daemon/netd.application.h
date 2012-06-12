/**
@brief	
@details	
@file	
@note	
@see	
@version	1.0.0.1.2011.12.07
@author	kuoxin	
@bug
*/
#ifndef __NETD_APPLICATION_H__
#define __NETD_APPLICATION_H__

class netd_application;

#include "..\core\matrix.application.h"
#include "..\core\matrix.queue.h"
#include "netd.socket.serv.h"
#include "netd.pcap.serv.h"

class inp_data
{
public:
	enum{MAX_IN_BUFFER_SIZE = 128};//256

	inp_data(){SecureZeroMemory(buf_, sizeof(buf_));};
	inp_data(inp_data& data){
		if(&data != this){
			CopyMemory(buf_, data.buf_, sizeof(buf_));
		}
	};
	~inp_data(){};
	unsigned char buf_[MAX_IN_BUFFER_SIZE];
	inp_data& operator= (inp_data const& d){
		if(this != &d){
			CopyMemory(buf_, d.buf_, sizeof(buf_));
		}
		return *this;
	};
	unsigned int copy(const unsigned char* buf, unsigned int size){
		int length = 0;
		SecureZeroMemory(buf_, sizeof(buf_));
		if(size <= MAX_IN_BUFFER_SIZE){	CopyMemory(buf_, buf, length = size);}
		else{							CopyMemory(buf_, buf, length = sizeof(buf_));}
		return length;		 
	};
};

class outp_data
{
public:
	enum{MAX_OUT_BUFFER_SIZE = 128};

public:
	outp_data(unsigned char* buf, unsigned int size){
		SecureZeroMemory(buf_, sizeof(buf_));
		copy(buf, size);
	};

	outp_data(){SecureZeroMemory(buf_, sizeof(buf_));};

	outp_data(outp_data& data){
		if(&data != this){
			CopyMemory(buf_, data.buf_, sizeof(buf_));
		}
	};

	~outp_data(){};

	unsigned char buf_[MAX_OUT_BUFFER_SIZE];

	outp_data& operator= (outp_data const& d){
		if(this != &d){
			CopyMemory(buf_, d.buf_, sizeof(buf_));
		}
		return *this;
	};

	unsigned int copy(const unsigned char* buf, unsigned int size){
		int length = 0;
		SecureZeroMemory(buf_, sizeof(buf_));
		if(size <= MAX_OUT_BUFFER_SIZE){	CopyMemory(buf_, buf, length = size);}
		else{								CopyMemory(buf_, buf, length = sizeof(buf_));}
		return length;		 
	};
};


class netd_application:public matrix_application
{
public:
	netd_application(int argc, char_t **argv, char_t **envp = NULL);
	~netd_application();

public:
	void output_log(TCHAR* log);

protected:
	int run_private();
	bool init_instance_private();
	void release_instance_private();
	void start();
	void stop();
	void clear_console();
	void show_helper(bool first_running, TCHAR cmd_char);
	void show_status_dialog();
	int  read_opt();
	unsigned get_line(FILE* file_ptr, char* buf, int buf_size);

protected:
	static unsigned __stdcall status_dialog_thread_proc(void* args);
	static INT_PTR CALLBACK status_dialog_proc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);

public:
	unsigned int netcard_id_; //!< pcap绑定网卡编号
	unsigned int lci_ip_;//!< LCI ip地址(pcap读取发送数据到该ip) 
	unsigned int lci_inp_port_;//<! pcap读取LCI 端口
	unsigned int lci_outp_port_;//<! pcap写入端口

	unsigned int matrix_service_ip_;//!< 上位机ip地址(socket监听该ip,并将LCI上行数据包发送到该ip)
	unsigned int matrix_service_listen_port_;//<! 36866 #socket写入上位机端口

	unsigned int netd_ip_; //!< 中转程序ip地址
	unsigned int netd_listen_port_; //!< 中转程序监听端口
	unsigned int netd_outp_port_; //!< 中转程序通过pcap写入到LCI的中转程序发送端口
	unsigned int netd_recv_buffer_size_;//<! 10485760 #socket上位机接受缓冲
	unsigned int netd_snd_buffer_size_;//<! 10485760 #socket上位机发送缓冲

	unsigned int pcap_buff_size_; //<! pcap缓冲大小
	unsigned int pcap_max_package_size_;//!< pcap指定最大数据包大小
	unsigned int pcap_timeout_;	//!< pcap操作超时时间以毫秒为单位
	unsigned int pcap_outp_poll_time_; //!< pcap写入LCI时,轮询outp_queue队列时间
	char pcap_filter_[MAX_STRING_SIZE]; //!< 指定当前pcap使用的过滤器参数

	unsigned int inp_queue_size_;//!< 存放pcap输入(读取)队列缓冲大小 
	unsigned int outp_queue_size_;//!< 存放pcap输出(写入)队列缓冲大小

	unsigned int netd_recv_poll_time_;//<! 10 #中转程序从上位机接受数据的轮询时间
	unsigned int netd_snd_poll_time_;//<! 10 #中转程序向上位机发送数据时,轮询缓冲队列时间

	volatile long pcap_data_inp_num_; //!< pcap接受数据数量
	volatile long pcap_data_outp_num_; //!< pcap发送数据数量
	volatile long socket_data_received_num_; //!< socket接受上位机数据数量
	volatile long socket_data_sent_num_; //!< socket发送给上位机数据数量

	matrix_queue<inp_data>* inp_queue_; //!< pcap输入(读取)数据
	matrix_queue<outp_data>* outp_queue_; //!< pcap的输出(写入)数据

private:
	HWND status_dialog_;//!< 统计对话框 
	HINSTANCE instance_;//!< 当前进程实例句柄
	HANDLE status_dialog_thread_handle_;//!< 统计对话框线程句柄
	unsigned status_dialog_state_;//!< 定义当前流量统计对话框状态 0 : 没有创建窗体, 1: 显示窗体, 2: 隐藏窗体

	UINT_PTR event_id_; //!< 统计对话框的timer标记ID

	CRITICAL_SECTION log_critial_section_; //!< 日志输出临界值

	netd_socket_service* socket_service_ptr_;//!< 上位机接受发送数据类
	netd_pcap_service* pcap_service_ptr_; //!< pcap接受发送数据到LCI类

	bool netd_service_be_started_;//!<当前程序是否开始中转功能,如果为true表示开始,否则没有开始;默认为false没有开始
};

#endif /*__NETD_APPLICATION_H__*/
