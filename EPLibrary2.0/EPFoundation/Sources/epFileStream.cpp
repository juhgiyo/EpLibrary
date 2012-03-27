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
using namespace epl;

FileStream::FileStream(TCHAR *fileName) :Stream()
{
	m_fileName=EP_NEW TCHAR[FILENAME_MAX ];
	System::StrCpy(m_fileName,fileName);
}

FileStream::FileStream(const FileStream& b):Stream()
{
	m_fileName=EP_NEW TCHAR[FILENAME_MAX ];
	System::StrCpy(m_fileName,b.m_fileName);
	m_stream=b.m_stream;
	m_offset=b.m_offset;
}

FileStream & FileStream::operator=(const FileStream&b)
{
	if(this!=&b)
	{
		if(!m_fileName)
			m_fileName=EP_NEW TCHAR[FILENAME_MAX];
		memset(m_fileName,0,sizeof(TCHAR)*FILENAME_MAX);
		System::StrCpy(m_fileName,b.m_fileName);
		m_stream=b.m_stream;
		m_offset=b.m_offset;
	}
	return *this;
}

FileStream::~FileStream()
{
	m_streamLock.Lock();
	EP_DELETE[] m_fileName;
	m_streamLock.Unlock();
}

void FileStream::SetFileName(const TCHAR* fileName)
{
	LockObj lock(&m_streamLock);
	memset(m_fileName,0,FILENAME_MAX);
	System::StrCpy(m_fileName,fileName);
}
const TCHAR *FileStream::GetFileName() const
{
	return m_fileName;
}

bool FileStream::LoadStreamFromFile()
{
	LockObj lock(&m_streamLock);
	m_stream.clear();

	if(System::StrLen(m_fileName)==0)
	{
		LOG_THIS_MSG(_T("File Name Not Set!"));
		return false;
	}
	EpFile *file;
	int fileSize;
	System::FOpen(file,m_fileName,_T("rt"));
	fileSize=System::FSize(file);
	m_stream.resize(fileSize);
	System::FRead(&m_stream.at(0),sizeof(unsigned char), fileSize,file);
	System::FClose(file);
	m_offset=m_stream.size();
	return true;
}
bool FileStream::WriteStreamToFile()
{
	LockObj lock(&m_streamLock);
	if(System::StrLen(m_fileName)==0)
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
	System::FOpen(file,m_fileName,_T("wt"));
	System::FWrite(&m_stream.at(0),sizeof(unsigned char),m_stream.size(),file);
	System::FClose(file);
	return true;
}

bool FileStream::write(const void *value,const int byteSize)
{
	if(!value)
		return false;
	if(m_stream.size()<(size_t)(m_offset+byteSize))
	{
		m_stream.resize(m_offset+byteSize);
	}
	memcpy(&m_stream.at(m_offset), value, byteSize);
	m_offset+=byteSize;

	return true;
}

bool FileStream::read(void *value,const int byteSize)
{
	if(m_stream.empty() || !value)
		return false;

	if(m_stream.size()>(size_t)(m_offset+byteSize))
	{
		memcpy(value,&m_stream.at(m_offset) , byteSize);
		m_offset+=byteSize;
		return true;
	}
	return false;

}
