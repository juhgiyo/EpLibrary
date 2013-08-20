/*! 
@file epAssert.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2011
@brief Assert Interface
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