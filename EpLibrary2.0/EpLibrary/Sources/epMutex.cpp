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
#include "epException.h"
using namespace epl;


Mutex::Mutex(const TCHAR *mutexName, LPSECURITY_ATTRIBUTES lpsaAttributes) :BaseLock()
{
	m_isMutexAbandoned=false;
	m_lpsaAttributes=lpsaAttributes;
	m_mutex=CreateMutex(lpsaAttributes,FALSE,mutexName);
#if defined(_DEBUG)
	m_mutexDebug=CreateMutex(lpsaAttributes,FALSE,NULL);
#endif //defined(_DEBUG)
}

Mutex::Mutex(const Mutex& b)
{
	m_isMutexAbandoned=false;
	m_lpsaAttributes=b.m_lpsaAttributes;
	m_mutex=CreateMutex(m_lpsaAttributes,FALSE,NULL);
#if defined(_DEBUG)
	m_mutexDebug=CreateMutex(m_lpsaAttributes,FALSE,NULL);
#endif //defined(_DEBUG)
}

Mutex::~Mutex()
{

	CloseHandle(m_mutex);
#if defined(_DEBUG)
	CloseHandle(m_mutexDebug);
#endif //defined(_DEBUG)
}

bool Mutex::Lock()
{

#if _DEBUG
	unsigned long resDebug=System::WaitForSingleObject(m_mutexDebug,WAITTIME_INIFINITE);
	EP_ASSERT_EXPR(resDebug!=WAIT_ABANDONED,_T("Obtained abandoned Debug Mutex."));
	
	std::vector<int>::iterator iter;
	int threadID=GetCurrentThreadId();
	for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
	{
		EP_VERIFY_THREAD_DEADLOCK_ERROR(*iter!=threadID);
	}
#endif //_DEBUG

	bool returnVal=true;
	unsigned long res=System::WaitForSingleObject(m_mutex,WAITTIME_INIFINITE);
	if(res==WAIT_ABANDONED)
	{
		m_isMutexAbandoned=true;
		returnVal=false;
	}

#if defined(_DEBUG)
	m_threadList.push_back(threadID);
	ReleaseMutex(m_mutexDebug);
#endif //defined(_DEBUG)
	return returnVal;
}

long Mutex::TryLock()
{
#if defined(_DEBUG)
	unsigned long resDebug=System::WaitForSingleObject(m_mutexDebug,WAITTIME_INIFINITE);
	EP_ASSERT_EXPR(resDebug!=WAIT_ABANDONED,_T("Obtained abandoned Debug Mutex."));
	std::vector<int>::iterator iter;
	int threadID=GetCurrentThreadId();
	for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
	{
		EP_VERIFY_THREAD_DEADLOCK_ERROR(*iter!=threadID);
	}
	ReleaseMutex(m_mutexDebug);
#endif //defined(_DEBUG)
	long ret=0;
	unsigned long mutexStatus=System::WaitForSingleObject(m_mutex,WAITTIME_IGNORE);
	if(mutexStatus== WAIT_OBJECT_0)
		ret=1;
	else if(mutexStatus==WAIT_ABANDONED)
		m_isMutexAbandoned=true;
#if defined(_DEBUG)
	if(ret)
	{
		unsigned long resDebug=System::WaitForSingleObject(m_mutexDebug,WAITTIME_INIFINITE);
		EP_ASSERT_EXPR(resDebug!=WAIT_ABANDONED,_T("Obtained abandoned Debug Mutex."));
		m_threadList.push_back(threadID);
		ReleaseMutex(m_mutexDebug);
	}
#endif //defined(_DEBUG)
	return ret;

}

long Mutex::TryLockFor(const unsigned int dwMilliSecond)
{
#if defined(_DEBUG)
	unsigned long resDebug=System::WaitForSingleObject(m_mutexDebug,WAITTIME_INIFINITE);
	EP_ASSERT_EXPR(resDebug!=WAIT_ABANDONED,_T("Obtained abandoned Debug Mutex."));
	std::vector<int>::iterator iter;
	int threadID=GetCurrentThreadId();
	for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
	{
		EP_VERIFY_THREAD_DEADLOCK_ERROR(*iter!=threadID);
	}
	ReleaseMutex(m_mutexDebug);
#endif //defined(_DEBUG)
	long ret=0;
	unsigned long mutexStatus=System::WaitForSingleObject(m_mutex,dwMilliSecond);
	if(mutexStatus==WAIT_OBJECT_0)
		ret=1;
	else if(mutexStatus==WAIT_ABANDONED)
		m_isMutexAbandoned=true;
#if _DEBUG
	if(ret)
	{
		unsigned long resDebug=System::WaitForSingleObject(m_mutexDebug,WAITTIME_INIFINITE);
		EP_ASSERT_EXPR(resDebug!=WAIT_ABANDONED,_T("Obtained abandoned Debug Mutex."));
		m_threadList.push_back(threadID);
		ReleaseMutex(m_mutexDebug);
	}
#endif //_DEBUG
	return ret;

}
void Mutex::Unlock()
{
#if _DEBUG
	unsigned long resDebug=System::WaitForSingleObject(m_mutexDebug,WAITTIME_INIFINITE);
	EP_ASSERT_EXPR(resDebug!=WAIT_ABANDONED,_T("Obtained abandoned Debug Mutex."));
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
	ReleaseMutex(m_mutexDebug);
#endif //_DEBUG
	ReleaseMutex(m_mutex);
}

bool Mutex::IsMutexAbandoned()
{
	return m_isMutexAbandoned;
}