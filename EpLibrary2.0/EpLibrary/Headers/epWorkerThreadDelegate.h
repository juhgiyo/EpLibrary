/*! 
@file epWorkerThreadDelegate.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date July 15, 2010
@brief Interface of Worker Thread Delegate .
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

An Interface for Worker Thread Delegate.

*/
#ifndef __EP_WORKER_THREAD_DELEGATE_H__
#define __EP_WORKER_THREAD_DELEGATE_H__
#include "epLib.h"
namespace epl
{

	/*! 
	@class WorkerThreadDelegate epWorkerThreadDelegate.h
	@brief A pure virtual class for Worker Thread Delegate.
	*/
	class EP_LIBRARY WorkerThreadDelegate
	{
	public:
		/*!
		Default Constructor
		*/
		WorkerThreadDelegate(){}
		
		/*!
		Default Destructor
		*/
		virtual ~WorkerThreadDelegate(){}
		
		/*!
		Call Back Function.
		@param[in] p the argument for call back function.
		*/
		virtual void CallBackFunc(BaseWorkerThread *p)=0;
	};

}
#endif //__EP_WORKER_THREAD_DELEGATE_H__