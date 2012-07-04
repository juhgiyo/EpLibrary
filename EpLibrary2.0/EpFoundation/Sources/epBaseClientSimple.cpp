/*! 
BaseClientSimple for the EpLibrary
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
#include "epBaseClientSimple.h"
using namespace epl;

BaseClientSimple::BaseClientSimple(const TCHAR * hostName, const TCHAR * port,LockPolicy lockPolicyType):BaseClient(hostName,port,lockPolicyType)
{
}

BaseClientSimple::BaseClientSimple(const BaseClientSimple& b):BaseClient(b)
{

}
BaseClientSimple::~BaseClientSimple()
{
}



void BaseClientSimple::processClientThread() 
{
	int iResult;
	/// Receive buffer
	Packet recvSizePacket(NULL,4);
	// Receive until the peer shuts down the connection
	do {
		int size =receive(recvSizePacket);
		if(size>0)
		{
			unsigned int shouldReceive=(reinterpret_cast<unsigned int*>(const_cast<char*>(recvSizePacket.GetPacket())))[0];
			Packet recvPacket(NULL,shouldReceive);
			iResult = receive(recvPacket);

			if (iResult == shouldReceive) {
				// Process Received Packet and Send Result to Client
				parsePacket(recvPacket);
			}
			else if (iResult == 0)
			{
				LOG_THIS_MSG(_T("Connection closing...\n"));
				break;
			}
			else  {
				LOG_THIS_MSG(_T("recv failed with error\n"));
				break;
			}
		}
		else
		{
			break;
		}

	} while (iResult > 0);
}

