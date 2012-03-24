/*! 
CStringEx for the EpLibrary
@author Marc Clifton
	edited by Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
Please refer to <http://www.codeproject.com/Articles/2396/Simple-CString-Extension> for the license.
*/
#include "epCStringEx.h"
#include <stdio.h>
#include <string.h>

using namespace epl;

CStringEx::CStringEx(int i, const TCHAR* format, DWORD options)
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


CStringEx::CStringEx(double d, const TCHAR* format, DWORD options)
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
		q=q.SubStr(1, _tcslen(q)-1);
	}
	CStringEx dp=s2.Right(_T('.'));								// remember everything to the right of the decimal point
	CStringEx q2;											// working string
	while (_tcslen(q) > 3)									// if more than three digits...
	{
		CStringEx s3=CStringEx(_T(","))+q.SubStr(_tcslen(q)-3, 3);		// insert a comma before the last three digits (100's)
		q2=s3+q2;											// append this to our working string
		q=q.SubStr(0, _tcslen(q)-3);							// get everything except the last three digits
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

