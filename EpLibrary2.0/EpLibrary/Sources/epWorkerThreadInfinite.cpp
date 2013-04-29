/*! 
WorkerThreadInfinite for the EpLibrary
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
#include "epWorkerThreadInfinite.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;
WorkerThreadInfinite::WorkerThreadInfinite(const ThreadLifePolicy policy):BaseWorkerThread(policy)
{
	m_terminateEvent=EventEx(false,false);
}

WorkerThreadInfinite::WorkerThreadInfinite(const WorkerThreadInfinite & b):BaseWorkerThread(b)
{
	m_terminateEvent=EventEx(false,false);
}

Thread::TerminateResult WorkerThreadInfinite::TerminateWorker(unsigned int waitTimeInMilliSec)
{
	m_terminateEvent.SetEvent();
	return TerminateAfter(waitTimeInMilliSec);

}

void WorkerThreadInfinite::execute()
{
	while(true)
	{
		if(m_terminateEvent.WaitForEvent(0))
		{
			return;
		}
		if(m_workPool.IsEmpty())
		{
			if(m_lifePolicy==THREAD_LIFE_SUSPEND_AFTER_WORK)
			{
				callCallBack();
				Suspend();
				continue;
			}
			callCallBack();
			Sleep(0);
			continue;
		}
		EP_ASSERT_EXPR(m_jobProcessor,_T("Job Processor is NULL!"));
		if(!m_jobProcessor)
			break;
		BaseJob * jobPtr=m_workPool.Front();
		jobPtr->RetainObj();
		m_workPool.Pop();
		jobPtr->JobReport(BaseJob::JOB_STATUS_IN_PROCESS);
		m_jobProcessor->DoJob(GetID(),m_lifePolicy, jobPtr);
		jobPtr->JobReport(BaseJob::JOB_STATUS_DONE);
		jobPtr->ReleaseObj();
	}
}