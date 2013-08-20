/*! 
@file epBinarySearch.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 16, 2011
@brief Binary Search Algorithm Interface
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

An Interface for Binary Search Algorithm.

*/
#ifndef __EP_BINARY_SEARCH_H__
#define __EP_BINARY_SEARCH_H__

#include "epLib.h"
#include "epAlgorithm.h"
#include "epSystem.h"
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
		size_t low = 0;
		size_t high= listSize-1;
		size_t med = (low+high)/2;

		T2 * tpCur = NULL;
		while ( 1 )
		{

			tpCur = searchList+med;

			CompResultType ret = CompareFunc(&_pKey, tpCur );
			if ( ret == COMP_RESULT_EQUAL)
			{
				retIdx=med;
				return tpCur;
			}
			else if (ret == COMP_RESULT_GREATERTHAN)
			{
				low=med+1;
			}
			else
			{
				if(med<1)
					break;
				high= med-1;
			}
			med = (high+low)/2;

			if (high<low)
			{
				break;
			}
		}
		retIdx=low;
		return NULL;
	}

}
#endif //__EP_BINARY_SEARCH_H__