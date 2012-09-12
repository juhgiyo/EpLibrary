/*! 
@file epLib.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 16, 2011
@brief EpLibrary Interface
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

An Interface for EpLibrary.

*/

#ifndef __EP_LIB_H__
#define __EP_LIB_H__

#ifdef EP_LIBRARY_DLL_EXPORT

#define EP_LIBRARY __declspec(dllexport)
#define EP_LOCK_POLICY LOCK_POLICY_MUTEX

#else   //EP_LIBRARY_DLL_EXPORT

#ifdef EP_LIBRARY_DLL_IMPORT

#define EP_LIBRARY __declspec(dllimport)
#define EP_LOCK_POLICY LOCK_POLICY_MUTEX

#else   //EP_LIBRARY_DLL_IMPORT

#define EP_LIBRARY
#define EP_LOCK_POLICY LOCK_POLICY_CRITICALSECTION

#endif  //EP_LIBRARY_DLL_IMPORT
#endif  //EP_LIBRARY_DLL_EXPORT


#ifdef _DEBUG
#define EP_ENABLE_PROFILE
#define EP_ENABLE_LOG
#endif //_DEBUG

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#define __WFUNCTION__ WIDEN(__FUNCTION__)
#if defined(_UNICODE) || defined(UNICODE)
#define __TFILE__ __WFILE__
#define __TFUNCTION__ __WFUNCTION__
#else//defined(_UNICODE) || defined(UNICODE)
#define __TFILE__ __FILE__
#define __TFUNCTION__ __FUNCTION__
#endif//defined(_UNICODE) || defined(UNICODE)

#ifndef NULL
#define NULL 0
#endif

#include "epPlatform.h"
#endif //__EP_LIB_H__