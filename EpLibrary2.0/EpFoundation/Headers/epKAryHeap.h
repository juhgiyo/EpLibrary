/*! 
@file epKAryHeap.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date June 09, 2008
@brief K-ary Heap Interface
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

An Interface for K-ary Heap Template class.

*/
#ifndef __EP_KARYHEAP_H__
#define __EP_KARYHEAP_H__
#include "epFoundationLib.h"
#include "epDynamicArray.h"
#include "epException.h"
namespace epl
{
	/*! 
	@class KAryHeap epKAryHeap.h
	@brief A K-ary Heap Template class.
	*/

	template <typename FKEY,typename FDATA, unsigned int k=5, CompResultType (*KeyCompareFunc)(const void *,const void *)=CompClass<FKEY>::CompFunc >
	class KAryHeap
	{
	public:
		/*!
		Default Constructor

		Initializes the K-ary Heap
		@param[in] lockPolicyType The lock policy
		*/
		KAryHeap(LockPolicy lockPolicyType=EP_LOCK_POLICY);
		
		/*!
		Default Constructor

		Initializes the K-ary Heap with given heap
		@param[in] b the K-ary Heap Object to copy from
		*/
		KAryHeap(const KAryHeap & b);

		/*!
		Default Destructor

		Destroys the K-ary Heap
		*/
		virtual ~KAryHeap();
		

		/*!
		Initialize this heap to given heap
		@param[in] b the K-ary Heap structure to initialize this heap
		@return the result K-ary Heap
		*/
		KAryHeap &operator=(const KAryHeap & b);

		/*!
		Return the reference to data with given key if not exist insert and return the reference to data
		@param[in] key The key to return the reference to the data.
		@return the reference to the data with given key.
		*/
		FDATA &operator[](const FKEY & key);

		/*!
		Remove the minimum of heap
		@param[out] retKey The minimum key value of the heap.
		@param[out] retData the data with the minimum key of the heap
		@return true if succeeded otherwise false
		*/
		bool Pop( FKEY &retKey, FDATA &retData );

		/*!
		Insert the key with given data to the heap
		@param[in] key The key value to insert.
		@param[in] data the data with the given key
		@return true if succeeded otherwise false
		*/
		bool Push(const FKEY &key, const FDATA &data);

		/*!
		Return the minimum of heap
		@param[out] retKey The minimum key value of the heap.
		@param[out] retData the data with the minimum key of the heap
		@return true if succeeded otherwise false
		*/
		bool Front(FKEY &retKey, FDATA &retData );

		/*!
		Change the given key to given new key.
		@param[in] key The original key.
		@param[in] newKey the new key for the given original key
		@return true if succeeded otherwise false
		*/
		bool ChangeKey(const FKEY &key, const FKEY &newKey);

		/*!
		Clear the heap
		*/
		void Clear();

		/*!
		Erase the given key from the heap.
		@param[in] key The key to remove.
		@return true if succeeded otherwise false
		*/
		bool Erase(const FKEY &key);

		/*!
		Check if the heap is empty
		@return true if heap is empty otherwise false
		*/
		bool IsEmpty() const;


		/*!
		return the number of element in the heap.
		@return the number of element in the heap
		*/
		unsigned int Size() const;
			
		

	protected:

		/*!
		Actually remove the minimum of heap
		@param[out] retMin The minimum key and data value pair of the heap.
		@return true if succeeded otherwise false
		*/
		bool pop( Pair<FKEY,FDATA> &retMin );

		/*!
		Actually insert the key with given data to the heap
		@param[in] key The key value to insert.
		@param[in] data the data with the given key
		@return true if succeeded otherwise false
		*/
		void push(const FKEY &key, const FDATA &data);

		/*!
		Actually change the key of the node with given index with new key.
		@param[in] index The index of the node to change the key.
		@param[in] newKey the new key for the given original key
		@return true if succeeded otherwise false
		*/
		void changeKey(int index, const FKEY &newKey);

		/*!
		Actually erase the given key from the heap.
		@param[in] index The index of the node to remove.
		@return true if succeeded otherwise false
		*/
		void erase(int index);
		
		/*!
		Heapify up the node with given index
		@param[in] idx The index for the node to heapify up.
		*/
		void heapifyUp(int idx);

		/*!
		Heapify down the node with given index
		@param[in] idx The index for the node to heapify down.
		*/
		void heapifyDown(int idx);

