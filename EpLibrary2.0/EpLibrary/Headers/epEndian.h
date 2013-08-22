/*! 
@file epEndian.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 16, 2011
@brief Endian Interface
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
