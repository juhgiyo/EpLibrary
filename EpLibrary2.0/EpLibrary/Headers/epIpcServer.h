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


	class EP_LIBRARY IpcServer:public Thread{
	public:
		IpcServer(epl::LockPolicy lockPolicyType=epl::EP_LOCK_POLICY);
	
		~IpcServer();
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
	private:
		virtual void execute();

		void stopServer();
		
	private:

		vector<Pipe*> m_pipes; 
		vector<HANDLE> m_events; 

		bool m_started;
		IpcServerOps m_options;
		EpTString m_pipeName;
		LockPolicy m_lockPolicy;
	};

}

#endif //__EP_IPC_SERVER_H__