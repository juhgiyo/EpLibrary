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

	/// Thread Priority Enumerator
	typedef enum _threadPriority
	{
#if (_MSC_VER >=MSVC80) && (WINVER>=WINDOWS_XP) // Only for VS2005 and above and Windows XP and above
		/// Priority 1 point above the priority class.
		EP_THREAD_PRIORITY_ABOVE_NORMAL=THREAD_PRIORITY_ABOVE_NORMAL,
		/// Priority 1 point below the priority class.
		EP_THREAD_PRIORITY_BELOW_NORMAL=THREAD_PRIORITY_BELOW_NORMAL,
#endif //(_MSC_VER >=MSVC80) && (WINVER>=WINDOWS_XP)
		/// Priority 2 points above the priority class.
		EP_THREAD_PRIORITY_HIGHEST=THREAD_PRIORITY_HIGHEST,
		/// Base priority of 1 for IDLE_PRIORITY_CLASS, BELOW_NORMAL_PRIORITY_CLASS, <br/>
		///    NORMAL_PRIORITY_CLASS, ABOVE_NORMAL_PRIORITY_CLASS, or HIGH_PRIORITY_CLASS processes,<br/>
		///    and a base priority of 16 for REALTIME_PRIORITY_CLASS processes.
		EP_THREAD_PRIORITY_IDLE=THREAD_PRIORITY_IDLE,
		/// Priority 2 points below the priority class.
		EP_THREAD_PRIORITY_LOWEST=THREAD_PRIORITY_LOWEST ,
		/// Normal priority for the priority class.
		EP_THREAD_PRIORITY_NORMAL=THREAD_PRIORITY_NORMAL,
		/// Base priority of 15 for IDLE_PRIORITY_CLASS, BELOW_NORMAL_PRIORITY_CLASS,<br/>
		///    NORMAL_PRIORITY_CLASS, ABOVE_NORMAL_PRIORITY_CLASS, or HIGH_PRIORITY_CLASS processes,<br/>
		///    and a base priority of 31 for REALTIME_PRIORITY_CLASS processes.
		EP_THREAD_PRIORITY_TIME_CRITICAL=THREAD_PRIORITY_TIME_CRITICAL,
		/// Returns when GetThreadPriority fails
		EP_THREAD_PRIORITY_ERROR_RETURN=THREAD_PRIORITY_ERROR_RETURN,
#if (_MSC_VER >=MSVC90) && (WINVER>=WINDOWS_VISTA) // Only for VS2008 and above and Windows Vista and above
		/// Begin background processing mode. <br/>
		///    The system lowers the resource scheduling priorities of the thread so <br/>
		///    that it can perform background work without significantly affecting activity in the foreground. <br/>
		///    This value can be specified only if hThread is a handle to the current thread. <br/>
		///    The function fails if the thread is already in background processing mode. <br/>
		///    Windows Server 2003 and Windows XP:  This value is not supported.
		EP_THREAD_PRIORITY_MODE_BACKGROUND_BEGIN=THREAD_MODE_BACKGROUND_BEGIN,
		/// End background processing mode.<br/>
		///    The system restores the resource scheduling priorities of the thread<br/>
		///    as they were before the thread entered background processing mode.<br/>
		///    This value can be specified only if hThread is a handle to the current thread.<br/>
		///    The function fails if the thread is not in background processing mode.<br/>
		///    Windows Server 2003 and Windows XP:  This value is not supported.<br/>
		EP_THREAD_PRIORITY_MODE_BACKGROUND_END=THREAD_MODE_BACKGROUND_END,
#endif //(_MSC_VER >=MSVC90) && (WINVER>=WINDOWS_VISTA)
	}ThreadPriority;
	

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
		@param[in] priority The priority of the thread.
		@return The handle to the thread created.
		*/
		static HANDLE CreateThread(LPTHREAD_START_ROUTINE routineFunc,LPVOID param, ThreadPriority priority = EP_THREAD_PRIORITY_NORMAL);

		/*!
		Return the current Thread Priority.
		@param[in] threadHandle the thread handle to get the priority
		@return the current Thread Priority.
		@remark if the function fails, the return value is THREAD_PRIORITY_ERROR_RETURN.
		*/
		static ThreadPriority GetPriority(HANDLE threadHandle);

		/*!
		Set Priority of the thread
		@param[in] threadHandle the thread handle to set the priority
		@param[in] priority The priority of the thread
		@return true if successfully set otherwise false
		*/
		static bool SetPriority(HANDLE threadHandle, ThreadPriority priority);

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
			THREAD_TYPE_CREATE_THREAD,
			/// Unknown thread type
			THREAD_TYPE_UNKNOWN
		};

		/// Enumerator for Thread Terminate Result
		enum TerminateResult{
			/// Failed to terminate the thread
			TERMINATE_RESULT_FAILED=0,
			/// The thread terminated gracefully
			TERMINATE_RESULT_GRACEFULLY_TERMINATED,
			/// The thread terminated forcefully
			TERMINATE_RESULT_FORCEFULLY_TERMINATE,
			/// The thread was not running
			TERMINATE_RESULT_NOT_ON_RUNNING,
		};

		/// type definition for Thread ID.
		typedef unsigned int ThreadID;
		/// type definition for Thread Handle.
		typedef void * ThreadHandle;

		/*!
		Default Constructor

		Initializes the thread class
		@param[in] priority The priority of the thread.
		@param[in] lockPolicyType The lock policy
		*/
		Thread(ThreadPriority priority=EP_THREAD_PRIORITY_NORMAL,LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Constructor

		Initializes the thread class and start the thread with given function.
		@param[in] threadFunc the function for the thread
		@param[in] priority The priority of the thread.
		@param[in] lockPolicyType The lock policy
		*/
		Thread(void (__cdecl *threadFunc)(), ThreadPriority priority=EP_THREAD_PRIORITY_NORMAL,LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the Thread class
		@param[in] b the second object
		@remark This is NOT copying!!<br/>
		        This moves b to a, if m_threadFunc==dummyThreadFunc
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
		@remark This is NOT copying!!<br/>
		        This moves b to a, if m_threadFunc==dummyThreadFunc<br/>
		        otherwise, it stop the current thread and resets.
		*/
		Thread &operator=(const Thread & b);
		

		/*!
		Start the Thread according to parameters given.
		@param[in] opCode The operation code for creating thread.
		@param[in] threadType The type of thread creation to use.
		@param[in] stackSize The stack size for the thread.
		@return true, if succeeded, otherwise false.
		*/
		bool Start(const ThreadOpCode opCode=THREAD_OPCODE_CREATE_START, const ThreadType threadType=THREAD_TYPE_BEGIN_THREAD, const int stackSize=0);

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
		Join the thread
		*/
		void Join();

		 /*!
		Check if the thread class is joinable
		@return true if joinable otherwise false
		*/
		bool Joinable();

		/*!
		Detach the thread
		*/
		void Detach();

		/*!
		Wait for thread to terminate, and if not terminated, then Terminate.
		@param[in] tMilliseconds the time-out interval, in milliseconds.
		@return the event that caused the function to return.(WaitForSingleObject)
		*/
		TerminateResult TerminateAfter(const unsigned long tMilliseconds);


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
		Return the current Thread Priority.
		@return the current Thread Priority.
		@remark if the function fails, the return value is THREAD_PRIORITY_ERROR_RETURN.
		*/
		ThreadPriority GetPriority();

		/*!
		Set Priority of the thread
		@param[in] priority The priority of the thread
		@return true if successfully set otherwise false
		*/
		bool SetPriority(ThreadPriority priority);


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
		Dummy thread function
		*/
		static void dummyThreadFunc();
	
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
		Reset Thread
		*/
		void resetThread();

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
		/// ThreadPriority
		ThreadPriority m_threadPriority;
		
		/// Parent Thread ID
		ThreadID m_parentThreadId;
		/// Parent Thread Handle
		ThreadHandle m_parentThreadHandle;
		
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
		/// thread Func
		void (*m_threadFunc)();


	};
}
#endif //__EP_THREAD_H__