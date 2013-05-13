/*! 
IpcServer for the EpLibrary
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
#include "epIpcServer.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;

IpcServer::IpcServer(epl::LockPolicy lockPolicyType):Thread(EP_THREAD_PRIORITY_NORMAL,lockPolicyType)
{
	m_lockPolicy=lockPolicyType;
	switch(m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_pipesLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_pipesLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_pipesLock=EP_NEW NoLock();
		break;
	default:
		m_pipesLock=NULL;
		break;
	}
	m_serverThreadEvent=EventEx(false,false);
}

IpcServer::~IpcServer()
{
	StopServer();
	if(m_pipesLock)
		EP_DELETE m_pipesLock;
}

epl::EpTString IpcServer::GetFullPipeName() const
{
	return m_pipeName;
}
unsigned int IpcServer::GetMaximumInstances() const
{
	return m_options.maximumInstances;
}
void IpcServer::SetCallbackObject(IpcServerCallbackInterface *callBackObj)
{
	m_options.callBackObj=callBackObj;
}
IpcServerCallbackInterface *IpcServer::GetCallbackObject()
{
	return m_options.callBackObj;
}

bool IpcServer::StartServer(const IpcServerOps &ops)
{
	EP_ASSERT(ops.maximumInstances<=PIPE_UNLIMITED_INSTANCES);
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

	

	m_pipesLock->Lock();
	for(unsigned int trav=0;trav<m_options.maximumInstances;trav++)
	{
		IpcPipe *pipeInst=EP_NEW IpcPipe(m_pipeName,m_options,m_lockPolicy);
		if(pipeInst->Create())
		{
			
			m_events.push_back(pipeInst->GetEventHandle());
			m_pipes.push_back(pipeInst);	
		}
		else
		{
			EP_DELETE pipeInst;
			m_pipesLock->Unlock();
			return false;
		}
	}
	m_pipesLock->Unlock();

	m_started=Start();
	return m_started;


}
void IpcServer::execute()
{
	while (1) 
	{ 
		if(m_serverThreadEvent.WaitForEvent(0))
		{
			break;
		}
		unsigned long waitResult;
		unsigned long retBytes;
		bool success;
		unsigned int index;
		// Wait for a client to connect, or for a read or write 
		// operation to be completed, which causes a completion 
		// routine to be queued for execution. 

		waitResult = WaitForMultipleObjects( 
			m_options.maximumInstances,    // number of event objects 
			&m_events.at(0),      // array of event objects 
			FALSE,        // does not wait for all 
			INFINITE);    // waits indefinitely 



		switch (waitResult) 
		{ 
		case WAIT_IO_COMPLETION:
			break;
		case WAIT_FAILED:
			printf("WaitForMultipleObjectEx (%d)\n", GetLastError()); 
			stopServer();
			return;
			break;
			// The wait conditions are satisfied by a completed connect 
			// operation. 
		default: 
			// If an operation is pending, get the result of the 
			// connect operation. 
			index = waitResult - WAIT_OBJECT_0;  // determines which pipe 
			if (index < 0 || index > (m_options.maximumInstances - 1)) 
			{
				printf("Index out of range.\n"); 
				stopServer();
				return;
			}

			m_pipesLock->Lock();
			if (m_pipes.at(index)->m_pendingIO) 
			{ 
				success = GetOverlappedResult( 
					m_pipes.at(index)->m_pipeHandle,     // pipe handle 
					&m_pipes.at(index)->m_overlap, // OVERLAPPED structure 
					&retBytes,    // bytes transferred 
					FALSE);    // does not wait 
				if (!success) 
				{
					printf("ConnectNamedPipe (%d)\n", GetLastError()); 
					m_pipesLock->Unlock();
					stopServer();					
					return;
				}
			} 
			m_options.callBackObj->OnNewConnection(m_pipes.at(index));
			ReadFileEx( 
				m_pipes.at(index)->m_pipeHandle, 
				m_pipes.at(index)->m_readBuffer, 
				m_pipes.at(index)->m_options.numOfReadBytes, 
				(LPOVERLAPPED) m_pipes.at(index), 
				(LPOVERLAPPED_COMPLETION_ROUTINE) IpcPipe::OnReadComplete); 
			//Pipe::OnWriteComplete(0, 0, (LPOVERLAPPED) m_pipes.at(index)); 
			m_pipesLock->Unlock();
			break; 
		} 
	} 
	stopServer();
}
void IpcServer::StopServer()
{
	m_serverThreadEvent.SetEvent();
	WaitFor(m_options.waitTimeInMilliSec);
	m_serverThreadEvent.ResetEvent();
}

void IpcServer::stopServer()
{
	LockObj lock(m_pipesLock);
	for(int trav=0;trav<m_pipes.size();trav++)
	{
		m_pipes.at(trav)->KillConnection();
		m_pipes.at(trav)->ReleaseObj();
	}
	m_pipes.clear();
	m_events.clear();
	m_started=false;
}

bool IpcServer::IsServerStarted() const
{
	return m_started;
}
void IpcServer::ShutdownAllClient()
{
	LockObj lock(m_pipesLock);
	for(int trav=0;trav<m_pipes.size();trav++)
	{
		IpcPipe::DisconnectAndReconnect(m_pipes.at(trav));
	}
	
}

unsigned int IpcServer::GetMaxReadDataByteSize() const
{
	return m_options.numOfReadBytes;
}
unsigned int IpcServer::GetMaxWriteDataByteSize() const
{
	return m_options.numOfWriteBytes;
}