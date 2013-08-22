/*! 
@file epIpcServer.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief IPC Server Interface
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

An Interface for the IPC Server Class.

*/

#ifndef __EP_IPC_SERVER_H__
#define __EP_IPC_SERVER_H__
#include "epLib.h"
#include "epThread.h"
#include "epEventEx.h"
#include "epIpcServerInterfaces.h"
#include "epIpcPipe.h"
#include <vector>

using namespace std;
namespace epl{

	/*! 
	@class IpcServer epIpcServer.h
	@brief A class for IPC Server.
	*/
	class EP_LIBRARY IpcServer:public Thread,public IpcServerInterface{
	public:
		/*!
		Default Constructor

		Initializes the IPC Server
		@param[in] lockPolicyType lock policy
		*/
		IpcServer(epl::LockPolicy lockPolicyType=epl::EP_LOCK_POLICY);
	
		/*!
		Default Destructor

		Destroy the Server
		*/
		virtual ~IpcServer();

		
		/*!
		Get the pipe name of server
		@return the pipe name in string
		*/
		virtual epl::EpTString GetFullPipeName() const;

		/*!
		Get the Maximum Instances of server
		@return the Maximum Instances
		*/
		virtual unsigned int GetMaximumInstances() const;

		/*!
		Set the Callback Object for the server.
		@param[in] callBackObj The Callback Object to set.
		*/
		virtual void SetCallbackObject(IpcServerCallbackInterface *callBackObj);

		/*!
		Get the Callback Object of server
		@return the current Callback Object
		*/
		virtual IpcServerCallbackInterface *GetCallbackObject();

		/*!
		Start the server
		@param[in] ops the server options
		@return true if successfully started otherwise false
		@remark if argument is NULL then previously setting value is used
		*/
		virtual bool StartServer(const IpcServerOps &ops=IpcServerOps::defaultIpcServerOps);
		/*!
		Stop the server
		*/
		virtual void StopServer();

		/*!
		Check if the server is started
		@return true if the server is started otherwise false
		*/
		virtual bool IsServerStarted() const=0;

		/*!
		Terminate all clients' socket connected.
		*/
		virtual void ShutdownAllClient()=0;

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
	private:
		/*!
		Listening Loop Function
		*/
		virtual void execute();

		/*!
		Actually Stop the server
		*/
		void stopServer();
		
	private:
		/// pipe list
		vector<IpcPipe*> m_pipes; 
		/// overlapped event list
		vector<HANDLE> m_events; 

		/// flag whether the server is started
		bool m_started;
		/// IPC server options
		IpcServerOps m_options;
		/// Name of the pipe
		EpTString m_pipeName;
		/// Lock policy
		LockPolicy m_lockPolicy;
		/// pipe list lock
		BaseLock *m_pipesLock;

		/// Server termination event
		EventEx m_serverThreadEvent;
	};

}

#endif //__EP_IPC_SERVER_H__