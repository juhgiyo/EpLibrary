/*! 
BaseClientUDP for the EpLibrary
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
#include "epBaseClientUDP.h"
#include "epThread.h"
#include "epSimpleLogger.h"
using namespace epl;

BaseClientUDP::BaseClientUDP(const TCHAR * hostName, const TCHAR * port,LockPolicy lockPolicyType)
{
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_sendLock=EP_NEW CriticalSectionEx();
		m_generalLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_sendLock=EP_NEW Mutex();
		m_generalLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_sendLock=EP_NEW NoLock();
		m_generalLock=EP_NEW NoLock();
		break;
	default:
		m_sendLock=NULL;
		m_generalLock=NULL;
		break;
	}
	SetHostName(hostName);
	SetPort(port);
	m_connectSocket=NULL;
	m_result=0;
	m_ptr=0;
	m_isConnected=false;
	m_maxPacketSize=0;
}

BaseClientUDP::BaseClientUDP(const BaseClientUDP& b)
{
	m_connectSocket=NULL;
	m_result=0;
	m_ptr=0;
	m_isConnected=false;
	m_hostName=b.m_hostName;
	m_port=b.m_port;
	m_lockPolicy=b.m_lockPolicy;
	m_maxPacketSize=b.m_maxPacketSize;
	switch(m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_sendLock=EP_NEW CriticalSectionEx();
		m_generalLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_sendLock=EP_NEW Mutex();
		m_generalLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_sendLock=EP_NEW NoLock();
		m_generalLock=EP_NEW NoLock();
		break;
	default:
		m_sendLock=NULL;
		m_generalLock=NULL;
		break;
	}
}
BaseClientUDP::~BaseClientUDP()
{
	Disconnect();
	if(m_sendLock)
		EP_DELETE m_sendLock;
	if(m_generalLock)
		EP_DELETE m_generalLock;
}

bool BaseClientUDP::SetHostName(const TCHAR * hostName)
{
	LockObj lock(m_generalLock);
	if(m_isConnected)
		return false;

	unsigned int strLength=System::TcsLen(hostName);
	if(strLength==0)
		m_hostName=DEFAULT_HOSTNAME;
	else
	{		
#if defined(_UNICODE) || defined(UNICODE)
		m_hostName=System::WideCharToMultiByte(hostName);
#else// defined(_UNICODE) || defined(UNICODE)
		m_hostName=hostName;
#endif// defined(_UNICODE) || defined(UNICODE)
	}
	return true;
}

bool BaseClientUDP::SetPort(const TCHAR *port)
{
	LockObj lock(m_generalLock);
	if(m_isConnected)
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
EpTString BaseClientUDP::GetHostName() const
{
	if(!m_hostName.length())
		return _T("");

#if defined(_UNICODE) || defined(UNICODE)
	EpTString retString=System::MultiByteToWideChar(m_hostName.c_str());
	return retString;
#else //defined(_UNICODE) || defined(UNICODE)
	return m_hostName;
#endif //defined(_UNICODE) || defined(UNICODE)

}
EpTString BaseClientUDP::GetPort() const
{
	if(!m_port.length())
		return _T("");

#if defined(_UNICODE) || defined(UNICODE)
	EpTString retString=System::MultiByteToWideChar(m_port.c_str());;
	return retString;
#else //defined(_UNICODE) || defined(UNICODE)
	return m_port;
#endif //defined(_UNICODE) || defined(UNICODE)

}

unsigned int BaseClientUDP::GetMaxPacketByteSize() const
{
	return m_maxPacketSize;
}

int BaseClientUDP::Send(const Packet &packet)
{
	LockObj lock(m_sendLock);
	int sentLength=0;
	const char *packetData=packet.GetPacket();
	int length=packet.GetPacketByteSize();
	EP_ASSERT(length<=m_maxPacketSize);
	if(length>0)
	{
		//int sentLength=send(m_connectSocket,packetData,length,0);
		sentLength=sendto(m_connectSocket,packetData,length,0,m_ptr->ai_addr,sizeof(sockaddr));
		if(sentLength<=0)
		{
			disconnect();
			return sentLength;
		}
	}
	return sentLength;
}

int BaseClientUDP::receive(Packet &packet)
{

	int length=packet.GetPacketByteSize();
	char *packetData=const_cast<char*>(packet.GetPacket());
	sockaddr tmpInfo;
	int tmpInfoSize=sizeof(sockaddr);
	int recvLength = recvfrom(m_connectSocket,packetData,length,0,&tmpInfo,&tmpInfoSize);
	return recvLength;
}

bool BaseClientUDP::Connect()
{
	LockObj lock(m_generalLock);
	if(m_isConnected)
		return true;
	if(!m_port.length())
	{
		m_port=DEFAULT_PORT;
	}

	if(!m_hostName.length())
	{
		m_hostName=DEFAULT_HOSTNAME;
	}


	WSADATA wsaData;
	m_connectSocket = INVALID_SOCKET;
	m_maxPacketSize=0;
	struct addrinfo hints;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		EP_NOTICEBOX(_T("WSAStartup failed with error\n"));
		return false;
	}

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	// Resolve the server address and port
	iResult = getaddrinfo(m_hostName.c_str(), m_port.c_str(), &hints, &m_result);
	if ( iResult != 0 ) {
		EP_NOTICEBOX(_T("getaddrinfo failed with error\n"));
		WSACleanup();
		return false;
	}

	// Attempt to connect to an address until one succeeds
	for(m_ptr=m_result; m_ptr != NULL ;m_ptr=m_ptr->ai_next) {

		// Create a SOCKET for connecting to server
		m_connectSocket = socket(m_ptr->ai_family, m_ptr->ai_socktype, 
			m_ptr->ai_protocol);
		if (m_connectSocket == INVALID_SOCKET) {
			EP_NOTICEBOX(_T("socket failed with error\n"));
			disconnect();
			return false;
		}
		break;
	}
	if (m_connectSocket == INVALID_SOCKET) {
		EP_NOTICEBOX(_T("Unable to connect to server!\n"));
		disconnect();
		return false;
	}

	int nTmp = sizeof(int);
	getsockopt(m_connectSocket, SOL_SOCKET,SO_MAX_MSG_SIZE, (char *)&m_maxPacketSize,&nTmp);

	m_isConnected=true;

	m_clientThreadHandle = CreateThread( NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(BaseClientUDP::ClientThread), this, 0, NULL);  
	if(m_clientThreadHandle)
		return true;
	return true;
}


unsigned long BaseClientUDP::ClientThread( LPVOID lpParam ) 
{
	BaseClientUDP *pMainClass=reinterpret_cast<BaseClientUDP*>(lpParam);
	pMainClass->processClientThread();
	pMainClass->m_clientThreadHandle=0;
	return 0; 
}

bool BaseClientUDP::IsConnected() const
{
	return m_isConnected;
}

void BaseClientUDP::disconnect()
{
	if(m_result)
		freeaddrinfo(m_result);

	if(m_isConnected)
	{
		if(m_clientThreadHandle)
			TerminateThread(m_clientThreadHandle,0);
		int iResult = shutdown(m_connectSocket, SD_SEND);
		if (iResult == SOCKET_ERROR)
			LOG_THIS_MSG(_T("shutdown failed with error: %d\n"), WSAGetLastError());
		closesocket(m_connectSocket);
	}
	m_isConnected=false;
	m_connectSocket = INVALID_SOCKET;
	m_maxPacketSize=0;
	m_result=NULL;
	WSACleanup();
}

void BaseClientUDP::Disconnect()
{
	LockObj lock(m_generalLock);
	// No longer need server socket
	disconnect();
}

unsigned long BaseClientUDP::passPacket(void *param)
{
	Packet *recvPacket=( reinterpret_cast<PacketPassUnit*>(param))->m_packet;
	BaseClientUDP *worker=( reinterpret_cast<PacketPassUnit*>(param))->m_this;
	EP_DELETE reinterpret_cast<PacketPassUnit*>(param);
	worker->parsePacket(*recvPacket);
	recvPacket->ReleaseObj();
	return 0;
}

void BaseClientUDP::processClientThread() 
{
	int iResult=0;
	// Receive until the peer shuts down the connection
	Packet recvPacket(NULL,m_maxPacketSize);
	do {
		iResult = receive(recvPacket);
		
		if (iResult > 0) {
			LockObj lock(m_generalLock);
			Thread::ThreadID threadID;
			PacketPassUnit *passUnit=EP_NEW PacketPassUnit();
			passUnit->m_packet=EP_NEW Packet(recvPacket.GetPacket(),iResult);
			passUnit->m_this=this;
			::CreateThread(NULL,0,passPacket,passUnit,Thread::THREAD_OPCODE_CREATE_START,(LPDWORD)&threadID);
		}
		else if (iResult == 0)
		{
			LOG_THIS_MSG(_T("Connection closing...\n"));
			break;
		}
		else  {
			LOG_THIS_MSG(_T("recv failed with error\n"));
			break;
		}

	} while (iResult > 0);

}