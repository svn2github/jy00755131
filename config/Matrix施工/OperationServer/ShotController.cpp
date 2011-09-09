#include "StdAfx.h"
#include "ShotController.h"
#include "..\common\LineStruct.h"
CShotController::CShotController(void)
: m_dwFDUIP(0)
, m_dwFDUSN(0)
, m_pHostSocket(NULL)
{
	m_dwShotNb = 0;
	/** 震源点测线 */
	m_fSourceLine= 0;
	/** 震源点号 */
	m_fSourceNb= 0;
	/** 震源点索引 */
	m_dwSwathID= 0;
	/** 爆炸机号 */
	m_dwSourceNb= 0;
	/** 爆炸机状态 */
	m_byBlasterState= 0; 
	/** 确认TB时刻 */
	m_fConfirmedTB= 0;
	/** 井口时间 */
	m_fUpholeTime= 0;
	m_bNewState = FALSE;
}

CShotController::~CShotController(void)
{
}
void   CShotController::Reset(void)
{
	m_dwShotNb = 0;
	/** 震源点测线 */
	m_fSourceLine= 0;
	/** 震源点号 */
	m_fSourceNb= 0;
	/** 震源点索引 */
	m_dwSwathID= 0;
	/** 爆炸机号 */
	m_dwSourceNb= 0;
	/** 爆炸机状态 */
	m_byBlasterState= 0; 
	/** 确认TB时刻 */
	m_fConfirmedTB= 0;
	/** 井口时间 */
	m_fUpholeTime= 0;
	m_bNewState = FALSE;
}
/**
 * @brief 设置爆炸机控制器所连的采集站SN号
 * @note  设置爆炸机控制器所连的采集站SN号
 * @param DWORD dwSN,爆炸机控制器所连的采集站SN号
 * @return void
 */
void CShotController::SetFDUSN(DWORD dwSN)
{
	m_dwFDUSN = dwSN;
	m_dwFDUIP = CIPList::GetBlastMachineIP(dwSN);
	SendFDUConfigCmd();
}
/**
 * @brief 发送爆炸机数据返回的主机地址及端口
 * @note  系统启动。
 * @param unsigned char* pBuf，数据帧缓冲区
 * @paramint nSize，缓冲区数据大小
 * @return 返回已经解析完缓冲区字节数，失败则返回0。
 */
int CShotController::SendFDUConfigCmd(void)
{
	unsigned char pBuf[128];
	if(!m_pHostSocket)
		return 0;
	memset(pBuf,0xAA,15);
	memcpy_s(pBuf+16,4,&m_pHostSocket->m_dwHostIP,sizeof(DWORD));
	memcpy_s(pBuf+20,4,&m_dwFDUIP,sizeof(DWORD));
	memcpy_s(pBuf+24,2,&m_pHostSocket->m_wBlasterPort,sizeof(WORD));
	pBuf[26]=0x01;
	pBuf[27]=0;			// 设置命令

	pBuf[32]=0x07;		// 命令码:自动数据返回地址
	memcpy_s(pBuf+33,4,&(m_pHostSocket->m_dwHostIP),sizeof(DWORD));

	pBuf[37]=0x08;		// 命令码
	pBuf[38]=LOBYTE(m_pHostSocket->m_wBlasterPort); // 自动数据返回端口(SOCKET_REV8PORT: 0xB000)
	pBuf[39]=HIBYTE(m_pHostSocket->m_wBlasterPort);
	pBuf[40]=0x03;		//  命令2Byte,( 命令最高位为1时标识采用循环端口,低位用0x03表示采样数据命令)
	pBuf[41]=0x00;

	pBuf[42] = 0;
	return m_pHostSocket->SendToFDU(pBuf,128);
}
// 发送点火命令
int CShotController::SendFireOrder(CShotClient* pShotClient)
{
	return false;
}
/**
 * @brief 解析爆炸机数据帧
 * @note  解析缓冲区中保存的爆炸机数据。
 * @param unsigned char* pBuf，数据帧缓冲区
 * @paramint nSize，缓冲区数据大小
 * @return 返回已经解析完缓冲区字节数，失败则返回0。
 */
