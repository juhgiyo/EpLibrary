/*! 
@file epBaseServer.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date February 13, 2012
@brief An Base Server Interface
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

An Interface for Base Server.
(Send->Receive->Send->Receive Model)

*/
#ifndef __EP_BASE_SERVER_H__
#define __EP_BASE_SERVER_H__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif //WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "epLib.h"
#include "epSystem.h"
#include "epMemory.h"
#include "epBaseServerWorker.h"
#ifdef EP_MULTIPROCESS
#include "epMutex.h"
#else //EP_MULTIPROCESS
#include "epCriticalSectionEx.h"
#endif //EP_MULTIPROCESS

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")




using namespace std;

namespace epl{

	
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
	@class BaseServer epBaseServer.h
	@brief A class for Base Server.
	*/
	class EP_FOUNDATION BaseServer{

	public:
		/*!
		Default Constructor

		Initializes the Server
		*/
		BaseServer(CString port=_T(DEFAULT_PORT));

		/*!
		Default Destructor

		Destroy the Server
		*/
		virtual ~BaseServer();

		/*!
		Get Client List
		@return the reference to the list of the client
		*/
		vector<BaseServerWorker*> GetClientList();
		
		/*!
		Set the port for the server.
		**Cannot be changed while connected to server
		@param[in] port The port to set.
		@return true if succeeded otherwise false
		*/
		bool SetPort(CString port);

		/*!
		Get the port number of server
		@return the port number in string
		*/
		CString GetPort();

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
		bool IsServerStarted();

		/*!
		Terminate all clients' socket connected.
		*/
		void ShutdownAllClient();

	private:
		/*!
		Default Copy Constructor

		Initializes the BaseServer
		**Should not call this
		@param[in] b the second object
		*/
		BaseServer(const BaseServer& b){EP_ASSERT(0);}
		/*!
		Assignment operator overloading
		**Should not call this
		@param[in] b the second object
		@return the new copied object
		*/
		BaseServer & operator=(const BaseServer&b)
		{
			EP_ASSERT(0);
			return *this;
		}

		/*!
		Listening Loop Function
		@param[in] lpParam self class object
		@return the thread terminating status
		*/
		static DWORD ServerThread( LPVOID lpParam ) ;
	protected:

		/*!
		Return the new server worker
		** Sub-class should implement this to create new worker
		** Server will automatically release this worker
		@return the new server worker
		*/
		virtual BaseServerWorker* createNewWorker()=0;

		/// port number
		char *m_port;
		/// listening socket
		SOCKET m_listenSocket;
		/// internal use variable
		struct addrinfo *m_result;
		/// internal use variable2
		struct addrinfo m_hints;

		/// the client socket list
		vector<BaseServerWorker*> m_clientList;
		/// the server thread handle
		HANDLE m_serverThreadHandle;
		/// the status of the server
		bool m_isServerStarted;

		/// general lock 
		BaseLock *m_lock;
	};
}
#endif //__EP_BASE_SERVER_H__