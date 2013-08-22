/*! 
@file epThreadSafePQueue.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date September 02, 2010
@brief ThreadSafe Priority Queue Interface
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

An Interface for Thread Safe Priority Queue.

*/
#ifndef __EP_THREAD_SAFE_PQUEUE_H__
#define __EP_THREAD_SAFE_PQUEUE_H__
#include "epLib.h"
#include "epThreadSafeQueue.h"
#include "epBinarySearch.h"

namespace epl
{
	/*! 
	@class ThreadSafePQueue epThreadSafePQueue.h
	@brief A class for Thread Safe Priority Queue.
	*/
	template <typename DataType, typename Compare=CompClass<DataType> >
	class ThreadSafePQueue:public ThreadSafeQueue<DataType>
	{
	public:
		/*!
		Default Constructor

		Initializes the priority queue
		@param[in] lockPolicyType The lock policy
		*/
		ThreadSafePQueue(LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the queue
		@param[in] b the second object
		*/
		ThreadSafePQueue(const ThreadSafePQueue& b);

		/*!
		Default Destructor

		Destroy the priority queue
		*/
		virtual ~ThreadSafePQueue();

		/*!
		Assignment Operator Overloading

		the ThreadSafeQueue set as given ThreadSafeQueue b
		@param[in] b right side of ThreadSafeQueue
		@return this object
		*/
		ThreadSafePQueue & operator=(const ThreadSafePQueue&b);

		/*!
		Insert the new item into the priority queue.
		@param[in] data The inserting data.
		*/
		virtual void Push(DataType const &data);

	};

	template <typename DataType, typename Compare>
	ThreadSafePQueue<DataType,Compare>::ThreadSafePQueue(LockPolicy lockPolicyType) :ThreadSafeQueue<DataType>(lockPolicyType)
	{
	}
	template <typename DataType, typename Compare>
	ThreadSafePQueue<DataType,Compare>::ThreadSafePQueue(const ThreadSafePQueue& b):ThreadSafeQueue<DataType>(b)
	{
	}
	template <typename DataType, typename Compare>
	ThreadSafePQueue<DataType,Compare>::~ThreadSafePQueue()
	{
	}
	
	template <typename DataType, typename Compare>
	ThreadSafePQueue<DataType,Compare> & ThreadSafePQueue<DataType,Compare>::operator=(const ThreadSafePQueue&b)
	{
		if(this != &b)
		{
			ThreadSafeQueue::operator =(b);
		}
		return *this;
	}

	template <typename DataType, typename Compare>
	void ThreadSafePQueue<DataType,Compare>::Push(DataType const & data)
	{
		LockObj lock(m_queueLock);
		DataType* retVal=NULL;
		if(m_queue.size())
		{
			size_t retIdx;
			if(BinarySearch(data,&m_queue.at(0),m_queue.size(),Compare::CompFunc,retIdx))
				EP_ASSERT_EXPR(0,_T("Same Object already in the Queue!!"));
			m_queue.insert(m_queue.begin()+retIdx,data);
		}
		else
		{
			m_queue.push_back(data);
		}

	}
}


#endif //__EP_THREAD_SAFE_PQUEUE_H__