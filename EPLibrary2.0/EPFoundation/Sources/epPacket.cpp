/*! 
Packet for the EpLibrary
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
#include "stdafx.h"
#include "epPacket.h"

using namespace epl;

Packet::Packet(char *packet, unsigned int byteSize, bool shouldAllocate):SmartObject()
{
	m_packet=NULL;
	m_packetSize=0;
	m_isAllocated=shouldAllocate;
	if(shouldAllocate)
	{
		if(byteSize>0)
		{
			m_packet=EP_NEW char[byteSize];
			if(packet)
				memcpy(m_packet,packet,byteSize);
			else
				memset(m_packet,0,byteSize);
			m_packetSize=byteSize;
		}
	}
	else
	{
		m_packet=packet;
		m_packetSize=byteSize;
	}
}

Packet::Packet(const Packet& b):SmartObject()
{
	m_packet=NULL;
	if(b.m_isAllocated)
	{
		if(b.m_packetSize>0)
		{
			m_packet=EP_NEW char[b.m_packetSize];
			memcpy(m_packet,b.m_packet,b.m_packetSize);
		}
		m_packetSize=b.m_packetSize;
	}
	else
	{
		m_packet=b.m_packet;
		m_packetSize=b.m_packetSize;
	}
	m_isAllocated=b.m_isAllocated;
}
Packet & Packet::operator=(const Packet&b)
{
	if(this!=&b)
	{
		if(m_isAllocated)
		{
			EP_DELETE[] m_packet;
		}
		m_packet=NULL;

		if(b.m_isAllocated)
		{
			if(b.m_packetSize>0)
			{
				m_packet=EP_NEW char[b.m_packetSize];
				EP_WASSERT(0,_T("Allocation Failed."));
				memcpy(m_packet,b.m_packet,b.m_packetSize);
			}
			m_packetSize=b.m_packetSize;
		}
		else
		{
			m_packet=b.m_packet;
			m_packetSize=b.m_packetSize;
		}
		m_isAllocated=b.m_isAllocated;
	}
	return *this;
}

Packet::~Packet()
{
	if(m_isAllocated && m_packet)
	{
		EP_DELETE[] m_packet;
	}
	m_packet=NULL;
}

unsigned int Packet::GetPacketByteSize() const
{
	return m_packetSize;
}

const char *Packet::GetPacket() const
{
	return m_packet;	
}

void Packet::SetPacket(char* packet, unsigned int packetByteSize)
{
	if(m_isAllocated)
	{
		if(m_packet)
			EP_DELETE[] m_packet;
		m_packet=NULL;
		if(packetByteSize>0)
		{
			m_packet=EP_NEW char[packetByteSize];
			EP_WASSERT(0,_T("Allocation Failed."));
		}
		if(packet)
			memcpy(m_packet,packet,packetByteSize);
		else
			memset(m_packet,0,packetByteSize);
		m_packetSize=packetByteSize;

	}
	else
	{
		m_packet=packet;
		m_packetSize=packetByteSize;
	}
}