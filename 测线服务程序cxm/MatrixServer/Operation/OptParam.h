#ifndef _OptParam_H
#define _OptParam_H
// 定义震源个数上限
#define ActiveSourceNumLimit	10
// 数据处理对象类型
#define PROCESS_IMPULSIVE		0		// 脉冲
#define PROCESS_STACKIMPULSIVE	1		// 脉冲叠加
#define PROCESS_CORRELAFTER		2		// 叠加后相关
#define PROCESS_CORRELBEFORE	3		// 叠加前相关
#define PROCESS_STACK			4		// 可控震源地震叠加
// 处理设置选项
#define PROCESS_STANDARD		0		// 标准
#define PROCESS_ADVANCE			1		// 高级
#define ControlsInterval		5		// 控件之间的间距
// 定义选择字符串
#define VIBROMOVING_STRING_SEQ	_T("SEQUIENTIAL")
#define VIBROMOVING_STRING_RND	_T("RANDOM")
#define SHOTBREAKPOINT_STRING_NO	_T("No")
#define SHOTBREAKPOINT_STRING_YES	_T("Yes")
#define ACQSIGNSTACK_STRING_PLUS	_T("plus")
#define ACQOUTPUT_STRING_NONE		_T("none")
#define ACQOUTPUT_STRING_DUMP		_T("dump")
#define ACQOUTPUT_STRING_XDUMP		_T("xdump")
// OutPut输出行数
#define OutPutStoreRowsLimit	200
/** C/S接收缓冲区大小*/
#define ClientRecBufferSize					8192000
/** C/S发送缓冲区大小*/
#define ClientSndBufferSize					8192000
/** 界面刷新定时器序号*/
#define AcqReloadTimerID			1
/** 界面刷新定时器时间*/
#define AcqReloadTime				1000
/** 界面刷新定时器序号*/
#define ActiveSourceReloadTimerID	1
/** 界面刷新定时器时间*/
#define ActiveSourceReloadTime		1000
/** 界面刷新定时器序号*/
#define AllVPReloadTimerID			1
/** 界面刷新定时器时间*/
#define AllVPReloadTime				1000
/** 界面刷新定时器序号*/
#define VPToDoReloadTimerID			1
/** 界面刷新定时器时间*/
#define VPToDoReloadTime			1000
/** 界面刷新定时器序号*/
#define VPDoneReloadTimerID			1
/** 界面刷新定时器时间*/
#define VPDoneReloadTime			1000
/** 将标题栏设置在居中位置，在前面添加空格*/
extern void MoveTitleToCenter(CWnd* pWnd);
#endif