/*! 
CriticalSectionEx for the EpLibrary
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
#include "epCriticalSectionEx.h"
#include "epSystem.h"
using namespace epl;


CriticalSectionEx::CriticalSectionEx() :BaseLock()
{
	InitializeCriticalSection(&m_criticalSection);
}

CriticalSectionEx::CriticalSectionEx(const CriticalSectionEx& b):BaseLock()
{
	InitializeCriticalSection(&m_criticalSection);
}

CriticalSectionEx::~CriticalSectionEx()
{
	DeleteCriticalSection(&m_criticalSection);
}

void CriticalSectionEx::Lock()
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
	EnterCriticalSection(&m_criticalSection);
#if _DEBUG
	m_threadList.push_back(threadID);
#endif //_DEBUG
}

long CriticalSectionEx::TryLock()
{
	long ret=TryEnterCriticalSection(&m_criticalSection);
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
long CriticalSectionEx::TryLockFor(const unsigned int dwMilliSecond)
{
	long ret=0;
	if(ret=TryEnterCriticalSection(&m_criticalSection))
	{
		return ret;	
	}
	else
	{
		unsigned int startTime,timeUsed;
		unsigned int waitTime=dwMilliSecond;
		startTime=System::GetTickCount();
		while(WaitForSingleObject(m_criticalSection.LockSemaphore,waitTime)==WAIT_OBJECT_0)
		{
			if(ret=TryEnterCriticalSection(&m_criticalSection))
			{
#if _DEBUG
				std::vector<int>::iterator iter;
				int threadID=GetCurrentThreadId();
				for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
				{
					if(*iter==threadID)
						EP_ASSERT(0);
				}
				m_threadList.push_back(threadID);
#endif //_DEBUG
				return ret;	
			}		
			timeUsed=System::GetTickCount()-startTime;
			waitTime=waitTime-(unsigned int)timeUsed;
			startTime=System::GetTickCount();
		}
		return 0;
	}
}
void CriticalSectionEx::Unlock()
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
	LeaveCriticalSection(&m_criticalSection);
}

