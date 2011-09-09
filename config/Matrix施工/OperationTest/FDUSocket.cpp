// FDUSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "OperationTest.h"
#include "FDUSocket.h"
#include "OperationTestDlg.h"

// CFDUSocket

CFDUSocket::CFDUSocket()
: m_pDlg(NULL)
{
}

CFDUSocket::~CFDUSocket()
{
}


// CFDUSocket 成员函数

void CFDUSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	char   pBuf[128];
	if(Receive(pBuf,128)>0)
	{
		// 处理重发
		m_pDlg->ProcessRetrans(pBuf);
	}
	CSocket::OnReceive(nErrorCode);
}
