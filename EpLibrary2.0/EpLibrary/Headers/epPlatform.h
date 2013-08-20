/*! 
@file epPlatform.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2011
@brief Platform Dependencies Interface
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

An Interface for the Platform Dependencies.

*/
#ifndef __EP_PLATFORM_H__
#define __EP_PLATFORM_H__

#include "epLib.h"

#define MSVC110 1700
#define MSVC100 1600
#define MSVC90  1500
#define MSVC80  1400
#define MSVC71  1310
#define MSVC70  1300
#define MSVC60  1200
#define MSVC50  1100
#define MSVC40  1000

#define WINDOWS_7          0x0601
#define WINDOWS_SERVER2008      0x0600
#define WINDOWS_VISTA      0x0600
#define WINDOWS_SERVER2003 0x0502
#define WINDOWS_XP         0x0501
#define WINDOWS_2000       0x0500
#define WINDOWS_ME         0x0500
#define WINDOWS_98         0x0410
#define WINDOWS_NT40       0x0400
#define WINDOWS_95         0x0400

#if defined(_WIN32) || defined(_WIN64)

// MSVC++ 9.0   _MSC_VER = 1500
// MSVC++ 8.0   _MSC_VER = 1400
// MSVC++ 7.1   _MSC_VER = 1310
// MSVC++ 7.0   _MSC_VER = 1300
// MSVC++ 6.0   _MSC_VER = 1200
// MSVC++ 5.0   _MSC_VER = 1100
#if _MSC_VER >= MSVC80
// 0x0601 -> Windows 7
// 0x0600 -> Windows Vista
// 0x0502 -> Windows Server 2003
// 0x0501 -> Windows XP
// 0x0500 -> Windows 2000
#ifndef WINVER
#define WINVER WINDOWS_XP
#endif //WINVER

#ifndef _WIN32_WINNT
#define _WIN32_WINNT WINDOWS_XP
#endif //_WIN32_WINNT

#else //_MSC_VER >= 1400
#error EpLibrary Supports only Visual Studio 2005 or higher.
#endif //_MSC_VER >= 1400

#ifndef _WIN32_WINDOWS          // Specifies that the minimum required platform is Windows 98.
#define _WIN32_WINDOWS WINDOWS_XP // Change this to the appropriate value to target Windows Me or later.
#endif //_WIN32_WINDOWS

#ifndef _WIN32_IE                       // Specifies that the minimum required platform is Internet Explorer 7.0.
#define _WIN32_IE 0x0700        // Change this to the appropriate value to target other versions of IE.
#endif //_WIN32_IE

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif //_SECURE_ATL

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif //VC_EXTRALEAN

// The following macros define the minimum required platform.  The minimum required platform
// is the earliest version of Windows, Internet Explorer etc. that has the necessary features to run 
// your application.  The macros work by enabling all features available on platform versions up to and 
// including the version specified.

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some EpTString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif //_AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#pragma warning( disable : 4018 4244 4251 4291 4345 4786 4800 4996 4275 4200)

#else //defined(_WIN32) || defined(_WIN64)

#ifndef TCHAR
#if defined(_UNICODE) || defined(UNICODE)
typedef unsigned short TCHAR 
#else// defined(_UNICODE) || defined(UNICODE)
#define char TCHAR 
#endif// defined(_UNICODE) || defined(UNICODE)
#endif// TCHAR

#ifndef _T
#if defined(_UNICODE) || defined(UNICODE)
#define _T(x) L ## x
#else// defined(_UNICODE) || defined(UNICODE)
#define _T(x) x
#endif//defined(_UNICODE) || defined(UNICODE)
#endif// _T(x)
#endif //defined(_WIN32) || defined(_WIN64)

#endif //__EP_PLATFORM_H__