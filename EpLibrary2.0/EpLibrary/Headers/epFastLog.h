/*! 
@file epFastLog.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2011
@brief Fast Log Function Interface
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

An Interface for the Fast Natural Log Approximation.

*/
#ifndef __EP_FAST_LOG_H__
#define __EP_FAST_LOG_H__

#include "epLib.h"

namespace epl{
#define FLOG_ERROR 0xff

	/*! 
	@class FastLog epFastLog.h
	@brief A class that calculates Logarithm.
	*/
	class EP_LIBRARY FastLog
	{
	public:
		/*!
		Fast Log Function by upper bound
		@param[in] input the value for log.
		@return the log of x
		*/
		static unsigned int FLogU(unsigned int input);

		/*!
		Fast Log Function by upper bound
		@param[in] input the value for log.
		@return the log of x
		*/
		static int FLogU(int input);

		/*!
		Fast Log Function by upper bound
		@param[in] input the value for log.
		@return the log of x
		*/
		static unsigned short FLogU(unsigned short input);

		/*!
		Fast Log Function by upper bound
		@param[in] input the value for log.
		@return the log of x
		*/
		static short FLogU(short input);


		/*!
		Fast Log Function by lower bound
		@param[in] input the value for log.
		@return the log of x
		*/
		static unsigned int FLogL(unsigned int input);

		/*!
		Fast Log Function by lower bound
		@param[in] input the value for log.
		@return the log of x
		*/
		static int FLogL(int input);

		/*!
		Fast Log Function by lower bound
		@param[in] input the value for log.
		@return the log of x
		*/
		static unsigned short FLogL(unsigned short input);

		/*!
		Fast Log Function by lower bound
		@param[in] input the value for log.
		@return the log of x
		*/
		static short FLogL(short input);

	};

	

}
#endif //__EP_FAST_LOG_H__
