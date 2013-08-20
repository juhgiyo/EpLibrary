/*! 
@file epThreadSafeClass.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 16, 2010
@brief Thread Safe Class Interface
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
			m_threadSafeLock=NULL;
			m_lockPolicy=lockPolicyType;
			switch(lockPolicyType)
			{
			case LOCK_POLICY_CRITICALSECTION:
				m_threadSafeLock=EP_NEW CriticalSectionEx();
				break;
			case LOCK_POLICY_MUTEX:
				m_threadSafeLock=EP_NEW Mutex();
				break;
			case LOCK_POLICY_NONE:
				m_threadSafeLock=EP_NEW NoLock();
				break;
			default:
				m_threadSafeLock=NULL;
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
			m_threadSafeLock=NULL;
			m_lockPolicy=b.m_lockPolicy;
			switch(m_lockPolicy)
			{
			case LOCK_POLICY_CRITICALSECTION:
				m_threadSafeLock=EP_NEW CriticalSectionEx();
				break;
			case LOCK_POLICY_MUTEX:
				m_threadSafeLock=EP_NEW Mutex();
				break;
			case LOCK_POLICY_NONE:
				m_threadSafeLock=EP_NEW NoLock();
				break;
			default:
				m_threadSafeLock=NULL;
				break;
			}
		}

		/*!
		Default Destructor

		Deletes the lock
		*/
		virtual ~ThreadSafeClass()
		{	
			if(m_threadSafeLock)
				EP_DELETE m_threadSafeLock;
			m_threadSafeLock=NULL;
		}
		
		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		ThreadSafeClass & operator=(const ThreadSafeClass&b)
		{
			if(this!=&b)
			{
				if(m_threadSafeLock)
					EP_DELETE m_threadSafeLock;
				m_threadSafeLock=NULL;

				m_lockPolicy=b.m_lockPolicy;
				switch(m_lockPolicy)
				{
				case LOCK_POLICY_CRITICALSECTION:
					m_threadSafeLock=EP_NEW CriticalSectionEx();
					break;
				case LOCK_POLICY_MUTEX:
					m_threadSafeLock=EP_NEW Mutex();
					break;
				case LOCK_POLICY_NONE:
					m_threadSafeLock=EP_NEW NoLock();
					break;
				default:
					m_threadSafeLock=NULL;
					break;
				}
			}
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
		BaseLock * m_threadSafeLock;
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
			@remark This is NOT copying!! This moves b to a.
			*/
			ThreadSafeObj & operator=(const ThreadSafeObj&b)
			{
				if(this!=&b)
				{
					if(m_lockPtr)
					{
						m_lockPtr->Unlock();
						m_lockPtr=NULL;
					}
					m_lockPtr=b.m_lockPtr;
					ThreadSafeObj& unSafeB=const_cast<ThreadSafeObj&>(b);
					unSafeB.m_lockPtr=NULL;

				}
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
			return m_threadSafeLock;
		}

	public:
		///  Instant Lock Type Definition
		typedef ThreadSafeObj InstantLock; 
	};
}
#endif //__EP_THREAD_SAFE_CLASS_H__