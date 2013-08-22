/*! 
EventEx for the EpLibrary

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
#include "epEventEx.h"
#include "epSystem.h"
#include "epException.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;


EventEx::EventEx(const TCHAR *eventName, LPSECURITY_ATTRIBUTES lpsaAttributes) :BaseLock()
{
	m_isManualReset=false;
	m_isInitialRaised=true;
	m_lpsaAttributes=NULL;
	if(lpsaAttributes)
	{
		m_lpsaAttributes=EP_NEW SECURITY_ATTRIBUTES();
		*m_lpsaAttributes=*lpsaAttributes;
	}
	if(eventName)
		m_name=eventName;
	else
		m_name=_T("");
	m_event=CreateEvent(m_lpsaAttributes,(BOOL)m_isManualReset,(BOOL)m_isInitialRaised,eventName);
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	m_eventDebug=CreateEvent(m_lpsaAttributes,FALSE,TRUE,NULL);
	m_threadID=0;
#endif //defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
}

EventEx::EventEx(bool isInitialRaised, bool isManualReset,const TCHAR *eventName , LPSECURITY_ATTRIBUTES lpsaAttributes) :BaseLock()
{
	m_isManualReset=isManualReset;
	m_isInitialRaised=isInitialRaised;
	m_lpsaAttributes=NULL;
	if(lpsaAttributes)
	{
		m_lpsaAttributes=EP_NEW SECURITY_ATTRIBUTES();
		*m_lpsaAttributes=*lpsaAttributes;
	}

	if(eventName)
		m_name=eventName;
	else
		m_name=_T("");
	m_event=CreateEvent(m_lpsaAttributes,(BOOL)m_isManualReset,(BOOL)m_isInitialRaised,eventName);
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	m_eventDebug=CreateEvent(m_lpsaAttributes,FALSE,TRUE,NULL);
	m_threadID=0;
	if(m_isInitialRaised)
		m_threadID=GetCurrentThreadId();
#endif //defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
}

EventEx::EventEx(const EventEx& b)
{
	m_isManualReset=false;
	m_lpsaAttributes=NULL;
	if(b.m_lpsaAttributes)
	{
		m_lpsaAttributes=EP_NEW SECURITY_ATTRIBUTES();
		*m_lpsaAttributes=*b.m_lpsaAttributes;
	}
	m_isInitialRaised=b.m_isInitialRaised;
	m_name=b.m_name;
	if(m_name.size())
		m_event=CreateEvent(m_lpsaAttributes,(BOOL)m_isManualReset,(BOOL)m_isInitialRaised,m_name.c_str());
	else
		m_event=CreateEvent(m_lpsaAttributes,(BOOL)m_isManualReset,(BOOL)m_isInitialRaised,NULL);
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	m_eventDebug=CreateEvent(m_lpsaAttributes,FALSE,TRUE,NULL);
	m_threadID=0;
	if(m_isInitialRaised)
		m_threadID=GetCurrentThreadId();
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
}

EventEx::~EventEx()
{

	CloseHandle(m_event);
	m_event=NULL;
	if(m_lpsaAttributes)
	{
		EP_DELETE m_lpsaAttributes;
	}		
	m_lpsaAttributes=NULL;
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	CloseHandle(m_eventDebug);
	m_eventDebug=NULL;
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
}

EventEx & EventEx::operator=(const EventEx&b)
{
	if(this != &b)
	{
		CloseHandle(m_event);
		m_event=NULL;
		if(m_lpsaAttributes)
		{
			EP_DELETE m_lpsaAttributes;
		}		
		m_lpsaAttributes=NULL;

		if(b.m_lpsaAttributes)
		{
			m_lpsaAttributes=EP_NEW SECURITY_ATTRIBUTES();
			*m_lpsaAttributes=*b.m_lpsaAttributes;
		}
		m_isManualReset=b.m_isManualReset;
		m_isInitialRaised=b.m_isInitialRaised;
		m_name=b.m_name;
		if(m_name.size())
			m_event=CreateEvent(m_lpsaAttributes,(BOOL)m_isManualReset,(BOOL)m_isInitialRaised,m_name.c_str());
		else
			m_event=CreateEvent(m_lpsaAttributes,(BOOL)m_isManualReset,(BOOL)m_isInitialRaised,NULL);
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
		m_threadID=0;
		if(m_isInitialRaised)
			m_threadID=GetCurrentThreadId();
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	}
	return *this;
}
bool EventEx::Lock()
{

#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	System::WaitForSingleObject(m_eventDebug,WAITTIME_INIFINITE);
	unsigned long threadID=GetCurrentThreadId();
	EP_ASSERT_EXPR(threadID!=m_threadID,_T("Possible Deadlock detected!"));
	::SetEvent(m_eventDebug);
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)

	bool returnVal=true;
	unsigned long res=System::WaitForSingleObject(m_event,WAITTIME_INIFINITE);
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	System::WaitForSingleObject(m_eventDebug,WAITTIME_INIFINITE);
	m_threadID=threadID;
	::SetEvent(m_eventDebug);
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	return returnVal;
}

long EventEx::TryLock()
{
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	System::WaitForSingleObject(m_eventDebug,WAITTIME_INIFINITE);
	unsigned long threadID=GetCurrentThreadId();
	EP_ASSERT_EXPR(threadID!=m_threadID,_T("Possible Deadlock detected!"));
	::SetEvent(m_eventDebug);
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	long ret=0;
	unsigned long eventStatus=System::WaitForSingleObject(m_event,WAITTIME_IGNORE);
	if(eventStatus== WAIT_OBJECT_0)
		ret=1;
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	if(ret)
	{
		System::WaitForSingleObject(m_eventDebug,WAITTIME_INIFINITE);
		m_threadID=threadID;
		::SetEvent(m_eventDebug);
	}
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	return ret;

}

long EventEx::TryLockFor(const unsigned int dwMilliSecond)
{
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	System::WaitForSingleObject(m_eventDebug,WAITTIME_INIFINITE);
	unsigned long threadID=GetCurrentThreadId();
	EP_ASSERT_EXPR(threadID!=m_threadID,_T("Possible Deadlock detected!"));
	::SetEvent(m_eventDebug);
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	long ret=0;
	unsigned long eventStatus=System::WaitForSingleObject(m_event,dwMilliSecond);
	if(eventStatus==WAIT_OBJECT_0)
		ret=1;
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	if(ret)
	{
		System::WaitForSingleObject(m_eventDebug,WAITTIME_INIFINITE);
		m_threadID=threadID;
		::SetEvent(m_eventDebug);
	}
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	return ret;

}
void EventEx::Unlock()
{
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	System::WaitForSingleObject(m_eventDebug,WAITTIME_INIFINITE);
	m_threadID=0;
	::SetEvent(m_eventDebug);
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	::SetEvent(m_event);
}

bool EventEx::IsManualReset() const
{
	return m_isManualReset;
}

bool EventEx::ResetEvent()
{
	if(::ResetEvent(m_event))
	{
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
		System::WaitForSingleObject(m_eventDebug,WAITTIME_INIFINITE);
		m_threadID=GetCurrentThreadId();
		::SetEvent(m_eventDebug);
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
		return true;
	}
	return false;
}

bool EventEx::SetEvent()
{
	if(::SetEvent(m_event))
	{
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
		System::WaitForSingleObject(m_eventDebug,WAITTIME_INIFINITE);
		m_threadID=0;
		::SetEvent(m_eventDebug);
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
		return true;
	}
	return false;
}
bool EventEx::WaitForEvent(const unsigned int dwMilliSecond)
{	
	unsigned long eventStatus=System::WaitForSingleObject(m_event,dwMilliSecond);
	if(eventStatus==WAIT_OBJECT_0)
	{
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
		System::WaitForSingleObject(m_eventDebug,WAITTIME_INIFINITE);
		m_threadID=GetCurrentThreadId();
		::SetEvent(m_eventDebug);
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
		return true;
	}
	return false;
}

HANDLE EventEx::GetEventHandle()
{
	return m_event;
}