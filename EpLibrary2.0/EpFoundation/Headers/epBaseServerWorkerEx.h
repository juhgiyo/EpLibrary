/*! 
@file epBaseServerWorkerEx.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date February 13, 2012
@brief Base Worker Extension Interface
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

An Interface for Base Server Worker Extension.
(Send Receive Independent Model)

*/
#ifndef __EP_BASE_SERVER_WORKER_EX_H__
#define __EP_BASE_SERVER_WORKER_EX_H__

#include "epFoundationLib.h"
#include "epBaseServerWorker.h"
#include <vector>

using namespace std;

namespace epl
{
	/*! 
	@class BaseServerWorkerEx epBaseServerWorkerEx.h
	@brief A class for Base Server Worker Extension.
	*/
	class EP_FOUNDATION BaseServerWorkerEx:public BaseServerWorker
	{
	public:
		/*!
		Default Constructor

		Initializes the Worker
		@param[in] waitTimeMilliSec the wait time in millisecond for terminating thread
		@param[in] parserWaitTimeMilliSec the wait time in millisecond for terminating parser thread
		@param[in] lockPolicyType The lock policy
		*/
		BaseServerWorkerEx(unsigned int waitTimeMilliSec=DEFAULT_WAITTIME,unsigned int parserWaitTimeMilliSec=DEFAULT_WAITTIME,LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the BaseServer
		@param[in] b the second object
		*/
		BaseServerWorkerEx(const BaseServerWorkerEx& b);

		/*!
		Default Destructor

		Destroy the Worker
		*/
		virtual ~BaseServerWorkerEx();
		
		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		BaseServerWorkerEx & operator=(const BaseServerWorkerEx&b)
		{
			if(this!=&b)
			{
				BaseServerWorker::operator =(b);
				m_parserWaitTime=b.m_parserWaitTime;
			}
			return *this;
		}

		/*!
		Set the wait time for the parser thread termination
		@param[in] milliSec the time for waiting in millisecond
		*/
		void SetWaitTimeForParserTerminate(unsigned int milliSec);

		/*!
		Get the wait time for the parser thread termination
		@return the current time for waiting in millisecond
		*/
		unsigned int GetWaitTimeForParserTerminate();
	protected:
		/*!
		Parse the given packet and do relevant operation.
		@remark Subclasses must implement this
		@param[in] packet the packet to parse
		*/
		virtual void parsePacket(const Packet &packet)=0;

	private:


		/*!
		thread loop function
		*/
		virtual void execute();
	
		/*!
		Handle the packet parsing thread.
		@param[in] param the packet Pass Unit
		@return status of thread execution
		*/
		static unsigned long __stdcall passPacket(void *param);

		/*! 
		@struct PacketPassUnit epBaseServerWorkerEx.h
		@brief A class for Packet Passing Unit for Packet Parsing Thread.
		*/
		struct PacketPassUnit{
			/// BaseServerWorkerEx Object
			BaseServerWorkerEx *m_this;
			/// Packet to parse
			Packet *m_packet;
		};

		/// list lock
		BaseLock *m_listLock;

		/// parser thread list
		vector<HANDLE> m_parserList;

		/// wait time in millisecond for terminating parser thread
		unsigned int m_parserWaitTime;

	};

}

#endif //__EP_BASE_SERVER_WORKER_EX_H__