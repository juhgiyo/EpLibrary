/*! 
DateTimeHelper for the EpLibrary

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
*/
#include "epDateTimeHelper.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl; 

SYSTEMTIME DateTimeHelper::GetCurrentDateTime()
{
	SYSTEMTIME currentDate;
	GetLocalTime(&currentDate);
	return currentDate;
}

__int64 DateTimeHelper::Diff(const SYSTEMTIME &time1, const SYSTEMTIME &time2)
{
	union timeunion 
	{ 
		FILETIME fileTime; 
		ULARGE_INTEGER ul; 
	};

	timeunion ft1;
	timeunion ft2;

	ft1.fileTime=SystemTimeToFileTime(time1);
	ft2.fileTime=SystemTimeToFileTime(time2);

	return __int64(ft1.ul.QuadPart) - __int64(ft2.ul.QuadPart); 
}

__int64 DateTimeHelper::AbsDiff(const SYSTEMTIME &time1, const SYSTEMTIME &time2)
{
	union timeunion 
	{ 
		FILETIME fileTime; 
		ULARGE_INTEGER ul; 
	};

	timeunion ft1;
	timeunion ft2;

	ft1.fileTime=SystemTimeToFileTime(time1);
	ft2.fileTime=SystemTimeToFileTime(time2);

	if(ft1.ul.QuadPart > ft2.ul.QuadPart)
		return (ft1.ul.QuadPart - ft2.ul.QuadPart); 
	return (ft2.ul.QuadPart-ft1.ul.QuadPart); 
}

__int64 DateTimeHelper::DiffInMilliSec(const SYSTEMTIME &time1, const SYSTEMTIME &time2)
{
	return DateTimeHelper::Diff(time1,time2)/__int64(10000.0); 
}

__int64 DateTimeHelper::AbsDiffInMilliSec(const SYSTEMTIME &time1, const SYSTEMTIME &time2)
{
	return DateTimeHelper::AbsDiff(time1,time2)/__int64(10000.0); 
}

__int64 DateTimeHelper::DiffInSec(const SYSTEMTIME &time1, const SYSTEMTIME &time2)
{
	return DateTimeHelper::Diff(time1,time2)/__int64(10000000.0); 
}

__int64 DateTimeHelper::AbsDiffInSec(const SYSTEMTIME &time1, const SYSTEMTIME &time2)
{
	return DateTimeHelper::AbsDiff(time1,time2)/__int64(10000000.0); 
}



EpTString DateTimeHelper::DateToString(const SYSTEMTIME &date)
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

EpTString DateTimeHelper::TimeToString(const SYSTEMTIME &time)
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

EpTString DateTimeHelper::DateTimeToString(const SYSTEMTIME &dateTime)
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


SYSTEMTIME DateTimeHelper::FileTimeToSystemTime( const FILETIME &fileTime)
{
	SYSTEMTIME systemTime;
	::FileTimeToSystemTime(&fileTime,&systemTime);
	return systemTime;
}

FILETIME DateTimeHelper::SystemTimeToFileTime( const SYSTEMTIME &systemTime)
{
	FILETIME fileTime;
	::SystemTimeToFileTime(&systemTime,&fileTime);
	return fileTime;
}



__int64 DateTimeHelper::Diff(const FILETIME &time1, const FILETIME &time2)
{
	union timeunion 
	{ 
		FILETIME fileTime; 
		ULARGE_INTEGER ul; 
	};

	timeunion ft1;
	timeunion ft2;

	ft1.fileTime=time1;
	ft2.fileTime=time2;

	return __int64(ft1.ul.QuadPart) - __int64(ft2.ul.QuadPart); 
}

__int64 DateTimeHelper::AbsDiff(const FILETIME &time1, const FILETIME &time2)
{
	union timeunion 
	{ 
		FILETIME fileTime; 
		ULARGE_INTEGER ul; 
	};

	timeunion ft1;
	timeunion ft2;
	ft1.fileTime=time1;
	ft2.fileTime=time2;

	if(ft1.ul.QuadPart > ft2.ul.QuadPart)
		return (ft1.ul.QuadPart - ft2.ul.QuadPart); 
	return (ft2.ul.QuadPart-ft1.ul.QuadPart); 
}

