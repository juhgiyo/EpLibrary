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

#if !defined(_DEBUG)
void SmartObject::Retain()
{
	LockObj lock(m_refCounterLock);
	m_refCount++;
}

void SmartObject::Release()
{
	LockObj lock(m_refCounterLock);
	m_refCount--;

	if(m_refCount==0)
	{
		m_refCount++; // this increment is dummy addition to make pair with destructor.
		EP_DELETE this;
		return;
	}
	if(m_refCount<0)
		EP_WASSERT(0, _T("Reference Count is negative Value!"));
}

SmartObject::SmartObject()
{
	m_refCount=1;
#ifdef EP_MULTIPROCESS
	m_refCounterLock=EP_NEW Mutex();
#else //EP_MULTIPROCESS
	m_refCounterLock=EP_NEW CriticalSectionEx();
#endif //EP_MULTIPROCESS
}

#endif //defined(_DEBUG)

SmartObject::~SmartObject()
{
	m_refCount--;
	if( m_refCount!=0)
		EP_WASSERT(0,_T("The Reference Count is not 0!\nThe Reference Count is %d !"),m_refCount);
	EP_DELETE m_refCounterLock;
}


