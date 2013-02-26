/*! 
@file epDateTimeHelper.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief DateTime Function Class Interface
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

An Interface for the DateTime Operation.

*/
#ifndef __EP_DATE_TIME_HELPER_H__
#define __EP_DATE_TIME_HELPER_H__
#include "epLib.h"
#include "epSystem.h"

namespace epl 
{
	/*! 
	@class DateTimeHelper epDateTimeHelper.h
	@brief This is a class for DateTime Helping Class

	Implements the DateTime Helping Functions.
	*/
	class EP_LIBRARY DateTimeHelper
	{
	public:
		/*!
		Return the current date time
		@return the current date time.
		*/
		static SYSTEMTIME GetCurrentDateTime();

		/*!
		Translate the given FILETIME to SYSTEMTIME
		@param[in] fileTime the FILETIME to translate
		@return the SYSTEMTIME converted.
		*/
		static SYSTEMTIME FileTimeToSystemTime( const FILETIME &fileTime);
		
		/*!
		Translate the given SYSTEMTIME to FILETIME
		@param[in] systemTime the SYSTEMTIME to translate
		@return the FILETIME converted.
		*/
		static FILETIME SystemTimeToFileTime( const SYSTEMTIME &systemTime);


		/*!
		Find the difference between two time and return the difference in (1/(10000000)) second or 100-nano second
		@remark result = time1 - time2
		@param[in] time1 the first time to find the difference
		@param[in] time2 the second time to find the difference
		@return the difference between two given time in millisecond
		*/
		static __int64 Diff(const SYSTEMTIME &time1, const SYSTEMTIME &time2);

		/*!
		Find the absolute difference between two time and return the difference in (1/(10000000)) second or 100-nano second
		@remark result = absolute value of (time1-time2)
		@param[in] time1 the first time to find the difference
		@param[in] time2 the second time to find the difference
		@return the absolute difference between two given time in millisecond
		*/
		static __int64 AbsDiff(const SYSTEMTIME &time1, const SYSTEMTIME &time2);

		/*!
		Find the difference between two time and return the difference in millisecond 
		@remark result = time1 - time2
		@param[in] time1 the first time to find the difference
		@param[in] time2 the second time to find the difference
		@return the difference between two given time in millisecond
		
		*/
		static __int64 DiffInMilliSec(const SYSTEMTIME &time1, const SYSTEMTIME &time2);

		/*!
		Find the absolute difference between two time and return the difference in second 
		@remark result = absolute value of (time1-time2)
		@param[in] time1 the first time to find the difference
		@param[in] time2 the second time to find the difference
		@return the absolute difference between two given time in millisecond
		*/
		static __int64 AbsDiffInMilliSec(const SYSTEMTIME &time1, const SYSTEMTIME &time2);

		/*!
		Find the difference between two time and return the difference in second 
		@remark result = time1 - time2
		@param[in] time1 the first time to find the difference
		@param[in] time2 the second time to find the difference
		@return the difference between two given time in millisecond
		
		*/
		static __int64 DiffInSec(const SYSTEMTIME &time1, const SYSTEMTIME &time2);

		/*!
		Find the absolute difference between two time and return the difference in millisecond 
		@remark result = absolute value of (time1-time2)
		@param[in] time1 the first time to find the difference
		@param[in] time2 the second time to find the difference
		@return the absolute difference between two given time in millisecond
		*/
		static __int64 AbsDiffInSec(const SYSTEMTIME &time1, const SYSTEMTIME &time2);

		/*!
		Translate the given SYSTEMTIME date part to Date String 
		@param[in] date the date to translate
		@return the date string converted.
		*/
		static EpTString DateToString(const SYSTEMTIME &date);

		/*!
		Translate the given SYSTEMTIME time part to Time String 
		@param[in] time the time to translate
		@return the time string converted.
		*/
		static EpTString TimeToString(const SYSTEMTIME &time);

		/*!
		Translate the given SYSTEMTIME's Date & Time to DateTime String 
		@param[in] dateTime the DateTime to translate
		@return the DateTime string converted.
		*/
		static EpTString DateTimeToString(const SYSTEMTIME &dateTime);


		/*!
		Find the difference between two time and return the difference in (1/(10000000)) second or 100-nano second
		@remark result = time1 - time2
		@param[in] time1 the first time to find the difference
		@param[in] time2 the second time to find the difference
		@return the difference between two given time in millisecond
		*/
		static __int64 Diff(const FILETIME &time1, const FILETIME &time2);

		/*!
		Find the absolute difference between two time and return the difference in (1/(10000000)) second or 100-nano second
		@remark result = absolute value of (time1-time2)
		@param[in] time1 the first time to find the difference
		@param[in] time2 the second time to find the difference
		@return the absolute difference between two given time in millisecond
		*/
		static __int64 AbsDiff(const FILETIME &time1, const FILETIME &time2);

		/*!
		Find the difference between two time and return the difference in millisecond 
		@remark result = time1 - time2
		@param[in] time1 the first time to find the difference
		@param[in] time2 the second time to find the difference
		@return the difference between two given time in millisecond
		
		*/
		static __int64 DiffInMilliSec(const FILETIME &time1, const FILETIME &time2);

