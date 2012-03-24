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
using namespace epl;


Semaphore::Semaphore(unsigned int count,TCHAR *semName, LPSECURITY_ATTRIBUTES lpsaAttributes) :BaseLock()
{
	m_sem=EP_NEW CSemaphore(count,count,semName,lpsaAttributes);
	m_singleLock=EP_NEW CSingleLock(m_sem,FALSE);
}

Semaphore::~Semaphore()
{
	EP_DELETE m_singleLock;
	EP_DELETE m_sem;
}

void Semaphore::Lock()
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
	m_singleLock->Lock();
#if _DEBUG
	m_threadList.push_back(threadID);
#endif //_DEBUG
}

long Semaphore::TryLock()
{
	long ret=(long)m_singleLock->Lock(0);
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
long Semaphore::TryLockFor(const unsigned int dwMilliSecond)
{
	long ret=(long)m_singleLock->Lock(dwMilliSecond);
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
void Semaphore::Unlock()
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

