/*! 
@file epBaseServerWorkerUDP.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date February 13, 2012
@brief Base UDP Worker Interface
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

An Interface for Base UDP Server Worker.

*/
#ifndef __EP_BASE_SERVER_WORKER_UDP_H__
#define __EP_BASE_SERVER_WORKER_UDP_H__

#include "epFoundationLib.h"
#include "epSystem.h"
#include "epMemory.h"
#include "epThread.h"
#include "epSmartObject.h"
#include "epPacket.h"
#include "epCriticalSectionEx.h"
#include "epMutex.h"
#include "epNoLock.h"
#include "epServerConf.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif //WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>



// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

namespace epl
{
	class BaseServerUDP;
	/*! 
	@class BaseServerWorker epBaseServerWorkerUDP.h
	@brief A class for Base UDP Server Worker.
	*/
	class EP_FOUNDATION BaseServerWorkerUDP:public Thread, public SmartObject
	{
		friend class BaseServerUDP;
	public:
		/*!
		Default Constructor

		Initializes the Worker
		@param[in] waitTimeMilliSec the wait time in millisecond for terminating thread
		@param[in] lockPolicyType The lock policy
		*/
		BaseServerWorkerUDP(unsigned int waitTimeMilliSec=DEFAULT_WAITTIME,LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the BaseServer
		@param[in] b the second object
		*/
		BaseServerWorkerUDP(const BaseServerWorkerUDP& b);

		/*!
		Default Destructor

		Destroy the Worker
		*/
		virtual ~BaseServerWorkerUDP();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		BaseServerWorkerUDP & operator=(const BaseServerWorkerUDP&b)
		{
			if(this!=&b)
			{
				LockObj lock(m_lock);
				m_waitTime=b.m_waitTime;
				Thread::operator =(b);
				SmartObject::operator =(b);
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
		Get the maximum packet byte size
		@return the maximum packet byte size
		*/
		unsigned int GetMaxPacketByteSize() const;

		/*!
		Set the wait time for the thread termination
		@param[in] milliSec the time for waiting in millisecond
		*/
		void SetWaitTimeForSafeTerminate(unsigned int milliSec);

		/*!
		Get the wait time for the thread termination
		@return the current time for waiting in millisecond
		*/
		unsigned int GetWaitTimeForSafeTerminate();

	protected:
		/*!
		Parse the given packet and do relevant operation
		@remark  Subclasses must implement this
		@param[in] packet the packet to parse
		*/
		virtual void parsePacket(const Packet &packet)=0;

	private:	


		/*! 
		@struct PacketPassUnit epBaseServerWorkerUDP.h
		@brief A class for Packet Passing Unit for Packet Parsing Thread.
		*/
		struct PacketPassUnit{
			/// BaseServerUDP Object
			BaseServerUDP *m_server;
			/// Packet to parse
			Packet *m_packet;
			/// client socket
			sockaddr m_clientSocket;
		};

		/*!
		thread loop function
		*/
		virtual void execute();

		/*!
		Set the server object as given
		@param[in] server the pointer to the server object
		*/
		void setServer(BaseServerUDP *server);

		/*!
		Set the argument for the base server worker thread.
		@param[in] a The client socket from server.
		*/
		virtual void SetArg(void* a);
	
		/// client socket
		sockaddr m_clientSocket;

		/// server object
		BaseServerUDP *m_server;

		/// packet received
		Packet *m_packet;

		/// Maximum UDP Datagram byte size
		unsigned int m_maxPacketSize;

		/// general lock 
		BaseLock *m_lock;

		/// Lock Policy
		LockPolicy m_lockPolicy;
        
		/// wait time in millisecond for terminating thread
		unsigned int m_waitTime;
	};

}

#endif //__EP_BASE_SERVER_WORKER_UDP_H__