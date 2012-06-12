/**
@brief	驱动入口函数文件
@details	定义了驱动程序使用的主体结构
@file	timerd.c	
@note	
@see	
@version	1.0.0.1.2011.12.14
@author	kuoxin	
@bug
*/

#include <ntddk.h>
#include "..\include\guiid.h"
#include "hpetd.h"

NTSTATUS DriverEntry( IN PDRIVER_OBJECT  DriverObject, 	IN PUNICODE_STRING  RegistryPath)
{
	NTSTATUS status;

	return status
}

NTSTATUS AddDevice( IN PDRIVER_OBJECT  DriverObject,  IN PDEVICE_OBJECT  PhysicalDeviceObject )
{

}


