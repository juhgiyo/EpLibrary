/*! 
@file epBaseCallbackObject.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date July 20, 2012
@brief Base Server Callback Object Interface
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

An Interface for Base Callback Object.

*/
#ifndef __EP_BASE_CALLBACK_OBJECT_H__
#define __EP_BASE_CALLBACK_OBJECT_H__

#include "epFoundationLib.h"
#include "epSystem.h"
#include "epSmartObject.h"
namespace epl{
	class Thread;
	/*! 
	@class BaseCallbackObject epBaseCallbackObject.h
	@brief A class for Base Callback Object.
	*/
	class EP_FOUNDATION BaseCallbackObject:public SmartObject
	{
	public:
		/*!
		Default Constructor

		Initializes the Object
		@param[in] lockPolicyType The lock policy
		*/
		BaseCallbackObject(LockPolicy lockPolicyType=EP_LOCK_POLICY):SmartObject(lockPolicyType)
		{
		}

		/*!
		Default Copy Constructor

		Initializes the Object
		@param[in] b the second object
		*/
		BaseCallbackObject(const BaseCallbackObject& b):SmartObject(b)
		{
		}
		/*!
		Default Destructor

		Destroy the Object
		*/
		virtual ~BaseCallbackObject()
		{

		}

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		BaseCallbackObject & operator=(const BaseCallbackObject&b)
		{
			if(this!=&b)
			{
				SmartObject::operator =(b);
			}
			return *this;
		}

		/*!
		Call back function
		@param[in] terminatedThread the thread object that terminated
		@remark Sub-class should implement this to get call back.
		*/
		virtual void Callback(Thread* terminatedThread)=0;
	};
}


#endif //__EP_BASE_CALLBACK_OBJECT_H__