/*! 
@file epBaseServerWorker.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date February 13, 2012
@brief An Account Worker Interface
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

An Interface for Account Worker.
(Send->Receive->Send->Receive Model)

*/
#ifndef __EP_BASE_SERVER_WORKER_H__
#define __EP_BASE_SERVER_WORKER_H__

#include "epLib.h"
#include "epSystem.h"
#include "epMemory.h"
#include "epThread.h"
#include "epSmartObject.h"
#include "epPacket.h"
#ifdef EP_MULTIPROCESS
#include "epMutex.h"
#else //EP_MULTIPROCESS
#include "epCriticalSectionEx.h"
#endif //EP_MULTIPROCESS

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
	public:
		/*!
		Default Constructor

		Initializes the Worker
		*/
		BaseServerWorker();

		/*!
		Default Destructor

		Destroy the Worker
		*/
		virtual ~BaseServerWorker();

		/*!
		Send the packet to the client
		@param[in] packet the packet to be sent
		@return sent byte size
		*/
		int Send(const Packet &packet);


	protected:
		/*!
		Parse the given packet and do relevant operation
		@param[in] packet the packet to parse
		@return status of the process
		*/
		virtual int parsePacket(const Packet &packet)=0;

	private:
		/*!
		Default Copy Constructor

		Initializes the BaseServer
		**Should not call this
		@param[in] b the second object
		*/
		BaseServerWorker(const BaseServerWorker& b){EP_ASSERT(0);}
		/*!
		Assignment operator overloading
		**Should not call this
		@param[in] b the second object
		@return the new copied object
		*/
		BaseServerWorker & operator=(const BaseServerWorker&b)
		{
			EP_ASSERT(0);
			return *this;
		}

		/*!
		Receive the packet from the client
		@param[out] packet the packet received
		@return received byte size
		*/
		int receive(Packet &packet);

		/*!
		Set the argument for the base server worker thread.
		@param[in] a The client socket from server.
		*/
		virtual void SetArg(void* a);

		/*!
		thread loop function
		*/
		virtual void execute();

	
		/// client socket
		SOCKET m_clientSocket;

		/// send lock
		BaseLock *m_sendLock;
	};

}

#endif //__EP_BASE_SERVER_WORKER_H__