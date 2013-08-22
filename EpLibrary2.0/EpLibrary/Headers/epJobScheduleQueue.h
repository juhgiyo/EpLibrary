/*! 
@file epJobScheduleQueue.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date September 10, 2010
@brief JobScheduleQueue Interface
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

An Interface for Job Scheudle Queue.

*/
#ifndef __EP_JOB_SCHEDULE_QUEUE_H__
#define __EP_JOB_SCHEDULE_QUEUE_H__
#include "epLib.h"
#include "epThreadSafePQueue.h"
#include "epBaseJob.h"

namespace epl
{
	/*! 
	@class JobScheduleQueue epJobScheduleQueue.h
	@brief A class for Thread Safe Priority Queue.
	*/
	class EP_LIBRARY JobScheduleQueue :public ThreadSafePQueue<BaseJob*, BaseJob>
	{
	public:
		/*!
		Default Constructor

		Initializes the schedule queue
		@param[in] lockPolicyType The lock policy
		*/
		JobScheduleQueue(LockPolicy lockPolicyType=EP_LOCK_POLICY);
		/*!
		Default Copy Constructor

		Initializes the Semaphore
		@param[in] b the second object
		*/
		JobScheduleQueue(const JobScheduleQueue& b):ThreadSafePQueue<BaseJob*, BaseJob>(b)
		{
		}
		/*!
		Default Destructor

		Destroy the schedule queue
		*/
		virtual ~JobScheduleQueue();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		JobScheduleQueue & operator=(const JobScheduleQueue&b)
		{
			if(this!=&b)
			{
				ThreadSafePQueue::operator =((const ThreadSafePQueue<BaseJob*, BaseJob>&)b);
			}
			return *this;
		}
		/*!
		Insert the new item into the schedule queue.
		@param[in] data The inserting data.'
		@param[in] status the status to set for the data
		*/
		virtual void Push(BaseJob* const &data,const BaseJob::JobStatus status=BaseJob::JOB_STATUS_IN_QUEUE);

		/*!
		Remove the first item from the queue.
		*/
		virtual void Pop();

		/*!
		Erase the element with given schedule policy holder
		@param[in] object the schedule policy holder to erase
		@return true if successful, otherwise false.
		*/
		virtual bool Erase(BaseJob * const object);

		/*!
		Make Report to the all element in the queue as given status
		@param[in] status the status to give to all element in the queue
		*/
		void ReportAllJob(const BaseJob::JobStatus status);
	};
}
#endif //__EP_JOB_SCHEDULE_QUEUE_H__