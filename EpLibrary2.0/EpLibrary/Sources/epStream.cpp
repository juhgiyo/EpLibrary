/*! 
Stream for the EpLibrary
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
#include "epStream.h"
#include "epSimpleLogger.h"
using namespace epl;


Stream::Stream(LockPolicy lockPolicyType)
{
	m_offset=0;
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_streamLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_streamLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_streamLock=EP_NEW NoLock();
		break;
	default:
		m_streamLock=NULL;
		break;
	}
}

Stream::Stream(const Stream& b)
{
	m_stream=b.m_stream;
	m_offset=b.m_offset;
	m_lockPolicy=b.m_lockPolicy;
	switch(m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_streamLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_streamLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_streamLock=EP_NEW NoLock();
		break;
	default:
		m_streamLock=NULL;
		break;
	}
}

Stream & Stream::operator=(const Stream&b)
{
	if(this!=&b)
	{
		LockObj lock(m_streamLock);
		m_stream=b.m_stream;
		m_offset=b.m_offset;
	}
	return *this;
}


Stream::~Stream()
{
	if(m_streamLock)
		EP_DELETE m_streamLock;
}

void Stream::Clear()
{
	LockObj lock(m_streamLock);
	m_offset=0;
	m_stream.clear();
}

int Stream::GetStreamSize() const
{
	return m_stream.size();
}

const unsigned char *Stream::GetBuffer() const
{
	if(m_stream.size())
		return &m_stream.at(0);
	return NULL;
}

void Stream::SetSeek(const StreamSeekType seekType,const unsigned int offset)
{
	LockObj lock(m_streamLock);
	switch(seekType)
	{
	case STREAM_SEEK_TYPE_SEEK_SET:
		m_offset=offset;
		break;
	case STREAM_SEEK_TYPE_SEEK_CUR:
		m_offset+=offset;
		break;
	case STREAM_SEEK_TYPE_SEEK_END:
		m_offset=m_stream.size();
	}
}
unsigned int Stream::GetSeek() const
{
	return m_offset;
}

bool Stream::write(const void *value,const int byteSize)
{
	if(!value)
		return false;
	if(m_stream.size()<static_cast<size_t>(m_offset+byteSize))
	{
		m_stream.resize(m_offset+byteSize);
	}
	System::Memcpy(&m_stream.at(m_offset), value, byteSize);
	m_offset+=byteSize;

	return true;
}

bool Stream::read(void *value,const int byteSize)
{
	if(m_stream.empty() || !value)
		return false;

	if(m_stream.size()>static_cast<size_t>(m_offset+byteSize))
	{
		System::Memcpy(value,&m_stream.at(m_offset) , byteSize);
		m_offset+=byteSize;
		return true;
	}
	return false;

}


bool Stream::WriteShort(const short value)
{
	LockObj lock(m_streamLock);
	return write(&value,sizeof(short));
}

bool Stream::WriteUShort(const unsigned short value)
{
	LockObj lock(m_streamLock);
	return write(&value,sizeof(unsigned short));
}

bool Stream::WriteInt(const int value)
{
	LockObj lock(m_streamLock);
	return write(&value,sizeof(int));
}

bool Stream::WriteUInt(const unsigned int value)
{
	LockObj lock(m_streamLock);
	return write(&value,sizeof(unsigned int));
}

bool Stream::WriteFloat(const float value)
{
	LockObj lock(m_streamLock);
	return write(&value,sizeof(float));
}

bool Stream::WriteDouble(const double value)
{
	LockObj lock(m_streamLock);
	return write(&value,sizeof(double));
}
bool Stream::WriteByte(const unsigned char value)
{
	LockObj lock(m_streamLock);
	return write(&value,sizeof(unsigned char));
}

bool Stream::WriteShorts(const short *shortList, const unsigned int listSize)
{
	LockObj lock(m_streamLock);
	return write(shortList,sizeof(short)*listSize);
}
bool Stream::WriteUShorts(const unsigned short *ushortList, const unsigned int listSize)
{
	LockObj lock(m_streamLock);
	return write(ushortList,sizeof(unsigned short)*listSize);
}
bool Stream::WriteInts(const int *intList, const unsigned int listSize)
{
	LockObj lock(m_streamLock);
	return write(intList,sizeof(int)*listSize);
}
bool Stream::WriteUInts(const unsigned int *uintList, const unsigned int listSize)
{
	LockObj lock(m_streamLock);
	return write(uintList,sizeof(unsigned int)*listSize);
}
bool Stream::WriteFloats(const float *floatList, const unsigned int listSize)
{
	LockObj lock(m_streamLock);
	return write(floatList,sizeof(float)*listSize);
}
bool Stream::WriteDoubles(const double *doubleList,const unsigned int listSize)
{
	LockObj lock(m_streamLock);
	return write(doubleList,sizeof(double)*listSize);
}
bool Stream::WriteBytes(const unsigned char* byteList,const unsigned int listSize)
{
	LockObj lock(m_streamLock);
	return write(byteList,sizeof(unsigned char)*listSize);
}

bool Stream::WriteString(const char *str)
{
	if(!str)
		return false;
	EpString convString=str;
	return WriteBytes(reinterpret_cast<const unsigned char*>(convString.c_str()),convString.size()+1);
}

bool Stream::WriteWString(const wchar_t *str)
{
	if(!str)
		return false;
	EpWString convString=str;
	return WriteBytes(reinterpret_cast<const unsigned char*>(convString.c_str()),(convString.size()+1)*sizeof(wchar_t));
}
bool Stream::WriteTString(const TCHAR *str)
{
	if(!str)
		return false;
	EpTString convString=str;
	return WriteBytes(reinterpret_cast<const unsigned char*>(convString.c_str()),(convString.size()+1)*sizeof(TCHAR));
}


bool Stream::WriteString(const EpString &str)
{
	return WriteBytes(reinterpret_cast<const unsigned char*>(str.c_str()),str.size()+1);
}

bool Stream::WriteWString(const EpWString &str)
{
	return WriteBytes(reinterpret_cast<const unsigned char*>(str.c_str()),(str.size()+1)*sizeof(wchar_t));
}
bool Stream::WriteTString(const EpTString &str)
{
	return WriteBytes(reinterpret_cast<const unsigned char*>(str.c_str()),(str.size()+1)*sizeof(TCHAR));
}


bool Stream::ReadShort(short &retVal)
{
	LockObj lock(m_streamLock);
	return read(&retVal,sizeof(short));
}
bool Stream::ReadUShort(unsigned short &retVal)
{
	LockObj lock(m_streamLock);
	return read(&retVal,sizeof(unsigned short));
}
bool Stream::ReadInt(int &retVal)
{
	LockObj lock(m_streamLock);
	return read(&retVal,sizeof(int));
}
bool Stream::ReadUInt(unsigned int &retVal)
{
	LockObj lock(m_streamLock);
	return read(&retVal,sizeof(unsigned int));
}
bool Stream::ReadFloat(float &retVal)
{
	LockObj lock(m_streamLock);
	return read(&retVal,sizeof(float));
}
bool Stream::ReadDouble(double &retVal)
{
	LockObj lock(m_streamLock);
	return read(&retVal,sizeof(double));
}
bool Stream::ReadByte(unsigned char &retVal)
{
	LockObj lock(m_streamLock);
	return read(&retVal,sizeof(unsigned char));
}

bool Stream::ReadShorts(short *retShortList, const unsigned int listSize)
{
	LockObj lock(m_streamLock);
	return read(retShortList,sizeof(short)*listSize);
}
bool Stream::ReadUShorts(unsigned short *retUshortList, const unsigned int listSize)
{
	LockObj lock(m_streamLock);
	return read(retUshortList,sizeof(unsigned short)*listSize);
}
bool Stream::ReadInts(int *retIntList, const unsigned int listSize)
{
	LockObj lock(m_streamLock);
	return read(retIntList,sizeof(int)*listSize);
}
bool Stream::ReadUInts(unsigned int *retUintList, const unsigned int listSize)
{
	LockObj lock(m_streamLock);
	return read(retUintList,sizeof(unsigned int)*listSize);
}
bool Stream::ReadFloats(float *retFloatList, const unsigned int listSize)
{
	LockObj lock(m_streamLock);
	return read(retFloatList,sizeof(float)*listSize);
}
bool Stream::ReadDoubles(double *retDoubleList, const unsigned int listSize)
{
	LockObj lock(m_streamLock);
	return read(retDoubleList,sizeof(float)*listSize);
}
bool Stream::ReadBytes(unsigned char* retByteList, const unsigned int listSize)
{
	LockObj lock(m_streamLock);
	return read(retByteList,sizeof(unsigned char)*listSize);
}

bool Stream::ReadString(EpString &retString)
{
	LockObj lock(m_streamLock);
	char retChar;
	retString="";
	bool status=false;
	while((status=read(&retChar,sizeof(char))) && (char)retChar!='\0')
	{
		retString=retString.append(1,retChar);
	}
	return status;
}
bool Stream::ReadWString(EpWString &retString)
{
	LockObj lock(m_streamLock);
	wchar_t retChar;
	retString=WIDEN("");
	bool status=false;
	while((status=read(&retChar,sizeof(wchar_t))) && (wchar_t)retChar!=WIDEN('\0'))
	{
		retString=retString.append(1,retChar);
	}
	return status;
}
bool Stream::ReadTString(EpTString &retString)
{
	LockObj lock(m_streamLock);
	TCHAR retChar;
	retString=_T("");
	bool status=false;
	while((status=read(&retChar,sizeof(TCHAR))) && (TCHAR)retChar!=_T('\0'))
	{
		retString=retString.append(1,static_cast<TCHAR>(retChar));
	}
	return status;
}


bool Stream::WriteStreamToFile(const TCHAR *fileName)
{
	LockObj lock(m_streamLock);
	if(System::TcsLen(fileName)==0)
	{
		LOG_THIS_MSG(_T("File Name Not Set!"));
		return false;
	}
	if(m_stream.empty())
	{
		LOG_THIS_MSG(_T("There is no stream data!"));
		return false;
	}
	EpFile *file;
	System::FTOpen(file,fileName,_T("wt"));
	System::FWrite(&m_stream.at(0),sizeof(unsigned char),m_stream.size(),file);
	System::FClose(file);
	return true;
}
