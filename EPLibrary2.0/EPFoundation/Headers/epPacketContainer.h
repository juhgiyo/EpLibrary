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

An Interface for Packet Container.

@secion SPECIAL THANKS TO
 
Sang Yong Yoon <flyingshiri@gmail.com>

*/
#ifndef __EP_PACKET_CONTAINER_H__
#define __EP_PACKET_CONTAINER_H__

#include "epLib.h"

namespace epl
{
	/*! 
	@class PacketContainer epPacketContainer.h
	@brief A class for Packet Container.
	*/
	template<typename PacketStruct, typename ArrayType=char>
	class PacketContainer
	{
	public:
		/*!
		Default Constructor

		Initializes given size of array of ArrayType addition to the PacketStruct.
		@param[in] arraySize the size of array addition to PacketStruct
		*/
		PacketContainer(int arraySize=0)
		{
			m_packetContainer=(PacketContainerStruct*)EP_Malloc(sizeof(PacketContainerStruct)+ (arraySize*sizeof(ArrayType)) );
			m_length=arraySize;
		}

		/*!
		Copy Constructor

		Copy the given Packet Container to this Packet Container.
		@param[in] orig the original PacketContainer
		*/
		PacketContainer(const PacketContainer& orig)
		{
			m_packetContainer=EP_Malloc(sizeof(PacketContainerStruct)+ (orig.m_length*sizeof(ArrayType)) );
			m_packetContainer->m_packet=orig.m_packetContainer->m_packet;
			m_length=orig.m_length;
			for(int trav=0;trav<m_length;trav++)
			{
				m_packetContainer->m_array[trav]=orig.m_packetContainer->m_array[trav];
			}
		}

		/*!
		Default Destructor

		Destroy the Packet
		*/
		~PacketContainer()
		{
			EP_Free(m_packetContainer);	
		}
				
		/*!
		Get a reference to the packet
		@return the reference to the packet
		*/
		PacketStruct &GetPacket()
		{
			return m_packetContainer->m_packet;
		}

		/*!
		Get the pointer to the array
		@return the pointer to the array
		*/
		ArrayType* GetArray()
		{
			return m_packetContainer->m_array;
		}

		/*!
		Get the length of the array
		@return the length of the array
		*/
		unsigned int GetArrayLength()
		{
			return m_length;
		}

		/*!
		Get the total byte size of the packet including the array
		@return the total byte size of the packet including the array
		*/
		unsigned int GetPacketByteSize()
		{
			return (sizeof(PacketStruct)+(sizeof(ArrayType)*m_length));
		}

		/*!
		Change the size of the array
		* the arrSize must be larger than current array size.
		@param[in] arrSize the new size of the array
		*/
		void SetArraySize(unsigned int arrSize)
		{
			if(m_length==arrSize)
				return;
			EP_ASSERT(m_packetContainer->m_length<arrSize);

			PacketContainerStruct *tempcontainer=EP_Malloc(sizeof(PacketContainerStruct)+ (arrSize*sizeof(ArrayType)) );
			tempcontainer->m_packet=m_packetContainer;
			m_length=arrSize;
			for(int trav=0;trav<m_length;trav++)
			{
				tempcontainer->m_array[trav]=m_packetContainer->m_array[trav];
			}
			EP_Free(m_packetContainer);
			m_packetContainer=tempcontainer;

		}

		/*!
		return the reference to the array element at given index
		* the arrSize must be smaller than current array size.
		@param[in] index the index of the array to get the array element
		*/
		ArrayType & operator[](unsigned int index)
		{
			EP_ASSERT(index<m_length);
			return m_packetContainer->m_array[index];
		}

	private:
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
	};
}
#endif //__EP_PACKET_CONTAINER_H__