#include "StdAfx.h"
#include "Operation.h"
#include "MainFrm.h"
#include "ActiveShot.h"

/**
 * @brief CActiveShot构造函数
 */
CActiveShot::CActiveShot(void)
: m_pShotPoint(NULL)
, m_pShotSource(NULL)
, m_pSpread(NULL)
, m_pProcessType(NULL)
, m_pSvrSocket(NULL)
, m_dwShotNb(0)
, m_dwSourceNb(0)
, m_dwServerID(0)
, m_dwTimerID(1)
, m_dwRetransCount(0)
, m_pAllAuxChannels(NULL)
{
	m_dwThreadID = 0;
	m_dwServerIP = 0xFFFFFFFF;
	m_wServerPort = OPERATION_SVRPORT;

	m_byBlasterState = 0; 	
	m_fConfirmedTB = 0;
	m_fUpholeTime = 0;
	m_bITBErr = FALSE;
	m_bNewBlaster = FALSE;
}
/**
 * @brief CActiveShot析构函数
 */
CActiveShot::~CActiveShot(void)
{
}
/**
 * @note 设置当前激活的炮点及属性对象；
 * @note 在创建激活对象后，需要调用该函数设置炮点及该炮点所对应的采集排列、处理类型等属性。 
 * @param CShotPoint*   pShotPoint 准备放炮的炮点对象
 * @param CShotSpread*  pSpread    炮点所对应的采集排列
 * @param CProcessType* pProcess   炮点所采样的处理类型
 * @return void
 */
void CActiveShot::SetShotPoint(CShotPoint* pShotPoint,CShotSpread* pSpread,CProcessType* pProcess)
{ 
	m_pShotPoint = pShotPoint;
	m_pSpread = pSpread;
	m_pProcessType = pProcess;
	if(m_pShotPoint)
	{
		m_dwShotNb = m_pShotPoint->m_dwShotNb;
		m_dwSpreadNb =m_pShotPoint->m_dwSpreadNb;
		m_dwProcessNb=m_pShotPoint->m_dwProcessNb;
	}
	else
	{
		m_dwShotNb = 0;
		m_dwSpreadNb = 0;
		m_dwProcessNb=0;
	}
}
/**
 * @note  设置放炮所使用的震源对象 
 * @param CShotSource* pShotSrc 震源对象
 * @param CAuxChannels* pAuxis,测线程序定义的辅助道
 * @return void
 */
void CActiveShot::SetShotSource(CShotSource* pShotSrc,CAuxChannels* pAuxis)
{
	m_pShotSource = pShotSrc; 
	m_pAllAuxChannels = pAuxis;
	if(m_pShotSource)
		m_dwSourceNb = m_pShotSource->m_dwSourceNb;
	else
		m_dwSourceNb = 0;
}
/**
 * @brief 是否是当前激活对象
 * @note  从网络帧格式中解析出炮点编号和震源编号后，通过这两个参数判断是否为该对象。
 * @param DWORD dwShotNb   炮点编号
 * @param DWORD dwSourceNb 震源编号
 * @return 如果炮号和震源号与当前对象保存的炮号和震源号都相等，则返回true；否则返回false。
 */
bool CActiveShot::IsActiveShot(DWORD dwShotNb,DWORD dwSourceNb )
{
	if(  m_dwSourceNb!=dwSourceNb || m_dwShotNb!=dwShotNb )
		return false;
	else
		return true;
}

/**
 * @brief 向服务器发送数据
 * @note  该函数封装服务器的IP地址及端口转换，被其它命令发送函数所调用，向施工服务器发送信息 。
 * @param unsigned char*  pBuf   数据缓冲区
 * @param int nSize 数据缓冲区字节大小
 * @return 返回已发送的字节数。
 */
