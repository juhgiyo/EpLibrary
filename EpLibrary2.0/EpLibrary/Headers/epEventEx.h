/*! 
@file epEventEx.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 16, 2011
@brief EventEx Interface
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

An Interface for EventEx Class.

*/
#ifndef __EP_EVENT_EX_H__
#define __EP_EVENT_EX_H__
#include "epLib.h"
#include "epSystem.h"
#include "epBaseLock.h"

namespace epl
{

	/*! 
	@class EventEx epEventEx.h
	@brief A class that handles the event functionality.
	*/
	class EP_LIBRARY EventEx :public BaseLock
	{
	public:
		/*!
		Default Constructor

		Initializes the lock.
		@remark eventName must be supplied if the object will be used across process boundaries.
		@param[in] eventName name of the event to distinguish
		@param[in] lpsaAttributes the security attribute
		*/
		EventEx(const TCHAR *eventName=NULL, LPSECURITY_ATTRIBUTES lpsaAttributes = NULL);

		/*!
		Default Constructor

		Initializes the lock.
		@param[in] isInitialRaised flag to raise the event on creation
		@param[in] isManualReset flag to whether event is reset manually
		@param[in] eventName name of the event to distinguish
		@param[in] lpsaAttributes the security attribute
		@remark eventName must be supplied if the object will be used across process boundaries.
		@remark if isInitialRaised is TRUE then the event is raised on creation.
		*/
		EventEx(bool isInitialRaised,bool isManualReset,const TCHAR *eventName=NULL , LPSECURITY_ATTRIBUTES lpsaAttributes=NULL);

		/*!
		Default Copy Constructor

		Initializes the EventEx
		@param[in] b the second object
		*/
		EventEx(const EventEx& b);

		/*!
		Default Destructor

		Deletes the lock
		*/
		virtual ~EventEx();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		EventEx & operator=(const EventEx&b);


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
		Reset the event raised
		@return true if succeeded otherwise false
		@remark if event is not raised then no effect
		*/
		bool ResetEvent();

		/*!
		Set the event to be raised
		@return true if succeeded otherwise false
		@remark if event is already raised then no effect
		@remark this function is same as unlock
		*/
		bool SetEvent();

		/*!
		Returns the flag whether this event is resetting manually.
		@return true if the event is resetting manually, otherwise false.
		*/
		bool IsManualReset() const;

		/*!
		Wait for the event raised for given time
		@param[in] dwMilliSecond the wait time.
		@return true if the wait is succeeded, otherwise false.
		*/
		bool WaitForEvent(const unsigned int dwMilliSecond=WAITTIME_INIFINITE);


		/*!
		Get Event Handle
		@return event handle of this object
		*/
		HANDLE GetEventHandle();
	private:

		/// EventEx
		HANDLE m_event;
		/// Creation Security Info
		LPSECURITY_ATTRIBUTES m_lpsaAttributes;
		/// Flag for whether the event is raised on creation
		bool m_isInitialRaised;
		/// Flag for whether the event is resetting manually.
		bool m_isManualReset;
		/// Event Name
		EpTString m_name;

#if defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
		/// thread ID that currently holding this event
		unsigned long m_threadID;
		/// EventEx Debug
		HANDLE m_eventDebug;
#endif //defined(_DEBUG) && defined(ENABLE_POSSIBLE_DEADLOCK_CHECK)
	};

}

#endif //__EP_EVENT_EX_H__