// MatrixCommDll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "MatrixCommDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: 更改错误代码以符合您的需要
			_tprintf(_T("错误: MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: 在此处为应用程序的行为编写代码。
		}
	}
	else
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: GetModuleHandle 失败\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
CMatrixCommDll* CreateMatrixCommDll(void)
{
	return new CMatrixCommDll;
}
void DeleteMatrixCommDll(CMatrixCommDll* pClass)
{
	if (pClass != NULL)
	{
		delete pClass;
	}
}
CMatrixCommDll::CMatrixCommDll()
{

}
CMatrixCommDll::~CMatrixCommDll()
{

}
// 初始化
void CMatrixCommDll::OnInit(void)
{
	// 初始化套接字库
	OnInitSocketLib();
// 	// 初始化服务端AsyncSocket
// 	m_oServerAsyncSocket.OnInit();
	m_pCommClient = CreateCommClient();
}


// 关闭
void CMatrixCommDll::OnClose(void)
{
// 	// 关闭服务端AsyncSocket
// 	m_oServerAsyncSocket.OnClose();
	DeleteCommClient(m_pCommClient);
	// 释放套接字库
	OnCloseSocketLib();
}

// 初始化套接字库
void CMatrixCommDll::OnInitSocketLib(void)
{
	WSADATA wsaData;
	CString str = _T("");
	if (WSAStartup(0x0202, &wsaData) != 0)
	{
		str.Format(_T("WSAStartup() failed with error %d"), WSAGetLastError());
		AfxMessageBox(str);
	}
}
// 释放套接字库
void CMatrixCommDll::OnCloseSocketLib(void)
{
	CString str = _T("");
	// 释放套接字库
	if (WSACleanup() != 0)
	{
		str.Format(_T("WSACleanup() failed with error %d"), WSAGetLastError());
		AfxMessageBox(str);
	}
}