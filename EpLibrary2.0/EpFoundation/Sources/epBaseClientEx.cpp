/*! 
BaseClientEx for the EpLibrary
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
#include "epBaseClientEx.h"
#include "epThread.h"
#include "epSimpleLogger.h"
using namespace epl;

BaseClientEx::BaseClientEx(const TCHAR * hostName, const TCHAR * port, unsigned int waitTimeMilliSec,unsigned int parserWaitTimeMilliSec,LockPolicy lockPolicyType):BaseClient(hostName,port,waitTimeMilliSec,lockPolicyType)
{
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_listLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_listLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_listLock=EP_NEW NoLock();
		break;
	default:
		m_listLock=NULL;
		break;
	}
	m_parserWaitTime=parserWaitTimeMilliSec;
}

BaseClientEx::BaseClientEx(const BaseClientEx& b):BaseClient(b)
{
	switch(b.m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_listLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_listLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_listLock=EP_NEW NoLock();
		break;
	default:
		m_listLock=NULL;
		break;
	}

	m_parserWaitTime=b.m_parserWaitTime;
}
BaseClientEx::~BaseClientEx()
{
	m_listLock->Lock();
	vector<HANDLE>::iterator iter;
	for(iter=m_parserList.begin();iter!=m_parserList.end();iter++)
	{
		if(System::WaitForSingleObject(*iter,m_parserWaitTime)==WAIT_TIMEOUT)
			System::TerminateThread(*iter,0);
	}
	m_listLock->Unlock();

	if(m_listLock)
		EP_DELETE m_listLock;
}

void BaseClientEx::SetWaitTimeForParserTerminate(unsigned int milliSec)
{
	m_parserWaitTime=milliSec;
}
unsigned int BaseClientEx::GetWaitTimeForParserTerminate()
{
	return m_parserWaitTime;
}

unsigned long BaseClientEx::passPacket(void *param)
{
	Packet *recvPacket=( reinterpret_cast<PacketPassUnit*>(param))->m_packet;
	BaseClientEx *client=( reinterpret_cast<PacketPassUnit*>(param))->m_this;
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

void BaseClientEx::processClientThread() 
{
	int iResult;
	// Receive until the peer shuts down the connection
	do {
		int size =receive(m_recvSizePacket);
		if(size>0)
		{
			unsigned int shouldReceive=(reinterpret_cast<unsigned int*>(const_cast<char*>(m_recvSizePacket.GetPacket())))[0];
			// TODO: Possible Memory Leak from here
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

