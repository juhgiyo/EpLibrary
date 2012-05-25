/*! 
@file epBaseServerEx.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date February 13, 2012
@brief Base Server Interface
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

An Interface for Base Server Extension.
(Send Receive Independent Model)

*/
#ifndef __EP_BASE_SERVER_EX_H__
#define __EP_BASE_SERVER_EX_H__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif //WIN32_LEAN_AND_MEAN


#include "epFoundationLib.h"
#include "epSystem.h"
#include "epMemory.h"
#include "epBaseServerWorkerEx.h"
#include "epCriticalSectionEx.h"
#include "epMutex.h"
#include "epNoLock.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
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
#endif //DEFAULT_PORT



	/*! 
	@class BaseServerEx epBaseServerEx.h
	@brief A class for Base Server Extension.
	*/
	class EP_FOUNDATION BaseServerEx{

	public:
		/*!
		Default Constructor

		Initializes the Server
		@param[in] port the port string
		@param[in] lockPolicyType The lock policy
		*/
		BaseServerEx(const TCHAR * port=_T(DEFAULT_PORT), LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the BaseServer
		@param[in] b the second object
		*/
		BaseServerEx(const BaseServerEx& b);

		/*!
		Default Destructor

		Destroy the Server
		*/
		virtual ~BaseServerEx();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		BaseServerEx & operator=(const BaseServerEx&b)
		{
			return *this;
		}

		/*!
		Get Client List
		@return the reference to the list of the client
		*/
		vector<BaseServerWorkerEx*> GetClientList() const;
		
		/*!
		Set the port for the server.
		@remark Cannot be changed while connected to server.
		@param[in] port The port to set.
		@return true if succeeded otherwise false
		*/
		bool SetPort(const TCHAR * port);

		/*!
		Get the port number of server
		@return the port number in string
		*/
		EpTString GetPort() const;

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

	private:


		/*!
		Listening Loop Function
		@param[in] lpParam self class object
		@return the thread terminating status
		*/
		static unsigned long ServerThread( LPVOID lpParam ) ;
	protected:

		/*!
		Return the new server worker.
		@remark  Sub-class should implement this to create new worker
		@remark  Server will automatically release this worker
		@return the new server worker
		*/
		virtual BaseServerWorkerEx* createNewWorker()=0;

		/// port number
		EpString m_port;
		/// listening socket
		SOCKET m_listenSocket;
		/// internal use variable
		struct addrinfo *m_result;
		/// internal use variable2
		struct addrinfo m_hints;

		/// the client socket list
		vector<BaseServerWorkerEx*> m_clientList;
		/// the server thread handle
		HANDLE m_serverThreadHandle;
		/// the status of the server
		bool m_isServerStarted;

		/// general lock 
		BaseLock *m_lock;
		/// Lock Policy
		LockPolicy m_lockPolicy;

	};
}
#endif //__EP_BASE_SERVER_EX_H__