/*! 
@file epBaseClient.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date February 13, 2012
@brief Base Client Interface
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

An Interface for Base Client.
(Send->Receive->Send->Receive Model)

*/
#ifndef __EP_BASE_CLIENT_H__
#define __EP_BASE_CLIENT_H__

#include "epFoundationLib.h"
#include "epSystem.h"
#include "epMemory.h"
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
#include <stdlib.h>
#include <stdio.h>
#include <vector>



// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")



using namespace std;


namespace epl{

#ifndef DEFAULT_HOSTNAME
	/*!
	@def DEFAULT_HOSTNAME
	@brief default hostname

	Macro for the default hostname.
	*/
	#define DEFAULT_HOSTNAME "localhost"
#endif //DEFAULT_HOSTNAME

#ifndef DEFAULT_PORT
	/*!
	@def DEFAULT_PORT
	@brief default port

	Macro for the default port.
	*/
	#define DEFAULT_PORT "80808"
#endif //DEFAULT_PORT

	/*! 
	@class BaseClient epBaseClient.h
	@brief A class for Base Client.
	*/
	class EP_FOUNDATION BaseClient{

	public:
				/*!
		Default Constructor

		Initializes the Client
		@param[in] hostName the hostname string
		@param[in] port the port string
		@param[in] lockPolicyType The lock policy
		*/
		BaseClient(const TCHAR * hostName=_T(DEFAULT_HOSTNAME), const TCHAR * port=_T(DEFAULT_PORT),LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the BaseClient
		@param[in] b the second object
		*/
		BaseClient(const BaseClient& b);
		/*!
		Default Destructor

		Destroy the Client
		*/
		virtual ~BaseClient();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		BaseClient & operator=(const BaseClient&b)
		{
			return *this;
		}


		/*!
		Set the hostname for the server.
		@remark Cannot be changed while connected to server
		@param[in] hostName The hostname to set.
		@return true if succeeded otherwise false
		*/
		bool SetHostName(const TCHAR * hostName);

		/*!
		Set the port for the server.
		@remark Cannot be changed while connected to server
		@param[in] port The port to set.
		@return true if succeeded otherwise false
		*/
		bool SetPort(const TCHAR * port);

		/*!
		Get the hostname of server
		@return the hostname in string
		*/
		EpTString GetHostName() const;

		/*!
		Get the port number of server
		@return the port number in string
		*/
		EpTString GetPort() const;

		/*!
		Connect to the server
		*/
		bool Connect();

		/*!
		Disconnect from the server
		*/
		void Disconnect();

		/*!
		Check if the connection is established
		@return true if the connection is established otherwise false
		*/
		bool IsConnected() const;

		/*!
		Send the packet to the server
		@param[in] packet the packet to be sent
		@return sent byte size
		*/
		int Send(const Packet &packet);

	protected:
		/*!
		Parse the given packet and do relevant operation.
		@remark  Subclasses must implement this
		@param[in] packet the packet to parse
		*/
		virtual void parsePacket(const Packet &packet )=0;
	private:
		
		/*!
		Actually Disconnect from the server
		*/
		void disconnect();
		/*!
		Receive the packet from the server
		@param[out] packet the packet received
		@return received byte size
		*/
		int receive(Packet &packet);

		/*!
		Receiving Loop Function
		@param[in] lpParam self class object
		@return the thread terminating status
		*/
		static unsigned long ClientThread( LPVOID lpParam ) ;

		/// port
		EpString m_port;
		/// hostname
		EpString m_hostName;
		/// connection socket
		SOCKET m_connectSocket;
		/// internal variable
		struct addrinfo *m_result;
		/// internal variable2
		struct addrinfo *m_ptr;
		/// internal variable3
		struct addrinfo m_hints;

		/// flag for the connection
		bool m_isConnected;

		/// the client thread handle
		HANDLE m_clientThreadHandle;

		/// send lock
		BaseLock *m_sendLock;
		/// general lock
		BaseLock *m_generalLock;
		/// Lock Policy
		LockPolicy m_lockPolicy;
	};
}


#endif //__EP_BASE_CLIENT_H__