__int64 DateTimeHelper::DiffInMilliSec(const FILETIME &time1, const FILETIME &time2)
{
	return DateTimeHelper::Diff(time1,time2)/__int64(10000.0); 
}

__int64 DateTimeHelper::AbsDiffInMilliSec(const FILETIME &time1, const FILETIME &time2)
{
	return DateTimeHelper::AbsDiff(time1,time2)/__int64(10000.0); 
}

__int64 DateTimeHelper::DiffInSec(const FILETIME &time1, const FILETIME &time2)
{
	return DateTimeHelper::Diff(time1,time2)/__int64(10000000.0); 
}

__int64 DateTimeHelper::AbsDiffInSec(const FILETIME &time1, const FILETIME &time2)
{
	return DateTimeHelper::AbsDiff(time1,time2)/__int64(10000000.0); 
}



EpTString DateTimeHelper::DateToString(const FILETIME &fTime)
{
	SYSTEMTIME systemTime=FileTimeToSystemTime(fTime);
	return DateToString(systemTime);
}

EpTString DateTimeHelper::TimeToString(const FILETIME &fTime)
{
	SYSTEMTIME systemTime=FileTimeToSystemTime(fTime);
	return TimeToString(systemTime);
}

EpTString DateTimeHelper::DateTimeToString(const FILETIME &fTime)
{
	SYSTEMTIME systemTime=FileTimeToSystemTime(fTime);
	return DateTimeToString(systemTime);
}

#if (_MSC_VER >=MSVC90) && (WINVER>=WINDOWS_VISTA) // Only for VS2008 and above and Windows Vista and above
unsigned __int64 DateTimeHelper::GetThreadCPUCycleCount(HANDLE threadHandle)
{
	unsigned __int64 cycleCount=0.0;
	::QueryThreadCycleTime(threadHandle,&cycleCount);
	return cycleCount;
}


unsigned __int64 DateTimeHelper::GetProcessCPUCycleCount(HANDLE processHandle)
{
	unsigned __int64 cycleCount=0.0;
	::QueryProcessCycleTime(processHandle,&cycleCount);
	return cycleCount;
}
#endif //(_MSC_VER >=MSVC90) && (WINVER>=WINDOWS_VISTA)

FILETIME DateTimeHelper::GetThreadCreationTime(HANDLE threadHandle)
{
	FILETIME creationTime;
	FILETIME exitTime;
	FILETIME kernalTime;
	FILETIME userTime;
	::GetThreadTimes(threadHandle,&creationTime,&exitTime,&kernalTime,&userTime);
	return creationTime;
}

FILETIME DateTimeHelper::GetThreadExitTime(HANDLE threadHandle)
{
	FILETIME creationTime;
	FILETIME exitTime;
	FILETIME kernalTime;
	FILETIME userTime;
	::GetThreadTimes(threadHandle,&creationTime,&exitTime,&kernalTime,&userTime);
	return exitTime;
}
FILETIME DateTimeHelper::GetThreadKernalTime(HANDLE threadHandle)
{
	FILETIME creationTime;
	FILETIME exitTime;
	FILETIME kernalTime;
	FILETIME userTime;
	::GetThreadTimes(threadHandle,&creationTime,&exitTime,&kernalTime,&userTime);
	return kernalTime;
}
FILETIME DateTimeHelper::GetThreadUserTime(HANDLE threadHandle)
{
	FILETIME creationTime;
	FILETIME exitTime;
	FILETIME kernalTime;
	FILETIME userTime;
	::GetThreadTimes(threadHandle,&creationTime,&exitTime,&kernalTime,&userTime);
	return userTime;
}
FILETIME DateTimeHelper::GetThreadTotalRunningTime(HANDLE threadHandle)
{
	FILETIME creationTime;
	FILETIME exitTime;
	FILETIME kernalTime;
	FILETIME userTime;
	::GetThreadTimes(threadHandle,&creationTime,&exitTime,&kernalTime,&userTime);

	union timeunion 
	{ 
		FILETIME fileTime; 
		ULARGE_INTEGER ul; 
	};

	timeunion ft1;
	timeunion ft2;
	timeunion ft3;
	ft1.fileTime=kernalTime;
	ft2.fileTime=userTime;
	ft3.ul.QuadPart=(ft2.ul.QuadPart+ft1.ul.QuadPart);
	return ft3.fileTime; 
}

