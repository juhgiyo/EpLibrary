/*! 
@file epPacket.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date February 13, 2012
@brief An Packet Interface
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

An Interface for Packet.

*/
#ifndef __EP_PACKET_H__
#define __EP_PACKET_H__

#include "epLib.h"
#include "epSystem.h"
#include "epMemory.h"
#include "epCriticalSectionEx.h"
#include "epSmartObject.h"

namespace epl{

	/*! 
	@class Packet epPacket.h
	@brief A class for Packet.
	*/
	class EP_FOUNDATION Packet:public SmartObject{

	public:
		/*!
		Default Constructor

		Initializes the Packet
		*/
		Packet(char *packet=NULL,int byteSize=0);

		/*!
		Default Copy Constructor

		Initializes the Packet
		*/
		Packet(const Packet& b);

		/*!
		Assignment Operator Overloading

		the Packet set as given packet b
		*/
		Packet & operator=(const Packet&b);

		/*!
		Default Destructor

		Destroy the Packet
		*/
		~Packet();

		/*!
		Return the currently stored packet byte size
		@return byte size of the holding packet
		*/
		int GetPacketByteSize() const;

		/*!
		Return the currently holding packet
		@return holding packet
		*/
		const char *GetPacket() const;

		/*!
		Set the packet as given
		@param[in] packet the packet data
		@param[in] packetByteSize the size of packet given
		*/
		void SetPacket(char* packet, int packetByteSize);

	private:
		/// packet
		char *m_packet;
		/// packet Byte Size
		int m_packetSize;
	};
}


#endif //__EP_PACKET_H__