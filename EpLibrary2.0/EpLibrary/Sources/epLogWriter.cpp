/*! 
LogWriter for the EpWinServiceCLI

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
#include "epLogWriter.h"
#include "epFolderHelper.h"
using namespace epl;
#if  defined(_UNICODE) || defined(UNICODE)
LogWriter::LogWriter(LockPolicy lockPolicyType):BaseTextFile(FILE_ENCODING_TYPE_UTF16LE,lockPolicyType)
#else // defined(_UNICODE) || defined(UNICODE)
LogWriter::LogWriter(LockPolicy lockPolicyType):BaseTextFile(FILE_ENCODING_TYPE_UTF8,lockPolicyType)
#endif//  defined(_UNICODE) || defined(UNICODE)
{
	m_fileName=FolderHelper::GetModuleFileName().c_str();
	m_fileName.Replace(_T(".exe"),_T(".log"));
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_logLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_logLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_logLock=EP_NEW NoLock();
		break;
	default:
		m_logLock=NULL;
		break;
	}
}

LogWriter::~LogWriter()
{
	if(m_logLock)
		EP_DELETE m_logLock;
}

LogWriter::LogWriter(const LogWriter& b):BaseTextFile(b)
{
	m_fileName=b.m_fileName;
	m_lockPolicy=b.m_lockPolicy;
	switch(m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_logLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_logLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_logLock=EP_NEW NoLock();
		break;
	default:
		m_logLock=NULL;
		break;
	}
}

void LogWriter::WriteLog(const  TCHAR* pMsg)
{
	// write error or other information into log file
	LockObj lock(m_logLock);
	SYSTEMTIME oT;
	::GetLocalTime(&oT);
	m_logString=_T("");
	m_logString.AppendFormat(_T("%02d/%02d/%04d, %02d:%02d:%02d\n    %s\n"),oT.wMonth,oT.wDay,oT.wYear,oT.wHour,oT.wMinute,oT.wSecond,pMsg);
	AppendToFile(m_fileName);
}

void LogWriter::writeLoop()
{
	writeToFile(m_logString.GetString());
}

void LogWriter::loadFromFile(const EpTString &lines)
{
}