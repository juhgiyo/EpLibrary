/*! 
@file epPacketContainer.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com> 
		
@date March 27, 2012
@brief A Packet Container Interface
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

An Interface for Packet Container with variable length array.

@secion SPECIAL THANKS TO
 
Sang Yong Yoon <flyingshiri@gmail.com>

*/
#ifndef __EP_PACKET_CONTAINER_H__
#define __EP_PACKET_CONTAINER_H__

#include "epFoundationLib.h"
#include "epCriticalSectionEx.h"
#include "epMutex.h"
#include "epNoLock.h"

namespace epl
{
	/*! 
	@class PacketContainer epPacketContainer.h
	@brief A class for Packet Container with variable length array.
	*/
	template<typename PacketStruct, typename ArrayType=char>
	class PacketContainer
	{
	public:
		/*!
		Default Constructor

		Initializes given size of array of ArrayType addition to the PacketStruct.
		@param[in] arraySize the size of array addition to PacketStruct
		@param[in] shouldAllocate flag for the allocation of memory for itself
		@param[in] lockPolicyType The lock policy
		*/
		PacketContainer(unsigned int arraySize=0, bool shouldAllocate=true, LockPolicy lockPolicyType=EP_LOCK_POLICY);
		

		/*!
		Default Constructor

		Initializes given size of array of ArrayType addition to the PacketStruct.
		@param[in] packet the packet to copy from
		@param[in] arraySize the size of array addition to PacketStruct
		@param[in] shouldAllocate flag for the allocation of memory for itself
		@param[in] lockPolicyType The lock policy
		*/
		PacketContainer(const PacketStruct & packet, unsigned int arraySize=0, bool shouldAllocate=true, LockPolicy lockPolicyType=EP_LOCK_POLICY);
		
		/*!
		Default Constructor

		Initializes PacketContainer with the raw data given.
		@param[in] rawData the rawData to copy from
		@param[in] byteSize the byte size of raw data given.
		@param[in] shouldAllocate flag for the allocation of memory for itself
		@param[in] lockPolicyType The lock policy
		*/
		PacketContainer(void * rawData, unsigned int byteSize, bool shouldAllocate=true, LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Copy Constructor

		Copy the given Packet Container to this Packet Container.
		@param[in] orig the original PacketContainer
		*/
		PacketContainer(const PacketContainer& orig);
		
		/*!
		Default Destructor

		Destroy the Packet
		*/
		virtual ~PacketContainer();
				

		/*!
		Get a pointer to the packet
		@return the pointer to the packet
		*/
		PacketStruct *GetPacketPtr() const;
		

		/*!
		Get the flag whether memory is allocated or not
		@return true if the memory is allocated by this object, otherwise false
		*/
		bool IsAllocated() const;
		
		/*!
		Set the packet container according to the packet given
		@param[in] packet the packet to copy from
		@param[in] arraySize the size of variable length array of given packet
		*/
		void SetPacket(const PacketStruct & packet, unsigned int arraySize=0);

		/*!
		Set the packet container from the raw data
		@param[in] rawData the rawData to copy from
		@param[in] byteSize the byte size of raw data given.
		@return true if successful otherwise false
		*/
		bool SetPacket(void * rawData, unsigned int byteSize);

		/*!
		Get the pointer to the array
		@return the pointer to the array
		*/
		ArrayType* GetArray() const;

		/*!
		Set the array with given array
		@param[in] arr the array to copy from
		@param[in] arraySize the size of given array
		@param[in] offset the offset for start position of copying
		@return true if successful otherwise false
		*/
		bool SetArray(ArrayType *arr,unsigned int arraySize, unsigned int offset=0);
		
		/*!
		Get the length of the array
		@return the length of the array
		*/
		unsigned int GetArrayLength() const;

		/*!
		Get the total byte size of the packet including the array
		@return the total byte size of the packet including the array
		*/
		unsigned int GetPacketByteSize() const;

		/*!
		Change the size of the array
		* the arrSize must be larger than current array size.
		* Should NOT be used when m_isAllocated is false.
		@param[in] arrSize the new size of the array
		@return true if successful otherwise false
		*/
		bool SetArraySize(unsigned int arrSize);

		/*!
		return the reference to the array element at given index
		* the arrSize must be smaller than current array size.
		@param[in] index the index of the array to get the array element
		@return the reference to the array element at given index
		*/
		ArrayType & operator[](unsigned int index);

		/*!
		return the reference to the array element at given index
		* the arrSize must be smaller than current array size.
		@param[in] index the index of the array to get the array element
		@return the reference to the array element at given index
		*/
		const ArrayType & operator[](unsigned int index) const;

		/*!
		Copy the given Packet Container b to this Packet Container.
		@param[in] b the PacketContainer to copy from
		@return this object
		*/
		PacketContainer& operator =(const PacketContainer& b);

		/*!
		Copy the given Packet b to this Packet Container.
		* Should NOT be used when m_isAllocated is false.
		@param[in] b the Packet to copy from
		@return this object
		*/
		PacketContainer& operator =(const PacketStruct& b);
			

	private:

		/*!
		Actual change the size of the array
		* the arrSize must be larger than current array size.
		* Should NOT be used when m_isAllocated is false.
		@param[in] arrSize the new size of the array
		@return true if successful otherwise false
		*/
		bool setArraySize(unsigned int arrSize);

		/*!
		@struct PacketContainerStruct epPacketContainer.h
		@brief the actual Packet Container Class.	
		*/
		struct PacketContainerStruct
		{
			/// Original Packet
			PacketStruct m_packet;
			/// Dynamic Array of the Packet
			ArrayType m_array[0];
		};
		/// length of the array
		unsigned int m_length;
		/// pointer to the actual packet container
		PacketContainerStruct *m_packetContainer;
		/// flag whether memory is allocated in this object or now
		bool m_isAllocated;
		/// lock
		BaseLock *m_lock;
		/// Lock Policy
		LockPolicy m_lockPolicy;
	};




