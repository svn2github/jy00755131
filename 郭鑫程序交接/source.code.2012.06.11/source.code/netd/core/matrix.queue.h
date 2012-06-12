/**
@brief	定义matrix_queue基类
@details	该类主要实现队列机制
@file	matrix.queue.h
@note	
@see	
@version	1.0.2.8.2012.02.15
@author	kuoxin	
@bug
*/
#ifndef __MATRIX_QUEUE_H__
#define __MATRIX_QUEUE_H__

#define DEFAULT_QUEUE_SIZE 100 

template<typename T> class matrix_queue
{
public:
	matrix_queue(unsigned int size)	:current_size_(0), max_size_(size), front_(0), rear_(0){
		if(size > 0){	data_ = new T[max_size_];}
		else{			data_ = new T[max_size_ = DEFAULT_QUEUE_SIZE];}

		InitializeCriticalSection(&lock_);
	};

	~matrix_queue(){
		if(max_size_ > 0){
			delete []data_;
			data_ = NULL;
			max_size_ = current_size_ = front_ = rear_ = 0x0;
		}

		DeleteCriticalSection(&lock_);
	};

public:
	void clear(){
		EnterCriticalSection(&lock_);
		front_ = rear_ = 0x0;
		LeaveCriticalSection(&lock_);
	};

	bool empty(){
		bool is_empty = true;
		EnterCriticalSection(&lock_);
		is_empty = (front_ == rear_ ? true : false);
		LeaveCriticalSection(&lock_);
		return is_empty;
	};

	T* front(){
		T* t = NULL;
		EnterCriticalSection(&lock_);
		if(front_ != rear_)	t = data_ + front_;
		LeaveCriticalSection(&lock_);
		return t;
	};

	bool push(T& data){
		bool is_ok = true;

		EnterCriticalSection(&lock_);
		if(((rear_ + 1) % max_size_) == front_){
			LeaveCriticalSection(&lock_);
			return is_ok = false;
		}

		data_[rear_] = data;
		rear_ = (rear_ + 1) % max_size_;
		LeaveCriticalSection(&lock_);

		return is_ok;
	};

	bool pop(T& data){

		bool is_ok = false;
		EnterCriticalSection(&lock_);
		if(rear_ == front_){
			LeaveCriticalSection(&lock_);
			return is_ok;
		}

		data = *(data_ + front_);
		front_ = (front_ + 1) % max_size_;
		LeaveCriticalSection(&lock_);
		return is_ok = true;
	};

	int size(){
		int length = 0;
		EnterCriticalSection(&lock_);
		length = rear_ - front_ + max_size_;
		LeaveCriticalSection(&lock_);
		length %= max_size_;
		return length;
	};

private:
	unsigned int current_size_;
	unsigned int max_size_;
	T* data_;
	CRITICAL_SECTION lock_;
	int front_;
	int rear_;
};

#endif /*__MATRIX_QUEUE_H__*/