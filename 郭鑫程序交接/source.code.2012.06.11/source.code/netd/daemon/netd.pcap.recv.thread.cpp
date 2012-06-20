/**
@brief	定义netd_pcap_recv_thread类
@details	该类主要实现pcap的接受数据功能
@file	netd.pcap.recv.thread.cpp
@note	
@see	
@version	1.0.2.8.2012.02.17
@author	kuoxin	
@bug
*/
#include "..\include\config.h"
#include "netd.application.h"
#include "netd.pcap.serv.h"
#include "netd.pcap.recv.thread.h"
#include "public.h"

BOOLEAN ManageDriver(__in LPCTSTR  DriverName, __in LPCTSTR  ServiceName, __in USHORT   Function);
BOOLEAN SetupDriverName(__inout_bcount_full(BufferLength) PCHAR DriverLocation,	__in ULONG BufferLength);

netd_pcap_recv_thread::netd_pcap_recv_thread(netd_pcap_service* pcap_service_ptr)
												: pcap_service_ptr_(pcap_service_ptr)
{
	stop_event_ = CreateEvent(NULL, TRUE, FALSE, NULL);
}

netd_pcap_recv_thread::~netd_pcap_recv_thread()
{
	if(stop_event_ != NULL)	CloseHandle(stop_event_);
}

void netd_pcap_recv_thread::stop()
{
	if(stop_event_ != NULL) SetEvent(stop_event_);
	if(!wait(100))	terminate();
}

void netd_pcap_recv_thread::run()
{
	int status = 0x0;
	ip_header* ih = NULL;
	unsigned int ip_len = 0x0;
	udp_header* uh = NULL;
	struct pcap_pkthdr* pkt_header = NULL;
	const u_char* pkt_data = NULL;
	unsigned short sport = 0x0;
	unsigned short dport = 0x0;
	inp_data data;
	int i = 0x0;

	REGISTER_EVENT registerEvent;
	HANDLE  hDevice;
	ULONG   ulReturnedLength;
	FLOAT fDelay = 1;
	UINT type = EVENT_BASED;
	DWORD errNum = 0;
	char driverLocation[MAX_PATH] = {0x0};
	TCHAR driverLocation_a[MAX_PATH] = {0x0};

	//open the device
	if((hDevice = CreateFile(
		L"\\\\.\\Event_Sample",                     // lpFileName
		GENERIC_READ | GENERIC_WRITE,       // dwDesiredAccess
		FILE_SHARE_READ | FILE_SHARE_WRITE, // dwShareMode
		NULL,                               // lpSecurityAttributes
		OPEN_EXISTING,                      // dwCreationDistribution
		0,                                  // dwFlagsAndAttributes
		NULL                                // hTemplateFile
		)) == INVALID_HANDLE_VALUE) {

			errNum = GetLastError();
			if (errNum != ERROR_FILE_NOT_FOUND) {
				printf("CreateFile failed!  ERROR_FILE_NOT_FOUND = %d\n", errNum);
				return ;
			}

			// The driver is not started yet so let us the install driver.
			// First setup full path to driver name.
			if (!SetupDriverName(driverLocation, sizeof(driverLocation) )) return ;
			
			mbstowcs(driverLocation_a, driverLocation, strlen(driverLocation) + 1);

			if (!ManageDriver(L"event", driverLocation_a, DRIVER_FUNC_INSTALL)) {
					printf("Unable to install driver. \n");
					// Error - remove driver.
					ManageDriver(L"event", driverLocation_a, DRIVER_FUNC_REMOVE);
					return;
			}

			//
			// Now open the device again.
			//
			hDevice = CreateFile(
				L"\\\\.\\Event_Sample",                     // lpFileName
				GENERIC_READ | GENERIC_WRITE,       // dwDesiredAccess
				FILE_SHARE_READ | FILE_SHARE_WRITE, // dwShareMode
				NULL,                               // lpSecurityAttributes
				OPEN_EXISTING,                      // dwCreationDistribution
				0,                                  // dwFlagsAndAttributes
				NULL                                // hTemplateFile
				);

			if ( hDevice == INVALID_HANDLE_VALUE ){
				printf ( "Error: CreatFile Failed : %d\n", GetLastError());
				return;
			}
	}

	//
	// set the event signal delay. Use relative time for this sample
	//
	registerEvent.DueTime.QuadPart = -((LONGLONG)(fDelay * 10.0E6));
	registerEvent.hEvent = NULL;
	registerEvent.Type = IRP_BASED;

	do{

		status = pcap_next_ex(pcap_service_ptr_->pcap_handle_ptr_, &pkt_header, &pkt_data);
		if(status == 1){

			ih = (ip_header *)(pkt_data + sizeof(ethernet_header));
			ip_len = (ih->ver_ihl & 0xf) * 4;
			uh = (udp_header *)((u_char*)ih + ip_len);

			//if(//uh->dport == pcap_service_ptr_->application_ptr_->lci_inp_port_ && 
			//	pcap_service_ptr_->application_ptr_->lci_ip_ == ih->saddr &&
			//	pcap_service_ptr_->application_ptr_->netd_ip_ == ih->daddr){

				for(i = 0; i < pcap_service_ptr_->application_ptr_->lci_inp_port_size_; ++i)
					if(uh->dport == pcap_service_ptr_->application_ptr_->lci_inp_port_[i]) break;
				
				if(i >= pcap_service_ptr_->application_ptr_->lci_inp_port_size_) continue;

				InterlockedIncrement(&pcap_service_ptr_->application_ptr_->pcap_data_inp_num_);
				data.copy(pkt_data + 42, inp_data::MAX_IN_BUFFER_SIZE);
				pcap_service_ptr_->application_ptr_->inp_queue_->push(data);
//			}
		}

		DeviceIoControl(
			hDevice,                // Handle to device
			IOCTL_REGISTER_EVENT,        // IO Control code
			&registerEvent,              // Input Buffer to driver.
			SIZEOF_REGISTER_EVENT,        // Length of input buffer in bytes.
			NULL,                   // Output Buffer from driver.
			0,                      // Length of output buffer in bytes.
			&ulReturnedLength,      // Bytes placed in buffer.
			NULL                    // synchronous call
			);

		//wait...
//		if(WaitForSingleObject(stop_event_, 0) == WAIT_OBJECT_0)	break;
	}while(status >= 0);


	// close the handle to the device.
	CloseHandle(hDevice);
	// Unload the driver.  Ignore any errors.
	ManageDriver(L"event", driverLocation_a, DRIVER_FUNC_REMOVE);
}
