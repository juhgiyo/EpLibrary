/*! 
@file epWorkerThreadDelegate.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date July 15, 2010
@brief Interface of Worker Thread Delegate .
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