int  CActiveShot::SendToServer(unsigned char* pBuf,int nSize)
{
	if(NULL==m_pSvrSocket)
		return -1;
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = m_dwServerIP;
	service.sin_port = htons(m_wServerPort);
	return  m_pSvrSocket->SendTo(pBuf,nSize,(SOCKADDR*)&service,sizeof(sockaddr_in));	
}
/*
*****************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 解析来自服务器的数据
	          
			   被CMainFrame类的AnalysisProtocolOfServerSock函数调用

			   数据帧格式：
			   帧头（2Byte）＋帧长度（2Byte）  ＋帧计数（4Byte）＋帧类型（1Byte）			   
			   ＋命令(2Byte)＋数据个数（2Byte）＋数据区（变长） ＋校验位（1Byte）＋帧尾（2Byte）

    修改历史：

******************************************************************************
void CActiveShot::ParseServerData(unsigned char* pBuf,int nSize)
{	
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	WORD		wFrmCmdCode;	// 命令码
	wFrmCmdCode=MAKEWORD(pBuf[9],pBuf[10]);
	switch(wFrmCmdCode)
	{
	case OPERATION_CMD_REQUEST:		// 请求放炮服务获得应答
		{
			if(m_dwServerID)
				return;
			pBuf[4] = 0x01;		// 帧计数
			pBuf[8] = 0x01;		// 帧类型
			pBuf[9] = LOBYTE(OPERATION_CMD_CONFIRM);	// 命令码低位
			pBuf[10]= HIBYTE(OPERATION_CMD_CONFIRM);	// 命令码高位
			SendToServer(pBuf,nSize);
			pFrm->WriteLog(_T(" Request Acked!"));
			return;
		}
		break;
	case OPERATION_CMD_CONFIRM:		// 确认放炮服务获得应答
		{	
			// 服务器提供的通信ID
			memcpy_s(&m_dwServerID,4,pBuf+25,4);		
			// 线程ID
			memcpy_s(&m_dwThreadID,4,pBuf+29,4);
			// 发送炮号及 震源
			SendShotPointAndSource();
			// 发送处理类型对象
			SendProcessType();
			// 发送采集排列
			SendShotSpread();
			// 启动放炮
			SendGeneralCMD(OPERATION_CMD_START);
			pFrm->WriteLog(_T("Confirm Ack!"));
			return;
		}
		break;
	case OPERATION_ACK_NAME:		// 点名无应答
		{
			// 解析出采集排列总数，无应答的个数
			DWORD  dwNoAck;
			memcpy_s(&dwNoAck,4,pBuf+21,4);
			pFrm->WriteLog(_T("Named Ack!"));
			CString str;
			str.Format(_T("采集排列总数：%d，无应答道数: %d,是否继续？"),m_pSpread->GetCount(), dwNoAck);
			if(pFrm->MessageBox(str,_T("警告"),MB_YESNO|MB_ICONQUESTION)==IDYES)
			{
				// 确认放炮
				
			}
		}
		break;
	default:
		break;
	}
}*/

/**
 * @brief 通用命令发送函数
 * @note  在施工服务器和施工客户端通信过程中，有部分的命令帧格式相同，仅仅命令代码不同，采用该通用函数向施工服务器发送数据。
 * @note  帧格式为:
 * @note  帧头（2Byte）＋帧长度（2Byte）＋帧计数（4Byte）＋帧类型（1Byte）＋命令码(2Byte)＋数据个数（2Byte）＋数据区（通信编号ID(4byte)和线程索引号(4byte)）＋校验位（1Byte）＋帧尾（2Byte）
 * @param  WORD  wCmdCode 命令码
 * @return 返回已发送的字节数。
 */
int  CActiveShot::SendGeneralCMD(WORD  wCmdCode)
{
	if(!m_pShotSource ||!m_pShotPoint || !m_pSvrSocket)
		return -1;
	unsigned char		pBuf[26];
	// 帧头
	pBuf[0] = 0xEB;
	pBuf[1] = 0x90;
	// 帧长
	pBuf[2] = 24;
	pBuf[3] = 0;
	// 帧计数
	memset(pBuf+4,0,4);
	// 帧类型
	pBuf[8] = 0x01;
	// 命令码
	pBuf[9]  = LOBYTE(wCmdCode);
	pBuf[10] = HIBYTE(wCmdCode);
	// 数据个数
	pBuf[11] =0x08;
	pBuf[12] =0;
	// 炮号、震源
	memcpy_s(pBuf+13,4,&m_dwServerID,4);
	memcpy_s(pBuf+17,4,&m_dwThreadID,4);
	// 帧校验
	pBuf[21]=0;
	// 帧尾
	pBuf[22] = 0x14;
	pBuf[23] = 0x6F;

	return SendToServer(pBuf,24);
}
/**
 * @brief 发送放炮请求函数
 * @note  在选定炮号和震源后，向施工服务器发送放炮请求，施工服务程序接收到请求后,寻找空闲线程。
 * @note  调用该函数前，应将服务器IP地址变量m_dwServerIP设为广播地址。
 * @param  void
 * @return 返回已发送的字节数。
 */
