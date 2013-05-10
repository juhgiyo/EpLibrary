/*! 
@file epIpcServerInterfaces.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief Pipe Server Interfaces
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

An Interface for the IPC Server Interfaces.

*/

#ifndef __EP_IPC_SERVER_INTERFACES_H__
#define __EP_IPC_SERVER_INTERFACES_H__


#include "epLib.h"

namespace epl
{

	/// Read Status
	typedef enum _readStatus{
		/// Success
		READ_STATUS_SUCCESS=0,
		/// Time-out
		READ_STATUS_FAIL_TIME_OUT,
		/// Not connected
		READ_STATUS_FAIL_NOT_CONNECTED,
		/// Connection closing
		READ_STATUS_FAIL_CONNECTION_CLOSING,
		/// Socket error
		READ_STATUS_FAIL_PIPE_ERROR,
		/// Receive failed
		READ_STATUS_FAIL_READ_FAILED,
		/// Not supported
		READ_STATUS_FAIL_NOT_SUPPORTED,

	}ReadStatus;

	/// Write Status
	typedef enum _writeStatus{
		/// Success
		WRITE_STATUS_SUCCESS=0,
		/// Time-out
		WRITE_STATUS_FAIL_TIME_OUT,
		/// Socket error
		WRITE_STATUS_FAIL_PIPE_ERROR,
		/// Send failed
		WRITE_STATUS_FAIL_WRITE_FAILED,
		/// Not connected
		WRITE_STATUS_FAIL_NOT_CONNECTED,
	}WriteStatus;

#define DEFAULT_WRITE_BUF_SIZE 4096
#define DEFAULT_READ_BUF_SIZE 4096

	class IpcServerCallbackInterface;
	/*! 
	@struct ServerOps epPipeServerInterfaces.h
	@brief A class for Server Options.
	*/
	struct EP_LIBRARY IpcServerOps{
		/// Callback Object
		IpcServerCallbackInterface *callBackObj;
		
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
			callBackObj=NULL;
			pipeName=NULL;
			waitTimeInMilliSec=WAITTIME_INIFINITE;
			maximumInstances=PIPE_UNLIMITED_INSTANCES;
			numOfReadBytes=DEFAULT_READ_BUF_SIZE;
			numOfWriteBytes=DEFAULT_WRITE_BUF_SIZE;

		}

		static IpcServerOps defaultIpcServerOps;
	};

	/*! 
	@class IpcServerInterface epIpcServerInterfaces.h
	@brief A class for IPC Server Interface.
	*/
	class EP_LIBRARY IpcServerInterface{
		/*!
		Set the name for the pipe.
		@remark Cannot be changed while connected to server
		@param[in] pipeName The pipe name to set.
		*/
		virtual void SetPipeName(const TCHAR *  pipeName)=0;

		/*!
		Get the pipe name of server
		@return the pipe name in string
		*/
		virtual epl::EpTString GetPipeName() const=0;

		/*!
		Set the Maximum Instances for the pipe.
		@param[in] maxInstances The Maximum Instances to set.
		@remark 0 means there is no limit
		@remark maximum instance cannot exceed 255.
		*/
		virtual void SetMaximumInstances(unsigned int maxInstances)=0;

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
		Terminate all clients' socket connected.
		*/
		virtual void ShutdownAllClient()=0;

		/*!
		Get the maximum packet byte size
		@return the maximum packet byte size
		*/
		virtual unsigned int GetMaxPacketByteSize() const
		{
			return 0;
		}

	};

	
	/*! 
	@class IpcInterface epIpcServerInterfaces.h
	@brief A class for IPC Interface.
	*/
	class EP_LIBRARY IpcInterface{
	public:

		/*!
		Send the packet to the client
		@param[in] packet the packet to be sent
		@param[in] waitTimeInMilliSec wait time for sending the packet in millisecond
		@return sent byte size
		@remark return -1 if error occurred
		*/
		virtual void Send(char *data,unsigned int dataByteSize)=0;
	
		/*!
		Check if the connection is alive
		@return true if the connection is alive otherwise false
		*/
		virtual bool IsConnectionAlive() const=0;

		/*!
		Kill the connection
		*/
		virtual void KillConnection()=0;

	};

	
	/*! 
	@class ServerCallbackInterface epServerInterfaces.h
	@brief A class for Server Callback Interface.
	*/
	class EP_LIBRARY IpcServerCallbackInterface{
	public:
		/*!
		When accepted client tries to make connection.
		@param[in] socket the client socket
		@remark When this function calls, it is right before making connection,<br/>
		        so user can configure the socket before the connection is actually made.		
		*/
		virtual void OnNewConnection(IpcInterface *pipe){}

		/*!
		Received the packet from the client.
		@param[in] socket the client socket which received the packet
		@param[in] receivedPacket the received packet
		@param[in] status the status of Receive
		@remark for Asynchronous Server Use Only!
		*/
		virtual void OnReadComplete(IpcInterface *pipe,const char*receivedData, unsigned int receivedDataByteSize)=0;

		/*!
		The client is disconnected.
		@param[in] socket the client socket, disconnected.
		*/
		virtual void OnDisconnect(IpcInterface *pipe){}
	};


}

#endif //__EP_PIPE_SERVER_INTERFACES_H__