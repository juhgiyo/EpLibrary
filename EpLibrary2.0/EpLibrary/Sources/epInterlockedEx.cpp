/*! 
InterlockedEx for the EpLibrary

The MIT License (MIT)

Copyright (c) 2008-2013 Woong Gyu La <juhgiyo@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "epInterlockedEx.h"
#include "epSystem.h"
#include "epException.h"
#include "epDateTimeHelper.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

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

