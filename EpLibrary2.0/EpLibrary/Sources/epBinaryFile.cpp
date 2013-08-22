/*! 
BinaryFile for the EpLibrary

The MIT License (MIT)

Copyright (c) 2008-2013 Woong Gyu La <juhgiyo@gmail.com>

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
#include "epBinaryFile.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;

BinaryFile::BinaryFile(LockPolicy lockPolicyType)
{
	m_file=NULL;
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_baseTextLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_baseTextLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_baseTextLock=EP_NEW NoLock();
		break;
	default:
		m_baseTextLock=NULL;
		break;
	}
}
BinaryFile::BinaryFile(const BinaryFile& b)
{
	m_file=b.m_file;
	m_lockPolicy=b.m_lockPolicy;
	m_stream=b.m_stream;
	switch(m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_baseTextLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_baseTextLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_baseTextLock=EP_NEW NoLock();
		break;
	default:
		m_baseTextLock=NULL;
		break;
	}
}
BinaryFile::~BinaryFile()
{
	if(m_baseTextLock)
		EP_DELETE m_baseTextLock;
	m_baseTextLock=NULL;
}
BinaryFile & BinaryFile::operator=(const BinaryFile&b)
{
	if(this!=&b)
	{
		if(m_baseTextLock)
			EP_DELETE m_baseTextLock;
		m_baseTextLock=NULL;

		m_file=b.m_file;
		m_stream=b.m_stream;

		m_lockPolicy=b.m_lockPolicy;
		switch(m_lockPolicy)
		{
		case LOCK_POLICY_CRITICALSECTION:
			m_baseTextLock=EP_NEW CriticalSectionEx();
			break;
		case LOCK_POLICY_MUTEX:
			m_baseTextLock=EP_NEW Mutex();
			break;
		case LOCK_POLICY_NONE:
			m_baseTextLock=EP_NEW NoLock();
			break;
		default:
			m_baseTextLock=NULL;
			break;
		}
	}
	return *this;
}



bool BinaryFile::SaveToFile(const TCHAR *filename)
{
	LockObj lock(m_baseTextLock);
	size_t strLength=System::TcsLen(filename);
	if(strLength<=0)
		return false;

	int e;

	Mutex fileLock=Mutex(filename);
	fileLock.Lock();

	e= System::FTOpen(m_file,filename,_T("wb"));
	
	if (e != 0) 
	{
		fileLock.Unlock();
		return false; // failed..
	}

	if(m_file)
		System::FWrite(m_stream.GetBuffer(),sizeof(unsigned char),m_stream.GetStreamSize(),m_file);

	System::FClose(m_file);
	fileLock.Unlock();
	m_file=NULL;
	return true;
}

bool BinaryFile::AppendToFile(const TCHAR *filename)
{
	LockObj lock(m_baseTextLock);
	size_t strLength=System::TcsLen(filename);
	if(strLength<=0)
		return false;

	int e;

	Mutex fileLock=Mutex(filename);
	fileLock.Lock();

	e= System::FTOpen(m_file,filename,_T("ab"));

	if (e != 0) 
	{
		fileLock.Unlock();
		return false; // failed..
	}

	if(m_file)
		System::FWrite(m_stream.GetBuffer(),sizeof(unsigned char),m_stream.GetStreamSize(),m_file);

	System::FClose(m_file);
	fileLock.Unlock();
	m_file=NULL;
	return true;
}


bool BinaryFile::LoadFromFile(const TCHAR *filename)
{
	LockObj lock(m_baseTextLock);
	size_t strLength=System::TcsLen(filename);
	if(strLength<=0)
		return false;

	int e;
	Mutex fileLock=Mutex(filename);
	fileLock.Lock();

	e= System::FTOpen(m_file,filename,_T("rb"));
	if (e != 0) 
	{
		fileLock.Unlock();
		return false; // failed..
	}
	
	//Find the actual length of file
	size_t length= System::FSize(m_file);
	if(length<=0)
	{
		fileLock.Unlock();
		return false;
	}

	
	unsigned char *cFileBuf=EP_NEW unsigned char[length];
	size_t read=System::FRead(cFileBuf,sizeof(unsigned char),length,m_file);
	System::FClose(m_file);
	EP_DELETE[] cFileBuf;

	fileLock.Unlock();
	m_stream.Clear();
	m_stream.WriteBytes(cFileBuf,length);
	m_stream.SetSeek(Stream::STREAM_SEEK_TYPE_SEEK_SET);

	m_file=NULL;
	return true;

}


Stream &BinaryFile::GetStream()
{
	LockObj lock(m_baseTextLock);
	return m_stream;
}

const Stream &BinaryFile::GetStream() const
{
	return m_stream;
}

void BinaryFile::SetStream(const Stream &stream)
{
	m_stream=stream;
}