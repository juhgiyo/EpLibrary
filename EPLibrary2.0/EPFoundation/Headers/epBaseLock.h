/*! 
@file epBaseLock.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date April 16, 2011
@brief A Virtual Lock Class Interface
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

An Interface for Virtual Lock Class.

*/
#ifndef __EP_BASE_LOCK_H__
#define __EP_BASE_LOCK_H__
#include "epLib.h"
#include "epSystem.h"

namespace epl
{
	/*! 
	@class BaseLock epBaseLock.h
	@brief A class that handles the virtual base lock.
	*/
	class EP_FOUNDATION BaseLock
	{
	public:
		/*!
		Default Constructor

		Initializes the lock
		*/
		BaseLock();

		/*!
		Default Destructor

		Deletes the lock
		*/
		virtual ~BaseLock();

		/*!
		Locks the Critical Section
		*/
		virtual void Lock() =0;

		/*!
		Try to Lock the Critical Section

		If other thread is already in the Critical Section, it just returns false and continue, otherwise obtain the Critical Section.
		@return true if the lock is succeeded, otherwise false.
		*/
		virtual long TryLock()=0;

		/*!
		Locks the Critical Section

		if other thread is already in the Critical Section,
		and if it fails to lock in given time, it returns false, otherwise lock and return true.
		@param[in] dwMilliSecond the wait time.
		*/
		virtual long TryLockFor(const unsigned int dwMilliSecond)=0;

		/*!
		Leave the Critical Section

		The Lock and Unlock has to be matched for each Critical Section.
		*/
		virtual void Unlock()=0;


		/*! 
		@class BaseLockObj epBaseLock.h
		@brief A class that handles the lock.
		*/
		class EP_FOUNDATION BaseLockObj
		{
		public:
			/*!
			Default Constructor

			Locks where this object instantiated.
			@param[in] lock the pointer to the lock to lock.
			*/
			BaseLockObj(BaseLock *lock);

			/*!
			Default Destructor

			Unlock when this object destroyed.
			*/
			virtual ~BaseLockObj();

		private:
			/*!
			Default Constructor

			*Cannot be Used.
			*/
			BaseLockObj();

			/*!
			Default Copy Constructor

			*Cannot be Used.
			*/
			BaseLockObj(const BaseLockObj & b){EP_ASSERT(0);m_lock=NULL;}

			/*!
			Assignment operator overloading

			*Cannot be Used.
			@param[in] b the second object
			@return the new copied object
			*/
			BaseLockObj &operator=(const BaseLockObj & b)
			{
				EP_ASSERT(0);
				return *this;
			}

			/// The pointer to the lock used.
			BaseLock *m_lock;
		};

	};

	/// type definition  for lock object
	typedef BaseLock::BaseLockObj LockObj;
}

#endif //__EP_BASE_LOCK_H__