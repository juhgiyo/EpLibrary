/*! 
Semaphore for the EpLibrary
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
#include "epSemaphore.h"
#include "epSystem.h"
#include "epException.h"
using namespace epl;


Semaphore::Semaphore(unsigned int count,const TCHAR *semName, LPSECURITY_ATTRIBUTES lpsaAttributes) :BaseLock()
{
	m_lpsaAttributes=lpsaAttributes;
	m_count=count;
	m_sem=CreateSemaphore(lpsaAttributes,count,count,semName);
#if defined(_DEBUG)
	m_semDebug=CreateSemaphore(lpsaAttributes,1,1,NULL);
#endif //defined(_DEBUG)
}
Semaphore::Semaphore(const Semaphore& b) :BaseLock()
{
	m_lpsaAttributes=b.m_lpsaAttributes;
	m_count=b.m_count;
	m_sem=CreateSemaphore(m_lpsaAttributes,m_count,m_count,NULL);
#if defined(_DEBUG)
	m_semDebug=CreateSemaphore(m_lpsaAttributes,1,1,NULL);
#endif //defined(_DEBUG)
}
Semaphore::~Semaphore()
{
	CloseHandle(m_sem);
#if defined(_DEBUG)
	CloseHandle(m_semDebug);
#endif //defined(_DEBUG)
}

bool Semaphore::Lock()
{
#if _DEBUG
	System::WaitForSingleObject(m_semDebug,WAITTIME_INIFINITE);
	std::vector<int>::iterator iter;
	int threadID=GetCurrentThreadId();
	for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
	{
		EP_VERIFY_THREAD_DEADLOCK_ERROR(*iter!=threadID);
	}
	ReleaseSemaphore(m_semDebug,1,NULL);
#endif //_DEBUG
	System::WaitForSingleObject(m_sem,WAITTIME_INIFINITE);
#if defined(_DEBUG)
	System::WaitForSingleObject(m_semDebug,WAITTIME_INIFINITE);
	m_threadList.push_back(threadID);
	ReleaseSemaphore(m_semDebug,1,NULL);
#endif //defined(_DEBUG)
	return true;
}

long Semaphore::TryLock()
{
#if defined(_DEBUG)
	System::WaitForSingleObject(m_semDebug,WAITTIME_INIFINITE);
	std::vector<int>::iterator iter;
	int threadID=GetCurrentThreadId();
	for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
	{
		EP_VERIFY_THREAD_DEADLOCK_ERROR(*iter!=threadID);
	}
	ReleaseSemaphore(m_semDebug,1,NULL);
#endif //defined(_DEBUG)
	long ret=0;
	if(System::WaitForSingleObject(m_sem,WAITTIME_IGNORE) == WAIT_OBJECT_0 )
		ret=1;
#if defined(_DEBUG)
	if(ret)
	{
		System::WaitForSingleObject(m_semDebug,WAITTIME_INIFINITE);
		m_threadList.push_back(threadID);
		ReleaseSemaphore(m_semDebug,1,NULL);
	}
#endif //defined(_DEBUG)
	return ret;
}
long Semaphore::TryLockFor(const unsigned int dwMilliSecond)
{
#if defined(_DEBUG)
	System::WaitForSingleObject(m_semDebug,WAITTIME_INIFINITE);
	std::vector<int>::iterator iter;
	int threadID=GetCurrentThreadId();
	for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
	{
		EP_VERIFY_THREAD_DEADLOCK_ERROR(*iter!=threadID);
	}
	ReleaseSemaphore(m_semDebug,1,NULL);
#endif //defined(_DEBUG)
	long ret=0;
	if( System::WaitForSingleObject(m_sem,dwMilliSecond) == WAIT_OBJECT_0)
		ret=1;
#if _DEBUG
	if(ret)
	{
		System::WaitForSingleObject(m_semDebug,WAITTIME_INIFINITE);
		m_threadList.push_back(threadID);
		ReleaseSemaphore(m_semDebug,1,NULL);
	}
#endif //_DEBUG
	return ret;
}
void Semaphore::Unlock()
{
#if defined(_DEBUG)
	System::WaitForSingleObject(m_semDebug,WAITTIME_INIFINITE);
	std::vector<int>::iterator iter;
	int threadID=GetCurrentThreadId();
	for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
	{
		if(*iter==threadID)
		{
			m_threadList.erase(iter);
			break;
		}
	}
	ReleaseSemaphore(m_semDebug,1,NULL);
#endif //_DEBUG
	ReleaseSemaphore(m_sem,1,NULL);
}

long Semaphore::Release(long count, long * retPreviousCount)
{
#if defined(_DEBUG)
	long debugCount=count;
	System::WaitForSingleObject(m_semDebug,WAITTIME_INIFINITE);
	std::vector<int>::iterator iter;
	int threadID=GetCurrentThreadId();
	for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
	{
		if(*iter==threadID)
		{
			m_threadList.erase(iter);
			debugCount--;
			break;
		}
	}
	if(debugCount>=m_threadList.size())
		m_threadList.clear();
	ReleaseSemaphore(m_semDebug,1,NULL);
#endif //_DEBUG
	return ReleaseSemaphore(m_sem,count,retPreviousCount);
}

