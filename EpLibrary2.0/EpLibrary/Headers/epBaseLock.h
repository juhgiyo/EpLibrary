/*! 
@file epBaseLock.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 16, 2011
@brief Virtual Lock Class Interface
@version 2.0

@section LICENSE

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
	class EP_LIBRARY BaseLock
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
		@return true if locked, false otherwise
		*/
		virtual bool Lock() =0;

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
		@return true if the lock is succeeded, otherwise false.
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
		class EP_LIBRARY BaseLockObj
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
			
			/*!
			Assignment operator overloading
			@param[in] b the second object
			@return the new copied object
			@remark this is NOT copying!! This moves b to a.
			*/
			BaseLockObj &operator=(const BaseLockObj & b);
			
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

			/// The pointer to the lock used.
			BaseLock *m_lock;
		};

	};

	/// type definition  for lock object
	typedef BaseLock::BaseLockObj LockObj;
}

#endif //__EP_BASE_LOCK_H__