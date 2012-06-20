/**
@brief	实现AD采集,使用8端口,定时使用recv获取数据,使用的采样率为1K, 定时器: 100MS, 默认端口0x5001 到 0x5008

@details	具体实现方法: 每次定时器到达时,检查当前N端口是否收到数据,并且回	逆到之前的N-1, N-2, N-3端口,
			如果发现N-1, N-2,N-3端口是否收到全部AD数据,如果不全,N-3直接保存AD文件中,不再重发;N-1,N-2进行重发AD查询请求

@file	ad.collection.resp.cpp

@note	每一个设备保存为一个文件,文件名为当前设备的IP地址+log,当AD出错或重发时, 将信息写入ad.recv.data.err中
@see	贺玲的Borland cpp builder 三端口程序
@version	1.0.12.55.2011.8.30.EIGHT.PORTS.1K
@author	kuoxin	
@bug	1.0.12.55.2011.8.30.EIGHT.PORTS.1K	修改回收资源错误,修改在未开始采集时,发送AD数据帧重发操作
*/

#include "../common/config.h"
#include "../common/typedef.h"
#include "../common/sockimpl.h"
#include "../common/code.conv.h"
#include "matrix.h"
#include "equipment.comm.order.h"
#include "common.functions.h"
#include "ad.collection.resp.h"

/**	\enum (non-name)
	\brief 定义该文件中使用的常量 
*/
enum
{
	MATRIX_AD_COLLECTION_RESP_SND_PORT = DESTINATION_PORT,
	MAX_AD_COUNTER = 0x3,
	MAX_AD_WAIT_COUNTER = 0x3,
	DEFAULT_PIPE_SIZE = 0x8,//!< 当前的监听端口数量	
	DEFAULT_CHECKING_ARRAY_SIZE = 0x4, //!< 操作中将使用的队列大小,该值主要是由多端口,重发机制相关 
	DEFAULT_CHECKINg_ARRAY_ITEM_SIZE = 0x2000,//!< 当前使用0x2000个设备 
	DEFAULT_AD_DATA_COMMAND_SIZE = 0x2000,//!< 当前最多能使用的AD命令缓冲大小 
	DEFAULT_AD_CIRCLING_COUNTER = 2048,//!< 当前AD指针的循环计数器,但达到该值是将重新从当前应该数字减去本计数器 
};

/** \struct __tagAD_DATA_COMMAND_PACK
	\brief 保存重发数据包 
*/
typedef struct __tagAD_DATA_COMMAND_PACK
{
	unsigned char pack_[MAX_OUTPUT_PACKAGE_SIZE]; //!< 当前重发数据包内容 
	__tagAD_DATA_COMMAND_PACK* next_;				//!< 指向下一个机构体指针,便于形成一个链表 
}AD_DATA_COMMAND_PACK, *AD_DATA_COMMAND_PACK_PTR;

/** \struct	__tagAD_DATA_INFO
	\brief AD采集数据数据结构 
*/
typedef struct __tagAD_DATA_INFO
{
	EQUIPMENT_ITEM_PTR			equipment_ptr_;		//!< 设备数据结构
	unsigned int				resend_counter_;//!< 重发错误计数器
	unsigned int				offset_;//!< 偏移量
	unsigned char				recv_pack_[MAX_INPUT_PACKAGE_SIZE]; //!< 接受的指令
	AD_DATA_COMMAND_PACK_PTR	resent_pack_; //!< 发送查询指令
	FILE*						output_log_; //!< 每个设备AD采集输出文件
	bool						be_pack_received_;//!< 是否收到数据
	unsigned					pkg_time_;	//!< 包时间
}AD_DATA_INFO, *AD_DATA_INFO_PTR;

/** \struct __tagTHE_AD_DATA_REC_PACKAGE_INFO
	\brief	AD采集数据帧 
*/
typedef struct __tagTHE_AD_DATA_REC_PACKAGE_INFO	
{
	unsigned ip_;	//!< IP地址
	unsigned port_;	//!< 端口
	unsigned cmd_word_;	//!< 关键值
	unsigned offset_;        //!< 偏移量
	unsigned pkg_time_;      //!< 打包时间
	unsigned char* data_addr_ptr_; //!< 记录指针
}THE_AD_DATA_REC_PACKAGE_INFO, *THE_AD_DATA_REC_PACKAGE_INFO_PTR;

