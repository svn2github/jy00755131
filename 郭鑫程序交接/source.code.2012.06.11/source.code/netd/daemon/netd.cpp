/**
@brief	
@details	
@file	
@note	
@see	
@version	1.0.0.1.2011.12.07
@author	kuoxin	
@bug
*/
#include "..\include\config.h"
#include "netd.h"
#include "netd.application.h"

int _tmain(int argc, char_t **argv, char_t **envp)
{
	netd_application netd_app(argc, argv, envp);
	return netd_app.run();
}
