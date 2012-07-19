/*! 
@file epBaseServer.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
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

An Interface for Base Server.

*/
#ifndef __EP_BASE_SERVER_H__
#define __EP_BASE_SERVER_H__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif //WIN32_LEAN_AND_MEAN

#include "epFoundationLib.h"
#include "epSystem.h"
#include "epMemory.h"
#include "epBaseServerWorker.h"
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

#ifndef DEFAULT_WAITTIME
	/*!
	@def DEFAULT_WAITTIME
	@brief default wait time

	Macro for the default wait time in millisec.
	*/
	#define DEFAULT_WAITTIME INFINITE
#endif //DEFAULT_WAITTIME

	/*! 
	@class BaseServer epBaseServer.h
	@brief A class for Base Server.
	*/
	class EP_FOUNDATION BaseServer{

	public:
		/*!
		Default Constructor

		Initializes the Server
		@param[in] port the port string
		@param[in] waitTimeMilliSec the wait time in millisecond for terminating thread
		@param[in] lockPolicyType The lock policy
		*/
		BaseServer(const TCHAR * port=_T(DEFAULT_PORT), unsigned int waitTimeMilliSec=DEFAULT_WAITTIME, LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the BaseServer
		@param[in] b the second object
		*/
		BaseServer(const BaseServer& b);
		/*!
		Default Destructor

		Destroy the Server
		*/
		virtual ~BaseServer();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		BaseServer & operator=(const BaseServer&b)
		{
			return *this;
		}


		/*!
		Get Client List
		@return the reference to the list of the client
		*/
		vector<BaseServerWorker*> GetClientList() const;
		
		/*!
		Set the port for the server.
		@remark Cannot be changed while connected to server
		@param[in] port The port to set.
		@return true if succeeded otherwise false
		*/
		bool SetPort(const TCHAR *  port);

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
		@remark Sub-class should implement this to create new worker.
		@remark Server will automatically release this worker.
		@return the new server worker
		*/
		virtual BaseServerWorker* createNewWorker()=0;

		/*!
		Actually terminate all clients' socket connected.
		*/
		void shutdownAllClient();

		/*!
		Actually Stop the server
		*/
		void stopServer();

		/// port number
		EpString m_port;
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
		/// Lock Policy
		LockPolicy m_lockPolicy;

		/// wait time in millisecond for terminating thread
		unsigned int m_waitTime;
	};
}
#endif //__EP_BASE_SERVER_H__