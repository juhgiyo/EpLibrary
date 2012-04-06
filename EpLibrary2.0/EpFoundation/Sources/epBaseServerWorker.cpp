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
#include "epSimpleLogger.h"

using namespace epl;
BaseServerWorker::BaseServerWorker(LockPolicy lockPolicyType): Thread(lockPolicyType), SmartObject(lockPolicyType)
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
}
BaseServerWorker::BaseServerWorker(const BaseServerWorker& b) : Thread(b),SmartObject(b)
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
}

BaseServerWorker::~BaseServerWorker()
{
	m_sendLock->Lock();
	int iResult;
	iResult = shutdown(m_clientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		LOG_THIS_MSG(_T("shutdown failed with error\n"));
		closesocket(m_clientSocket);
	}
	m_sendLock->Unlock();
	if(m_sendLock)
		EP_DELETE m_sendLock;
}

void BaseServerWorker::SetArg(void* a)
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
			return false;
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
	int iResult;
	Packet recvSizePacket(NULL,4);
	
	// Receive until the peer shuts down the connection
	do {
		int size =receive(recvSizePacket);
		if(size>0)
		{
			unsigned int shouldReceive=(reinterpret_cast<unsigned int*>(const_cast<char*>(recvSizePacket.GetPacket())))[0];
			Packet recvPacket(NULL,shouldReceive);
			iResult = receive(recvPacket);

			if (iResult == shouldReceive) {
				// Process Received Packet and Send Result to Client
				if(parsePacket(recvPacket)<=0)
				{
					break;
				}
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
		}
		else
		{
			break;
		}

	} while (iResult > 0);
}
