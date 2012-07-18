/*! 
BaseServerWorkerUDP for the EpLibrary
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
#include "epBaseServerWorkerUDP.h"
#include "epSimpleLogger.h"
#include "epBaseServerUDP.h"
using namespace epl;
BaseServerWorkerUDP::BaseServerWorkerUDP(LockPolicy lockPolicyType): Thread(lockPolicyType), SmartObject(lockPolicyType)
{
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_lock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_lock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_lock=EP_NEW NoLock();
		break;
	default:
		m_lock=NULL;
		break;
	}
	m_packet=NULL;
	m_server=NULL;
	m_maxPacketSize=0;
}
BaseServerWorkerUDP::BaseServerWorkerUDP(const BaseServerWorkerUDP& b) : Thread(b),SmartObject(b)
{
	m_packet=NULL;
	m_server=NULL;
	m_maxPacketSize=0;
	m_lockPolicy=b.m_lockPolicy;
	switch(m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_lock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_lock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_lock=EP_NEW NoLock();
		break;
	default:
		m_lock=NULL;
		break;
	}
}

BaseServerWorkerUDP::~BaseServerWorkerUDP()
{
	m_lock->Lock();
	if(m_packet)
	{
		m_packet->ReleaseObj();	
		m_packet=NULL;
	}

	if(m_server)
		m_server->removeWorker(this);
	m_lock->Unlock();
	if(m_lock)
		EP_DELETE m_lock;
}

void BaseServerWorkerUDP::SetArg(void* a)
{
	LockObj lock(m_lock);
	PacketPassUnit *clientSocket=reinterpret_cast<PacketPassUnit*>(a);
	m_clientSocket=clientSocket->m_clientSocket;
	m_server=clientSocket->m_server;
	m_packet=clientSocket->m_packet;
	m_maxPacketSize=m_server->m_maxPacketSize;
}

int BaseServerWorkerUDP::Send(const Packet &packet)
{
	LockObj lock(m_lock);
	EP_ASSERT(packet.GetPacketByteSize()<=m_maxPacketSize);
	if(m_server)
		return m_server->send(packet,m_clientSocket);
	return 0;
}
void BaseServerWorkerUDP::setServer(BaseServerUDP *server)
{
	LockObj lock(m_lock);
	m_server=server;
}

void BaseServerWorkerUDP::execute()
{
	parsePacket(*m_packet);
	LockObj lock(m_lock);
	m_packet->ReleaseObj();
	m_packet=NULL;
}

