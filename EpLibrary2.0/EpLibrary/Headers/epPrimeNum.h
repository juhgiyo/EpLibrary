/*! 
@file epPrimeNum.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2011
@brief Prime Number Function Interface
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