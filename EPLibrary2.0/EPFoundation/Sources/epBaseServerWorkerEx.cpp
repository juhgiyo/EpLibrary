/*! 
BaseServerWorkerEx for the EpLibrary
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
#include "epBaseServerWorkerEx.h"

using namespace epl;
BaseServerWorkerEx::BaseServerWorkerEx(): Thread(), SmartObject()
{
}

BaseServerWorkerEx::~BaseServerWorkerEx()
{
	int iResult;
	iResult = shutdown(m_clientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error\n"); // TODO:: Log
		closesocket(m_clientSocket);
	}
}

void BaseServerWorkerEx::SetArg(void* a)
{
	SOCKET clientSocket=(SOCKET)a;
	m_clientSocket=clientSocket;
}

int BaseServerWorkerEx::Send(const Packet &packet)
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

int BaseServerWorkerEx::receive(Packet &packet)
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

unsigned long BaseServerWorkerEx::passPacket(void *param)
{
	Packet *recvPacket=( (PacketPassUnit*)param)->m_packet;
	BaseServerWorkerEx *worker=( (PacketPassUnit*)param)->m_this;
	worker->parsePacket(*recvPacket);
	recvPacket->Release();
	return 0;
}

void BaseServerWorkerEx::execute()
{
	int iResult;
	Packet recvSizePacket(NULL,4);

	// Receive until the peer shuts down the connection
	do {
		int size =receive(recvSizePacket);
		if(size>0)
		{
			unsigned int shouldReceive=((unsigned int*)(recvSizePacket.GetPacket()))[0];
			Packet *recvPacket=new Packet(NULL,shouldReceive);
			iResult = receive(*recvPacket);

			if (iResult == shouldReceive) {
				ThreadID threadID;
				PacketPassUnit passUnit;
				passUnit.m_packet=recvPacket;
				passUnit.m_this=this;
				::CreateThread(NULL,0,passPacket,&passUnit,THREAD_OPCODE_CREATE_START,(LPDWORD)&threadID);
			}
			else if (iResult == 0)
			{
				printf("Connection closing...\n"); // TODO:: LOG
				recvPacket->Release();
				break;
			}
			else  {
				printf("recv failed with error\n"); // TODO:: LOG
				recvPacket->Release();
				break;
			}
		}
		else
		{
			break;
		}

	} while (iResult > 0);
}
