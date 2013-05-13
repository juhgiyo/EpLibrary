/*! 
@file epIpcConf.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief IPC Configuration Interfaces
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

An Interface for the IPC Configuration Interfaces.

*/
#ifndef __EP_IPC_CONF_H__
#define __EP_IPC_CONF_H__

#include "epLib.h"
#include "epSmartObject.h"
namespace epl
{

#define DEFAULT_WRITE_BUF_SIZE 4096
#define DEFAULT_READ_BUF_SIZE 4096


	/// Connect Status
	typedef enum _connectStatus{
		/// Success
		CONNECT_STATUS_SUCCESS=0,
		/// Pipe open failed
		CONNECT_STATUS_FAIL_PIPE_OPEN_FAILED,
		/// SetNamedPipeHandleState failed
		CONNECT_STATUS_FAIL_SET_NAMED_PIPE_HANDLE_STATE,
		/// Read failed
		CONNECT_STATUS_FAIL_READ_FAILED,
		/// Timed Out
		CONNECT_STATUS_FAIL_TIME_OUT,
	}ConnectStatus;


	/// Read Status
	typedef enum _readStatus{
		/// Success
		READ_STATUS_SUCCESS=0,
		/// Receive failed
		READ_STATUS_FAIL_READ_FAILED,
	}ReadStatus;

	/// Write Status
	typedef enum _writeStatus{
		/// Success
		WRITE_STATUS_SUCCESS=0,
		/// Send failed
		WRITE_STATUS_FAIL_WRITE_FAILED,
	}WriteStatus;

	/*! 
	@class PipeWriteElem epIpcPipe.h
	@brief A class for IPC Write Element.
	*/
	struct EP_LIBRARY PipeWriteElem:public SmartObject{
		/*!
		Default Constructor

		Initializes the IPC Pipe Write Element
		@param[in] lockPolicyType lock policy
		*/
		PipeWriteElem(epl::LockPolicy lockPolicyType=EP_LOCK_POLICY);
		/*!
		Default Constructor

		Initializes the IPC Pipe Write Element
		@param[in] dataSize the byte size of the data
		@param[in] lockPolicyType lock policy
		*/
		PipeWriteElem(unsigned int dataSize,epl::LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Destructor

		Destroy the Write Element
		*/
		virtual ~PipeWriteElem();
		
		/// Byte size of the data
		unsigned int m_dataSize;
		/// Data buffer
		char *m_data;
	};
}
#endif //__EP_IPC_CONF_H__