		/*!
		Find the absolute difference between two time and return the difference in second 
		@remark result = absolute value of (time1-time2)
		@param[in] time1 the first time to find the difference
		@param[in] time2 the second time to find the difference
		@return the absolute difference between two given time in millisecond
		*/
		static __int64 AbsDiffInMilliSec(const FILETIME &time1, const FILETIME &time2);

		/*!
		Find the difference between two time and return the difference in second 
		@remark result = time1 - time2
		@param[in] time1 the first time to find the difference
		@param[in] time2 the second time to find the difference
		@return the difference between two given time in millisecond
		
		*/
		static __int64 DiffInSec(const FILETIME &time1, const FILETIME &time2);

		/*!
		Find the absolute difference between two time and return the difference in millisecond 
		@remark result = absolute value of (time1-time2)
		@param[in] time1 the first time to find the difference
		@param[in] time2 the second time to find the difference
		@return the absolute difference between two given time in millisecond
		*/
		static __int64 AbsDiffInSec(const FILETIME &time1, const FILETIME &time2);

		/*!
		Translate the given SYSTEMTIME date part to Date String 
		@param[in] fTime the file time's date to translate
		@return the date string converted.
		*/
		static EpTString DateToString(const FILETIME &fTime);

		/*!
		Translate the given SYSTEMTIME time part to Time String 
		@param[in] fTime the file time to translate
		@return the time string converted.
		*/
		static EpTString TimeToString(const FILETIME &fTime);

		/*!
		Translate the given SYSTEMTIME's Date & Time to DateTime String 
		@param[in] fTime the file time's DateTime to translate
		@return the DateTime string converted.
		*/
		static EpTString DateTimeToString(const FILETIME &fTime);

#if (_MSC_VER >=MSVC90) && (WINVER>=WINDOWS_VISTA) // Only for VS2008 and above and Windows Vista and above
		/*!
		Get Thread's actual CPU Cycle Count
		@param[in] threadHandle the thread handle
		@return the actual CPU Cycle Count ran by given thread.
		*/
		static unsigned __int64 GetThreadCPUCycleCount(HANDLE threadHandle);

		/*!
		Get Process's actual CPU Cycle Count
		@param[in] processHandle the process handle
		@return the actual CPU Cycle Count ran by given process.
		*/
		static unsigned __int64 GetProcessCPUCycleCount(HANDLE processHandle);
#endif //(_MSC_VER >=MSVC90) && (WINVER>=WINDOWS_VISTA)

		/*!
		Get thread's creation time
		@param[in] threadHandle the thread handle
		@return the creation time.
		*/
		static FILETIME GetThreadCreationTime(HANDLE threadHandle);
		
		/*!
		Get thread's exit time
		@param[in] threadHandle the thread handle
		@return the exit time.
		*/
		static FILETIME GetThreadExitTime(HANDLE threadHandle);

		/*!
		Get thread's kernal time used
		@param[in] threadHandle the thread handle
		@return the kernal time used by given thread.
		*/
		static FILETIME GetThreadKernalTime(HANDLE threadHandle);

		/*!
		Get thread's user time used
		@param[in] threadHandle the thread handle
		@return the user time used by given thread.
		*/
		static FILETIME GetThreadUserTime(HANDLE threadHandle);

		/*!
		Get thread's total run time
		@param[in] threadHandle the thread handle
		@return the total run time spent by given thread.
		*/
		static FILETIME GetThreadTotalRunningTime(HANDLE threadHandle);


		/*!
		Get process's creation time
		@param[in] processHandle the process handle
		@return the creation time.
		*/
		static FILETIME GetProcessCreationTime(HANDLE processHandle);
		
		/*!
		Get process's exit time
		@param[in] processHandle the process handle
		@return the exit time.
		*/
		static FILETIME GetProcessExitTime(HANDLE processHandle);

		/*!
		Get process's kernal time used
		@param[in] processHandle the process handle
		@return the kernal time used by given process.
		*/
		static FILETIME GetProcessKernalTime(HANDLE processHandle);

		/*!
		Get process's user time used
		@param[in] processHandle the process handle
		@return the user time used by given process.
		*/
		static FILETIME GetProcessUserTime(HANDLE processHandle);

		/*!
		Get process's total run time
		@param[in] processHandle the process handle
		@return the total run time spent by given process.
		*/
		static FILETIME GetProcessTotalRunningTime(HANDLE processHandle);


		/*!
		Translate FILETIME in (1/(10000000)) second or 100-nano second
		@param[in] fileTime the file time to translate
		@return the translated time in (1/(10000000)) second or 100-nano second.
		*/
		static __int64 In100NanoSec(const FILETIME &fileTime);

		/*!
		Translate FILETIME in milli-second
		@param[in] fileTime the file time to translate
		@return the translated time in milli-second.
		*/
		static __int64 InMilliSec(const FILETIME &fileTime);

		/*!
		Translate FILETIME in second
		@param[in] fileTime the file time to translate
		@return the translated time in second.
		*/
		static __int64 InSec(const FILETIME &fileTime);

	};
}


#endif //__EP_DATE_TIME_HELPER_H__