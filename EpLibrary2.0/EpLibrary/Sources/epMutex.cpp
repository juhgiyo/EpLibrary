/*! 
Mutex for the EpLibrary

The MIT License (MIT)

Copyright (c) 2012-2013 Woong Gyu La <juhgiyo@gmail.com>

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
#include "epMutex.h"
#include "epSystem.h"
#include "epException.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

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
	if(mutexName)
		m_name=mutexName;
	else
		m_name=_T("");
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
	if(mutexName)
		m_name=mutexName;
	else
		m_name=_T("");
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