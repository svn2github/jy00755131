#ifndef __MATRIX_HEARTBEAT_H__
#define __MATRIX_HEARTBEAT_H__

bool init_heartbeat_proc(TCHAR const* remote_ip);
void release_heartbeat_proc();
void heartbeat_proc();
BOOL on_receive_heartbeat_package(WPARAM wparam, LPARAM lparam);

#endif//__MATRIX_HEARTBEAT_H__