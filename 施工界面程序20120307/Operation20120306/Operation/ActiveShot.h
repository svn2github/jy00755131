#pragma once
#include "..//common/OperaStruct.h"
#include "..//common/FDUConfig.h"
#include "..\common\DiskRecordConfig.h"
#include "ShotSpreads.h"

/**
 * @class CActiveShot
 * @brief 放炮属性封装类,放炮管理对象类
 *
 * 一次放炮所需的全部对象的封装类，负责管理炮号、震源、采集排列、
 * 处理类型以及完成此次放炮的施工服务器信息。
 * 该对象仅仅是封装管理正在放炮所需的对象指针，这些对象并不由CActiveShot来释放资源。
 * 而是由主框架中的对象集合类来释放。
 */
class CActiveShot
{
public:
	CActiveShot(void);
	~CActiveShot(void);

// 属性
public:
	/** 当前放炮的炮点对象 */
	CShotPoint*			m_pShotPoint;
	/** 当前放炮所使用的震源对象 */
	CShotSource*		m_pShotSource;
	/** 当前放炮所使用的采集排列对象 */
	CShotSpread*		m_pSpread;
	/** 当前放炮所使用的处理类型对象 */
	CProcessType*       m_pProcessType;
	/** 与施工服务器程序通信的Socket指针 */
	// cxm 2012.3.7
	CSocket*			m_pSvrSocket;
	/** 测线程序定义的所有辅助道 */
	CAuxChannels*		m_pAllAuxChannels;
	/** 施工服务器的IP地址,初始化为广播地址 */
	DWORD				m_dwServerIP;	
	/** 施工服务程序与客户端通信的网络端口 */
	WORD                m_wServerPort;	
	 /** 服务器端线程ID */
	DWORD				m_dwThreadID;		
	/**  施工客户端与服务器之间通信的标识号 */
	DWORD				m_dwServerID;		
	/** 用于数据重发的定时器*/
	DWORD				m_dwTimerID;	
	/** 数据帧重发计数器 */	
	DWORD				m_dwRetransCount;	
	/** 采集排列的第一条测线 */
	unsigned long  m_dwSpreadSFL;

	// --------爆炸机参数----------------------
	/** 爆炸机参数是否更新*/
	BOOL		m_bNewBlaster;
	/** 爆炸机状态 */
	unsigned char m_byBlasterState; 
	/** 确认TB时刻,毫秒ms */
	float       m_fConfirmedTB;
	/** 井口时间,毫秒ms */
	float       m_fUpholeTime;
	/** 是否发生ITB错误*/
	BOOL		m_bITBErr;
private:
	/** 炮点编号 */	
	DWORD			    m_dwShotNb;
	/** 震源编号 */
	DWORD			    m_dwSourceNb;
	/** 排列编号 */
	DWORD               m_dwSpreadNb;
	/** 处理类型编号 */
	DWORD               m_dwProcessNb;	
	
// 操作
public:
	/// 炮点Nb
	DWORD GetShotPointNb(void)		{ return m_dwShotNb; };
	/// 震源Nb
	DWORD GetShotSourceNb(void)		{ return m_dwSourceNb; };
	// 设置炮号、采集排列、处理类型
	void SetShotPoint(CShotPoint* pShotPoint,CShotSpread* pSpread,CProcessType* pProcess);
	// 设置震源
	void SetShotSource(CShotSource* pShotSrc,CAuxChannels* pAuxis);
	// 通过炮号、震源编号判断是否为该对象
	bool IsActiveShot(DWORD dwShotNb,DWORD dwSourceNb );
	
	int  SendToServer(unsigned char* pBuf,int nSize);
	//void ParseServerData(unsigned char* pBuf,int nSize);

	// 发送FDU配置信息
	int SendFDUConfig(CFDUConfig* pFDUConfig,CDiskRecordConfig* pDiskRecord);
	// 广播发送请求放炮服务
	int  SendRequestShotService(void);
	// 向服务器发送采集排列
	int  SendShotSpreadLabel(void);
	//int  SendShotSpreadChannel(void);
	int  SendShotSpreadChannel(CShotSpreads* m_AllSpreads,unsigned long m_dwSpreadSFL);
	// 发送炮点及震源对象
	int  SendShotPointAndSource(void);
	// 发送炮点及震源对象
	int  SendProcessType(void);
	// 发送一些通用的命令
	int  SendGeneralCMD(WORD  wCmdCode);

};
/**
 * @class CActiveShots
 * @brief 激活放炮对象的集合类
 *
 * 准备放炮对象的集合类
 */
class CActiveShots
{
public:
	// 正在放炮的炮号、震源索引
	CArray<CActiveShot*,CActiveShot*>	m_arrActiveShot;
public:
	CActiveShots(void);
	~CActiveShots(void);

	// 得到指定索引的炮号，索引从0开始
	CActiveShot* GetActiveShot(int iIndex);
	int GetCount(void);
	int Add(CActiveShot* pActiveShot);
	int FindFirstActiveShotBySource(CShotSource* pSource);
	CActiveShot*  FindActiveShotByNb(DWORD dwShotNb,DWORD dwSourceNb);
	int  FindActiveShotByID(DWORD dwServerID,DWORD dwThreadID);
	void  RemoveActiveShotAt(int iIndex);


};
