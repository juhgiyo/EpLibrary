/*! 
@file epCStringEx.h
@author Marc Clifton
	edited by Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
			  <http://github.com/juhgiyo/eplibrary>
@date August 08, 2010
@brief CStringEx Interface

@section LICENSE

Please refer to <http://www.codeproject.com/Articles/2396/Simple-CString-Extension> for the license.

@section DESCRIPTION

An Interface for CStringEx Class.
*/
#ifndef __EP_CSTRING_EX_H__
#define __EP_CSTRING_EX_H__
#include "epFoundationLib.h"
#include "epSystem.h"

namespace epl
{
	/*! 
	@class CStringEx epCStringEx.h
	@brief A class for CStringEx.
	*/
	class EP_FOUNDATION CStringEx: public CString
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
