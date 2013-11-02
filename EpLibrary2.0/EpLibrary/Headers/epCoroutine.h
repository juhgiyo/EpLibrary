/*! 
@file epCoroutine.h
@author Lyn - April 03, 2012
     edited by Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		       <http://github.com/juhgiyo/eplibrary>
@date October 29, 2013
@brief Coroutine Interface
@version 2.0

@section LICENSE

The MIT License (MIT)

Copyright (c) 2008-2013 Woong Gyu La <juhgiyo@gmail.com>

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

------------------------------------------------------------------------------

Part of this software is

Lyn's blog

Please refer to <http://lunapiece.net/Article/101670> for the license.

------------------------------------------------------------------------------

@section DESCRIPTION

An Interface for Coroutine Class.


*/

#ifndef __EP_COROUTINE_H__
#define __EP_COROUTINE_H__

#include "epLib.h"
#include <windows.h>

#define yield this->__yield()
#define yield_return(x) this->__yield_return(x)

namespace epl
{
	/*! 
	@class Coroutine epCoroutine.h
	@brief A class for Coroutine.
	*/
	template<typename RETURN_TYPE>
	class Coroutine
	{
	public:
		/*!
		Default Constructor

		Initializes the coroutine
		*/
		Coroutine()
		{
			m_fiber = CreateFiber(0, &Coroutine::CoroutineFunc, this);
		}

		/*!
		Default Destructor

		Destroy the coroutine
		*/
		virtual ~Coroutine()
		{
			DeleteFiber(m_fiber);
		}

		/*!
		Start or resume the coroutine.
		@returns the return value of the coroutine.
		*/
		RETURN_TYPE StartCoroutine()
		{
			ConvertThreadToFiber(reinterpret_cast<LPVOID>(GetCurrentThreadId()));
			m_owner = GetCurrentFiber();
			SwitchToFiber(m_fiber);
			return m_result;
		}
	protected:
		/*!
		Pause the execution and be resume the owner execution
		*/
		void __yield()
		{
			SwitchToFiber(m_owner);
		}

		/*!
		Pause the execution and be resume the owner execution
		@param[in] returnValue the return value of the coroutine
		@returns the return value of the coroutine.
		*/
		void __yield_return(RETURN_TYPE& returnValue)
		{
			m_result = returnValue;
			SwitchToFiber(m_owner);
		}

		/*!
		User defined coroutine function
		@remark Subclass should override this function to create the coroutine
		*/
		virtual void Execute() = 0;	
	private:

		/*!
		Actual coroutine execution function
		*/
		void executeCoroutineMethod()
		{
			Execute();
		}

		/*!
		Method to interface the fiber and class to execute the coroutine on the fiber.
		@param[in] __this the instance of coroutine class
		*/
		static void __stdcall CoroutineFunc(LPVOID __this)
		{
			Coroutine* _this = (Coroutine*)__this;
			_this->executeCoroutineMethod();
		}

	private:
		/// Fiber handle
		HANDLE m_fiber;
		/// Owner fiber handle
		LPVOID m_owner;
		/// Temporary result of coroutine execution
		RETURN_TYPE m_result;
	};

	/*! 
	@class Coroutine epCoroutine.h
	@brief A class for Coroutine with void return type.
	*/
	template<> class Coroutine<void>
	{
	public:
		/*!
		Default Constructor

		Initializes the coroutine
		*/
		Coroutine()
		{
			m_fiber = CreateFiber(0, &Coroutine::CoroutineFunc, this);
		}

		/*!
		Default Destructor

		Destroy the coroutine
		*/
		virtual ~Coroutine()
		{
			DeleteFiber(m_fiber);
		}

		/*!
		Start or resume the coroutine.
		@returns the return value of the coroutine.
		*/
		void StartCoroutine()
		{
			ConvertThreadToFiber(reinterpret_cast<LPVOID>(GetCurrentThreadId()));
			m_owner = GetCurrentFiber();
			SwitchToFiber(m_fiber);
		}
	protected:
		/*!
		Pause the execution and be resume the owner execution
		*/
		void __yield()
		{
			SwitchToFiber(m_owner);
		}

		/*!
		User defined coroutine function
		@remark Subclass should override this function to create the coroutine
		*/
		virtual void Execute() = 0;	

	private:
		/*!
		Actual coroutine execution function
		*/
		void executeCoroutineMethod()
		{
			Execute();
		}

		/*!
		Method to interface the fiber and class to execute the coroutine on the fiber.
		@param[in] __this the instance of coroutine class
		*/
		static void __stdcall CoroutineFunc(LPVOID __this)
		{
			Coroutine* _this = (Coroutine*)__this;
			_this->executeCoroutineMethod();
		}

	private:
		/// Fiber handle
		HANDLE m_fiber;
		/// Owner fiber handle
		LPVOID m_owner;


	};
}
#endif //__EP_COROUTINE_H__