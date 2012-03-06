
#include <list>

using std::list;
using std::string;

#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _LogOutPut_H
#define   _LogOutPut_H
// 日志文件夹
#define LogFolderPath				_T("..\\Log")
// 系统日志文件夹（包含操作、警告、错误）
#define SysOptLogFolderPath			_T("\\系统运行日志")
// 时统日志文件夹（包含尾包时刻查询及时统设置应答及结果统计）
#define TimeDelayLogFolderPath		_T("\\时统日志")
// 误码查询日志文件夹（包含误码查询应答及结果统计）
#define ErrorCodeLogFolderPath		_T("\\误码查询日志")
// 帧时间和偏移量日志（包含丢帧、重发帧及失效帧结果统计）
#define ADCFrameTimeLogFolderPath	_T("\\采样数据帧时间及偏移量")
// ADC数据帧
#define ADCDataLogFolderPath		_T("\\采样数据")

// 输出选择:Debug输出则为0，Release输出则为1
#define OutPutSelect				0
// 输出错误日志上限
#define OutPutLogErrorLimit			100
// 日志文件单个文件输出信息条数
#define OutPutLogFileInfoNumLimit	5000
// 日志输出类型
enum{LogType, WarningType, ErrorType, ExpandType};
// 日志文件类型
enum{OptLogType, TimeDelayLogType, ErrorCodeLogType, ADCFrameTimeLogType};


// 日志输出结构
typedef struct LogOutPut_Struct
{
	// 日志输出资源同步对象
	CRITICAL_SECTION m_oSecLogFile;
	// 日志输出文件指针
	FILE* m_pFile;
	// 日志输出结构
	list<string> m_oLogOutputList;
	// 错误计数
	unsigned int m_uiErrorCount;
	// 日志输出路径
	string m_SaveLogFilePath;
	// 日志文件序号
	unsigned int m_uiLogFileNb;
	// 日志文件存储信息条数记数
	unsigned int m_uiLogInfoCount;
	// 日志文件类型
	byte m_byLogFileType;
}m_oLogOutPutStruct;
#endif