int  CActiveShot::SendRequestShotService()
{
	if(!m_pShotSource ||!m_pShotPoint || !m_pSvrSocket)
		return -1;
	unsigned char		pBuf[36];
	// 帧头
	pBuf[0] = 0xEB;
	pBuf[1] = 0x90;
	// 帧长
	pBuf[2] = 36;
	pBuf[3] = 0;
	// 帧计数
	memset(pBuf+4,0,4);
	// 帧类型
	pBuf[8] = 0x01;
	// 命令码
	pBuf[9]  = LOBYTE(OPERATION_CMD_REQUEST);
	pBuf[10] = HIBYTE(OPERATION_CMD_REQUEST);
	// 数据个数
	pBuf[11] =20;
	pBuf[12] =0;
	// 炮号、震源
	memcpy_s(pBuf+13,4,&m_dwShotNb,4);
	memcpy_s(pBuf+17,4,&m_dwSourceNb,4);
	// 采集排列
	memcpy_s(pBuf+21,4,&m_dwSpreadNb,4);
	// 处理类型
	memcpy_s(pBuf+25,4,&m_dwProcessNb,4);
	// 线程，保留该区域，由服务器填充
	memset(pBuf+29,0,4);

	// 帧校验
	pBuf[33]=0;
	// 帧尾
	pBuf[34] = 0x14;
	pBuf[35] = 0x6F;

	return SendToServer(pBuf,36);
}

int  CActiveShot::SendFDUConfig(CFDUConfig* pFDUConfig,CDiskRecordConfig* pDiskRecord)
{
	if(!pFDUConfig || !pDiskRecord)
		return -1;
	CString		strNormalFilePath;
	// 正常采集数据文件存盘路径
	strNormalFilePath = pDiskRecord->GetNormalFilePath();
	int nNormalLen = WORD(strNormalFilePath.GetLength()*sizeof(TCHAR));
	CString     strTestFilePath;
	// 测试采集数据文件存盘路径
	strTestFilePath = pDiskRecord->GetTestFilePath();
	int nTestLen = WORD(strTestFilePath.GetLength()*sizeof(TCHAR));

	unsigned char* pBuf=new unsigned char[56+nNormalLen+nTestLen];
	// 帧头
	pBuf[0] = 0xEB;
	pBuf[1] = 0x90;
	// 帧长
	pBuf[2] = 56+nNormalLen+nTestLen;
	pBuf[3] = 0;
	// 帧计数
	memset(pBuf+4,0,4);
	// 帧类型
	pBuf[8] = 0x02;
	// 命令码
	pBuf[9]  = LOBYTE(OPERATION_CMD_FDUCONF);
	pBuf[10] = HIBYTE(OPERATION_CMD_FDUCONF);
	// 数据个数
	pBuf[11] =2;
	pBuf[12] =0;
	// 炮号、震源
	memcpy_s(pBuf+13,4,&m_dwServerID,4);
	memcpy_s(pBuf+17,4,&m_dwThreadID,4);
	// 采集站配置信息
	memcpy_s(pBuf+21,4,&pFDUConfig->m_dwGain3301Index,4);	
	memcpy_s(pBuf+25,4,&pFDUConfig->m_dwMux3301Index,4);	
	memcpy_s(pBuf+29,4,&pFDUConfig->m_dwMode5373Index,4);
	memcpy_s(pBuf+33,4,&pFDUConfig->m_dwAttr5373Index,4);
	memcpy_s(pBuf+37,4,&pFDUConfig->m_dwSampleRateIndex,4);
	memcpy_s(pBuf+41,4,&pFDUConfig->m_dwFilterIIRIndex,4);
	memcpy_s(pBuf+45,4,&pFDUConfig->m_dwFilterFIRIndex,4);
	
	// 正常采集数据文件存盘路径
	memcpy_s(pBuf+49,nNormalLen, LPCTSTR(strNormalFilePath),nNormalLen);
	pBuf[49+nNormalLen]='\0';	// 结束符
	pBuf[50+nNormalLen]='\0';
	
	// 测试采集数据文件存盘路径
	memcpy_s(pBuf+51+nNormalLen,nTestLen, LPCTSTR(strTestFilePath),nTestLen);
	pBuf[51+nNormalLen+nTestLen]='\0';	// 结束符
	pBuf[52+nNormalLen+nTestLen]='\0';

	// 帧校验
	pBuf[53+nNormalLen+nTestLen]=0;
	// 帧尾
	pBuf[54+nNormalLen+nTestLen] = 0x14;
	pBuf[55+nNormalLen+nTestLen] = 0x6F;
	
	nNormalLen = SendToServer(pBuf,56+nNormalLen+nTestLen);
	delete []pBuf;
	return nNormalLen; 
}
/**
 * @brief 发送采集排列的编号、标签等属性
 * @note  与施工服务程序建立连接后，在开始放炮前，向服务器下发采集排列。
 发送采集排列分为两步：先调用该函数发送采集排列的编号、标签等属性，然后调用SendShotSpreadChannel
 函数发送采集道。 
 * @note  帧格式为：帧头（2Byte）＋帧长度（2Byte）  ＋帧计数（4Byte）＋帧类型（1Byte）			   
 ＋命令(2Byte)＋数据个数（2Byte）＋数据区（变长） ＋校验位（1Byte）＋帧尾（2Byte）。
 * @note  在帧格式的数据区包括：
            第一部分: 唯一编号ID（4byte），线程索引号（4byte） ,
                      采集道编号（4Byte），采集道类型（1Byte），采集道标签（多字节字符串）;
			第二部分：紧挨着第一部分之后，传送辅助道信息字节数2+20*m，
			 		  包括辅助道个数m（2Byte），每个辅助道的编号Nb（4Byte）、序列号SN（4Byte）、
					  占地震道道号ChannelNb（4Byte）、IP地址（4Byte）、增益（4Byte）
 * @param  void
 * @return 返回已发送的字节数。
 */
