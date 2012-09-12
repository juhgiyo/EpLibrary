/*! 
@file epThread.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date July 07, 2010
@brief Thread Class Frame Interface
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
#include "epLib.h"
#include <process.h>
#include "epCriticalSectionEx.h"
#include "epMutex.h"
#include "epNoLock.h"

namespace epl
{

	/*! 
	@class Thread epThread.h
	@brief A class that implements base thread class operation.
	*/
	class EP_LIBRARY Thread
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
		@param[in] lockPolicyType The lock policy
		*/
		Thread(LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the Thread class
		@param[in] b the second object
		*/
		Thread(const Thread & b);

		
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
			if(this!=&b)
			{
			}
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
		@remark this function should be use with care! Memory leak might occur!
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
		Return the parent's Thread Handle.
		@return the parent's Thread Handle.
		*/
		ThreadHandle GetParentThreadHandle() const
		{
			return m_parentThreadHandle;
		}

		/*!
		Return the parent's Thread ID
		@return the parent's Thread ID
		*/
		ThreadID GetParentThreadID() const
		{
			return m_parentThreadId;
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
		Return the Thread Exit Code.
		@return the thread exit code.
		@remark 0 means successful termination, 1 means unsafe termination.
		*/
		unsigned long GetExitCode() const
		{
			return m_exitCode;
		}


		/*!
		Return the thread argument list.
		@return the thread argument list.
		*/
		void * GetArg() const;

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
		Set the thread argument input.
		@param[in] a The argument input for the thread.
		@remark Subclass should override this function to set member variables when thread starts. (Optional)
		*/
		virtual void setArg(void* a);

		/*!
		Actual Thread Code.
		@remark Subclass should override this function for executing the thread function.
		*/
		virtual void execute();

		/*!
		Calls when the thread terminated.
		@param[in] exitCode the exit code of the thread
		@param[in] isInDeletion the flag whether the thread class is in deletion or not
		@remark Subclass should override this function for clean up such as deallocation.
		@remark exitCode = 0 means successful termination, 1 means unsafe termination.
		@remark if isInDeletion is true, then onTerminated is called within destructor!
		*/
		virtual void onTerminated(unsigned long exitCode,bool isInDeletion=false);

	private:
		/*!
		Set argument when thread start and call setArg function with given argument
		@param[in] a the argument input
		*/
		void setDefaultArgument(void *a);
		

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



		/// Thread ID
		ThreadID m_threadId;
		/// Thread Handle
		ThreadHandle m_threadHandle;
		
		/// Parent Thread ID
		ThreadID m_parentThreadId;
		/// Parent Thread Handle
		ThreadHandle m_parentThreadHandle;
		
		/// Thread Argument List
		void * m_arg;
		/// Thread Status
		ThreadStatus m_status;
		/// Thread Type
		ThreadType m_type;
		/// Lock
		BaseLock *m_threadLock;
		/// Thread Lock Policy
		LockPolicy m_lockPolicy;
		/// exit code
		unsigned long m_exitCode;


	};
}
#endif //__EP_THREAD_H__