/*! 
@file epNoLock.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 16, 2011
@brief NoLock Interface
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

An Interface for NoLock Class.

*/
#ifndef __EP_NO_LOCK_H__
#define __EP_NO_LOCK_H__
#include "epLib.h"
#include "epSystem.h"
#include "epBaseLock.h"

namespace epl
{
	/*! 
	@class NoLock epNoLock.h
	@brief A class that handles the NoLock functionality.
	*/
	class EP_LIBRARY NoLock :public BaseLock
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
		NoLock & operator=(const NoLock&b);
		
		/*!
		Locks the NoLock
		@return true if locked, false otherwise
		*/
		virtual bool Lock();

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

	};

}

#endif //__EP_NO_LOCK_H__