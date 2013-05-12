/*! 
IpcPipe for the EpLibrary
Copyright (C) 2012  Woong Gyu La <juhgiyo@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "epIpcPipe.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;


Pipe::Pipe(EpTString pipeName, IpcServerOps options,epl::LockPolicy lockPolicyType): SmartObject(lockPolicyType)
{
	m_pipeName=pipeName;
	m_options=options;

	m_pipeEvent=EventEx(true,true);

	m_overlap.hEvent=m_pipeEvent.GetEventHandle();

	m_readBuffer=EP_NEW char[options.numOfReadBytes]; 
	m_writeBuffer=EP_NEW char[options.numOfWriteBytes]; 

}
Pipe::~Pipe()
{
	if(m_readBuffer)
		EP_DELETE m_readBuffer;
	if(m_writeBuffer)
		EP_DELETE m_writeBuffer;
	KillConnection();
}
bool Pipe::Create()
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

void Pipe::Send(char *data,unsigned int dataByteSize)
{
	EP_ASSERT(dataByteSize<=m_options.numOfWriteBytes);
	System::Memcpy(m_writeBuffer,data, dataByteSize );
	m_bytesToWrite = dataByteSize;
}
bool Pipe::IsConnectionAlive() const
{
	return (m_pipeState!=PIPE_STATE_CONNECTING);
		
}
void Pipe::KillConnection()
{
	if(IsConnectionAlive())
		disconnect();
}

HANDLE Pipe::GetEventHandle()
{
	return m_pipeEvent.GetEventHandle();
}

bool Pipe::connectToNewClient(HANDLE pipeHandle,OVERLAPPED *overlappedIO )
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
void Pipe::disconnect()
{
	if (! DisconnectNamedPipe(m_pipeHandle) ) 
	{
		printf("DisconnectNamedPipe failed with %d.\n", GetLastError());
		return;
	}
	m_pipeState=PIPE_STATE_CONNECTING;
	m_options.callBackObj->OnDisconnect(this);
}

void Pipe::reconnect()
{
	m_pendingIO = connectToNewClient( 
		m_pipeHandle, 
		&m_overlap); 

	m_pipeState = m_pendingIO ? PIPE_STATE_CONNECTING : // still connecting 
								PIPE_STATE_READING;     // ready to read 
}


void Pipe::disconnectAndReconnect(Pipe *pipeInst) 
{ 
	pipeInst->disconnect();
	pipeInst->reconnect();
}



// CompletedWriteRoutine(DWORD, DWORD, LPOVERLAPPED) 
// This routine is called as a completion routine after writing to 
// the pipe, or when a new client has connected to a pipe instance.
// It starts another read operation. 

void Pipe::OnWriteComplete(DWORD dwErr, DWORD cbWritten, LPOVERLAPPED lpOverLap) 
{ 
	Pipe *pipeInst; 
	bool fRead = FALSE; 

	// lpOverlap points to storage for this instance. 

	pipeInst = (Pipe *) lpOverLap; 

	// The write operation has finished, so read the next request (if 
	// there is no error). 

	if ((dwErr == 0) && (cbWritten == pipeInst->m_bytesToWrite)) 
		fRead = ReadFileEx( 
		pipeInst->m_pipeHandle, 
		pipeInst->m_readBuffer, 
		pipeInst->m_options.numOfReadBytes, 
		(LPOVERLAPPED) pipeInst, 
		(LPOVERLAPPED_COMPLETION_ROUTINE) OnReadComplete); 

	// Disconnect if an error occurred. 

	if (! fRead) 
		disconnectAndReconnect(pipeInst); 
} 

// CompletedReadRoutine(DWORD, DWORD, LPOVERLAPPED) 
// This routine is called as an I/O completion routine after reading 
// a request from the client. It gets data and writes it to the pipe. 

void Pipe::OnReadComplete(DWORD dwErr, DWORD cbBytesRead, LPOVERLAPPED lpOverLap) 
{ 
	Pipe * pipeInst; 
	BOOL fWrite = FALSE; 

	// lpOverlap points to storage for this instance. 

	pipeInst = (Pipe *) lpOverLap; 

	// The read operation has finished, so write a response (if no 
	// error occurred). 

	if ((dwErr == 0) && (cbBytesRead != 0)) 
	{ 
		pipeInst->m_options.callBackObj->OnReadComplete(pipeInst,pipeInst->m_readBuffer,cbBytesRead); 

		fWrite = WriteFileEx( 
			pipeInst->m_pipeHandle, 
			pipeInst->m_writeBuffer, 
			pipeInst->m_bytesToWrite, 
			(LPOVERLAPPED) pipeInst, 
			(LPOVERLAPPED_COMPLETION_ROUTINE) OnWriteComplete); 
	} 

	// Disconnect if an error occurred. 

	if (! fWrite) 
		disconnectAndReconnect(pipeInst); 
} 

