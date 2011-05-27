#pragma once
#include <vector>
#include "OScopeCtrl.h"
#include "Parameter.h"
#include <afxmt.h>


// CADCDataRecThread

class CADCDataRecThread : public CWinThread
{
	DECLARE_DYNCREATE(CADCDataRecThread)

public:
	CADCDataRecThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CADCDataRecThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();

public:
	// 线程结束事件
	HANDLE m_hADCDataThreadClose;
	// AD数据接收Socket
	CSocket m_ADCDataSocket;
	// 线程关闭标志
	bool m_bclose;
	// ADC接收数据缓冲区
	byte ADCData[256];
	// 选择仪器对象
	int* m_pSelectObject;
	// 选择仪器对象做噪声监测
	int* m_pSelectObjectNoise;
	// 图形模板指针
	COScopeCtrl* m_pOScopeCtrl;
	// 定义临界区变量
	CCriticalSection m_Sec;
	// 记录前一个接收到的ADC数据的指针偏移量                  
	unsigned short m_usDataPointPrevious[GraphViewNum];
	// 记录前一次接收到的ADC数据帧的本地时间
	unsigned int m_uiFDUSystimePrevious[GraphViewNum];
	// 记录应接收ADC数据包个数
	unsigned int m_uiADCDataFrameCount[GraphViewNum];
	// 记录实际接收ADC数据包个数
	unsigned int m_uiADCDataFrameRecCount[GraphViewNum];
	// 判断ADC采样开始标志位，用于丢弃TB之前的无效数据
	BOOL m_bStartSample;
	// ADC数据重发帧
	unsigned char m_ucADCRetransmission[128];
	// 源IP地址
	CString m_csIPSource;
	// 发送端口
	unsigned int m_uiSendPort;
	// 需要重发的ADC数据信息结构
	typedef struct m_structADC
	{
		unsigned int uiIPAim;			// 重发帧IP地址
		unsigned short usDataPointNb;	// 重发帧的指针偏移量
		unsigned int uiDataCount;		// 重发帧所对应的帧位置（个数）
		unsigned short uiRetransmissionNum;	// 重发计数
		BOOL operator == (const m_structADC&   tdl)const
		{ 

			if(uiIPAim != tdl.uiIPAim) 
				return   FALSE; 
			if(usDataPointNb != tdl.usDataPointNb) 
				return   FALSE; 
			return   TRUE; 
		} 
	};
	// 需要重发的ADC数据链表
	CList <m_structADC, m_structADC&> m_listADC;
	// 有ADC数据重发故先保留不连续的数据
	vector <double> m_dSampleTemp[GraphViewNum];
	// 测试丢帧情况
	unsigned int m_uiTestADCLost;
	// ADC数据保存成文件的数据缓冲区
	vector <double> m_dADCSave[GraphViewNum];
	// 保存数据到文件的存储路径
	CString m_csSaveFilePath;
	// 保存ADC数据文件个数
	unsigned int m_uiADCSaveFileNum;
	// 保存文件的文件指针
	FILE * m_pFileSave;
	// 文件打开标志位
	BOOL m_bOpenADCSaveFile;
	// 选中的仪器对象名称
	char* m_cSelectObjectName[GraphViewNum];
	// 被保存的ADC数据个数
	unsigned int m_uiADCDataToSaveNum;
	// ADC数据保存文件尺寸
	unsigned int m_uiADCFileLength;
	// ADC图形显示设备IP
	unsigned int m_uiADCGraphIP;
	// ADC数据界面显示缓冲区
	double m_dADCDataShow[GraphViewNum];
	// 记录采样开始时的指针偏移量
	unsigned short m_usDataPointStart[GraphViewNum];
	// 记录采样停止时的指针偏移量
	unsigned short m_usDataPointStop[GraphViewNum];
	// ADC帧保存文件的文件指针
	FILE* m_pFileDataPoint[GraphViewNum];
	// ADC接收到的数据重发帧的序号
	unsigned int m_uiADCRetransmissionNb;

public:
	// 初始化
	void OnInit(void);
	// 关闭并结束线程
	void OnClose(void);
	// 打开
	void OnOpen(void);
	// 停止
	void OnStop(void);
	// 接收采样数据
	void ReceiveSampleData(byte* pSampleData);
	// 生成ADC数据重发帧
	void MakeADCDataRetransmissionFrame(unsigned short usDataPointNb, unsigned int uiIPAim);
	// ADC数据需要重新发送帧对应的最小个数
	unsigned int OnADCRetransimissionMinNb(void);
	// 创建并打开ADC保存数据文件
	void OnOpenADCSaveFile(void);
	// 关闭ADC保存数据文件
	void OnCloseADCSaveFile(void);
	// 将ADC数据保存到文件中
	void OnSaveADCToFile(void);
	// 被选择仪器的ADC数据个数的最小值
	unsigned int OnADCRecDataMinNum(void);
	// 关闭采样后保存剩余数据
	void OnSaveRemainADCData(unsigned int uiADCDataNeedToSave);
	// ADC数据重发
	void OnADCDataRetransmission(void);
	// 打开ADC帧指针偏移量文件
	void OnOpenDataPointFile(unsigned int uiInstrumentNb);
	// 关闭ADC帧指针偏移量文件
	void OnCloseDataPointFile(unsigned int uiInstrumentNb);
	// 记录ADC帧指针偏移量到文件
	void OnSaveDataPointFile(unsigned int uiInstrumentNb, CString strOutput);
	// 检查接收帧是否为重发帧
	BOOL OnCheckFrameIsRetransmission(unsigned int uiInstrumentNb, unsigned int uiIPAim, unsigned short usDataPointNow, unsigned int uiFDUSystimeNow);
	// 该帧为最后一个数据重发帧
	void OnRecOkIsRetransimissionFrame(unsigned int uiInstrumentNb, unsigned int uiIPAim, double* pReceiveData);
	// 该帧为普通帧且之前没有重发帧
	void OnRecOkIsNormalFrame(unsigned int uiInstrumentNb, unsigned int uiIPAim, double* pReceiveData);
	// 该帧为重发帧但不是最后一个重发帧
	void OnRecNotOkIsRetransimissionFrame(unsigned int uiInstrumentNb, unsigned int uiIPAim, double* pReceiveData);
	// 该帧为普通帧且之前有重发帧
	void OnRecNotOkIsNormalFrame(unsigned int uiInstrumentNb, double* pReceiveData);
	// 设置缓冲区尺寸
	BOOL OnSetBufferSize(int iBufferSize);

};


