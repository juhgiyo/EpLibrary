/*! 
BaseServer for the EpLibrary
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
#include "epBaseServer.h"
#include "epSimpleLogger.h"

using namespace epl;

BaseServer::BaseServer(const TCHAR *  port, unsigned int waitTimeMilliSec, LockPolicy lockPolicyType)
{
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_lock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_lock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_lock=EP_NEW NoLock();
		break;
	default:
		m_lock=NULL;
		break;
	}
	SetPort(port);
	m_waitTime=waitTimeMilliSec;
	m_serverThreadHandle=0;
	m_listenSocket=NULL;
	m_result=0;
	m_isServerStarted=false;
}

BaseServer::BaseServer(const BaseServer& b)
{
	m_serverThreadHandle=0;
	m_listenSocket=NULL;
	m_result=0;
	m_isServerStarted=false;
	m_port=b.m_port;
	m_waitTime=b.m_waitTime;
	m_lockPolicy=b.m_lockPolicy;
	switch(m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_lock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_lock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_lock=EP_NEW NoLock();
		break;
	default:
		m_lock=NULL;
		break;
	}
}
BaseServer::~BaseServer()
{
	StopServer();
	if(m_lock)
		EP_DELETE m_lock;
}

void  BaseServer::SetPort(const TCHAR *  port)
{
	LockObj lock(m_lock);

	unsigned int strLength=System::TcsLen(port);
	if(strLength==0)
		m_port=DEFAULT_PORT;
	else
	{
#if defined(_UNICODE) || defined(UNICODE)
		m_port=System::WideCharToMultiByte(port);
#else// defined(_UNICODE) || defined(UNICODE)
		m_port=port;
#endif// defined(_UNICODE) || defined(UNICODE)
	}
}

EpTString BaseServer::GetPort() const
{
	if(!m_port.length())
		return _T("");
#if defined(_UNICODE) || defined(UNICODE)
	EpTString retString=System::MultiByteToWideChar(m_port.c_str());
	return retString;
#else //defined(_UNICODE) || defined(UNICODE)
	return m_port;
#endif //defined(_UNICODE) || defined(UNICODE)
}

vector<BaseServerWorker*> BaseServer::GetClientList() const
{
	return m_clientList;
}
unsigned long BaseServer::ServerThread( LPVOID lpParam ) 
{
	BaseServer *pMainClass=reinterpret_cast<BaseServer*>(lpParam);

	SOCKET clientSocket;
	while(1)
	{
		clientSocket=accept(pMainClass->m_listenSocket, NULL, NULL);
		if(clientSocket == INVALID_SOCKET)
		{
			break;			
		}
		else
		{
			BaseServerWorker *accWorker=pMainClass->createNewWorker();
			pMainClass->m_clientList.push_back(accWorker);
			accWorker->Start(reinterpret_cast<void*>(clientSocket));
		}

		vector<BaseServerWorker*>::iterator iter;
		for(iter=pMainClass->m_clientList.begin();iter!=pMainClass->m_clientList.end();)
		{
			if((*iter)->GetStatus()==Thread::THREAD_STATUS_TERMINATED)
			{
				(*iter)->ReleaseObj();
				iter=pMainClass->m_clientList.erase(iter);
			}
			else
				iter++;

		}
	}
	pMainClass->m_serverThreadHandle=0;
	return 0; 
} 


bool BaseServer::StartServer()
{
	LockObj lock(m_lock);
	if(m_isServerStarted)
		return true;
	if(!m_port.length())
	{
		m_port=DEFAULT_PORT;
	}

	WSADATA wsaData;
	int iResult;

	m_listenSocket= INVALID_SOCKET;

	m_result = NULL;


	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {

		System::OutputDebugString(_T("%s::%s(%d) WSAStartup failed with error\n"),__TFILE__,__TFUNCTION__,__LINE__);
		return false;
	}

	ZeroMemory(&m_hints, sizeof(m_hints));
	m_hints.ai_family = AF_INET;
	m_hints.ai_socktype = SOCK_STREAM;
	m_hints.ai_protocol = IPPROTO_TCP;
	m_hints.ai_flags = AI_PASSIVE;


	// Resolve the server address and port
	iResult = getaddrinfo(NULL, m_port.c_str(), &m_hints, &m_result);
	if ( iResult != 0 ) {
		System::OutputDebugString(_T("%s::%s(%d) getaddrinfo failed with error\n"),__TFILE__,__TFUNCTION__,__LINE__);		
		WSACleanup();
		return false;
	}

	// Create a SOCKET for connecting to server
	m_listenSocket = socket(m_result->ai_family, m_result->ai_socktype, m_result->ai_protocol);
	if (m_listenSocket == INVALID_SOCKET) {
		System::OutputDebugString(_T("%s::%s(%d) socket failed with error\n"),__TFILE__,__TFUNCTION__,__LINE__);
		stopServer();
		return false;
	}

	// set SO_REUSEADDR for setsockopt function to reuse the port immediately as soon as the service exits.
	int sockoptval = 1;
	setsockopt(m_listenSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&sockoptval), sizeof(int));

	// Setup the TCP listening socket
	iResult = bind( m_listenSocket, m_result->ai_addr, static_cast<int>(m_result->ai_addrlen));
	if (iResult == SOCKET_ERROR) {
		System::OutputDebugString(_T("%s::%s(%d) bind failed with error\n"),__TFILE__,__TFUNCTION__,__LINE__);
		stopServer();
		return false;
	}

	iResult = listen(m_listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		System::OutputDebugString(_T("%s::%s(%d) listen failed with error\n"),__TFILE__,__TFUNCTION__,__LINE__);
		stopServer();
		return false;
	}
	m_isServerStarted=true;

	// Create thread 1.
	m_serverThreadHandle = CreateThread( NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(BaseServer::ServerThread), this, 0, NULL);  
	if(m_serverThreadHandle)
		return true;
	return false;


}

void BaseServer::ShutdownAllClient()
{
	LockObj lock(m_lock);
	shutdownAllClient();
}

void BaseServer::SetWaitTimeForSafeTerminate(unsigned int milliSec)
{
	m_waitTime=milliSec;
}

unsigned int BaseServer::GetWaitTimeForSafeTerminate()
{
	return m_waitTime;
}

void BaseServer::shutdownAllClient()
{
	if(!m_isServerStarted)
		return;
	// shutdown the connection since we're done
	vector<BaseServerWorker*>::iterator iter;
	for(iter=m_clientList.begin();iter!=m_clientList.end();iter++)
	{
		if(*iter)
			(*iter)->ReleaseObj();
	}
	m_clientList.clear();
}

bool BaseServer::IsServerStarted() const
{
	return m_isServerStarted;
}

void BaseServer::StopServer()
{
	LockObj lock(m_lock);
	stopServer();
}

void BaseServer::stopServer()
{
	if(m_result)
		freeaddrinfo(m_result);
	if(m_isServerStarted==true)
	{
		shutdownAllClient();	
		// No longer need server socket
		if(m_listenSocket)
			closesocket(m_listenSocket);
		if(m_serverThreadHandle)
		{
			if(System::WaitForSingleObject(m_serverThreadHandle, m_waitTime)==WAIT_TIMEOUT)
				System::TerminateThread(m_serverThreadHandle,0);
		}
	}
	m_serverThreadHandle=0;
	m_isServerStarted=false;
	m_result=NULL;
	WSACleanup();
}

