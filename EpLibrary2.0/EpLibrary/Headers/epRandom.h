/*! 
@file epRandom.h
@author Woong Gyu La a.k.a Chris.  <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2011
@brief Random Number Function Interface
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
