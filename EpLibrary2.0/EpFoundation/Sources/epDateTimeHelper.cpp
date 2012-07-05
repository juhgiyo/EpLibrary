/*! 
DateTimeHelper for the EpLibrary
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
*/
#include "epDateTimeHelper.h"

using namespace epl; 

SYSTEMTIME DateTimeHelper::GetCurrentDateTime()
{
	SYSTEMTIME currentDate;
	GetLocalTime(&currentDate);
	return currentDate;
}

__int64 DateTimeHelper::Diff(const SYSTEMTIME time1, const SYSTEMTIME time2)
{
	union timeunion 
	{ 
		FILETIME fileTime; 
		ULARGE_INTEGER ul; 
	};

	timeunion ft1;
	timeunion ft2;

	SystemTimeToFileTime(&time1, &ft1.fileTime);
	SystemTimeToFileTime(&time2, &ft2.fileTime);

	return (ft1.ul.QuadPart - ft2.ul.QuadPart)/10; 
}

__int64 DateTimeHelper::AbsDiff(const SYSTEMTIME time1, const SYSTEMTIME time2)
{
	union timeunion 
	{ 
		FILETIME fileTime; 
		ULARGE_INTEGER ul; 
	};

	timeunion ft1;
	timeunion ft2;

	SystemTimeToFileTime(&time1, &ft1.fileTime);
	SystemTimeToFileTime(&time2, &ft2.fileTime);

	return _abs64((ft1.ul.QuadPart - ft2.ul.QuadPart)/10); 
}


EpTString DateTimeHelper::DateToString(const SYSTEMTIME date)
{
	int nDateStringLength = GetDateFormat(LOCALE_USER_DEFAULT, 0, &date, NULL, NULL, 0);
	if(nDateStringLength == 0)
	{
		// error condition
		return _T("");
	}

	TCHAR* pszDate = EP_NEW TCHAR[nDateStringLength];
	if(GetDateFormat(LOCALE_USER_DEFAULT, 0, &date, NULL, pszDate, nDateStringLength) == 0)
	{
		// error condition
		EP_DELETE[] pszDate;
		return _T("");
	}

	// Append date to the output stream and free resources
	EpTString retString=pszDate;
	EP_DELETE[] pszDate;
	return retString;
}

EpTString DateTimeHelper::TimeToString(const SYSTEMTIME time)
{
	int nTimeStringLength = GetTimeFormat(LOCALE_USER_DEFAULT, 0, &time, NULL, NULL, 0);
	if(nTimeStringLength == 0)
	{
		// error condition
		return _T("");
	}

	TCHAR* pszTime = EP_NEW TCHAR[nTimeStringLength];
	if(GetTimeFormat(LOCALE_USER_DEFAULT, 0, &time, NULL, pszTime, nTimeStringLength) == 0)
	{
		// error condition
		EP_DELETE[] pszTime;
		return _T("");
	}

	// Append time to the output stream and free resources
	EpTString retString=pszTime;
	EP_DELETE[] pszTime;
	return retString;
}

EpTString DateTimeHelper::DateTimeToString(const SYSTEMTIME dateTime)
{
	int nDateStringLength = GetDateFormat(LOCALE_USER_DEFAULT, 0, &dateTime, NULL, NULL, 0);
	if(nDateStringLength == 0)
	{
		// error condition
		return _T("");
	}

	TCHAR* pszDate = EP_NEW TCHAR[nDateStringLength];
	if(GetDateFormat(LOCALE_USER_DEFAULT, 0, &dateTime, NULL, pszDate, nDateStringLength) == 0)
	{
		// error condition
		EP_DELETE[] pszDate;
		return _T("");
	}

	// Append date to the output stream and free resources
	EpTString retString=pszDate;
	EP_DELETE[] pszDate;

	int nTimeStringLength = GetTimeFormat(LOCALE_USER_DEFAULT, 0, &dateTime, NULL, NULL, 0);
	if(nTimeStringLength == 0)
	{
		// error condition
		return _T("");
	}

	TCHAR* pszTime = EP_NEW TCHAR[nTimeStringLength];
	if(GetTimeFormat(LOCALE_USER_DEFAULT, 0, &dateTime, NULL, pszTime, nTimeStringLength) == 0)
	{
		// error condition
		EP_DELETE[] pszTime;
		return _T("");
	}

	// Append time to the output stream and free resources
	retString.append(_T(" "));
	retString.append(pszTime);
	EP_DELETE[] pszTime;

	return retString;
}