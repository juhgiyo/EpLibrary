/*! 
@file epWorkerThreadSingle.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date July 07, 2010
@brief Worker Thread Class Interface with single job.
@version 2.0

@section LICENSE

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

@section DESCRIPTION

A Interface for Single Job Worker Thread Class.

*/
#ifndef __EP_WORKER_THREAD_SINGLE_H__
#define __EP_WORKER_THREAD_SINGLE_H__
#include "epLib.h"
#include "epWorkerThreadFactory.h"
#include "epBaseJobProcessor.h"

namespace epl
{

	/*! 
	@class WorkerThreadSingle epWorkerThreadSingle.h
	@brief A class that implements single-job Worker Thread Class.
	*/
	class EP_LIBRARY WorkerThreadSingle:public BaseWorkerThread
	{
	public:
		/*!
		Default Constructor

		Initializes the thread class
		@param[in] policy the life policy of this worker thread.
		@param[in] lockPolicyType The lock policy
		*/
		WorkerThreadSingle(const ThreadLifePolicy policy,LockPolicy lockPolicyType=EP_LOCK_POLICY):BaseWorkerThread(policy,lockPolicyType)
		{}

		/*!
		Default Copy Constructor

		Initializes the Thread class
		@param[in] b the second object
		*/
		WorkerThreadSingle(const WorkerThreadSingle & b):BaseWorkerThread(b)
		{}
		
		/*!
		Default Destructor

		Destroy the Thread Class
		*/
		virtual ~WorkerThreadSingle(){}

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		WorkerThreadSingle &operator=(const WorkerThreadSingle & b)
		{
			if(this!=&b)
			{
				BaseWorkerThread::operator =(b);
			}
			return *this;
		}

	protected:
		/*!
		Actual single-job Thread Code.
		*/
		virtual void execute();
	
	};

}

#endif //__EP_WORKER_THREAD_SINGLE_H__