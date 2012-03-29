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

#include "epLib.h"
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
		PacketContainer(unsigned int arraySize=0, bool shouldAllocate=true, LockPolicy lockPolicyType=EP_LOCK_POLICY)
		{
			if(shouldAllocate)
			{
				m_packetContainer=(PacketContainerStruct*)EP_Malloc(sizeof(PacketContainerStruct) + (arraySize*sizeof(ArrayType)) );
				EP_WASSERT(m_packetContainer,_T("Allocation Failed"));
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

		/*!
		Default Constructor

		Initializes given size of array of ArrayType addition to the PacketStruct.
		@param[in] packet the packet to copy from
		@param[in] arraySize the size of array addition to PacketStruct
		@param[in] shouldAllocate flag for the allocation of memory for itself
		@param[in] lockPolicyType The lock policy
		*/
		PacketContainer(const PacketStruct & packet, unsigned int arraySize=0, bool shouldAllocate=true, LockPolicy lockPolicyType=EP_LOCK_POLICY)
		{
			if(shouldAllocate)
			{
				m_packetContainer=(PacketContainerStruct*)EP_Malloc(sizeof(PacketContainerStruct) + (arraySize*sizeof(ArrayType)) );
				EP_WASSERT(m_packetContainer,_T("Allocation Failed"));
				memcpy(m_packetContainer,&packet,sizeof(PacketContainerStruct) + (arraySize*sizeof(ArrayType)));
				m_length=arraySize;
			}
			else
			{
				m_packetContainer=(PacketContainerStruct*)&packet;
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

		/*!
		Default Constructor

		Initializes PacketContainer with the raw data given.
		@param[in] rawData the rawData to copy from
		@param[in] byteSize the byte size of raw data given.
		@param[in] shouldAllocate flag for the allocation of memory for itself
		@param[in] lockPolicyType The lock policy
		*/
		PacketContainer(void * rawData, unsigned int byteSize, bool shouldAllocate=true, LockPolicy lockPolicyType=EP_LOCK_POLICY)
		{


			m_packetContainer=NULL;
			m_isAllocated=shouldAllocate;
			if(byteSize<sizeof(PacketStruct))
			{
				EP_WASSERT(0,_T("byteSize is smaller than PacketStruct size.\nbyteSize must be greater than sizeof(PacketStruct)."));
			}
			if(m_isAllocated)
			{
				m_packetContainer=(PacketContainerStruct*)EP_Malloc(byteSize);
				EP_WASSERT(m_packetContainer,_T("Allocation Failed"));
				memcpy(m_packetContainer,rawData,byteSize);
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


		/*!
		Copy Constructor

		Copy the given Packet Container to this Packet Container.
		@param[in] orig the original PacketContainer
		*/
		PacketContainer(const PacketContainer& orig)
		{	
			if(orig.m_isAllocated)
			{
				m_packetContainer=EP_Malloc(sizeof(PacketContainerStruct) + (orig.m_length*sizeof(ArrayType)) );
				EP_WASSERT(m_packetContainer,_T("Allocation Failed"));
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

		/*!
		Default Destructor

		Destroy the Packet
		*/
		virtual ~PacketContainer()
		{
			if(m_isAllocated && m_packetContainer)
				EP_Free(m_packetContainer);	
			if(m_lock)
				EP_DELETE m_lock;
		}
		

		/*!
		Get a pointer to the packet
		@return the pointer to the packet
		*/
		PacketStruct *GetPacketPtr() const
		{
			return (PacketStruct*)m_packetContainer;
		}

		/*!
		Get the flag whether memory is allocated or not
		@return true if the memory is allocated by this object, otherwise false
		*/
		bool IsAllocated() const
		{
			return m_isAllocated;
		}
		/*!
		Set the packet container according to the packet given
		@param[in] packet the packet to copy from
		@param[in] arraySize the size of variable length array of given packet
		*/
		void SetPacket(const PacketStruct & packet, unsigned int arraySize=0)
		{
			LockObj lock(m_lock);
			if(m_isAllocated && m_packetContainer)
				EP_Free(m_packetContainer);
			m_packetContainer=NULL;

			if(m_isAllocated)
			{
				m_packetContainer=(PacketContainerStruct*)EP_Malloc(sizeof(PacketContainerStruct) + (arraySize*sizeof(ArrayType)) );
				EP_WASSERT(m_packetContainer,_T("Allocation Failed"));
				memcpy(m_packetContainer,&packet,sizeof(PacketContainerStruct) + (arraySize*sizeof(ArrayType)));
				m_length=arraySize;
			}
			else
			{
				m_packetContainer=(PacketContainerStruct*)&packet;
				m_length=arraySize;
			}
		}

		/*!
		Set the packet container from the raw data
		@param[in] rawData the rawData to copy from
		@param[in] byteSize the byte size of raw data given.
		@return true if successful otherwise false
		*/
		bool SetPacket(void * rawData, unsigned int byteSize)
		{
			LockObj lock(m_lock);
			if(byteSize<sizeof(PacketStruct))
			{
				EP_WASSERT(0,_T("byteSize is smaller than PacketStruct size.\nbyteSize must be greater than sizeof(PacketStruct)."));
				return false;
			}

			if(m_isAllocated && m_packetContainer)
				EP_Free(m_packetContainer);
			m_packetContainer=NULL;

			if(m_isAllocated)
			{
				m_packetContainer=(PacketContainerStruct*)EP_Malloc(byteSize);
				EP_WASSERT(m_packetContainer,_T("Allocation Failed"));
				memcpy(m_packetContainer,rawData,byteSize);
				m_length=(byteSize-sizeof(PacketContainerStruct))/sizeof(ArrayType);
			}
			else
			{
				m_packetContainer=(PacketContainerStruct*)rawData;
				m_length=(byteSize-sizeof(PacketContainerStruct))/sizeof(ArrayType);
			}
			return true;
		}

		/*!
		Get the pointer to the array
		@return the pointer to the array
		*/
		ArrayType* GetArray() const
		{
			if(m_packetContainer && m_length>0)
				return m_packetContainer->m_array;
			return NULL;
		}

		/*!
		Set the array with given array
		@param[in] arr the array to copy from
		@param[in] arraySize the size of given array
		@param[in] offset the offset for start position of copying
		@return true if successful otherwise false
		*/
		bool SetArray(ArrayType *arr,unsigned int arraySize, unsigned int offset=0)
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

		/*!
		Get the length of the array
		@return the length of the array
		*/
		unsigned int GetArrayLength() const
		{
			return m_length;
		}

		/*!
		Get the total byte size of the packet including the array
		@return the total byte size of the packet including the array
		*/
		unsigned int GetPacketByteSize() const
		{
			return (sizeof(PacketStruct)+(sizeof(ArrayType)*m_length));
		}

		/*!
		Change the size of the array
		* the arrSize must be larger than current array size.
		* Should NOT be used when m_isAllocated is false.
		@param[in] arrSize the new size of the array
		@return true if successful otherwise false
		*/
		bool SetArraySize(unsigned int arrSize)
		{
			LockObj lock(m_lock);
			return setArraySize(arrSize);
		}

		/*!
		return the reference to the array element at given index
		* the arrSize must be smaller than current array size.
		@param[in] index the index of the array to get the array element
		@return the reference to the array element at given index
		*/
		ArrayType & operator[](unsigned int index)
		{
			EP_WASSERT(m_packetContainer,_T("Actual packet is NULL."));
			EP_WASSERT(index<m_length,_T("The given index is greater than the memory allocated"));
			return m_packetContainer->m_array[index];
		}

		/*!
		Copy the given Packet Container b to this Packet Container.
		@param[in] b the PacketContainer to copy from
		@return this object
		*/
		PacketContainer& operator =(const PacketContainer& b)
		{
			if(this!=&b)
			{
				LockObj lock(m_lock);
				if(m_isAllocated && m_packetContainer)
					EP_Free(m_packetContainer);
				m_packetContainer=NULL;
				if(b.m_isAllocated)
				{
					m_packetContainer=EP_Malloc(sizeof(PacketContainerStruct) + (b.m_length*sizeof(ArrayType)) );
					EP_WASSERT(m_packetContainer,_T("Allocation Failed."));
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

		/*!
		Copy the given Packet b to this Packet Container.
		* Should NOT be used when m_isAllocated is false.
		@param[in] b the Packet to copy from
		@return this object
		*/
		PacketContainer& operator =(const PacketStruct& b)
		{
			LockObj lock(m_lock);
			if(m_isAllocated)
				m_packetContainer->m_packet=b;
			else
				EP_WASSERT(0,_T("Should not be used when m_isAllocated is false.\nInstead USE SetPacket Function."));
			return *this;
		}
		
	

	private:

		/*!
		Actual change the size of the array
		* the arrSize must be larger than current array size.
		* Should NOT be used when m_isAllocated is false.
		@param[in] arrSize the new size of the array
		@return true if successful otherwise false
		*/
		bool setArraySize(unsigned int arrSize)
		{
			if(m_isAllocated)
			{
				if(m_length==arrSize)
					return true;
				EP_WASSERT(m_packetContainer->m_length<arrSize,_T("Given size is smaller than the original.\nNew array size must be greater than original array size."));
				m_packetContainer=EP_Realloc(m_packetContainer,sizeof(PacketContainerStruct)+ (arrSize*sizeof(ArrayType)));
				EP_WASSERT(m_packetContainer,_T("Allocation Failed"));
				m_length=arrSize;
				return true;
			}
			else
			{
				EP_WASSERT(0,_T("Should not be used when m_isAllocated is false."));
				return false;
			}
		}
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
}
#endif //__EP_PACKET_CONTAINER_H__