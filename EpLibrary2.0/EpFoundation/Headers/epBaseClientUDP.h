/*! 
@file epBaseClientUDP.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date July 18, 2012
@brief Base UDP Client Interface
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

An Interface for Base UDP Client.

*/
#ifndef __EP_BASE_CLIENT_UDP_H__
#define __EP_BASE_CLIENT_UDP_H__

#include "epFoundationLib.h"
#include "epSystem.h"
#include "epMemory.h"
#include "epPacket.h"
#include "epCriticalSectionEx.h"
#include "epMutex.h"
#include "epNoLock.h"
#include "epBaseServerSendObject.h"
#include "epServerConf.h"
#include "epBasePacketParser.h"
#include "epServerObjectList.h"

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
	@class BaseClient epBaseClient.h
	@brief A class for Base Client.
	*/
	class EP_FOUNDATION BaseClientUDP:public BaseServerSendObject{

	public:
		/*!
		Default Constructor

		Initializes the Client
		@param[in] hostName the hostname string
		@param[in] port the port string
		@param[in] lockPolicyType The lock policy
		*/
		BaseClientUDP(const TCHAR * hostName=_T(DEFAULT_HOSTNAME), const TCHAR * port=_T(DEFAULT_PORT),LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the BaseClient
		@param[in] b the second object
		*/
		BaseClientUDP(const BaseClientUDP& b);
		/*!
		Default Destructor

		Destroy the Client
		*/
		virtual ~BaseClientUDP();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		BaseClientUDP & operator=(const BaseClientUDP&b)
		{
			if(this!=&b)
			{
				LockObj lock(m_generalLock);
				m_port=b.m_port;
				m_hostName=b.m_hostName;
			}
			return *this;
		}

		/*!
		Set the hostname for the server.
		@remark Cannot be changed while connected to server
		@param[in] hostName The hostname to set.
		*/
		void SetHostName(const TCHAR * hostName);

		/*!
		Set the port for the server.
		@remark Cannot be changed while connected to server
		@param[in] port The port to set.
		*/
		void SetPort(const TCHAR * port);

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
		Get the maximum packet byte size
		@return the maximum packet byte size
		*/
		unsigned int GetMaxPacketByteSize() const;

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
		virtual int Send(const Packet &packet);

		/*!
		Get Packet Parser List
		@return the list of the packet parser
		*/
		vector<BaseServerObject*> GetPacketParserList() const;

	protected:
		/*!
		Return the new packet parser
		@remark Sub-class should implement this to create new parser.
		@remark Client will automatically release this parser.
		@return the new packet parser
		*/
		virtual BasePacketParser* createNewPacketParser()=0;


	private:
		
		
		/*!
		Receive the packet from the server
		@param[out] packet the packet received
		@return received byte size
		*/
		int receive(Packet &packet);

		/*!
		Actually processing the client thread
		@remark  Subclasses must implement this
		*/
		virtual void execute();

		/*!
		Receiving Loop Function
		@param[in] lpParam self class object
		@return the thread terminating status
		*/
		static unsigned long ClientThread( LPVOID lpParam ) ;

		/*!
		Actually Disconnect from the server
		@param[in] fromInternal flag to check if the call is from internal or not
		*/
		void disconnect(bool fromInternal);


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

		/// send lock
		BaseLock *m_sendLock;
		/// general lock
		BaseLock *m_generalLock;

		/// disconnect lock
		BaseLock *m_disconnectLock;

		/// Lock Policy
		LockPolicy m_lockPolicy;

		/// Parser list
		ServerObjectList m_parserList;

		/// Maximum UDP Datagram byte size
		unsigned int m_maxPacketSize;
	};
}


#endif //__EP_BASE_CLIENT_UDP_H__