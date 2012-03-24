/*! 
@file epBaseServerWorkerEx.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date February 13, 2012
@brief An Account Worker Extension Interface
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

An Interface for Server Worker Extension.
(Send Receive Independent Model)

*/
#ifndef __EP_BASE_SERVER_WORKER_EX_H__
#define __EP_BASE_SERVER_WORKER_EX_H__

#include "epLib.h"
#include "epSystem.h"
#include "epMemory.h"
#include "epThread.h"
#include "epCriticalSectionEx.h"
#include "epSmartObject.h"
#include "epPacket.h"

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
	@class BaseServerWorkerEx epBaseServerWorkerEx.h
	@brief A class for Base Server Worker Extension.
	*/
	class EP_FOUNDATION BaseServerWorkerEx:public Thread, public SmartObject
	{
	public:
		/*!
		Default Constructor

		Initializes the Worker
		*/
		BaseServerWorkerEx();

		/*!
		Default Destructor

		Destroy the Worker
		*/
		virtual ~BaseServerWorkerEx();
		
		/*!
		Send the packet to the client
		@param[in] packet the packet to be sent
		@return sent byte size
		*/
		int Send(const Packet &packet);


	protected:
		/*!
		Parse the given packet and do relevant operation
		** Subclasses must implement this
		@param[in] packet the packet to parse
		*/
		virtual void parsePacket(const Packet &packet)=0;

	private:
		/*!
		Receive the packet from the client
		@param[out] packet the packet received
		@return received byte size
		*/
		int receive(Packet &packet);

		/*!
		Handle the packet parsing thread.
		@param[in] param the packet Pass Unit
		@return status of thread execution
		*/
		static unsigned long __stdcall passPacket(void *param);

		/*! 
		@struct PacketPassUnit epBaseServerWorkerEx.h
		@brief A class for Packet Passing Unit for Packet Parsing Thread.
		*/
		struct PacketPassUnit{
			/// BaseServerWorkerEx Object
			BaseServerWorkerEx *m_this;
			/// Packet to parse
			Packet *m_packet;
		};

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
		CriticalSectionEx m_sendLock;
	};

}

#endif //__EP_BASE_SERVER_WORKER_EX_H__