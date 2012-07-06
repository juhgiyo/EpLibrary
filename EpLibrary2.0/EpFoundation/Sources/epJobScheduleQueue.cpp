/*! 
JobScheduleQueue for the EpLibrary
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
*/
#include "epJobScheduleQueue.h"
#include "epQuickSort.h"
using namespace epl;
JobScheduleQueue::JobScheduleQueue(LockPolicy lockPolicyType) :ThreadSafePQueue<BaseJob*, BaseJob>(lockPolicyType)
{
}

JobScheduleQueue::~JobScheduleQueue()
{
	while(!IsEmpty())
	{
		Pop();
	}
}

void JobScheduleQueue::Push(BaseJob* const &data, BaseJob::JobStatus status)
{
	data->RetainObj();
	ThreadSafePQueue::Push(data);
	if(status!=BaseJob::JOB_STATUS_NONE)
	{
		data->JobReport(status);
	}
}
void JobScheduleQueue::Pop()
{
	BaseJob* jobObj=Front();
	jobObj->ReleaseObj();
	ThreadSafePQueue::Pop();

}

bool JobScheduleQueue::Erase(BaseJob * const object)
{
	LockObj lock(m_queueLock);
	if(m_queue.empty())
		return false;
	std::vector<BaseJob*>::iterator iter;
	for(iter=m_queue.begin();iter!=m_queue.end();iter++)
	{
		if(*iter==object)
		{
			(*iter)->JobReport(BaseJob::JOB_STATUS_TIMEOUT);
			(*iter)->ReleaseObj();
			m_queue.erase(iter);
			return true;
		}
	}
	return false;
}

void JobScheduleQueue::ReportAllJob(const BaseJob::JobStatus status)
{
	std::vector<BaseJob *>::iterator iter;
	m_queueLock->Lock();
	std::vector<BaseJob *> queue=m_queue;
	m_queueLock->Unlock();
	for(iter=queue.begin();iter!=queue.end();iter++)
	{
		(*iter)->JobReport(BaseJob::JOB_STATUS_INCOMPLETE);
	}

}