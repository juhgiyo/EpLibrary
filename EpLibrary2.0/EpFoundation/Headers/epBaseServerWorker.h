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
#include "epServerConf.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif //WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <vector>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

using namespace std;

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
		@param[in] parserWaitTimeMilliSec the wait time in millisecond for terminating parser thread
		@param[in] lockPolicyType The lock policy
		*/
		BaseServerWorker(unsigned int parserWaitTimeMilliSec=DEFAULT_WAITTIME,LockPolicy lockPolicyType=EP_LOCK_POLICY);

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
				m_parserWaitTime=b.m_parserWaitTime;
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
		Set the wait time for the parser thread termination
		@param[in] milliSec the time for waiting in millisecond
		*/
		void SetWaitTimeForParserTerminate(unsigned int milliSec);

		/*!
		Get the wait time for the parser thread termination
		@return the current time for waiting in millisecond
		*/
		unsigned int GetWaitTimeForParserTerminate();
	protected:
		/*!
		Parse the given packet and do relevant operation
		@remark  Subclasses must implement this
		@param[in] packet the packet to parse
		*/
		virtual void parsePacket(const Packet &packet)=0;



	private:	
		/*!
		thread loop function
		*/
		virtual void execute();

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
			BaseServerWorker *m_this;
			/// Packet to parse
			Packet *m_packet;
		};

	
		/// client socket
		SOCKET m_clientSocket;

		/// send lock
		BaseLock *m_sendLock;
		
		/// Lock Policy
		LockPolicy m_lockPolicy;

		/// Temp Packet;
		Packet m_recvSizePacket;

		/// wait time in millisecond for terminating thread
		unsigned int m_parserWaitTime;

		/// list lock
		BaseLock *m_listLock;

		/// parser thread list
		vector<HANDLE> m_parserList;
	};

}

#endif //__EP_BASE_SERVER_WORKER_H__