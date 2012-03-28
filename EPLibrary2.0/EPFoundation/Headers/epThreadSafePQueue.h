/*! 
@file epThreadSafePQueue.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date September 02, 2010
@brief A ThreadSafePQueue Interface
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
	template <typename FDATA, typename Compare=CompClass<FDATA> >
	class ThreadSafePQueue:public ThreadSafeQueue<FDATA>
	{
	public:
		/*!
		Default Constructor

		Initializes the priority queue
		*/
		ThreadSafePQueue();

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
		virtual void Push(FDATA const &data);

	};

	template <typename FDATA, typename Compare>
	ThreadSafePQueue<FDATA,Compare>::ThreadSafePQueue() :ThreadSafeQueue<FDATA>()
	{
	}
	template <typename FDATA, typename Compare>
	ThreadSafePQueue<FDATA,Compare>::ThreadSafePQueue(const ThreadSafePQueue& b)
	{
		m_queue=b.m_queue;
#ifdef EP_MULTIPROCESS
		m_queueLock=EP_NEW Mutex();
#else //EP_MULTIPROCESS
		m_queueLock=EP_NEW CriticalSectionEx();
#endif //EP_MULTIPROCESS
	}
	template <typename FDATA, typename Compare>
	ThreadSafePQueue<FDATA,Compare>::~ThreadSafePQueue()
	{
	}
	
	template <typename FDATA, typename Compare>
	ThreadSafePQueue<FDATA,Compare> & ThreadSafePQueue<FDATA,Compare>::operator=(const ThreadSafePQueue&b)
	{
		if(this != &b)
		{
			m_queue=b.m_queue;
		}
		return *this;
	}

	template <typename FDATA, typename Compare>
	void ThreadSafePQueue<FDATA,Compare>::Push(FDATA const & data)
	{
		LockObj lock(m_queueLock);
		FDATA* retVal=NULL;
		if(m_queue.size())
		{
			int retIdx;
			if(BinarySearch(data,&m_queue.at(0),m_queue.size(),Compare::CompFunc,retIdx))
				EP_WASSERT(0,_T("Same Object already in the Queue!!"));
			m_queue.insert(m_queue.begin()+retIdx,data);
		}
		else
		{
			m_queue.push_back(data);
		}

	}
}


#endif //__EP_THREAD_SAFE_PQUEUE_H__