int   CActiveShot::SendShotSpreadLabel()
{
	if(!m_pShotSource ||!m_pShotPoint|| !m_pSpread || !m_pSvrSocket || !m_pAllAuxChannels)
		return -1;
	// 采集排列标签字符串的字节长度
	WORD wLabelLen = WORD(m_pSpread->m_strLabel.GetLength()*sizeof(TCHAR));
	WORD wAuxiCount = WORD(m_pAllAuxChannels->GetCount());
	// 帧的总长度 
	WORD wFrmLen =wLabelLen+16+13+2+ wAuxiCount*20+2;
		
	unsigned char*		pBuf=new unsigned char[wFrmLen];
	// 帧头
	pBuf[0] = 0xEB;
	pBuf[1] = 0x90;
	// 帧长
	pBuf[2] = LOBYTE(wFrmLen);
	pBuf[3] = HIBYTE(wFrmLen);
	// 帧计数
	memset(pBuf+4,0,4);
	// 帧类型,不需要回令
	pBuf[8] = 0x02;
	// 命令码
	pBuf[9]  = LOBYTE(OPERATION_CMD_SPREAD);
	pBuf[10] = HIBYTE(OPERATION_CMD_SPREAD);
	// 数据个数: 13Byte+ 字符串长度+2个‘\0’字符 + wAuxiCount*20+2
	pBuf[11] =LOBYTE(15+wLabelLen+ wAuxiCount*20+2);
	pBuf[12] =HIBYTE(15+wLabelLen+ wAuxiCount*20+2);
	// 服务器ID、线程ID
	memcpy_s(pBuf+13,4,&m_dwServerID,4);
	memcpy_s(pBuf+17,4,&m_dwThreadID,4);
	// 采集排列编号
	memcpy_s(pBuf+21,4,&m_pSpread->m_dwNb,4);
	// 采集排列类型
	pBuf[25] = m_pSpread->m_bySpreadType;
	// 采集排列标签
	memcpy_s(pBuf+26,wLabelLen, LPCTSTR(m_pSpread->m_strLabel),wLabelLen);
	pBuf[26+wLabelLen]='\0';	// 结束符
	pBuf[27+wLabelLen]='\0';

	// ------------保存第二部分数据：辅助道数据-----------------
	pBuf[28+wLabelLen] = LOBYTE(wAuxiCount);
	pBuf[29+wLabelLen] = HIBYTE(wAuxiCount);
	CAuxChannel* pAuxi=NULL;
	for(int i=0;i<wAuxiCount;i++)
	{	
		pAuxi = m_pAllAuxChannels->GetAuxChannel(i);
		// 辅助道Nb
		memcpy_s(pBuf+30+wLabelLen+i*20,4,&pAuxi->m_dwNb,4);
		// 辅助道SN
		memcpy_s(pBuf+34+wLabelLen+i*20,4,&pAuxi->m_dwSN,4);
		// 辅助道道号
		memcpy_s(pBuf+38+wLabelLen+i*20,4,&pAuxi->m_dwChannelNb,4);
		// 辅助道IP
		memcpy_s(pBuf+42+wLabelLen+i*20,4,&pAuxi->m_dwIP,4);
		// 辅助道增益
		memcpy_s(pBuf+46+wLabelLen+i*20,4,&pAuxi->m_dwGain,4);		
	}
	
	// 帧校验
	pBuf[30+wLabelLen+20*wAuxiCount]=0;
	// 帧尾
	pBuf[31+wLabelLen+20*wAuxiCount] = 0x14;
	pBuf[32+wLabelLen+20*wAuxiCount] = 0x6F;

	SendToServer(pBuf,wFrmLen);
	delete []pBuf;
	return wFrmLen;

}
/**
 * @brief 发送采集排列函数
 * @note  与施工服务程序建立连接后，在开始放炮前，调用该函数，向服务器下发采集排列。
 * @note  该函数内部每次发送1000道，循环多次发送直到采集排列发送完毕。
 * @note  帧格式为：帧头（2Byte）＋帧长度（2Byte）  ＋帧计数（4Byte）＋帧类型（1Byte）			   
 ＋命令(2Byte)＋数据个数（2Byte）＋数据区（变长） ＋校验位（1Byte）＋帧尾（2Byte）。
 * @note  在帧格式的数据区包括：唯一编号ID（4byte），线程索引号（4byte） ,采集道属性（21Byte）。
		  其中采集道属性为一组，可以有n组。
		  帧格式的总长度为 16+8+21*n，其中n为采集道的总数
 * @param  void
 * @return 返回已发送的字节数。
 */
