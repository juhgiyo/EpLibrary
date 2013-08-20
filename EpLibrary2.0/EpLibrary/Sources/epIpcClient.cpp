/*! 
IpcClient for the EpLibrary

The MIT License (MIT)

Copyright (c) 2012-2013 Woong Gyu La <juhgiyo@gmail.com>

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
#include "epIpcClient.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;


IpcClient::IpcClient(epl::LockPolicy lockPolicyType)
{
	m_lockPolicy=lockPolicyType;
	m_readBuffer=NULL;

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
}


IpcClient::~IpcClient()
{
	Disconnect();
	if(m_readBuffer)
		EP_DELETE m_readBuffer;
	if(m_writeQueueLock)
		EP_DELETE m_writeQueueLock;
}

epl::EpTString IpcClient::GetFullPipeName() const
{
	return m_pipeName;
}

void IpcClient::SetCallbackObject(IpcClientCallbackInterface *callBackObj)
{
	m_options.callBackObj=callBackObj;
}
IpcClientCallbackInterface *IpcClient::GetCallbackObject()
{
	return m_options.callBackObj;
}

ConnectStatus IpcClient::Connect(const IpcClientOps &ops, unsigned int waitTimeInMilliSec)
{
	EP_ASSERT(ops.callBackObj);
	if(ops.pipeName)
	{
		m_pipeName=_T("\\\\");
		m_pipeName.append(ops.domain);
		m_pipeName.append(_T("\\pipe\\"));
		m_pipeName.append(ops.pipeName);
	}
	m_options=ops;
	if(ops.numOfWriteBytes==0)
		m_options.numOfWriteBytes=DEFAULT_WRITE_BUF_SIZE;
	if(ops.numOfReadBytes==0)
		m_options.numOfWriteBytes=DEFAULT_READ_BUF_SIZE;

	m_readBuffer=EP_NEW char[m_options.numOfReadBytes]; 
	while(1)
	{
		m_pipeHandle= CreateFile( 
			m_pipeName.c_str(),   // pipe name 
			GENERIC_READ |  // read and write access 
			GENERIC_WRITE, 
			0,              // no sharing 
			NULL,           // default security attributes
			OPEN_EXISTING,  // opens existing pipe 
			FILE_FLAG_OVERLAPPED,              // default attributes 
			NULL);          // no template file 

		// Break if the pipe handle is valid. 

		if (m_pipeHandle != INVALID_HANDLE_VALUE) 
			break; 

		// Exit if an error other than ERROR_PIPE_BUSY occurs. 

		if (GetLastError() != ERROR_PIPE_BUSY) 
		{
			return CONNECT_STATUS_FAIL_PIPE_OPEN_FAILED;
		}

 		if(!WaitNamedPipe(m_pipeName.c_str(), waitTimeInMilliSec)) 
 		{ 
 			return CONNECT_STATUS_FAIL_TIME_OUT;
 		} 
	}


	unsigned long dwMode = PIPE_READMODE_MESSAGE; 
	bool fSuccess = SetNamedPipeHandleState( 
		m_pipeHandle,    // pipe handle 
		&dwMode,  // new pipe mode 
		NULL,     // don't set maximum bytes 
		NULL);    // don't set maximum time 
	if (! fSuccess) 
	{
		return CONNECT_STATUS_FAIL_SET_NAMED_PIPE_HANDLE_STATE;
	}
	
	m_connected= ReadFileEx( 
		m_pipeHandle, 
		m_readBuffer, 
		m_options.numOfReadBytes, 
		(LPOVERLAPPED) this, 
		(LPOVERLAPPED_COMPLETION_ROUTINE) OnReadComplete); 
	if(m_connected)
		return CONNECT_STATUS_SUCCESS;
	else
		return CONNECT_STATUS_FAIL_READ_FAILED;

}
void IpcClient::Disconnect()
{
	if(m_connected)
	{
		CloseHandle(m_pipeHandle); 
		m_connected=false;
		m_options.callBackObj->OnDisconnect(this);
		LockObj lock(m_writeQueueLock);
		while(m_writeQueue.size())
		{
			PipeWriteElem *elem=m_writeQueue.front();
			m_writeQueue.pop();
			elem->ReleaseObj();
		}
	}

}
void IpcClient::Write(char *data,unsigned int dataByteSize)
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

		if (IsConnected() && ! fWrite) 
			Disconnect(); 
	}

	//System::Memcpy(m_writeBuffer,data, dataByteSize );

}
bool IpcClient::IsConnected() const
{
	return m_connected;
}

unsigned int IpcClient::GetMaxReadDataByteSize() const
{
	return m_options.numOfReadBytes;
}
unsigned int IpcClient::GetMaxWriteDataByteSize() const
{
	return m_options.numOfWriteBytes;
}


// CompletedWriteRoutine(DWORD, DWORD, LPOVERLAPPED) 
// This routine is called as a completion routine after writing to 
// the pipe, or when a new client has connected to a pipe instance.
// It starts another read operation. 

void IpcClient::OnWriteComplete(DWORD dwErr, DWORD cbWritten, LPOVERLAPPED lpOverLap) 
{ 
	IpcClient *pipeInst; 

	// lpOverlap points to storage for this instance. 

	pipeInst = (IpcClient *) lpOverLap; 

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
			if(pipeInst->IsConnected())
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

		if (pipeInst->IsConnected() && ! fWrite) 
			pipeInst->Disconnect(); 
	}

	// 	bool fRead = FALSE; 
	// 
	// 	// The write operation has finished, so read the next request (if 
	// 	// there is no error). 
	// 
	// 	if ((dwErr == 0) && (cbWritten == m_bytesToWrite)) 
	// 	{
	// 		pipeInst->m_options.callBackObj->OnWriteComplete(pipeInst); 
	// 		if(pipeInst->IsConnected())
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
	// 	if (pipeInst->IsConnected() && ! fRead) 
	// 		pipeInst->Disconnect(); 


} 

// CompletedReadRoutine(DWORD, DWORD, LPOVERLAPPED) 
// This routine is called as an I/O completion routine after reading 
// a request from the client. It gets data and writes it to the pipe. 

void IpcClient::OnReadComplete(DWORD dwErr, DWORD cbBytesRead, LPOVERLAPPED lpOverLap) 
{ 
	IpcClient * pipeInst; 

	// lpOverlap points to storage for this instance. 

	pipeInst = (IpcClient *) lpOverLap; 

	// The read operation has finished, so write a response (if no 
	// error occurred). 
	BOOL fRead = FALSE;
	if ((dwErr == 0) && (cbBytesRead != 0)) 
	{ 
		pipeInst->m_options.callBackObj->OnReadComplete(pipeInst,pipeInst->m_readBuffer,cbBytesRead,READ_STATUS_SUCCESS,dwErr); 
		if(pipeInst->IsConnected())
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

	if (pipeInst->IsConnected() && ! fRead) 
		pipeInst->Disconnect(); 

	// 	BOOL fWrite = FALSE; 
	// 	if ((dwErr == 0) && (cbBytesRead != 0)) 
	// 	{ 
	// 		pipeInst->m_options.callBackObj->OnReadComplete(pipeInst,pipeInst->m_readBuffer,cbBytesRead); 
	// 
	// 		if(pipeInst->IsConnected())
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
	// 	if (pipeInst->IsConnected() && ! fWrite) 
	// 		pipeInst->Disconnect();
} 