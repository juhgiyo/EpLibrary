/*! 
@file epRandom.h
@author Woong Gyu La a.k.a Chris.  <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2011
@brief Random Number Function Interface
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

An Interface for the Random Number Generator.

*/
#ifndef __EP_RANDOM_H__
#define __EP_RANDOM_H__

#include "epLib.h"
#include "epSingletonHolder.h"

#define RANDOM_INSTANCE epl::SingletonHolder<Random>::Instance()
namespace epl
{
	/*! 
	@class Random epRandom.h
	@brief A class that generate the random number.
	*/
	class EP_LIBRARY Random
	{
	public:
		friend class SingletonHolder<Random>;
		/*!
		Return the random number
		@return the random number
		*/
		int GetRandom();

		/*!
		Return the random number between startNum and endNum inclusive
		(*endNum must be greater than or equal to the startNum, otherwise assertion)
		@param[in] startNum the start number of the random choice
		@param[in] endNum the end number of the random choice
		@return the random number between startNum and endNum inclusive
		*/
		int GetRandom(int startNum, int endNum);
	private:
		/*!
		Default Constructor
		*/
		Random();
		/*!
		Default Destructor
		*/
		~Random();
	};
}
#endif //__EP_RANDOM_H__
