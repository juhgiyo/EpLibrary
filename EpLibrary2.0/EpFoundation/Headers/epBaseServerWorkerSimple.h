/*! 
@file epBaseServerWorkerSimple.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date February 13, 2012
@brief Simple Base Worker Interface
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

An Interface for Simple Base Worker.
(Send->Receive->Send->Receive Model)

*/
#ifndef __EP_BASE_SERVER_WORKER_SIMPLE_H__
#define __EP_BASE_SERVER_WORKER_SIMPLE_H__

#include "epFoundationLib.h"
#include "epBaseServerWorker.h"
namespace epl
{
	/*! 
	@class BaseServerWorkerSimple epBaseServerWorkerSimple.h
	@brief A class for Simple Base Server Worker.
	*/
	class EP_FOUNDATION BaseServerWorkerSimple:public BaseServerWorker
	{
		
	public:
		/*!
		Default Constructor

		Initializes the Worker
		@param[in] lockPolicyType The lock policy
		*/
		BaseServerWorkerSimple(unsigned int waitTimeMilliSec=DEFAULT_WAITTIME,LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the BaseServer
		@param[in] b the second object
		*/
		BaseServerWorkerSimple(const BaseServerWorkerSimple& b);

		/*!
		Default Destructor

		Destroy the Worker
		*/
		virtual ~BaseServerWorkerSimple();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		BaseServerWorkerSimple & operator=(const BaseServerWorkerSimple&b)
		{
			if(this!=&b)
			{
				BaseServerWorker::operator =(b);
			}
			return *this;
		}

	protected:
		/*!
		Parse the given packet and do relevant operation
		@remark  Subclasses must implement this
		@param[in] packet the packet to parse
		*/
		virtual void parsePacket(const Packet &packet)=0;

	private:
		/*!
		thread loop function
		*/
		virtual void execute();
	};

}

#endif //__EP_BASE_SERVER_WORKER_SIMPLE_H__