/*! 
WorkerThreadInfinite for the EpLibrary

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
	Resume();
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
		m_jobProcessor->DoJob(this, jobPtr);
		jobPtr->JobReport(BaseJob::JOB_STATUS_DONE);
		jobPtr->ReleaseObj();
	}
}