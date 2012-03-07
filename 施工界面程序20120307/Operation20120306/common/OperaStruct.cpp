#include "stdafx.h"
#include "OperaStruct.h"
#include "SocketParam.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/////////////////////////    CShotPoint  ////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/**
 * @brief CShotPoint构造函数
 */
CShotPoint::CShotPoint()
: m_VPState(VP_TODO)
, m_dwShotNb(0)
, m_byBreakPoint(SHOTPOINT_BREAK_NO)
, m_dwSwathNb(1)
, m_dwSourcePointIndex(1)
, m_fSourceLine(0)
, m_fSourceNb(0)
, m_dwSpreadNb(1)
, m_dwSpreadSFL(0)
, m_dwSpreadSFN(0)
, m_dwSuperSpread(1)
, m_dwProcessNb(0)
, m_szComments(_T(""))
{

}
/**
 * @brief CShotPoint析构函数
 */
CShotPoint::~CShotPoint(void)
{

}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/////////////////////////    CShotSource  ////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/**
 * @brief CShotSource构造函数
 */
CShotSource::CShotSource(void)
: m_dwSourceNb(0)
, m_dwSourceType(SHOTTYPE_EXPLO)
, m_strLabel(_T(""))
, m_dwShooterNb(0)
, m_strComment(_T(""))
, m_lStep(0)
, m_dwSourceIndex(0)
, m_dwReadyVPNb(0)
, m_bReady(FALSE)
{
	m_bySourceState = SHOTSOURCE_UNUSED;
}
/**
 * @brief CShotSource析构函数
 */
CShotSource::~CShotSource(void)
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////   CShotSourceExplo   //////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/**
 * @brief CShotSourceExplo构造函数
 */
CShotSourceExplo::CShotSourceExplo(void)
{
	m_dwSourceType=SHOTTYPE_EXPLO;
}
/**
 * @brief CShotSourceExplo析构函数
 */
