/*! 
@file epIpcServer.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief IPC Server Interface
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