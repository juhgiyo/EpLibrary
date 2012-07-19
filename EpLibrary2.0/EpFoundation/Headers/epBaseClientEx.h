/*! 
@file epBaseClientEx.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date February 13, 2012
@brief Base Client Extension Interface
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

An Interface for Base Client Extension.
(Send Receive Independent Model)

*/
#ifndef __EP_BASE_CLIENT_EX_H__
#define __EP_BASE_CLIENT_EX_H__

#include "epFoundationLib.h"
#include "epBaseClient.h"


using namespace std;


namespace epl
{

	/*! 
	@class BaseClientEx epBaseClientEx.h
	@brief A class for Base Client Extension.
	*/
	class EP_FOUNDATION BaseClientEx:public BaseClient
	{

	public:
				/*!
		Default Constructor

		Initializes the Client
		@param[in] hostName the hostname string
		@param[in] port the port string
		@param[in] waitTimeMilliSec the wait time in millisecond for terminating thread
		@param[in] lockPolicyType The lock policy
		*/
		BaseClientEx(const TCHAR * hostName=_T(DEFAULT_HOSTNAME), const TCHAR * port=_T(DEFAULT_PORT), unsigned int waitTimeMilliSec=DEFAULT_WAITTIME,LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the BaseClientEx
		@param[in] b the second object
		*/
		BaseClientEx(const BaseClientEx& b);
		/*!
		Default Destructor

		Destroy the Client
		*/
		virtual ~BaseClientEx();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		BaseClientEx & operator=(const BaseClientEx&b)
		{
			return *this;
		}
		
	
	protected:
		/*!
		Parse the given packet and do relevant operation.
		@remark Subclasses must implement this
		@param[in] packet the packet to parse
		@return status of the process
		*/
		virtual void parsePacket(const Packet &packet )=0;
	private:

		/*!
		Handle the packet parsing thread.
		@param[in] param the packet Pass Unit
		@return status of thread execution
		*/
		static unsigned long __stdcall passPacket(void *param);

		/*! 
		@struct PacketPassUnit epBaseClientEx.h
		@brief A class for Packet Passing Unit for Packet Parsing Thread.
		*/
		struct PacketPassUnit{
			/// BaseClientEx Object
			BaseClientEx *m_this;
			/// Packet to parse
			Packet *m_packet;
		};

		/*!
		Actually processing the client thread
		@remark  Subclasses must implement this
		*/
		virtual void processClientThread();

	};
}


#endif //__EP_BASE_CLIENT_EX_H__