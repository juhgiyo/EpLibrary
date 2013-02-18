/*! 
@file epThreadSafeQueue.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date July 07, 2010
@brief ThreadSafeQueue Interface
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

An Interface for Thread Safe Queue.

*/
#ifndef __EP_THREAD_SAFE_QUEUE_H__
#define __EP_THREAD_SAFE_QUEUE_H__
#include "epLib.h"
#include <vector>
#include "epCriticalSectionEx.h"
#include "epMutex.h"
#include "epNoLock.h"
#include "epException.h"

namespace epl
{

	/*! 
	@class ThreadSafeQueue epThreadSafeQueue.h
	@brief A class for Thread Safe Queue.
	*/
	template <typename DataType>
	class ThreadSafeQueue
	{
	public:

		/*!
		Default Constructor

		Initializes the queue
		@param[in] lockPolicyType The lock policy
		*/
		ThreadSafeQueue(LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the queue
		@param[in] b the second object
		*/
		ThreadSafeQueue(const ThreadSafeQueue& b);

		/*!
		Default Destructor

		Destroy the queue
		*/
		virtual ~ThreadSafeQueue();

		/*!
		Assignment Operator Overloading

		the ThreadSafeQueue set as given ThreadSafeQueue b
		@param[in] b right side of ThreadSafeQueue
		@return this object
		*/
		ThreadSafeQueue & operator=(const ThreadSafeQueue&b);

		/*!
		Check if the queue is empty.
		@returns Returns true if the queue is empty, otherwise false.
		*/
		bool IsEmpty() const;

		/*!
		Check if the given obj exists in the queue.
		@returns Returns true if exists, otherwise false.
		*/
		bool IsExist(DataType const &data) const;

		/*!
		Return the size of the queue.
		@return the size of the queue.
		*/
		int Size() const;

		/*!
		Return the first item within the queue.
		@return the first element of the queue.
		*/
		DataType &Front();

		/*!
		Return the last item within the queue.
		@return the last element of the queue.
		*/
		DataType &Back();

		/*!
		Insert the new item into the queue.
		@param[in] data The inserting data.
		*/
		virtual void Push(DataType const &data);

		/*!
		Erase the given item from the queue.
		@param[in] data The data to erase.
		*/
		bool Erase(DataType const &data);

		/*!
		Remove the first item from the queue.
		*/
		virtual void Pop();

		/*!
		Clear the queue.
		*/
		void Clear();

	protected:
		/// Actual queue structure
		std::vector<DataType> m_queue;

		/// lock
		BaseLock *m_queueLock;

		/// Lock Policy
		LockPolicy m_lockPolicy;
	};


	template <typename DataType>
	ThreadSafeQueue<DataType>::ThreadSafeQueue(LockPolicy lockPolicyType)
	{
		m_lockPolicy=lockPolicyType;
		switch(lockPolicyType)
		{
		case LOCK_POLICY_CRITICALSECTION:
			m_queueLock=EP_NEW CriticalSectionEx();
			break;
		case LOCK_POLICY_MUTEX:
			m_queueLock=EP_NEW Mutex();
			break;
		case LOCK_POLICY_NONE:
			m_queueLock=EP_NEW NoLock();
			break;
		default:
			m_queueLock=NULL;
			break;
		}
	}

	template <typename DataType>
	ThreadSafeQueue<DataType>::ThreadSafeQueue(const ThreadSafeQueue& b)
	{
		m_queue=b.m_queue;
		m_lockPolicy=b.m_lockPolicy;
		switch(m_lockPolicy)
		{
		case LOCK_POLICY_CRITICALSECTION:
			m_queueLock=EP_NEW CriticalSectionEx();
			break;
		case LOCK_POLICY_MUTEX:
			m_queueLock=EP_NEW Mutex();
			break;
		case LOCK_POLICY_NONE:
			m_queueLock=EP_NEW NoLock();
			break;
		default:
			m_queueLock=NULL;
			break;
		}
	}

	template <typename DataType>
	ThreadSafeQueue<DataType>::~ThreadSafeQueue()
	{
		m_queueLock->Lock();
		m_queue.clear();
		m_queueLock->Unlock();
		if(m_queueLock)
			EP_DELETE m_queueLock;
	}

	template <typename DataType>
	bool ThreadSafeQueue<DataType>::IsEmpty() const
	{
		return m_queue.empty();
	}
	
	template <typename DataType>
	bool ThreadSafeQueue<DataType>::IsExist(DataType const &data) const
	{
		LockObj lock(m_queueLock);
		if(m_queue.empty())
			return false;

		std::vector<DataType>::const_iterator iter;
		for(iter=m_queue.begin();iter!=m_queue.end();iter++)
		{
			if((*iter)==data)
				return true;
		}
		return false;
		
	}

	template <typename DataType>
	void ThreadSafeQueue<DataType>::Clear()
	{
		LockObj lock(m_queueLock);
		m_queue.clear();

	}

	template <typename DataType>
	int ThreadSafeQueue<DataType>::Size() const
	{
		return m_queue.size();
	}

	template <typename DataType>
	DataType &ThreadSafeQueue<DataType>::Front()
	{
		LockObj lock(m_queueLock);
		if(m_queue.empty())
		{
			EP_ASSERT_EXPR(0,_T("Empty Queue"));
		}
		return m_queue.front();
	}

	template <typename DataType>
	DataType &ThreadSafeQueue<DataType>::Back()
	{
		LockObj lock(m_queueLock);
		if(m_queue.empty())
		{
			EP_ASSERT_EXPR(0,_T("Empty Queue"));
		}
		return m_queue.back();
	}

	template <typename DataType>
	void ThreadSafeQueue<DataType>::Push(DataType const & data)
	{
		LockObj lock(m_queueLock);
		m_queue.push_back(data);
	}

	template <typename DataType>
	bool ThreadSafeQueue<DataType>::Erase(DataType const &data)
	{
		LockObj lock(m_queueLock);
		std::vector<DataType>::iterator iter;
		for(iter=m_queue.begin();iter!=m_queue.end();iter++)
		{
			if(*iter==data)
			{
				m_queue.erase(iter);
				return true;
			}
		}
		return false;
	}

	template <typename DataType>
	void ThreadSafeQueue<DataType>::Pop()
	{
		LockObj lock(m_queueLock);
		if(m_queue.empty())
		{
			EP_ASSERT_EXPR(0,_T("Empty Queue"));
		}
		m_queue.erase(m_queue.begin());
	}

	template <typename DataType>
	ThreadSafeQueue<DataType> & ThreadSafeQueue<DataType>::operator=(const ThreadSafeQueue& b)
	{
		if(this != &b)
		{
			LockObj lock(m_queueLock);
			m_queue=b.m_queue;
		}
		return *this;
	}
}
#endif //__EP_THREAD_SAFE_QUEUE_H__