/*! 
@file epIpcServerInterfaces.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief IPC Server Interfaces
@version 2.0

@section LICENSE

The MIT License (MIT)

Copyright (c) 2012-2013 Woong Gyu La <juhgiyo@gmail.com>

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

An Interface for the IPC Server Interfaces.

*/

#ifndef __EP_IPC_SERVER_INTERFACES_H__
#define __EP_IPC_SERVER_INTERFACES_H__


#include "epLib.h"
#include "epIpcConf.h"

namespace epl
{

	class IpcServerCallbackInterface;
	/*! 
	@struct ServerOps epIpcServerInterfaces.h
	@brief A class for IPC Server Options.
	*/
	struct EP_LIBRARY IpcServerOps{
		/// Callback Object
		IpcServerCallbackInterface *callBackObj;
		
		/// Domain
		TCHAR *domain;
		/// Name of the pipe
		TCHAR *pipeName;
		/// Wait time in millisecond for pipe threads
		unsigned int waitTimeInMilliSec;
		///The maximum possible number of pipe instances
		unsigned int maximumInstances;
		/// read byte size
		unsigned int numOfReadBytes;
		/// write byte size
		unsigned int numOfWriteBytes;

		/*!
		Default Constructor

		Initializes the Server Options
		*/
		IpcServerOps()
		{
			domain=_T(".");
			callBackObj=NULL;
			pipeName=NULL;
			waitTimeInMilliSec=WAITTIME_INIFINITE;
			maximumInstances=PIPE_UNLIMITED_INSTANCES;
			numOfReadBytes=DEFAULT_READ_BUF_SIZE;
			numOfWriteBytes=DEFAULT_WRITE_BUF_SIZE;

		}

		/// Default IPC Server options
		static IpcServerOps defaultIpcServerOps;
	};

	/*! 
	@class IpcServerInterface epIpcServerInterfaces.h
	@brief A class for IPC Server Interface.
	*/
	class EP_LIBRARY IpcServerInterface{

		/*!
		Get the pipe name of server
		@return the pipe name in string
		*/
		virtual epl::EpTString GetFullPipeName() const=0;

		/*!
		Get the Maximum Instances of server
		@return the Maximum Instances
		*/
		virtual unsigned int GetMaximumInstances() const=0;

		/*!
		Set the Callback Object for the server.
		@param[in] callBackObj The Callback Object to set.
		*/
		virtual void SetCallbackObject(IpcServerCallbackInterface *callBackObj)=0;

		/*!
		Get the Callback Object of server
		@return the current Callback Object
		*/
		virtual IpcServerCallbackInterface *GetCallbackObject()=0;

		/*!
		Start the server
		@param[in] ops the server options
		@return true if successfully started otherwise false
		@remark if argument is NULL then previously setting value is used
		*/
		virtual bool StartServer(const IpcServerOps &ops=IpcServerOps::defaultIpcServerOps)=0;
		/*!
		Stop the server
		*/
		virtual void StopServer()=0;

		/*!
		Check if the server is started
		@return true if the server is started otherwise false
		*/
		virtual bool IsServerStarted() const=0;

		/*!
		Terminate all clients' pipe connected.
		*/
		virtual void ShutdownAllClient()=0;

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
	@class IpcInterface epIpcServerInterfaces.h
	@brief A class for IPC Interface.
	*/
	class EP_LIBRARY IpcInterface{
	public:

		/*!
		Write data to the pipe
		@param[in] data the data to write
		@param[in] dataByteSize byte size of the data
		*/
		virtual void Write(char *data,unsigned int dataByteSize)=0;
	
		/*!
		Check if the connection is alive
		@return true if the connection is alive otherwise false
		*/
		virtual bool IsConnectionAlive() const=0;

		/*!
		Kill the connection
		*/
		virtual void KillConnection()=0;

		/*!
		Set the Callback Object for the server.
		@param[in] callBackObj The Callback Object to set.
		*/
		virtual void SetCallbackObject(IpcServerCallbackInterface *callBackObj)=0;

		/*!
		Get the Callback Object of server
		@return the current Callback Object
		*/
		virtual IpcServerCallbackInterface *GetCallbackObject()=0;

	};

	
	/*! 
	@class IpcServerCallbackInterface epIpcServerInterfaces.h
	@brief A class for Server Callback Interface.
	*/
	class EP_LIBRARY IpcServerCallbackInterface{
	public:
		/*!
		When accepted client tries to make connection.
		@param[in] pipe the pipe
		@remark When this function calls, it is right before making connection,<br/>
		        so user can configure the pipe before the connection is actually made.		
		*/
		virtual void OnNewConnection(IpcInterface *pipe){}

		/*!
		Received the data from the client.
		@param[in] pipe the pipe which received the packet
		@param[in] receivedData the received data
		@param[in] receivedDataByteSize the received data byte size
		@param[in] status the status of read
		@param[in] errCode the error code
		*/
		virtual void OnReadComplete(IpcInterface *pipe,const char*receivedData, unsigned int receivedDataByteSize, ReadStatus status, unsigned long errCode)=0;

		/*!
		Received the packet from the client.
		@param[in] pipe the pipe which wrote the packet
		@param[in] writtenDataByteSize the byte size of data written
		@param[in] status the status of write
		@param[in] errCode the error code
		*/
		virtual void OnWriteComplete(IpcInterface *pipe,unsigned int writtenDataByteSize, WriteStatus status, unsigned long errCode){}

		/*!
		The pipe is disconnected.
		@param[in] pipe the pipe, disconnected.
		*/
		virtual void OnDisconnect(IpcInterface *pipe){}
	};


}

#endif //__EP_PIPE_SERVER_INTERFACES_H__