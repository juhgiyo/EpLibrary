/*! 
BasePacketParser for the EpLibrary
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
#include "epBasePacketParser.h"
#include "epThread.h"
using namespace epl;

BasePacketParser::BasePacketParser(unsigned int waitTimeMilliSec,LockPolicy lockPolicyType):SmartObject(lockPolicyType),Thread(lockPolicyType)
{
	m_waitTime=waitTimeMilliSec;
	m_owner=NULL;
	m_packetReceived=NULL;
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_generalLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_generalLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_generalLock=EP_NEW NoLock();
		break;
	default:
		m_generalLock=NULL;
		break;
	}
}

BasePacketParser::BasePacketParser(const BasePacketParser& b):SmartObject(b),Thread(b)
{
	m_waitTime=b.m_waitTime;
	m_owner=b.m_owner;
	m_packetReceived=b.m_packetReceived;
	if(m_packetReceived)
		m_packetReceived->RetainObj();
	m_lockPolicy=b.m_lockPolicy;
	switch(m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_generalLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_generalLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_generalLock=EP_NEW NoLock();
		break;
	default:
		m_generalLock=NULL;
		break;
	}
}
BasePacketParser::~BasePacketParser()
{
	WaitFor(m_waitTime);
	
	m_generalLock->Lock();
	if(m_packetReceived)
		m_packetReceived->ReleaseObj();
	m_generalLock->Unlock();

	if(m_generalLock)
		EP_DELETE m_generalLock;
}

int BasePacketParser::Send(const Packet &packet)
{
	LockObj lock(m_generalLock);
	if(m_owner)
	{
		return m_owner->Send(packet);
	}
	return 0;
}

void BasePacketParser::SetWaitTimeForParserTerminate(unsigned int milliSec)
{
	LockObj lock(m_generalLock);
	m_waitTime=milliSec;
}

unsigned int BasePacketParser::GetWaitTimeForParserTerminate()
{
	LockObj lock(m_generalLock);
	return m_waitTime;
}
void BasePacketParser::execute()
{
	if(m_packetReceived)
		ParsePacket(*m_packetReceived);
}

void BasePacketParser::SetArg(void* a)
{
	LockObj lock(m_generalLock);
	m_packetReceived=( reinterpret_cast<PacketPassUnit*>(a))->m_packet;
	if(m_packetReceived)
		m_packetReceived->RetainObj();
	m_owner=( reinterpret_cast<PacketPassUnit*>(a))->m_this;
	
}
