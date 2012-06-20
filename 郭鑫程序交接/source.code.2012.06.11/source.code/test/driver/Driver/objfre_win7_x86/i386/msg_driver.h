///////////////////////////////////////////////////////////////////////////////
///
/// Copyright (c) 2011 - <company name here>
///
/// Original filename: Driver.h
/// Project          : Driver
/// Date of creation : 2011-12-19
/// Author(s)        : <see Driver.cpp>
///
/// Purpose          : Message table.
///
/// Revisions:         <see Driver.cpp>
///
///////////////////////////////////////////////////////////////////////////////

// $Id$

#ifndef __MSG_DRIVER_MC_VERSION__
#define __MSG_DRIVER_MC_VERSION__ 100
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/*
MessageIdTypedef=DRIVER_STATUS

SeverityNames=(Success=0x0:STATUS_SEVERITY_SUCCESS
               Informational=0x1:STATUS_SEVERITY_INFORMATIONAL
               Warning=0x2:STATUS_SEVERITY_WARNING
               Error=0x3:STATUS_SEVERITY_ERROR
              )

FacilityNames=(System=0x0
               DriverWhatever=0x1:FACILITY_DRIVER_WHATEVER
              )

MessageId=0x0001 Facility=System Severity=Informational SymbolicName=DRIVERSTATUS_SOME_MESSAGE
Language=English
Some message.
.

*/

#endif // __DRIVER_MC_VERSION__
