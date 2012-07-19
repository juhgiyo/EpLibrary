/*! 
@file epBaseServerWorker.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date February 13, 2012
@brief Base Worker Interface
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

An Interface for Base Server Worker.

*/
#ifndef __EP_BASE_SERVER_WORKER_H__
#define __EP_BASE_SERVER_WORKER_H__

#include "epFoundationLib.h"
#include "epSystem.h"
#include "epMemory.h"
#include "epThread.h"
#include "epSmartObject.h"
#include "epPacket.h"
#include "epCriticalSectionEx.h"
#include "epMutex.h"
#include "epNoLock.h"

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
	/*! 
	@class BaseServerWorker epBaseServerWorker.h
	@brief A class for Base Server Worker.
	*/
	class EP_FOUNDATION BaseServerWorker:public Thread, public SmartObject
	{
		friend class BaseServerWorkerEx;
		friend class BaseServerWorkerSimple;
	public:
		/*!
		Default Constructor

		Initializes the Worker
		@param[in] lockPolicyType The lock policy
		*/
		BaseServerWorker(LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the BaseServer
		@param[in] b the second object
		*/
		BaseServerWorker(const BaseServerWorker& b);

		/*!
		Default Destructor

		Destroy the Worker
		*/
		virtual ~BaseServerWorker();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		BaseServerWorker & operator=(const BaseServerWorker&b)
		{
			if(this!=&b)
			{
				LockObj lock(m_sendLock);
				m_clientSocket=b.m_clientSocket;
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


	protected:
		/*!
		Parse the given packet and do relevant operation
		@remark  Subclasses must implement this
		@param[in] packet the packet to parse
		*/
		virtual void parsePacket(const Packet &packet)=0;

		/*!
		thread loop function
		*/
		virtual void execute()=0;

	private:	
		/*!
		Receive the packet from the client
		@remark  Subclasses must implement this
		@param[out] packet the packet received
		@return received byte size
		*/
		int receive(Packet &packet);
	
		/*!
		Set the argument for the base server worker thread.
		@param[in] a The client socket from server.
		*/
		virtual void SetArg(void* a);
	
		/// client socket
		SOCKET m_clientSocket;

		/// send lock
		BaseLock *m_sendLock;
		
		/// Lock Policy
		LockPolicy m_lockPolicy;

		/// Temp Packet;
		Packet m_recvSizePacket;
	};

}

#endif //__EP_BASE_SERVER_WORKER_H__