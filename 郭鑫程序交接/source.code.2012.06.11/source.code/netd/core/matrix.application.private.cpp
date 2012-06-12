/**
@brief	应用程序框架类私有类
@details 定义整个应用程序容器私有类,该类不能被直接使用
@file	matrix.application.private.h
@note	
@see	
@version	1.0.0.1.2011.12.07
@author	kuoxin	
@bug
*/
#include "../include/config.h"
#include "matrix.application.private.h"

matrix_application_private::matrix_application_private(int argc, char_t **argv, char_t **envp)
							:argc_(argc), argv_(argv), envp_(envp),matrix_object(NULL)	
{
}

matrix_application_private::~matrix_application_private()
{
	release_instance_private();
}

int matrix_application_private::run()
{
	if(!init_instance_private()){
		
		return 0;
	}

	return run_private();
}

void matrix_application_private::release_instance_private(){}
