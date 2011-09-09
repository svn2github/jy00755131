#pragma once
#include "SocketShotBlaster.h"
#include "..\common\OperaStruct.h"
/******************************************************************************
    类    型：CShotController
    
    功    能：爆炸机控制器基类，解析爆炸机控制器的通信协议等。

	          其他爆炸机控制器应该从此类派生出来

    修改历史：

*******************************************************************************/
class CShotController
{
public:
	CShotController(void);
	virtual ~CShotController(void);

public:	
	/** 爆炸机控制器所连的采集站SN号*/
	DWORD	    m_dwFDUSN;
	/** 爆炸机控制器所连的采集站IP地址*/
	DWORD		m_dwFDUIP;
	/** 用于与爆炸机所连的采集站通信的Socket，实际类型为CSocketShotBlaster*/
	CSocketShotBlaster*		m_pHostSocket;

	/** 正在放炮的炮点 */
	DWORD		m_dwShotNb;
	/** 震源点测线 */
	float		m_fSourceLine;
	/** 震源点号 */
	float		m_fSourceNb;
	/** 震源点索引 */
	DWORD		m_dwSwathID;
	/** 爆炸机号 */
	DWORD		m_dwSourceNb;
	/** 爆炸机状态 */
	unsigned char m_byBlasterState; 
	/** 确认TB时刻,毫秒ms */
	float       m_fConfirmedTB;
	/** 井口时间,毫秒ms */
	float       m_fUpholeTime;
protected:
	BOOL		m_bNewState;
public:
	// 复位爆炸机状态
	void   Reset(void);
	// 判断爆炸机状态是否更新
	BOOL   IsNewState(void)	{ return m_bNewState;};
	// 设置爆炸机控制器所连的采集站SN号
	void SetFDUSN(DWORD dwSN);
	// 初始化爆炸机控制器所连的采集站
	int SendFDUConfigCmd(void);
	// 发送点火命令
	virtual int SendFireOrder(CShotClient* pShotClient);
	// 分析缓冲区数据
	virtual bool  AnalysisFrame(unsigned char* pBuf,int nSize);
	

	
};

/******************************************************************************
    类    型：CShotCtrlProII
    
    功    能：Shot Pro II 爆炸机控制器，解析爆炸机控制器的通信协议等。

    修改历史：

*******************************************************************************/
class CShotCtrlProII : public CShotController
{
public:// 属性
	

public:
	CShotCtrlProII(void);
	~CShotCtrlProII(void);

public:
	// 发送点火命令
	virtual int SendFireOrder(CShotClient* pShotClient);
	// 分析缓冲区数据
	virtual bool  AnalysisFrame(unsigned char* pBuf,int nSize);
private:
	bool AnalysisBlasterToHostFrame(unsigned char* pBuf,int nSize);
};
