#pragma once

#include "resource.h"

BOOL WINAPI main_Wnd_proc(HWND, UINT, WPARAM, LPARAM);
BOOL init_dialog(HWND, HWND, LPARAM);
BOOL close_dialog(HWND);
BOOL destroy_dialog(HWND);

BOOL command_message(HWND, int, HWND, UINT);

BOOL do_menu(HWND, WORD);
BOOL do_Accelerator(HWND, WORD);
BOOL do_control(HWND, int, HWND, UINT);


VOID CALLBACK recv_timer_proc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
VOID CALLBACK snd_timer_proc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
VOID CALLBACK timer_proc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

unsigned __stdcall udp_recv_thread_proc(void* args);
unsigned __stdcall udp_snd_thread_proc(void* args);

void start_socket_button_down();
void recv_button_down();
void snd_button_down();
