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
#include "epException.h"
#include "epDateTimeHelper.h"
using namespace epl;


CriticalSectionEx::CriticalSectionEx() :BaseLock()
{
	InitializeCriticalSection(&m_criticalSection);
	m_lockCounter=0;
// #if defined(_DEBUG)
// 	InitializeCriticalSection(&m_criticalSectionDebug);
// #endif //defined(_DEBUG)
}

CriticalSectionEx::CriticalSectionEx(const CriticalSectionEx& b):BaseLock()
{
	InitializeCriticalSection(&m_criticalSection);
	m_lockCounter=0;
// #if defined(_DEBUG)
// 	InitializeCriticalSection(&m_criticalSectionDebug);
// #endif //defined(_DEBUG)
}

CriticalSectionEx::~CriticalSectionEx()
{
	EP_ASSERT_EXPR(m_lockCounter==0,_T("Lock Counter is not 0!"));
	DeleteCriticalSection(&m_criticalSection);
// #if defined(_DEBUG)
// 	DeleteCriticalSection(&m_criticalSectionDebug);
// #endif //defined(_DEBUG)
}

bool CriticalSectionEx::Lock()
{
// #if defined(_DEBUG)
// 	EnterCriticalSection(&m_criticalSectionDebug);
// 	std::vector<int>::iterator iter;
// 	int threadID=GetCurrentThreadId();
// 	for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
// 	{
// 		EP_ASSERT_EXPR(*iter!=threadID,_T("Possible Deadlock detected!"));
// 	}
// 	LeaveCriticalSection(&m_criticalSectionDebug);
// #endif //defined(_DEBUG)
	EnterCriticalSection(&m_criticalSection);
	m_lockCounter++;
// #if defined(_DEBUG)
// 	EnterCriticalSection(&m_criticalSectionDebug);
// 	m_threadList.push_back(threadID);
// 	LeaveCriticalSection(&m_criticalSectionDebug);
// #endif //defined(_DEBUG)
	return true;
}

long CriticalSectionEx::TryLock()
{
// #if defined(_DEBUG)
// 	EnterCriticalSection(&m_criticalSectionDebug);
// 	std::vector<int>::iterator iter;
// 	int threadID=GetCurrentThreadId();
// 	for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
// 	{
// 		EP_ASSERT_EXPR(*iter!=threadID,_T("Possible Deadlock detected!"));
// 	}
// 	LeaveCriticalSection(&m_criticalSectionDebug);
// #endif //defined(_DEBUG)
	long ret=TryEnterCriticalSection(&m_criticalSection);
	if(ret)
		m_lockCounter++;
// #if defined(_DEBUG)
// 	if(ret)
// 	{
// 		EnterCriticalSection(&m_criticalSectionDebug);
// 		m_threadList.push_back(threadID);
// 		LeaveCriticalSection(&m_criticalSectionDebug);
// 	}
// 	
// #endif //defined(_DEBUG)
	return ret;
}
long CriticalSectionEx::TryLockFor(const unsigned int dwMilliSecond)
{
// #if defined(_DEBUG)
// 	EnterCriticalSection(&m_criticalSectionDebug);
// 	std::vector<int>::iterator iter;
// 	int threadID=GetCurrentThreadId();
// 	for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
// 	{
// 		EP_ASSERT_EXPR(*iter!=threadID,_T("Possible Deadlock detected!"));
// 	}
// 	LeaveCriticalSection(&m_criticalSectionDebug);
// #endif //defined(_DEBUG)
	long ret=0;
	if(ret=TryEnterCriticalSection(&m_criticalSection))
	{
// #if defined(_DEBUG)
// 		EnterCriticalSection(&m_criticalSectionDebug);
// 		m_threadList.push_back(threadID);
// 		LeaveCriticalSection(&m_criticalSectionDebug);
// #endif //defined(_DEBUG)
		m_lockCounter++;
		return ret;	
	}
	else
	{
		SYSTEMTIME startTime;
		__int64 timeUsed;
		__int64 waitTime=(__int64)dwMilliSecond;
		startTime=DateTimeHelper::GetCurrentDateTime();
		
		while(System::WaitForSingleObject(m_criticalSection.LockSemaphore,waitTime)==WAIT_OBJECT_0)
		{
			if(ret=TryEnterCriticalSection(&m_criticalSection))
			{
// #if defined(_DEBUG)
// 				EnterCriticalSection(&m_criticalSectionDebug);
// 				m_threadList.push_back(threadID);
// 				LeaveCriticalSection(&m_criticalSectionDebug);
// #endif //defined(_DEBUG)
				m_lockCounter++;
				return ret;	
			}		
			timeUsed=DateTimeHelper::AbsDiffInMilliSec(DateTimeHelper::GetCurrentDateTime(),startTime);
			waitTime=waitTime-timeUsed;
			startTime=DateTimeHelper::GetCurrentDateTime();
		}
		return 0;
	}
}
void CriticalSectionEx::Unlock()
{
// #if defined(_DEBUG)
// 	EnterCriticalSection(&m_criticalSectionDebug);
// 	std::vector<int>::iterator iter;
// 	int threadID=GetCurrentThreadId();
// 	for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
// 	{
// 		if(*iter==threadID)
// 		{
// 			m_threadList.erase(iter);
// 			break;
// 		}
// 	}
// 	LeaveCriticalSection(&m_criticalSectionDebug);
// #endif //defined(_DEBUG)
	m_lockCounter--;
	EP_ASSERT_EXPR(m_lockCounter>=0,_T("Lock Counter is less than 0!"));
	LeaveCriticalSection(&m_criticalSection);
}

