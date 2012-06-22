/*! 
BaseServerEx for the EpLibrary
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
#include "epBaseServerEx.h"
#include "epSimpleLogger.h"

using namespace epl;

BaseServerEx::BaseServerEx(const TCHAR * port, LockPolicy lockPolicyType)
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
	m_serverThreadHandle=0;
	m_listenSocket=NULL;
	m_result=0;
	m_isServerStarted=false;

}

BaseServerEx::BaseServerEx(const BaseServerEx& b)
{
	m_serverThreadHandle=0;
	m_listenSocket=NULL;
	m_result=0;
	m_isServerStarted=false;
	m_port=b.m_port;
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
BaseServerEx::~BaseServerEx()
{
	StopServer();
	if(m_lock)
		EP_DELETE m_lock;
}

bool BaseServerEx::SetPort(const TCHAR * port)
{
	LockObj lock(m_lock);
	if(m_isServerStarted)
		return false;
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
	return true;
}

EpTString BaseServerEx::GetPort() const
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

vector<BaseServerWorkerEx*> BaseServerEx::GetClientList() const
{
	return m_clientList;
}
unsigned long BaseServerEx::ServerThread( LPVOID lpParam ) 
{
	BaseServerEx *pMainClass=reinterpret_cast<BaseServerEx*>(lpParam);

	SOCKET clientSocket;
	while(clientSocket=accept(pMainClass->m_listenSocket, NULL, NULL))
	{
		if(clientSocket == INVALID_SOCKET)
		{
			continue;			
		}
		else
		{
			BaseServerWorkerEx *accWorker=pMainClass->createNewWorker();
			pMainClass->GetClientList().push_back(accWorker);
			accWorker->Start(reinterpret_cast<void*>(clientSocket));
		}

		vector<BaseServerWorkerEx*>::iterator iter;
		for(iter=pMainClass->m_clientList.begin();iter!=pMainClass->m_clientList.end();)
		{
			if((*iter)->GetStatus()==Thread::THREAD_STATUS_TERMINATED)
			{
				(*iter)->Release();
				iter=pMainClass->m_clientList.erase(iter);
			}
			else
				iter++;

		}
	}
	pMainClass->m_serverThreadHandle=0;
	pMainClass->StopServer();

	return 0; 
} 


bool BaseServerEx::StartServer()
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

		EP_NOTICEBOX(_T("WSAStartup failed with error\n"));
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
		EP_NOTICEBOX(_T("getaddrinfo failed with error\n"));
		WSACleanup();
		return false;
	}

	// Create a SOCKET for connecting to server
	m_listenSocket = socket(m_result->ai_family, m_result->ai_socktype, m_result->ai_protocol);
	if (m_listenSocket == INVALID_SOCKET) {
		EP_NOTICEBOX(_T("socket failed with error\n"));
		stopServer();
		return false;
	}

	// set SO_REUSEADDR for setsockopt function to reuse the port immediately as soon as the service exits.
	int sockoptval = 1;
	setsockopt(m_listenSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&sockoptval), sizeof(int));

	// Setup the TCP listening socket
	iResult = bind( m_listenSocket, m_result->ai_addr, static_cast<int>(m_result->ai_addrlen));
	if (iResult == SOCKET_ERROR) {
		EP_NOTICEBOX(_T("bind failed with error\n"));
		stopServer();
		return false;
	}

	iResult = listen(m_listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		EP_NOTICEBOX(_T("listen failed with error\n"));
		stopServer();
		return false;
	}
	m_isServerStarted=true;

	// Create thread 1.
	m_serverThreadHandle = CreateThread( NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(BaseServerEx::ServerThread), this, 0, NULL);  
	if(m_serverThreadHandle)
		return true;
	return false;


}


void BaseServerEx::ShutdownAllClient()
{
	LockObj lock(m_lock);
	shutdownAllClient();
}

void BaseServerEx::shutdownAllClient()
{
	if(!m_isServerStarted)
		return;
	// shutdown the connection since we're done
	vector<BaseServerWorkerEx*>::iterator iter;
	for(iter=m_clientList.begin();iter!=m_clientList.end();iter++)
	{
		if(*iter)
			(*iter)->Release();
	}
	m_clientList.clear();
}

bool BaseServerEx::IsServerStarted() const
{
	return m_isServerStarted;
}
void BaseServerEx::StopServer()
{
	LockObj lock(m_lock);
	stopServer();
}

void BaseServerEx::stopServer()
{
	if(m_isServerStarted==true)
	{
		shutdownAllClient();	
		// No longer need server socket
	}
	if(m_serverThreadHandle)
		TerminateThread(m_serverThreadHandle,0);
	if(m_listenSocket)
		closesocket(m_listenSocket);
	m_isServerStarted=false;
	if(m_result)
		freeaddrinfo(m_result);
	m_result=NULL;
	m_serverThreadHandle=0;
	WSACleanup();
}


