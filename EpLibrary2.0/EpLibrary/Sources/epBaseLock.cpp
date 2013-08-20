/*! 
BaseLock for the EpLibrary

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
#include "epBaseLock.h"
#include "epSystem.h"
#include "epException.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;


BaseLock::BaseLock()
{

}

BaseLock::~BaseLock()
{
}

BaseLock::BaseLockObj::BaseLockObj(BaseLock *lock)
{
	EP_ASSERT_EXPR(lock,_T("Lock is NULL!"));
	m_lock=lock;
	if(m_lock)
		m_lock->Lock();
}

BaseLock::BaseLockObj::~BaseLockObj()
{
	if(m_lock)
	{
		m_lock->Unlock();
	}
}
BaseLock::BaseLockObj &BaseLock::BaseLockObj::operator=(const BaseLockObj & b)
{
	if(this!=&b)
	{
		if(m_lock)
		{
			m_lock->Unlock();
			m_lock=NULL;
		}
		m_lock=b.m_lock;
		BaseLockObj &unSafeB=const_cast<BaseLockObj&>(b);
		unSafeB.m_lock=NULL;
	}
	return *this;
}
BaseLock::BaseLockObj::BaseLockObj()
{
	m_lock=NULL;
}

