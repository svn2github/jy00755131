
#pragma once
#include "line.socket.h"
#include "STATUS_DIALOG.h"

#define WM_LOGIN_SERVER											(WM_USER + 102)
#define WM_QUERY_RECV_SECTION							(WM_USER + 103)
#define WM_QUERY_RECV_SECTION_RESULT				(WM_USER + 104)
#define WM_UPDATE_DEVICE_TABLE							(WM_USER + 105)
#define WM_FIELD_ON_COMPLETE								(WM_USER + 106)
#define WM_FIELD_OFF_COMPLETE								(WM_USER + 107)
#define WM_QUERY_INSTRUMENT_TEST_BASE_LIMIT	(WM_USER + 108)

class line_socket_impl : public line_socket
{
public:
	enum {
		CURRENT_CMD_NULL = 0x0, //!< 定义默认操作指令
		MAX_REPEAT_COUNT = 3, //!< 重复次数,其中包括发送,接受等
		MAX_TIMEOUT_SIZE = 3000, //!< 等待超时时间
	};
private:

	/**
		@enum TIMER_TYPE
		@detail 定时器类型的时间ID
	*/
	enum TIMER_TYPE{
		CLIENT_CONNECT_RESP_TIMER = 111,//!<等待CLIENT CONNECT响应
		FIELD_ON_RESP_TIMER, //!< 等待Field On响应
		FIELD_ON_WAIT_TIME_TIMER, //!< 等待Field On等待时间通知
		FIELD_OFF_RESP_TIMER, //!< 等待Field Off应答
		QUERY_INSTRUMENT_INFO_RESP_TIMER, //!<等待CmdQueryInstrumentInfo应答
		QUERY_INSTRUMENT_INFO_ALL_RESP_TIMER, //!<等待CmdQueryInstrumentInfoAll应答

		QUERY_INSTRUMENT_INFO_TIMER, //!<等待分包时CmdQueryInstrumentInfo指令
		QUERY_INSTRUMENT_INFO_ALL_TIMER, //!<等待CmdQueryInstrumentInfoAll指令
		QUERY_RECV_SECTION_RESULT_TIMER, //!< 等待CmdQueryRecvSection指令返回结果集合
		UPDATE_DEVICE_TABLE_TIMER, //!< 等待CmdQueryUpdateTable指令返回结果集合
		QUERY_INSTRUMENT_TEST_BASE_LIMIT_RESULT_TIMER, //!< 等待CmdQueryInstrumentTestLimitXMLInfo指令返回结果集合
	};

public:
	line_socket_impl(CString ip, unsigned short port, unsigned int recv_buff_size);
	~line_socket_impl(void);

public: //virtual function
	virtual void OnConnect(int nErrorCode);

public://export function
	bool start_heartbeat_thread();
	bool end_heartbeat_thread();
	bool connect_server();
	bool login_server();
	bool field_on();
	bool field_off();
	bool query_instrument_info(int line_id, int zone_id);
	bool query_instrument_info_all();
	bool query_recv_section();
	bool query_instrument_test_limit();

protected://virtual function
	void send_socket_message(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr);
	char* alloc_recv_pack_buff(int size);
	char* get_recv_pack_buff(){return recv_pack_buff_;};

protected:
	void login_server_resp(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr);
	void field_on_resp(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr);
	void field_off_resp(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr);
	void query_instrument_info_resp(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr);
	void query_instrument_info_all_resp(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr);
	void field_on_wait_time(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr);
	void notify_update_table(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr);
	void recv_querying_instrument_info(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr);
	void recv_querying_instrument_info_all(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr);
	void query_recv_section_resp(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr);
	void query_recv_section_result(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr);
	void query_instrument_test_limit_resp(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr);
	void query_instrument_test_limit_result(SOCKET_PACKAGE_RECV_PTR recv_pack_ptr);

private://static thread function
	static UINT __cdecl hit_test_thread_proc(LPVOID pParam);
	static VOID CALLBACK timer_proc(HWND hwnd,	  UINT uMsg,	 UINT_PTR idEvent, DWORD dwTime);
private:
	static unsigned short current_cmd_;//!< 当前发送并等待响应的命令字
	static bool is_timeout_;//!< 当前发送并等待响应的命令是否超时
	static UINT_PTR time_id_;//!< 定时器标记
	static unsigned int timer_event_id_;//!< 定时器事件ID
private:
	CWinThread* hit_test_thread_; //!< 心跳操作定时发送数据
	CEvent*	hit_test_event_;//!< 心跳操作控制事件对象

	char* recv_pack_buff_;//分配接受数据缓冲,主要用于支持多报文情况
	int current_recv_pack_buff_index_;//!< 当前已经使用的内存缓冲大小
	int recv_pack_buff_size_;//分配接受数据缓冲大小
public:
	virtual void OnClose(int nErrorCode);
};
