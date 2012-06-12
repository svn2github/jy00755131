#ifndef __REGISTRY_OPTR_H__
#define __REGISTRY_OPTR_H__

/*	registry	*/
int read_registry(APPLICATION_ENVIRONMENT_PTR env_ptr);
int write_registry(APPLICATION_ENVIRONMENT_PTR env_ptr);

bool open_registry_private(PHKEY key_handler, DWORD* disposition);
int read_registry_private(APPLICATION_ENVIRONMENT_PTR env_ptr, HKEY key_handler);
int write_registry_private(APPLICATION_ENVIRONMENT_PTR env_ptr, HKEY key_handler);
void close_registry_private(HKEY key_handler);

#endif //__REGISTRY_OPTR_H__