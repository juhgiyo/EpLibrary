/*! 
@file epIpcPipe.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief IPC Pipe Interface
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

An Interface for the IPC Pipe Class.

*/
#ifndef __EP_IPC_PIPE_H__
#define __EP_IPC_PIPE_H__
#include "epLib.h"
#include "epEventEx.h"
#include "epIpcServerInterfaces.h"
#include "epSmartObject.h"
#include <queue>

using namespace std;
 
namespace epl{

	/// Pipe States
	typedef enum _pipeState{
		/// Connecting State
		PIPE_STATE_CONNECTING=0,
		/// Reading State
		PIPE_STATE_READING,
		/// Writing State
		PIPE_STATE_WRITING
	}PipeState;

	
	/*! 
	@class IpcPipe epIpcPipe.h
	@brief A class for IPC Pipe.
	*/
	class EP_LIBRARY IpcPipe:public IpcInterface, public SmartObject
	{
		friend class IpcServer;
	public:
		/*!
		Default Constructor

		Initializes the IPC Server
		@param[in] pipeName the name of the pipe
		@param[in] options the options for the pipe
		@param[in] lockPolicyType lock policy
		*/
		IpcPipe(EpTString pipeName, IpcServerOps options,epl::LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Destructor

		Destroy the Server
		*/
		virtual ~IpcPipe();

		/*!
		Create the pipe
		@return true if successfully created otherwise false
		*/
		bool Create();
		/*!
		Write data to the pipe
		@param[in] data the data to write
		@param[in] dataByteSize byte size of the data
		*/
		virtual void Write(char *data,unsigned int dataByteSize);
	
		/*!
		Check if the connection is alive
		@return true if the connection is alive otherwise false
		*/
		virtual bool IsConnectionAlive() const;

		/*!
		Kill the connection
		*/
		virtual void KillConnection();

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
		Get the event handle for IOOverlap
		@return the event handle
		*/
		HANDLE GetEventHandle();

	private:
		/*!
		Connect to new client
		@param[in] pipeHandle the pipe handle
		@param[in] overlappedIO the overlapped IO structure
		@return true if successfully connected otherwise false
		*/
		bool connectToNewClient(HANDLE pipeHandle,OVERLAPPED *overlappedIO );

		/*!
		Disconnect from the client
		*/
		void disconnect();
		/*!
		Reconnect to new client
		*/
		void reconnect();

		/*!
		Disconnect and reconnect to the new client
		@param[in] pipeInst the pipe instance to disconnect and reconnect
		*/
		static void DisconnectAndReconnect(IpcPipe *pipeInst) ;

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

	
	private:
		/// Name of the pipe
		EpTString m_pipeName;
		/// Pipe options
		IpcServerOps m_options;

		/// Pipe state
		PipeState m_pipeState; 
		/// Flag whether this pipe is in pending for IO
		bool m_pendingIO; 
		/// Overlap structure
		OVERLAPPED m_overlap; 
		/// Pipe handle
		HANDLE m_pipeHandle; 
		/// Read buffer
		char *m_readBuffer; 
		/// Size of bytes read from pipe
		unsigned int m_bytesRead;
		/// Pipe Event
		EventEx m_pipeEvent;
		
		/// Write buffer queue
		queue<PipeWriteElem*> m_writeQueue;

		/// Lock for write buffer queue
		BaseLock *m_writeQueueLock;

		/// Lock Policy
		LockPolicy m_lockPolicy;
	};
}
#endif //__EP_IPC_PIPE_H__