int   CActiveShot::SendShotSpreadChannel()
{
	if(!m_pShotSource ||!m_pShotPoint|| !m_pSpread || !m_pSvrSocket)
		return -1;

	DWORD	dwFrmIndex=1;
	int		i;
	INT_PTR		nCount = m_pSpread->GetCount(); 
	WORD	wLen; 
	unsigned char* pBuf=new unsigned char[21024];		// 可以一次发送1000个采集排列
	SHOTSPREAD*	pChannel=NULL;

	// 每次发送3000个采集道，需要发送多少次
	int  nCycleNum = int(nCount/1000);
	int  nIndex;

	// 帧头
	pBuf[0] = 0xEB;
	pBuf[1] = 0x90;
	// 帧长,每次固定的道数
	pBuf[2] = LOBYTE(21024);
	pBuf[3] = HIBYTE(21024);
	
	// 帧类型,不需要回令
	pBuf[8] = 0x02;
	// 命令码
	pBuf[9]  = LOBYTE(OPERATION_CMD_CHANNEL);
	pBuf[10] = HIBYTE(OPERATION_CMD_CHANNEL);
	// 服务器ID、线程ID
	memcpy_s(pBuf+13,4,&m_dwServerID,4);
	memcpy_s(pBuf+17,4,&m_dwThreadID,4);
	for(i=0;i<nCycleNum;i++)
	{		
		// 帧计数
		memcpy_s(pBuf+4,4,&dwFrmIndex,4);
		dwFrmIndex++;
		// 采集道数量，不是字节数
		pBuf[11] = LOBYTE(1000);
		pBuf[12] = HIBYTE(1000);
			
		for(nIndex=0;nIndex<1000;nIndex++)
		{
			pChannel = &(m_pSpread->m_arrSpread[nIndex+i*1000]);
			memcpy_s(pBuf+21+21*nIndex,4,&(pChannel->m_dwIP),4);
			memcpy_s(pBuf+25+21*nIndex,4,&(pChannel->m_dwGain),4);
			memcpy_s(pBuf+29+21*nIndex,4,&(pChannel->m_dwLineNb),4);
			memcpy_s(pBuf+33+21*nIndex,4,&(pChannel->m_dwPointNb),4);
			memcpy_s(pBuf+37+21*nIndex,4,&(pChannel->m_dwChannelNb),4);			
			pBuf[41+21*nIndex] = pChannel->m_byType;
		}		
		// 帧校验
		pBuf[21021]=0;
		// 帧尾
		pBuf[21022] = 0x14;
		pBuf[21023] = 0x6F;
		SendToServer(pBuf,21024);
	}

	nIndex = nCount%1000;	// 计算最后剩余的采集道
	if (nIndex<=0)
	{
		delete []pBuf;
		return 21024;
	}
	// 计算最后一帧的长度,最后一帧不是1000道
	wLen = 24+21*nIndex;		 
	pBuf[2] = LOBYTE(wLen);
	pBuf[3] = HIBYTE(wLen);

	// 帧计数
	memcpy_s(pBuf+4,4,&dwFrmIndex,4);
	// 采集道数量，不是字节数
	// wLen = 8+21*nIndex;
	pBuf[11] = LOBYTE(nIndex);
	pBuf[12] = HIBYTE(nIndex);
	
	for(i=0;i<nIndex;i++)
	{
		pChannel =&( m_pSpread->m_arrSpread[i+nCycleNum*1000] );
		memcpy_s(pBuf+21+21*i,4,&(pChannel->m_dwIP),4);
		memcpy_s(pBuf+25+21*i,4,&(pChannel->m_dwGain),4);
		memcpy_s(pBuf+29+21*i,4,&(pChannel->m_dwLineNb),4);
		memcpy_s(pBuf+33+21*i,4,&(pChannel->m_dwPointNb),4);
		memcpy_s(pBuf+37+21*i,4,&(pChannel->m_dwChannelNb),4);			
		pBuf[41+21*i] = pChannel->m_byType;
	}		
	// 帧校验
	pBuf[21+21*i]=0;
	// 帧尾
	pBuf[21+21*i+1] = 0x14;
	pBuf[21+21*i+2] = 0x6F;
	nIndex = SendToServer(pBuf,24+21*nIndex);
	delete []pBuf;
	return nIndex;

}

