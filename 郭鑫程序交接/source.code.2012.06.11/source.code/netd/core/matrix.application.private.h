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
#ifndef __MATRIX_APPLICATION_PRIVATE_H__
#define __MATRIX_APPLICATION_PRIVATE_H__

#include "matrix.object.h"

class matrix_application_private:public matrix_object
{
public:
	matrix_application_private(int argc, char_t **argv, char_t **envp = NULL);
	virtual ~matrix_application_private();

public: 
	int run();

protected:
	virtual int run_private() = 0;
	virtual bool init_instance_private() = 0;
	virtual void release_instance_private();
protected:
	int argc_;
	char_t** argv_;
	char_t** envp_;
};

#endif /*__MATRIX_APPLICATION_PRIVATE_H__*/
