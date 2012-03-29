/*! 
@file epThread.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date July 07, 2010
@brief A Thread Class Frame Interface
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

A Frame Interface for Thread Class.

*/
#ifndef __EP_THREAD_H__
#define __EP_THREAD_H__

#include <process.h>
#ifdef EP_MULTIPROCESS
#include "epMutex.h"
#else //EP_MULTIPROCESS
#include "epCriticalSectionEx.h"
#endif //EP_MULTIPROCESS

namespace epl
{

	/*! 
	@class Thread epThread.h
	@brief A class that implements base thread class operation.
	*/
	class EP_FOUNDATION Thread
	{
	public:

		/*!
		Create a thread using given routine function with given parameter
		@param[in] routineFunc The function to make thread.
		@param[in] param The parameter for given function.
		@return The handle to the thread created.
		*/
		static HANDLE CreateThread(LPTHREAD_START_ROUTINE routineFunc,LPVOID param);

		/// Enumerator for Thread Operation Code
		enum ThreadOpCode{
			/// The thread is started when it is created.
			THREAD_OPCODE_CREATE_START=0,
			/// The thread is suspended when it is created.
			TRHEAD_OPCODE_CREATE_SUSPEND=CREATE_SUSPENDED
		};

		/// Enumerator for Thread Status
		enum ThreadStatus{
			/// The thread is started and running.
			THREAD_STATUS_STARTED=0,
			/// The thread is suspended.
			THREAD_STATUS_SUSPENDED,
			/// The thread is terminated.
			THREAD_STATUS_TERMINATED
		};

		// Enumerator for Thread Type
		enum ThreadType{
			/// Create the thread using _beginthreadex
			THREAD_TYPE_BEGIN_THREAD=0,
			/// Create the thead using CreateThread
			THREAD_TYPE_CREATE_THREAD
		};

		/// type definition for Thread ID.
		typedef unsigned int ThreadID;
		/// type definition for Thread Handle.
		typedef void * ThreadHandle;

		/*!
		Default Constructor

		Initializes the thread class
		*/
		Thread();

		
		/*!
		Default Destructor

		Destroy the Thread Class
		*/
		virtual ~Thread();
		
		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		Thread &operator=(const Thread & b)
		{
			return *this;
		}
		
		/*!
		Start the Thread according to parameters given.
		@param[in] arg The argument list for thread parameter.
		@param[in] opCode The operation code for creating thread.
		@param[in] threadType The type of thread creation to use.
		@param[in] stackSize The stack size for the thread.
		@return true, if succeeded, otherwise false.
		*/
		bool Start(void * arg=NULL,const ThreadOpCode opCode=THREAD_OPCODE_CREATE_START, const ThreadType threadType=THREAD_TYPE_BEGIN_THREAD, const int stackSize=0);

		/*!
		Resume the suspended thread.
		@return true, if succeeded, otherwise false.
		*/
		bool Resume();

		/*!
		Suspend the running thread.
		@return true, if succeeded, otherwise false.
		*/
		bool Suspend();

		/*!
		Terminate the running or suspended thread.
		@return true, if succeeded, otherwise false.
		*/
		bool Terminate();


		/*!
		Wait for thread to terminate
		@param[in] tMilliseconds the time-out interval, in milliseconds.
		@return the event that caused the function to return.(WaitForSingleObject)
		*/
		unsigned long WaitFor(const unsigned long tMilliseconds);

		/*!
		Wait for thread to terminate, and if not terminated, then Terminate.
		@param[in] tMilliseconds the time-out interval, in milliseconds.
		@return the event that caused the function to return.(WaitForSingleObject)
		*/
		bool TerminateAfter(const unsigned long tMilliseconds);


		/*!
		Return the Thread ID.
		@return the current thread id.
		*/
		ThreadID GetID() const
		{
			return m_threadId;
		}

		/*!
		Return the Thread Status.
		@return the current thread status.
		*/
		ThreadStatus GetStatus() const
		{
			return m_status;
		}


		/*!
		Return the thread argument list.
		@return the thread argument list.
		*/
		void * GetArg() const;

		/*!
		Set the thread argument list.
		@param[in] a The argument list for the thread.
		*/
		virtual void SetArg(void* a);

	private:
		/*!
		Default Copy Constructor

		*Cannot be Used.
		*/
		Thread(const Thread & b){EP_ASSERT(0);}


		/*!
		Terminate the thread successfully.
		*/
		void successTerminate();

		/*!
		Running the thread when thread is created.
		@return the exit code of the current thread.
		*/
		int run();

		/*!
		Entry point for the thread created with _beginthreadex
		@param[in] pthis The argument for the thread (this for current case)
		@return the exit code.
		*/
		static unsigned int __stdcall entryPoint(void* pthis);

		/*!
		Entry point for the thread created with CreateThread
		@param[in] pthis The argument for the thread (this for current case)
		@return the exit code.
		*/
		static unsigned long __stdcall entryPoint2(void* pthis);

	protected:
		/*!
		Return the Thread Handle.
		@return the current thread handle.
		*/
		ThreadHandle getHandle() const
		{
			return m_threadHandle;
		}

		/*!
		Setup the thread when it started.
		*/
		virtual void setup();

		/*!
		Actual Thread Code.
		*/
		virtual void execute();

		/// Thread ID
		ThreadID m_threadId;
		/// Thread Handle
		ThreadHandle m_threadHandle;
		/// Thread Argument List
		void * m_arg;
		/// Thread Status
		ThreadStatus m_status;
		/// Thread Type
		ThreadType m_type;
		/// Lock
		BaseLock *m_threadLock;


	};
}
#endif //__EP_THREAD_H__