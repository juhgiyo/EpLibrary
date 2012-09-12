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
		Find the difference between two time and return the difference in microsecond 
		@remark result = time1 - time2
		@param[in] time1 the first time to find the difference
		@param[in] time2 the second time to find the difference
		@return the difference between two given time in microsecond
		
		*/
		static __int64 Diff(const SYSTEMTIME time1, const SYSTEMTIME time2);

		/*!
		Find the absolute difference between two time and return the difference in microsecond 
		@remark result = absolute value of (time1-time2)
		@param[in] time1 the first time to find the difference
		@param[in] time2 the second time to find the difference
		@return the absolute difference between two given time in microsecond
		*/
		static __int64 AbsDiff(const SYSTEMTIME time1, const SYSTEMTIME time2);

		/*!
		Translate the given SYSTEMTIME date part to Date String 
		@param[in] date the date to translate
		@return the date string converted.
		*/
		static EpTString DateToString(const SYSTEMTIME date);

		/*!
		Translate the given SYSTEMTIME time part to Time String 
		@param[in] time the time to translate
		@return the time string converted.
		*/
		static EpTString TimeToString(const SYSTEMTIME time);

		/*!
		Translate the given SYSTEMTIME's Date & Time to DateTime String 
		@param[in] dateTime the DateTime to translate
		@return the DateTime string converted.
		*/
		static EpTString DateTimeToString(const SYSTEMTIME dateTime);
	};
}


#endif //__EP_DATE_TIME_HELPER_H__