/** \typedef AD_DATA_HASH_MAP
 \brief 定义接受AD数据的存储的hash表
 */
typedef stdext::hash_map<unsigned long, AD_DATA_INFO_PTR> AD_DATA_HASH_MAP;

/** \typedef AD_DATA_HASH_MAP_ITERATOR
 \brief 定义接受AD数据的存储的hash表的迭代器
 */
typedef AD_DATA_HASH_MAP::iterator AD_DATA_HASH_MAP_ITERATOR;

extern stdext_hash_map __route_hash_map;
extern stdext_hash_map __ip_hash_map;
extern APPLICATION_ENVIRONMENT	__application_environment;
extern bool __be_ad_collection_stopping; //!< 是否停止AD采集

unsigned int __pipe_size; //!< 现在使用的存储AD帧的通道数量
unsigned int __current_pipe_index; //!< 当前需要接受的AD帧的通道数组索引
SOCKET_IMPL_PTR __ad_collection_sockets; //!< 通道存储结构体指针, 当前使用了动态分配内存方式
FILE* __error_file = NULL; //!< AD错误数据信息

AD_DATA_INFO __ad_data_array[DEFAULT_CHECKING_ARRAY_SIZE * DEFAULT_CHECKINg_ARRAY_ITEM_SIZE]; //!< 存放接受AD数据所用到的元数据
AD_DATA_HASH_MAP __ad_data_hash_map[DEFAULT_CHECKING_ARRAY_SIZE];//使用IP对应AD存储结构体 //!< 存放接受到AD采集数据后,存放在0 ... DEFAULT_CHECKING_ARRAY_SIZE hash表缓冲中

AD_DATA_COMMAND_PACK __ad_data_commands_array[DEFAULT_AD_DATA_COMMAND_SIZE];//!< 存放发送的AD命令元数据队列
AD_DATA_COMMAND_PACK_PTR __free_ad_data_commands; //!< 建议一个可重用的资源链表,该变量指向整个队列的第一个元素
int __the_first_recv_ad_data_counter; //!< 指示是不是当前是第一次查询接受数据,这个变量主要是为了在开始接受数据时,进行特殊处理
bool __the_first_recv_ad_data; //!< 是不是第一次收到数据,初始化为true, 但收到数据后将其变为false

/**
@fn	AD_DATA_COMMAND_PACK_PTR get_ad_command_pack()
@brief 从AD命令资源列表中得到空的AD命令数据结构
@param	void
@return	AD_DATA_COMMAND_PACK_PTR	返回一个有效指针,要么返回NULL
@sa:
*/
AD_DATA_COMMAND_PACK_PTR get_ad_command_pack()
{
	AD_DATA_COMMAND_PACK_PTR pack_ptr = NULL;

	if(__free_ad_data_commands)	{	pack_ptr = __free_ad_data_commands;	__free_ad_data_commands = __free_ad_data_commands->next_;}
	else						{	pack_ptr = NULL;}
	return pack_ptr;
}

