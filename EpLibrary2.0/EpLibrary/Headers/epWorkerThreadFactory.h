/*! 
@file epWorkerThreadFactory.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date July 07, 2010
@brief Interface of Factory Class for Worker Thread .
@version 2.0

@section LICENSE

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

@section DESCRIPTION

An Interface for Factory Class for Worker Thread.

*/
#ifndef __EP_WORKER_THREAD_FACTORY_H__
#define __EP_WORKER_THREAD_FACTORY_H__
#include "epLib.h"
#include "epBaseWorkerThread.h"

namespace epl
{

	/*! 
	@class WorkerThreadFactory epWorkerThreadFactory.h
	@brief A factory class that returns the Worker Thread objects.
	*/
	class EP_LIBRARY WorkerThreadFactory
	{
	public:
		/*!
		Return the new worker thread object with given life policy.
		@param[in] policy the life policy of the thread to create.
		@return Return the new worker thread object with given life policy.
		*/
		static	BaseWorkerThread *GetWorkerThread(const BaseWorkerThread::ThreadLifePolicy policy=BaseWorkerThread::THREAD_LIFE_INFINITE);
	};

}


#endif //__EP_WORKER_THREAD_FACTORY_H__