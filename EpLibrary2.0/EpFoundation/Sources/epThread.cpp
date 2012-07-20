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

using namespace epl;

HANDLE Thread::CreateThread(LPTHREAD_START_ROUTINE routineFunc,LPVOID param)
{
	unsigned long threadID=0;
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
		System::TerminateThread(m_threadHandle,0);
	}
	if(m_threadLock)
		EP_DELETE m_threadLock;
}


bool Thread::Start(void * arg,const ThreadOpCode opCode, const ThreadType threadType, const int stackSize)
{
	LockObj lock(m_threadLock);
	if(m_status==THREAD_STATUS_TERMINATED && !m_threadHandle)
	{
		setArg(arg);
		m_type=threadType;
		if(m_type==THREAD_TYPE_BEGIN_THREAD)
			m_threadHandle=reinterpret_cast<ThreadHandle>(_beginthreadex(NULL,stackSize,Thread::entryPoint,this,opCode,&m_threadId));
		else
			m_threadHandle=::CreateThread(NULL,stackSize,Thread::entryPoint2,this,opCode,reinterpret_cast<LPDWORD>(&m_threadId));
		if(!m_threadHandle)
		{
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
		return true;
	}
	else
	{
		// Error Thread already exists
		System::OutputDebugString(_T("The thread (%x): Thread already exists!"),m_threadId);
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
		System::OutputDebugString(_T("The thread (%x): Thread must be in suspended state in order to resume!"),m_threadId);

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
		System::OutputDebugString(_T("The thread (%x): Thread must be in running state in order to suspend!"),m_threadId);

	}
	return false;
}


bool Thread::Terminate()
{
	LockObj lock(m_threadLock);
	if(m_status!=THREAD_STATUS_TERMINATED && m_threadHandle)
	{
		unsigned long exitCode=0;
		m_status=THREAD_STATUS_TERMINATED;
		m_threadHandle=0;
		m_threadId=0;

		if(System::TerminateThread(m_threadHandle,exitCode))
		{
			CloseHandle(m_threadHandle);
			return true;
		}
		else
		{
			// TerminateThread Failed
			EpTString lastErrMsg;
			unsigned long lastErrNum=0;
			System::FormatLastErrorMessage(lastErrMsg,&lastErrNum);
			EP_ASSERT_EXPR(0,_T("Cannot terminate thread!\r\nThread ID: %d\r\n Error Code: %d\r\nError Message: %s"),lastErrNum,lastErrMsg);
			return false;
		}
	}
	else
	{
		// No Thread Exists
		System::OutputDebugString(_T("The thread (%x): There is no thread to terminate!"),m_threadId);
	}
	return true;
}


unsigned long Thread::WaitFor(const unsigned long tMilliseconds)
{
	if(m_status!=THREAD_STATUS_TERMINATED && m_threadHandle)
		return System::WaitForSingleObject(m_threadHandle,tMilliseconds);
	else
	{
		System::OutputDebugString(_T("The thread (%x): Thread is not started!"),m_threadId);
		return WAIT_FAILED;
	}
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
		System::OutputDebugString(_T("The thread (%x): Thread is not started!"),m_threadId);
		return true;
	}

}


void * Thread::GetArg() const
{
	return m_arg;
}



void Thread::setArg(void* a)
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


