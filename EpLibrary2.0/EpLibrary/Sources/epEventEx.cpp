/*! 
EventEx for the EpLibrary
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
#include "epEventEx.h"
#include "epSystem.h"
#include "epException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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