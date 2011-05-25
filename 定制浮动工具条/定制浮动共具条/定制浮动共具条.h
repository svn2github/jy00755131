// 定制浮动共具条.h : 定制浮动共具条 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// C定制浮动共具条App:
// 有关此类的实现，请参阅 定制浮动共具条.cpp
//

class C定制浮动共具条App : public CWinApp
{
public:
	C定制浮动共具条App();


// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern C定制浮动共具条App theApp;