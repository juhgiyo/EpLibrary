/*! 
BaseServerUDP for the EpLibrary
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
#include "epBaseServerUDP.h"
#include "epSimpleLogger.h"

using namespace epl;

BaseServerUDP::BaseServerUDP(const TCHAR *  port, LockPolicy lockPolicyType)
{
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_lock=EP_NEW CriticalSectionEx();
		m_sendLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_lock=EP_NEW Mutex();
		m_sendLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_lock=EP_NEW NoLock();
		m_sendLock=EP_NEW NoLock();
		break;
	default:
		m_lock=NULL;
		m_sendLock=NULL;
		break;
	}
	SetPort(port);
	m_serverThreadHandle=0;
	m_listenSocket=NULL;
	m_result=0;
	m_isServerStarted=false;
	m_maxPacketSize=0;
}

BaseServerUDP::BaseServerUDP(const BaseServerUDP& b)
{
	m_serverThreadHandle=0;
	m_listenSocket=NULL;
	m_result=0;
	m_isServerStarted=false;
	m_port=b.m_port;
	m_lockPolicy=b.m_lockPolicy;
	m_maxPacketSize=b.m_maxPacketSize;
	switch(m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_lock=EP_NEW CriticalSectionEx();
		m_sendLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_lock=EP_NEW Mutex();
		m_sendLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_lock=EP_NEW NoLock();
		m_sendLock=EP_NEW NoLock();
		break;
	default:
		m_lock=NULL;
		m_sendLock=NULL;
		break;
	}
}
BaseServerUDP::~BaseServerUDP()
{
	StopServer();
	if(m_lock)
		EP_DELETE m_lock;
	if(m_sendLock)
		EP_DELETE m_sendLock;
}

bool BaseServerUDP::SetPort(const TCHAR *  port)
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

EpTString BaseServerUDP::GetPort() const
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

unsigned int BaseServerUDP::GetMaxPacketByteSize() const
{
	return m_maxPacketSize;
}


int BaseServerUDP::send(const Packet &packet,const sockaddr &clientSockAddr)
{

	LockObj lock(m_sendLock);

	int sentLength=0;
	const char *packetData=packet.GetPacket();
	int length=packet.GetPacketByteSize();
	EP_ASSERT(length<=m_maxPacketSize);
	if(length>0)
	{
		int sockAddrSize=sizeof(sockaddr);
		sentLength=sendto(m_listenSocket,packetData,length,0,&clientSockAddr,sizeof(sockaddr));
	}
	return sentLength;
}

vector<BaseServerWorkerUDP*> BaseServerUDP::GetClientList() const
{
	return m_clientList;
}

unsigned long BaseServerUDP::ServerThread( LPVOID lpParam ) 
{
	BaseServerUDP *pMainClass=reinterpret_cast<BaseServerUDP*>(lpParam);

	Packet recvPacket(NULL,pMainClass->m_maxPacketSize);
	char *packetData=const_cast<char*>(recvPacket.GetPacket());
	int length=recvPacket.GetPacketByteSize();
	sockaddr clientSockAddr;
	int sockAddrSize=sizeof(sockaddr);
	while(pMainClass->m_listenSocket!=INVALID_SOCKET)
	{
		int recvLength=recvfrom(pMainClass->m_listenSocket,packetData,length, 0,&clientSockAddr,&sockAddrSize);
		if(recvLength<=0)
			continue;
		LockObj lock(pMainClass->m_lock);
		/// Create Worker Thread
		BaseServerWorkerUDP *accWorker=pMainClass->createNewWorker();
		pMainClass->GetClientList().push_back(accWorker);
		BaseServerWorkerUDP::PacketPassUnit unit;
		unit.m_clientSocket=clientSockAddr;
		unit.m_packet=EP_NEW Packet(packetData,recvLength);
		unit.m_server=pMainClass;
		accWorker->Start(reinterpret_cast<void*>(&unit));

	}
	pMainClass->m_serverThreadHandle=0;
	return 0; 
} 


bool BaseServerUDP::StartServer()
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
	m_maxPacketSize=0;

	m_result = NULL;


	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {

		EP_NOTICEBOX(_T("WSAStartup failed with error\n"));
		return false;
	}

	ZeroMemory(&m_hints, sizeof(m_hints));
	m_hints.ai_family = AF_INET;
	m_hints.ai_socktype = SOCK_DGRAM;
	m_hints.ai_protocol = IPPROTO_UDP;
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

	int nTmp = sizeof(int);
	getsockopt(m_listenSocket, SOL_SOCKET,SO_MAX_MSG_SIZE, (char *)&m_maxPacketSize,&nTmp);

	m_isServerStarted=true;

	// Create thread 1.
	m_serverThreadHandle = CreateThread( NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(BaseServerUDP::ServerThread), this, 0, NULL);  
	if(m_serverThreadHandle)
		return true;
	return false;


}

void BaseServerUDP::ShutdownAllClient()
{
	LockObj lock(m_lock);
	shutdownAllClient();
}

void BaseServerUDP::shutdownAllClient()
{
	if(!m_isServerStarted)
		return;
	// shutdown the connection since we're done
	vector<BaseServerWorkerUDP*>::iterator iter;
	for(iter=m_clientList.begin();iter!=m_clientList.end();iter++)
	{
		if(*iter)
		{
			(*iter)->setServer(NULL);
			(*iter)->ReleaseObj();
		}
	}
	m_clientList.clear();
}
void BaseServerUDP::removeWorker(BaseServerWorkerUDP *worker)
{
	LockObj lock(m_lock);
	vector<BaseServerWorkerUDP*>::iterator iter;
	for(iter=m_clientList.begin();iter!=m_clientList.end();iter++)
	{
		if(*iter==worker)
			m_clientList.erase(iter);
	}
}
bool BaseServerUDP::IsServerStarted() const
{
	return m_isServerStarted;
}

void BaseServerUDP::StopServer()
{
	LockObj lock(m_lock);
	stopServer();
}

void BaseServerUDP::stopServer()
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
	m_maxPacketSize=0;
	m_result=NULL;
	m_serverThreadHandle=0;
	WSACleanup();
}

