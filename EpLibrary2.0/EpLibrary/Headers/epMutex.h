/*! 
@file epMutex.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 16, 2011
@brief Mutex Interface
@version 2.0

@section LICENSE

The MIT License (MIT)

Copyright (c) 2008-2013 Woong Gyu La <juhgiyo@gmail.com>

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

An Interface for Mutex Class.

*/
#ifndef __EP_MUTEX_H__
#define __EP_MUTEX_H__
#include "epLib.h"
#include "epSystem.h"
#include "epBaseLock.h"

namespace epl
{

	/*! 
	@class Mutex epMutex.h
	@brief A class that handles the mutex functionality.
	*/
	class EP_LIBRARY Mutex :public BaseLock
	{
	public:
		/*!
		Default Constructor

		Initializes the lock.
		@remark mutexName must be supplied if the object will be used across process boundaries.
		@param[in] mutexName name of the mutex to distinguish
		@param[in] lpsaAttributes the security attribute
		*/
		Mutex(const TCHAR *mutexName=NULL, LPSECURITY_ATTRIBUTES lpsaAttributes = NULL);

		/*!
		Default Constructor

		Initializes the lock.
		@param[in] isInitialOwner flag to own the mutex on creation
		@param[in] mutexName name of the mutex to distinguish
		@param[in] lpsaAttributes the security attribute
		@remark mutexName must be supplied if the object will be used across process boundaries.
		@remark if isInitialOwner is TRUE then the mutex is created as locked.
		*/
		Mutex(bool isInitialOwner,const TCHAR *mutexName=NULL , LPSECURITY_ATTRIBUTES lpsaAttributes=NULL);

		/*!
		Default Copy Constructor

		Initializes the Mutex
		@param[in] b the second object
		*/
		Mutex(const Mutex& b);

		/*!
		Default Destructor

		Deletes the lock
		*/
		virtual ~Mutex();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		Mutex & operator=(const Mutex&b);


		/*!
		Locks the Critical Section
		@return true if locked, false otherwise
		*/
		virtual bool Lock();

		/*!
		Try to Lock the Critical Section

		If other process is already in the Critical Section, it just returns false and continue, otherwise obtain the Critical Section.
		@return true if the lock is succeeded, otherwise false.
		*/
		virtual long TryLock();

		/*!
		Locks the Critical Section

		if other process is already in the Critical Section,
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
		Returns the flag whether this mutex is abandoned or now.
		@return true if the mutex is abandoned, otherwise false.
		*/
		bool IsMutexAbandoned() const;

	private:

		/// Mutex
		HANDLE m_mutex;
		/// Creation Security Info
		LPSECURITY_ATTRIBUTES m_lpsaAttributes;
		/// Flag for whether the mutex is locked on creation
		bool m_isInitialOwner;
		/// Flag for whether the mutex is abandoned or not.
		bool m_isMutexAbandoned;
		/// Mutex Name
		EpTString m_name;

#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
		/// thread ID that currently holding this mutex
		unsigned long m_threadID;
		/// Mutex Debug
		HANDLE m_mutexDebug;
#endif //defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	};

}

#endif //__EP_MUTEX_H__