/*! 
@file epBaseJob.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date August 27, 2010
@brief Base Job Class Interface
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

An Interface for Base Job.

*/
#ifndef __EP_BASE_JOB_H__
#define __EP_BASE_JOB_H__
#include "epLib.h"
#include "epThreadSafePQueue.h"
#include "epSmartObject.h"

namespace epl
{
	///Priority Type
	typedef int Priority;

	/// Normal Priority is 0
	#define PRIORITY_NORMAL 0

	/*! 
	@class BaseJob epBaseJob.h
	@brief A base class for Job Objects.
	*/
	class EP_LIBRARY BaseJob: public SmartObject
	{
	public:
		friend class BaseWorkerThread;
		friend class WorkerThreadInfinite;
		friend class WorkerThreadSingle;
		friend class ThreadSafePQueue<BaseJob*,BaseJob>;
		friend class JobScheduleQueue;

		/// Enumeration for Job Status
		enum JobStatus{
			/// Job never entered to Job Processor
			JOB_STATUS_NONE=0,
			/// Job is in the Job Processor's Queue
			JOB_STATUS_IN_QUEUE,
			/// Job is in the Process
			JOB_STATUS_IN_PROCESS,
			/// Job is finished by Job Processor
			JOB_STATUS_DONE,
			/// Job is incomplete due to Thread Error or Job Processor Error
			JOB_STATUS_INCOMPLETE,
			/// Job Processor is Timed Out
			JOB_STATUS_JOB_PROCESSOR_TIMEOUT,
			/// Job is Timed out
			JOB_STATUS_TIMEOUT,
			/// Job Processor and Job is in Pending State
			JOB_STATUS_PENDING,
		};

		/*!
		Default Destructor
		*/
		virtual ~BaseJob();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		BaseJob & operator=(const BaseJob&b)
		{
			if(this!=&b)
			{
				SmartObject::operator =(b);
				m_status=b.m_status;
				m_priority=m_priority;
				
			}
			return *this;
		}

		/*!
		Return the current Job Status.
		@return the current Job Status
		*/
		JobStatus GetStatus() const;

		/*!
		Return the priority of this job
		@return the priority of this Job
		*/
		Priority GetPriority() const;

		/*!
		Set the priority of this job
		@param[in] newPrio new priority of this job
		*/
		void SetPriority(Priority newPrio);

		
	protected:
		/*!
		Default Constructor
		@param[in] priority the priority of the job
		@param[in] lockPolicyType The lock policy
		*/
		BaseJob(Priority priority=PRIORITY_NORMAL,LockPolicy lockPolicyType=EP_LOCK_POLICY);

		
		/*!
		Default Copy Constructor

		Initializes the Semaphore
		@param[in] b the second object
		*/
		BaseJob(const BaseJob& b):SmartObject(b)
		{
			m_status=b.m_status;
			m_priority=m_priority;
		}

		/*!
		Handles when Job Status Changed
		Subclass should overwrite this function!!
		@param[in] status The Status of the Job
		*/
		virtual void handleReport(const JobStatus status);

	private:
		/*!
		Call Back Function When Job's Status Changed.
		@param[in] status The Status of the Job
		*/
		void JobReport(const JobStatus status);

		/*!
		Compares Job object a with Job object b
		@param[in] a the pointer to the Job object.
		@param[in] b the pointer to another Job object.
		@return the Result of Comparison
		*/
		static CompResultType CompFunc(const void *a,const void *b);


		/// current Job Status
		JobStatus m_status;

		/// priority of the Job
		Priority m_priority;


	};
}
#endif //__EP_BASE_JOB_H__