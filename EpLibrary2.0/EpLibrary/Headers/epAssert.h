/*! 
@file epAssert.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2011
@brief Assert Interface
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

An Interface for Assert.

*/
#ifndef __EP_ASSERT_H__
#define __EP_ASSERT_H__

#include "epLib.h"

#include <assert.h>
#include "epMemory.h"

/*!
@def EP_ASSERT_EXPR
@brief Macro Function for assert with message
@param[in] _Expression the boolean expression to evaluate the assert
@param[in] formatString the format string for the message
*/
#if defined(_DEBUG)
#if defined(_UNICODE) || defined(UNICODE)
#define EP_ASSERT_EXPR(_Expression,formatString, ...)\
	do{\
	if(!!!(_Expression)){\
	int len=_scwprintf(formatString,__VA_ARGS__)+1;\
	wchar_t * expression=EP_NEW wchar_t[len];\
	swprintf_s(expression,len,formatString,__VA_ARGS__);\
	\
	int len2=_scwprintf(WIDEN("%s\r\n\r\nMessage: %s"),WIDEN(#_Expression),expression )+1;\
	wchar_t * expression2=EP_NEW wchar_t[len2];\
	swprintf_s(expression2,len2,WIDEN("%s\r\n\r\nMessage: %s"),WIDEN(#_Expression),expression);\
	\
	(void)( (_wassert(expression2,__WFILE__, __LINE__), 0) );\
	EP_DELETE[] expression;\
	EP_DELETE[] expression2;\
	}\
	}while(0)
#else // defined(_UNICODE) || defined(UNICODE)
#define EP_ASSERT_EXPR(_Expression,formatString, ...)\
	do{\
	if(!!!(_Expression)){\
	int len=_scprintf(formatString,__VA_ARGS__)+1;\
	char * expression=EP_NEW char[len];\
	sprintf_s(expression,len,formatString,__VA_ARGS__);\
	\
	int len2=_scprintf("%s\r\n\r\nMessage: %s",#_Expression,expression )+1;\
	wchar_t * expression2=EP_NEW wchar_t[len2];\
	swprintf_s(expression2,len2,WIDEN("%s\r\n\r\nMessage: %s"),WIDEN(#_Expression),expression);\
	\
	(void)( (_wassert(expression2,__WFILE__, __LINE__), 0) );\
	EP_DELETE[] expression;\
	EP_DELETE[] expression2;\
	}\
	}while(0)
#endif // defined(_UNICODE) || defined(UNICODE)
#else //defined(_DEBUG)
#define EP_ASSERT_EXPR(_Expression,formatString, ...) ((void)0)
#endif //defined(_DEBUG)


/*!
@def EP_ASSERT
@brief Macro Function for assert
@param[in] expr the boolean expression to evaluate the assert
*/
#define EP_ASSERT(expr) _ASSERT(expr)


#endif //__EP_ASSERT_H__