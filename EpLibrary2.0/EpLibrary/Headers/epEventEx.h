/*! 
@file epEventEx.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 16, 2011
@brief EventEx Interface
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