/**
@fn	bool init_ad_collection_resp(unsigned int pipe_size, unsigned short base_port)
@brief 初始化接受AD采集数据
@param	pipe_size	uint	指定当前可以接受的通道数量,该参数必须为8的倍数	
@param	base_port	ushort	指定当前可以接受的端口的最小端口号, 初始化时,将根据通道数量,逐个递增来分配端口
@return	bool	如果成功返回true,否则返回false
@sa:由于pipe_size要求使用是8的倍数,因此有必要去检查
*/
bool init_ad_collection_resp(unsigned int pipe_size, unsigned short base_port)
{
	bool is_ok = true;
	int size = 0x0;
	int i = 0, j = 0;
	SOCKET_IMPL_PTR socket_ptr = NULL;
	char buf[MAX_STRING_SIZE] = {0};

	if(pipe_size <= 0)	__pipe_size = DEFAULT_PIPE_SIZE;
	else				__pipe_size = pipe_size;

	if(__ad_collection_sockets)	free(__ad_collection_sockets);

	//create socket
	size = __pipe_size * sizeof(SOCKET_IMPL);
	__ad_collection_sockets = (SOCKET_IMPL_PTR)malloc(size);
	if(__ad_collection_sockets == NULL){SetLastError(ALLOC_MEMORY_ERROR); return is_ok = false;}

	SecureZeroMemory(__ad_collection_sockets, size);
	for(socket_ptr = __ad_collection_sockets, i = 0; i < __pipe_size; ++i, ++socket_ptr){

		socket_ptr->listen_port_ = base_port + i;
		socket_ptr->remote_port_ = MATRIX_AD_COLLECTION_RESP_SND_PORT;
		is_ok = create_udp_socket(socket_ptr, SOCKET_BOARDCAST_ADDRESS);
		if(!is_ok){	SetLastError(CREATE_SOCKET_ERROR); return is_ok;}
	}

	__current_pipe_index = 0x0;
	SecureZeroMemory(&__ad_data_array, sizeof(__ad_data_array));
	SecureZeroMemory(__ad_data_commands_array, sizeof(__ad_data_commands_array));

	//make a free data pool
	__free_ad_data_commands = __ad_data_commands_array;
	size = sizeof(__ad_data_commands_array) / sizeof(__ad_data_commands_array[0]) - 1;
	for(i = 0; i < size; ++i)	__ad_data_commands_array[i].next_ = __ad_data_commands_array + i + 1;

	for(i = 0; i < DEFAULT_CHECKING_ARRAY_SIZE; ++i)	__ad_data_hash_map[i].clear();

	//AD data structure
	size = sizeof(__ad_data_array) / sizeof(__ad_data_array[0]);
	if(size < (__ip_hash_map.size() * 4)){	
		SetLastError(ALLOCATE_MEMORY_SIZE_TOO_SMALL);	
		output_log(IDS_STRING165, size, __ip_hash_map.size() * 4);	
		return is_ok = false;
	}

	//create a file for per-ip
	i = 0;
	for(stdext_hash_map::iterator itr = __ip_hash_map.begin(); itr != __ip_hash_map.end(); ++itr){
		
		__ad_data_array[i].equipment_ptr_ = itr->second;
		if(__ad_data_array[i].equipment_ptr_->is_valid_equipment_ && __ad_data_array[i].equipment_ptr_->type_ == FDU){

			sprintf_s(buf, sizeof(buf), "%X.log", itr->first);
			__ad_data_array[i].output_log_ = fopen(buf, "w");
			if(__ad_data_array[i].output_log_ == NULL) {SetLastError(CREATE_AD_DATA_FILE_ERROR); return is_ok = false;}
			
			for(j = 0; j < DEFAULT_CHECKING_ARRAY_SIZE; ++j){

				memcpy(__ad_data_array + i + j, __ad_data_array + i, sizeof(__ad_data_array[0]));
				__ad_data_hash_map[j][itr->first] = __ad_data_array + i + j;
			}
			i += j;
		}
	}

	//error file
	__error_file = fopen("ad.recv.data.err", "w");
	if(__error_file == NULL){SetLastError(CREATE_AD_DATA_ERROR_FILE_ERROR); return is_ok = false;}

	__the_first_recv_ad_data_counter = 0; //指示是不是当前是第一次接受数据
	__the_first_recv_ad_data = true;
	__be_ad_collection_stopping = false;
	return is_ok;
}

void release_ad_collection_resp()
{
	int i = 0;
	SOCKET_IMPL_PTR sock_ptr = __ad_collection_sockets;

	if(sock_ptr != NULL){
		for(i = 0; i < __pipe_size; ++i, ++sock_ptr)	if(sock_ptr)	close_udp_socket(sock_ptr);

		free(__ad_collection_sockets);
		__ad_collection_sockets = NULL;
	}

	for(AD_DATA_HASH_MAP_ITERATOR itr = __ad_data_hash_map[0].begin(); itr != __ad_data_hash_map[0].end(); ++itr){
		fclose(itr->second->output_log_);
		itr->second->output_log_ = NULL;
	}

	for(i = 0; i < DEFAULT_CHECKING_ARRAY_SIZE; ++i)	__ad_data_hash_map[i].clear();

	if(__error_file) fclose(__error_file);

}

