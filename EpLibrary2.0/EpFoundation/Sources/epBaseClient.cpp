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

BaseClient::BaseClient(const TCHAR * hostName, const TCHAR * port,BaseServerCallbackObject *callbackObj,LockPolicy lockPolicyType) :BaseServerSendObject(callbackObj,lockPolicyType)
{
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_sendLock=EP_NEW CriticalSectionEx();
		m_generalLock=EP_NEW CriticalSectionEx();
		m_disconnectLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_sendLock=EP_NEW Mutex();
		m_generalLock=EP_NEW Mutex();
		m_disconnectLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_sendLock=EP_NEW NoLock();
		m_generalLock=EP_NEW NoLock();
		m_disconnectLock=EP_NEW NoLock();
		break;
	default:
		m_sendLock=NULL;
		m_generalLock=NULL;
		m_disconnectLock=NULL;
		break;
	}
	m_recvSizePacket=Packet(NULL,4);
	SetHostName(hostName);
	SetPort(port);
	m_connectSocket=INVALID_SOCKET;
	m_result=0;
	m_ptr=0;
}

BaseClient::BaseClient(const BaseClient& b) :BaseServerSendObject(b)
{
	m_connectSocket=INVALID_SOCKET;
	m_result=0;
	m_ptr=0;
	m_hostName=b.m_hostName;
	m_port=b.m_port;
	m_recvSizePacket=Packet(NULL,4);
	m_lockPolicy=b.m_lockPolicy;
	switch(m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_sendLock=EP_NEW CriticalSectionEx();
		m_generalLock=EP_NEW CriticalSectionEx();
		m_disconnectLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_sendLock=EP_NEW Mutex();
		m_generalLock=EP_NEW Mutex();
		m_disconnectLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_sendLock=EP_NEW NoLock();
		m_generalLock=EP_NEW NoLock();
		m_disconnectLock=EP_NEW NoLock();
		break;
	default:
		m_sendLock=NULL;
		m_generalLock=NULL;
		m_disconnectLock=NULL;
		break;
	}
}
BaseClient::~BaseClient()
{
	Disconnect();

	if(m_sendLock)
		EP_DELETE m_sendLock;
	if(m_generalLock)
		EP_DELETE m_generalLock;
	if(m_disconnectLock)
		EP_DELETE m_disconnectLock;
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
	LockObj lock(m_generalLock);
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
	LockObj lock(m_generalLock);
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
	if(!IsConnected())
		return 0;
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
			return writeLength;
		}
		length-=sentLength;
		packetData+=sentLength;
	}
	return writeLength;
}

vector<BaseServerObject*> BaseClient::GetPacketParserList() const
{
	return m_parserList.GetList();
}

void BaseClient::execute() 
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
				BasePacketParser::PacketPassUnit passUnit;
				passUnit.m_packet=recvPacket;
				passUnit.m_this=this;
				BasePacketParser *parser =createNewPacketParser();
				parser->Start(reinterpret_cast<void*>(&passUnit));
				m_parserList.Push(parser);
				parser->ReleaseObj();
				recvPacket->ReleaseObj();
			}
			else if (iResult == 0)
			{
				System::OutputDebugString(_T("%s::%s(%d)(%x) Connection closing...\r\n"),__TFILE__,__TFUNCTION__,__LINE__,this);
				recvPacket->ReleaseObj();
				break;
			}
			else  {
				System::OutputDebugString(_T("%s::%s(%d)(%x) recv failed with error\r\n"),__TFILE__,__TFUNCTION__,__LINE__,this);
				recvPacket->ReleaseObj();
				break;
			}

			m_parserList.RemoveTerminated();
		}
		else
		{
			break;
		}

	} while (iResult > 0);
	disconnect(true);
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
	if(IsConnected())
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
		System::OutputDebugString(_T("%s::%s(%d)(%x) WSAStartup failed with error\r\n"),__TFILE__,__TFUNCTION__,__LINE__,this);
		return false;
	}

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(m_hostName.c_str(), m_port.c_str(), &hints, &m_result);
	if ( iResult != 0 ) {
		System::OutputDebugString(_T("%s::%s(%d)(%x) getaddrinfo failed with error\r\n"),__TFILE__,__TFUNCTION__,__LINE__,this);
		WSACleanup();
		return false;
	}

	// Attempt to connect to an address until one succeeds
	for(m_ptr=m_result; m_ptr != NULL ;m_ptr=m_ptr->ai_next) {

		// Create a SOCKET for connecting to server
		m_connectSocket = socket(m_ptr->ai_family, m_ptr->ai_socktype, 
			m_ptr->ai_protocol);
		if (m_connectSocket == INVALID_SOCKET) {
			System::OutputDebugString(_T("%s::%s(%d)(%x) Socket failed with error\r\n"),__TFILE__,__TFUNCTION__,__LINE__,this);
			cleanUpClient();
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
		System::OutputDebugString(_T("%s::%s(%d)(%x) Unable to connect to server!\r\n"),__TFILE__,__TFUNCTION__,__LINE__,this);
		cleanUpClient();
		return false;
	}
	if(Start())
	{
		return true;
	}
	cleanUpClient();
	return false;
}


bool BaseClient::IsConnected() const
{
	return (m_status==Thread::THREAD_STATUS_STARTED);
}

void BaseClient::cleanUpClient()
{

	if(m_connectSocket!=INVALID_SOCKET)
	{
		closesocket(m_connectSocket);
		m_connectSocket = INVALID_SOCKET;
	}
	if(m_result)
	{
		freeaddrinfo(m_result);
		m_result=NULL;
	}
	WSACleanup();

}

void BaseClient::disconnect(bool fromInternal)
{
	if(!m_disconnectLock->TryLock())
	{
		return;
	}

	if(IsConnected())
	{
		if(m_connectSocket!=INVALID_SOCKET)
		{
			// shutdown the connection since no more data will be sent
			int iResult = shutdown(m_connectSocket, SD_SEND);
			if (iResult == SOCKET_ERROR) {
				System::OutputDebugString(_T("%s::%s(%d)(%x) shutdown failed with error: %d\r\n"),__TFILE__,__TFUNCTION__,__LINE__,this, WSAGetLastError());
			}
			closesocket(m_connectSocket);
			m_connectSocket = INVALID_SOCKET;

		}
		if(!fromInternal)
			TerminateAfter(WAITTIME_INIFINITE);
		m_parserList.Clear();
	}

	cleanUpClient();
	
	m_disconnectLock->Unlock();
}

void BaseClient::Disconnect()
{
	LockObj lock(m_generalLock);
	if(!IsConnected())
	{
		return;
	}
	disconnect(false);
}



