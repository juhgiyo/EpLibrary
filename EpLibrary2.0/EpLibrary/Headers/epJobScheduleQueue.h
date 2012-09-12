/*! 
@file epJobScheduleQueue.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date September 10, 2010
@brief JobScheduleQueue Interface
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