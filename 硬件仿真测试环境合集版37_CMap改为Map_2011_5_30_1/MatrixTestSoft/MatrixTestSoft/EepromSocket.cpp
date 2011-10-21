// EepromSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixTestSoft.h"
#include "EepromSocket.h"


// CEepromSocket

CEepromSocket::CEepromSocket()
{
}

CEepromSocket::~CEepromSocket()
{
}


// CEepromSocket 成员函数

void CEepromSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	CSocket::OnReceive(nErrorCode);
}
