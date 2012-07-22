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


using namespace epl;

BaseServerUDP::BaseServerUDP(const TCHAR *  port,BaseCallbackObject *callbackObj, LockPolicy lockPolicyType): BaseServerObject(callbackObj,lockPolicyType)
{
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_lock=EP_NEW CriticalSectionEx();
		m_sendLock=EP_NEW CriticalSectionEx();
		m_disconnectLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_lock=EP_NEW Mutex();
		m_sendLock=EP_NEW Mutex();
		m_disconnectLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_lock=EP_NEW NoLock();
		m_sendLock=EP_NEW NoLock();
		m_disconnectLock=EP_NEW NoLock();
		break;
	default:
		m_lock=NULL;
		m_sendLock=NULL;
		m_disconnectLock=NULL;
		break;
	}
	SetPort(port);
	m_listenSocket=INVALID_SOCKET;
	m_result=0;
	m_maxPacketSize=0;
}

BaseServerUDP::BaseServerUDP(const BaseServerUDP& b):BaseServerObject(b)
{
	m_listenSocket=INVALID_SOCKET;
	m_result=0;
	m_port=b.m_port;
	m_lockPolicy=b.m_lockPolicy;
	m_maxPacketSize=b.m_maxPacketSize;
	switch(m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_lock=EP_NEW CriticalSectionEx();
		m_sendLock=EP_NEW CriticalSectionEx();
		m_disconnectLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_lock=EP_NEW Mutex();
		m_sendLock=EP_NEW Mutex();
		m_disconnectLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_lock=EP_NEW NoLock();
		m_sendLock=EP_NEW NoLock();
		m_disconnectLock=EP_NEW NoLock();
		break;
	default:
		m_lock=NULL;
		m_sendLock=NULL;
		m_disconnectLock=NULL;
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
	if(m_disconnectLock)
		EP_DELETE m_disconnectLock;
}

void  BaseServerUDP::SetPort(const TCHAR *  port)
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

EpTString BaseServerUDP::GetPort() const
{
	LockObj lock(m_lock);
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

vector<BaseServerObject*> BaseServerUDP::GetWorkerList() const
{
	return m_workerList.GetList();
}

void BaseServerUDP::execute()
{
	Packet recvPacket(NULL,m_maxPacketSize);
	char *packetData=const_cast<char*>(recvPacket.GetPacket());
	int length=recvPacket.GetPacketByteSize();
	sockaddr clientSockAddr;
	int sockAddrSize=sizeof(sockaddr);
	while(m_listenSocket!=INVALID_SOCKET)
	{
		int recvLength=recvfrom(m_listenSocket,packetData,length, 0,&clientSockAddr,&sockAddrSize);
		if(recvLength<=0)
			continue;
		/// Create Worker Thread
		Packet *passPacket=EP_NEW Packet(packetData,recvLength);
		BaseServerWorkerUDP *accWorker=createNewWorker();
		BaseServerWorkerUDP::PacketPassUnit unit;
		unit.m_clientSocket=clientSockAddr;
		unit.m_packet=passPacket;
		unit.m_server=this;
		accWorker->Start(reinterpret_cast<void*>(&unit));
		m_workerList.Push(accWorker);
		accWorker->ReleaseObj();
		passPacket->ReleaseObj();

		m_workerList.RemoveTerminated();

	}
	stopServer(true);
} 


bool BaseServerUDP::StartServer()
{
	LockObj lock(m_lock);
	if(IsServerStarted())
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

		System::OutputDebugString(_T("%s::%s(%d)(%x) WSAStartup failed with error\r\n"),__TFILE__,__TFUNCTION__,__LINE__,this);
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
		System::OutputDebugString(_T("%s::%s(%d)(%x) getaddrinfo failed with error\r\n"),__TFILE__,__TFUNCTION__,__LINE__,this);
		WSACleanup();
		return false;
	}

	// Create a SOCKET for connecting to server
	m_listenSocket = socket(m_result->ai_family, m_result->ai_socktype, m_result->ai_protocol);
	if (m_listenSocket == INVALID_SOCKET) {
		System::OutputDebugString(_T("%s::%s(%d)(%x) socket failed with error\r\n"),__TFILE__,__TFUNCTION__,__LINE__,this);
		cleanUpServer();
		return false;
	}

	// set SO_REUSEADDR for setsockopt function to reuse the port immediately as soon as the service exits.
	int sockoptval = 1;
	setsockopt(m_listenSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&sockoptval), sizeof(int));

	// Setup the TCP listening socket
	iResult = bind( m_listenSocket, m_result->ai_addr, static_cast<int>(m_result->ai_addrlen));
	if (iResult == SOCKET_ERROR) {
		System::OutputDebugString(_T("%s::%s(%d)(%x) bind failed with error\r\n"),__TFILE__,__TFUNCTION__,__LINE__,this);
		cleanUpServer();
		return false;
	}

	int nTmp = sizeof(int);
	getsockopt(m_listenSocket, SOL_SOCKET,SO_MAX_MSG_SIZE, (char *)&m_maxPacketSize,&nTmp);


	// Create thread 1.
	if(Start())
	{
		return true;
	}
	cleanUpServer();
	return false;


}

void BaseServerUDP::Broadcast(const Packet& packet)
{
	m_workerList.RemoveTerminated();
	vector<BaseServerObject*>::iterator iter;
	vector<BaseServerObject*> clientList=m_workerList.GetList();
	for(iter=clientList.begin();iter!=clientList.end();iter++)
	{
		((BaseServerWorkerUDP*)(*iter))->Send(packet);
	}
}


void BaseServerUDP::ShutdownAllClient()
{
	m_workerList.Clear();
}


bool BaseServerUDP::IsServerStarted() const
{
	return (GetStatus()==Thread::THREAD_STATUS_STARTED);
}

void BaseServerUDP::StopServer()
{
	LockObj lock(m_lock);
	if(!IsServerStarted())
	{
		return;
	}
	stopServer(false);
}


void BaseServerUDP::cleanUpServer()
{
	if(m_listenSocket!=INVALID_SOCKET)
	{
		closesocket(m_listenSocket);
		m_listenSocket=INVALID_SOCKET;
	}
	if(m_result)
	{
		freeaddrinfo(m_result);
		m_result=NULL;
	}
	m_maxPacketSize=0;
	WSACleanup();

}

void BaseServerUDP::stopServer(bool fromInternal)
{
	if(!m_disconnectLock->TryLock())
	{
		return;
	}
	if(IsServerStarted())
	{
		// No longer need server socket
		if(m_listenSocket!=INVALID_SOCKET)
		{
			int iResult;
			iResult = shutdown(m_listenSocket, SD_SEND);
			if (iResult == SOCKET_ERROR) {
				System::OutputDebugString(_T("%s::%s(%d)(%x) shutdown failed with error\r\n"),__TFILE__,__TFUNCTION__,__LINE__,this);
			}
			closesocket(m_listenSocket);
			m_listenSocket=INVALID_SOCKET;
		}
		if(!fromInternal)
			TerminateAfter(WAITTIME_INIFINITE);

		ShutdownAllClient();
	}
	
	cleanUpServer();
	
	m_disconnectLock->Unlock();
}

