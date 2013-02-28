/*! 
Thread for the EpLibrary
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
#include "epThread.h"
#include "epException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace epl;

HANDLE Thread::CreateThread(LPTHREAD_START_ROUTINE routineFunc,LPVOID param, ThreadPriority priority)
{
	unsigned long threadID=0;
	HANDLE retHandle=0;
	retHandle=::CreateThread(NULL, 0,routineFunc,param,0,&threadID);
	Thread::SetPriority(retHandle,priority);
	return retHandle;
}

bool Thread::SetPriority(HANDLE threadHandle, ThreadPriority priority)
{
	bool ret=false;
	if(threadHandle!=0)
	{
		ret=(bool)::SetThreadPriority(threadHandle,(int)priority);
	}
	return ret;

}
ThreadPriority Thread::GetPriority(HANDLE threadHandle)
{
	ThreadPriority ret=EP_THREAD_PRIORITY_ERROR_RETURN;
	if(threadHandle!=0)
		ret=(ThreadPriority)::GetThreadPriority(threadHandle);
	return ret;

}

void Thread::dummyThreadFunc()
{

}

Thread::Thread(ThreadPriority priority,LockPolicy lockPolicyType)
{
	m_threadId=0;
	m_threadHandle=0;
	m_threadPriority=priority;
	m_parentThreadHandle=0;
	m_parentThreadId=0;
	m_type=THREAD_TYPE_UNKNOWN;
	m_status=THREAD_STATUS_TERMINATED;
	m_exitCode=0;
	m_lockPolicy=lockPolicyType;
	m_threadFunc=dummyThreadFunc;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_threadLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_threadLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_threadLock=EP_NEW NoLock();
		break;
	default:
		m_threadLock=NULL;
		break;
	}
}

Thread::Thread(void (__cdecl *threadFunc)(), ThreadPriority priority,LockPolicy lockPolicyType)
{
	EP_ASSERT(threadFunc);
	m_threadId=0;
	m_threadHandle=0;
	m_threadPriority=priority;
	m_parentThreadHandle=0;
	m_parentThreadId=0;
	m_status=THREAD_STATUS_TERMINATED;
	m_exitCode=0;
	m_lockPolicy=lockPolicyType;
	m_threadFunc=threadFunc;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_threadLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_threadLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_threadLock=EP_NEW NoLock();
		break;
	default:
		m_threadLock=NULL;
		break;
	}

	m_type=THREAD_TYPE_BEGIN_THREAD;
	m_parentThreadHandle=GetCurrentThread();
	m_parentThreadId=GetCurrentThreadId();
	m_threadHandle=reinterpret_cast<ThreadHandle>(_beginthreadex(NULL,0,Thread::entryPoint,this,THREAD_OPCODE_CREATE_START,&m_threadId));
	if(!m_threadHandle)
	{
		m_type=THREAD_TYPE_UNKNOWN;
		m_parentThreadId=0;
		m_parentThreadHandle=0;
		EpTString lastErrMsg;
		unsigned long lastErrNum=0;
		System::FormatLastErrorMessage(lastErrMsg,&lastErrNum);
		EP_ASSERT_EXPR(0,_T("Cannot create the thread!\r\nError Code: %d\r\nError Message: %s"),lastErrNum,lastErrMsg);
		return;

	}
	m_status=THREAD_STATUS_STARTED;
	SetPriority(priority);
}
Thread::Thread(const Thread & b)
{
	m_threadFunc=b.m_threadFunc;
	if(m_threadFunc!=dummyThreadFunc)
	{
		m_lockPolicy=b.m_lockPolicy;
		m_threadLock=b.m_threadLock;
		m_type=b.m_type;
		m_parentThreadHandle=b.m_parentThreadHandle;
		m_parentThreadId=b.m_parentThreadId;
		m_threadHandle=b.m_threadHandle;
		m_threadPriority=b.m_threadPriority;
		m_threadId=b.m_threadId;
		m_status=b.m_status;
		m_exitCode=b.m_exitCode;

		Thread &unSafeB=const_cast<Thread&>(b);
		unSafeB.m_type=THREAD_TYPE_UNKNOWN;
		unSafeB.m_parentThreadHandle=0;
		unSafeB.m_parentThreadId=0;
		unSafeB.m_threadHandle=0;
		unSafeB.m_threadId=0;
		unSafeB.m_status=THREAD_STATUS_TERMINATED;
		unSafeB.m_exitCode=0;

		unSafeB.m_threadLock=NULL;
	}
	else
	{
		m_threadId=0;
		m_threadHandle=0;
		m_threadPriority=b.m_threadPriority;
		m_parentThreadHandle=0;
		m_parentThreadId=0;
		m_type=THREAD_TYPE_UNKNOWN;
		m_exitCode=0;

		m_status=THREAD_STATUS_TERMINATED;
		m_lockPolicy=b.m_lockPolicy;
		switch(m_lockPolicy)
		{
		case LOCK_POLICY_CRITICALSECTION:
			m_threadLock=EP_NEW CriticalSectionEx();
			break;
		case LOCK_POLICY_MUTEX:
			m_threadLock=EP_NEW Mutex();
			break;
		case LOCK_POLICY_NONE:
			m_threadLock=EP_NEW NoLock();
			break;
		default:
			m_threadLock=NULL;
			break;
		}
	}


	
}
Thread::~Thread()
{
	resetThread();
}

Thread &Thread::operator=(const Thread & b)
{
	if(this!=&b)
	{
		m_threadFunc=b.m_threadFunc;
		if(m_threadFunc!=dummyThreadFunc)
		{
			resetThread();
			m_lockPolicy=b.m_lockPolicy;
			m_threadLock=b.m_threadLock;
			m_type=b.m_type;
			m_parentThreadHandle=b.m_parentThreadHandle;
			m_parentThreadId=b.m_parentThreadId;
			m_threadHandle=b.m_threadHandle;
			m_threadPriority=b.m_threadPriority;
			m_threadId=b.m_threadId;
			m_status=b.m_status;
			m_exitCode=b.m_exitCode;

			Thread &unSafeB=const_cast<Thread&>(b);
			unSafeB.m_type=THREAD_TYPE_UNKNOWN;
			unSafeB.m_parentThreadHandle=0;
			unSafeB.m_parentThreadId=0;
			unSafeB.m_threadHandle=0;
			unSafeB.m_threadId=0;
			unSafeB.m_status=THREAD_STATUS_TERMINATED;
			unSafeB.m_exitCode=0;

			unSafeB.m_threadLock=NULL;
		}
		else
		{
			resetThread();
			m_threadId=0;
			m_threadHandle=0;
			m_threadPriority=b.m_threadPriority;
			m_parentThreadHandle=0;
			m_parentThreadId=0;
			m_type=THREAD_TYPE_UNKNOWN;
			m_exitCode=0;

			m_status=THREAD_STATUS_TERMINATED;
			m_lockPolicy=b.m_lockPolicy;
			switch(m_lockPolicy)
			{
			case LOCK_POLICY_CRITICALSECTION:
				m_threadLock=EP_NEW CriticalSectionEx();
				break;
			case LOCK_POLICY_MUTEX:
				m_threadLock=EP_NEW Mutex();
				break;
			case LOCK_POLICY_NONE:
				m_threadLock=EP_NEW NoLock();
				break;
			default:
				m_threadLock=NULL;
				break;
			}
		}
	}
	return *this;
}

void Thread::resetThread()
{
	if(m_status!=THREAD_STATUS_TERMINATED)
	{
		m_exitCode=1;
		System::TerminateThread(m_threadHandle,m_exitCode);

		onTerminated(m_exitCode,true);
	}

	if(m_threadLock)
	{
		EP_DELETE m_threadLock;
	}
	m_threadLock=NULL;

	m_threadId=0;
	m_threadHandle=0;
	m_parentThreadHandle=0;
	m_parentThreadId=0;
	m_type=THREAD_TYPE_UNKNOWN;
	m_exitCode=0;

	m_status=THREAD_STATUS_TERMINATED;
}

bool Thread::Start(const ThreadOpCode opCode, const ThreadType threadType, const int stackSize)
{
	LockObj lock(m_threadLock);
	if(m_status==THREAD_STATUS_TERMINATED && !m_threadHandle)
	{
		m_type=threadType;
		m_parentThreadHandle=GetCurrentThread();
		m_parentThreadId=GetCurrentThreadId();
		if(m_type==THREAD_TYPE_BEGIN_THREAD)
			m_threadHandle=reinterpret_cast<ThreadHandle>(_beginthreadex(NULL,stackSize,Thread::entryPoint,this,opCode,&m_threadId));
		else
			m_threadHandle=::CreateThread(NULL,stackSize,Thread::entryPoint2,this,opCode,reinterpret_cast<LPDWORD>(&m_threadId));
		if(!m_threadHandle)
		{
			m_type=THREAD_TYPE_UNKNOWN;
			m_parentThreadId=0;
			m_parentThreadHandle=0;
			EpTString lastErrMsg;
			unsigned long lastErrNum=0;
			System::FormatLastErrorMessage(lastErrMsg,&lastErrNum);
			EP_ASSERT_EXPR(0,_T("Cannot create the thread!\r\nError Code: %d\r\nError Message: %s"),lastErrNum,lastErrMsg);
			return false;

		}
		else if(opCode==THREAD_OPCODE_CREATE_START)
		{
			m_status=THREAD_STATUS_STARTED;
		}
		else
		{
			m_status=THREAD_STATUS_SUSPENDED;
		}
		SetPriority(m_threadPriority);
		return true;
	}
	else
	{
		// Error Thread already exists
		System::OutputDebugString(_T("The thread (%x): Thread already exists!\r\n"),m_threadId);
	}
	return false;

}


bool Thread::Resume()
{
	LockObj lock(m_threadLock);
	if(m_status==THREAD_STATUS_SUSPENDED && m_threadHandle)	
	{
		ResumeThread(m_threadHandle);
		m_status=THREAD_STATUS_STARTED;
		return true;
	}
	else
	{
		// Thread Not in Suspended State
		System::OutputDebugString(_T("The thread (%x): Thread must be in suspended state in order to resume!\r\n"),m_threadId);

	}
	return false;
}


bool Thread::Suspend()
{
	m_threadLock->Lock();
	if(m_status==THREAD_STATUS_STARTED && m_threadHandle)
	{
		m_status=THREAD_STATUS_SUSPENDED;
		m_threadLock->Unlock();
		SuspendThread(m_threadHandle);
		return true;
	}
	else
	{
		// Thread Not in Started State
		System::OutputDebugString(_T("The thread (%x): Thread must be in running state in order to suspend!\r\n"),m_threadId);

	}
	m_threadLock->Unlock();
	return false;
}


bool Thread::Terminate()
{
	EP_ASSERT_EXPR(m_threadHandle!=System::GetCurrentThread(),_T("Exception : Thread should not terminate self."));
	m_threadLock->Lock();
	if(m_status!=THREAD_STATUS_TERMINATED && m_threadHandle)
	{
		
		m_status=THREAD_STATUS_TERMINATED;
		m_exitCode=1;
		m_threadLock->Unlock();
		if(System::TerminateThread(m_threadHandle,m_exitCode))
		{
			m_threadLock->Lock();
			if(m_type==THREAD_TYPE_CREATE_THREAD)
				CloseHandle(m_threadHandle);
			m_threadId=0;
			m_threadHandle=0;
			m_parentThreadId=0;
			m_parentThreadHandle=0;
			m_type=THREAD_TYPE_UNKNOWN;
			unsigned long exitCode=m_exitCode;
			m_threadLock->Unlock();
			onTerminated(exitCode);
			return true;
		}
		else
		{
			// TerminateThread Failed
			EpTString lastErrMsg;
			unsigned long lastErrNum=0;
			System::FormatLastErrorMessage(lastErrMsg,&lastErrNum);
			EP_ASSERT_EXPR(0,_T("Cannot terminate thread!\r\nThread ID: %d\r\n Error Code: %d\r\nError Message: %s"),m_threadId,lastErrNum,lastErrMsg);
			m_threadLock->Lock();
			m_threadId=0;
			m_threadHandle=0;
			m_parentThreadId=0;
			m_parentThreadHandle=0;
			m_type=THREAD_TYPE_UNKNOWN;
			m_threadLock->Unlock();
			return false;
		}
	}
	else
	{
		m_threadLock->Unlock();
		// No Thread Exists
		System::OutputDebugString(_T("The thread (%x): There is no thread to terminate!\r\n"),m_threadId);
	}
	return true;
}


unsigned long Thread::WaitFor(const unsigned long tMilliseconds)
{
	if(m_status!=THREAD_STATUS_TERMINATED && m_threadHandle)
	{
		return System::WaitForSingleObject(m_threadHandle,tMilliseconds);
	}
	else
	{
		System::OutputDebugString(_T("The thread (%x): Thread is not started!\r\n"),m_threadId);
		return WAIT_FAILED;
	}
}
bool Thread::Joinable()
{
	return (m_status!=THREAD_STATUS_TERMINATED && m_threadHandle);
}

void Thread::Join()
{
	EP_ASSERT(Joinable()==true);
	System::WaitForSingleObject(m_threadHandle,WAITTIME_INIFINITE);
}

void Thread::Detach()
{
	EP_ASSERT(Joinable()==true);
	m_threadLock->Lock();
	m_status=THREAD_STATUS_TERMINATED;
	if(m_type==THREAD_TYPE_CREATE_THREAD)
		CloseHandle(m_threadHandle);
	m_threadHandle=0;
	m_threadId=0;
	m_parentThreadHandle=0;
	m_parentThreadId=0;
	m_type=THREAD_TYPE_UNKNOWN;
	m_exitCode=0;
	m_threadLock->Unlock();

}
bool Thread::TerminateAfter(const unsigned long tMilliseconds)
{
	if(m_status!=THREAD_STATUS_TERMINATED && m_threadHandle)
	{
		unsigned long status=System::WaitForSingleObject(m_threadHandle,tMilliseconds);
		switch(status)
		{
		case WAIT_ABANDONED:
		case WAIT_TIMEOUT:
		case WAIT_FAILED:

			return Terminate();
			break;
		default:
			break;
		}
		return true;
	}
	else
	{
		System::OutputDebugString(_T("The thread (%x): Thread is not started!\r\n"),m_threadId);
		return true;
	}

}


int Thread::run()
{
	execute();
	successTerminate();
	return 0;
}

/*static */