	template<typename PacketStruct, typename ArrayType>
	PacketContainer<PacketStruct,ArrayType>::PacketContainer(unsigned int arraySize, bool shouldAllocate, LockPolicy lockPolicyType)
	{
		if(shouldAllocate)
		{
			m_packetContainer=reinterpret_cast<PacketContainerStruct*>(EP_Malloc(sizeof(PacketContainerStruct) + (arraySize*sizeof(ArrayType)) ));
			EP_VERIFY_BAD_ALLOC(m_packetContainer);
			m_length=arraySize;
		}
		else
		{
			m_packetContainer=NULL;
			m_length=arraySize;
		}
		m_isAllocated=shouldAllocate;
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

	template<typename PacketStruct, typename ArrayType>
	PacketContainer<PacketStruct,ArrayType>::PacketContainer(const PacketStruct & packet, unsigned int arraySize, bool shouldAllocate, LockPolicy lockPolicyType)
	{
		if(shouldAllocate)
		{
			m_packetContainer=reinterpret_cast<PacketContainerStruct*>( EP_Malloc(sizeof(PacketContainerStruct) + (arraySize*sizeof(ArrayType)) ) );
			EP_VERIFY_BAD_ALLOC(m_packetContainer);
			System::Memcpy(m_packetContainer,&packet,sizeof(PacketContainerStruct) + (arraySize*sizeof(ArrayType)));
			m_length=arraySize;
		}
		else
		{
			m_packetContainer=reinterpret_cast<PacketContainerStruct*>(&packet);
			m_length=arraySize;
		}
		m_isAllocated=shouldAllocate;
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

	template<typename PacketStruct, typename ArrayType>
	PacketContainer<PacketStruct,ArrayType>::PacketContainer(void * rawData, unsigned int byteSize, bool shouldAllocate, LockPolicy lockPolicyType)
	{


		m_packetContainer=NULL;
		m_isAllocated=shouldAllocate;
		if(byteSize<sizeof(PacketStruct))
		{
			EpString errMsg;
			System::SPrintf(errMsg,"byteSize is smaller than PacketStruct size.\nbyteSize must be greater than sizeof(PacketStruct)=%d.\nbyteSize = %d\n",sizeof(PacketStruct),byteSize);
			EP_VERIFY_INVALID_ARGUMENT_W_MSG(byteSize>=sizeof(PacketStruct),errMsg);
		}


		if(m_isAllocated)
		{
			m_packetContainer=reinterpret_cast<PacketContainerStruct*>( EP_Malloc(byteSize) );
			EP_VERIFY_BAD_ALLOC(m_packetContainer);
			System::Memcpy(m_packetContainer,rawData,byteSize);
			m_length=(byteSize-sizeof(PacketContainerStruct))/sizeof(ArrayType);
		}
		else
		{
			m_packetContainer=(PacketContainerStruct*)rawData;
			m_length=(byteSize-sizeof(PacketContainerStruct))/sizeof(ArrayType);
		}
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


	template<typename PacketStruct, typename ArrayType>
	PacketContainer<PacketStruct,ArrayType>::PacketContainer(const PacketContainer<PacketStruct,ArrayType>& orig)
	{	
		if(orig.m_isAllocated)
		{
			m_packetContainer=reinterpret_cast<PacketContainerStruct*>( EP_Malloc(sizeof(PacketContainerStruct) + (orig.m_length*sizeof(ArrayType)) ) );
			EP_VERIFY_BAD_ALLOC(m_packetContainer);
			m_packetContainer->m_packet=orig.m_packetContainer->m_packet;
			m_length=orig.m_length;
			for(int trav=0;trav<m_length;trav++)
			{
				m_packetContainer->m_array[trav]=orig.m_packetContainer->m_array[trav];
			}
		}
		else
		{
			m_packetContainer=orig.m_packetContainer;
			m_length=orig.m_length;
		}
		m_isAllocated=orig.m_isAllocated;
		m_lockPolicy=orig.m_lockPolicy;
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

	template<typename PacketStruct, typename ArrayType>
	PacketContainer<PacketStruct,ArrayType>::~PacketContainer()
	{
		m_lock->Lock();
		if(m_isAllocated && m_packetContainer)
			EP_Free(m_packetContainer);	
		m_lock->Unlock();
		if(m_lock)
			EP_DELETE m_lock;
	}
	

	template<typename PacketStruct, typename ArrayType>
	PacketStruct *PacketContainer<PacketStruct,ArrayType>::GetPacketPtr() const
	{
		return reinterpret_cast<PacketStruct*>(m_packetContainer);
	}

	template<typename PacketStruct, typename ArrayType>
	bool PacketContainer<PacketStruct,ArrayType>::IsAllocated() const
	{
		return m_isAllocated;
	}
	
	template<typename PacketStruct, typename ArrayType>
	void PacketContainer<PacketStruct,ArrayType>::SetPacket(const PacketStruct & packet, unsigned int arraySize=0)
	{
		LockObj lock(m_lock);
		if(m_isAllocated && m_packetContainer)
			EP_Free(m_packetContainer);
		m_packetContainer=NULL;

		if(m_isAllocated)
		{
			m_packetContainer=reinterpret_cast<PacketContainerStruct*>( EP_Malloc(sizeof(PacketContainerStruct) + (arraySize*sizeof(ArrayType)) ) );
			EP_VERIFY_BAD_ALLOC(m_packetContainer);
			System::Memcpy(m_packetContainer,&packet,sizeof(PacketContainerStruct) + (arraySize*sizeof(ArrayType)));
			m_length=arraySize;
		}
		else
		{
			m_packetContainer=reinterpret_cast<PacketContainerStruct*>(&packet);
			m_length=arraySize;
		}
	}

	template<typename PacketStruct, typename ArrayType>
	bool PacketContainer<PacketStruct,ArrayType>::SetPacket(void * rawData, unsigned int byteSize)
	{
		LockObj lock(m_lock);
		if(byteSize<sizeof(PacketStruct))
		{
			EpString errMsg;
			System::SPrintf(errMsg,"byteSize is smaller than PacketStruct size.\nbyteSize must be greater than sizeof(PacketStruct)=%d.\nbyteSize = %d\n",sizeof(PacketStruct),byteSize);
			EP_VERIFY_INVALID_ARGUMENT_W_MSG(byteSize>=sizeof(PacketStruct),errMsg);
		}


		if(m_isAllocated && m_packetContainer)
			EP_Free(m_packetContainer);
		m_packetContainer=NULL;

		if(m_isAllocated)
		{
			m_packetContainer=reinterpret_cast<PacketContainerStruct*>( EP_Malloc(byteSize) );
			EP_VERIFY_BAD_ALLOC(m_packetContainer);
			System::Memcpy(m_packetContainer,rawData,byteSize);
			m_length=(byteSize-sizeof(PacketContainerStruct))/sizeof(ArrayType);
		}
		else
		{
			m_packetContainer=(PacketContainerStruct*)rawData;
			m_length=(byteSize-sizeof(PacketContainerStruct))/sizeof(ArrayType);
		}
		return true;
	}

	template<typename PacketStruct, typename ArrayType>
	ArrayType* PacketContainer<PacketStruct,ArrayType>::GetArray() const
	{
		if(m_packetContainer && m_length>0)
			return m_packetContainer->m_array;
		return NULL;
	}

	template<typename PacketStruct, typename ArrayType>
	bool PacketContainer<PacketStruct,ArrayType>::SetArray(ArrayType *arr,unsigned int arraySize, unsigned int offset=0)
	{
		LockObj lock(m_lock);
		if(m_isAllocated)
		{
			if(m_length<arraySize+offset)
			{
				if(!setArraySize(arraySize+offset))
					return false;
			}
			for(int trav=0;trav<arraySize;trav++)
			{
				m_packetContainer->m_array[offset+trav]=arr[trav];
			}			
			return true;
		}
		else
		{
			if(m_length<arraySize+offset)
			{
				return false;
			}
			for(int trav=0;trav<arraySize;trav++)
			{
				m_packetContainer->m_array[offset+trav]=arr[trav];
			}			
			return true;
		}
	}

	template<typename PacketStruct, typename ArrayType>
	unsigned int PacketContainer<PacketStruct,ArrayType>::GetArrayLength() const
	{
		return m_length;
	}

	template<typename PacketStruct, typename ArrayType>
	unsigned int PacketContainer<PacketStruct,ArrayType>::GetPacketByteSize() const
	{
		return (sizeof(PacketStruct)+(sizeof(ArrayType)*m_length));
	}

	template<typename PacketStruct, typename ArrayType>
	bool PacketContainer<PacketStruct,ArrayType>::SetArraySize(unsigned int arrSize)
	{
		LockObj lock(m_lock);
		return setArraySize(arrSize);
	}

	template<typename PacketStruct, typename ArrayType>
	ArrayType & PacketContainer<PacketStruct,ArrayType>::operator[](unsigned int index)
	{
		EP_VERIFY_DOMAIN_ERROR_W_MSG(m_packetContainer,"Actual packet is NULL.");
		EP_VERIFY_OUT_OF_RANGE(index<m_length);
		return m_packetContainer->m_array[index];
	}

	template<typename PacketStruct, typename ArrayType>
	const ArrayType & PacketContainer<PacketStruct,ArrayType>::operator[](unsigned int index) const
	{
		EP_VERIFY_DOMAIN_ERROR_W_MSG(m_packetContainer,"Actual packet is NULL.");
		EP_VERIFY_OUT_OF_RANGE(index<m_length);
		return m_packetContainer->m_array[index];
	}

	template<typename PacketStruct, typename ArrayType>
	PacketContainer<PacketStruct,ArrayType>& PacketContainer<PacketStruct,ArrayType>::operator =(const PacketContainer<PacketStruct,ArrayType>& b)
	{
		if(this!=&b)
		{
			LockObj lock(m_lock);
			if(m_isAllocated && m_packetContainer)
				EP_Free(m_packetContainer);
			m_packetContainer=NULL;
			if(b.m_isAllocated)
			{
				m_packetContainer=reinterpret_cast<PacketContainerStruct*>( EP_Malloc(sizeof(PacketContainerStruct) + (b.m_length*sizeof(ArrayType)) ) );
				EP_VERIFY_BAD_ALLOC(m_packetContainer);
				m_packetContainer->m_packet=b.m_packetContainer->m_packet;
				m_length=b.m_length;
				for(int trav=0;trav<m_length;trav++)
				{
					m_packetContainer->m_array[trav]=b.m_packetContainer->m_array[trav];
				}
			}
			else
			{
				m_packetContainer=b.m_packetContainer;
				m_length=b.m_length;
			}
			m_isAllocated=b.m_isAllocated;
		}
		return *this;
	}

	template<typename PacketStruct, typename ArrayType>
	PacketContainer<PacketStruct,ArrayType>& PacketContainer<PacketStruct,ArrayType>::operator =(const PacketStruct& b)
	{
		LockObj lock(m_lock);
		if(m_isAllocated)
			m_packetContainer->m_packet=b;
		else
			EP_VERIFY_LOGIC_ERROR_W_MSG(0,"This Function Should NOT be used when m_isAllocated is false.\nInstead USE SetPacket Function.");
		return *this;
	}
	

	template<typename PacketStruct, typename ArrayType>
	bool PacketContainer<PacketStruct,ArrayType>::setArraySize(unsigned int arrSize)
	{
		if(m_isAllocated)
		{
			if(m_length==arrSize)
				return true;

			if(arrSize<=m_length)
			{
				EpString errMsg;
				System::SPrintf(errMsg,"Given size = %d is smaller than the original = %d.\nNew array size must be (greater than/equal to) original array size.",arrSize,m_length);
				EP_VERIFY_INVALID_ARGUMENT_W_MSG(arrSize>=m_length,errMsg);
			}
			
			m_packetContainer=reinterpret_cast<PacketContainerStruct*>(EP_Realloc(m_packetContainer,sizeof(PacketContainerStruct)+ (arrSize*sizeof(ArrayType))));
			System::Memset(((char*)m_packetContainer)+sizeof(PacketContainerStruct)+ (m_length*sizeof(ArrayType)),0,((arrSize-m_length)*sizeof(ArrayType)));
			EP_VERIFY_BAD_ALLOC(m_packetContainer);
			m_length=arrSize;
			return true;
		}
		else
		{
			EP_VERIFY_LOGIC_ERROR_W_MSG(0,"This Function Should NOT be used when m_isAllocated is false.\nInstead USE SetPacket Function.");
			return false;
		}
	}





}
#endif //__EP_PACKET_CONTAINER_H__