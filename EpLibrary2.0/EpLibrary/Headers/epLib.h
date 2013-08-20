/*! 
@file epLib.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 16, 2011
@brief EpLibrary Interface
@version 2.0

@section LICENSE

The MIT License (MIT)

Copyright (c) 2012-2013 Woong Gyu La <juhgiyo@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

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


#if defined(_DEBUG)
#define EP_ENABLE_PROFILE
#define EP_ENABLE_LOG
#define EP_ENABLE_CRTDBG

/// Limits the usage of the synchronization objects<br/>
/// Uncomment below line and recompile if you really want this functinality
// #define ENABLE_POSSIBLE_DEADLOCK_CHECK

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

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#ifndef _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif // _CRTDBG_MAP_ALLOC
#endif //defined(_DEBUG)

#include "epPlatform.h"

#endif //__EP_LIB_H__