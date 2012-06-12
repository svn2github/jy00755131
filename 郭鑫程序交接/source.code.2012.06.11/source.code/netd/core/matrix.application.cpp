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
#include "../include/config.h"
#include "matrix.application.h"

matrix_application::matrix_application(int argc, char_t **argv, char_t **envp)
			:matrix_application_private(argc, argv, envp)
{
}

matrix_application::~matrix_application(){}

int matrix_application::run_private(){return APP_SUCCESS;}
bool matrix_application::init_instance_private(){return true;}
void matrix_application::release_instance_priate(){}
