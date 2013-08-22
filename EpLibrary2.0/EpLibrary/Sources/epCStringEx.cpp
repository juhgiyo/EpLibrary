/*! 
CStringEx for the EpLibrary

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
*/
#include "epCStringEx.h"
#include <stdio.h>
#include <string>

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;

CStringEx::CStringEx(int i, const TCHAR* format, unsigned long options)
{
	TCHAR s[64];							// Eeek, a hard coded limit on integer sizes!
	_stprintf(s, _T("%d"), i);
	if (options & COMMA_DELIMIT)
	{
		CString::operator=(CommaDelimitNumber(s));
	}
	else
	{
		CString::operator=(s);
	}
}


CStringEx::CStringEx(double d, const TCHAR* format, unsigned long options)
{
	TCHAR s[64];							// Eeek, a hard coded limit on double sizes!
	_stprintf(s, format, d);
	if (options & COMMA_DELIMIT)
	{
		CString::operator=(CommaDelimitNumber(s));
	}
	else
	{
		CString::operator=(s);
	}
}


// extracts string to the left of c, or entire string if c does not exist in str
CStringEx CStringEx::Left(TCHAR c, int q) const
{
	int n=0;
	while (q)
	{
		n=Find(c, n);
		if (n != -1)
		{
			++n;
		}
		else
		{
			return *this;
		}
		--q;
	}

	return CStringEx(CString::Left(n-1));
}


// extracts string to the right of c, or returns empty string if c does not exist in str
CStringEx CStringEx::Right(TCHAR c, int q) const
{
	int n=0;
	while (q)
	{
		n=Find(c, n);
		if (n == -1)
		{
			return _T("");
		}
		--q;
		++n;
	}
	return CStringEx(CString::Right(GetLength()-n));
}


// return the string to the right of the rightmost instance of c, or an empty string if c does not exist in str
CStringEx CStringEx::Rightmost(TCHAR c) const
{
	int n=ReverseFind(c);
	if (n == -1)
	{
		return _T("");
	}
	return CStringEx(CString::Right(GetLength()-n-1));
}


// return the string to the left of the rightmost instance of c, or an empty string if c does not exist in str
CStringEx CStringEx::LeftOfRightmost(TCHAR c) const
{
	int n=ReverseFind(c);
	if (n == -1)
	{
		return CStringEx(_T(""));
	}
	return CStringEx(CString::Left(n));
}


CStringEx CStringEx::SubStr(int begin, int len) const
{
	return CStringEx(CString::Mid(begin, len));
}


CStringEx CStringEx::CommaDelimitNumber(const TCHAR* s)
{
	CStringEx s2=s;										// convert to CStringEx
	CStringEx q=s2.Left(_T('.'));								// get everything to the left of the first decimal point
	TCHAR posNegChar=q[0];									// get the first char
	bool posNeg=!isdigit(posNegChar);							// if not digit, then assume + or -
	if (posNeg)											// if so, strip off
	{
		q=q.SubStr(1, static_cast<int>(_tcslen(q))-1);
	}
	CStringEx dp=s2.Right(_T('.'));								// remember everything to the right of the decimal point
	CStringEx q2;											// working string
	while (_tcslen(q) > 3)									// if more than three digits...
	{
		CStringEx s3=CStringEx(_T(","))+q.SubStr(static_cast<int>(_tcslen(q))-3, 3);		// insert a comma before the last three digits (100's)
		q2=s3+q2;											// append this to our working string
		q=q.SubStr(0, static_cast<int>(_tcslen(q))-3);							// get everything except the last three digits
	}
	q2=q+q2;												// prepend remainder to the working string
	if (dp != _T(""))											// if we have decimal point...
	{
		q2=q2+_T(".")+dp;										// append it and the digits
	}
	if (posNeg)											// if we stripped off a +/- ...
	{
		q2=CStringEx(posNegChar)+q2;							// add it back in
	}
	return CStringEx(q2);											// this is our final comma delimited string
}

