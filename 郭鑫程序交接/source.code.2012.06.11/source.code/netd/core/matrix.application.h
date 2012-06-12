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
#ifndef __MATRIX_APPLICATION_H__
#define __MATRIX_APPLICATION_H__

#include "matrix.application.private.h"

class matrix_application:public matrix_application_private
{
public:
	matrix_application(int argc, char_t **argv, char_t **envp = NULL);
	virtual ~matrix_application();
protected:
	int run_private();
	bool init_instance_private();
	void release_instance_priate();
};

#endif /*__MATRIX_APPLICATION_H__*/
