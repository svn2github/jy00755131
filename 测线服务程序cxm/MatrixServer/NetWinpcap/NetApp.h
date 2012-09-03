#pragma once
class CNetApp
{
public:
	CNetApp(int argc, _TCHAR* argv[]);
	~CNetApp(void);

private:
	void PhraseCommandLine(_TCHAR* argv);
};

