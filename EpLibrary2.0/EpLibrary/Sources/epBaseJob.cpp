/*! 
BaseJob for the EpLibrary

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
#include "epBaseJob.h"
#include "epSystem.h"
#include "epSingletonHolder.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)


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
	BaseJob **_a=reinterpret_cast<BaseJob **>(const_cast<void*>(a));
	BaseJob **_b=reinterpret_cast<BaseJob **>(const_cast<void*>(b));
	if((*_a)->GetPriority() > (*_b)->GetPriority())
		return COMP_RESULT_LESSTHAN;
	else if((*_a)->GetPriority() < (*_b)->GetPriority())
		return COMP_RESULT_GREATERTHAN;
	return COMP_RESULT_GREATERTHAN;

}
void BaseJob::handleReport(const JobStatus status)
{

}