/**
 * @brief 发送炮点和震源函数
 * @note  与施工服务程序建立连接后，在开始放炮前，调用该函数向服务器下发炮点属性及震源属性。
 * @note  帧格式为：帧头（2Byte）＋帧长度（2Byte）  ＋帧计数（4Byte）＋帧类型（1Byte）			   
 ＋命令(2Byte)＋数据个数（2Byte）＋数据区（变长） ＋校验位（1Byte）＋帧尾（2Byte）。
 * @note  在帧格式的数据区包括：服务器ID（4Byte），线程ID（4Byte），CShotPoint对象（多字节）,CShotSource对象（多字节）。
 * @note  在存储对象的属性中，每个字符串的后面都增加两个空结束符'\0'。
 * @note  对于爆炸机震源对象，依据类型不同存储对象属性不同，解析时也要按照类型来创建不同对象。
 * @param  void
 * @return 返回已发送的字节数。
 */
int  CActiveShot::SendShotPointAndSource()
{
	if(!m_pShotSource ||!m_pShotPoint || !m_pSvrSocket)
		return -1;

	unsigned char*		pBuf;
	WORD wFrmLen;	
	WORD wPointLen;
	WORD wLabelLen,wCommentLen; 
	LPCTSTR lpBuf;
	
	// 计算数据长度
	wPointLen = WORD(m_pShotPoint->m_szComments.GetLength()*sizeof(TCHAR));
	wLabelLen = WORD(m_pShotSource->m_strLabel.GetLength()*sizeof(TCHAR));
	wCommentLen =WORD(m_pShotSource->m_strComment.GetLength()*sizeof(TCHAR));
	wFrmLen = 97+wPointLen+wLabelLen+wCommentLen+4;
	pBuf = new unsigned char[wFrmLen + 9];
	if(!pBuf)
		return -1;
	// 帧头
	pBuf[0] = 0xEB;
	pBuf[1] = 0x90;
	
	// 帧计数
	memset(pBuf+4,0,4);
	// 帧类型
	pBuf[8] = 0x01;
	// 命令码
	pBuf[9]  = LOBYTE(OPERATION_CMD_SHOT);
	pBuf[10] = HIBYTE(OPERATION_CMD_SHOT);
	
	// 服务器ID，线程ID
	memcpy_s(pBuf+13,4,&m_dwServerID,4);
	memcpy_s(pBuf+17,4,&m_dwThreadID,4);
	// 存储CShotPoint对象		
	pBuf[21] = m_pShotPoint->m_VPState;
	pBuf[22] = m_pShotPoint->m_byBreakPoint;
	memcpy_s(pBuf+23,4,&(m_pShotPoint->m_dwShotNb),4);
	memcpy_s(pBuf+27,4,&(m_pShotPoint->m_dwSwathNb),4);
	memcpy_s(pBuf+31,4,&(m_pShotPoint->m_dwSourcePointIndex),4);
	memcpy_s(pBuf+35,4,&(m_pShotPoint->m_fSourceLine),4);
	memcpy_s(pBuf+39,4,&(m_pShotPoint->m_fSourceNb),4);
	memcpy_s(pBuf+43,4,&(m_pShotPoint->m_dwSpreadSFL),4);
	memcpy_s(pBuf+47,4,&(m_pShotPoint->m_dwSpreadSFN),4);
	memcpy_s(pBuf+51,4,&(m_pShotPoint->m_dwSpreadNb),4);
	memcpy_s(pBuf+55,4,&(m_pShotPoint->m_dwSuperSpread),4);
	memcpy_s(pBuf+59,4,&(m_pShotPoint->m_dwProcessNb),4);
	
	// 存储comment，类型是字符串，字符串后面需要加两个结束符'\0'
	lpBuf= m_pShotPoint->m_szComments.GetBuffer();	
	memcpy_s(pBuf+63,wPointLen, LPCSTR(lpBuf),wPointLen); 
	m_pShotPoint->m_szComments.ReleaseBuffer();
	pBuf[63+wPointLen]='\0';	// 结束符
	pBuf[64+wPointLen]='\0';
	
	// 存储CShotSource对象
//	memcpy_s(pBuf+23+wPointLen,nSourceLen,m_pShotSource,nSourceLen);
 	memcpy_s(pBuf+65+wPointLen,4,&(m_pShotSource->m_dwSourceType),4);
 	pBuf[69+wPointLen]=m_pShotSource->m_bySourceState;
	memcpy_s(pBuf+70+wPointLen,4,&(m_pShotSource->m_dwSourceNb),4);
	memcpy_s(pBuf+74+wPointLen,4,&(m_pShotSource->m_dwShooterNb),4);
	memcpy_s(pBuf+78+wPointLen,4,&(m_pShotSource->m_lStep),4);
	memcpy_s(pBuf+82+wPointLen,4,&(m_pShotSource->m_dwSourceIndex),4);
	memcpy_s(pBuf+86+wPointLen,4,&(m_pShotSource->m_dwReadyVPNb),4);
	memcpy_s(pBuf+90+wPointLen,4,&(m_pShotSource->m_bReady),4);

	// 存储标签属性
	lpBuf= m_pShotSource->m_strLabel.GetBuffer();	
	memcpy_s(pBuf+94+wPointLen,wLabelLen, LPCSTR(lpBuf),wLabelLen);	
	m_pShotSource->m_strLabel.ReleaseBuffer();
	pBuf[94+wPointLen+wLabelLen]='\0';		// 结束符
	pBuf[95+wPointLen+wLabelLen]='\0';
	// Comment属性
	lpBuf= m_pShotSource->m_strComment.GetBuffer();	
	memcpy_s(pBuf+96+wPointLen+wLabelLen, wCommentLen,LPCSTR(lpBuf), wCommentLen); 
	m_pShotSource->m_strComment.ReleaseBuffer();
	pBuf[96+wPointLen+wLabelLen+wCommentLen]='\0';		// 结束符
	pBuf[97+wPointLen+wLabelLen+wCommentLen]='\0';
	
	if(m_pShotSource->m_dwSourceType == SHOTTYPE_EXPLO)	// 脉冲爆炸机
	{
		// 数据个数，字节数
		wFrmLen = 97+wPointLen+wLabelLen+wCommentLen-12;
		pBuf[11] =LOBYTE(wFrmLen);
		pBuf[12] =HIBYTE(wFrmLen);

		// 帧长
		wFrmLen = 97+wPointLen+wLabelLen+wCommentLen+4;
		pBuf[2] = LOBYTE(wFrmLen);
		pBuf[3] = HIBYTE(wFrmLen);	
	}
	else	// 可控震源爆炸机 SHOTTYPE_VIBRO
	{
		CShotSourceVibro* pSource =(CShotSourceVibro*)m_pShotSource;
		pBuf[98+wPointLen+wLabelLen+wCommentLen]=pSource->m_byMoving;
		memcpy_s(pBuf+99+wPointLen+wLabelLen+wCommentLen,4,&(pSource->m_bWorkByAcq),4);
		memcpy_s(pBuf+103+wPointLen+wLabelLen+wCommentLen,4,&(pSource->m_dwClusterNb),4);

		// 数据个数，字节数
		wFrmLen = 106+wPointLen+wLabelLen+wCommentLen-12;
		pBuf[11] =LOBYTE(wFrmLen);
		pBuf[12] =HIBYTE(wFrmLen);

		// 帧长
		wFrmLen = 106+wPointLen+wLabelLen+wCommentLen+4;
		pBuf[2] = LOBYTE(wFrmLen);
		pBuf[3] = HIBYTE(wFrmLen);	
	}

	// 帧校验
	pBuf[wFrmLen-3]=0;
	// 帧尾
	pBuf[wFrmLen-2] = 0x14;
	pBuf[wFrmLen-1] = 0x6F;
	// 发送数据包
	SendToServer(pBuf,wFrmLen);
	delete []pBuf;
	return 1;
}

