/**
@brief	定义matrix_object基类
@details	该类主要实现基本机制
@file	matrix.object.h
@note	
@see	
@version	1.0.0.1.2011.12.07
@author	kuoxin	
@bug
*/
#ifndef __MATRIX_OBJECT_H__
#define __MATRIX_OBJECT_H__

class matrix_object
{
public:
	matrix_object(matrix_object* parent_object_ptr = NULL);
	virtual ~matrix_object();
public:
	matrix_object* get_parent_ptr(){return parent_object_ptr_;};
protected:
	matrix_object* parent_object_ptr_;
};

#endif /*__MATRIX_OBJECT_H__*/