/*! 
@file epFastSqrt.h
@author Maxim Shemanarev (http://www.antigrain.com)
    edited by Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		      <http://github.com/juhgiyo/eplibrary>
@date April 17, 2011
@brief Fast Square Root Function Interface
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

----------------------------------------------------------------------------

Part of this software is

Anti-Grain Geometry - Version 2.4
Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)

Permission to copy, use, modify, sell and distribute this software 
is granted provided this copyright notice appears in all copies. 
This software is provided "as is" without express or implied
warranty, and with no claim as to its suitability for any purpose.

----------------------------------------------------------------------
Contact: mcseem@antigrain.com
         mcseemagg@yahoo.com
         http://www.antigrain.com
----------------------------------------------------------------------
Bessel function (besj) was adapted for use in AGG library by Andy Wilk 
Contact: castor.vulgaris@gmail.com

----------------------------------------------------------------------

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