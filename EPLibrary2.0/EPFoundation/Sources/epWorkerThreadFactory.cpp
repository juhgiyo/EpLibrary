/*! 
WorkerThreadFactory for the EpLibrary
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
#include "epWorkerThreadFactory.h"
#include "epWorkerThreadInfinite.h"
#include "epWorkerThreadSingle.h"

using namespace epl;

BaseWorkerThread *WorkerThreadFactory::GetWorkerThread(const BaseWorkerThread::ThreadLifePolicy policy)
{
	if(policy==BaseWorkerThread::THREAD_LIFE_INFINITE)
		return (BaseWorkerThread*)EP_NEW WorkerThreadInfinite(policy);
	else if(policy==BaseWorkerThread::THREAD_LIFE_TERMINATE_AFTER_WORK)
		return (BaseWorkerThread*)EP_NEW WorkerThreadSingle(policy);
	else if(policy==BaseWorkerThread::THREAD_LIFE_SUSPEND_AFTER_WORK)
		return (BaseWorkerThread*)EP_NEW WorkerThreadInfinite(policy);
	else
		EP_WASSERT(0,_T("Unknown Thread Life Policy Input(%d)!"),policy);
	return NULL;
}