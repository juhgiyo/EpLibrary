/*! 
BinaryFile for the EpLibrary
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