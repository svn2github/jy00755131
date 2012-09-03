#include "StdAfx.h"
#include "NetApp.h"
#include <iostream>
using std::cout;
using std::endl;

CNetApp::CNetApp(int argc, _TCHAR* argv[])
{
	for (int i=1; i<argc; i++)
	{
		PhraseCommandLine(argv[i]);
	}
}


CNetApp::~CNetApp(void)
{
}

void CNetApp::PhraseCommandLine(_TCHAR* argv)
{
	if (0 == strcmp(argv, "你们好么"))
	{
		cout << argv << endl;
		return;
	}
	if (0 == strcmp(argv, "不好"))
	{
		cout << argv << endl;
		return;
	}
}