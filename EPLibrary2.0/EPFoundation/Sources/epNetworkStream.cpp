/*! 
NetworkStream for the EpLibrary
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
#include "epNetworkStream.h"

using namespace epl;

NetworkStream::NetworkStream(const NetworkStreamFlushType type, LockPolicy lockPolicyType) :Stream(lockPolicyType)
{
	m_flushType=type;
	m_readOffset=0;
}

NetworkStream::NetworkStream(const NetworkStream& b):Stream(b)
{
	m_flushType=b.m_flushType;
	m_readOffset=b.m_readOffset;
}

NetworkStream & NetworkStream::operator=(const NetworkStream&b)
{
	if(this!=&b)
	{
		m_flushType=b.m_flushType;
		m_readOffset=b.m_readOffset;
		Stream::operator =(b);
	}
	return *this;
}


NetworkStream::~NetworkStream()
{

}


NetworkStream::NetworkStreamFlushType NetworkStream::GetFlushType() const
{
	return m_flushType;
}


void NetworkStream::Flush()
{
	LockObj lock(m_streamLock);
	if(m_readOffset==0)
		return;

	m_stream.erase(m_stream.begin(),m_stream.begin()+m_readOffset-1);
	m_offset-=m_readOffset;
	m_readOffset=0;
}

void NetworkStream::SetWriteSeek(const StreamSeekType seekType,const unsigned int offset)
{
	Stream::SetSeek(seekType,offset);
}

unsigned int NetworkStream::GetWriteSeek() const
{
	return Stream::GetSeek();
}


void NetworkStream::SetReadSeek(const StreamSeekType seekType,const unsigned int offset)
{
	LockObj lock(m_streamLock);
	switch(seekType)
	{
	case STREAM_SEEK_TYPE_SEEK_SET:
		m_readOffset=offset;
		break;
	case STREAM_SEEK_TYPE_SEEK_CUR:
		m_readOffset+=offset;
		break;
	case STREAM_SEEK_TYPE_SEEK_END:
		m_readOffset=m_stream.size();
	}
}

unsigned int NetworkStream::GetReadSeek() const
{
	return m_readOffset;
}

void NetworkStream::SetSeek(const StreamSeekType seekType,const unsigned int offset)
{
	return;
}

unsigned int NetworkStream::GetSeek() const
{
	return 0;
}

bool NetworkStream::read(void *value,const int byteSize)
{
	bool retVal=false;

	if(!m_stream.empty() && value && m_stream.size()>=(size_t)(m_readOffset+byteSize))
	{
		memcpy(value,&m_stream.at(m_readOffset) , byteSize);
		m_readOffset+=byteSize;
		retVal=true;
	}
	m_streamLock->Unlock();	
	if(m_flushType==NETWORK_STREAM_FLUSH_TYPE_AUTO)
		Flush();
	return retVal;
}