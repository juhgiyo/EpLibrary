/*! 
@file epBaseServerUDP.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date February 13, 2012
@brief Base UDP Server Interface
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

An Interface for Base UDP Server.

*/
#ifndef __EP_BASE_SERVER_UDP_H__
#define __EP_BASE_SERVER_UDP_H__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif //WIN32_LEAN_AND_MEAN

#include "epFoundationLib.h"
#include "epSystem.h"
#include "epMemory.h"
#include "epBaseServerWorkerUDP.h"
#include "epCriticalSectionEx.h"
#include "epMutex.h"
#include "epNoLock.h"
#include "epPacket.h"
#include "epServerConf.h"
#include "epBaseServerObject.h"
#include "epServerObjectList.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")




using namespace std;

namespace epl{

	/*! 
	@class BaseServer epBaseServerUDP.h
	@brief A class for Base UDP Server.
	*/
	class EP_FOUNDATION BaseServerUDP:public BaseServerObject{
		friend class BaseServerWorkerUDP;
	public:
		
		/*!
		Default Constructor

		Initializes the Server
		@param[in] port the port string
		@param[in] lockPolicyType The lock policy
		*/
		BaseServerUDP(const TCHAR * port=_T(DEFAULT_PORT), LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the BaseServer
		@param[in] b the second object
		*/
		BaseServerUDP(const BaseServerUDP& b);
		/*!
		Default Destructor

		Destroy the Server
		*/
		virtual ~BaseServerUDP();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		BaseServerUDP & operator=(const BaseServerUDP&b)
		{
			if(this!=&b)
			{
				BaseServerObject::operator =(b);
				LockObj lock(m_lock);
				m_port=b.m_port;
			}
			return *this;
		}

		/*!
		Get Worker List
		@return the list of the worker
		*/
		vector<BaseServerObject*> GetWorkerList() const;

		/*!
		Set the port for the server.
		@remark Cannot be changed while connected to server
		@param[in] port The port to set.
		*/
		void SetPort(const TCHAR *  port);

		/*!
		Get the port number of server
		@return the port number in string
		*/
		EpTString GetPort() const;
		
		/*!
		Get the maximum packet byte size
		@return the maximum packet byte size
		*/
		unsigned int GetMaxPacketByteSize() const;

		/*!
		Start the server
		*/
		bool StartServer();
		/*!
		Stop the server
		*/
		void StopServer();


		/*!
		Check if the server is started
		@return true if the server is started otherwise false
		*/
		bool IsServerStarted() const;

		/*!
		Terminate all clients' socket connected.
		*/
		void ShutdownAllClient();

	protected:
		/*!
		Return the new server worker.
		@remark Sub-class should implement this to create new worker.
		@remark Server will automatically release this worker.
		@return the new server worker
		*/
		virtual BaseServerWorkerUDP* createNewWorker()=0;

	private:
		

		/*!
		Listening Loop Function
		*/
		virtual void execute() ;
		
		/*!
		Send the packet to the client
		@param[in] packet the packet to be sent
		@param[in] clientSockAddr the client socket address, which the packet will be delivered
		@return sent byte size
		*/
		int send(const Packet &packet,const sockaddr &clientSockAddr);

		/*!
		Actually Stop the server
		@param[in] fromInternal flag to check if the call is from internal or not
		*/
		void stopServer(bool fromInternal);

		/// port number
		EpString m_port;
		/// listening socket
		SOCKET m_listenSocket;
		/// internal use variable
		struct addrinfo *m_result;
		/// internal use variable2
		struct addrinfo m_hints;
		/// Maximum UDP Datagram byte size
		unsigned int m_maxPacketSize;

		/// the status of the server
		bool m_isServerStarted;

		/// general lock 
		BaseLock *m_lock;
		/// send lock
		BaseLock *m_sendLock;
		/// disconnect lock
		BaseLock *m_disconnectLock;

		/// Lock Policy
		LockPolicy m_lockPolicy;

		/// worker list
		ServerObjectList m_workerList;

	};
}
#endif //__EP_BASE_SERVER_UDP_H__