/*! 
NetworkStream for the EpLibrary

The MIT License (MIT)

Copyright (c) 2012-2013 Woong Gyu La <juhgiyo@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "epNetworkStream.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

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
		Stream::operator =(b);
		LockObj lock(b.m_streamLock);
		m_flushType=b.m_flushType;
		m_readOffset=b.m_readOffset;
		
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

void NetworkStream::SetWriteSeek(const StreamSeekType seekType,size_t offset)
{
	Stream::SetSeek(seekType,offset);
}

size_t NetworkStream::GetWriteSeek() const
{
	return Stream::GetSeek();
}


void NetworkStream::SetReadSeek(const StreamSeekType seekType,size_t offset)
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

size_t NetworkStream::GetReadSeek() const
{
	return m_readOffset;
}

void NetworkStream::SetSeek(const StreamSeekType seekType,size_t offset)
{
	return;
}

size_t NetworkStream::GetSeek() const
{
	return 0;
}

bool NetworkStream::read(void *value,size_t byteSize)
{
	bool retVal=false;

	if(!m_stream.empty() && value && m_stream.size()>=m_readOffset+byteSize)
	{
		System::Memcpy(value,&m_stream.at(m_readOffset) , byteSize);
		m_readOffset+=byteSize;
		retVal=true;
	}
	m_streamLock->Unlock();	
	if(m_flushType==NETWORK_STREAM_FLUSH_TYPE_AUTO)
		Flush();
	return retVal;
}