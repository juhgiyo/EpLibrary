/*! 
@file epInsertionSort.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 16, 2011
@brief Insertion Sort Algorithm Interface
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

An Interface for Insertion Sort Algorithm Function.

*/
#ifndef __EP_INSERTION_SORT_H__
#define __EP_INSERTION_SORT_H__
#include "epLib.h"
#include "epAlgorithm.h"
#include "epSystem.h"

namespace epl
{
	/*!
	Template Insertion Sort Function

	Sort the given list with Sort Function Pointer.
	@param[in] sortList The list to sort.
	@param[in] low the low index.
	@param[in] high the high index
	@param[in] SortFunc The Compare Function pointer.
	*/
	template<typename T>
	inline void InsertionSort(T* sortList, size_t low, size_t high,CompResultType (__cdecl *SortFunc)(const void * , const void *))
	{
		size_t i;
		for(i=low+1; i<=high; i++)
		{
			T tmp;
			tmp=sortList[i];
			size_t j=i-1;
			while(j!=size_t(-1) && j>=low && SortFunc(&sortList[j],&tmp)>COMP_RESULT_EQUAL)
			{
				sortList[j+1]=sortList[j];
				j=j-1;
			}
			sortList[j+1]=tmp;
		}
	}
}

#endif //__EP_INSERTION_SORT_H__