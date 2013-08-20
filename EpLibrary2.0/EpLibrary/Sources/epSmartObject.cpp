/*! 
SmartObject for the EpLibrary

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
#include "epSmartObject.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;


SmartObject & SmartObject::operator=(const SmartObject&b)
{
	if(this!=&b)
	{
		
		if(m_refCounterLock)
		{
			EP_DELETE m_refCounterLock;
		}
		m_refCounterLock=NULL;

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