unsigned int Thread::entryPoint(void * pthis)
{
	Thread * pt = reinterpret_cast<Thread*>(pthis);
	pt->run();
	return 0;
}


unsigned long Thread::entryPoint2(void * pthis)
{
	Thread * pt = reinterpret_cast<Thread*>(pthis);
	pt->run();
	return 0;
}
bool Thread::SetPriority(ThreadPriority priority)
{
	LockObj lock(m_threadLock);
	bool ret=false;
	if(m_threadHandle!=0)
	{
		ret=(bool)::SetThreadPriority(m_threadHandle,(int)priority);
		if(ret)
			m_threadPriority=priority;
	}
	return ret;

}
ThreadPriority Thread::GetPriority()
{
	LockObj lock(m_threadLock);
	ThreadPriority ret=EP_THREAD_PRIORITY_ERROR_RETURN;
	if(m_threadHandle!=0)
		ret=(ThreadPriority)::GetThreadPriority(m_threadHandle);
	return ret;

}
void Thread::execute()
{
	// Do any execution here
	m_threadFunc();
}
void Thread::onTerminated(unsigned long exitCode,bool isInDeletion)
{
	// Do any clean up here
}
void Thread::successTerminate()
{
	m_threadLock->Lock();
	m_status=THREAD_STATUS_TERMINATED;
	if(m_type==THREAD_TYPE_CREATE_THREAD)
		CloseHandle(m_threadHandle);
	m_threadHandle=0;
	m_threadId=0;
	m_parentThreadHandle=0;
	m_parentThreadId=0;
	m_type=THREAD_TYPE_UNKNOWN;
	m_exitCode=0;
	m_threadLock->Unlock();
	onTerminated(m_exitCode);
	
// 	unsigned long exitCode=0;
// 	if(m_type==THREAD_TYPE_BEGIN_THREAD)
// 		_endthreadex(exitCode);
// 	else
// 		ExitThread(exitCode);
}


