/**
@brief	定义matrix_thread类
@details	该类主要实现线程机制
@file	matrix.thread.h
@note	
@see	
@version	1.0.2.8.2012.02.14
@author	kuoxin	
@bug
*/
#ifndef __MATRIX_THREAD_H__
#define __MATRIX_THREAD_H__

#include "matrix.object.h"

class matrix_thread : public matrix_object
{
public:
	matrix_thread(matrix_object* parent_object_ptr = NULL);
	virtual ~matrix_thread();

public:

	void start(unsigned int stack_size = 0x0, unsigned int flag = 0x0);
	void terminate();
	static bool wait(HANDLE thread_handle, unsigned int timeout);
	bool wait(unsigned int timeout);

private:
	static unsigned int __stdcall thread_proc(void* args);

protected:
	virtual void run();
private:
	HANDLE thread_handle_;
	unsigned int thread_id_;
	unsigned int flag_;
	unsigned int stack_size_;
};

#endif /*__MATRIX_THREAD_H__*/