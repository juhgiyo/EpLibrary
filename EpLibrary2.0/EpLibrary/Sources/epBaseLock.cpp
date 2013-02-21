/*! 
BaseLock for the EpLibrary
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
#include "epBaseLock.h"
#include "epSystem.h"
#include "epException.h"
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

