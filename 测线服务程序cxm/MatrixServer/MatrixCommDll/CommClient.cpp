#include "stdafx.h"
#include "MatrixCommDll.h"

CCommClient* CreateCommClient(void)
{
	return new CCommClient;
}
void DeleteCommClient(CCommClient* pClass)
{
	if (pClass != NULL)
	{
		delete pClass;
	}
}

CCommClient::CCommClient()
{

}

CCommClient::~CCommClient()
{

}