/*! 
BaseJob for the EpLibrary
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
#include "epBaseJob.h"
#include "epSystem.h"
#include "epSingletonHolder.h"


using namespace epl;

BaseJob::BaseJob(Priority priority,LockPolicy lockPolicyType):SmartObject(lockPolicyType){
	//SingletonHolder<JobPool>::Instance().insert(this);	
	m_status=JOB_STATUS_NONE;
	m_priority=priority;
}

BaseJob::~BaseJob(){
	//SingletonHolder<JobPool>::Instance().remove(this);		
}

BaseJob::JobStatus BaseJob::GetStatus() const
{
	return m_status;
}

Priority BaseJob::GetPriority() const
{
	return m_priority;
}

void BaseJob::SetPriority(Priority newPrio)
{
	m_priority=newPrio;
}

void BaseJob::JobReport(const JobStatus status)
{
	handleReport(status);
	m_status=status;
}

CompResultType BaseJob::CompFunc(const void *a,const void *b)
{
	BaseJob **_a=(BaseJob **)a;
	BaseJob **_b=(BaseJob **)b;
	if((*_a)->GetPriority() > (*_b)->GetPriority())
		return COMP_RESULT_LESSTHAN;
	else if((*_a)->GetPriority() < (*_b)->GetPriority())
		return COMP_RESULT_GREATERTHAN;
	return COMP_RESULT_GREATERTHAN;

}
void BaseJob::handleReport(const JobStatus status)
{

}