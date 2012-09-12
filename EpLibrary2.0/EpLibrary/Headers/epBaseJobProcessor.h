/*! 
@file epBaseJobProcessor.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date July 14, 2010
@brief Base Job Processor Class Interface
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

An Interface for Base Job Processor.

*/
#ifndef __EP_JOB_PROCESSOR_H__
#define __EP_JOB_PROCESSOR_H__
#include "epLib.h"
#include "epBaseWorkerThread.h"
#include "epBaseJob.h"

namespace epl
{

	/*! 
	@class BaseJobProcessor epBaseJobProcessor.h
	@brief A base class for Job Processing Objects.
	*/
	class EP_LIBRARY BaseJobProcessor: public SmartObject
	{
	public:
		friend class JobProcessorScheduleQueue;

		/// Enumeration for Job Processor Status
		enum JobProcessorStatus{
			/// Job Processor never entered to the scheduler
			JOB_PROCESSOR_STATUS_NONE=0,
			/// Job Processor is in the Pending State
			JOB_PROCESSOR_STATUS_PENDING,
			/// Job Processor is in the Process
			JOB_PROCESSOR_STATUS_IN_PROCESS,
			/// Job Processor is finished working
			JOB_PROCESSOR_STATUS_DONE,
			/// Job Processor is suspended by other processor
			JOB_PROCESSOR_STATUS_SUSPENDED,
			/// Job Processor terminated due to Thread Error or Internal Error
			JOB_PROCESSOR_STATUS_INCOMPLETE,
			/// Job PRocessor TImed Out
			JOB_PROCESSOR_STATUS_TIMEOUT,
		};

		/*!
		Default Destructor
		*/
		virtual ~BaseJobProcessor();



		/*!
		Process the job given, subclasses must implement this function.
		@param[in] threadId The Thread Id which doing the job.
		@param[in] threadLifePolicy the life policy of the thread that this job assigned to.
		@param[in] data The job given to this object.
		*/
		virtual void DoJob(const BaseWorkerThread::ThreadID threadId, const BaseWorkerThread::ThreadLifePolicy threadLifePolicy,  BaseJob* const data)=0;


	protected:
		/*!
		Handles when Job Status Changed
		Subclass should overwrite this function!!
		@param[in] status The Status of the Job
		*/
		virtual void handleReport(const JobProcessorStatus status);
		
		/*!
		Default Constructor
		@param[in] lockPolicyType The lock policy
		*/
		BaseJobProcessor(LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the Semaphore
		@param[in] b the second object
		*/
		BaseJobProcessor(const BaseJobProcessor& b):SmartObject(b)
		{
			m_status=b.m_status;
		}

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		BaseJobProcessor & operator=(const BaseJobProcessor&b)
		{
			if(this!=&b)
			{
				SmartObject::operator =(b);
				m_status=b.m_status;
				
			}
			return *this;
		}
	
	private:
		/*!
		Call Back Function When Job's Status Changed.
		@param[in] status The Status of the Job
		*/
		void JobProcessorReport(const JobProcessorStatus status);


		/// current Job Processor Status
		JobProcessorStatus m_status;


	};
}
#endif //__EP_JOB_PROCESSOR_H__