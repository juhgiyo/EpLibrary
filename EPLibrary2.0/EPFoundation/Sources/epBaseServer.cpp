/*! 
BaseServer for the EpLibrary
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
*/
#include "stdafx.h"
#include "epBaseServer.h"

using namespace epl;

BaseServer::BaseServer(CString port)
{
	m_port=NULL;
	SetPort(port);
	m_serverThreadHandle=0;
	m_listenSocket=NULL;
	m_result=0;
	m_isServerStarted=false;
#ifdef EP_MULTIPROCESS
	m_lock=EP_NEW Mutex();
#else //EP_MULTIPROCESS
	m_lock=EP_NEW CriticalSectionEx();
#endif //EP_MULTIPROCESS
}
BaseServer::~BaseServer()
{
	if(m_port)
		EP_DELETE[] m_port;
	StopServer();
	EP_DELETE m_lock;
}

bool BaseServer::SetPort(CString port)
{
	LockObj lock(m_lock);
	if(m_isServerStarted)
		return false;
	if(m_port==NULL)
		m_port=EP_NEW char[PORT_MAX_SIZE];

	memset(m_port,0,PORT_MAX_SIZE);

	if(port.GetLength()==0)
		memcpy(m_port,DEFAULT_PORT,sizeof(DEFAULT_PORT));
	else
	{
		wcstombs(m_port,port.GetString(),port.GetLength());
		m_port[port.GetLength()]='\0';
	}
	return true;
}

CString BaseServer::GetPort()
{
	if(!m_port)
		return _T("");
	CString retString;
	TCHAR port[PORT_MAX_SIZE];
	System::MultiByteToWideChar(m_port,PORT_MAX_SIZE,port);
	retString=port;
	return retString;
}

vector<BaseServerWorker*> BaseServer::GetClientList()
{
	return m_clientList;
}
DWORD BaseServer::ServerThread( LPVOID lpParam ) 
{
	BaseServer *pMainClass=(BaseServer*)lpParam;

	SOCKET clientSocket;
	while(clientSocket=accept(pMainClass->m_listenSocket, NULL, NULL))
	{
		if(clientSocket == INVALID_SOCKET)
		{
			continue;			
		}
		else
		{
			BaseServerWorker *accWorker=pMainClass->createNewWorker();
			pMainClass->GetClientList().push_back(accWorker);
			accWorker->Start((void*)clientSocket);
		}

		vector<BaseServerWorker*>::iterator iter;
		for(iter=pMainClass->m_clientList.begin();iter!=pMainClass->m_clientList.end();)
		{
			if((*iter)->GetStatus()==Thread::THREAD_STATUS_TERMINATED)
			{
				(*iter)->Release();
				iter=pMainClass->m_clientList.erase(iter);
			}
			else
				iter++;

		}
	}
	pMainClass->m_serverThreadHandle=0;
	pMainClass->StopServer();

	return 0; 
} 


bool BaseServer::StartServer()
{
	LockObj lock(m_lock);
	if(m_isServerStarted)
		return true;
	if(m_port==NULL)
	{
		m_port=EP_NEW char[PORT_MAX_SIZE];
		memcpy(m_port,DEFAULT_PORT,sizeof(DEFAULT_PORT));
	}

	WSADATA wsaData;
	int iResult;

	m_listenSocket= INVALID_SOCKET;

	m_result = NULL;


	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {

		MessageBox(NULL,_T("WSAStartup failed with error\n"),_T("Error"),MB_OK);
		return false;
	}

	ZeroMemory(&m_hints, sizeof(m_hints));
	m_hints.ai_family = AF_INET;
	m_hints.ai_socktype = SOCK_STREAM;
	m_hints.ai_protocol = IPPROTO_TCP;
	m_hints.ai_flags = AI_PASSIVE;


	// Resolve the server address and port
	iResult = getaddrinfo(NULL, m_port, &m_hints, &m_result);
	if ( iResult != 0 ) {
		MessageBox(NULL,_T("getaddrinfo failed with error\n"),_T("Error"),MB_OK);
		WSACleanup();
		return false;
	}

	// Create a SOCKET for connecting to server
	m_listenSocket = socket(m_result->ai_family, m_result->ai_socktype, m_result->ai_protocol);
	if (m_listenSocket == INVALID_SOCKET) {
		MessageBox(NULL,_T("socket failed with error\n"),_T("Error"),MB_OK);
		StopServer();
		return false;
	}

	// Setup the TCP listening socket
	iResult = bind( m_listenSocket, m_result->ai_addr, (int)m_result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		MessageBox(NULL,_T("bind failed with error\n"),_T("Error"),MB_OK);
		StopServer();
		return false;
	}

	iResult = listen(m_listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		MessageBox(NULL,_T("listen failed with error\n"),_T("Error"),MB_OK);
		StopServer();
		return false;
	}
	m_isServerStarted=true;

	// Create thread 1.
	m_serverThreadHandle = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)BaseServer::ServerThread, this, 0, NULL);  
	if(m_serverThreadHandle)
		return true;
	return false;


}


void BaseServer::ShutdownAllClient()
{
	LockObj lock(m_lock);
	if(!m_isServerStarted)
		return;
	// shutdown the connection since we're done
	vector<BaseServerWorker*>::iterator iter;
	for(iter=m_clientList.begin();iter!=m_clientList.end();iter++)
	{
		if(*iter)
			(*iter)->Release();
	}
	m_clientList.clear();

}

bool BaseServer::IsServerStarted()
{
	return m_isServerStarted;
}
void BaseServer::StopServer()
{
	LockObj lock(m_lock);
	if(m_isServerStarted==true)
	{
		ShutdownAllClient();	
		// No longer need server socket
	}
	if(m_serverThreadHandle)
		TerminateThread(m_serverThreadHandle,0);
	if(m_listenSocket)
		closesocket(m_listenSocket);
	m_isServerStarted=false;
	if(m_result)
		freeaddrinfo(m_result);
	m_result=NULL;
	m_serverThreadHandle=0;
	WSACleanup();
}


