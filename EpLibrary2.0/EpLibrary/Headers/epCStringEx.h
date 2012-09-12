/*! 
@file epCStringEx.h
@author Marc Clifton - June 12, 2002
	edited by Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com> - August 08, 2010
			  <http://github.com/juhgiyo/eplibrary>
@date June 12, 2002
@brief CStringEx Interface

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
