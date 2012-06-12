#include "../common/config.h"
#include "pcid.h"
#include "output.queue.h"

#define MAX_PARALLEL_QUEUE_SIZE	20000

typedef struct __tagPARALLEL_OUTPUT_QUEUE
{
	PARALLEL_OUTPUT_QUEUE_ITEM items_[MAX_PARALLEL_QUEUE_SIZE];
	CRITICAL_SECTION lock_;
	int front_;
	int rear_;
}PARALLEL_OUTPUT_QUEUE, *PARALLEL_OUTPUT_QUEUE_PTR;

PARALLEL_OUTPUT_QUEUE __parallel_output_queue;


void parallel_output_queue_init()
{
	SecureZeroMemory(__parallel_output_queue.items_, sizeof(__parallel_output_queue.items_));
	__parallel_output_queue.front_ = __parallel_output_queue.rear_ = 0;
	InitializeCriticalSection(&__parallel_output_queue.lock_);
}

void parallel_output_queue_destroy()
{
	DeleteCriticalSection(&__parallel_output_queue.lock_);
}

void parallel_output_queue_clear()
{
	EnterCriticalSection(&__parallel_output_queue.lock_);
	__parallel_output_queue.front_ = __parallel_output_queue.rear_ = 0;
	LeaveCriticalSection(&__parallel_output_queue.lock_);
}

bool parallel_output_queue_push(PARALLEL_OUTPUT_QUEUE_ITEM_PTR item)
{
	bool is_ok = true;

	EnterCriticalSection(&__parallel_output_queue.lock_);
	if(((__parallel_output_queue.rear_ + 1) % MAX_PARALLEL_QUEUE_SIZE) == __parallel_output_queue.front_){

			LeaveCriticalSection(&__parallel_output_queue.lock_);
			return is_ok = false;
	}

	memcpy(__parallel_output_queue.items_ + __parallel_output_queue.rear_, 
			item, sizeof(PARALLEL_OUTPUT_QUEUE_ITEM));

	__parallel_output_queue.rear_ = (__parallel_output_queue.rear_ + 1) % MAX_PARALLEL_QUEUE_SIZE;
	
	LeaveCriticalSection(&__parallel_output_queue.lock_);

	return is_ok;
}

bool parallel_output_queue_pop(PARALLEL_OUTPUT_QUEUE_ITEM_PTR item)
{
	bool is_ok = true;

	EnterCriticalSection(&__parallel_output_queue.lock_);

	if(__parallel_output_queue.rear_ == __parallel_output_queue.front_){

		LeaveCriticalSection(&__parallel_output_queue.lock_);
		return is_ok = false;
	}

	memcpy(item, __parallel_output_queue.items_ + __parallel_output_queue.front_, sizeof(PARALLEL_OUTPUT_QUEUE_ITEM));
	__parallel_output_queue.front_ = (__parallel_output_queue.front_ + 1) % MAX_PARALLEL_QUEUE_SIZE;

	LeaveCriticalSection(&__parallel_output_queue.lock_);

	return is_ok;
}

bool parallel_output_queue_is_empty()
{
	bool is_empty = true;
	EnterCriticalSection(&__parallel_output_queue.lock_);
	is_empty = __parallel_output_queue.front_ == __parallel_output_queue.rear_ ? true : false;	
	LeaveCriticalSection(&__parallel_output_queue.lock_);
	return is_empty;
}

bool parallel_output_queue_get_head(PARALLEL_OUTPUT_QUEUE_ITEM_PTR item)
{
	bool is_ok = true;
	EnterCriticalSection(&__parallel_output_queue.lock_);
	if(__parallel_output_queue.front_ == __parallel_output_queue.rear_) is_ok = false;
	else item = __parallel_output_queue.items_ + __parallel_output_queue.front_;
	LeaveCriticalSection(&__parallel_output_queue.lock_);
	return is_ok;
}

int parallel_output_queue_get_size()
{
	int length = 0;
	EnterCriticalSection(&__parallel_output_queue.lock_);
	length = __parallel_output_queue.rear_ - __parallel_output_queue.front_ + MAX_PARALLEL_QUEUE_SIZE;
	LeaveCriticalSection(&__parallel_output_queue.lock_);
	length %= MAX_PARALLEL_QUEUE_SIZE;
	return length;
}