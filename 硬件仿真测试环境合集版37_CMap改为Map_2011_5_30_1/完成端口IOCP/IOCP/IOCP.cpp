// IOCP.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#define PORT 5150
#define DATA_BUFSIZE 8192

#pragma comment(lib, "Ws2_32")

typedef struct
{
	OVERLAPPED Overlapped;
	WSABUF DataBuf;
	CHAR Buffer[DATA_BUFSIZE];
	DWORD BytesSEND;
	DWORD BytesRECV;
} PER_IO_OPERATION_DATA, * LPPER_IO_OPERATION_DATA;


typedef struct 
{
	SOCKET Socket;
} PER_HANDLE_DATA, * LPPER_HANDLE_DATA;

DWORD WINAPI ServerWorkerThread(LPVOID CompletionPortID);

int _tmain(int argc, _TCHAR* argv[])
{
	SOCKADDR_IN InternetAddr;
	SOCKET Listen;
	SOCKET Accept;
	HANDLE CompletionPort;
	SYSTEM_INFO SystemInfo;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_OPERATION_DATA PerIoData;
	DWORD i;
	DWORD RecvBytes;
	DWORD Flags;
	DWORD ThreadID;
	WSADATA wsaData;
	DWORD Ret;

	if ((Ret = WSAStartup(0x0202, &wsaData)) != 0)
	{
		printf("WSAStartup failed with error %d/n", Ret);
		return 0;
	}

	// Setup an I/O completion port.
	// 1） 创建一个完成端口。第四个参数保持为0，指定在完成端口上，每个处理器一次只允许
	//	执行一个工作者线程。

	if ((CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
	{
		printf( "CreateIoCompletionPort failed with error: %d/n", GetLastError());
		return 0;
	}

	// Determine how many processors are on the system.
	// 2) 判断系统内到底安装了多少个处理器。
	GetSystemInfo(&SystemInfo);

	// Create worker threads based on the number of processors available on the
	// system. Create two worker threads for each processor.
	// 	3) 创建工作者线程，根据步骤2 )得到的处理器信息，在完成端口上，为已完成的 I / O请求
	// 		提供服务。在这个简单的例子中，我们为每个处理器都只创建一个工作者线程。这是由于事
	// 		先已预计到，到时不会有任何线程进入“挂起”状态，造成由于线程数量的不足，而使处理
	// 		器空闲的局面（没有足够的线程可供执行）。调用CreateThread函数时，必须同时提供一个工
	// 		作者例程，由线程在创建好执行。
	for(i = 0; i < SystemInfo.dwNumberOfProcessors * 2; i++)
	{
		HANDLE ThreadHandle;

		// Create a server worker thread and pass the completion port to the thread.

		if ((ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, CompletionPort,
			0, &ThreadID)) == NULL)
		{
			printf("CreateThread() failed with error %d/n", GetLastError());
			return 0;
		}

		// Close the thread handle
		CloseHandle(ThreadHandle);
	}

	// Create a listening socket
	// 4) 准备好一个监听套接字，在端口5150上监听进入的连接请求。
	if ((Listen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0,
		WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		printf("WSASocket() failed with error %d/n", WSAGetLastError());
		return 0;
	} 

	InternetAddr.sin_family = AF_INET;
	InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	InternetAddr.sin_port = htons(PORT);

	if (bind(Listen, (PSOCKADDR) &InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
	{
		printf("bind() failed with error %d/n", WSAGetLastError());
		return 0;
	}

	// Prepare socket for listening

	if (listen(Listen, 5) == SOCKET_ERROR)
	{
		printf("listen() failed with error %d/n", WSAGetLastError());
		return 0;
	}

	// Accept connections and assign to the completion port.
	// 5) 使用accept函数，接受进入的连接请求。
	while(TRUE)
	{
		if ((Accept = WSAAccept(Listen, NULL, NULL, NULL, 0)) == SOCKET_ERROR)
		{
			printf("WSAAccept() failed with error %d/n", WSAGetLastError());
			return 0;
		}

		// Create a socket information structure to associate with the socket
		// 6) 创建一个数据结构，用于容纳“单句柄数据”，同时在结构中存入接受的套接字句柄。
		if ((PerHandleData = (LPPER_HANDLE_DATA) GlobalAlloc(GPTR, 
			sizeof(PER_HANDLE_DATA))) == NULL)
		{
			printf("GlobalAlloc() failed with error %d/n", GetLastError());
			return 0;
		}

		// Associate the accepted socket with the original completion port.
		// 7) 调用CreateIoComletionPort，将自accept返回的新套接字句柄同完成端口关联到一起。
		// 通过完成键（CompletionKey）参数，将单句柄数据结构传递给CreateIoComletionPort。

		printf("Socket number %d connected/n", Accept);
		PerHandleData->Socket = Accept;
		
		if (CreateIoCompletionPort((HANDLE) Accept, CompletionPort, (DWORD) PerHandleData,
			0) == NULL)
		{
			printf("CreateIoCompletionPort failed with error %d/n", GetLastError());
			return 0;
		}

		// Create per I/O socket information structure to associate with the 
		// WSARecv call below.
		// 8) 开始在已接受的连接上进行 I / O操作。在此，我们希望通过重叠 I / O机制，在新建的套
		// 接字上投递一个或多个异步 WSARecv或WSASend请求。这些I / O请求完成后，一个工作者线
		//	程会为I / O请求提供服务，同时继续处理未来的 I / O请求，稍后便会在步骤3 )指定的工作者例程
		//	中，体验到这一点。


		if ((PerIoData = (LPPER_IO_OPERATION_DATA) GlobalAlloc(GPTR,          sizeof(PER_IO_OPERATION_DATA))) == NULL)
		{
			printf("GlobalAlloc() failed with error %d/n", GetLastError());
			return 0;
		}

		ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
		PerIoData->BytesSEND = 0;
		PerIoData->BytesRECV = 0;
		PerIoData->DataBuf.len = DATA_BUFSIZE;
		PerIoData->DataBuf.buf = PerIoData->Buffer;

		Flags = 0;
		if (WSARecv(Accept, &(PerIoData->DataBuf), 1, &RecvBytes, &Flags,
			&(PerIoData->Overlapped), NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				printf("WSARecv() failed with error %d/n", WSAGetLastError());
				return 0;
			}
		}
	}
}

DWORD WINAPI ServerWorkerThread(LPVOID CompletionPortID)
{
	HANDLE CompletionPort = (HANDLE) CompletionPortID;
	DWORD BytesTransferred;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_OPERATION_DATA PerIoData;
	DWORD SendBytes, RecvBytes;
	DWORD Flags;

	while(TRUE)
	{

		if (GetQueuedCompletionStatus(CompletionPort, &BytesTransferred,
			(LPDWORD)&PerHandleData, (LPOVERLAPPED *) &PerIoData, INFINITE) == 0)
		{
			printf("GetQueuedCompletionStatus failed with error %d/n", GetLastError());
			return 0;
		}


		// First check to see if an error has occured on the socket and if so
		// then close the socket and cleanup the SOCKET_INFORMATION structure
		// associated with the socket.

		if (BytesTransferred == 0)
		{
			printf("Closing socket %d/n", PerHandleData->Socket);

			if (closesocket(PerHandleData->Socket) == SOCKET_ERROR)
			{
				printf("closesocket() failed with error %d/n", WSAGetLastError());
				return 0;
			}
			// GlobalFree函数释放开辟的内存空间
			GlobalFree(PerHandleData);
			GlobalFree(PerIoData);
			continue;
		}

		// Check to see if the BytesRECV field equals zero. If this is so, then
		// this means a WSARecv call just completed so update the BytesRECV field
		// with the BytesTransferred value from the completed WSARecv() call.

		if (PerIoData->BytesRECV == 0)
		{
			PerIoData->BytesRECV = BytesTransferred;
			PerIoData->BytesSEND = 0;
		}
		else
		{
			PerIoData->BytesSEND += BytesTransferred;
		}

		if (PerIoData->BytesRECV > PerIoData->BytesSEND)
		{

			// Post another WSASend() request.
			// Since WSASend() is not gauranteed to send all of the bytes requested,
			// continue posting WSASend() calls until all received bytes are sent.

			ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));

			PerIoData->DataBuf.buf = PerIoData->Buffer + PerIoData->BytesSEND;
			PerIoData->DataBuf.len = PerIoData->BytesRECV - PerIoData->BytesSEND;

			if (WSASend(PerHandleData->Socket, &(PerIoData->DataBuf), 1, &SendBytes, 0,
				&(PerIoData->Overlapped), NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					printf("WSASend() failed with error %d/n", WSAGetLastError());
					return 0;
				}
			}
		}
		else
		{
			PerIoData->BytesRECV = 0;

			// Now that there are no more bytes to send post another WSARecv() request.

			Flags = 0;
			// ZeroMemory函数把内存填为0
			ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));

			PerIoData->DataBuf.len = DATA_BUFSIZE;
			PerIoData->DataBuf.buf = PerIoData->Buffer;

			if (WSARecv(PerHandleData->Socket, &(PerIoData->DataBuf), 1, &RecvBytes, &Flags,
				&(PerIoData->Overlapped), NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					printf("WSARecv() failed with error %d/n", WSAGetLastError());
					return 0;
				}
			}
		}
	}
}

