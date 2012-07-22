/*! 
WorkerThreadSingle for the EpLibrary
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
#include "epWorkerThreadSingle.h"

using namespace epl;

void WorkerThreadSingle::execute()
{
	while(true)
	{
		if(m_workPool.IsEmpty())
			break;
		EP_ASSERT_EXPR(GetArg(),_T("Job Processor is NULL!"));
		if(GetArg())
			break;
		BaseJob * jobPtr=m_workPool.Front();
		jobPtr->RetainObj();
		m_workPool.Pop();
		jobPtr->JobReport(BaseJob::JOB_STATUS_IN_PROCESS);
		(reinterpret_cast<BaseJobProcessor*>(GetArg()))->DoJob(GetID(),m_lifePolicy,jobPtr);
		jobPtr->JobReport(BaseJob::JOB_STATUS_DONE);
		jobPtr->ReleaseObj();
	}
	callCallBack();
}