CShotSourceExplo::~CShotSourceExplo(void)
{
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
///////////////////////////      CShotSourceVibro    ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/**
 * @brief CShotSourceVibro构造函数
 */
CShotSourceVibro::CShotSourceVibro(void)
: m_bWorkByAcq(false)
, m_dwClusterNb(1)

{
	m_byMoving = VIBROMOVING_SEQUENTIAL;
	m_dwSourceType = SHOTTYPE_VIBRO;
}
/**
 * @brief CShotSourceVibro析构函数
 */
CShotSourceVibro::~CShotSourceVibro(void)
{
}
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////      CAuxChannel      ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
CAuxChannel::~CAuxChannel(void)
{

}

CAuxChannels::~CAuxChannels(void)
{
	RemoveAll();
}
/**
 * @brief 删除所有辅助道对象
 * @note  删除所有辅助道对象
 * @param void
 * @return void
 */
void   CAuxChannels::RemoveAll(void)
{
	int i,nCount = m_arrAuxChannel.GetCount();
	for (i=0;i<nCount;i++)
	{
		if(m_arrAuxChannel[i])
		{
			delete m_arrAuxChannel[i];
			m_arrAuxChannel[i] = NULL;
		}	
	}
	m_arrAuxChannel.RemoveAll();
}
// 通过索引获得辅助道对象，索引从0开始
/**
 * @brief 通过索引获得辅助道对象，索引从0开始
 * @note  通过索引获得辅助道对象，索引从0开始
 * @param int iIndex，索引号
 * @return 成功则返回指针，失败则返回NULL
 */
CAuxChannel* CAuxChannels::GetAuxChannel(int iIndex)
{
	int nCount = GetCount();
	if(iIndex>=nCount)
		return NULL;

	return m_arrAuxChannel[iIndex];
}
/**
 * @brief 通过炮点编号查找对象
 * @note  通过炮点编号查找对象，返回辅助道对象
 * @param DWORD dwNb，编号
 * @return 成功则返回辅助道对象，失败则返回NULL
 */
CAuxChannel* CAuxChannels::GetAuxChannelByNb(DWORD dwNb)
{
	int i,nCount = m_arrAuxChannel.GetCount();
	for (i=0;i<nCount;i++)
	{
		if(m_arrAuxChannel[i]->m_dwNb == dwNb)
		{			
			return m_arrAuxChannel[i];
		}	
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////      CShotSpread      ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief CShotSpread构造函数
 */
CShotSpread::CShotSpread(void)
{
	m_dwNb = 0;
	m_strLabel = _T("");
	m_bySpreadType = SPREADTYPE_ABSOLUTE;
}
/**
 * @brief CShotSpread析构函数
 
 * 释放所有的采集道数据。
 */
CShotSpread::~CShotSpread(void)
{
	m_arrSpread.RemoveAll();
}

/**
 * @brief 查找采集道。
 * @note  通过IP地址查找采集道 。
 * @param  unsigned long dwIP，采集道IP地址。
 * @return 如果找到，返回采集道在数组中索引号；如果没找到，则返回-1。
 */
int CShotSpread::Find(unsigned long dwIP)
{
	int i,nCount;
	nCount = m_arrSpread.GetCount();
	for (i=0;i<nCount;i++)
	{
		if(m_arrSpread[i].m_dwIP == dwIP)
			return i;
	}
	return -1;
}
/**
 * @brief 添加采集道。
 * @note  向采集排列中添加一个采集道。
 * @param  SHOTSPREAD&  ShotSpread，采集道
 * @return 返回在采集道数组中索引号。
 */
int CShotSpread::Append(SHOTSPREAD&  ShotSpread)
{
	return m_arrSpread.Add(ShotSpread);
}
/**
 * @brief 采集排列清零
 * @note  删除采集排列内全部采集道对象，编号及辅助道个数清零
 * @param  void
 * @return 返回辅助道总数。
*/
void CShotSpread::ReleaseAll(void)
{
	m_arrSpread.RemoveAll();
	m_dwNb = 0;
	m_dwAuxiCount= 0;
} 
/**
 * @brief 计算辅助道总数
 * @note  计算辅助道总数
 * @param  void
 * @return 返回辅助道总数。

unsigned long CShotSpread::GetAuxiChannelCount(void)
{
	unsigned long  lSum = 0;
	int i,nCount=m_arrSpread.GetCount();
	for (i=0;i<nCount;i++)
	{
		if(m_arrSpread[i].m_byType = CHN_AUXI)
			lSum++;
	}
	return lSum;
} */

/**
 * @brief 采集排列点名全部有应答
 * @note  向采集排列点名后，调用该函数，通过分析每个采集道m_lAckedType参数判断是否有应答
 * @param  long bAckedValue, 应答类型
 * @return 如果所有采集道都有应答，则返回true；其他情况返回false
 */
bool CShotSpread::IsAllAcked(long bAckedValue)
{
	int i,nCount=m_arrSpread.GetCount();
	for(i=0;i<nCount;i++)
	{
		if(m_arrSpread[i].m_byType!=CHN_MUTE) // 不是哑道
		{
			if(bAckedValue == SHOTSPREAD_ACKED_CONFIGCMD)
			{	// 如果是点名配置命令，则直接判断
				if(m_arrSpread[i].m_lAckedType!= bAckedValue )
					return false;
			}
			else
			{
				// 检查的应答状态不是点名配置命令,如果点名存在，而bAckedValue得应答不存在，则认为当前命令无应答
				if(m_arrSpread[i].m_lAckedType>SHOTSPREAD_ACKED_NULL && m_arrSpread[i].m_lAckedType!= bAckedValue)
					return false;
			}
			
		}		
	}
	return true;
}

/**
 * @brief 计算有应答的采集道总数。
 * @note  通过分析每个采集道m_lAckedType参数，来计算有应答的采集道总数。
          写SEGD函数中调用
 * @param  void
 * @return 返回有应答的采集道总数。
 */
unsigned int  CShotSpread::TotalOfAcked(void)
{
	unsigned int nSum=0;
	int i,nCount=m_arrSpread.GetCount();
	for(i=0;i<nCount;i++)
	{
		if(m_arrSpread[i].m_lAckedType && m_arrSpread[i].m_byType!=CHN_AUXI)
		{
		    nSum++;
		}		
	}
	return nSum;
}
/**
 * @brief 设置采集道的应答标志
 * @note  通过IP地址查找采集道的位置，再设置有应答标志。
 * @param  DWORD dwIP，采集道的IP地址。
 * @return 如果找到采集道并设置应答标志，则返回true；如果没有找到采集道，则返回false。
 */
bool CShotSpread::SetAck(DWORD dwIP,long lAckedType)
{
	int i,nCount=m_arrSpread.GetCount();
	for(i=0;i<nCount;i++)
	{
		if(m_arrSpread[i].m_dwIP == dwIP)
		{
			m_arrSpread[i].m_lAckedType=lAckedType;
			return true;
		}
	}
	return false;
}

/////////////////////////////   数据处理类型定义  //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////      CProcessType      ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief CProcessType构造函数
 */
CProcessType::CProcessType(void)
: m_ProcessSetup(PROCESS_STANDARD)
, m_byProcessType(PROCESS_IMPULSIVE)
, m_dwRecordLen(1000)
{
}
/**
 * @brief CProcessType析构函数
 */
CProcessType::~CProcessType(void)
{
	m_arrAuxiChannel.RemoveAll();
}

/**
 * @brief 计算处理类型对象的字节数
 * @note  计算处理类型对象的属性数据总的字节长度，对于每个字符串，还要加上两个字节的'\0';
 对于数组，要加上四个字节的数组个数值。
 * @param  void
 * @return 返回处理类型对象属性总的字节数。
 */
DWORD CProcessType::GetObjectByteSize()
{
	DWORD  dwSize =18;
	dwSize+=m_strLabel.GetLength()*sizeof(TCHAR)+2;
	dwSize+=m_arrAuxiChannel.GetCount()*sizeof(AUXICHANNEL)+4;	// 需要保存大小，在保存数据
	return dwSize;
}

/**
 * @brief 保存处理类型对象
 * @note  将处理类型对象所有属性写入缓冲区，对于每个字符串，还要加上两个字节的'\0';
 对于数组，要加上四个字节的数组个数值。一般用于将对象保存在缓冲区后发送给另一端。
 * @param  unsigned char* pBuf，缓冲区指针
 * @return 返回保存的处理类型对象所占缓冲区的字节数。
 */
int  CProcessType::SaveToBuf(unsigned char* pBuf)
{
	DWORD nLen=0;
	DWORD i=0,nCount=0;
	// 先保存处理类型
	pBuf[0]=m_byProcessType;
	pBuf[1]=m_ProcessSetup;
	memcpy_s(pBuf+2,4,&(m_dwProcessNb),4);
	memcpy_s(pBuf+6,4,&(m_dwFDUSN),4);
	memcpy_s(pBuf+10,4,&(m_dwRecordLen),4);
	memcpy_s(pBuf+14,4,&(m_dwTBWindow),4);
	
	nCount = m_arrAuxiChannel.GetCount();
	memcpy_s(pBuf+18,4,&(nCount),4);
	nLen = 22;	// 当前缓冲区指针
	for (i=0;i<nCount;i++)
	{
		memcpy_s(pBuf+nLen,sizeof(AUXICHANNEL),&(m_arrAuxiChannel[i]),sizeof(AUXICHANNEL));
		nLen+=sizeof(AUXICHANNEL);	// (i+1)*
	}
	nCount = m_strLabel.GetLength()*sizeof(TCHAR);
	memcpy_s(pBuf+nLen,nCount, LPCTSTR(m_strLabel),nCount);	
	pBuf[nLen+nCount]='\0';
	pBuf[nLen+nCount+1]='\0';
	return nLen+nCount+2;
}

/**
 * @brief 读取处理类型对象
 * @note  与SaveToBuf函数相对应，将处理类型对象所有属性从缓冲区读出，对于每个字符串，还要加上两个字节的'\0';
 对于数组，要先读四个字节的数组大小值。一般用于将对象保存在缓冲区后发送给另一端，从缓冲区读出。
 * @param  unsigned char* pBuf，缓冲区指针
 * @return 返回处理类型对象所占缓冲区的字节数。
 */
int  CProcessType::ReadFromBuf(unsigned char* pBuf)
{
	DWORD nLen=0;
	DWORD i=0,nCount=0;
	m_byProcessType=pBuf[0];
	m_ProcessSetup=pBuf[1];
	memcpy_s(&(m_dwProcessNb),4,pBuf+2,4);
	memcpy_s(&(m_dwFDUSN),4,pBuf+6,4);
	memcpy_s(&(m_dwRecordLen),4,pBuf+10,4);
	memcpy_s(&(m_dwTBWindow),4,pBuf+14,4);

	memcpy_s(&(nCount),4,pBuf+18,4);
	m_arrAuxiChannel.SetSize(nCount);	
	nLen = 22;	// 当前缓冲区指针
	for (i=0;i<nCount;i++)
	{	
		memcpy_s(&(m_arrAuxiChannel[i]),sizeof(AUXICHANNEL),pBuf+nLen,sizeof(AUXICHANNEL));
		nLen+=sizeof(AUXICHANNEL);	// (i+1)*
	}	
	//memcpy_s(pBuf+nLen,nCount, LPCTSTR(m_strLabel),nCount);	
	m_strLabel = LPCTSTR(pBuf+nLen);
	nCount = m_strLabel.GetLength()*sizeof(TCHAR);
	return nLen+nCount+2;

}
/*
 * @brief 复制处理类型对象
 * @note  从另一个对象将处理要求拷贝到本对象，完全复制一个新的对象。
 * @param  CProcessType* pProcessSrc，数据处理对象
 * @return bool。
 
bool  CProcessType::Copy(CProcessType* pProcessSrc)
{
	unsigned char *pBuf;
	DWORD nSize =pProcessSrc->GetObjectByteSize();
	pBuf = new unsigned char[nSize];
	if(pBuf==NULL)
		return false;
	pProcessSrc->SaveToBuf(pBuf);
	this->ReadFromBuf(pBuf);
	delete[]pBuf;
	return true;
}
*/
/////////////////////////////   数据处理类型定义  //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////      CProcessImpulsive      ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief CProcessImpulsive构造函数
 */
CProcessImpulsive::CProcessImpulsive(void)
{
	m_dwRefractionDelay = 0;
	m_dwTBWindow = 1000;
}
/**
 * @brief CProcessImpulsive析构函数
 */
CProcessImpulsive::~CProcessImpulsive(void)
{
}
/**
 * @brief 计算处理类型对象的字节数
 * @note  计算处理类型对象的属性数据总的字节长度，对于每个字符串，还要加上两个字节的'\0';
 对于数组，要加上四个字节的数组个数值。
 * @param  void
 * @return 返回处理类型对象属性总的字节数。
 */
DWORD CProcessImpulsive::GetObjectByteSize()
{
	return CProcessType::GetObjectByteSize()+4;
}
/**
 * @brief 保存处理类型对象
 * @note  将处理类型对象所有属性写入缓冲区，对于每个字符串，还要加上两个字节的'\0';
 对于数组，要加上四个字节的数组个数值。一般用于将对象保存在缓冲区后发送给另一端。
 先调用基类的对象，在增加该对象特有的属性。
 * @param  unsigned char* pBuf，缓冲区指针
 * @return 返回保存的处理类型对象所占缓冲区的字节数。
 */
int  CProcessImpulsive::SaveToBuf(unsigned char* pBuf)
{
	int nLen = CProcessType::SaveToBuf(pBuf);
	memcpy_s(pBuf+nLen,4,&m_dwRefractionDelay,4);
	return nLen+4;

}
/**
 * @brief 读取处理类型对象
 * @note  与SaveToBuf函数相对应，将处理类型对象所有属性从缓冲区读出，对于每个字符串，还要加上两个字节的'\0';
 对于数组，要先读四个字节的数组大小值。一般用于将对象保存在缓冲区后发送给另一端，从缓冲区读出。
 * @param  unsigned char* pBuf，缓冲区指针
 * @return 返回处理类型对象所占缓冲区的字节数。
 */
int  CProcessImpulsive::ReadFromBuf(unsigned char* pBuf)
{
	int nLen = CProcessType::ReadFromBuf(pBuf);
	memcpy_s(&m_dwRefractionDelay,4,pBuf+nLen,4);
	return nLen+4;
}
/////////////////////////////   施工客户端属性管理  //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////       CShotClient      ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief CShotClient构造函数
 */
CShotClient::CShotClient(void)
: m_dwClientIP ( 0xC00A8C0)
, m_wClientPort( OPERATION_CLIENTPORT  )
, m_pShotSource(NULL),m_pShotPoint(NULL),m_pSpread(NULL), m_pProcessType(NULL)
, m_pFDUConfig(NULL)
, m_strNormalFilePath(_T("D:\\"))
, m_strTestFilePath(_T("D:\\"))
, m_dwFileNumber(1)
, m_strFileName(_T(""))
, m_strSEGDFileName(_T(""))
{
	if(m_pShotSource)
	{
		delete m_pShotSource;
		m_pShotSource = NULL;
	}
	if(m_pShotPoint)
	{
		delete m_pShotPoint;
		m_pShotPoint = NULL;
	}
	if(m_pSpread)
	{
		delete m_pSpread;
		m_pSpread = NULL;
	}
	if(m_pProcessType)
	{
		delete m_pProcessType;
		m_pProcessType = NULL;
	}
	if(m_pFDUConfig)
	{
		delete m_pFDUConfig;
		m_pFDUConfig = NULL;
	}
}
/**
 * @brief CShotClient析构函数
 释放炮点、震源、采集排列、处理类型对象。
 */
CShotClient::~CShotClient(void)
{
	if(m_pShotSource)
	{
		delete m_pShotSource;
		m_pShotSource = NULL;
	}
	if(m_pShotPoint)
	{
		delete m_pShotPoint;
		m_pShotPoint = NULL;
	}
	if(m_pSpread)
	{
		delete m_pSpread;
		m_pSpread = NULL;
	}
	if(m_pProcessType)
	{
		delete m_pProcessType;
		m_pProcessType = NULL;
	}
	if(m_pFDUConfig)
	{
		delete m_pFDUConfig;
		m_pFDUConfig = NULL;
	}
}
