#include "StdAfx.h"
#include "Receive.h"
#include <string>
using namespace std;



CReceive::CReceive(void)
{
}

CReceive::~CReceive(void)
{
}
int CReceive::Receive( void* lpBuf,int nBufLen,int nFlags)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CSocket::Receive( lpBuf,nBufLen,nFlags = 0);
}
BOOL CReceive::OnInit()
{
	BOOL bReturn = false;
	u_short m_UDPSendDataPort = 0x5010;
	CString m_UDPSendDataIP = _T("192.168.1.56");

	// 生成网络端口
	bReturn = Create(m_UDPSendDataPort, SOCK_DGRAM, m_UDPSendDataIP);

	m_ReceiveData.reserve(1000);

// 	设置接收缓冲区大小
	int RevBuf = 20*1024*1024;
	if (this->SetSockOpt( SO_RCVBUF, (char*)&RevBuf, sizeof(RevBuf)) == SOCKET_ERROR) 
	{
		this->Close();		
		return false;
	}
	return bReturn;	
}


//将接收到的数据保存到文件中
void CReceive::SaveReceiveDataToFile(vector<UCHAR> Buf)
{
	CString sPathName;	//读取文件的绝对路径
	CString str;
	const wchar_t pszFilter[] = _T("TXT Document (*.txt)|*.txt||");
	CFileDialog dlg(FALSE, _T("EC"), _T("*.txt"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("Text Document(*.txt)|*.txt")); 

	if ( dlg.DoModal() != IDOK )
		return;
	sPathName = dlg.GetPathName();


	//保存成UNICODE格式的文件
	// 	CFile savefile;	
	// 	savefile.Open(sPathName, CFile::modeCreate|CFile::modeWrite);
	// 	CArchive ar(&savefile, CArchive::store);
	// 	for (int j = 0; j < Buf.size(); j++)
	// 	{
	// 	
	// 		str.Format(_T("%c\r\n"), Buf[j]);
	// 		ar.WriteString(str);
	// 	}
	// 	ar.Close();
	// 	savefile.Close();

	//保存成ANSI格式的文件
	errno_t err;
	char cPath[1000];
	FILE * savefile;
	wchar_t wcPath[1000];

	//将CString转换为wchar_t数组
	USES_CONVERSION_EX;
	wcscpy_s(wcPath, CT2CW(sPathName)); 

	wstring strtemp = (LPCTSTR)wcPath;

	//字符格式由WideChar转换为MultiChar
	string return_value;
	//获取缓冲区的大小，并申请空间，缓冲区大小是按字节计算的
	int len = WideCharToMultiByte(CP_ACP,0,strtemp.c_str(),((int)strtemp.size()),NULL,0,NULL,NULL);
	char *buffer = new char[len+1];
	WideCharToMultiByte(CP_ACP,0,strtemp.c_str(),((int)strtemp.size()),buffer,len,NULL,NULL);
	buffer[len] = '\0';
	//删除缓冲区并返回值
	return_value.append(buffer);
	delete []buffer;

	string mstring = return_value;

	//将string转换为char数组
	strcpy_s( cPath, sizeof(cPath), mstring.c_str() );
	if((err = fopen_s(&savefile,cPath,"w+"))==NULL)
	{

		for (unsigned int j = 0; j < Buf.size(); j++)	//只保存最后一次采集到的数据
		{
			fprintf(savefile,"%c", Buf[j]); 
		}
	}
	fclose(savefile); 
}
/**
* 得到网络接收缓冲区收到的帧数量
* @param void
* @return DWORD 帧数量
*/
DWORD CReceive::GetFrameCount()
{
	DWORD dwFrameCount = 0;

	// 得到网络接收缓冲区数据字节数
	IOCtl(FIONREAD, &dwFrameCount);

	return dwFrameCount;
}