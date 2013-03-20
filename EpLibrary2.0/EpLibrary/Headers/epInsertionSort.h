/*! 
@file epInsertionSort.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 16, 2011
@brief Insertion Sort Algorithm Interface
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
	inline void InsertionSort(T* sortList, ssize_t low, ssize_t high,CompResultType (__cdecl *SortFunc)(const void * , const void *))
	{
		ssize_t i;
		for(i=low+1; i<=high; i++)
		{
			T tmp;
			tmp=sortList[i];
			ssize_t j=i-1;
			while(j>=low && SortFunc(&sortList[j],&tmp)>COMP_RESULT_EQUAL)
			{
				sortList[j+1]=sortList[j];
				j=j-1;
			}
			sortList[j+1]=tmp;
		}
	}
}

#endif //__EP_INSERTION_SORT_H__