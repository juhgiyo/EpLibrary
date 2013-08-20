/*! 
Semaphore for the EpLibrary

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
#include "epSemaphore.h"
#include "epSystem.h"
#include "epException.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;


Semaphore::Semaphore(long count,const TCHAR *semName, LPSECURITY_ATTRIBUTES lpsaAttributes) :BaseLock()
{
	m_lpsaAttributes=NULL;
	if(lpsaAttributes)
	{
		m_lpsaAttributes=EP_NEW SECURITY_ATTRIBUTES();
		*m_lpsaAttributes=*lpsaAttributes;
	}
	m_count=count;
	m_initialiCount=count;
	if(semName)
		m_name=semName;
	else
		m_name=_T("");
	m_sem=CreateSemaphore(m_lpsaAttributes,m_initialiCount,count,semName);
}

Semaphore::Semaphore(long count, long initialCount,const TCHAR *semName, LPSECURITY_ATTRIBUTES lpsaAttributes) :BaseLock()
{
	m_lpsaAttributes=NULL;
	if(lpsaAttributes)
	{
		m_lpsaAttributes=EP_NEW SECURITY_ATTRIBUTES();
		*m_lpsaAttributes=*lpsaAttributes;
	}
	m_count=count;
	m_initialiCount=initialCount;
	if(semName)
		m_name=semName;
	else
		m_name=_T("");
	m_sem=CreateSemaphore(m_lpsaAttributes,initialCount,count,semName);
}

Semaphore::Semaphore(const Semaphore& b) :BaseLock()
{
	m_lpsaAttributes=NULL;
	if(b.m_lpsaAttributes)
	{
		m_lpsaAttributes=EP_NEW SECURITY_ATTRIBUTES();
		*m_lpsaAttributes=*b.m_lpsaAttributes;
	}
	m_initialiCount=b.m_initialiCount;
	m_count=b.m_count;
	m_name=b.m_name;
	if(m_name.size())
		m_sem=CreateSemaphore(m_lpsaAttributes,m_initialiCount,m_count,m_name.c_str());
	else
		m_sem=CreateSemaphore(m_lpsaAttributes,m_initialiCount,m_count,NULL);
}

Semaphore & Semaphore::operator=(const Semaphore&b)
{
	if(this!=&b)
	{
		CloseHandle(m_sem);
		m_sem=NULL;
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
		m_initialiCount=b.m_initialiCount;
		m_count=b.m_count;
		m_name=b.m_name;
		if(m_name.size())
			m_sem=CreateSemaphore(m_lpsaAttributes,m_initialiCount,m_count,m_name.c_str());
		else
			m_sem=CreateSemaphore(m_lpsaAttributes,m_initialiCount,m_count,NULL);
	}
	return *this;
}

Semaphore::~Semaphore()
{
	CloseHandle(m_sem);
	m_sem=NULL;
	if(m_lpsaAttributes)
	{
		EP_DELETE m_lpsaAttributes;
	}
	m_lpsaAttributes=NULL;
}

bool Semaphore::Lock()
{
	System::WaitForSingleObject(m_sem,WAITTIME_INIFINITE);
	return true;
}

long Semaphore::TryLock()
{
	long ret=0;
	if(System::WaitForSingleObject(m_sem,WAITTIME_IGNORE) == WAIT_OBJECT_0 )
		ret=1;
	return ret;
}
long Semaphore::TryLockFor(const unsigned int dwMilliSecond)
{
	long ret=0;
	if( System::WaitForSingleObject(m_sem,dwMilliSecond) == WAIT_OBJECT_0)
		ret=1;
	return ret;
}
void Semaphore::Unlock()
{
	ReleaseSemaphore(m_sem,1,NULL);
}

long Semaphore::Release(long count, long * retPreviousCount)
{
	return ReleaseSemaphore(m_sem,count,retPreviousCount);
}

