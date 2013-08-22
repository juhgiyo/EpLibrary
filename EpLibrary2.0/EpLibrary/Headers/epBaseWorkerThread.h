/*! 
@file epBaseWorkerThread.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date July 07, 2010
@brief Base Worker Thread Class Interface.
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

A Interface for Base Worker Thread Class.

*/
#ifndef __EP_BASE_WORKER_THREAD_H__
#define __EP_BASE_WORKER_THREAD_H__
#include "epLib.h"
#include "epJobScheduleQueue.h"
#include "epThread.h"
#include "epBaseJob.h"
#include "epWorkerThreadDelegate.h"

namespace epl
{
	class BaseJobProcessor;
	/*! 
	@class BaseWorkerThread epBaseWorkerThread.h
	@brief A class that implements Base Worker Thread Class.
	*/
	class EP_LIBRARY BaseWorkerThread :public Thread
	{
	public:
		/// Enumerator for Thread Life Policy
		enum ThreadLifePolicy
		{
			/// The thread is infinitely looping.
			THREAD_LIFE_INFINITE=0,
			/// The thread suspends if work pool is empty.
			THREAD_LIFE_SUSPEND_AFTER_WORK,
			/// The thread terminates if work pool is empty.
			THREAD_LIFE_TERMINATE_AFTER_WORK,
			/// Thread Life Policy Count
			THREAD_LIFE_COUNT,
		};

		/*!
		Default Constructor

		Initializes the thread class
		@param[in] policy the life policy of this worker thread.
		@param[in] lockPolicyType The lock policy
		*/
		BaseWorkerThread(const ThreadLifePolicy policy,LockPolicy lockPolicyType=EP_LOCK_POLICY);
		
		/*!
		Default Copy Constructor

		Initializes the Thread class
		@param[in] b the second object
		*/
		BaseWorkerThread(const BaseWorkerThread & b);

		/*!
		Default Destructor

		Destroy the Thread Class
		*/
		virtual ~BaseWorkerThread();

		/*!
		Push in the new work to the work pool.
		@param[in] work the new work to put into the work pool.
		*/
		void Push(BaseJob * const  work);

		/*!
		Pop a work from the work pool.
		*/
		void Pop();

		/*!
		Get First Job in the Job Queue.
		*/
		BaseJob * &Front();

		/*!
		Erase the given work from the work pool.
		@param[in] work the work to erase from the work pool
		@return true if successful, otherwise false.
		*/
		bool Erase(BaseJob *const work);

		
		/*!
		Return the life policy of this worker thread.
		@return the life policy of this worker thread.
		*/
		ThreadLifePolicy GetLifePolicy() const
		{
			return m_lifePolicy;
		}

		/*!
		Set call back class to call when work is done.
		@param[in] callBackClass the call back class.
		*/
		virtual void SetCallBackClass(WorkerThreadDelegate *callBackClass);

		/*!
		Get job count in work pool.
		@return the job count in work pool.
		*/
		size_t GetJobCount() const;

		/*!
		Set new Job Processor.
		@param[in] jobProcessor set new Job Processor for this thread.
		*/
		void SetJobProcessor(BaseJobProcessor* jobProcessor);

		/*!
		Get Job Processor.
		@return the Job Processor for this thread.
		*/
		BaseJobProcessor* GetJobProcessor();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		BaseWorkerThread &operator=(const BaseWorkerThread & b);

		/*!
		Wait for worker thread to terminate, and if not terminated, then Terminate.
		@param[in] waitTimeInMilliSec the time-out interval, in milliseconds.
		@return the terminate result of the thread
		*/
		virtual TerminateResult TerminateWorker(unsigned int waitTimeInMilliSec=WAITTIME_INIFINITE);

	protected:


		/*!
		Pure Worker Thread Code.
		*/
		virtual void execute()=0;

		/*!
		Call the Call Back Class if callback class is assigned.
		*/
		void callCallBack();

		/// the work list
		JobScheduleQueue m_workPool;
		/// the life policy of the thread
		ThreadLifePolicy m_lifePolicy;
		/// the call back class
		WorkerThreadDelegate *m_callBackClass;

		/// callback Lock
		BaseLock * m_callBackLock;
		/// Thread Lock Policy
		LockPolicy m_lockPolicy;
		/// Job Processor
		BaseJobProcessor* m_jobProcessor;

	};
}
#endif //__EP_BASE_WORKER_THREAD_H__