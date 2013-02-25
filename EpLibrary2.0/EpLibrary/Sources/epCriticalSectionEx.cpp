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
}

CriticalSectionEx::CriticalSectionEx(const CriticalSectionEx& b):BaseLock()
{
	InitializeCriticalSection(&m_criticalSection);
	m_lockCounter=0;
}

CriticalSectionEx::~CriticalSectionEx()
{
	EP_ASSERT_EXPR(m_lockCounter==0,_T("Lock Counter is not 0!"));
	DeleteCriticalSection(&m_criticalSection);
}
CriticalSectionEx & CriticalSectionEx::operator=(const CriticalSectionEx&b)
{
	if(this!=&b)
	{
		EP_ASSERT_EXPR(m_lockCounter==0,_T("Lock Counter is not 0!"));
		DeleteCriticalSection(&m_criticalSection);

		InitializeCriticalSection(&m_criticalSection);
		m_lockCounter=0;
	}
	return *this;
}

bool CriticalSectionEx::Lock()
{
	EnterCriticalSection(&m_criticalSection);
	m_lockCounter++;
	return true;
}

long CriticalSectionEx::TryLock()
{
	long ret=TryEnterCriticalSection(&m_criticalSection);
	if(ret)
		m_lockCounter++;
	return ret;
}
long CriticalSectionEx::TryLockFor(const unsigned int dwMilliSecond)
{
	long ret=0;
	if(ret=TryEnterCriticalSection(&m_criticalSection))
	{
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
	m_lockCounter--;
	EP_ASSERT_EXPR(m_lockCounter>=0,_T("Lock Counter is less than 0!"));
	LeaveCriticalSection(&m_criticalSection);
}

