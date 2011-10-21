#pragma once

// CEepromSocket ÃüÁîÄ¿±ê

class CEepromSocket : public CSocket
{
public:
	CEepromSocket();
	virtual ~CEepromSocket();
	virtual void OnReceive(int nErrorCode);
};


