/*! 
@file epBinarySearch.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 16, 2011
@brief Binary Search Algorithm Interface
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

An Interface for Binary Search Algorithm.

*/
#ifndef __EP_BINARY_SEARCH_H__
#define __EP_BINARY_SEARCH_H__

#include "epLib.h"
#include "epAlgorithm.h"

namespace epl
{
	/*!
	Template Binary Search Function

	Search the given list with Compare Function Pointer and the key, 
	and if exists return the pointer to the item 
	and the index of the item in the list.
	@param[in] _pKey The key to search the list
	@param[in] searchList The list to search.
	@param[in] listSize The size of the list.
	@param[in] CompareFunc The Compare Function pointer.
	@param[out] retIdx The found item's index in the list.
	@return the pointer to the item found. If not found returns NULL.
	*/
	template <typename T,typename T2>
	T2 * BinarySearch (T const &_pKey,T2* searchList,size_t listSize,CompResultType (__cdecl *CompareFunc)(const void * , const void *), size_t &retIdx)
	{
		if (searchList == NULL || listSize<1)
		{
			retIdx=listSize;
			return NULL;
		}
		ssize_t low = 0;
		ssize_t high= ssize_t(listSize)-1;
		ssize_t med = (low+high)/2;

		T2 * tpCur = NULL;
		while ( 1 )
		{

			tpCur = searchList+med;

			CompResultType ret = CompareFunc(&_pKey, tpCur );
			if ( ret == COMP_RESULT_EQUAL)
			{
				retIdx=size_t(med);
				return tpCur;
			}
			else if (ret > COMP_RESULT_EQUAL)
			{
				low=med+1;
			}
			else
			{
				high= med-1;
			}
			med = (high+low)/2;

			if (high<low)
			{
				break;
			}
		}
		retIdx=size_t(low);
		return NULL;
	}

}
#endif //__EP_BINARY_SEARCH_H__