/*! 
@file epIpcConf.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief IPC Configuration Interfaces
@version 2.0

@section LICENSE

The MIT License (MIT)

Copyright (c) 2008-2013 Woong Gyu La <juhgiyo@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

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