		/*!
		Return the minimum child of the given node index
		@param[in] parentIdx The index for the node to find minimum child.
		@remark return -1 if there is no child for the node with given index
		@return the index of the minimum child
		*/
		int findMinChild(int parentIdx);

		/*!
		Return the index of parent
		@param[in] childIdx The index for the node to find parent index.
		@remark return -1 if there is no parent for the node with given index
		@return the index of the parent
		*/
		int getParentIDX(int childIdx) const;

		/*!
		Find the node index with given key
		@param[in] key The key of the node to find index.
		@param[in] rootIdx the root index to start from
		@remark return -1 if there is no node with the given key
		@return the index of the found node
		*/
		int findIndex(const FKEY &key, int rootIdx=0);

		///  K-ary heap
		DynamicArray<Pair<FKEY,FDATA> *> m_heap;
		/// the actual heap size
		unsigned int m_heapSize;
		/// lock
		BaseLock *m_lock;
		/// Lock Policy
		LockPolicy m_lockPolicy;

	};

	template <typename FKEY,typename FDATA,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *)>  
	KAryHeap<FKEY,FDATA,k,KeyCompareFunc>::KAryHeap(LockPolicy lockPolicyType)
	{
		EP_VERIFY_DOMAIN_ERROR_W_MSG(k>0,"Template Declaration Error: k cannnot be less than/equal to 0");

		m_heap=DynamicArray<Pair<FKEY,FDATA> *>(lockPolicyType);
		m_heapSize=0;
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
		}

	}

	template <typename FKEY,typename FDATA,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *)>  
	KAryHeap<FKEY,FDATA,k,KeyCompareFunc>::KAryHeap(const KAryHeap<FKEY,FDATA,k,KeyCompareFunc> & b)
	{
		EP_VERIFY_DOMAIN_ERROR_W_MSG(k>0,"Template Declaration Error: k cannnot be less than/equal to 0");
		m_lockPolicy=b.m_lockPolicy;
		m_heap.Resize(b.m_heap.Size());
		for(int trav=0;trav<b.m_heap.Size();trav++)
		{
			m_heap[trav]=EP_NEW Pair<FKEY,FDATA>();
			m_heap[trav]->first=b.m_heap[trav]->first;
			m_heap[trav]->second=b.m_heap[trav]->second;

		}
		m_heapSize=b.m_heapSize;
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

	template <typename FKEY,typename FDATA,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	KAryHeap<FKEY,FDATA,k,KeyCompareFunc>::~KAryHeap()
	{
		for(int trav=0;trav<m_heapSize;trav++)
		{
			if(m_heap[trav])	
				EP_DELETE m_heap[trav];

		}
		m_heap.Delete();
		if(m_lock)
			EP_DELETE m_lock;
	}


	template <typename FKEY,typename FDATA,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *)>  
	KAryHeap<FKEY,FDATA,k,KeyCompareFunc> &KAryHeap<FKEY,FDATA,k,KeyCompareFunc>::operator=(const KAryHeap<FKEY,FDATA,k,KeyCompareFunc> & b)
	{
		if(this!=&b)
		{
			LockObj lock(m_lock);
			m_heap.Resize(b.m_heap.Size());
			for(int trav=0;trav<b.m_heap.Size();trav++)
			{
				m_heap[trav]=EP_NEW Pair<FKEY,FDATA>();
				m_heap[trav]->first=b.m_heap[trav]->first;
				m_heap[trav]->second=b.m_heap[trav]->second;

			}
			m_heapSize=b.m_heapSize;
		}
		return *this;
	}
	
	template <typename FKEY,typename FDATA,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *)>  
	FDATA &KAryHeap<FKEY,FDATA,k,KeyCompareFunc>::operator[](const FKEY & key)
	{
		LockObj lock(m_lock);
		int idx=findIndex(key, 0);
		if(idx>=0)
			return m_heap[idx]->second;
		push(key,reinterpret_cast<FDATA>(0));
		idx=findIndex(key,0);
		if(idx>=0)
			return m_heap[idx]->second;
		EP_VERIFY_DOMAIN_ERROR(0);
	}

	template <typename FKEY,typename FDATA,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	bool KAryHeap<FKEY,FDATA,k,KeyCompareFunc>::Pop( FKEY &retKey, FDATA &retData )
	{
		LockObj lock(m_lock);
		Pair<FKEY,FDATA> retMin;
		if(pop(retMin))
		{
			retKey=retMin.first;
			retData=retMin.second;
			return true;
		}
		return false;
				
	}

	template <typename FKEY,typename FDATA,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	bool KAryHeap<FKEY,FDATA,k,KeyCompareFunc>::Front( FKEY &retKey, FDATA &retData )
	{		
		LockObj lock(m_lock);
		if(m_heapSize>0)
		{
			retKey=m_heap[0]->first;
			retData=m_heap[0]->second;
			return true;
		}
		return false;
	}


	template <typename FKEY,typename FDATA,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *)>  
	bool KAryHeap<FKEY,FDATA,k,KeyCompareFunc>::ChangeKey(const FKEY &key, const FKEY &newKey)
	{
		LockObj lock(m_lock);
		int idx=findIndex(key, 0);
		if(idx>=0)
		{
			changeKey(idx,newKey);
			return true;
		}
		return false;
	}

	template <typename FKEY,typename FDATA,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	void KAryHeap<FKEY,FDATA,k,KeyCompareFunc>::Clear()
	{
		for(int trav=0;trav<m_heapSize;trav++)
		{
			if(m_heap[trav])	
				EP_DELETE m_heap[trav];
			m_heap[trav]=NULL;
		}
		m_heapSize=0;
	}

	template <typename FKEY,typename FDATA,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *)>  
	bool KAryHeap<FKEY,FDATA,k,KeyCompareFunc>::Erase(const FKEY &key)
	{
		LockObj lock(m_lock);
		int idx=findIndex(key,0);
		if(idx>=0)
		{
			erase(key);
		}
		return false;
	}
	template <typename FKEY,typename FDATA,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	bool KAryHeap<FKEY,FDATA,k,KeyCompareFunc>::IsEmpty() const
	{
		LockObj lock(m_lock);
		if(m_heapSize)
			return true;
		return false;
	}

	template <typename FKEY,typename FDATA,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	unsigned int KAryHeap<FKEY,FDATA,k,KeyCompareFunc>::Size() const
	{
		LockObj lock(m_lock);
		return m_heapSize;
	}



	template <typename FKEY,typename FDATA,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	bool KAryHeap<FKEY,FDATA,k,KeyCompareFunc>::Push(const FKEY &key, const FDATA &data)
	{	
		LockObj lock(m_lock);
		if(findIndex(key,0)==-1)
		{
			push(key,data);
			return true;
		}
		return false;
	}



	template <typename FKEY,typename FDATA,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	bool KAryHeap<FKEY,FDATA,k,KeyCompareFunc>::pop( Pair<FKEY,FDATA> &retMin )
	{
		if(m_heap.Size()> 0 && m_heapSize >0)
		{
			retMin.first=m_heap[0]->first;
			retMin.second=m_heap[0]->second;
			EP_DELETE m_heap[0];
			m_heap[0]= m_heap[m_heapSize-1];
			m_heap[m_heapSize-1]=NULL;
			m_heapSize--;
			heapifyDown(0);
			return true;
		}
		return false;
	}

	template <typename FKEY,typename FDATA,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	void KAryHeap<FKEY,FDATA,k,KeyCompareFunc>::push(const FKEY &key, const FDATA &data)
	{
	
		if(m_heapSize+1>m_heap.Size())
		{
			long newSize=m_heap.Size()+m_heap.Size()/2;
			newSize=(m_heap.Size()==newSize)?m_heap.Size()+1:newSize;
			m_heap.Resize(newSize);
		}
		m_heap[m_heapSize]=EP_NEW Pair<FKEY,FDATA>(key,data);
		m_heapSize++;
		heapifyUp(m_heapSize-1);

	}

	template <typename FKEY,typename FDATA,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	void KAryHeap<FKEY,FDATA,k,KeyCompareFunc>::changeKey(int index, const FKEY &newKey)
	{

		if(index<m_heapSize)
		{
			if(KeyCompareFunc(&(m_heap[index]->first),&newKey)==COMP_RESULT_LESSTHAN)
			{	
				m_heap[index]->first=newKey;
				heapifyDown(index);
			}
			else
			{
				m_heap[index]->first=newKey;
				heapifyUp(index);
			}
		}
	}

	template <typename FKEY,typename FDATA,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	void KAryHeap<FKEY,FDATA,k,KeyCompareFunc>::erase(int index)
	{

		if(index<m_heapSize)
		{
			Pair<FKEY,FDATA> * tmp=m_heap[m_heapSize-1];
			m_heap[m_heapSize-1]=m_heap[index];
			m_heap[index]=tmp;
			EP_DELETE m_heap[m_heapSize-1];
			m_heap[m_heapSize-1]=NULL;
			m_heapSize--;
			heapifyDown(index);
		}
	}



	template <typename FKEY,typename FDATA,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	void KAryHeap<FKEY,FDATA,k,KeyCompareFunc>::heapifyUp(int idx)
	{
		if(m_heap.Size()>0)
		{
			int currentIdx=idx;//size-1;
			int parentIdx=getParentIDX(currentIdx);
			Pair<FKEY,FDATA> *tmp;
			while(parentIdx!=-1)
			{
				if(KeyCompareFunc(&(m_heap[parentIdx]->first) ,&(m_heap[currentIdx]->first))==COMP_RESULT_GREATERTHAN)
				{
					tmp = m_heap[currentIdx];
					m_heap[currentIdx]=m_heap[parentIdx];
					m_heap[parentIdx]=tmp;
					currentIdx=parentIdx;
					parentIdx=getParentIDX(currentIdx);
				}
				else
				{
					break;
				}
			}
		}
	}

	template <typename FKEY,typename FDATA,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	void KAryHeap<FKEY,FDATA,k,KeyCompareFunc>::heapifyDown(int idx)
	{
		if(m_heap.Size()>0)
		{
			
			int parentIdx=idx;//0;
			int minChildIdx=findMinChild(parentIdx);
			Pair<FKEY,FDATA> *tmp;
			while(minChildIdx!=-1)
			{
				if(KeyCompareFunc(&(m_heap[parentIdx]->first ), &(m_heap[minChildIdx]->first))!=COMP_RESULT_LESSTHAN)
				{
					tmp = m_heap[parentIdx];
					m_heap[parentIdx]=m_heap[minChildIdx];
					m_heap[minChildIdx]=tmp;
					parentIdx= minChildIdx;
					minChildIdx=findMinChild(parentIdx);
				}
				else
				{
					break;
				}
			}
		}
	}

	template <typename FKEY,typename FDATA,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	int KAryHeap<FKEY,FDATA,k,KeyCompareFunc>::findMinChild(int parentIdx)
	{
		int arrTrav;
		if(k*parentIdx+1<m_heapSize)
		{
			Pair<FKEY,FDATA> *minNode=m_heap[k*parentIdx+1];
			int minIdx=k*parentIdx+1;
			for(arrTrav=k*parentIdx+2; arrTrav<=k*parentIdx+k;arrTrav++)
			{
				if(arrTrav<m_heapSize && KeyCompareFunc(&(minNode->first) , &(m_heap[arrTrav]->first))==COMP_RESULT_GREATERTHAN)
				{
					minNode=m_heap[arrTrav];
					minIdx=arrTrav;
				}
			}
			return minIdx;
		}
		return -1;

	}


	template <typename FKEY,typename FDATA,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	int KAryHeap<FKEY,FDATA,k,KeyCompareFunc>::findIndex(const FKEY &key, int rootIdx)
	{
		if(rootIdx<m_heapSize)
		{
			Pair<FKEY,FDATA> *pair=m_heap[rootIdx];

			if(KeyCompareFunc(&(pair->first) ,&(key))==COMP_RESULT_GREATERTHAN)
			{
				return -1;
			}
			else if(KeyCompareFunc(&(pair->first), &(key))==COMP_RESULT_EQUAL)
			{	
				return rootIdx;
			}

			int trav;
			for(trav=1; trav<=k; trav++)
			{
				if(rootIdx*k+trav<m_heapSize)
				{
					int retIdx = findIndex(key,rootIdx*k+trav);
					if(retIdx!=-1)
					{
						return retIdx;
					}
				}
				else
					return -1;
			}
			return -1;


		}
		else
		{
			return -1;
		}
	}

	template <typename FKEY,typename FDATA,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	int KAryHeap<FKEY,FDATA,k,KeyCompareFunc>::getParentIDX(int childIdx) const
	{
		if(childIdx%k ==0)
		{
			return childIdx/k-1;
		}
		else
		{
			return childIdx/k;
		}
	}
}
#endif //__EP_KARYHEAP_H__
