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
#include "epDateTimeHelper.h"
using namespace epl;


InterlockedEx::InterlockedEx() :BaseLock()
{
	m_interLock=0;
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	m_interLockDebug=0;
	m_threadID=0;
#endif //defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
}
InterlockedEx::InterlockedEx(const InterlockedEx& b) :BaseLock()
{
	m_interLock=0;
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	m_interLockDebug=0;
	m_threadID=0;
#endif //defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
}
InterlockedEx::~InterlockedEx()
{
}

InterlockedEx & InterlockedEx::operator=(const InterlockedEx&b)
{
	if(this!=&b)
	{
		m_interLock=0;
	}
	return *this;
}

bool InterlockedEx::Lock()
{
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	while(InterlockedExchange(&m_interLockDebug, 1)	!= 0)
	{
		Sleep(0);
	}
	unsigned long threadID=GetCurrentThreadId();
	EP_ASSERT_EXPR(threadID!=m_threadID,_T("Possible Deadlock detected!"));
	InterlockedExchange(&m_interLockDebug, 0);
#endif //defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	while(InterlockedExchange(&m_interLock, 1)	!= 0)
	{
		Sleep(0);
	}
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	while(InterlockedExchange(&m_interLockDebug, 1)	!= 0)
	{
		Sleep(0);
	}
	m_threadID=threadID;
	InterlockedExchange(&m_interLockDebug, 0);
#endif //defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	return true;
}

long InterlockedEx::TryLock()
{
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	while(InterlockedExchange(&m_interLockDebug, 1)	!= 0)
	{
		Sleep(0);
	}
	unsigned long threadID=GetCurrentThreadId();
	EP_ASSERT_EXPR(threadID!=m_threadID,_T("Possible Deadlock detected!"));
	InterlockedExchange(&m_interLockDebug, 0);
#endif //defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	long ret=0;
	if(InterlockedExchange(&m_interLock, 1)	== 0)
	{
		ret=1;
	}
	
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	if(ret)
	{
		while(InterlockedExchange(&m_interLockDebug, 1)	!= 0)
		{
			Sleep(0);
		}
		m_threadID=threadID;
		InterlockedExchange(&m_interLockDebug, 0);
	}
#endif //defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	return ret;
}
long InterlockedEx::TryLockFor(const unsigned int dwMilliSecond)
{
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	while(InterlockedExchange(&m_interLockDebug, 1)	!= 0)
	{
		Sleep(0);
	}
	unsigned long threadID=GetCurrentThreadId();
	EP_ASSERT_EXPR(threadID!=m_threadID,_T("Possible Deadlock detected!"));
	InterlockedExchange(&m_interLockDebug, 0);
#endif //defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	long ret=0;
	
	SYSTEMTIME startTime;
	__int64 timeUsed;
	__int64 waitTime=(__int64)dwMilliSecond;
	startTime=DateTimeHelper::GetCurrentDateTime();
	
	do
	{
		if(InterlockedExchange(&m_interLock, 1) != 0)
		{
			Sleep(0);
			timeUsed=DateTimeHelper::AbsDiffInMilliSec(DateTimeHelper::GetCurrentDateTime(),startTime);
			waitTime=waitTime-timeUsed;
			startTime=DateTimeHelper::GetCurrentDateTime();
		}
		else
		{
			ret=1;
		}
	}while(waitTime>0);
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	if(ret)
	{
		while(InterlockedExchange(&m_interLockDebug, 1)	!= 0)
		{
			Sleep(0);
		}
		m_threadID=threadID;
		InterlockedExchange(&m_interLockDebug, 0);
	}
#endif //defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	return ret;
}
void InterlockedEx::Unlock()
{
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	while(InterlockedExchange(&m_interLockDebug, 1)	!= 0)
	{
		Sleep(0);
	}
	m_threadID=0;
	InterlockedExchange(&m_interLockDebug, 0);
#endif //defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	InterlockedExchange(&m_interLock, 0);
}

