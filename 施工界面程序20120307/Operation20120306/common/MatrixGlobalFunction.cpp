#include "stdafx.h"
#include "MatrixDef.h"
#include <afxinet.h>

////////////////////////////////////////////////////////////////////////////////////////////
/// 自定义消息
UINT NEAR WM_NEWXMLFILE = RegisterWindowMessage(MSG_XMLFILE_NEW);

/**
* 由FTP服务器得到Matrix设置文件
* @param void
* @return bool true：成功；false：失败
*/
bool GetMatrixXMLFromFTPServer(LPCTSTR strFTPServerIP,LPCTSTR strFTPFile,LPCTSTR strLocalFile)
{
	bool bReturn = false;
	BOOL bData;
	int iCount = 0;
	CInternetSession oSession;
	CFtpConnection* pConnection = oSession.GetFtpConnection(strFTPServerIP);
		
	while(true)
	{
		bData = pConnection->GetFile(strFTPFile, strLocalFile, FALSE);
		if(TRUE == bData)
		{
			bReturn = true;
			break;
		}
		iCount++;
		if(4 <= iCount)
		{
			break;
		}
		Sleep(50);
	}
	pConnection->Close();
	delete pConnection;
	return bReturn;
}
/**
* 保存Matrix设置文件到FTP服务器
* @param void
* @return bool true：成功；false：失败
*/
bool PutMatrixXMLToFTPServer(LPCTSTR strFTPServerIP,LPCTSTR strFTPFile,LPCTSTR strLocalFile)
{
	bool bReturn = false;
	BOOL bData;
	int iCount = 0;
	CInternetSession oSession;
	CFtpConnection* pConnection = oSession.GetFtpConnection(strFTPServerIP);
	
	while(true)
	{
		//bData = pConnection->PutFile(strLocalFile, strFTPFile);
		bData = pConnection->PutFile(strLocalFile, strLocalFile);
		if(TRUE == bData)
		{
			bReturn = true;
			break;
		}
		iCount++;
		if(4 <= iCount)
		{
			break;
		}
		Sleep(50);
	}
	pConnection->Close();
	delete pConnection;
	return bReturn;
}