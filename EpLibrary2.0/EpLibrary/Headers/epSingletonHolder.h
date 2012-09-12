/*! 
@file epSingletonHolder.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 16, 2009
@brief Singleton Holder Interface
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
