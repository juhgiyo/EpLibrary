/*! 
@file epSmartObject.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date April 17, 2009
@brief Smart Object Interface
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

An Interface for the Smart Object.

*/
#ifndef __EP_SMART_OBJECT_H__
#define __EP_SMART_OBJECT_H__
#include "epFoundationLib.h"
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
	class EP_FOUNDATION SmartObject
	{
	public:
		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		SmartObject & operator=(const SmartObject&b)
		{
			return *this;
		}

	#if !defined(_DEBUG)
		/*!
		Increment this object's reference count
		*/
		void Retain();

		/*!
		Decrement this object's reference count
		if the reference count is 0 then delete this object.
		*/
		void Release();

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
		
	#else //!defined(_DEBUG)
		/*!
		Increment this object's reference count
		*/
		__forceinline void Retain()
		{
			LockObj lock(m_refCounterLock);
			m_refCount++;
			LOG_THIS_MSG(_T("%s::%s(%d) Retained Object : %d (Current Refence Count = %d)"),__TFILE__,__TFUNCTION__,__LINE__,this, this->m_refCount);
		}

		/*!
		Decrement this object's reference count
		if the reference count is 0 then delete this object.
		*/
		__forceinline  void Release()
		{
			LockObj lock(m_refCounterLock);
			m_refCount--;
			LOG_THIS_MSG(_T("%s::%s(%d) Released Object : %d (Current Refence Count = %d)"),__TFILE__,__TFUNCTION__,__LINE__,this, this->m_refCount);
			if(m_refCount==0)
			{
				m_refCount++; // this increment is dummy addition to make pair with destructor.
				EP_DELETE this;
				return;
			}
			if(m_refCount<0)
			{
				EpString errMsg;
				System::SPrintf(errMsg,"Reference Count is negative Value! Reference Count : %d",m_refCount);
				EP_VERIFY_DOMAIN_ERROR_W_MSG(m_refCount>=0, errMsg);
			}

		}

	protected:
		/*!
		Default Contructor
		@param[in] lockPolicyType The lock policy
		*/
		__forceinline SmartObject(LockPolicy lockPolicyType=EP_LOCK_POLICY)
		{
			m_refCount=1;
			LOG_THIS_MSG(_T("%s::%s(%d) Allocated Object : %d (Current Refence Count = %d)"),__TFILE__,__TFUNCTION__,__LINE__,this, this->m_refCount);
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
		__forceinline SmartObject(const SmartObject& b)
		{
			m_refCount=1;
			LOG_THIS_MSG(_T("%s::%s(%d) Allocated Object : %d (Current Refence Count = %d)"),__TFILE__,__TFUNCTION__,__LINE__,this, this->m_refCount);
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
	#endif //!defined(_DEBUG)
		/*!
		Default Destructor
		*/
		virtual ~SmartObject();

		
	private:

		/// Reference Counter
		int m_refCount;
		/// Lock
		BaseLock *m_refCounterLock;
		/// Lock Policy
		LockPolicy m_lockPolicy;
	};
}
#endif //__EP_SMART_OBJECT_H__