#if !defined(__OUTPUT_QUEUE_H__)
#define __OUTPUT_QUEUE_H__

typedef struct __tagPARALLEL_OUTPUT_QUEUE_ITEM
{
	enum{MAX_BUFFER_SIZE = 128,};
	unsigned char buf_[MAX_BUFFER_SIZE];
//	SOCKADDR_IN ip_;
}PARALLEL_OUTPUT_QUEUE_ITEM, *PARALLEL_OUTPUT_QUEUE_ITEM_PTR;

void parallel_output_queue_init();
void parallel_output_queue_destroy();
void parallel_output_queue_clear();
bool parallel_output_queue_is_empty();
bool parallel_output_queue_get_head(PARALLEL_OUTPUT_QUEUE_ITEM_PTR);
bool parallel_output_queue_push(PARALLEL_OUTPUT_QUEUE_ITEM_PTR/* [in] items*/);
bool parallel_output_queue_pop(PARALLEL_OUTPUT_QUEUE_ITEM_PTR/* [in/out] pre-allocation memory*/);
int parallel_output_queue_get_size();
void parallel_output_queue_traverse(void (*do_it)(PARALLEL_OUTPUT_QUEUE_ITEM_PTR));

#endif //__OUTPUT_QUEUE_H__

