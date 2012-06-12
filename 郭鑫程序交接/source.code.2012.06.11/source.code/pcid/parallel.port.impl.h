#ifndef __PARALLEL_PORT_IMPLE_H__
#define __PARALLEL_PORT_IMPLE_H__

#pragma once

typedef unsigned (*parallel_inputting_thread_callback)(unsigned char*, unsigned);
typedef void (*parallel_inputting_callback_function)();

bool init_parallel_port(unsigned, unsigned, unsigned, unsigned);

int output_data_into_parallel_port(unsigned char*, unsigned); //have a error, return -1, otherwise return 0;
int output_data_into_parallel_port_private(unsigned char*, unsigned);

bool start_inputting_callback(parallel_inputting_callback_function);
void stop_inputting_callback();

bool start_inputting_thread(parallel_inputting_thread_callback);
void stop_inputting_thread();

unsigned __stdcall input_thread_proc(void*);

void release_parallel_port();

#endif //__PARALLEL_PORT_IMPLE_H__