/**
 * @brief 发送数据处理函数
 * @note  与施工服务程序建立连接后，在开始放炮前，调用该函数向服务器下发炮点属性及震源属性。
 * @note  帧格式为：帧头（2Byte）＋帧长度（2Byte）  ＋帧计数（4Byte）＋帧类型（1Byte）			   
 ＋命令(2Byte)＋数据个数（2Byte）＋数据区（变长） ＋校验位（1Byte）＋帧尾（2Byte）。
 * @note  在帧格式的数据区包括：服务器ID（4Byte），线程ID（4Byte），CProcessType对象（多字节）。
 * @note  在存储对象的属性中，每个字符串的后面都增加两个空结束符'\0'。
 * @note  依据类型不同存储对象属性不同，解析时也要按照类型来创建不同对象。
 * @param  void
 * @return 返回已发送的字节数。
 */
int  CActiveShot::SendProcessType(void)
{
	if(!m_pProcessType || !m_pSvrSocket)
		return -1;

	WORD  wFrmLen = WORD(m_pProcessType->GetObjectByteSize())+24;
	BYTE* pBuf=new BYTE[wFrmLen];

	// 帧头
	pBuf[0] = 0xEB;
	pBuf[1] = 0x90;
	// 帧长
	pBuf[2] = LOBYTE(wFrmLen);
	pBuf[3] = HIBYTE(wFrmLen);
	// 帧计数
	memset(pBuf+4,1,4);
	// 帧类型
	pBuf[8] = 0x02;
	// 命令码
	pBuf[9]  = LOBYTE(OPERATION_CMD_PROCESS);
	pBuf[10] = HIBYTE(OPERATION_CMD_PROCESS);
	// 数据个数
	pBuf[11] =LOBYTE(wFrmLen-8);
	pBuf[12] =HIBYTE(wFrmLen-8);

	// 服务器ID，线程ID
	memcpy_s(pBuf+13,4,&m_dwServerID,4);
	memcpy_s(pBuf+17,4,&m_dwThreadID,4);
	m_pProcessType->SaveToBuf(pBuf+21);
	
	pBuf[wFrmLen-3]=0;
	pBuf[wFrmLen-2]=0x14;
	pBuf[wFrmLen-1]=0x6F;

	// 发送数据包
	SendToServer(pBuf,wFrmLen);
	delete []pBuf;
	return 1;

}
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////    CActiveShots   /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
CActiveShots::CActiveShots(void)
{
}

