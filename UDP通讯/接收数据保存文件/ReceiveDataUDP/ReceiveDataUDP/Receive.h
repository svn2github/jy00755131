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
	virtual void OnReceive(int nErrorCode);
public:
	vector <UCHAR> m_ReceiveData;
public:
	BOOL OnInit();
};
