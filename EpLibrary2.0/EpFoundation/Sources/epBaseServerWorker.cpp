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
BaseServerWorker::BaseServerWorker(unsigned int parserWaitTimeMilliSec,LockPolicy lockPolicyType): Thread(lockPolicyType), SmartObject(lockPolicyType)
{
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_sendLock=EP_NEW CriticalSectionEx();
		m_listLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_sendLock=EP_NEW Mutex();
		m_listLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_sendLock=EP_NEW NoLock();
		m_listLock=EP_NEW NoLock();
		break;
	default:
		m_listLock=NULL;
		m_sendLock=NULL;
		break;
	}
	m_parserWaitTime=parserWaitTimeMilliSec;
	m_recvSizePacket=Packet(NULL,4);
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
	m_parserWaitTime=b.m_parserWaitTime;
	m_recvSizePacket=Packet(NULL,4);
}

BaseServerWorker::~BaseServerWorker()
{
	m_sendLock->Lock();
	int iResult;
	iResult = shutdown(m_clientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		LOG_THIS_MSG(_T("shutdown failed with error\n"));
	}
	closesocket(m_clientSocket);
	m_sendLock->Unlock();
	WaitFor(WAITTIME_INIFINITE);

	if(m_parserWaitTime!=WAITTIME_SKIP)
	{
		LockObj lock(m_listLock);
		vector<HANDLE>::iterator iter;
		for(iter=m_parserList.begin();iter!=m_parserList.end();iter++)
		{
			if(System::WaitForSingleObject(*iter,m_parserWaitTime)==WAIT_TIMEOUT)
				System::TerminateThread(*iter,0);
		}
	}
	if(m_listLock)
		EP_DELETE m_listLock;

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
void BaseServerWorker::SetWaitTimeForParserTerminate(unsigned int milliSec)
{
	m_parserWaitTime=milliSec;
}

unsigned int BaseServerWorker::GetWaitTimeForParserTerminate()
{
	return m_parserWaitTime;
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

unsigned long BaseServerWorker::passPacket(void *param)
{
	Packet *recvPacket=( reinterpret_cast<PacketPassUnit*>(param))->m_packet;
	BaseServerWorker *worker=( reinterpret_cast<PacketPassUnit*>(param))->m_this;
	EP_DELETE reinterpret_cast<PacketPassUnit*>(param);
	worker->parsePacket(*recvPacket);
	recvPacket->ReleaseObj();

	LockObj(worker->m_listLock);
	vector<HANDLE>::iterator iter;
	for(iter=worker->m_parserList.begin();iter!=worker->m_parserList.end();iter++)
	{
		if(*iter == GetCurrentThread())
		{
			worker->m_parserList.erase(iter);
			break;
		}
	}
	return 0;
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
				ThreadID threadID;
				PacketPassUnit *passUnit=EP_NEW PacketPassUnit() ;
				passUnit->m_packet=recvPacket;
				passUnit->m_this=this;
				HANDLE parserThreadHandle=::CreateThread(NULL,0,passPacket,passUnit,THREAD_OPCODE_CREATE_START,(LPDWORD)&threadID);

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


