// CharConvertTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CharConvertTest.h"
#include <string>
#include <string.h>
#include <stdio.h>

// 使用CString必须使用MFC，并且不可包含<windows.h>

#include <afx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

//----------------------------------------------------------------------------------
//将 单字节char* 转换为 宽字节 wchar*
inline wchar_t* AnsiToUnicode( const char* szStr )
{
	int nLen = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0 );
	if (nLen == 0)
	{
		return NULL;
	}
	wchar_t* pResult = new wchar_t[nLen];
	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen );
	return pResult;
}
//----------------------------------------------------------------------------------
// 将 宽字节wchar_t* 转换 单字节char*
inline char* UnicodeToAnsi( const wchar_t* szStr )
{
	int nLen = WideCharToMultiByte( CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL );
	if (nLen == 0)
	{
		return NULL;
	}
	char* pResult = new char[nLen];
	WideCharToMultiByte( CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL );
	return pResult;
}
//----------------------------------------------------------------------------------
// 将单字符 string 转换为宽字符 wstring
inline void Ascii2WideString( const std::string& szStr, std::wstring& wszStr )
{
	int nLength = MultiByteToWideChar( CP_ACP, 0, szStr.c_str(), -1, NULL, NULL );
	wszStr.resize(nLength);
	LPWSTR lpwszStr = new wchar_t[nLength];
	MultiByteToWideChar( CP_ACP, 0, szStr.c_str(), -1, lpwszStr, nLength );
	wszStr = lpwszStr;
	delete [] lpwszStr;
}
//---------------------------------------------------------------------------------


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
		char*   pChar = "我喜欢char";
		wchar_t* pWideChar = L"我讨厌wchar_t";
		wchar_t   tagWideCharList[100] ;
		char   ch = 'A';
		char   tagChar[100] = {NULL};
		CString   cStr;
		std::string str;

		// 注：设置语言环境以便输出WideChar
		setlocale(LC_ALL,"chs");

		// 注： char* 转换 wchar_t*
		// 注： wchar_t 未重载 << ，所以不可使用 cout << 输出
		pWideChar = AnsiToUnicode( pChar );
		// 注：printf("%ls") 和 wprintf(L"%s") 一致
		printf( "%ls\n", pWideChar );

		// 注：wchar_t* 转换 wchar_t[]
		wcscpy_s( tagWideCharList, pWideChar );
		wprintf( L"%s\n", tagWideCharList );

		// 注：wchar_t[] 转换 wchar_t*
		pWideChar = tagWideCharList;
		wprintf( L"%s\n", pWideChar );

		// 注：char 转换 string
		str.insert( str.begin(), ch );
		cout << str << endl;

// 		// 注：wchar_t* 转换 string
// 		pWideChar = new wchar_t[str.length()];
// 		swprintf( pWideChar, L"%s", str.c_str());
// 		wprintf( L"%s\n", pWideChar );

		// 注：string 转换 char*
		pChar = const_cast<char*>(str.c_str());
		cout << pChar << endl;

		// 注：char* 转换 string
		str = std::string(pChar);
		// 注： cout 的 << 重载了string, 若printf 的话必须 printf("%s", str.c_str()); 
		//   而不可 print( "%s", str ); 因为 str 是个 string 类
		cout << str << endl;

		// 注：string 转换 char[]
		str = "无聊啊无聊";
		strcpy_s( tagChar, str.c_str() );
		printf( "%s\n", tagChar );

		// 注：string 转换 CString;
		cStr = str.c_str();
 
// 		// 注：CString 转换 string 
// 		str = string(cStr.GetBuffer(cStr.GetLength()));


		// 注：char* 转换 CString
		cStr = pChar;

// 		// 注：CString 转换 char*
// 		pChar = cStr.GetBuffer( cStr.GetLength() );
// 
// 		// 注：CString 转换 char[]
// 		strncpy( tagChar, (LPCTSTR)CString, sizeof(tagChar));

		// 注：CString 转换 wchar_t*
		pWideChar = cStr.AllocSysString();
		printf( "%ls\n", pWideChar );
	}

	return nRetCode;
}