/*! 
@file epPipe.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief Pipe Interface
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

An Interface for the Pipe Class.

*/

#include "epLib.h"
#include "epEventEx.h"
#include "epIpcServerInterfaces.h"
#include "epSmartObject.h"

namespace epl{

	typedef enum _pipeState{
		PIPE_STATE_CONNECTING=0,
		PIPE_STATE_READING,
		PIPE_STATE_WRITING
	}PipeState;

	class EP_LIBRARY Pipe:public IpcInterface, public SmartObject
	{
		friend class IpcServer;
	public:
		Pipe(EpTString pipeName, IpcServerOps options,epl::LockPolicy lockPolicyType);

		virtual ~Pipe();

		bool Create();
		/*!
		Send the packet to the client
		@param[in] packet the packet to be sent
		@param[in] waitTimeInMilliSec wait time for sending the packet in millisecond
		@return sent byte size
		@remark return -1 if error occurred
		*/
		virtual void Send(char *data,unsigned int dataByteSize);
	
		/*!
		Check if the connection is alive
		@return true if the connection is alive otherwise false
		*/
		virtual bool IsConnectionAlive() const;

		/*!
		Kill the connection
		*/
		virtual void KillConnection();


		HANDLE GetEventHandle();
	private:
		bool connectToNewClient(HANDLE pipeHandle,OVERLAPPED *overlappedIO );
		void disconnect();
		void reconnect();

		static void disconnectAndReconnect(Pipe *pipeInst) ;
		static void OnReadComplete(DWORD dwErr, DWORD cbBytesRead, LPOVERLAPPED lpOverLap);
		static void OnWriteComplete(DWORD dwErr, DWORD cbWritten, LPOVERLAPPED lpOverLap) ;
	private:
		EpTString m_pipeName;
		IpcServerOps m_options;

		PipeState m_pipeState; 
		bool m_pendingIO; 
		OVERLAPPED m_overlap; 
		HANDLE m_pipeHandle; 
		char *m_readBuffer; 
		unsigned int m_bytesRead;
		char *m_writeBuffer;
		unsigned int m_bytesToWrite; 
		EventEx m_pipeEvent;
		
	};
}