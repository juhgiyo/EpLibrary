/*! 
BaseWorkerThread for the EpLibrary

The MIT License (MIT)

Copyright (c) 2012-2013 Woong Gyu La <juhgiyo@gmail.com>

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
#include "epBaseWorkerThread.h"
#include "epSmartObject.h"
#include "epWorkerThreadDelegate.h"
#include "epBaseJobProcessor.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;

BaseWorkerThread::BaseWorkerThread(const ThreadLifePolicy policy,LockPolicy lockPolicyType) :Thread(EP_THREAD_PRIORITY_NORMAL,lockPolicyType)
{
	m_lifePolicy=policy;
	m_callBackClass=NULL;
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_callBackLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_callBackLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_callBackLock=EP_NEW NoLock();
		break;
	default:
		m_callBackLock=NULL;
		break;
	}
}

BaseWorkerThread::BaseWorkerThread(const BaseWorkerThread & b):Thread(b)
{
	m_lockPolicy=b.m_lockPolicy;
	switch(m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_callBackLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_callBackLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_callBackLock=EP_NEW NoLock();
		break;
	default:
		m_callBackLock=NULL;
		break;
	}
	
	LockObj lock(b.m_callBackLock);
	m_lifePolicy=b.m_lifePolicy;
	m_callBackClass=b.m_callBackClass;
	m_jobProcessor=b.m_jobProcessor;
	if(m_jobProcessor)
		m_jobProcessor->RetainObj();
	m_workPool=b.m_workPool;
	
}
BaseWorkerThread::~BaseWorkerThread()
{
	if(m_jobProcessor)
	{
		m_jobProcessor->ReleaseObj();
	}
	while(!m_workPool.IsEmpty())
	{
		m_workPool.Front()->JobReport(BaseJob::JOB_STATUS_INCOMPLETE);
		m_workPool.Pop();
	}
	if(m_callBackLock)
		EP_DELETE m_callBackLock;
}

BaseWorkerThread &BaseWorkerThread::operator=(const BaseWorkerThread & b)
{
	if(this!=&b)
	{
		if(m_jobProcessor)
		{
			m_jobProcessor->ReleaseObj();
		}
		m_jobProcessor=NULL;
		while(!m_workPool.IsEmpty())
		{
			m_workPool.Front()->JobReport(BaseJob::JOB_STATUS_INCOMPLETE);
			m_workPool.Pop();
		}
		if(m_callBackLock)
			EP_DELETE m_callBackLock;
		m_callBackLock=NULL;

		Thread::operator =(b);

		m_lockPolicy=b.m_lockPolicy;
		switch(m_lockPolicy)
		{
		case LOCK_POLICY_CRITICALSECTION:
			m_callBackLock=EP_NEW CriticalSectionEx();
			break;
		case LOCK_POLICY_MUTEX:
			m_callBackLock=EP_NEW Mutex();
			break;
		case LOCK_POLICY_NONE:
			m_callBackLock=EP_NEW NoLock();
			break;
		default:
			m_callBackLock=NULL;
			break;
		}

		LockObj lock(b.m_callBackLock);
		m_lifePolicy=b.m_lifePolicy;
		m_callBackClass=b.m_callBackClass;
		m_jobProcessor=b.m_jobProcessor;
		if(m_jobProcessor)
			m_jobProcessor->RetainObj();
		m_workPool=b.m_workPool;


	}
	return *this;
}
void BaseWorkerThread::Push(BaseJob* const  work)
{
	m_workPool.Push(work);
	if(m_lifePolicy==THREAD_LIFE_SUSPEND_AFTER_WORK)
		Resume();
}

BaseJob * &BaseWorkerThread::Front()
{
	return m_workPool.Front();
}
void BaseWorkerThread::Pop()
{
	m_workPool.Pop();
}

bool BaseWorkerThread::Erase(BaseJob *const work)
{
	bool retVal=false;
	if(m_workPool.Erase(work))
	{
		work->ReleaseObj();
		retVal=true;
	}
	return retVal;
}
size_t BaseWorkerThread::GetJobCount() const
{
	return m_workPool.Size();
}
void BaseWorkerThread::SetJobProcessor(BaseJobProcessor* jobProcessor)
{
	if(m_jobProcessor)
		m_jobProcessor->ReleaseObj();
	m_jobProcessor=jobProcessor;
	if(m_jobProcessor)
		m_jobProcessor->RetainObj();
}

BaseJobProcessor* BaseWorkerThread::GetJobProcessor()
{
	return m_jobProcessor;
}
void BaseWorkerThread::SetCallBackClass(WorkerThreadDelegate *callBackClass)
{
	LockObj lock(m_callBackLock);
	m_callBackClass=callBackClass;
}

Thread::TerminateResult BaseWorkerThread::TerminateWorker(unsigned int waitTimeInMilliSec)
{
	return TerminateAfter(waitTimeInMilliSec);

}

void BaseWorkerThread::callCallBack()
{
	LockObj lock(m_callBackLock);
	if(m_callBackClass)	
		m_callBackClass->CallBackFunc(this);
	m_callBackClass=NULL;
}