// mktools.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "mktools.h"
#include <CommDlg.h>
#include <complex>
#include <time.h>

#define MAX_LOADSTRING 100
#define MAX_STRING_SIZE_LOCAL	128

#pragma comment(lib, "Comdlg32")

/**
@struct	__tagAD_DATA_FILE_HEAD_PRIVATE 
@brief	AD数据文件头信息
@note	typedef struct ___tagAD_DATA_FILE_HEAD_PRIVATE  AD_DATA_FILE_HEAD_PRIVATE , *AD_DATA_FILE_HEAD_PRIVATE_PTR;
*/
typedef struct __tagAD_DATA_FILE_HEAD_PRIVATE 
{
	char	config_path_[MAX_STRING_SIZE_LOCAL];//!< 配置文件路径
	char	sps_path_[MAX_STRING_SIZE_LOCAL];//!< SPS文件路径
	char	dat_path_[MAX_STRING_SIZE_LOCAL];//!< AD数据文件路径(该文件为通用文件描述)
	char	output_path_[MAX_STRING_SIZE_LOCAL];//!< 输出SEG-D文件路径
	char	remote_ip_[MAX_STRING_SIZE_LOCAL];	//!< 监听ip地址
	unsigned	remote_port_; //!< 端口

	int	shot_number_;			//!< 炮号 1 到 9999
	int	FileNumber_;			//!< 文件编号  1 到 9999

	float source_line_number_;	//!< 测线号
	float source_point_number_;	//!< 震源点号

	int sample_frequency_;		//!< 采样率250、500、1000、2000、4000 微秒
	int sample_length_;			//!< 采集长度1000到128000 毫秒

	int receiver_line_number_; //!< 接受线数量
	int	auxiliary_line_number_;		//!< 辅助线数量
	int	auxiliary_trace_number_;	//!< 辅助记录道数量	
	int trace_total_number_;	//!< 记录道总数1到100000
	int source_type_;			//!< 29-32 XXXX bin 震源类型0 = 无震源 1 = 脉冲 2 = 可控震源
	int sample_number_in_trace_;	//!< 33-36 XXXX bin 记录道中样本数目1 到 128000

	float tb_window_; //!< TB窗口值
	time_t datetime_; //!< 开始时间
	int tb_time_;	  //内部TB时刻=本地时间，从该时刻起记录数据

	float resistor_lower_limit_; //!< 电阻下限
	float resistor_higher_limit_;//!< 电阻上限
	float resistor_value_; //!< 电阻值
	float tilt_limit_; //!< 倾斜度界限
	float tilt_value_; //!< 倾斜度值
	bool resistor_deviation_; //!< 电阻误差
	bool tilt_deviation_; //!< 倾斜度误差
}AD_DATA_FILE_HEAD_PRIVATE, *AD_DATA_FILE_HEAD__PRIVATE_PTR;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MKTOOLS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MKTOOLS));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MKTOOLS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MKTOOLS);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd) {      return FALSE;   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

enum OUTPUT_STATUS
{
	SUCCESS_OK = 0x0, //!< 成功
	SUCCESS_FAIL = 0x1, //!< 失败
	SAVE_DIALOG_ERROR, //!< 打开保存文件对话框错误
	FILE_NAME_IS_EMPTY_ERROR, //!< 得到保存文件错误
	CREATE_FILE_NAME_ERROR, //!< 创建文件失败!
	CREATE_PROCESS_ERROR, //!< 创建进程失败
};

static const int __trace_size = 600;
static const int __aux_trace_size = 1;
static const int __sample_length = 2000;//ms
static const int __sample_freq = 1000;//1000/s
static const int __frame_size = 72;
static const int __frame_header = 6;
static const int __vale_size = 3;

/**
@func		int get_segd_file_name(TCHAR* file_name, size_t& file_name_size)
@detail	打开文件对话框
@param[in/out] file_name	文件名
@param[in/out]	file_name_size 文件名大小
@return SUCCESS_OK 成功,否则返回错误信息
@remark 
*/
int get_segd_file_name(TCHAR* file_name, size_t file_name_size)
{
	OPENFILENAME  ofn;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = file_name;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = file_name_size;
	ofn.lpstrFilter = __TEXT("SEGD文件格式\0*.SEGD\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrDefExt = __TEXT("SEGD");
	ofn.Flags = OFN_OVERWRITEPROMPT;

	if(GetSaveFileName(&ofn) == 0) {	
		SetLastError(SAVE_DIALOG_ERROR);
		return SAVE_DIALOG_ERROR;	
	}

	if(_tcsclen(ofn.lpstrFile) == 0){
		SetLastError(FILE_NAME_IS_EMPTY_ERROR);
		return  FILE_NAME_IS_EMPTY_ERROR;
	}

	return SUCCESS_OK;
}

/**
@func		void make_sin_data(float* * data, size_t& data_length)
@detail	生成正弦数据
@param[in/out]	data	生成sin数据
@param[in/out]	data_length 数据长度
@return void
@remark 该函数将分配数据大小,使用后必须释放	
*/
void make_sin_data(float* * data, size_t& data_length)
{
	static const int count = 5;
	static const float max = 6.2832 * count;
	static const int freq = 80;
	float i = 0;
	int tmp_freq = freq/count;
	float tmp_val = 0x0;

	*data = new float[freq];
	data_length = 0x0;
	ZeroMemory(*data, sizeof(float) * freq);

	for(i = 0; i < tmp_freq; ++i )
		(*data)[data_length++] = std::sin(i);

	tmp_val = (*data)[data_length-1];
	for(i = 0; i < tmp_freq; ++i)
		(*data)[data_length++] = 2 * sin(i) + tmp_val;

	tmp_val = (*data)[data_length-1];
	for(i = 0; i < tmp_freq; ++i)
		(*data)[data_length++] = 3 * sin(i) + tmp_val;

	tmp_val = (*data)[data_length-1];
	for(i = 0; i < tmp_freq; ++i)
		(*data)[data_length++] = 2 * sin(i) + tmp_val;

	tmp_val = (*data)[data_length-1];
	for(i = 0; i < tmp_freq; ++i)
		(*data)[data_length++] = sin(i) + tmp_val;
}

int convert_float_to_int(float f_val)
{
	static const int min_freq = 0x7fffff / 2.5f;
	static const float max_freq = 0x800000/2.5f;
	int result = 0x0;

	if(f_val < 0){
		f_val = -f_val;
		result = (f_val * min_freq) + 0x800000;
	}
	else{
		result = max_freq * f_val;
	}
	return result;
}

void make_data_file_header(AD_DATA_FILE_HEAD_PRIVATE& data_file_header)
{
	ZeroMemory(&data_file_header, sizeof(AD_DATA_FILE_HEAD_PRIVATE));

	strcpy(data_file_header.sps_path_, "..\\parameter\\JO");//!< SPS文件路径
	strcpy(data_file_header.output_path_, "..\\segd");//!< 输出SEG-D文件路径
	strcpy(data_file_header.remote_ip_, "192.168.0.19");	//!< 监听ip地址
	data_file_header.remote_port_ = 0x6310; //!< 端口

	data_file_header.shot_number_ = 2;			//!< 炮号 1 到 9999
	data_file_header.FileNumber_ = 3366;			//!< 文件编号  1 到 9999

	data_file_header.source_line_number_ = 6.0f;	//!< 测线号
	data_file_header.source_point_number_ = 22695.0f;	//!< 震源点号

	data_file_header.sample_frequency_ = __sample_freq ;		//!< 采样率250、500、1000、2000、4000 微秒
	data_file_header.sample_length_ = __sample_length;			//!< 采集长度1000到128000 毫秒

	data_file_header.receiver_line_number_ = 1; //!< 接受线数量
	data_file_header.auxiliary_line_number_ = 1;		//!< 辅助线数量
	data_file_header.auxiliary_trace_number_ = __aux_trace_size;	//!< 辅助记录道数量	
	data_file_header.trace_total_number_ = __trace_size;	//!< 记录道总数1到100000
	data_file_header.source_type_ = 1;			//!< 29-32 XXXX bin 震源类型0 = 无震源 1 = 脉冲 2 = 可控震源
	data_file_header.datetime_ = time(&data_file_header.datetime_) ; //!< 开始时间
}

/**
@func		int make_data_file(TCHAR const* file_name)
@param[in] file_name	输入data文件名
@return 成功返回SUCCESS_OK, 否则返回错误信息
@remark 
*/

int __tb_time = 0x0;
int make_data_file(TCHAR const* file_name)
{
	int *tmp_data = NULL;
	HANDLE file_handle = NULL;
	float * data = NULL;
	size_t data_length = 0x0;
	int size = 0, i = 0, j =0, k =0, tmp_val = 0, tmp_val2 = 0x0;
	int tb_time = GetTickCount();
	DWORD result = 0x0;
	AD_DATA_FILE_HEAD_PRIVATE data_file_header;
	float outp_data [__trace_size + __aux_trace_size][__sample_length +1];

	size = __frame_size * __vale_size + __frame_header;
	tmp_data = new int[size];
	ZeroMemory(tmp_data, sizeof(int) * size);
	ZeroMemory(outp_data, sizeof(outp_data));

	make_data_file_header(data_file_header);
	__tb_time = data_file_header.tb_time_ = tb_time + 10;	  //内部TB时刻=本地时间，从该时刻起记录数据
	data_file_header.sample_number_in_trace_ = data_file_header.sample_length_ + 1;		//!< 33-36 XXXX bin 记录道中样本数目1 到 128000

	//生成正弦数据
	make_sin_data(&data, data_length);

	for(i = data_file_header.trace_total_number_/2, k = 0; i >=0 ; --i, ++k)
		for(j = 0; j < data_length; ++j)		outp_data[i][k*6 + j] = 	data[j];

	for(i = data_file_header.trace_total_number_/2 + 1, k = 0; i < data_file_header.trace_total_number_ ; ++i, ++k)
		for(j = 0; j < data_length; ++j)			outp_data[i][k*6 + j] = 	data[j];

	//创建文件
	file_handle = CreateFile(file_name, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, NULL, NULL);
	if(file_handle  == INVALID_HANDLE_VALUE){	
		SetLastError(CREATE_FILE_NAME_ERROR);
		return  SUCCESS_FAIL;	
	}

	//写入data header
	WriteFile(file_handle, &data_file_header, sizeof(data_file_header), &result, NULL);
//	FlushFileBuffers(file_handle);

	tmp_val = data_file_header.auxiliary_trace_number_ + data_file_header.trace_total_number_;
	for(i = 0; i < 10; ++i, ++tb_time){
		memcpy(tmp_data, &tb_time, sizeof(tb_time));
		for(k = 0; k <  tmp_val; ++k){
			WriteFile(file_handle, tmp_data, size, &result, NULL);
	//		FlushFileBuffers(file_handle);
		}
	}

	int h = 0, 
		tmp_val3 = data_file_header.trace_total_number_ + data_file_header.auxiliary_trace_number_;
	k = (data_file_header.sample_frequency_ / __frame_size) * 2;	
	for(i = 0; i < k; ++i, ++tb_time){

		ZeroMemory(tmp_data, size);
		memcpy(tmp_data, &tb_time, sizeof(tb_time));
		for(h = 0; h < tmp_val3; ++h){

			tmp_val = __frame_header;
			for(j = 0; j < __frame_size; ++j){

				tmp_val2 = convert_float_to_int(outp_data[h][ i * __frame_size + j ]);
				memcpy(tmp_data + tmp_val, &tmp_val2,  3);
				tmp_val += 3;
			}
			WriteFile(file_handle, tmp_data, tmp_val, &result, NULL);
		}
	}

	//最加数据
	tmp_val = data_file_header.auxiliary_trace_number_ + data_file_header.trace_total_number_;
	size = __frame_size * __vale_size + __frame_header;
	ZeroMemory(tmp_data, size);
	for(i = 0; i < 10; ++i, ++tb_time){
		memcpy(tmp_data, &tb_time, sizeof(tb_time));
		for(k = 0; k <  tmp_val; ++k)	WriteFile(file_handle, tmp_data, size, &result, NULL);
	}


	if(data) delete []data;
	if(tmp_data) delete []tmp_data;
	
	if(file_handle)	CloseHandle(file_handle);

	return SUCCESS_OK;
}

/**
@func		int call_segd_converter(char const* segd_file_name, char const* data_file_name)
@param[in] 
@return 
@remark 
*/
int call_segd_converter(TCHAR const* segd_file_name, TCHAR const* data_file_name)
{
	TCHAR cmd_line[MAX_PATH] = {0};
	STARTUPINFO startup_info;
	PROCESS_INFORMATION  pi;

	ZeroMemory(&startup_info, sizeof(startup_info));
	ZeroMemory(&pi, sizeof(pi));
	
	startup_info.cb = sizeof(startup_info);
	startup_info.dwFlags = STARTF_USESHOWWINDOW;
	startup_info.wShowWindow = SW_HIDE;
	_stprintf_s(cmd_line,  __TEXT(" --data-file %s --tb-time %d"), data_file_name, __tb_time);
	if(CreateProcess(__TEXT("segd.conv.exe"), cmd_line, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &startup_info, &pi) == 0){
		SetLastError(CREATE_PROCESS_ERROR);
		return SUCCESS_FAIL;
	}
	return SUCCESS_OK;
}

/**
@func void clear()
@param void
@return void
@remark  释放生成segd文件过程中使用的资源和变量
*/
void clear()
{

}

void show_error_message(int error)
{
	TCHAR buf[MAX_PATH] = {0};
	ZeroMemory(buf, sizeof(buf)/sizeof(buf[0]));
	_stprintf_s(buf, sizeof(buf)/sizeof(buf[0]), __TEXT("生成检验segd文件时,产生[%d]错误,过程将中断!"), error);
	MessageBox(NULL,  buf, __TEXT("错误"), MB_ICONERROR);
}

/**
@func void make_segd_file()
@param void
@return void
@remark  该函数将自动生成SEGD文件
*/
void make_segd_file()
{
	HANDLE file_handle = INVALID_HANDLE_VALUE;
	int file_name_length = MAX_PATH;
	DWORD result = 0x0;
	TCHAR file_name[MAX_PATH] = {0};
	TCHAR tmp_data_file_name[MAX_PATH] = {0};
	TCHAR tmp_path[MAX_PATH] = {0};

	if(get_segd_file_name(file_name, file_name_length) != SUCCESS_OK){
		show_error_message(GetLastError());
		return ;
	}

	//根据定义生成数据源文件
	result = GetTempPath(sizeof(tmp_path)/sizeof(tmp_path[0]), tmp_path);
	tmp_path[result] = 0x0;

	GetTempFileName(tmp_path, __TEXT("DAT"), 0, tmp_data_file_name);
	if(make_data_file(tmp_data_file_name) != SUCCESS_OK){
		show_error_message(GetLastError());
		return;
	}

	//调用segd生成程序生成segd文件
	if(call_segd_converter(file_name,  tmp_data_file_name) != SUCCESS_OK){
		show_error_message(GetLastError());
		return ;
	}

	clear();
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case FILE_MAKE_SEGD_DATA:
			make_segd_file();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

#define MAX_STRING_SIZE_LOCAL 128

