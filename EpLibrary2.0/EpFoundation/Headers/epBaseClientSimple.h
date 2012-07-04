/*! 
@file epBaseClientSimple.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date February 13, 2012
@brief Simple Base Client Interface
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

An Interface for Simple Base Client.
(Send->Receive->Send->Receive Model)

*/
#ifndef __EP_BASE_CLIENT_SIMPLE_H__
#define __EP_BASE_CLIENT_SIMPLE_H__

#include "epFoundationLib.h"
#include "epBaseClient.h"

using namespace std;


namespace epl{

	/*! 
	@class BaseClientSimple epBaseClientSimple.h
	@brief A class for Base Client.
	*/
	class EP_FOUNDATION BaseClientSimple:public BaseClient{

	public:
		/*!
		Default Constructor

		Initializes the Client
		@param[in] hostName the hostname string
		@param[in] port the port string
		@param[in] lockPolicyType The lock policy
		*/
		BaseClientSimple(const TCHAR * hostName=_T(DEFAULT_HOSTNAME), const TCHAR * port=_T(DEFAULT_PORT),LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the BaseClient
		@param[in] b the second object
		*/
		BaseClientSimple(const BaseClientSimple& b);
		/*!
		Default Destructor

		Destroy the Client
		*/
		virtual ~BaseClientSimple();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		BaseClientSimple & operator=(const BaseClientSimple&b)
		{
			return *this;
		}

	protected:
		/*!
		Parse the given packet and do relevant operation.
		@remark  Subclasses must implement this
		@param[in] packet the packet to parse
		*/
		virtual void parsePacket(const Packet &packet )=0;
	private:

		/*!
		Actually processing the client thread
		@remark  Subclasses must implement this
		*/
		virtual void processClientThread();
	};
}


#endif //__EP_BASE_CLIENT_SIMPLE_H__