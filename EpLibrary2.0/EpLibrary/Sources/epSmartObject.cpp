/*! 
SmartObject for the EpLibrary
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
#include "epSmartObject.h"

using namespace epl;


SmartObject & SmartObject::operator=(const SmartObject&b)
{
	if(this!=&b)
	{
		
		m_refCounterLock->Lock();
		m_refCount--;
		m_refCounterLock->Unlock();
#if defined(_DEBUG)
		LOG_THIS_MSG(_T("Copying Object : %d (Current Object Reference Count = %d)"),this, this->m_refCount);
#endif //defined(_DEBUG)
		EP_ASSERT_EXPR(m_refCount==0,_T("The Reference Count is not 0!! Reference Count : %d"),m_refCount);

		if(m_refCounterLock)
		{
			EP_DELETE m_refCounterLock;
		}
		m_refCounterLock=NULL;

		m_refCount=1;
#if defined(_DEBUG)
		LOG_THIS_MSG(_T("%s::%s(%d) Copied Object : %d (Current Reference Count = %d)"),__TFILE__,__TFUNCTION__,__LINE__,this, this->m_refCount);
#endif //defined(_DEBUG)
		m_lockPolicy=b.m_lockPolicy;
		switch(m_lockPolicy)
		{
		case LOCK_POLICY_CRITICALSECTION:
			m_refCounterLock=EP_NEW CriticalSectionEx();
			break;
		case LOCK_POLICY_MUTEX:
			m_refCounterLock=EP_NEW Mutex();
			break;
		case LOCK_POLICY_NONE:
			m_refCounterLock=EP_NEW NoLock();
			break;
		default:
			m_refCounterLock=NULL;
			break;
		}
	}
	

	return *this;
}

#if !defined(_DEBUG)
void SmartObject::RetainObj()
{
	LockObj lock(m_refCounterLock);
	m_refCount++;
}

void SmartObject::ReleaseObj()
{
	m_refCounterLock->Lock();
	m_refCount--;

	if(m_refCount==0)
	{
		m_refCount++; // this increment is dummy addition to make pair with destructor.
		m_refCounterLock->Unlock();
		EP_DELETE this;
		return;
	}
	EP_ASSERT_EXPR(m_refCount>=0, _T("Reference Count is negative Value! Reference Count : %d"),m_refCount);
	m_refCounterLock->Unlock();
}

SmartObject::SmartObject(LockPolicy lockPolicyType)
{
	m_refCount=1;
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_refCounterLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_refCounterLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_refCounterLock=EP_NEW NoLock();
		break;
	default:
		m_refCounterLock=NULL;
		break;
	}
}

SmartObject::SmartObject(const SmartObject& b)
{
	m_refCount=1;
	m_lockPolicy=b.m_lockPolicy;
	switch(m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_refCounterLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_refCounterLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_refCounterLock=EP_NEW NoLock();
		break;
	default:
		m_refCounterLock=NULL;
		break;
	}
}



SmartObject::~SmartObject()
{
	m_refCounterLock->Lock();
	m_refCount--;
	m_refCounterLock->Unlock();
	EP_ASSERT_EXPR(m_refCount==0,_T("The Reference Count is not 0!! Reference Count : %d"),m_refCount);
	
	if(m_refCounterLock)
	{
		EP_DELETE m_refCounterLock;
	}
}

#endif //!defined(_DEBUG)