CActiveShots::~CActiveShots(void)
{
	int i,nCount = int(m_arrActiveShot.GetCount());
	for (i=0;i<nCount;i++)
	{
		delete m_arrActiveShot[i];
		m_arrActiveShot[i] = NULL;
	}
	m_arrActiveShot.RemoveAll();
}
CActiveShot* CActiveShots::GetActiveShot(int iIndex)
{
	int nCount = GetCount();
	if(iIndex>=nCount || iIndex<0)
		return NULL;
	return m_arrActiveShot[iIndex];
}
int CActiveShots::GetCount(void)
{
	return int(m_arrActiveShot.GetCount());
}
int CActiveShots::Add(CActiveShot* pActiveShot)
{
	return int(m_arrActiveShot.Add(pActiveShot));
}
void  CActiveShots::RemoveActiveShotAt(int iIndex)
{
	CActiveShot* pActiveShot = GetActiveShot(iIndex);
	if(pActiveShot)
	{
		delete pActiveShot;
		pActiveShot = NULL;
		m_arrActiveShot.RemoveAt(iIndex);
	}	
}
/**
 * @brief 根据震源对象查找当前激活的放炮对象
 * @note  根据震源对象查找当前激活的放炮对象
 * @param  CShotSource* pSource，震源对象指针
 * @return 返回激活的放炮管理对象索引。
 */
int CActiveShots::FindFirstActiveShotBySource(CShotSource* pSource)
{
	int i,nCount = GetCount();
	for (i=0;i<nCount;i++)
	{
		if(pSource==m_arrActiveShot[i]->m_pShotSource)
			return i;
	}
	return -1;
}
/**
 * @brief 通过编号寻找激活的放炮管理
 * @note  通过炮点编号和震源编号寻找激活的放炮管理
 * @param DWORD dwShotNb，炮点编号
 * @param DWORD dwSourceNb，震源编号
 * @return 成功返回放炮管理对象指针，失败则返回NULL
 */
CActiveShot*  CActiveShots::FindActiveShotByNb(DWORD dwShotNb,DWORD dwSourceNb)
{
	CActiveShot *pShot=NULL;
	int i,nCount;
	nCount = GetCount();
	for (i=0;i<nCount;i++)
	{
		pShot = GetActiveShot(i);
		if(pShot->IsActiveShot(dwShotNb,dwSourceNb))
			return pShot;
	}
	return NULL;
}
/**
 * @brief 通过服务器ID和线程ID号寻找激活的放炮管理对象
 * @note  通过服务器ID和线程ID号寻找激活的放炮管理对象
 * @param DWORD dwServerID，服务器ID
 * @param DWORD dwThreadID，线程ID号
 * @return 成功返回放炮管理对象指针，失败则返回NULL
 */
int   CActiveShots::FindActiveShotByID(DWORD dwServerID,DWORD dwThreadID)
{
	CActiveShot *pShot=NULL;
	int i,nCount;
	nCount = GetCount();
	for (i=0;i<nCount;i++)
	{
		pShot = GetActiveShot(i);
		if(pShot->m_dwServerID==dwServerID && pShot->m_dwThreadID==dwThreadID)
			return i;
	}
	return -1;
}

