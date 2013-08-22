/*! 
@file epIpcClient.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief IPC Client Interface
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

An Interface for the IPC Client Class.

*/
#ifndef __EP_IPC_CLIENT_H__
#define __EP_IPC_CLIENT_H__
#include "epLib.h"
#include "epIpcClientInterfaces.h"
#include <queue>

using namespace std;

namespace epl{

	/*! 
	@class IpcClient epIpcClient.h
	@brief A class for IPC Client.
	*/
	class EP_LIBRARY IpcClient:public IpcClientInterface
	{
		/*!
		Default Constructor

		Initializes the IPC Client
		@param[in] lockPolicyType lock policy
		*/
		IpcClient(epl::LockPolicy lockPolicyType=epl::EP_LOCK_POLICY);

		/*!
		Default Destructor

		Destroy the Client
		*/
		virtual ~IpcClient();

			/*!
		Get the pipe name of server
		@return the pipe name in string
		*/
		virtual epl::EpTString GetFullPipeName() const;

		/*!
		Set the Callback Object for the server.
		@param[in] callBackObj The Callback Object to set.
		*/
		virtual void SetCallbackObject(IpcClientCallbackInterface *callBackObj);

		/*!
		Get the Callback Object of server
		@return the current Callback Object
		*/
		virtual IpcClientCallbackInterface *GetCallbackObject();

		/*!
		Start the server
		@param[in] ops the server options
		@param[in] waitTimeInMilliSec the wait time for connection in milli-second.
		@return connect status
		@remark if argument is NULL then previously setting value is used
		*/
		virtual ConnectStatus Connect(const IpcClientOps &ops=IpcClientOps::defaultIpcClientOps, unsigned int waitTimeInMilliSec=0);
		/*!
		Stop the server
		*/
		virtual void Disconnect();

		/*!
		Check if the server is started
		@return true if the server is started otherwise false
		*/
		virtual bool IsConnected() const=0;

		/*!
		Get the maximum write data byte size
		@return the maximum write data byte size
		*/
		virtual unsigned int GetMaxWriteDataByteSize() const;
		/*!
		Get the maximum read data byte size
		@return the maximum read data byte size
		*/
		virtual unsigned int GetMaxReadDataByteSize() const;

		/*!
		Write data to the pipe
		@param[in] data the data to write
		@param[in] dataByteSize byte size of the data
		*/
		virtual void Write(char *data,unsigned int dataByteSize)=0;
	

	private:

		/*!
		Handles when Read is completed
		@param[in] dwErr the error code
		@param[in] cbBytesRead the bytes read
		@param[in] lpOverLap the overlapped IO structure
		@remark lpOverLap is actually pointer to the pipe instance
		*/
		static void OnReadComplete(DWORD dwErr, DWORD cbBytesRead, LPOVERLAPPED lpOverLap);

		/*!
		Handles when Write is completed
		@param[in] dwErr the error code
		@param[in] cbWritten the bytes written
		@param[in] lpOverLap the overlapped IO structure
		@remark lpOverLap is actually pointer to the pipe instance
		*/
		static void OnWriteComplete(DWORD dwErr, DWORD cbWritten, LPOVERLAPPED lpOverLap) ;

		HANDLE m_pipeHandle;
		/// flag whether the server is started
		bool m_connected;
		/// IPC server options
		IpcClientOps m_options;
		/// Name of the pipe
		EpTString m_pipeName;
		/// Lock policy
		LockPolicy m_lockPolicy;

		/// Write buffer queue
		queue<PipeWriteElem*> m_writeQueue;
		/// Read buffer
		char *m_readBuffer; 
		/// Size of bytes read from pipe
		unsigned int m_bytesRead;

		/// Lock for write buffer queue
		BaseLock *m_writeQueueLock;

	};
}

#endif //__EP_IPC_CLIENT_H__