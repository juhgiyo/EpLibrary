/*! 
@file epBaseServerSendObject.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date July 20, 2012
@brief Base Server Send Object Interface
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

An Interface for Base Server Send Object.

*/
#ifndef __EP_BASE_SERVER_SEND_OBJECT_H__
#define __EP_BASE_SERVER_SEND_OBJECT_H__

#include "epFoundationLib.h"
#include "epSystem.h"
#include "epBaseServerObject.h"
namespace epl{

	/*! 
	@class BaseServerSendObject epBaseServerSendObject.h
	@brief A class for Base Server Send Object.
	*/
	class EP_FOUNDATION BaseServerSendObject:public BaseServerObject{
	public:
			/*!
		Default Constructor

		Initializes the Object
		@param[in] callbackObj the callback object to call when thread stops
		@param[in] lockPolicyType The lock policy
		*/
		BaseServerSendObject(BaseServerCallbackObject *callbackObj=NULL,LockPolicy lockPolicyType=EP_LOCK_POLICY):BaseServerObject(callbackObj,lockPolicyType)
		{
		}

		/*!
		Default Copy Constructor

		Initializes the Object
		@param[in] b the second object
		*/
		BaseServerSendObject(const BaseServerObject& b):BaseServerObject(b)
		{
		}
		/*!
		Default Destructor

		Destroy the Object
		*/
		virtual ~BaseServerSendObject(){}

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		BaseServerSendObject & operator=(const BaseServerSendObject&b)
		{
			if(this!=&b)
			{
				BaseServerObject::operator=(b);
			}
			return *this;
		}

		virtual int Send(const Packet &packet)=0;
	};
}


#endif //__EP_BASE_SERVER_SEND_OBJECT_H__