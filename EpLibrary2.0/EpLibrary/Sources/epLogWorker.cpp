/*! 
LogWorker for the EpLibrary

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
#include "epLogWorker.h"
#include "epFolderHelper.h"
using namespace epl;
LogWorker::LogWorker(EpTString fileName,FileEncodingType encodingType,LockPolicy lockPolicyType):BaseTextFile(encodingType,lockPolicyType),Thread(EP_THREAD_PRIORITY_ABOVE_NORMAL,lockPolicyType)
{
	m_fileName=fileName.c_str();
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_logLock=EP_NEW CriticalSectionEx();
		m_threadLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_logLock=EP_NEW Mutex();
		m_threadLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_logLock=EP_NEW NoLock();
		m_threadLock=EP_NEW NoLock();
		break;
	default:
		m_logLock=NULL;
		m_threadLock=NULL;
		break;
	}
	m_shouldTerminate=false;
}

LogWorker::~LogWorker()
{
	stop();
	if(m_logLock)
		EP_DELETE m_logLock;
	if(m_threadLock)
		EP_DELETE m_threadLock;
}

LogWorker::LogWorker(const LogWorker& b):BaseTextFile(b),Thread(b)
{
	m_fileName=b.m_fileName;
	switch(BaseTextFile::m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_logLock=EP_NEW CriticalSectionEx();
		m_threadLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_logLock=EP_NEW Mutex();
		m_threadLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_logLock=EP_NEW NoLock();
		m_threadLock=EP_NEW NoLock();
		break;
	default:
		m_logLock=NULL;
		m_threadLock=NULL;
		break;
	}
}
void LogWorker::execute()
{
	m_threadLock->Lock();
	while(!m_shouldTerminate)
	{
		m_threadLock->Unlock();
		Yield();
		
		m_logString=_T("");
		
		m_logLock->Lock();
		while(!m_logQueue.empty())
		{
			CString logString=m_logQueue.front();
			m_logQueue.pop();
			SYSTEMTIME oT;
			::GetLocalTime(&oT);
			m_logString.AppendFormat(_T("[%04d-%02d-%02d %02d:%02d:%02d -%04] : %s\n"),oT.wYear,oT.wMonth,oT.wDay,oT.wHour,oT.wMinute,oT.wSecond,oT.wMilliseconds,logString.GetString());
		}
		m_logLock->Unlock();

		if(m_logString.GetLength()>0)
			AppendToFile(m_fileName);


		m_threadLock->Lock();
	}
	m_threadLock->Unlock();
}
void LogWorker::stop()
{
	if(GetStatus()!=ThreadStatus::THREAD_STATUS_TERMINATED)
	{
		m_threadLock->Lock();
		m_shouldTerminate=true;
		m_threadLock->Unlock();
		WaitFor();
	}
}
void LogWorker::WriteLog(const  TCHAR* pMsg)
{
	// write error or other information into log file
	LockObj lock(m_logLock);
	m_logQueue.push(pMsg);
}

void LogWorker::writeLoop()
{
	writeToFile(m_logString.GetString());
}

void LogWorker::loadFromFile(const EpTString &lines)
{
}