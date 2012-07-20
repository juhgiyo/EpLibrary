/*! 
BaseClient for the EpLibrary
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
#include "epBaseClient.h"
#include "epThread.h"
using namespace epl;

BaseClient::BaseClient(const TCHAR * hostName, const TCHAR * port, unsigned int parserWaitTimeMilliSec,LockPolicy lockPolicyType)
{
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_sendLock=EP_NEW CriticalSectionEx();
		m_generalLock=EP_NEW CriticalSectionEx();
		m_listLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_sendLock=EP_NEW Mutex();
		m_generalLock=EP_NEW Mutex();
		m_listLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_sendLock=EP_NEW NoLock();
		m_generalLock=EP_NEW NoLock();
		m_listLock=EP_NEW NoLock();
		break;
	default:
		m_sendLock=NULL;
		m_generalLock=NULL;
		m_listLock=NULL;
		break;
	}
	m_recvSizePacket=Packet(NULL,4);
	SetHostName(hostName);
	SetPort(port);
	m_parserWaitTime=parserWaitTimeMilliSec;
	m_connectSocket=NULL;
	m_result=0;
	m_ptr=0;
	m_isConnected=false;
}

BaseClient::BaseClient(const BaseClient& b)
{
	m_connectSocket=NULL;
	m_result=0;
	m_ptr=0;
	m_isConnected=false;
	m_hostName=b.m_hostName;
	m_port=b.m_port;
	m_parserWaitTime=b.m_parserWaitTime;
	m_recvSizePacket=Packet(NULL,4);
	m_lockPolicy=b.m_lockPolicy;
	switch(m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_sendLock=EP_NEW CriticalSectionEx();
		m_generalLock=EP_NEW CriticalSectionEx();
		m_listLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_sendLock=EP_NEW Mutex();
		m_generalLock=EP_NEW Mutex();
		m_listLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_sendLock=EP_NEW NoLock();
		m_generalLock=EP_NEW NoLock();
		m_listLock=EP_NEW NoLock();
		break;
	default:
		m_sendLock=NULL;
		m_generalLock=NULL;
		m_listLock=NULL;
		break;
	}
}
BaseClient::~BaseClient()
{
	Disconnect();

	if(m_listLock)
		EP_DELETE m_listLock;

	if(m_sendLock)
		EP_DELETE m_sendLock;
	if(m_generalLock)
		EP_DELETE m_generalLock;
}

void  BaseClient::SetHostName(const TCHAR * hostName)
{
	LockObj lock(m_generalLock);

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
}

void  BaseClient::SetPort(const TCHAR *port)
{
	LockObj lock(m_generalLock);

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
EpTString BaseClient::GetHostName() const
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
EpTString BaseClient::GetPort() const
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

int BaseClient::Send(const Packet &packet)
{
	LockObj lock(m_sendLock);
	int writeLength=0;
	const char *packetData=packet.GetPacket();
	int length=packet.GetPacketByteSize();
	if(length>0)
	{
		int sentLength=send(m_connectSocket,reinterpret_cast<char*>(&length),4,0);
		if(sentLength<=0)
			return sentLength;
	}
	while(length>0)
	{
		int sentLength=send(m_connectSocket,packetData,length,0);
		writeLength+=sentLength;
		if(sentLength<=0)
		{
			disconnect();
			return writeLength;
		}
		length-=sentLength;
		packetData+=sentLength;
	}
	return writeLength;
}

void BaseClient::SetWaitTimeForParserTerminate(unsigned int milliSec)
{
	m_parserWaitTime=milliSec;
}

unsigned int BaseClient::GetWaitTimeForParserTerminate()
{
	return m_parserWaitTime;
}

unsigned long BaseClient::passPacket(void *param)
{
	Packet *recvPacket=( reinterpret_cast<PacketPassUnit*>(param))->m_packet;
	BaseClient *client=( reinterpret_cast<PacketPassUnit*>(param))->m_this;
	EP_DELETE reinterpret_cast<PacketPassUnit*>(param);
	client->parsePacket(*recvPacket);
	recvPacket->ReleaseObj();

	LockObj(client->m_listLock);
	vector<HANDLE>::iterator iter;
	for(iter=client->m_parserList.begin();iter!=client->m_parserList.end();iter++)
	{
		if(*iter == GetCurrentThread())
		{
			client->m_parserList.erase(iter);
			break;
		}
	}

	return 0;
}


void BaseClient::processClientThread() 
{
	int iResult;
	// Receive until the peer shuts down the connection
	do {
		int size =receive(m_recvSizePacket);
		if(size>0)
		{
			unsigned int shouldReceive=(reinterpret_cast<unsigned int*>(const_cast<char*>(m_recvSizePacket.GetPacket())))[0];
			Packet *recvPacket=EP_NEW Packet(NULL,shouldReceive);
			iResult = receive(*recvPacket);

			if (iResult == shouldReceive) {
				Thread::ThreadID threadID;
				PacketPassUnit *passUnit=EP_NEW PacketPassUnit();
				passUnit->m_packet=recvPacket;
				passUnit->m_this=this;
				HANDLE parserThreadHandle=::CreateThread(NULL,0,passPacket,passUnit,Thread::THREAD_OPCODE_CREATE_START,(LPDWORD)&threadID);

				LockObj lock(m_listLock);
				m_parserList.push_back(parserThreadHandle);
			}
			else if (iResult == 0)
			{
				LOG_THIS_MSG(_T("Connection closing...\n"));
				recvPacket->ReleaseObj();
				break;
			}
			else  {
				LOG_THIS_MSG(_T("recv failed with error\n"));
				recvPacket->ReleaseObj();
				break;
			}
			// To here
		}
		else
		{
			break;
		}

	} while (iResult > 0);
}


int BaseClient::receive(Packet &packet)
{
	
	int readLength=0;
	int length=packet.GetPacketByteSize();
	char *packetData=const_cast<char*>(packet.GetPacket());
	while(length>0)
	{
		int recvLength=recv(m_connectSocket,packetData, length, 0);
		readLength+=recvLength;
		if(recvLength<=0)
			break;
		length-=recvLength;
		packetData+=recvLength;
	}
	return readLength;
}

bool BaseClient::Connect()
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
	struct addrinfo hints;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		System::OutputDebugString(_T("%s::%s(%d) WSAStartup failed with error\n"),__TFILE__,__TFUNCTION__,__LINE__);
		return false;
	}

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(m_hostName.c_str(), m_port.c_str(), &hints, &m_result);
	if ( iResult != 0 ) {
		System::OutputDebugString(_T("%s::%s(%d) getaddrinfo failed with error\n"),__TFILE__,__TFUNCTION__,__LINE__);
		WSACleanup();
		return false;
	}

	// Attempt to connect to an address until one succeeds
	for(m_ptr=m_result; m_ptr != NULL ;m_ptr=m_ptr->ai_next) {

		// Create a SOCKET for connecting to server
		m_connectSocket = socket(m_ptr->ai_family, m_ptr->ai_socktype, 
			m_ptr->ai_protocol);
		if (m_connectSocket == INVALID_SOCKET) {
			System::OutputDebugString(_T("%s::%s(%d) Socket failed with error\n"),__TFILE__,__TFUNCTION__,__LINE__);
			disconnect();
			return false;
		}

		// Connect to server.
		iResult = connect( m_connectSocket, m_ptr->ai_addr, static_cast<int>(m_ptr->ai_addrlen));
		if (iResult == SOCKET_ERROR) {
			closesocket(m_connectSocket);
			m_connectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	if (m_connectSocket == INVALID_SOCKET) {
		System::OutputDebugString(_T("%s::%s(%d) Unable to connect to server!\n"),__TFILE__,__TFUNCTION__,__LINE__);
		disconnect();
		return false;
	}
	m_isConnected=true;

	m_clientThreadHandle = CreateThread( NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(BaseClient::ClientThread), this, 0, NULL);  
	if(m_clientThreadHandle)
		return true;
	return true;
}


unsigned long BaseClient::ClientThread( LPVOID lpParam ) 
{
	BaseClient *pMainClass=reinterpret_cast<BaseClient*>(lpParam);
	pMainClass->processClientThread();
	pMainClass->m_clientThreadHandle=0;
	return 0; 
}

bool BaseClient::IsConnected() const
{
	return m_isConnected;
}

void BaseClient::disconnect()
{
	if(m_result)
		freeaddrinfo(m_result);

	if(m_isConnected)
	{
		// shutdown the connection since no more data will be sent
		int iResult = shutdown(m_connectSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			LOG_THIS_MSG(_T("shutdown failed with error: %d\n"), WSAGetLastError());
		}
		closesocket(m_connectSocket);
		if(m_clientThreadHandle)
		{
			if(System::WaitForSingleObject(m_clientThreadHandle, WAITTIME_INIFINITE)==WAIT_TIMEOUT)
				System::TerminateThread(m_clientThreadHandle,0);
		}

		m_listLock->Lock();
		vector<HANDLE>::iterator iter;
		for(iter=m_parserList.begin();iter!=m_parserList.end();iter++)
		{
			if(m_parserWaitTime!=WAITTIME_SKIP && System::WaitForSingleObject(*iter,m_parserWaitTime)==WAIT_TIMEOUT)
				System::TerminateThread(*iter,0);
		}
		m_listLock->Unlock();
	}
	m_clientThreadHandle=0;
	m_isConnected=false;
	m_connectSocket = INVALID_SOCKET;
	m_result=NULL;
	WSACleanup();
}

void BaseClient::Disconnect()
{
	LockObj lock(m_generalLock);
	// No longer need server socket
	disconnect();
}