bool check_legal_package(unsigned char* buf, int buf_size, THE_AD_DATA_REC_PACKAGE_INFO_PTR ad_data_rec_pack_info)
{
	memcpy(&ad_data_rec_pack_info->ip_, &buf[0x10], 4);
	memcpy(&ad_data_rec_pack_info->port_, &buf[0x18], 2);
	memcpy(&ad_data_rec_pack_info->cmd_word_, &buf[0x1a], 2);
	if (ad_data_rec_pack_info->cmd_word_ == CMD_AD)
	{
		memcpy(&ad_data_rec_pack_info->offset_, &buf[0x1c], 2);
		memcpy(&ad_data_rec_pack_info->pkg_time_, &buf[0x1e], 2);
		ad_data_rec_pack_info->data_addr_ptr_ = buf + 0x24;
		return true;
	}
	else 
		return false;
}

int get_next_pipe_index()
{
	int tmp = __current_pipe_index;
	if(++tmp >= __pipe_size)	tmp = 0;
	return tmp;
}

int get_next_pipe_index(unsigned int current_pipe_index)
{
	int tmp = current_pipe_index;
	if(++tmp >= __pipe_size)	tmp = 0;
	return tmp;
}

int get_prev_pipe_index()
{
	int tmp = __current_pipe_index;
	if(--tmp < 0) tmp += DEFAULT_PIPE_SIZE;
	return tmp;
}

int get_prev_pipe_index(unsigned int current_pipe_index)
{
	int tmp = current_pipe_index;
	if(--tmp < 0) tmp += DEFAULT_PIPE_SIZE;
	return tmp;
}

int get_current_pipe_index(){	return __current_pipe_index;}

int get_ad_data_index_from_pipe_index(int pipe_index)
{
	int resend_array_index = 0x0;
	if(pipe_index < 4)  resend_array_index = pipe_index;
	else				resend_array_index = pipe_index - DEFAULT_CHECKING_ARRAY_SIZE;
	return resend_array_index;
}

bool is_valid_ad_frame(unsigned prev_offset, unsigned current_offset)
{
	bool is_ok = false;
	unsigned tmp = current_offset - prev_offset;
	if(tmp < 0) tmp += DEFAULT_AD_CIRCLING_COUNTER;
	return is_ok = (tmp / 72) == 1 ? true : false;
}

