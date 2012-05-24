#pragma once
#include "workspacebar.h"
class CVPToDoBar :
	public CWorkSpaceBar
{
public:
	CVPToDoBar(void);
	~CVPToDoBar(void);
	// 设置表头
	bool SetGridHead(void);
	// 载入全部炮点信息
	void LoadShotPoints(void);
private:
	// 载入单条炮点信息
	void LoadShotPoint(void);
};

