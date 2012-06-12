/**
@brief	定义matrix_object基类
@details	该类主要实现基本机制
@file	matrix.object.cpp
@note	
@see	
@version	1.0.0.1.2011.12.07
@author	kuoxin	
@bug
*/
#include "..\include\config.h"
#include "matrix.object.h"

matrix_object::matrix_object(matrix_object* parent_object_ptr):parent_object_ptr_(parent_object_ptr)
{

}

matrix_object::~matrix_object()
{

}
