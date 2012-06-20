// dead.cycle.console.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment(lib, "Winmm")

#define TARGET_RESOLUTION 1         // 1-millisecond target resolution

double freq;
ULONGLONG counter;
int i = 0;
double val_array[50000] = {0.0};
UINT event_id = 0x0;

void CALLBACK TimeProc(UINT uID,      
					   UINT uMsg,     
					   DWORD dwUser,  
					   DWORD dw1,     
					   DWORD dw2      
					   )
{
	double val;
	LARGE_INTEGER cur_counter;	

	QueryPerformanceCounter(&cur_counter);
	
	val = (double)(cur_counter.QuadPart - counter);
	val_array[i++] = (val/freq) * 1000;
	if(i == 50000){ printf("OK!\n"); timeKillEvent(event_id);}

	counter = cur_counter.QuadPart;
}

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int i = 0;

	LARGE_INTEGER li;
	li.QuadPart = 100;

	while(true) NULL;

/*
	LARGE_INTEGER li_freq;

	QueryPerformanceFrequency(&li_freq);
	freq = (double)li_freq.QuadPart;

	CloseHandle(
		CreateThread(NULL, NULL, ThreadProc, NULL,0,NULL)
	);

	MMRESULT result;
	TIMECAPS tc;
	UINT     wTimerRes;

	timeGetDevCaps(&tc, sizeof(TIMECAPS));

	wTimerRes = min(max(tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);
	timeBeginPeriod(wTimerRes); 	

	event_id = timeSetEvent(1, 0, TimeProc, NULL, TIME_PERIODIC|TIME_CALLBACK_FUNCTION);

	timeEndPeriod(1);


	Sleep(60000);

	FILE* file_ptr = NULL;
	file_ptr = fopen("d:\\timeSetEvent.txt", "w");
	for(int i = 0; i < 50000; ++i){
		fprintf(file_ptr, "%f\n", val_array[i]);
	}

	fclose(file_ptr);
*/
	return 0;
}

