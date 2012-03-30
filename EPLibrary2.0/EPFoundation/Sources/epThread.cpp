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
#include "epSimpleLogger.h"

using namespace epl;

HANDLE Thread::CreateThread(LPTHREAD_START_ROUTINE routineFunc,LPVOID param)
{
	DWORD threadID=0;
	return ::CreateThread(NULL, 0,routineFunc,param,0,&threadID);
}

Thread::Thread(LockPolicy lockPolicyType)
{
	m_threadId=0;
	m_threadHandle=0;
	m_arg=NULL;
	m_status=THREAD_STATUS_TERMINATED;
	m_lockPolicy=lockPolicyType;
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
Thread::Thread(const Thread & b)
{
	m_threadId=0;
	m_threadHandle=0;
	m_arg=NULL;
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
Thread::~Thread()
{
	if(m_status!=THREAD_STATUS_TERMINATED)
	{
		TerminateThread(m_threadHandle,0);
	}
	if(m_threadLock)
		EP_DELETE m_threadLock;
}


bool Thread::Start(void * arg,const ThreadOpCode opCode, const ThreadType threadType, const int stackSize)
{
	LockObj lock(m_threadLock);
	if(m_status==THREAD_STATUS_TERMINATED && !m_threadHandle)
	{
		SetArg(arg);
		m_type=threadType;
		if(m_type==THREAD_TYPE_BEGIN_THREAD)
			m_threadHandle=reinterpret_cast<ThreadHandle>(_beginthreadex(NULL,stackSize,Thread::entryPoint,this,opCode,&m_threadId));
		else
			m_threadHandle=::CreateThread(NULL,stackSize,Thread::entryPoint2,this,opCode,reinterpret_cast<LPDWORD>(&m_threadId));

		if(!m_threadHandle)
		{
			TCHAR errmsg[512];
			System::FormatLastErrorMessage(errmsg,512);
			EP_WASSERT(0,_T("Cannot create thread!\nError Code: %d\nError Message: %s"),System::GetLastError(),errmsg);
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
		return true;
	}
	else
	{
		// Error Thread already exists
		EP_NOTICEBOX(_T("Thread already exists! Thread ID: %d"),m_threadId);
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
		EP_NOTICEBOX(_T("Thread must be in suspended state in order to resume! Thread ID: %d"),m_threadId);

	}
	return false;
}


bool Thread::Suspend()
{
	LockObj lock(m_threadLock);
	if(m_status==THREAD_STATUS_STARTED && m_threadHandle)
	{
		m_status=THREAD_STATUS_SUSPENDED;
		SuspendThread(m_threadHandle);
		return true;
	}
	else
	{
		// Thread Not in Started State
		EP_NOTICEBOX(_T("Thread must be in running state in order to suspend! Thread ID: %d"),m_threadId);

	}
	return false;
}


bool Thread::Terminate()
{
	LockObj lock(m_threadLock);
	if(m_status!=THREAD_STATUS_TERMINATED && m_threadHandle)
	{
		DWORD exitCode=0;
		m_status=THREAD_STATUS_TERMINATED;
		m_threadHandle=0;
		m_threadId=0;

		if(TerminateThread(m_threadHandle,exitCode))
		{
			CloseHandle(m_threadHandle);
			return true;
		}
		else
		{
			// TerminateThread Failed


			TCHAR errmsg[512];
			System::FormatLastErrorMessage(errmsg,512);
			EP_WASSERT(0,_T("Cannot terminate thread!\nTerminateThread Function failed!\nThread ID: %d\n Error Code: %d\nError Message: %s"),
				m_threadId,System::GetLastError(),errmsg);
		}
	}
	else
	{
		// No Thread Exists
		EP_NOTICEBOX(_T("There is no thread to terminate! Thread ID: %d"),m_threadId);
	}
	return false;
}


unsigned long Thread::WaitFor(const unsigned long tMilliseconds)
{
	if(m_status!=THREAD_STATUS_TERMINATED && m_threadHandle)
		return ::WaitForSingleObject(m_threadHandle,tMilliseconds);
	else
	{
		EP_NOTICEBOX(_T("Thread %d is not Running state!"),m_threadId);
		return 0;
	}
}


bool Thread::TerminateAfter(const unsigned long tMilliseconds)
{
	if(m_status!=THREAD_STATUS_TERMINATED && m_threadHandle)
	{
		unsigned long status=::WaitForSingleObject(m_threadHandle,tMilliseconds);
		switch(status)
		{
		case WAIT_ABANDONED:
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
			Terminate();
			LOG_THIS_MSG(_T("Thread did not ended properly.\nError Code:%d"),status);
			break;
		default:
			break;
		}
		return true;
	}
	else
	{
		EP_NOTICEBOX(_T("Thread %d is not Running state!"),m_threadId);
		return false;
	}

}


void * Thread::GetArg() const
{
	return m_arg;
}



void Thread::SetArg(void* a)
{
	if(m_status!=THREAD_STATUS_STARTED)
		m_arg=a;
	else
	{
		EP_NOTICEBOX(_T("Cannot Set Argument during Thread Running!"));
	}
}


int Thread::run()
{
	setup();
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


void Thread::setup()
{
	// Do any setup here
}
void Thread::execute()
{
	// Do any setup here
}

void Thread::successTerminate()
{
	m_threadLock->Lock();
	m_status=THREAD_STATUS_TERMINATED;
	m_threadHandle=0;
	m_threadId=0;
	m_threadLock->Unlock();
	unsigned long exitCode=0;
	if(m_type==THREAD_TYPE_BEGIN_THREAD)
		_endthreadex(exitCode);
	else
		ExitThread(exitCode);
}


