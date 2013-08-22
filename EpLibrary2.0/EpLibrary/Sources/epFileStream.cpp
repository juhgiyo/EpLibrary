/*! 
FileStream for the EpLibrary

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
#include "epFileStream.h"
#include "epSimpleLogger.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

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
	size_t read=System::FRead(&m_stream.at(0),sizeof(unsigned char), fileSize,file);
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

bool FileStream::write(const void *value,size_t byteSize)
{
	if(!value)
		return false;
	if(m_stream.size()<m_offset+byteSize)
	{
		m_stream.resize(m_offset+byteSize);
	}
	System::Memcpy(&m_stream.at(m_offset), value, byteSize);
	m_offset+=byteSize;

	return true;
}

bool FileStream::read(void *value,size_t byteSize)
{
	if(m_stream.empty() || !value)
		return false;

	if(m_stream.size()>m_offset+byteSize)
	{
		System::Memcpy(value,&m_stream.at(m_offset) , byteSize);
		m_offset+=byteSize;
		return true;
	}
	return false;

}
