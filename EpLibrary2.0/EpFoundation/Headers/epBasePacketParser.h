/*! 
@file epBasePacketParser.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date July 20, 2012
@brief Base Packet Parser Interface
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

An Interface for Base Packet Parser.

*/
#ifndef __EP_BASE_PACKET_PARSER_H__
#define __EP_BASE_PACKET_PARSER_H__

#include "epFoundationLib.h"
#include "epSystem.h"
#include "epMemory.h"
#include "epThread.h"
#include "epSmartObject.h"
#include "epPacket.h"
#include "epBaseServerSendObject.h"
#include "epServerConf.h"
#include "epBaseCallbackObject.h"

namespace epl
{
	/*! 
	@class BasePacketParser epBasePacketParser.h
	@brief A class for Base Packet Parser.
	*/
	class EP_FOUNDATION BasePacketParser:public BaseServerObject
	{
	public:
		/*!
		Default Constructor

		Initializes the Worker
		@param[in] waitTimeMilliSec the wait time in millisecond for terminating
		@param[in] callbackObj the callback object to call when parser thread stops
		@param[in] lockPolicyType The lock policy
		*/
		BasePacketParser(unsigned int waitTimeMilliSec=DEFAULT_WAITTIME,BaseCallbackObject *callbackObj=NULL,LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the BasePacketParser
		@param[in] b the second object
		*/
		BasePacketParser(const BasePacketParser& b);

		/*!
		Default Destructor

		Destroy the Parser
		*/
		virtual ~BasePacketParser();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		BasePacketParser & operator=(const BasePacketParser&b)
		{
			if(this!=&b)
			{
				LockObj lock(m_generalLock);
				BaseServerObject::operator =(b);
				m_waitTime=b.m_waitTime;
				m_owner=b.m_owner;
				if(m_packetReceived)
					m_packetReceived->ReleaseObj();
				m_packetReceived=b.m_packetReceived;
				m_packetReceived->RetainObj();
				
			}
			return *this;
		}

		/*!
		Send the packet to the client
		@param[in] packet the packet to be sent
		@return sent byte size
		*/
		int Send(const Packet &packet);

		/*!
		Set the wait time for the parser thread termination
		@param[in] milliSec the time for waiting in millisecond
		*/
		void SetWaitTimeForParserTerminate(unsigned int milliSec);

		/*!
		Get the wait time for the parser thread termination
		@return the current time for waiting in millisecond
		*/
		unsigned int GetWaitTimeForParserTerminate();

		/*!
		Parse the given packet and do relevant operation
		@remark  Subclasses must implement this
		@param[in] packet the packet to parse
		*/
		virtual void ParsePacket(const Packet &packet)=0;

		/*! 
		@struct PacketPassUnit epBasePacketParser.h
		@brief A class for Packet Passing Unit for Packet Parsing Thread.
		*/
		struct PacketPassUnit{
			/// BaseServerWorkerEx Object
			BaseServerSendObject *m_this;
			/// Packet to parse
			Packet *m_packet;
		};

		


	private:	
		/*!
		Set the argument for the base server worker thread.
		@param[in] a The client socket from server.
		*/
		virtual void setArg(void* a);

		/*!
		thread loop function
		*/
		virtual void execute();



        /// Owner
		BaseServerSendObject *m_owner;

		/// Packet received
		Packet * m_packetReceived;

		/// wait time in millisecond for terminating thread
		unsigned int m_waitTime;

		/// Lock Policy
		LockPolicy m_lockPolicy;

		/// general lock
		BaseLock *m_generalLock;
	};

}

#endif //__EP_BASE_PACKET_PARSER_H__