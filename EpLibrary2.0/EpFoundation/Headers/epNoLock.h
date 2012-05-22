/*! 
@file epNoLock.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date April 16, 2011
@brief NoLock Interface
@version 2.0

@section LICENSE

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

@section DESCRIPTION

An Interface for NoLock Class.

*/
#ifndef __EP_NO_LOCK_H__
#define __EP_NO_LOCK_H__
#include "epFoundationLib.h"
#include "epSystem.h"
#include "epBaseLock.h"

#if _DEBUG
#include <vector>
#endif //_DEBUG

namespace epl
{
	/*! 
	@class NoLock epNoLock.h
	@brief A class that handles the NoLock functionality.
	*/
	class EP_FOUNDATION NoLock :public BaseLock
	{
	public:
		/*!
		Default Constructor

		Initializes the lock
		*/
		NoLock();

		/*!
		Default Copy Constructor

		Initializes the CriticalSectionEx
		@param[in] b the second object
		*/
		NoLock(const NoLock& b);

		/*!
		Default Destructor

		Deletes the lock
		*/
		virtual ~NoLock();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		NoLock & operator=(const NoLock&b)
		{
			return *this;
		}

		/*!
		Locks the NoLock
		*/
		virtual void Lock();

		/*!
		Try to Lock the NoLock

		@return true always.
		*/
		virtual long TryLock();

		/*!
		Locks the Critical Section

		@param[in] dwMilliSecond the wait time.
		@return true always.
		*/
		virtual long TryLockFor(const unsigned int dwMilliSecond);

		/*!
		Leave the Critical Section

		The Lock and Unlock has to be matched for each NoLock.
		*/
		virtual void Unlock();

	private:

		/// the actual lock member.
		CRITICAL_SECTION m_criticalSection;
#if _DEBUG
		std::vector<int> m_threadList;
#endif //_DEBUG
	};

}

#endif //__EP_NO_LOCK_H__