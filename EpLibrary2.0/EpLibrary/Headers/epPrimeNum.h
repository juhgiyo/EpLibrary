/*! 
@file epPrimeNum.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2011
@brief Prime Number Function Interface
@version 2.0

@section LICENSE

The MIT License (MIT)

Copyright (c) 2012-2013 Woong Gyu La <juhgiyo@gmail.com>

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

An Interface for the Fast Prime Number Calculation.

*/
#ifndef __EP_PRIME_NUM_H__
#define __EP_PRIME_NUM_H__

#include "epLib.h"

namespace epl
{
	/*! 
	@class PrimeNum epPrimeNum.h
	@brief A class that calculates Prime Number.
	*/
	class EP_LIBRARY PrimeNum
	{
	public:
		/*!
		Check if the given number is a prime number
		@param[in] x the value to check if it is a prime number.
		@return true if x is a prime number, false otherwise
		*/
		bool IsPrime(unsigned int x);

		/*!
		Find the first prime number larger than given number.
		@param[in] x the value to find a first prime number larger than x.
		@return first prime number larger than x
		*/
		unsigned int NextPrime(unsigned int x);
	};
}
#endif //__EP_PRIME_NUM_H__