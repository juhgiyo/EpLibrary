/*! 
@file epLib.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date April 16, 2011
@brief A EP Library Interface
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

An Interface for EP Library.

*/

#ifndef __EP_LIB_H__
#define __EP_LIB_H__

#ifdef EP_FOUNDATION_DLL_EXPORT

#define EP_FOUNDATION __declspec(dllexport)
#define EP_LOCK_POLICY LOCK_POLICY_MUTEX

#else   //EP_FOUNDATION_DLL_EXPORT

#ifdef EP_FOUNDATION_DLL_EXPORT

#define EP_FOUNDATION __declspec(dllimport)
#define EP_LOCK_POLICY LOCK_POLICY_MUTEX

#else   //EP_FOUNDATION_DLL_EXPORT

#define EP_FOUNDATION
#define EP_LOCK_POLICY LOCK_POLICY_CRITICALSECTION

#endif  //EP_FOUNDATION_DLL_EXPORT
#endif  //EP_FOUNDATION_DLL_EXPORT

namespace epl{
	#define WIDEN2(x) L ## x
	#define WIDEN(x) WIDEN2(x)
	#define __WFILE__ WIDEN(__FILE__)
	#define __WFUNCTION__ WIDEN(__FUNCTION__)

	#ifndef NULL
	#define NULL 0
	#endif

}

#endif //__EP_LIB_H__