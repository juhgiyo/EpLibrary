/*! 
@file epFastLog.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2011
@brief Fast Log Function Interface
@version 2.0

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
