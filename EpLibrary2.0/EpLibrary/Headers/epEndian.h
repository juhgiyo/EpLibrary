/*! 
@file epEndian.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 16, 2011
@brief Endian Interface
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

An Interface for Endian Class.

*/
#ifndef __EP_ENDIAN_H__
#define __EP_ENDIAN_H__

#include "epLib.h"


namespace epl
{
	/*! 
	@class Endian epEndian.h
	@brief A class for Endian.
	*/
	class EP_LIBRARY Endian
	{
		/*!
		Swap Short Endianness and return the swapped value.
		@param[in] value the value to swap.
		@return the swapped value
		*/
		static short Swap(const short value);

		/*!
		Swap Unsigned Short Endianness and return the swapped value.
		@param[in] value the value to swap.
		@return the swapped value
		*/
		static unsigned short Swap(const unsigned short value);

		/*!
		Swap Int Endianness and return the swapped value.
		@param[in] value the value to swap.
		@return the swapped value
		*/
		static int Swap(const int value);

		/*!
		Swap Unsigned Int Endianness and return the swapped value.
		@param[in] value the value to swap.
		@return the swapped value
		*/
		static unsigned int Swap(const unsigned int value);

		/*!
		Swap Float Endianness and return the swapped value.
		@param[in] value the value to swap.
		@return the swapped value
		*/
		static float Swap(const float value);

		/*!
		Swap Double Endianness and return the swapped value.
		@param[in] value the value to swap.
		@return the swapped value
		*/
		static double Swap(const double value);

	};
}
#endif //__EP_ENDIAN_H__
