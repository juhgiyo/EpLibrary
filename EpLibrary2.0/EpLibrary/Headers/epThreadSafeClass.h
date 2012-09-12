/*! 
@file epThreadSafeClass.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 16, 2010
@brief Thread Safe Class Interface
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

An Interface for Thread Safe Class.

*/
#ifndef __EP_THREAD_SAFE_CLASS_H__
#define __EP_THREAD_SAFE_CLASS_H__
#include "epLib.h"
#include "epSystem.h"
#include "epCriticalSectionEx.h"
#include "epMutex.h"
#include "epNoLock.h"
/*!
@def DECLARE_THREAD_SAFE_CLASS
@brief Macro for declaring Thread Safe class
*/
#define DECLARE_THREAD_SAFE_CLASS(classname) \
	typedef epl::ThreadSafeClass< classname> MyThreadSafeClass

/*!
@def CRITICAL_SECTION_BLOCK
@brief Macro for declaring Critical Section Block
*/
#define CRITICAL_SECTION_BLOCK \
	MyThreadSafeClass::InstantLock lock


namespace epl
{
	/*! 
	@class ThreadSafeClass epThreadSafeClass.h
	@brief A template class for Thread Safe classes.
	*/
	template <typename SafeClass>
	class ThreadSafeClass
	{
		friend class ThreadSafeObj;
	public:
		/*!
		Default Constructor

		Initializes the lock
		@param[in] lockPolicyType The lock policy
		*/
		ThreadSafeClass(LockPolicy lockPolicyType=EP_LOCK_POLICY)
		{
			m_lock=NULL;
			m_lockPolicy=lockPolicyType;
			switch(lockPolicyType)
			{
			case LOCK_POLICY_CRITICALSECTION:
				m_lock=EP_NEW CriticalSectionEx();
				break;
			case LOCK_POLICY_MUTEX:
				m_lock=EP_NEW Mutex();
				break;
			case LOCK_POLICY_NONE:
				m_lock=EP_NEW NoLock();
				break;
			default:
				m_lock=NULL;
				break;
			}
		}

		/*!
		Default Copy Constructor

		Initializes the ThreadSafeClass
		@param[in] b the second object
		*/
		ThreadSafeClass(const ThreadSafeClass& b)
		{
			m_lock=NULL;
			m_lockPolicy=b.m_lockPolicy;
			switch(m_lockPolicy)
			{
			case LOCK_POLICY_CRITICALSECTION:
				m_lock=EP_NEW CriticalSectionEx();
				break;
			case LOCK_POLICY_MUTEX:
				m_lock=EP_NEW Mutex();
				break;
			case LOCK_POLICY_NONE:
				m_lock=EP_NEW NoLock();
				break;
			default:
				m_lock=NULL;
				break;
			}
		}

		/*!
		Default Destructor

		Deletes the lock
		*/
		virtual ~ThreadSafeClass()
		{	
			if(m_lock)
				EP_DELETE m_lock;
		}
		
		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		ThreadSafeClass & operator=(const ThreadSafeClass&b)
		{
			return *this;
		}

		/*!
		Return the reference to the Thread Safe class.
		*/
		static ThreadSafeClass<SafeClass> *GetInstance()
		{
			static ThreadSafeClass<SafeClass> m_class;
			return &m_class;
		}

	protected:
		/// the actual lock member
		BaseLock * m_lock;
		/// Lock Policy
		LockPolicy m_lockPolicy;


		/*! 
		@class ThreadSafeObj epThreadSafeClass.h
		@brief A class for handling the Thread Safe within the class.

		Declare the ThreadSafeObj within the class functions or regions to 
		make the section as Critical Section.
		*/
		class EP_LIBRARY ThreadSafeObj
		{
			friend ThreadSafeClass;
		public:
			/*!
			Default Constructor 

			Lock the Critical Section.
			*/
			ThreadSafeObj()
			{
				m_lockPtr=ThreadSafeClass<SafeClass>::GetInstance()->getLock();
				if(m_lockPtr)
					m_lockPtr->Lock();
			}

			/*!
			Default Copy Constructor

			Initializes the Semaphore
			@param[in] b the second object
			*/
			ThreadSafeObj(const ThreadSafeObj& b)
			{
				m_lockPtr=ThreadSafeClass<SafeClass>::GetInstance()->getLock();
				if(m_lockPtr)
					m_lockPtr->Lock();
			}
			/*!
			Default Destructor

			Unlock the Critical Section.
			*/
			virtual ~ThreadSafeObj()
			{
				if(m_lockPtr)
					m_lockPtr->Unlock();
			}

			/*!
			Assignment operator overloading
			@param[in] b the second object
			@return the new copied object
			*/
			ThreadSafeObj & operator=(const ThreadSafeObj&b)
			{
				return *this;
			}
		private:



			/// the pointer to the actual lock member.
			BaseLock *m_lockPtr;
		};

	private:
	
		/*!
		Return the pointer to the lock
		*/
		BaseLock *getLock() const
		{
			return m_lock;
		}

	public:
		///  Instant Lock Type Definition
		typedef ThreadSafeObj InstantLock; 
	};
}
#endif //__EP_THREAD_SAFE_CLASS_H__