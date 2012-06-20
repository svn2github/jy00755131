///////////////////////////////////////////////////////////////////////////////
///
/// Copyright (c) 2011 - <company name here>
///
/// Original filename: Driver.h
/// Project          : Driver
/// Date of creation : <see Driver.cpp>
/// Author(s)        : <see Driver.cpp>
///
/// Purpose          : <see Driver.cpp>
///
/// Revisions:         <see Driver.cpp>
///
///////////////////////////////////////////////////////////////////////////////

// $Id$

#ifndef __DRIVER_H_VERSION__
#define __DRIVER_H_VERSION__ 100

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#include "msg_Driver.h"
#include "drvcommon.h"
#include "drvversion.h"

#define DEVICE_NAME			"\\Device\\DRIVER_DeviceName"
#define SYMLINK_NAME		"\\DosDevices\\DRIVER_DeviceName"
PRESET_UNICODE_STRING(usDeviceName, DEVICE_NAME);
PRESET_UNICODE_STRING(usSymlinkName, SYMLINK_NAME);

#ifndef FILE_DEVICE_DRIVER
#define FILE_DEVICE_DRIVER 0x8000
#endif

// Values defined for "Method"
// METHOD_BUFFERED
// METHOD_IN_DIRECT
// METHOD_OUT_DIRECT
// METHOD_NEITHER
// 
// Values defined for "Access"
// FILE_ANY_ACCESS
// FILE_READ_ACCESS
// FILE_WRITE_ACCESS

const ULONG IOCTL_DRIVER_OPERATION = CTL_CODE(FILE_DEVICE_DRIVER, 0x01, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA);

#endif // __DRIVER_H_VERSION__
