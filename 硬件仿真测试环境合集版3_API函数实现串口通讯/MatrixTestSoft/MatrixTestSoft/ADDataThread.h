#pragma once
#include "afxsock.h"
#include <vector>
#include "OScopeCtrl.h"
#include "Parameter.h"
#include <afxmt.h>
using namespace std;

// ADDataThread

class CADDataThread : public CWinThread
{
	DECLARE_DYNCREATE(CADDataThread)

public:
	CADDataThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CADDataThread();
	virtual int Run();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:

	// 初始化
	void OnInit(void);

	// 关闭并结束线程
	void OnClose(void);
	// 线程关闭标志
	bool m_bclose;
	// 线程结束事件
	HANDLE m_hADDDataThreadClose;
	// AD数据接收Socket
	CSocket m_ADDataSocket;
	
	// 打开
	byte ADData[256];
	void OnOpen(void);
	// 停止
	void OnStop(void);
	// 接收采样数据
	void ReceiveSampleData(byte* pSampleData);
	// 选择仪器对象
	int* m_pSelectObject;
	// 选择仪器对象做噪声监测
	int* m_pSelectObjectNoise;
	// 采集到的数据
	vector <double>*m_pSampleData[GraphViewNum];
	// 图形模板指针
	COScopeCtrl* m_pOScopeCtrl[GraphViewNum];
	// 定义临界区变量
	CCriticalSection m_Sec;
};


