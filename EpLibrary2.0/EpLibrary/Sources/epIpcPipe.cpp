/*! 
IpcPipe for the EpLibrary

The MIT License (MIT)

Copyright (c) 2008-2013 Woong Gyu La <juhgiyo@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "epIpcPipe.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;



IpcPipe::IpcPipe(EpTString pipeName, IpcServerOps options,epl::LockPolicy lockPolicyType): SmartObject(lockPolicyType)
{
	m_pipeName=pipeName;
	m_options=options;

	m_pipeEvent=EventEx(true,true);

	m_overlap.hEvent=m_pipeEvent.GetEventHandle();

	m_readBuffer=EP_NEW char[options.numOfReadBytes]; 

	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_writeQueueLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_writeQueueLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_writeQueueLock=EP_NEW NoLock();
		break;
	default:
		m_writeQueueLock=NULL;
		break;
	}
	m_lockPolicy=lockPolicyType;

}
IpcPipe::~IpcPipe()
{
	KillConnection();
	if(m_readBuffer)
		EP_DELETE m_readBuffer;
	if(m_writeQueueLock)
		EP_DELETE m_writeQueueLock;
}
bool IpcPipe::Create()
{
	m_pipeHandle=CreateNamedPipe(m_pipeName.c_str(),
		PIPE_ACCESS_DUPLEX|FILE_FLAG_OVERLAPPED|FILE_FLAG_WRITE_THROUGH,
		PIPE_TYPE_MESSAGE|PIPE_READMODE_MESSAGE|PIPE_WAIT,
		m_options.maximumInstances,
		m_options.numOfWriteBytes,
		m_options.numOfReadBytes,
		m_options.waitTimeInMilliSec,
		NULL);

	if (m_pipeHandle == INVALID_HANDLE_VALUE) 
	{
		epl::System::OutputDebugString(_T("CreateNamedPipe failed with %d.\r\n"), GetLastError());
		return false;
	}
	m_pendingIO=connectToNewClient(m_pipeHandle,&(m_overlap));
	m_pipeState = m_pendingIO ? 
		PIPE_STATE_CONNECTING : // still connecting 
		PIPE_STATE_READING;     // ready to read 
	return true;
}


bool IpcPipe::IsConnectionAlive() const
{
	return (m_pipeState!=PIPE_STATE_CONNECTING);
		
}
void IpcPipe::KillConnection()
{
	if(IsConnectionAlive())
		disconnect();
}

void IpcPipe::SetCallbackObject(IpcServerCallbackInterface *callBackObj)
{
	m_options.callBackObj=callBackObj;
}

IpcServerCallbackInterface *IpcPipe::GetCallbackObject()
{
	return m_options.callBackObj;
}

HANDLE IpcPipe::GetEventHandle()
{
	return m_pipeEvent.GetEventHandle();
}

bool IpcPipe::connectToNewClient(HANDLE pipeHandle,OVERLAPPED *overlappedIO )
{
	bool connected, pendingIO = false; 

	// Start an overlapped connection for this pipe instance. 
	connected = ConnectNamedPipe(pipeHandle, overlappedIO); 

	// Overlapped ConnectNamedPipe should return zero. 
	if (connected) 
	{
		epl::System::OutputDebugString(_T("ConnectNamedPipe failed with %d.\r\n"), GetLastError()); 
		return 0;
	}

	switch (GetLastError()) 
	{ 
		// The overlapped connection in progress. 
	case ERROR_IO_PENDING: 
		connected = TRUE; 
		break; 

		// Client is already connected, so signal an event. 
	case ERROR_PIPE_CONNECTED: 
		if (SetEvent(overlappedIO->hEvent)) 
			break; 

		// If an error occurs during the connect operation... 
	default: 
		{
			printf("ConnectNamedPipe failed with %d.\n", GetLastError());
			return 0;
		}
	} 

	return connected; 
}
void IpcPipe::disconnect()
{
	if (! DisconnectNamedPipe(m_pipeHandle) ) 
	{
		printf("DisconnectNamedPipe failed with %d.\n", GetLastError());
		return;
	}
	m_pipeState=PIPE_STATE_CONNECTING;
	m_options.callBackObj->OnDisconnect(this);

	LockObj lock(m_writeQueueLock);
	while(m_writeQueue.size())
	{
		PipeWriteElem *elem=m_writeQueue.front();
		m_writeQueue.pop();
		elem->ReleaseObj();
	}
}

void IpcPipe::reconnect()
{
	m_pendingIO = connectToNewClient( 
		m_pipeHandle, 
		&m_overlap); 

	m_pipeState = m_pendingIO ? PIPE_STATE_CONNECTING : // still connecting 
								PIPE_STATE_READING;     // ready to read 
}


void IpcPipe::DisconnectAndReconnect(IpcPipe *pipeInst) 
{ 
	pipeInst->disconnect();
	pipeInst->reconnect();
}

void IpcPipe::Write(char *data,unsigned int dataByteSize)
{
	EP_ASSERT(dataByteSize<=m_options.numOfWriteBytes);

	PipeWriteElem *elem=EP_NEW PipeWriteElem(dataByteSize,m_lockPolicy);
	System::Memcpy(elem->m_data,data, dataByteSize );
	
	BOOL fWrite = FALSE; 

	LockObj lock(m_writeQueueLock);
	if(m_writeQueue.size())
	{
		m_writeQueue.push(elem);
	}
	else
	{
		m_writeQueue.push(elem);
		fWrite = WriteFileEx( 
			m_pipeHandle, 
			elem->m_data, 
			elem->m_dataSize, 
			(LPOVERLAPPED) this, 
			(LPOVERLAPPED_COMPLETION_ROUTINE) OnWriteComplete); 

		if (IsConnectionAlive() && ! fWrite) 
			DisconnectAndReconnect(this); 
	}

	//System::Memcpy(m_writeBuffer,data, dataByteSize );
	
}

// CompletedWriteRoutine(DWORD, DWORD, LPOVERLAPPED) 
// This routine is called as a completion routine after writing to 
// the pipe, or when a new client has connected to a pipe instance.
// It starts another read operation. 

void IpcPipe::OnWriteComplete(DWORD dwErr, DWORD cbWritten, LPOVERLAPPED lpOverLap) 
{ 
	IpcPipe *pipeInst; 

	// lpOverlap points to storage for this instance. 

	pipeInst = (IpcPipe *) lpOverLap; 
	
	bool fWrite = FALSE; 
	LockObj lock(pipeInst->m_writeQueueLock);	
	if(pipeInst->m_writeQueue.size())
	{
		PipeWriteElem *elem=pipeInst->m_writeQueue.front();
		pipeInst->m_writeQueue.pop();

		// The write operation has finished, so read the next request (if 
		// there is no error). 

		if ((dwErr == 0) && (cbWritten == elem->m_dataSize)) 
		{
			pipeInst->m_options.callBackObj->OnWriteComplete(pipeInst,cbWritten,WRITE_STATUS_SUCCESS,dwErr); 
			if(pipeInst->IsConnectionAlive())
			{
				if(pipeInst->m_writeQueue.size())
				{
					PipeWriteElem *nextElem=pipeInst->m_writeQueue.front();
					fWrite = WriteFileEx( 
						pipeInst->m_pipeHandle, 
						nextElem->m_data, 
						nextElem->m_dataSize, 
						(LPOVERLAPPED) pipeInst, 
						(LPOVERLAPPED_COMPLETION_ROUTINE) OnWriteComplete); 
				}
			}
		}
		else
		{
			pipeInst->m_options.callBackObj->OnWriteComplete(pipeInst,cbWritten,WRITE_STATUS_FAIL_WRITE_FAILED,dwErr); 
		}
		elem->ReleaseObj();

		if (pipeInst->IsConnectionAlive() && ! fWrite) 
			DisconnectAndReconnect(pipeInst); 
	}

// 	bool fRead = FALSE; 
// 
// 	// The write operation has finished, so read the next request (if 
// 	// there is no error). 
// 
// 	if ((dwErr == 0) && (cbWritten == m_bytesToWrite)) 
// 	{
// 		pipeInst->m_options.callBackObj->OnWriteComplete(pipeInst); 
// 		if(pipeInst->IsConnectionAlive())
// 		{
// 			fRead = ReadFileEx( 
// 				pipeInst->m_pipeHandle, 
// 				pipeInst->m_readBuffer, 
// 				pipeInst->m_options.numOfReadBytes, 
// 				(LPOVERLAPPED) pipeInst, 
// 				(LPOVERLAPPED_COMPLETION_ROUTINE) OnReadComplete); 
// 
// 		}
// 	}
// 
// 
// 	if (pipeInst->IsConnectionAlive() && ! fRead) 
// 		DisconnectAndReconnect(pipeInst); 

	
} 

// CompletedReadRoutine(DWORD, DWORD, LPOVERLAPPED) 
// This routine is called as an I/O completion routine after reading 
// a request from the client. It gets data and writes it to the pipe. 

void IpcPipe::OnReadComplete(DWORD dwErr, DWORD cbBytesRead, LPOVERLAPPED lpOverLap) 
{ 
	IpcPipe * pipeInst; 
	
	// lpOverlap points to storage for this instance. 

	pipeInst = (IpcPipe *) lpOverLap; 

	// The read operation has finished, so write a response (if no 
	// error occurred). 
	BOOL fRead = FALSE;
	if ((dwErr == 0) && (cbBytesRead != 0)) 
	{ 
		pipeInst->m_options.callBackObj->OnReadComplete(pipeInst,pipeInst->m_readBuffer,cbBytesRead,READ_STATUS_SUCCESS,dwErr); 
		if(pipeInst->IsConnectionAlive())
		{
			fRead = ReadFileEx( 
				pipeInst->m_pipeHandle, 
				pipeInst->m_readBuffer, 
				pipeInst->m_options.numOfReadBytes, 
				(LPOVERLAPPED) pipeInst, 
				(LPOVERLAPPED_COMPLETION_ROUTINE) OnReadComplete); 

		}
	} 
	else
	{
		pipeInst->m_options.callBackObj->OnReadComplete(pipeInst,pipeInst->m_readBuffer,cbBytesRead,READ_STATUS_FAIL_READ_FAILED,dwErr); 
	}

	if (pipeInst->IsConnectionAlive() && ! fRead) 
		DisconnectAndReconnect(pipeInst); 

// 	BOOL fWrite = FALSE; 
// 	if ((dwErr == 0) && (cbBytesRead != 0)) 
// 	{ 
// 		pipeInst->m_options.callBackObj->OnReadComplete(pipeInst,pipeInst->m_readBuffer,cbBytesRead); 
// 
// 		if(pipeInst->IsConnectionAlive())
// 		{
// 			fWrite = WriteFileEx( 
// 				pipeInst->m_pipeHandle, 
// 				nextElem->m_data, 
// 				nextElem->m_dataSize, 
// 				(LPOVERLAPPED) pipeInst, 
// 				(LPOVERLAPPED_COMPLETION_ROUTINE) OnWriteComplete); 
// 
// 		}
// 
// 	} 
// 	if (pipeInst->IsConnectionAlive() && ! fWrite) 
// 		DisconnectAndReconnect(pipeInst); 

} 

