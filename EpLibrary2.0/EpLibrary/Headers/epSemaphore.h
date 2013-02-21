/*! 
@file epSemaphore.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 16, 2011
@brief Semaphore Interface
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

An Interface for Semaphore Class.

*/
#ifndef __EP_SEMAPHORE_H__
#define __EP_SEMAPHORE_H__

#include "epLib.h"
#include "epSystem.h"
#include "epBaseLock.h"


#if defined(_DEBUG)
#include <vector>
#endif //defined(_DEBUG)

namespace epl
{
	/*! 
	@class Semaphore epSemaphore.h
	@brief A class that handles the semaphore functionality.
	*/
	class EP_LIBRARY Semaphore :public BaseLock
	{
	public:
		/*!
		Default Constructor

		Initializes the lock.
		@remark semName must be supplied if the object will be used across process boundaries.
		@param[in] count lock count
		@param[in] semName name of the semaphore to distinguish
		@param[in] lpsaAttributes the security attribute
		*/
		Semaphore(unsigned int count=1,const TCHAR *semName=NULL, LPSECURITY_ATTRIBUTES lpsaAttributes = NULL);

		/*!
		Default Constructor

		Initializes the lock.
		@remark semName must be supplied if the object will be used across process boundaries.
		@param[in] count lock count
		@param[in] initialCount The initial count for the semaphore object. 
		@param[in] semName name of the semaphore to distinguish
		@param[in] lpsaAttributes the security attribute
		@remark initialCount must be greater than or equal to zero and less than or equal to lMaximumCount.<br/>
		          The state of a semaphore is signaled when its count is greater than zero and nonsignaled when it is zero.
		*/
		Semaphore(unsigned int count, unsigned int initialCount,const TCHAR *semName=NULL, LPSECURITY_ATTRIBUTES lpsaAttributes = NULL);


		/*!
		Default Copy Constructor

		Initializes the Semaphore
		@param[in] b the second object
		*/
		Semaphore(const Semaphore& b);

		/*!
		Default Destructor

		Deletes the lock
		*/
		virtual ~Semaphore();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		Semaphore & operator=(const Semaphore&b);

		/*!
		Locks the Critical Section
		@return true if locked, false otherwise
		*/
		virtual bool Lock();

		/*!
		Try to Lock the Critical Section

		If other thread is already in the Critical Section, it just returns false and continue, otherwise obtain the Critical Section.
		@return true if the lock is succeeded, otherwise false.
		*/
		virtual long TryLock();

		/*!
		Locks the Critical Section

		if other thread is already in the Critical Section,
		and if it fails to lock in given time, it returns false, otherwise lock and return true.
		@param[in] dwMilliSecond the wait time.
		@return true if the lock is succeeded, otherwise false.
		*/
		virtual long TryLockFor(const unsigned int dwMilliSecond);

		/*!
		Leave the Critical Section

		The Lock and Unlock has to be matched for each Critical Section.
		*/
		virtual void Unlock();

		/*!
		Release the semaphore with given count
		@param[in] releaseCount the count of the semaphore to release
		@param[out] retPreviousCount the count of the semaphore before the release
		@return nonzero if successful otherwise 0
		@remark Use with care since by calling this function, the debugger might indicate as deadlock even though it is not.
		*/
		long Release(long releaseCount, long * retPreviousCount=NULL);

	private:
		/// Actual Semaphore		
		HANDLE m_sem;
		/// Creation Info
		LPSECURITY_ATTRIBUTES m_lpsaAttributes;
		/// Semaphore Flag
		unsigned int m_count;
		/// Semaphore Initial Count
		unsigned int m_initialiCount;
		/// Semaphore Name
		EpTString m_name;
	};

}

#endif //__EP_SEMAPHORE_H__