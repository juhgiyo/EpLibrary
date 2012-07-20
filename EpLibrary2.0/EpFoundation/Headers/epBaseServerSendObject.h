/*! 
@file epBaseServerSendObject.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date July 20, 2012
@brief Base Server Send Object Interface
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

An Interface for Base Server Send Object.

*/
#ifndef __EP_BASE_SERVER_SEND_OBJECT_H__
#define __EP_BASE_SERVER_SEND_OBJECT_H__

#include "epFoundationLib.h"
#include "epSystem.h"

namespace epl{

	/*! 
	@class BaseServerSendObject epBaseServerSendObject.h
	@brief A class for Base Server Object.
	*/
	class EP_FOUNDATION BaseServerSendObject{
	public:
		virtual int Send(const Packet &packet)=0;
	};
}


#endif //__EP_BASE_SERVER_SEND_OBJECT_H__