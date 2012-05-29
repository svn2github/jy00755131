#pragma once
#include "workspacebar.h"
class CAllVPBar :
	public CWorkSpaceBar
{
public:
	CAllVPBar(void);
	~CAllVPBar(void);
	// 设置表头
	bool SetGridHead(void);
	// 载入全部炮点信息
	void LoadShotPoints(void);
private:
	// 载入单条炮点信息
	void LoadShotPoint(void);

};

