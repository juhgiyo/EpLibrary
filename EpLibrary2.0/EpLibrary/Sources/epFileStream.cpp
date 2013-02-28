/*! 
FileStream for the EpLibrary
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
#include "epFileStream.h"
#include "epSimpleLogger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace epl;

FileStream::FileStream(const TCHAR *fileName,LockPolicy lockPolicyType) :Stream(lockPolicyType)
{
	m_fileName=fileName;
}

FileStream::FileStream(const FileStream& b):Stream(b)
{
	m_fileName=b.m_fileName;
}

FileStream & FileStream::operator=(const FileStream&b)
{
	if(this!=&b)
	{
		Stream::operator =(b);
		LockObj lock(b.m_streamLock);
		m_fileName=b.m_fileName;
		
	}
	return *this;
}

FileStream::~FileStream()
{
}

void FileStream::SetFileName(const TCHAR *fileName)
{
	LockObj lock(m_streamLock);
	m_fileName=fileName;
}
EpTString FileStream::GetFileName() const
{
	return m_fileName;
}

bool FileStream::LoadStreamFromFile()
{
	LockObj lock(m_streamLock);
	m_stream.clear();

	if(m_fileName.length()==0)
	{
		LOG_THIS_MSG(_T("File Name Not Set!"));
		return false;
	}
	EpFile *file;
	int fileSize;
	System::FTOpen(file,m_fileName.c_str(),_T("rb"));
	fileSize=System::FSize(file);
	m_stream.resize(fileSize);
	int read=System::FRead(&m_stream.at(0),sizeof(unsigned char), fileSize,file);
	m_stream.erase(m_stream.begin()+read,m_stream.end());
	System::FClose(file);
	m_offset=m_stream.size();
	return true;
}
bool FileStream::WriteStreamToFile()
{
	LockObj lock(m_streamLock);
	if(m_fileName.length()==0)
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
	System::FTOpen(file,m_fileName.c_str(),_T("wb"));
	System::FWrite(&m_stream.at(0),sizeof(unsigned char),m_stream.size(),file);
	System::FClose(file);
	return true;
}

bool FileStream::write(const void *value,const int byteSize)
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

bool FileStream::read(void *value,const int byteSize)
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
