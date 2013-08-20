/*! 
@file epSmartObject.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2009
@brief Smart Object Interface
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

An Interface for the Smart Object.

*/
#ifndef __EP_SMART_OBJECT_H__
#define __EP_SMART_OBJECT_H__
#include "epLib.h"
#include "epSystem.h"
#include "epSimpleLogger.h"
#include "epCriticalSectionEx.h"
#include "epMutex.h"
#include "epNoLock.h"
#include "epException.h"

namespace epl
{

	/*! 
	@class SmartObject epSmartObject.h
	@brief This is a base class for Smart Object Classes  

	Implements the System Functions.
	*/
	class EP_LIBRARY SmartObject
	{
	public:
		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		SmartObject & operator=(const SmartObject&b);

		/*!
		Returns the current reference count.
		@return the current reference count.
		*/
		int GetReferenceCount()
		{
			LockObj lock(m_refCounterLock);
			return m_refCount;
		}


	#if !defined(_DEBUG)
		/*!
		Increment this object's reference count
		*/
		void RetainObj();

		/*!
		Decrement this object's reference count
		if the reference count is 0 then delete this object.
		*/
		void ReleaseObj();

	protected:
		/*!
		Default Contructor
		@param[in] lockPolicyType The lock policy
		*/
		SmartObject(LockPolicy lockPolicyType=EP_LOCK_POLICY);
		 
		/*!
		Default Copy Constructor
		@param[in] b the second object
		*/
		SmartObject(const SmartObject& b);

		/*!
		Default Destructor
		*/
		virtual ~SmartObject();
		
	#else //!defined(_DEBUG)
		/*!
		Increment this object's reference count
		*/
		void RetainObj(TCHAR *fileName, TCHAR *funcName, unsigned int lineNum)
		{
			LockObj lock(m_refCounterLock);
			m_refCount++;
			LOG_THIS_MSG(_T("%s::%s(%d) Retained Object : %d (Current Reference Count = %d)"),fileName,funcName,lineNum,this, this->m_refCount);
		}

		/*!
		Decrement this object's reference count
		if the reference count is 0 then delete this object.
		*/
		void ReleaseObj(TCHAR *fileName, TCHAR *funcName, unsigned int lineNum)
		{
			m_refCounterLock->Lock();
			m_refCount--;
			LOG_THIS_MSG(_T("%s::%s(%d) Released Object : %d (Current Reference Count = %d)"),fileName,funcName,lineNum,this, this->m_refCount);
			if(m_refCount==0)
			{
				m_refCount++; // this increment is dummy addition to make pair with destructor.
				m_refCounterLock->Unlock();
				EP_DELETE this;
				return;
			}
			EP_ASSERT_EXPR(m_refCount>=0, _T("Reference Count is negative Value! Reference Count : %d"),m_refCount);
			m_refCounterLock->Unlock();

		}

	protected:
		/*!
		Default Contructor
		@param[in] lockPolicyType The lock policy
		*/
		SmartObject(TCHAR *fileName, TCHAR *funcName, unsigned int lineNum,LockPolicy lockPolicyType=EP_LOCK_POLICY)
		{
			m_refCount=1;
			LOG_THIS_MSG(_T("%s::%s(%d) Allocated Object : %d (Current Reference Count = %d)"),fileName,funcName,lineNum,this, this->m_refCount);
			m_lockPolicy=lockPolicyType;
			switch(lockPolicyType)
			{
			case LOCK_POLICY_CRITICALSECTION:
				m_refCounterLock=EP_NEW CriticalSectionEx();
				break;
			case LOCK_POLICY_MUTEX:
				m_refCounterLock=EP_NEW Mutex();
				break;
			case LOCK_POLICY_NONE:
				m_refCounterLock=EP_NEW NoLock();
				break;
			default:
				m_refCounterLock=NULL;
				break;
			}
		}

		/*!
		Default Copy Constructor
		@param[in] b the second object
		*/
		SmartObject(TCHAR *fileName, TCHAR *funcName, unsigned int lineNum,const SmartObject& b)
		{
			m_refCount=1;
			LOG_THIS_MSG(_T("%s::%s(%d) Allocated Object : %d (Current Reference Count = %d)"),fileName,funcName,lineNum,this, this->m_refCount);
			m_lockPolicy=b.m_lockPolicy;
			switch(m_lockPolicy)
			{
			case LOCK_POLICY_CRITICALSECTION:
				m_refCounterLock=EP_NEW CriticalSectionEx();
				break;
			case LOCK_POLICY_MUTEX:
				m_refCounterLock=EP_NEW Mutex();
				break;
			case LOCK_POLICY_NONE:
				m_refCounterLock=EP_NEW NoLock();
				break;
			default:
				m_refCounterLock=NULL;
				break;
			}
		}

		/*!
		Default Destructor
		*/
		virtual ~SmartObject()
		{
			m_refCounterLock->Lock();
			m_refCount--;
			m_refCounterLock->Unlock();
			LOG_THIS_MSG(_T("Deleted Object : %d (Current Reference Count = %d)"),this, this->m_refCount);
			EP_ASSERT_EXPR(m_refCount==0,_T("The Reference Count is not 0!! Reference Count : %d"),m_refCount);
			if(m_refCounterLock)
			{
				EP_DELETE m_refCounterLock;
			}
		}
	#endif //!defined(_DEBUG)


		
	private:

		/// Reference Counter
		int m_refCount;
		/// Lock
		BaseLock *m_refCounterLock;
		/// Lock Policy
		LockPolicy m_lockPolicy;
	};
#if defined(_DEBUG)
#define SmartObject(...) SmartObject(__TFILE__,__TFUNCTION__,__LINE__,__VA_ARGS__)
#define ReleaseObj() ReleaseObj(__TFILE__,__TFUNCTION__,__LINE__)
#define RetainObj() RetainObj(__TFILE__,__TFUNCTION__,__LINE__)
#endif//defined(_DEBUG)
}
#endif //__EP_SMART_OBJECT_H__