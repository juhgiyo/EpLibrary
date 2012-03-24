/*! 
@file epBaseClient.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date February 13, 2012
@brief An Base Client Interface
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

#include "epLib.h"
#include "epSystem.h"
#include "epMemory.h"
#include "epCriticalSectionEx.h"
#include "epPacket.h"
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
	/*!
	@def DEFAULT_HOSTNAME
	@brief default hostname

	Macro for the default hostname.
	*/
	#define DEFAULT_HOSTNAME "localhost"

	/*!
	@def HOSTNAME_MAX_SIZE
	@brief Maximum size of the hostname string

	Macro for the maximum size of the hostname string.
	*/
	#define HOSTNAME_MAX_SIZE 40

#ifndef DEFAULT_PORT
	/*!
	@def DEFAULT_PORT
	@brief default port

	Macro for the default port.
	*/
	#define DEFAULT_PORT "80808"
	
	/*!
	@def PORT_MAX_SIZE
	@brief Maximum size of the port string

	Macro for the maximum size of the port string.
	*/
	#define PORT_MAX_SIZE 8
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
		*/
		BaseClient(CString hostName=_T(DEFAULT_HOSTNAME), CString port=_T(DEFAULT_PORT));
		/*!
		Default Destructor

		Destroy the Client
		*/
		virtual ~BaseClient();

		/*!
		Set the hostname for the server.
		**Cannot be changed while connected to server
		@param[in] hostName The hostname to set.
		@return true if succeeded otherwise false
		*/
		bool SetHostName(CString hostName);

		/*!
		Set the port for the server.
		**Cannot be changed while connected to server
		@param[in] port The port to set.
		@return true if succeeded otherwise false
		*/
		bool SetPort(CString port);

		/*!
		Get the hostname of server
		@return the hostname in string
		*/
		CString GetHostName();

		/*!
		Get the port number of server
		@return the port number in string
		*/
		CString GetPort();

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
		bool IsConnected();

		/*!
		Send the packet to the server
		@param[in] packet the packet to be sent
		@return sent byte size
		*/
		int Send(const Packet &packet);

	protected:
		/*!
		Parse the given packet and do relevant operation
		** Subclasses must implement this
		@param[in] packet the packet to parse
		@return status of the process
		*/
		virtual int parsePacket(const Packet &packet )=0;
	private:
		/*!
		Default Copy Constructor

		Initializes the BaseClient
		**Should not call this
		@param[in] b the second object
		*/
		BaseClient(const BaseClient& b){EP_ASSERT(0);}
		/*!
		Assignment operator overloading
		**Should not call this
		@param[in] b the second object
		@return the new copied object
		*/
		BaseClient & operator=(const BaseClient&b)
		{
			EP_ASSERT(0);
			return *this;
		}

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
		static DWORD ClientThread( LPVOID lpParam ) ;

		/// port
		char *m_port;
		/// hostname
		char *m_hostName;
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
		CriticalSectionEx m_sendLock;
		/// general lock
		CriticalSectionEx m_generalLock;
	};
}


#endif //__EP_BASE_CLIENT_H__