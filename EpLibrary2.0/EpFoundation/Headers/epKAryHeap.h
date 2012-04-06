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
	template <typename KeyType,typename DataType, unsigned int k=5, CompResultType (*KeyCompareFunc)(const void *,const void *)=CompClass<KeyType>::CompFunc >
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
		Default Copy Constructor

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
		DataType &operator[](const KeyType & key);

		/*!
		Remove the minimum of heap
		@param[out] retKey The minimum key value of the heap.
		@param[out] retData the data with the minimum key of the heap
		@return true if succeeded otherwise false
		*/
		bool Pop( KeyType &retKey, DataType &retData );

		/*!
		Insert the key with given data to the heap
		@param[in] key The key value to insert.
		@param[in] data the data with the given key
		@return true if succeeded otherwise false
		*/
		DataType &Push(const KeyType &key, const DataType &data);

		/*!
		Return the minimum of heap
		@param[out] retKey The minimum key value of the heap.
		@param[out] retData the data with the minimum key of the heap
		@return true if succeeded otherwise false
		*/
		bool Front(KeyType &retKey, DataType &retData );

		/*!
		Return the minimum of heap
		@remark raises the exception when heap is empty.
		@return the minimum pair with key and data of heap
		*/
		Pair<KeyType,DataType> Front();

		/*!
		Get the data of the node with given key.
		@param[in] key The index of the node to get the data.
		@param[out] retData The data of the node with given key.
		@return true if succeeded otherwise false.
		*/
		bool GetData(const KeyType &key,DataType &retData);
		
		/*!
		Get the data of the node with given key.
		@param[in] key The index of the node to get the data.
		@remark raises the exception when key does not exist.
		@return data of the node with given key
		*/
		DataType &GetData(const KeyType &key);


		/*!
		Change the data of the node with given key with new data.
		@param[in] key The index of the node to change the data.
		@param[in] newData the new data for the given key
		@return true if succeeded otherwise false
		*/
		bool ChangeData(const KeyType &key, const DataType &newData);

		/*!
		Change the given key to given new key.
		@param[in] key The original key.
		@param[in] newKey the new key for the given original key
		@return true if succeeded otherwise false
		*/
		bool ChangeKey(const KeyType &key, const KeyType &newKey);

		/*!
		Clear the heap
		*/
		void Clear();

		/*!
		Erase the given key from the heap.
		@param[in] key The key to remove.
		@return true if succeeded otherwise false
		*/
		bool Erase(const KeyType &key);

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
		bool pop( Pair<KeyType,DataType> &retMin );

		/*!
		Actually insert the key with given data to the heap
		@param[in] key The key value to insert.
		@param[in] data the data with the given key
		*/
		void push(const KeyType &key, const DataType &data);

		/*!
		Actually change the key of the node with given index with new key.
		@param[in] index The index of the node to change the key.
		@param[in] newKey the new key for the given original key
		*/
		void changeKey(int index, const KeyType &newKey);

		/*!
		Actually change the data of the node with given index with new data.
		@param[in] index The index of the node to change the data.
		@param[in] newData the new data for the given key
		*/
		void changeData(int index, const DataType &newData);

		/*!
		Actually erase the given key from the heap.
		@param[in] index The index of the node to remove.
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
		int getParentIdx(int childIdx) const;

		/*!
		Find the node index with given key
		@param[in] key The key of the node to find index.
		@param[in] rootIdx the root index to start from
		@remark return -1 if there is no node with the given key
		@return the index of the found node
		*/
		int findIndex(const KeyType &key, int rootIdx=0);

		///  K-ary heap
		DynamicArray<Pair<KeyType,DataType> *> m_heap;
		/// the actual heap size
		unsigned int m_heapSize;
		/// lock
		BaseLock *m_lock;
		/// Lock Policy
		LockPolicy m_lockPolicy;

	};

	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *)>  
	KAryHeap<KeyType,DataType,k,KeyCompareFunc>::KAryHeap(LockPolicy lockPolicyType)
	{
		EP_VERIFY_DOMAIN_ERROR_W_MSG(k>0,"Template Declaration Error: k cannnot be less than/equal to 0");

		m_heap=DynamicArray<Pair<KeyType,DataType> *>(lockPolicyType);
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

	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *)>  
	KAryHeap<KeyType,DataType,k,KeyCompareFunc>::KAryHeap(const KAryHeap<KeyType,DataType,k,KeyCompareFunc> & b)
	{
		EP_VERIFY_DOMAIN_ERROR_W_MSG(k>0,"Template Declaration Error: k cannnot be less than/equal to 0");
		m_lockPolicy=b.m_lockPolicy;
		m_heap.Resize(b.m_heap.Size());
		for(int trav=0;trav<b.m_heap.Size();trav++)
		{
			m_heap[trav]=EP_NEW Pair<KeyType,DataType>();
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

	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	KAryHeap<KeyType,DataType,k,KeyCompareFunc>::~KAryHeap()
	{
		m_lock->Lock();
		for(int trav=0;trav<m_heapSize;trav++)
		{
			if(m_heap[trav])	
				EP_DELETE m_heap[trav];

		}
		m_heap.Delete();
		m_lock->Unlock();
		if(m_lock)
			EP_DELETE m_lock;
	}


	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *)>  
	KAryHeap<KeyType,DataType,k,KeyCompareFunc> &KAryHeap<KeyType,DataType,k,KeyCompareFunc>::operator=(const KAryHeap<KeyType,DataType,k,KeyCompareFunc> & b)
	{
		if(this!=&b)
		{
			LockObj lock(m_lock);
			m_heap.Resize(b.m_heap.Size());
			for(int trav=0;trav<b.m_heap.Size();trav++)
			{
				m_heap[trav]=EP_NEW Pair<KeyType,DataType>();
				m_heap[trav]->first=b.m_heap[trav]->first;
				m_heap[trav]->second=b.m_heap[trav]->second;

			}
			m_heapSize=b.m_heapSize;
		}
		return *this;
	}
	
	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *)>  
	DataType &KAryHeap<KeyType,DataType,k,KeyCompareFunc>::operator[](const KeyType & key)
	{
		LockObj lock(m_lock);
		int idx=findIndex(key, 0);
		if(idx>=0)
			return m_heap[idx]->second;
		push(key,reinterpret_cast<DataType>(0));
		idx=findIndex(key,0);
		EP_VERIFY_DOMAIN_ERROR(idx>=0);
		return m_heap[idx]->second;
		
	}

	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *)>  
	DataType &KAryHeap<KeyType,DataType,k,KeyCompareFunc>::GetData(const KeyType &key)
	{
		LockObj lock(m_lock);
		int idx=findIndex(key, 0);
		EP_VERIFY_OUT_OF_RANGE_W_MSG(idx>=0,"The given key does not exist in the heap");
		return m_heap[idx]->second;
		

	}
	
	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *)>  
	bool KAryHeap<KeyType,DataType,k,KeyCompareFunc>::GetData(const KeyType &key,DataType &retData)
	{
		LockObj lock(m_lock);
		int idx=findIndex(key, 0);
		if(idx>=0)
		{
			retData=m_heap[idx]->second;
			return true;
		}
		return false;
	}

	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	bool KAryHeap<KeyType,DataType,k,KeyCompareFunc>::Pop( KeyType &retKey, DataType &retData )
	{
		LockObj lock(m_lock);
		Pair<KeyType,DataType> retMin;
		if(pop(retMin))
		{
			retKey=retMin.first;
			retData=retMin.second;
			return true;
		}
		return false;
				
	}

	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	bool KAryHeap<KeyType,DataType,k,KeyCompareFunc>::Front( KeyType &retKey, DataType &retData )
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


	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *)>  
	Pair<KeyType,DataType> KAryHeap<KeyType,DataType,k,KeyCompareFunc>::Front()
	{
		LockObj lock(m_lock);
		EP_VERIFY_OUT_OF_RANGE_W_MSG(m_heapSize>0,"The heap is empty.");
		return Pair(m_heap[0]->first,m_heap[0]->second);
	}

	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *)>  
	bool KAryHeap<KeyType,DataType,k,KeyCompareFunc>::ChangeKey(const KeyType &key, const KeyType &newKey)
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

	
	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	bool KAryHeap<KeyType,DataType,k,KeyCompareFunc>::ChangeData(const KeyType &key, const DataType &newData)
	{
		LockObj lock(m_lock);
		int idx=findIndex(key, 0);
		if(idx>=0)
		{
			changeData(idx,newData);
			return true;
		}
		return false;
	}

	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	void KAryHeap<KeyType,DataType,k,KeyCompareFunc>::Clear()
	{
		for(int trav=0;trav<m_heapSize;trav++)
		{
			if(m_heap[trav])	
				EP_DELETE m_heap[trav];
			m_heap[trav]=NULL;
		}
		m_heapSize=0;
	}

	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *)>  
	bool KAryHeap<KeyType,DataType,k,KeyCompareFunc>::Erase(const KeyType &key)
	{
		LockObj lock(m_lock);
		int idx=findIndex(key,0);
		if(idx>=0)
		{
			erase(key);
		}
		return false;
	}
	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	bool KAryHeap<KeyType,DataType,k,KeyCompareFunc>::IsEmpty() const
	{
		LockObj lock(m_lock);
		if(m_heapSize)
			return true;
		return false;
	}

	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	unsigned int KAryHeap<KeyType,DataType,k,KeyCompareFunc>::Size() const
	{
		LockObj lock(m_lock);
		return m_heapSize;
	}



	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	DataType &KAryHeap<KeyType,DataType,k,KeyCompareFunc>::Push(const KeyType &key, const DataType &data)
	{	
		LockObj lock(m_lock);
		int index=findIndex(key,0);
		EP_VERIFY_INVALID_ARGUMENT_W_MSG(index==-1,"Given key already exists in the K-ary heap. Duplicated insertion is not allowed.");
		push(key,data);
		index=findIndex(key,0);
		return m_heap[index]->second;
	}



	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	bool KAryHeap<KeyType,DataType,k,KeyCompareFunc>::pop( Pair<KeyType,DataType> &retMin )
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

	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	void KAryHeap<KeyType,DataType,k,KeyCompareFunc>::push(const KeyType &key, const DataType &data)
	{
	
		if(m_heapSize+1>m_heap.Size())
		{
			long newSize=m_heap.Size()+m_heap.Size()/2;
			newSize=(m_heap.Size()==newSize)?m_heap.Size()+1:newSize;
			m_heap.Resize(newSize);
		}
		m_heap[m_heapSize]=EP_NEW Pair<KeyType,DataType>(key,data);
		m_heapSize++;
		heapifyUp(m_heapSize-1);

	}

	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	void KAryHeap<KeyType,DataType,k,KeyCompareFunc>::changeKey(int index, const KeyType &newKey)
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

	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	void KAryHeap<KeyType,DataType,k,KeyCompareFunc>::changeData(int index, const DataType &newData)
	{

		if(index<m_heapSize)
		{
			m_heap[index]->second=data;
		}
	}

	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	void KAryHeap<KeyType,DataType,k,KeyCompareFunc>::erase(int index)
	{

		if(index<m_heapSize)
		{
			Pair<KeyType,DataType> * tmp=m_heap[m_heapSize-1];
			m_heap[m_heapSize-1]=m_heap[index];
			m_heap[index]=tmp;
			EP_DELETE m_heap[m_heapSize-1];
			m_heap[m_heapSize-1]=NULL;
			m_heapSize--;
			heapifyDown(index);
		}
	}



	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	void KAryHeap<KeyType,DataType,k,KeyCompareFunc>::heapifyUp(int idx)
	{
		if(m_heap.Size()>0)
		{
			int currentIdx=idx;//size-1;
			int parentIdx=getParentIdx(currentIdx);
			Pair<KeyType,DataType> *tmp;
			while(parentIdx!=-1)
			{
				if(KeyCompareFunc(&(m_heap[parentIdx]->first) ,&(m_heap[currentIdx]->first))==COMP_RESULT_GREATERTHAN)
				{
					tmp = m_heap[currentIdx];
					m_heap[currentIdx]=m_heap[parentIdx];
					m_heap[parentIdx]=tmp;
					currentIdx=parentIdx;
					parentIdx=getParentIdx(currentIdx);
				}
				else
				{
					break;
				}
			}
		}
	}

	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	void KAryHeap<KeyType,DataType,k,KeyCompareFunc>::heapifyDown(int idx)
	{
		if(m_heap.Size()>0)
		{
			
			int parentIdx=idx;//0;
			int minChildIdx=findMinChild(parentIdx);
			Pair<KeyType,DataType> *tmp;
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

	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	int KAryHeap<KeyType,DataType,k,KeyCompareFunc>::findMinChild(int parentIdx)
	{
		int arrTrav;
		if(k*parentIdx+1<m_heapSize)
		{
			Pair<KeyType,DataType> *minNode=m_heap[k*parentIdx+1];
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


	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	int KAryHeap<KeyType,DataType,k,KeyCompareFunc>::findIndex(const KeyType &key, int rootIdx)
	{
		if(rootIdx<m_heapSize)
		{
			Pair<KeyType,DataType> *pair=m_heap[rootIdx];

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

	template <typename KeyType,typename DataType,unsigned int k, CompResultType (*KeyCompareFunc)(const void *,const void *) >  
	int KAryHeap<KeyType,DataType,k,KeyCompareFunc>::getParentIdx(int childIdx) const
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
