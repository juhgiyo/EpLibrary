/*! 
InterlockedEx for the EpLibrary
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
#include "epInterlockedEx.h"
#include "epSystem.h"
#include "epException.h"
using namespace epl;


InterlockedEx::InterlockedEx() :BaseLock()
{
	m_interLock=0;
#if defined(_DEBUG)
	m_interLockDebug=0;
#endif //defined(_DEBUG)
}
InterlockedEx::InterlockedEx(const InterlockedEx& b) :BaseLock()
{
	m_interLock=0;
#if defined(_DEBUG)
	m_interLockDebug=0;
#endif //defined(_DEBUG)
}
InterlockedEx::~InterlockedEx()
{
}

bool InterlockedEx::Lock()
{
#if _DEBUG
	while(InterlockedExchange(&m_interLockDebug, 1)	!= 0)
	{
		Sleep(0);
	}
	std::vector<int>::iterator iter;
	int threadID=GetCurrentThreadId();
	for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
	{
		EP_VERIFY_THREAD_DEADLOCK_ERROR(*iter!=threadID);
	}
	InterlockedExchange(&m_interLockDebug, 0);
#endif //_DEBUG
	while(InterlockedExchange(&m_interLock, 1)	!= 0)
	{
		Sleep(0);
	}
#if defined(_DEBUG)
	while(InterlockedExchange(&m_interLockDebug, 1)	!= 0)
	{
		Sleep(0);
	}
	m_threadList.push_back(threadID);
	InterlockedExchange(&m_interLockDebug, 0);
#endif //defined(_DEBUG)
	return true;
}

long InterlockedEx::TryLock()
{
#if defined(_DEBUG)
	while(InterlockedExchange(&m_interLockDebug, 1)	!= 0)
	{
		Sleep(0);
	}
	std::vector<int>::iterator iter;
	int threadID=GetCurrentThreadId();
	for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
	{
		EP_VERIFY_THREAD_DEADLOCK_ERROR(*iter!=threadID);
	}
	InterlockedExchange(&m_interLockDebug, 0);
#endif //defined(_DEBUG)
	long ret=0;
	if(InterlockedExchange(&m_interLock, 1)	== 0)
	{
		ret=1;
	}
	
#if defined(_DEBUG)
	if(ret)
	{
		while(InterlockedExchange(&m_interLockDebug, 1)	!= 0)
		{
			Sleep(0);
		}
		m_threadList.push_back(threadID);
		InterlockedExchange(&m_interLockDebug, 0);
	}
#endif //defined(_DEBUG)
	return ret;
}
long InterlockedEx::TryLockFor(const unsigned int dwMilliSecond)
{
#if defined(_DEBUG)
	while(InterlockedExchange(&m_interLockDebug, 1)	!= 0)
	{
		Sleep(0);
	}
	std::vector<int>::iterator iter;
	int threadID=GetCurrentThreadId();
	for(iter=m_threadList.begin();iter!=m_threadList.end();iter++)
	{
		EP_VERIFY_THREAD_DEADLOCK_ERROR(*iter!=threadID);
	}
	InterlockedExchange(&m_interLockDebug, 0);
#endif //defined(_DEBUG)
	long ret=0;
	unsigned int startTime,timeUsed;
	unsigned int waitTime=dwMilliSecond;
	startTime=System::GetTickCount();
	do
	{
		if(InterlockedExchange(&m_interLock, 1) != 0)
		{
			Sleep(1);
			timeUsed=System::GetTickCount()-startTime;
			waitTime=waitTime-timeUsed;
			startTime=System::GetTickCount();
		}
		else
		{
			ret=1;
		}
	}while(waitTime>0);
#if _DEBUG
	if(ret)
	{
		while(InterlockedExchange(&m_interLockDebug, 1)	!= 0)
		{
			Sleep(0);
		}
		m_threadList.push_back(threadID);
		InterlockedExchange(&m_interLockDebug, 0);
	}
#endif //_DEBUG
	return ret;
}
void InterlockedEx::Unlock()
{
#if _DEBUG
	while(InterlockedExchange(&m_interLockDebug, 1)	!= 0)
	{
		Sleep(0);
	}
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
	InterlockedExchange(&m_interLockDebug, 0);
#endif //_DEBUG
	InterlockedExchange(&m_interLock, 0);
}

