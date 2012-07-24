#include "stdafx.h"
#include "MatrixCommDll.h"

CCommClient::CCommClient()
{

}
CCommClient::~CCommClient()
{

}
void CCommClient::OnConnect(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	OnInit();
	CCommSocket::OnConnect(nErrorCode);
}