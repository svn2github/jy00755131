// ComClient.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixServer.h"
#include "ComClient.h"


// CComClient

CComClient::CComClient()
{
}

CComClient::~CComClient()
{
}

// 创建一个客户端连接信息
void CComClient::OnInit(void)
{
	m_oClientSocket.OnInit(this, 1000, 1000);
	m_oClientRecFrame.OnInit();
	m_oClientRecThread.m_pClientRecFrame = &m_oClientRecFrame;
	m_oClientRecThread.OnInit();
}

// 释放一个客户端连接信息
void CComClient::OnClose(void)
{
	m_oClientSocket.OnClose();
	m_oClientRecFrame.OnClose();
	m_oClientRecThread.OnClose();
	delete this;
}


