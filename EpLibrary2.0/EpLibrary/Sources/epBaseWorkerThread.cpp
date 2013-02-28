/*! 
BaseWorkerThread for the EpLibrary
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
int BaseWorkerThread::GetJobCount() const
{
	return m_workPool.Size();
}
void BaseWorkerThread::setJobProcessor(BaseJobProcessor* jobProcessor)
{
	if(m_jobProcessor)
		m_jobProcessor->ReleaseObj();
	m_jobProcessor=jobProcessor;
	if(m_jobProcessor)
		m_jobProcessor->RetainObj();
}

BaseJobProcessor* BaseWorkerThread::getJobProcessor()
{
	return m_jobProcessor;
}
void BaseWorkerThread::SetCallBackClass(void *callBackClass)
{
	LockObj lock(m_callBackLock);
	m_callBackClass=callBackClass;
}

void BaseWorkerThread::callCallBack()
{
	LockObj lock(m_callBackLock);
	if(m_callBackClass)	
		(reinterpret_cast<WorkerThreadDelegate*>(m_callBackClass))->CallBackFunc(this);
	m_callBackClass=NULL;
}