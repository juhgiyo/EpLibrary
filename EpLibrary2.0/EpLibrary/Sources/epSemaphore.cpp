/*! 
Semaphore for the EpLibrary
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

