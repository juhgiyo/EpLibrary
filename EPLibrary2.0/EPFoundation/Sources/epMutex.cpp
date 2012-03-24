/*! 
Mutex for the EpLibrary
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
#include "epMutex.h"
#include "epSystem.h"
using namespace epl;


Mutex::Mutex(TCHAR *mutexName, LPSECURITY_ATTRIBUTES lpsaAttributes) :BaseLock()
{
#if TEST_NEW
	m_mutex=EP_NEW CMutex(FALSE,mutexName,lpsaAttributes);
	m_singleLock=EP_NEW CSingleLock(m_mutex,FALSE);
#else //TEST_NEW
	InitializeCriticalSection(&m_criticalSection);
#endif //TEST_NEW
}

Mutex::~Mutex()
{
#if TEST_NEW
	EP_DELETE m_singleLock;
	EP_DELETE m_mutex;
#else //TEST_NEW
	DeleteCriticalSection(&m_criticalSection);
#endif //TEST_NEW
}

void Mutex::Lock()
{

#if _DEBUG
	std::vector<int>::iterator iter;
	int threadID=GetCurrentThreadId();
	for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
	{
		if(*iter==threadID)
			EP_ASSERT(0);
	}
#endif //_DEBUG

#if TEST_NEW
	m_singleLock->Lock();
#else //TEST_NEW
	EnterCriticalSection (&m_criticalSection);	
	while (m_lockCnt)
	{
		LeaveCriticalSection(&m_criticalSection);
		Sleep(MUTEX_WAIT_TIME_IN_MILLI_SEC);
		EnterCriticalSection(&m_criticalSection);
	}
	m_lockCnt = 1;
	LeaveCriticalSection(&m_criticalSection);
#endif //TEST_NEW
}

long Mutex::TryLock()
{
	long ret=0;
#if TEST_NEW
	ret=(long)m_singleLock->Lock(0);
#else //TEST_NEW
	EnterCriticalSection (&m_criticalSection);	
	if (m_lockCnt)
	{
		LeaveCriticalSection(&m_criticalSection);
		ret=0;
	}
	else
	{
		m_lockCnt = 1;
		LeaveCriticalSection(&m_criticalSection);
		ret=1;
	}
#endif //TEST_NEW

#if _DEBUG
	if(ret)
	{
		std::vector<int>::iterator iter;
		int threadID=GetCurrentThreadId();
		for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
		{
			if(*iter==threadID)
				EP_ASSERT(0);
		}
		m_threadList.push_back(threadID);
	}
#endif //_DEBUG
	return ret;

}

long Mutex::TryLockFor(const unsigned int dwMilliSecond)
{
	long ret=0;
#if TEST_NEW
	ret=(long)m_singleLock->Lock(dwMilliSecond);
#else //TEST_NEW
	
	if(ret=TryLock())
	{
	}
	else
	{
		unsigned int startTime,timeUsed;
		unsigned int waitTime=dwMilliSecond;
		startTime=System::GetTickCount();
		while(waitTime>0)
		{
			if(ret=TryLock())
			{
				break;
			}		
			Sleep(MUTEX_WAIT_TIME_IN_MILLI_SEC);
			timeUsed=System::GetTickCount()-startTime;
			waitTime=waitTime-(unsigned int)timeUsed;
			startTime=System::GetTickCount();
		}
	}
#endif //TEST_NEW

#if _DEBUG
	if(ret)
	{
		std::vector<int>::iterator iter;
		int threadID=GetCurrentThreadId();
		for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
		{
			if(*iter==threadID)
				EP_ASSERT(0);
		}
		m_threadList.push_back(threadID);
	}
#endif //_DEBUG
	return ret;

}
void Mutex::Unlock()
{
#if _DEBUG
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
#endif //_DEBUG

#if TEST_NEW
	m_singleLock->Unlock();
#else //TEST_NEW
	EnterCriticalSection(&m_criticalSection);
	m_lockCnt= 0;
	LeaveCriticalSection(&m_criticalSection);	
#endif //TEST_NEW
}