FILETIME DateTimeHelper::GetProcessCreationTime(HANDLE processHandle)
{
	FILETIME creationTime;
	FILETIME exitTime;
	FILETIME kernalTime;
	FILETIME userTime;
	::GetProcessTimes(processHandle,&creationTime,&exitTime,&kernalTime,&userTime);
	return creationTime;
}
FILETIME DateTimeHelper::GetProcessExitTime(HANDLE processHandle)
{
	FILETIME creationTime;
	FILETIME exitTime;
	FILETIME kernalTime;
	FILETIME userTime;
	::GetProcessTimes(processHandle,&creationTime,&exitTime,&kernalTime,&userTime);
	return exitTime;
}

FILETIME DateTimeHelper::GetProcessKernalTime(HANDLE processHandle)
{
	FILETIME creationTime;
	FILETIME exitTime;
	FILETIME kernalTime;
	FILETIME userTime;
	::GetProcessTimes(processHandle,&creationTime,&exitTime,&kernalTime,&userTime);
	return kernalTime;
}
FILETIME DateTimeHelper::GetProcessUserTime(HANDLE processHandle)
{
	FILETIME creationTime;
	FILETIME exitTime;
	FILETIME kernalTime;
	FILETIME userTime;
	::GetProcessTimes(processHandle,&creationTime,&exitTime,&kernalTime,&userTime);
	return userTime;
}

FILETIME DateTimeHelper::GetProcessTotalRunningTime(HANDLE processHandle)
{
	FILETIME creationTime;
	FILETIME exitTime;
	FILETIME kernalTime;
	FILETIME userTime;
	::GetProcessTimes(processHandle,&creationTime,&exitTime,&kernalTime,&userTime);

	union timeunion 
	{ 
		FILETIME fileTime; 
		ULARGE_INTEGER ul; 
	};

	timeunion ft1;
	timeunion ft2;
	timeunion ft3;
	ft1.fileTime=kernalTime;
	ft2.fileTime=userTime;
	ft3.ul.QuadPart=(ft2.ul.QuadPart+ft1.ul.QuadPart);
	return ft3.fileTime; 
}

__int64 DateTimeHelper::In100NanoSec(const FILETIME &fileTime)
{
	union timeunion 
	{ 
		FILETIME fileTime; 
		ULARGE_INTEGER ul; 
	};
	timeunion ft1;
	ft1.fileTime=fileTime;
	return ft1.ul.QuadPart;
}
__int64 DateTimeHelper::InMilliSec(const FILETIME &fileTime)
{
	union timeunion 
	{ 
		FILETIME fileTime; 
		ULARGE_INTEGER ul; 
	};
	timeunion ft1;
	ft1.fileTime=fileTime;
	return ft1.ul.QuadPart/10000.0;
}
__int64 DateTimeHelper::InSec(const FILETIME &fileTime)
{
	union timeunion 
	{ 
		FILETIME fileTime; 
		ULARGE_INTEGER ul; 
	};
	timeunion ft1;
	ft1.fileTime=fileTime;
	return ft1.ul.QuadPart/10000000.0;
}


__int64 DateTimeHelper::In100NanoSec(const SYSTEMTIME &systemTime)
{
	return In100NanoSec(SystemTimeToFileTime(systemTime));
}
__int64 DateTimeHelper::InMilliSec(const SYSTEMTIME &systemTime)
{
	return InMilliSec(SystemTimeToFileTime(systemTime));
}
__int64 DateTimeHelper::InSec(const SYSTEMTIME &systemTime)
{
	return InSec(SystemTimeToFileTime(systemTime));
}