/*@
function:	void proc_ad_collection_resp_private(SOCKET_IMPL_PTR socket_impl_ptr, 
												unsigned pipe_index, 
												bool be_ad_data_resent_again)
args:	socket_impl_ptr			[in]	当前使用的socket数据结构指针,并且该指针要求是指针数组变量
		pipe_index				[in]	管道索引
		be_ad_data_resent_again	[in]	是否进行重发检查
return:	void
comment: 该函数将每次对socket进行遍历判断是否接收数据.
*/
void proc_ad_collection_resp_private(SOCKET_IMPL_PTR socket_impl_ptr, unsigned pipe_index, bool be_ad_data_resent_again)
{
	char recv_buf[MAX_INPUT_PACKAGE_SIZE] = {0};
	char error_buf[MAX_INPUT_PACKAGE_SIZE * 6] = {0};
	unsigned long ioctl_argp = 0L;
	int recv_size = 0;
	THE_AD_DATA_REC_PACKAGE_INFO pack;
	SOCKADDR_IN remote_ip;
	unsigned ad_data_index = 0x0;
	AD_DATA_HASH_MAP_ITERATOR itr;	
	AD_DATA_INFO_PTR ad_data_ptr = NULL;
	
	SecureZeroMemory(&remote_ip, sizeof(remote_ip));
	remote_ip.sin_family = AF_INET;
	remote_ip.sin_port = htons(MATRIX_AD_COLLECTION_RESP_SND_PORT);
	remote_ip.sin_addr.s_addr = htonl(INADDR_BROADCAST);

	ad_data_index = get_ad_data_index_from_pipe_index(pipe_index);
	while(true){

		if(ioctlsocket(socket_impl_ptr->socket_, FIONREAD, &ioctl_argp) == 0){

			if(ioctl_argp > 0){

				recv_size = recv(socket_impl_ptr->socket_, recv_buf, sizeof(recv_buf), 0);

				if(recv_size == SOCKET_ERROR)	break;
				else if(recv_size == 0x0){

					WSASetLastError(REMOTE_CLOSE_CONNECTION_GRACEFULLY);
					break;
				}
				else if(recv_size > MAX_INPUT_PACKAGE_SIZE){

					WSASetLastError(AD_DATA_RECV_SIZE_ERROR);
					recv_size = bin_to_ansi((unsigned char*)recv_buf, recv_size, error_buf, sizeof(error_buf));
					output_log(IDS_STRING163, recv_size, error_buf);
					continue;
				}
				else{

					__the_first_recv_ad_data = false;
					SecureZeroMemory(&pack, sizeof(pack));
					//check this data package
					if(!check_legal_package((unsigned char*)recv_buf, recv_size, &pack)){
						SetLastError(AD_DATA_FORMAT_ERROR);
						recv_size = bin_to_ansi((unsigned char*)recv_buf, recv_size, error_buf, sizeof(error_buf));
						output_log(IDS_STRING164, error_buf);
						continue;
					}

					itr = __ad_data_hash_map[ad_data_index].find(pack.ip_);
					if(itr != __ad_data_hash_map[ad_data_index].end()){
						
						ad_data_ptr = itr->second;
						/*提示:
							前提	
								1. 必须保证当前处理的速度能和AD数据填充速度保持相对稳定,不会出现转圈现象
								2. 硬件能够保证准确一致的发送AD数据不会出现中断问题

							操作	在前提的保证下可以不用考虑偏移量问题,目前的实现方法将不去考虑比较偏移量
							转圈现象	当前处理有一定的速度限制理论上只能处理1K采样率以下的情况,高于1K使用当
									前的多端口会出现转圈现象,使处理变得比较复杂,到不如直接使用单端口处理
							注释	以上说明是基于个人对于当前项目的理解,不能保证准确性,只能参考
						*/

						ad_data_ptr->be_pack_received_ = true;
						ad_data_ptr->offset_ = pack.offset_;
						ad_data_ptr->pkg_time_ = pack.pkg_time_;
						memcpy(ad_data_ptr->recv_pack_, recv_buf, recv_size);
					}
					else{
						
						WSASetLastError(NOT_FIND_IP_ADDR);
						recv_size = bin_to_ansi((unsigned char*)recv_buf, recv_size, error_buf, sizeof(error_buf));
						output_log(IDS_STRING166, pack.ip_, error_buf);
						continue;
					}
				}
			}
			else	break;
		}
		else	break;
	}

	if(be_ad_data_resent_again && !__the_first_recv_ad_data && !__be_ad_collection_stopping){

		itr = __ad_data_hash_map[ad_data_index].begin();
		while(itr != __ad_data_hash_map[ad_data_index].end()){

			ad_data_ptr = itr->second;
			if(!ad_data_ptr->be_pack_received_){

				ad_data_ptr->resent_pack_ = get_ad_command_pack();
				if(ad_data_ptr->resent_pack_ == NULL){	

					output_log(IDS_STRING167, sizeof(__ad_data_commands_array));	
					SetLastError(NOT_ENOUGH_AD_COMMAND_BUFFER);	
					PostMessage(__application_environment.main_window_, WM_CLOSE, NULL, NULL);
					return;
				}

				if(ad_data_ptr->resend_counter_ == 0)
					make_ad_loss_req_package(ad_data_ptr->equipment_ptr_, ad_data_ptr->resent_pack_->pack_, 
					sizeof(ad_data_ptr->resent_pack_->pack_), 
					socket_impl_ptr->listen_port_, ad_data_ptr->offset_);

				recv_size = udp_send(socket_impl_ptr, (char*)ad_data_ptr->resent_pack_->pack_, 
					sizeof(ad_data_ptr->resent_pack_->pack_), (SOCKADDR*)&remote_ip);

				if(recv_size != sizeof(ad_data_ptr->resent_pack_->pack_)){
					output_log(IDS_STRING169, recv_size, sizeof(ad_data_ptr->resent_pack_->pack_));
					continue;
				}
				else if(recv_size == SOCKET_ERROR){	output_log(IDS_STRING168, WSAGetLastError());	continue;}

				++ad_data_ptr->resend_counter_;
			}
			++itr;
		}
	}
}

