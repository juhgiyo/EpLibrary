/*! 
@file epPlatform.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2011
@brief Platform Dependencies Interface
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

An Interface for the Platform Dependencies.

*/
#ifndef __EP_PLATFORM_H__
#define __EP_PLATFORM_H__

#include "epLib.h"

#if defined(_WIN32) || defined(_WIN64)

#if _MSC_VER > 1400
// 0x0600 -> Windows Vista
// 0x0502 -> Windows Server 2003
// 0x0501 -> Windows XP
// 0x0500 -> Windows 2000
// 0x0601 -> Windows 7
#ifndef WINVER
#define WINVER 0x0601
#endif //WINVER

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif //_WIN32_WINNT
#endif

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

// The following macros define the minimum required platform.  The minimum required platform
// is the earliest version of Windows, Internet Explorer etc. that has the necessary features to run 
// your application.  The macros work by enabling all features available on platform versions up to and 
// including the version specified.

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER                          // Specifies that the minimum required platform is Windows Vista.
#define WINVER 0x0600           // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINNT 0x0600     // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINDOWS          // Specifies that the minimum required platform is Windows 98.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE                       // Specifies that the minimum required platform is Internet Explorer 7.0.
#define _WIN32_IE 0x0700        // Change this to the appropriate value to target other versions of IE.
#endif


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some EpTString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#pragma warning( disable : 4018 4244 4251 4291 4786 4800 4996 4275 4200)

#else //_WIN32

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
#endif //_WIN32

#endif //__EP_PLATFORM_H__