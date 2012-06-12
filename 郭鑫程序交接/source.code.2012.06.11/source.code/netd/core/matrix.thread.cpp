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
#include "matrix.thread.h"

matrix_thread::matrix_thread(matrix_object* parent_object_ptr)
							: matrix_object(parent_object_ptr)
{
	thread_handle_ = NULL;
	thread_id_ = 0x0;
	flag_ = 0x0;
	stack_size_ = 0x0;
}

matrix_thread::~matrix_thread()
{

}

void matrix_thread::start(unsigned int stack_size, unsigned int flag)
{
	stack_size_ = stack_size;
	flag_ = flag;
	thread_handle_ = (HANDLE)_beginthreadex(NULL, stack_size_, matrix_thread::thread_proc, 
									this, flag_, & thread_id_);
}

bool matrix_thread::wait(HANDLE thread_handle, unsigned int timeout)
{
	bool is_ok = true;
	if(thread_handle){
		if(WaitForSingleObject(thread_handle, timeout) != WAIT_OBJECT_0) is_ok = false;
	}
	return is_ok;
}

bool matrix_thread::wait(unsigned int timeout)
{
	bool is_ok = true;
	if(thread_handle_)
		if(WaitForSingleObject(thread_handle_, timeout) != WAIT_OBJECT_0) is_ok = false;
	return is_ok;
}


void matrix_thread::terminate()
{
	TerminateThread(thread_handle_, 0x0);
}

unsigned int __stdcall matrix_thread::thread_proc(void* args)
{
	matrix_thread* thrd = (matrix_thread*) args;
	thrd->run();
	_endthreadex(0);
	return 0; 
}

void matrix_thread::run()
{
}

