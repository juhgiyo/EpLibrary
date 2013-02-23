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
	m_lpsaAttributes=NULL;
	if(lpsaAttributes)
	{
		m_lpsaAttributes=EP_NEW SECURITY_ATTRIBUTES();
		*m_lpsaAttributes=*lpsaAttributes;
	}
	m_isInitialOwner=false;
	m_name=mutexName;
	m_mutex=CreateMutex(m_lpsaAttributes,FALSE,mutexName);
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	m_mutexDebug=CreateMutex(m_lpsaAttributes,FALSE,NULL);
	m_threadID=0;
#endif //defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
}

Mutex::Mutex(bool isInitialOwner,const TCHAR *mutexName , LPSECURITY_ATTRIBUTES lpsaAttributes) :BaseLock()
{
	m_isMutexAbandoned=false;
	m_lpsaAttributes=NULL;
	if(lpsaAttributes)
	{
		m_lpsaAttributes=EP_NEW SECURITY_ATTRIBUTES();
		*m_lpsaAttributes=*lpsaAttributes;
	}
	m_isInitialOwner=isInitialOwner;
	m_name=mutexName;
	m_mutex=CreateMutex(m_lpsaAttributes,(BOOL)isInitialOwner,mutexName);
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	m_mutexDebug=CreateMutex(m_lpsaAttributes,FALSE,NULL);
	m_threadID=0;
	if(m_isInitialOwner)
		m_threadID=GetCurrentThreadId();
#endif //defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
}

Mutex::Mutex(const Mutex& b)
{
	m_isMutexAbandoned=false;
	m_lpsaAttributes=NULL;
	if(b.m_lpsaAttributes)
	{
		m_lpsaAttributes=EP_NEW SECURITY_ATTRIBUTES();
		*m_lpsaAttributes=*b.m_lpsaAttributes;
	}
	m_isInitialOwner=b.m_isInitialOwner;
	m_name=b.m_name;
	if(m_name.size())
		m_mutex=CreateMutex(m_lpsaAttributes,(BOOL)m_isInitialOwner,m_name.c_str());
	else
		m_mutex=CreateMutex(m_lpsaAttributes,(BOOL)m_isInitialOwner,NULL);
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	m_mutexDebug=CreateMutex(m_lpsaAttributes,FALSE,NULL);
	m_threadID=0;
	if(m_isInitialOwner)
		m_threadID=GetCurrentThreadId();
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
}

Mutex::~Mutex()
{

	CloseHandle(m_mutex);
	m_mutex=0;
	if(m_lpsaAttributes)
	{
		EP_DELETE m_lpsaAttributes;
	}
	m_lpsaAttributes=NULL;			
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	CloseHandle(m_mutexDebug);
	m_mutexDebug=NULL;
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
}

Mutex & Mutex::operator=(const Mutex&b)
{
	if(this != &b)
	{
		CloseHandle(m_mutex);
		m_mutex=0;
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
		m_isMutexAbandoned=false;
		m_isInitialOwner=b.m_isInitialOwner;
		m_name=b.m_name;
		if(m_name.size())
			m_mutex=CreateMutex(m_lpsaAttributes,(BOOL)m_isInitialOwner,m_name.c_str());
		else
			m_mutex=CreateMutex(m_lpsaAttributes,(BOOL)m_isInitialOwner,NULL);
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
		m_threadID=0;
		if(m_isInitialOwner)
			m_threadID=GetCurrentThreadId();
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	}
	return *this;
}
bool Mutex::Lock()
{

#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	unsigned long resDebug=System::WaitForSingleObject(m_mutexDebug,WAITTIME_INIFINITE);
	EP_ASSERT_EXPR(resDebug!=WAIT_ABANDONED,_T("Obtained abandoned Debug Mutex."));
	
	unsigned long threadID=GetCurrentThreadId();
	EP_ASSERT_EXPR(threadID!=m_threadID,_T("Possible Deadlock detected!"));
	ReleaseMutex(m_mutexDebug);
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)

	bool returnVal=true;
	unsigned long res=System::WaitForSingleObject(m_mutex,WAITTIME_INIFINITE);
	if(res==WAIT_ABANDONED)
	{
		m_isMutexAbandoned=true;
		returnVal=false;
	}

#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	resDebug=System::WaitForSingleObject(m_mutexDebug,WAITTIME_INIFINITE);
	EP_ASSERT_EXPR(resDebug!=WAIT_ABANDONED,_T("Obtained abandoned Debug Mutex."));
	m_threadID=threadID;
	ReleaseMutex(m_mutexDebug);
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	return returnVal;
}

long Mutex::TryLock()
{
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	unsigned long resDebug=System::WaitForSingleObject(m_mutexDebug,WAITTIME_INIFINITE);
	EP_ASSERT_EXPR(resDebug!=WAIT_ABANDONED,_T("Obtained abandoned Debug Mutex."));

	unsigned long threadID=GetCurrentThreadId();
	EP_ASSERT_EXPR(threadID!=m_threadID,_T("Possible Deadlock detected!"));
	ReleaseMutex(m_mutexDebug);
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	long ret=0;
	unsigned long mutexStatus=System::WaitForSingleObject(m_mutex,WAITTIME_IGNORE);
	if(mutexStatus== WAIT_OBJECT_0)
		ret=1;
	else if(mutexStatus==WAIT_ABANDONED)
		m_isMutexAbandoned=true;
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	if(ret)
	{
		unsigned long resDebug=System::WaitForSingleObject(m_mutexDebug,WAITTIME_INIFINITE);
		EP_ASSERT_EXPR(resDebug!=WAIT_ABANDONED,_T("Obtained abandoned Debug Mutex."));
		m_threadID=threadID;
		ReleaseMutex(m_mutexDebug);
	}
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	return ret;

}

long Mutex::TryLockFor(const unsigned int dwMilliSecond)
{
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	unsigned long resDebug=System::WaitForSingleObject(m_mutexDebug,WAITTIME_INIFINITE);
	EP_ASSERT_EXPR(resDebug!=WAIT_ABANDONED,_T("Obtained abandoned Debug Mutex."));
	
	unsigned long threadID=GetCurrentThreadId();
	EP_ASSERT_EXPR(threadID!=m_threadID,_T("Possible Deadlock detected!"));
	ReleaseMutex(m_mutexDebug);
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	long ret=0;
	unsigned long mutexStatus=System::WaitForSingleObject(m_mutex,dwMilliSecond);
	if(mutexStatus==WAIT_OBJECT_0)
		ret=1;
	else if(mutexStatus==WAIT_ABANDONED)
		m_isMutexAbandoned=true;
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	if(ret)
	{
		unsigned long resDebug=System::WaitForSingleObject(m_mutexDebug,WAITTIME_INIFINITE);
		EP_ASSERT_EXPR(resDebug!=WAIT_ABANDONED,_T("Obtained abandoned Debug Mutex."));
		m_threadID=threadID;
		ReleaseMutex(m_mutexDebug);
	}
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	return ret;

}
void Mutex::Unlock()
{
#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	unsigned long resDebug=System::WaitForSingleObject(m_mutexDebug,WAITTIME_INIFINITE);
	EP_ASSERT_EXPR(resDebug!=WAIT_ABANDONED,_T("Obtained abandoned Debug Mutex."));
	m_threadID=0;
	ReleaseMutex(m_mutexDebug);
#endif // defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	ReleaseMutex(m_mutex);
}

bool Mutex::IsMutexAbandoned() const
{
	return m_isMutexAbandoned;
}