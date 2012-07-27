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

CCommClient* CMatrixCommDll::CreateCommClient(void)
{
	CCommClient* pCommClient = NULL;
	pCommClient = new CCommClient;
	pCommClient->m_pComClientMap = &m_oComClientMap;
	return pCommClient;
}
void CMatrixCommDll::DeleteCommClient(CCommClient* pClass)
{
	if (pClass != NULL)
	{
		delete pClass;
	}
}

CCommServer* CMatrixCommDll::CreateCommServer(void)
{
	CCommServer* pCommServer = NULL;
	pCommServer = new CCommServer;
	pCommServer->m_pComClientMap = &m_oComClientMap;
	return pCommServer;
}
void CMatrixCommDll::DeleteCommServer(CCommServer* pClass)
{
	if (pClass != NULL)
	{
		delete pClass;
	}
}

void CMatrixCommDll::OnInit(void)
{
	m_oComClientMap.clear();
}

void CMatrixCommDll::OnClose(void)
{
	m_oComClientMap.clear();
}