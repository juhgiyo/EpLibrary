/*! 
BaseServerWorker for the EpLibrary
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
#include "epBaseServerWorker.h"

using namespace epl;
BaseServerWorker::BaseServerWorker(): Thread(), SmartObject()
{
}

BaseServerWorker::~BaseServerWorker()
{
	int iResult;
	iResult = shutdown(m_clientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error\n"); // TODO:: Log
		closesocket(m_clientSocket);
	}
}

void BaseServerWorker::SetArg(void* a)
{
	SOCKET clientSocket=(SOCKET)a;
	m_clientSocket=clientSocket;
}

int BaseServerWorker::Send(const Packet &packet)
{
	
	LockObj lock(&m_sendLock);
	int writeLength=0;
	const char *packetData=packet.GetPacket();
	int length=packet.GetPacketByteSize();
	if(length>0)
	{
		int sentLength=send(m_clientSocket,(char*)&length,4,0);
		if(sentLength<=0)
			return false;
	}
	while(length>0)
	{
		int sentLength=send(m_clientSocket,packetData,length,0);
		writeLength+=sentLength;
		if(writeLength<=0)
			break;
		length-=sentLength;
		packetData+=sentLength;
	}
	return writeLength;
}

int BaseServerWorker::receive(Packet &packet)
{
	int readLength=0;
	int length=packet.GetPacketByteSize();
	char *packetData=(char*)packet.GetPacket();
	while(length>0)
	{
		int recvLength=recv(m_clientSocket,packetData, length, 0);
		readLength+=recvLength;
		if(recvLength<=0)
			break;
		length-=recvLength;
		packetData+=recvLength;
	}
	return readLength;
}


void BaseServerWorker::execute()
{
	int iResult;
	Packet recvSizePacket(NULL,4);
	
	// Receive until the peer shuts down the connection
	do {
		int size =receive(recvSizePacket);
		if(size>0)
		{
			unsigned int shouldReceive=((unsigned int*)(recvSizePacket.GetPacket()))[0];
			Packet recvPacket(NULL,shouldReceive);
			iResult = receive(recvPacket);

			if (iResult == shouldReceive) {
				// Process Received Packet and Send Result to Client
				if(parsePacket(recvPacket)<=0)
				{
					break;
				}
			}
			else if (iResult == 0)
			{
				printf("Connection closing...\n"); // TODO:: LOG
				break;
			}
			else  {
				printf("recv failed with error\n"); // TODO:: LOG
				break;
			}
		}
		else
		{
			break;
		}

	} while (iResult > 0);
}
