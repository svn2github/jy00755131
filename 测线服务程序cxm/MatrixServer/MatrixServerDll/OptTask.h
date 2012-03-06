#include <list>
#include <hash_map>
#include "OptInstrument.h"
using std::list;
using stdext::hash_map;
using std::string;
#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _OptTask_H
#define   _OptTask_H
// 施工任务结构体
typedef struct OptTask_Struct
{
	// 任务是否使用中
	bool m_bInUsed;
	// 任务序号
	unsigned int m_uiIndex;
	// 施工任务开始记录的数据帧数
	unsigned int m_uiStartFrame;
	// 施工数据输出文件指针
	FILE* m_pFile;
	// 施工数据输出前一个文件的文件指针
	FILE* m_pPreviousFile;
	// 最新的文件存储序号
	unsigned int m_uiFileSaveNb;
	// 施工数据存储文件路径
	string m_SaveLogFilePath;
	// 施工任务索引表，关键字为SN，内容为行号
	hash_map<unsigned int, unsigned int> m_oSNMap;
	// 参与施工的仪器队列
	list<m_oOptInstrumentStruct> m_olsOptInstrument;
}m_oOptTaskStruct;
#endif