/*! 
@file epSingletonHolder.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 16, 2009
@brief Singleton Holder Interface
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

@section DESCRIPTION

An Interface for the Template Singleton Holder.

*/
#ifndef __EP_SINGLETON_HOLDER_H__
#define __EP_SINGLETON_HOLDER_H__

#include "epLib.h"

namespace epl
{

	/*! 
	@class SingletonHolder epSingletonHolder.h
	@brief This is a template holding class for Singleton classes  

	Interface for the Singlton Holder class.
	*/
	template<typename SingletonClass>
	class SingletonHolder
	{
	public:

		/*!
		Get the Singleton Instance of the Object
		@return the reference to the Singleton Object.
		*/
		static SingletonClass &Instance();
	private:


		/*!
		Default Constructor

		@remark The default constructor is in private to protect the singleton property.
		*/
		SingletonHolder();

		/*
		Default Destructor
		
		@remark The destructor is in private to protect the singleton property.
		*/
		~SingletonHolder();

		/*
		Copy Constructor
		
		@remark The copy constructor is in private to protect the singleton property.
		*/
		SingletonHolder(const SingletonHolder&b);

		/*
		Copy Operator
		
		@remark The copy operator is in private to protect the singleton property.
		*/
		SingletonHolder& operator=(const SingletonHolder& b);
	};

	template<typename SingletonClass>
	SingletonClass &SingletonHolder<SingletonClass>::Instance()
	{
#if (EP_LOCK_POLICY==LOCK_POLICY_CRITICALSECTION)
		static CriticalSectionEx m_lock;
#elif (EP_LOCK_POLICY==LOCK_POLICY_MUTEX)
		static Mutex m_lock;
#elif (EP_LOCK_POLICY==LOCK_POLICY_NONE)
		static NoLock m_lock;
#endif //(EP_LOCK_POLICY==LOCK_POLICY_CRITICALSECTION)
		LockObj lock(&m_lock);
		static SingletonClass holder;
		return holder;

	}
}
#endif //__EP_SINGLETON_HOLDER_H__
