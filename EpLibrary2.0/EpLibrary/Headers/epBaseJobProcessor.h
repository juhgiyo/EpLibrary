/*! 
@file epBaseJobProcessor.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date July 14, 2010
@brief Base Job Processor Class Interface
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
		@param[in] workerThread The worker thread which called the DoJob.
		@param[in] data The job given to this object.
		*/
		virtual void DoJob(BaseWorkerThread *workerThread,  BaseJob* const data)=0;


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