/*! 
@file epCStringEx.h
@author Marc Clifton - June 12, 2002
	edited by Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com> - August 08, 2010
			  <http://github.com/juhgiyo/eplibrary>
@date June 12, 2002
@brief CStringEx Interface

@section LICENSE

The MIT License (MIT)

Copyright (c) 2008-2013 Woong Gyu La <juhgiyo@gmail.com>

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

------------------------------------------------------------------------------

Part of this software is

Copyright (c) 2002 Marc Clifton

Please refer to <http://www.codeproject.com/Articles/2396/Simple-CString-Extension> for the license.

@section DESCRIPTION

An Interface for CStringEx Class.
*/
#ifndef __EP_CSTRING_EX_H__
#define __EP_CSTRING_EX_H__
#include "epLib.h"
#include "epSystem.h"

namespace epl
{
	/*! 
	@class CStringEx epCStringEx.h
	@brief A class for CStringEx.
	*/
	class EP_LIBRARY CStringEx: public CString
	{
	public:

		enum Options
		{
			NO_OPTIONS=0,
			COMMA_DELIMIT=1,
		};

	public:

		CStringEx(void) {};
		CStringEx(const TCHAR* s) : CString(s) {}
		CStringEx(const TCHAR c) {TCHAR s[2]={_T('\0'), _T('\0')}; s[0]=c; CString::operator=(s);}
		CStringEx(int i, const TCHAR* format=_T("%d"), unsigned long options=NO_OPTIONS);
		CStringEx(double d, const TCHAR* format=_T("%02lf"), unsigned long options=NO_OPTIONS);
		virtual ~CStringEx() {};

		CStringEx Left(TCHAR c, int n=1) const;						// everything to the left of the nth occurance of <c>
		CStringEx Right(TCHAR c, int n=1) const;						// everything to the right of the nth occurance of <c>
		CStringEx Rightmost(TCHAR c) const;							// everything to the right of the last occurance of <c>
		CStringEx LeftOfRightmost(TCHAR c) const;					// everything to the left of the last occurance of <c>
		CStringEx SubStr(int begin, int len) const;					// substring from s[begin] to s[begin+len]

		void Trim(void) {TrimLeft(); TrimRight();};					// trims both left and right sides

		CStringEx& operator=(const CString& s) {CString::operator=(s); return *this;}

		CStringEx CommaDelimitNumber(const TCHAR* s);

	};

}

#endif //__EP_CSTRING_EX_H__