void save_ad_data(int pipe_index)
{
	unsigned ad_data_index = get_ad_data_index_from_pipe_index(pipe_index);
	AD_DATA_INFO_PTR ad_data_ptr = NULL;
	AD_DATA_HASH_MAP_ITERATOR itr;

	unsigned int size = 0;
	char buf[MAX_INPUT_PACKAGE_SIZE * 6];

	itr = __ad_data_hash_map[ad_data_index].begin();
	while(itr != __ad_data_hash_map[ad_data_index].end()){

		ad_data_ptr = itr->second;
		if(ad_data_ptr->be_pack_received_){ //写入AD数据文件

			size = bin_to_ansi(ad_data_ptr->recv_pack_, sizeof(ad_data_ptr->recv_pack_), buf, sizeof(buf));
			buf[size] = 0x0;
			fprintf(ad_data_ptr->output_log_, "[SN: %X][IP: %X][time: %d][offset: %d] %s\n", 
				ad_data_ptr->equipment_ptr_->serial_no_,
				ad_data_ptr->equipment_ptr_->ip_,
				ad_data_ptr->pkg_time_, ad_data_ptr->offset_, buf);

			//fflush(ad_data_ptr->output_log_);

			if(ad_data_ptr->resend_counter_ > 0){
				fprintf(__error_file, "当前SN为 %X, IP地址为 %X的设备, 在偏移量为 %d的地方, 通过重复请求AD数据%d次,才将数据找回...\n", 
				ad_data_ptr->equipment_ptr_->serial_no_, ad_data_ptr->equipment_ptr_->ip_,
				ad_data_ptr->offset_, ad_data_ptr->resend_counter_);
			//	fflush(__error_file);
			}

		}
		else{//写入错误日志

			fprintf(__error_file, "当前SN为 %X, IP地址为 %X的设备, 在偏移量为 %d的地方, 通过重复AD数据%d次请求, 但是没有收到AD采集数据...\n", 
				ad_data_ptr->equipment_ptr_->serial_no_, ad_data_ptr->equipment_ptr_->ip_,
				ad_data_ptr->offset_, ad_data_ptr->resend_counter_);
			//fflush(__error_file);
		}

		ad_data_ptr->be_pack_received_ = false;
		ad_data_ptr->offset_ = 0x0;
		ad_data_ptr->pkg_time_ = 0x0;
		ad_data_ptr->resend_counter_ = 0x0;

		//回收重发资源
		if(ad_data_ptr->resent_pack_){
			ad_data_ptr->resent_pack_->next_ = __free_ad_data_commands;
			__free_ad_data_commands = ad_data_ptr->resent_pack_;
			ad_data_ptr->resent_pack_ = NULL;
		}

		++itr;
	}
}

/**

*/
void proc_ad_collection_resp()
{
	int pipe_index = 0x0;

	//检查当前的端口是否有AD数据
	if(__the_first_recv_ad_data_counter >= 0){
		pipe_index = get_current_pipe_index();
		proc_ad_collection_resp_private(__ad_collection_sockets + pipe_index, pipe_index, true);
	}

	//检查当前端口前1个端口
	if(__the_first_recv_ad_data_counter >= 1){
		pipe_index = get_prev_pipe_index(pipe_index);
		proc_ad_collection_resp_private(__ad_collection_sockets + pipe_index, pipe_index, true);
	}

	//检查当前端口前2个端口
	if(__the_first_recv_ad_data_counter >= 2){
		pipe_index = get_prev_pipe_index(pipe_index);
		proc_ad_collection_resp_private(__ad_collection_sockets + pipe_index, pipe_index, true);
	}

	//检查当前端口前3个端口是否已经都收集完成数据
	if(__the_first_recv_ad_data_counter >= 3){
		pipe_index = get_prev_pipe_index(pipe_index);
		proc_ad_collection_resp_private(__ad_collection_sockets + pipe_index, pipe_index, false);
	}

	if(!__the_first_recv_ad_data){

		if(++__current_pipe_index >= __pipe_size)	__current_pipe_index = 0x0;

		//检查当前端口前3个端口是否已经都收集完成数据,并且写文件,将丢包数据也写文件
		if(__the_first_recv_ad_data_counter >= 3 && !__be_ad_collection_stopping)
			save_ad_data(pipe_index);

		if(++__the_first_recv_ad_data_counter > __pipe_size)	
			__the_first_recv_ad_data_counter = __pipe_size;
	}
}

