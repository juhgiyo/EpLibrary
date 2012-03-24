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

Packet::Packet(char *packet,int byteSize):SmartObject()
{
	m_packet=NULL;
	m_packetSize=0;
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

Packet::Packet(const Packet& b):SmartObject()
{
	m_packet=NULL;
	if(b.m_packetSize>0)
	{
		m_packet=EP_NEW char[b.m_packetSize];
		if(b.m_packet)
			memcpy(m_packet,b.m_packet,b.m_packetSize);
		else
			memset(m_packet,0,b.m_packetSize);
	}
	m_packetSize=b.m_packetSize;
}
Packet & Packet::operator=(const Packet&b)
{
	if(m_packetSize<b.m_packetSize)
	{
		EP_DELETE[] m_packet;
		m_packet=NULL;
	}
	if(m_packet==NULL)
	{
		m_packet=EP_NEW char[b.m_packetSize];
	}
	if(b.m_packet)
		memcpy(m_packet,b.m_packet,b.m_packetSize);
	else
		memset(m_packet,0,b.m_packetSize);
	m_packetSize=b.m_packetSize;
	return *this;
}

Packet::~Packet()
{
	if(m_packet)
	{
		EP_DELETE[] m_packet;
	}
	m_packet=NULL;
}

int Packet::GetPacketByteSize() const
{
	return m_packetSize;
}

const char *Packet::GetPacket() const
{
	return m_packet;	
}

void Packet::SetPacket(char* packet, int packetByteSize)
{
	if(m_packetSize<packetByteSize)
	{
		EP_DELETE[] m_packet;
		m_packet=NULL;
	}
	if(m_packet==NULL)
	{
		m_packet=EP_NEW char[packetByteSize];
	}
	if(packet)
		memcpy(m_packet,packet,packetByteSize);
	else
		memset(m_packet,0,packetByteSize);
	m_packetSize=packetByteSize;

}