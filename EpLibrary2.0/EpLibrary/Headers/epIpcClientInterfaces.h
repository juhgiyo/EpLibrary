/*! 
@file epIpcClientInterfaces.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief IPC Client Interfaces
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

An Interface for the IPC Client Interfaces.

*/

#ifndef __EP_IPC_CLIENT_INTERFACES_H__
#define __EP_IPC_CLIENT_INTERFACES_H__


#include "epLib.h"
#include "epIpcConf.h"
namespace epl
{


	class IpcClientCallbackInterface;
	/*! 
	@struct ServerOps epIpcServerInterfaces.h
	@brief A class for IPC Server Options.
	*/
	struct EP_LIBRARY IpcClientOps{
		/// Callback Object
		IpcClientCallbackInterface *callBackObj;
		
		/// Domain
		TCHAR *domain;
		/// Name of the pipe
		TCHAR *pipeName;
		/// Wait time in millisecond for pipe threads
		unsigned int waitTimeInMilliSec;
		/// read byte size
		unsigned int numOfReadBytes;
		/// write byte size
		unsigned int numOfWriteBytes;

		/*!
		Default Constructor

		Initializes the Server Options
		*/
		IpcClientOps()
		{
			domain=_T(".");
			callBackObj=NULL;
			pipeName=NULL;
			waitTimeInMilliSec=WAITTIME_INIFINITE;
			numOfReadBytes=DEFAULT_READ_BUF_SIZE;
			numOfWriteBytes=DEFAULT_WRITE_BUF_SIZE;

		}

		/// Default IPC Server options
		static IpcClientOps defaultIpcClientOps;
	};

	/*! 
	@class IpcClientInterface epIpcClientInterfaces.h
	@brief A class for IPC Client Interface.
	*/
	class EP_LIBRARY IpcClientInterface{

		/*!
		Get the pipe name of server
		@return the pipe name in string
		*/
		virtual epl::EpTString GetFullPipeName() const=0;

		/*!
		Set the Callback Object for the server.
		@param[in] callBackObj The Callback Object to set.
		*/
		virtual void SetCallbackObject(IpcClientCallbackInterface *callBackObj)=0;

		/*!
		Get the Callback Object of server
		@return the current Callback Object
		*/
		virtual IpcClientCallbackInterface *GetCallbackObject()=0;

		/*!
		Connect to the server
		@param[in] ops the server options
		@return Connect status
		@remark if argument is NULL then previously setting value is used
		*/
		virtual ConnectStatus Connect(const IpcClientOps &ops=IpcClientOps::defaultIpcClientOps)=0;
		/*!
		Disconnect from the server
		*/
		virtual void Disconnect()=0;

		/*!
		Check if the client is connected to server
		@return true if the client is connected to server otherwise false
		*/
		virtual bool IsConnected() const=0;

		/*!
		Get the maximum write data byte size
		@return the maximum write data byte size
		*/
		virtual unsigned int GetMaxWriteDataByteSize() const=0;
		/*!
		Get the maximum read data byte size
		@return the maximum read data byte size
		*/
		virtual unsigned int GetMaxReadDataByteSize() const=0;
		
	};

	
	/*! 
	@class IpcClientCallbackInterface epIpcClientInterfaces.h
	@brief A class for Server Callback Interface.
	*/
	class EP_LIBRARY IpcClientCallbackInterface{
	public:
		/*!
		Received the data from the client.
		@param[in] pipe the pipe which received the packet
		@param[in] receivedData the received data
		@param[in] receivedDataByteSize the received data byte size
		@param[in] status the status of read
		@param[in] errCode the error code
		*/
		virtual void OnReadComplete(IpcClientInterface *pipe,const char*receivedData, unsigned int receivedDataByteSize, ReadStatus status, unsigned long errCode)=0;

		/*!
		Received the packet from the client.
		@param[in] pipe the pipe which wrote the packet
		@param[in] writtenDataByteSize the byte size of data written
		@param[in] status the status of write
		@param[in] errCode the error code
		*/
		virtual void OnWriteComplete(IpcClientInterface *pipe,unsigned int writtenDataByteSize, WriteStatus status, unsigned long errCode){}

		/*!
		The pipe is disconnected.
		@param[in] pipe the pipe, disconnected.
		*/
		virtual void OnDisconnect(IpcClientInterface *pipe){}
	};


}

#endif //__EP_IPC_CLIENT_INTERFACES_H__