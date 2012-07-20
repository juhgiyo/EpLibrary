/*! 
BaseServerWorker for the EpLibrary
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
#include "epBaseServerWorker.h"


using namespace epl;
BaseServerWorker::BaseServerWorker(LockPolicy lockPolicyType): BaseServerSendObject(lockPolicyType)
{
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_sendLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_sendLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_sendLock=EP_NEW NoLock();
		break;
	default:
		m_sendLock=NULL;
		break;
	}
	m_recvSizePacket=Packet(NULL,4);
}
BaseServerWorker::BaseServerWorker(const BaseServerWorker& b) : BaseServerSendObject(b)
{
	m_lockPolicy=b.m_lockPolicy;
	switch(m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_sendLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_sendLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_sendLock=EP_NEW NoLock();
		break;
	default:
		m_sendLock=NULL;
		break;
	}
	m_recvSizePacket=Packet(NULL,4);
}

BaseServerWorker::~BaseServerWorker()
{
	m_sendLock->Lock();
	int iResult;
	iResult = shutdown(m_clientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		System::OutputDebugString(_T("%s::%s(%d)(%x) shutdown failed with error\n"),__TFILE__,__TFUNCTION__,__LINE__,this);
	}
	closesocket(m_clientSocket);
	m_sendLock->Unlock();
	WaitFor(WAITTIME_INIFINITE);

	m_parserList.Clear();

	if(m_sendLock)
		EP_DELETE m_sendLock;
}

void BaseServerWorker::setArg(void* a)
{
	SOCKET clientSocket=reinterpret_cast<SOCKET>(a);
	m_clientSocket=clientSocket;
}

int BaseServerWorker::Send(const Packet &packet)
{
	
	LockObj lock(m_sendLock);
	int writeLength=0;
	const char *packetData=packet.GetPacket();
	int length=packet.GetPacketByteSize();
	if(length>0)
	{
		int sentLength=send(m_clientSocket,reinterpret_cast<char*>(&length),4,0);
		if(sentLength<=0)
			return sentLength;
	}
	while(length>0)
	{
		int sentLength=send(m_clientSocket,packetData,length,0);
		writeLength+=sentLength;
		if(writeLength<=0)
			break;
		length-=sentLength;
		packetData+=sentLength;
	}
	return writeLength;
}
vector<BaseServerObject*> BaseServerWorker::GetPacketParserList() const
{
	return m_parserList.GetList();
}


int BaseServerWorker::receive(Packet &packet)
{
	int readLength=0;
	int length=packet.GetPacketByteSize();
	char *packetData=const_cast<char*>(packet.GetPacket());
	while(length>0)
	{
		int recvLength=recv(m_clientSocket,packetData, length, 0);
		readLength+=recvLength;
		if(recvLength<=0)
			break;
		length-=recvLength;
		packetData+=recvLength;
	}
	return readLength;
}


void BaseServerWorker::execute()
{
	int iResult=0;

	// Receive until the peer shuts down the connection
	do {
		iResult =receive(m_recvSizePacket);
		if(iResult>0)
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
				System::OutputDebugString(_T("%s::%s(%d)(%x) Connection closing...\n"),__TFILE__,__TFUNCTION__,__LINE__,this);
				recvPacket->ReleaseObj();
				break;
			}
			else  {
				System::OutputDebugString(_T("%s::%s(%d)(%x) recv failed with error\n"),__TFILE__,__TFUNCTION__,__LINE__,this);
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
}


