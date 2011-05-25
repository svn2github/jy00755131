#pragma once
#include "afxsock.h"
#include <vector>
using namespace std;

class CReceive :
	public CSocket
{
public:
	CReceive(void);
	~CReceive(void);
	virtual int Receive( void* lpBuf,int nBufLen,int nFlags);
public:
	vector <UCHAR> m_ReceiveData;
public:
	BOOL OnInit();
	void SaveReceiveDataToFile(vector<UCHAR> Buf);
	DWORD GetFrameCount();

};