bool CShotController::AnalysisFrame(unsigned char* pBuf,int nSize)
{
	return 0;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CShotCtrlProII::CShotCtrlProII(void)
{
	
}

CShotCtrlProII::~CShotCtrlProII(void)
{
}

/**
 * @brief 向爆炸机控制器发送点火FO命令
 * @note  采用下行命令帧格式向爆炸机控制器发送点火命令
 * @note  格式：
				*SP#.../SL#.../SN#.../SI#.../SEQ#...<CR><LF>
	其中：
								最小	最大
	炮号			SP#sssss	1		99999
	震源点测线		SL#lllll.l	0.0		99999.9
	震源点号		SN#lllll.l	0.0		99999.9
	震源点索引		SI#1		1		9
	爆炸机号		SEQ#ss		01		16

	* @param CShotClient* pShotClient，数据帧缓冲区
	* @return 发送成功返回true，否则返回false。
*/
int CShotCtrlProII::SendFireOrder(CShotClient* pShotClient)
{
	if(!pShotClient || !m_pHostSocket)
		return false;

	unsigned char pBuf[128];
	int		nPos;
	memset(pBuf,0xAA,15);

	// 源地址，主机IP
	nPos = 16;
	memcpy_s(pBuf+nPos,4,&(m_pHostSocket->m_dwHostIP),4);
	nPos+=4;
	// 目标地址，采集站IP
	memcpy_s(pBuf+nPos,4,&m_dwFDUIP,4);
	nPos+=4;
	// 源地址的端口：与爆炸机控制器所连的采集站通信Socket端口
	pBuf[nPos++] = LOBYTE(m_pHostSocket->m_wBlasterPort);
	pBuf[nPos++] = HIBYTE(m_pHostSocket->m_wBlasterPort);
	// 命令
	pBuf[nPos++] = 0x03;
	pBuf[nPos++] = 0x00;

	// 数据区
	nPos = 32;
	pBuf[nPos++] = 0xFF;
	pBuf[nPos++] = 0xFF;
	// *SP#11111
	pBuf[nPos++] = '*';
	pBuf[nPos++] = 'S';
	pBuf[nPos++] = 'P';
	pBuf[nPos++] = '#';
	// 最多5位的炮号
	sprintf_s((char*)(pBuf+nPos),6,"% 5d",pShotClient->m_pShotPoint->m_dwShotNb);
	nPos+=5;
	pBuf[nPos++]='/';
	// 震源点测线，SL#lllll.l
	pBuf[nPos++] = 'S';
	pBuf[nPos++] = 'L';
	pBuf[nPos++] = '#';
	sprintf_s((char*)(pBuf+nPos),8,"% 5.1f",pShotClient->m_pShotPoint->m_fSourceLine);
	nPos+=7;
	pBuf[nPos++]='/';

	// SN#lllll.l
	pBuf[nPos++] = 'S';
	pBuf[nPos++] = 'N';
	pBuf[nPos++] = '#';
	sprintf_s((char*)(pBuf+nPos),8,"% 5.1f",pShotClient->m_pShotPoint->m_fSourceNb);
	nPos+=7;
	pBuf[nPos++]='/';

	// SI#1
	pBuf[nPos++] = 'S';
	pBuf[nPos++] = 'I';
	pBuf[nPos++] = '#';
	sprintf_s((char*)(pBuf+nPos),2,"%1d",pShotClient->m_pShotPoint->m_dwSwathNb);
	nPos+=1;
	pBuf[nPos++]='/';

	// SEQ#ss
	pBuf[nPos++] = 'S';
	pBuf[nPos++] = 'E';
	pBuf[nPos++] = 'Q';
	pBuf[nPos++] = '#';
	sprintf_s((char*)(pBuf+nPos),3,"% 2d",pShotClient->m_pShotSource->m_dwSourceNb);	// ?????????如果Nb大于100，则存在问题
	nPos+=2;

	// <CR><LF>
	pBuf[nPos++]='\r';
	pBuf[nPos++]='\n';
	pBuf[nPos++] = '\0';
	return m_pHostSocket->SendToFDU(pBuf,128);
	
}
/**
 * @brief 解析爆炸机数据帧
 * @note  解析缓冲区中保存的爆炸机数据。
 * @note  来自爆炸机的数据帧采用上位帧格式，依据命令的不同进行解析：
          当命令为01时，主要是主机设置采集站命令的回令数据帧；
		  当命令为02时，时查询采集站的参数回令，对于爆炸机所连的采集站一般不需要查询状态；
		  当命令为03时，是传送的爆炸机数据，调用AnalysisBlasterToHostFrame函数解析。
 * @param int nSize，缓冲区数据大小
 * @return 返回已经解析完缓冲区字节数，失败则返回0。
 */
bool CShotCtrlProII::AnalysisFrame(unsigned char* pBuf,int nSize)
{
	// 判断命令字
	WORD	wFrmCode;
	wFrmCode = MAKEWORD(pBuf[26],pBuf[27]);
	unsigned char*  pBufTemp=NULL;
	switch(wFrmCode)
	{
	case 1:
		{
			// 向采集站设置参数的回令, 如调用函数SendFDUConfigCmd的回令
			// 先不作处理，直接丢掉
			break;
		}
	case 2:
		{
			break;
		}
	case 3:
		{
			// 向采集站设置数据的回令，先不处理
			
			// 来自爆炸机的数据,是否为"*SHOTPRO "
			pBufTemp = pBuf+30;			
			if(    pBufTemp[0]=='*' && pBufTemp[1]=='S' && pBufTemp[2]=='H'
				&& pBufTemp[3]=='O' && pBufTemp[4]=='T' && pBufTemp[5]=='P'
				&& pBufTemp[6]=='R' && pBufTemp[7]=='O' && pBufTemp[8]==' ')
				AnalysisBlasterToHostFrame(pBuf+30,228);
			break;
		}
	default:
		return false;
	}
	return true;
}
/**
 * @brief 解析爆炸机数据帧
 * @note  解析缓冲区中保存的爆炸机数据。
 * @note  格式：*SHOTPRO SP#.../SL#.../SN#.../SI#.../SEQ#.../STA:../CTB:00.000/UH:000.0
 其中: SHOTPRO 用于识别放炮系统的字符串

									最小	最大
 炮号					SP#sssss	1		99999
 震源点测线				SL#lllll.l	0.0		99999.9
 震源点号				SN#lllll.l	0.0		99999.9
 震源点索引				SI#1		1		9
 爆炸机号				SEQ#ss		01		16
 爆炸机状态				STA:s		0		7
 确认的时断信号			CTB:cc.ccc	00.000	33.000
 井口时间（初至波拾取） UH:uuu.u	000.0	999.5 （.5 毫秒步幅）

 * @param unsigned char* pBuf，数据帧缓冲区,是上位帧中的数据区（第30个字节开始）。
 * @param int nSize，缓冲区数据大小
 * @return 返回已经解析完缓冲区字节数，失败则返回0。
 */
bool CShotCtrlProII::AnalysisBlasterToHostFrame(unsigned char* pBuf,int nSize)
{
	unsigned char byTemp[10];
	int nPos=0;
	// 判断数据长度
	// WORD  wDataLen = MAKEWORD(pBuf[nPos],pBuf[nPos+1]);
	// if(wDataLen<77)
	// 	return false;
	// nPos+=2; 
	// *
	if(pBuf[nPos++]!='*')
		return false;

	// 是否为"SHOTPRO "
	if(    pBuf[nPos]  !='S'|| pBuf[nPos+1]!='H'
		|| pBuf[nPos+2]!='O'|| pBuf[nPos+3]!='T'
		|| pBuf[nPos+4]!='P'|| pBuf[nPos+5]!='R'
		|| pBuf[nPos+6]!='O'|| pBuf[nPos+7]!=' ')
		return false;
	nPos+=8;
	// 判断 "SP#" 
	if( pBuf[nPos]!='S'|| pBuf[nPos+1]!='P'|| pBuf[nPos+2]!='#')
		return false;
	nPos+=3;
	// 炮号
	byTemp[0] = pBuf[nPos++];
	byTemp[1] = pBuf[nPos++];
	byTemp[2] = pBuf[nPos++];
	byTemp[3] = pBuf[nPos++];
	byTemp[4] = pBuf[nPos++];
	byTemp[5] = '\0';
	byTemp[6] = '\0';

	if(pBuf[nPos++]!='/')
		return false;
	m_dwShotNb = atoi((char *)byTemp);

	// 判断 "SL#" 
	if( pBuf[nPos]!='S'|| pBuf[nPos+1]!='L'|| pBuf[nPos+2]!='#')
		return false;
	nPos+=3;
	byTemp[0] = pBuf[nPos++];
	byTemp[1] = pBuf[nPos++];
	byTemp[2] = pBuf[nPos++];
	byTemp[3] = pBuf[nPos++];
	byTemp[4] = pBuf[nPos++];
	byTemp[5] = pBuf[nPos++];
	byTemp[6] = pBuf[nPos++];
	byTemp[7] = '\0';
	byTemp[8] = '\0';
	if(pBuf[nPos++]!='/')
		return false;
	m_fSourceLine =(float)atof((char *)byTemp);

	// 判断 "SN#" 
	if( pBuf[nPos]!='S'|| pBuf[nPos+1]!='N'|| pBuf[nPos+2]!='#')
		return 0;
	nPos+=3;
	byTemp[0] = pBuf[nPos++];
	byTemp[1] = pBuf[nPos++];
	byTemp[2] = pBuf[nPos++];
	byTemp[3] = pBuf[nPos++];
	byTemp[4] = pBuf[nPos++];
	byTemp[5] = pBuf[nPos++];
	byTemp[6] = pBuf[nPos++];
	byTemp[7] = '\0';
	byTemp[8] = '\0';
	if(pBuf[nPos++]!='/')
		return false;
	m_fSourceNb =(float)atof((char *)byTemp);

	// 判断 "SI#" 
	if( pBuf[nPos]!='S'|| pBuf[nPos+1]!='I'|| pBuf[nPos+2]!='#')
		return false;
	nPos+=3;
	byTemp[0] = pBuf[nPos++];
	byTemp[1] = '\0';
	byTemp[2] = '\0';
	if(pBuf[nPos++]!='/')
		return false;
	m_dwSwathID =atoi((char *)byTemp);

	// 判断SEQ#
	if( pBuf[nPos]!='S'|| pBuf[nPos+1]!='E'|| pBuf[nPos+2]!='Q'|| pBuf[nPos+3]!='#')
		return 0;
	nPos+=4;
	byTemp[0] = pBuf[nPos++];
	byTemp[1] = pBuf[nPos++];
	byTemp[2] = '\0';
	byTemp[3] = '\0';
	if(pBuf[nPos++]!='/')
		return false;
	m_dwSourceNb =atoi((char *)byTemp);
	// 判断STA:
	if( pBuf[nPos]!='S'|| pBuf[nPos+1]!='T'|| pBuf[nPos+2]!='A'|| pBuf[nPos+3]!=':')
		return false;
	nPos+=4;
	byTemp[0] = pBuf[nPos++];	
	byTemp[1] = '\0';
	byTemp[2] = '\0';
	if(pBuf[nPos++]!='/')
		return false;
	m_byBlasterState =byTemp[0]-'0';

	// 判断CTB:cc.ccc
	if( pBuf[nPos]!='C'|| pBuf[nPos+1]!='T'|| pBuf[nPos+2]!='B'|| pBuf[nPos+3]!=':')
		return false;
	nPos+=4;
	byTemp[0] = pBuf[nPos++];
	byTemp[1] = pBuf[nPos++];
	byTemp[2] = pBuf[nPos++];
	byTemp[3] = pBuf[nPos++];
	byTemp[4] = pBuf[nPos++];
	byTemp[5] = pBuf[nPos++];
	byTemp[6] = '\0';
	byTemp[7] = '\0';
	byTemp[8] = '\0';
	if(pBuf[nPos++]!='/')
		return false;
	m_fUpholeTime =(float)atof((char *)byTemp);
	// 更新状态
	m_bNewState = TRUE;
	return true;
}