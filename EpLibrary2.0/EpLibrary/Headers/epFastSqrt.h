/*! 
@file epFastSqrt.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2011
@brief Fast Square Root Function Interface
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

An Interface for the Fast Square Root Approximation.

*/
#include "epLib.h"

#ifndef __EP_FAST_SQRT_H__
#define __EP_FAST_SQRT_H__
namespace epl
{
	/*! 
	@class FastSqrt epFastSqrt.h
	@brief A class that calculates SquareRoot.
	*/
	class EP_LIBRARY FastSqrt
	{
	public:
		/*!
		Fast Square Root Function 
		@param[in] val the value for sqrt.
		@return the square root of val
		*/
		static unsigned int Sqrt(const unsigned int val);

	};
}
#endif __EP_FAST_SQRT_H__