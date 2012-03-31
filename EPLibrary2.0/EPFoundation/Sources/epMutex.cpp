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
	m_lpsaAttributes=lpsaAttributes;
	m_mutex=EP_NEW CMutex(FALSE,mutexName,lpsaAttributes);
	m_singleLock=EP_NEW CSingleLock(m_mutex,FALSE);
}

Mutex::Mutex(const Mutex& b)
{
	m_lpsaAttributes=b.m_lpsaAttributes;
	m_mutex=EP_NEW CMutex(FALSE,NULL,m_lpsaAttributes);
	m_singleLock=EP_NEW CSingleLock(m_mutex,FALSE);
}

Mutex::~Mutex()
{
	if(m_singleLock)
		EP_DELETE m_singleLock;
	if(m_mutex)	
		EP_DELETE m_mutex;
}

void Mutex::Lock()
{

#if _DEBUG
	std::vector<int>::iterator iter;
	int threadID=GetCurrentThreadId();
	for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
	{
		EP_VERIFY_THREAD_DEADLOCK_ERROR(*iter!=threadID);
	}
#endif //_DEBUG

	m_singleLock->Lock();
}

long Mutex::TryLock()
{
	long ret=0;
	ret=(long)m_singleLock->Lock(0);
#if _DEBUG
	if(ret)
	{
		std::vector<int>::iterator iter;
		int threadID=GetCurrentThreadId();
		for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
		{
			EP_VERIFY_THREAD_DEADLOCK_ERROR(*iter!=threadID);
		}
		m_threadList.push_back(threadID);
	}
#endif //_DEBUG
	return ret;

}

long Mutex::TryLockFor(const unsigned int dwMilliSecond)
{
	long ret=0;

	ret=static_cast<long>(m_singleLock->Lock(dwMilliSecond));

#if _DEBUG
	if(ret)
	{
		std::vector<int>::iterator iter;
		int threadID=GetCurrentThreadId();
		for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
		{
			EP_VERIFY_THREAD_DEADLOCK_ERROR(*iter!=threadID);
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

	m_singleLock->Unlock();
}
