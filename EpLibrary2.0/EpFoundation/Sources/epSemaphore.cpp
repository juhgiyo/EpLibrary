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
	m_sem=EP_NEW CSemaphore(count,count,semName,lpsaAttributes);
	m_singleLock=EP_NEW CSingleLock(m_sem,FALSE);
#if defined(_DEBUG)
	m_semDebug=EP_NEW CSemaphore(1,1,NULL,lpsaAttributes);
	m_singleLockDebug=EP_NEW CSingleLock(m_semDebug,FALSE);
#endif //defined(_DEBUG)
}
Semaphore::Semaphore(const Semaphore& b) :BaseLock()
{
	m_lpsaAttributes=b.m_lpsaAttributes;
	m_count=b.m_count;
	m_sem=EP_NEW CSemaphore(m_count,m_count,NULL,m_lpsaAttributes);
	m_singleLock=EP_NEW CSingleLock(m_sem,FALSE);
#if defined(_DEBUG)
	m_semDebug=EP_NEW CSemaphore(1,1,NULL,m_lpsaAttributes);
	m_singleLockDebug=EP_NEW CSingleLock(m_semDebug,FALSE);
#endif //defined(_DEBUG)
}
Semaphore::~Semaphore()
{
	if(m_singleLock)
		EP_DELETE m_singleLock;
	if(m_sem)
		EP_DELETE m_sem;
#if defined(_DEBUG)
	if(m_singleLockDebug)
		EP_DELETE m_singleLockDebug;
	if(m_semDebug)
		EP_DELETE m_semDebug;
#endif //defined(_DEBUG)
}

void Semaphore::Lock()
{
#if _DEBUG
	m_singleLockDebug->Lock();
	std::vector<int>::iterator iter;
	int threadID=GetCurrentThreadId();
	for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
	{
		EP_VERIFY_THREAD_DEADLOCK_ERROR(*iter!=threadID);
	}
	m_singleLockDebug->Unlock();
#endif //_DEBUG
	m_singleLock->Lock();
#if defined(_DEBUG)
	m_singleLockDebug->Lock();
	m_threadList.push_back(threadID);
	m_singleLockDebug->Unlock();
#endif //defined(_DEBUG)
}

long Semaphore::TryLock()
{
#if defined(_DEBUG)
	m_singleLockDebug->Lock();
	std::vector<int>::iterator iter;
	int threadID=GetCurrentThreadId();
	for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
	{
		EP_VERIFY_THREAD_DEADLOCK_ERROR(*iter!=threadID);
	}
	m_singleLockDebug->Unlock();
#endif //defined(_DEBUG)
	long ret=static_cast<long>( m_singleLock->Lock(0) );
#if defined(_DEBUG)
	if(ret)
	{
		m_singleLockDebug->Lock();
		m_threadList.push_back(threadID);
		m_singleLockDebug->Unlock();
	}
#endif //defined(_DEBUG)
	return ret;
}
long Semaphore::TryLockFor(const unsigned int dwMilliSecond)
{
#if defined(_DEBUG)
	m_singleLockDebug->Lock();
	std::vector<int>::iterator iter;
	int threadID=GetCurrentThreadId();
	for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
	{
		EP_VERIFY_THREAD_DEADLOCK_ERROR(*iter!=threadID);
	}
	m_singleLockDebug->Unlock();
#endif //defined(_DEBUG)
	long ret=static_cast<long>( m_singleLock->Lock(dwMilliSecond) );
#if _DEBUG
	if(ret)
	{
		m_singleLockDebug->Lock();
		m_threadList.push_back(threadID);
		m_singleLockDebug->Unlock();
	}
#endif //_DEBUG
	return ret;
}
void Semaphore::Unlock()
{
#if _DEBUG
	m_singleLockDebug->Lock();
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
	m_singleLockDebug->Unlock();
#endif //_DEBUG
	m_singleLock->Unlock();
}

