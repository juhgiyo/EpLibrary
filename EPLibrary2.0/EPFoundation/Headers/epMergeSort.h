/*! 
@file epMergeSort.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date April 16, 2011
@brief A Merge Sort Algorithm Interface
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

An Interface for Merge Sort Algorithm Function.

*/
#ifndef __EP_MERGE_SORT_H__
#define __EP_MERGE_SORT_H__
#include "epLib.h"

namespace epl
{
	/*!
	Template Insertion Sort Function

	Sort the given list with Sort Function Pointer.
	@param[in] sortList The list to sort.
	@param[in] listSize The size of the list.
	@param[in] SortFunc The Compare Function pointer.
	*/
	template<typename T>
	inline void MergeSort(T *sortList, int listSize,CompResultType (__cdecl *SortFunc)(const void * , const void *))
	{ 
		if(sortList==NULL || listSize<=1)
			return;
		T* mergeSpace=(T*)EP_Malloc(sizeof(T)*listSize);
		T* sortedList=subMergeSort<T>(sortList,listSize,mergeSpace,SortFunc);
		EP_Free(mergeSpace);
		return;
	}

	/*!
	Actual Merge Sort Operation Function.
	@param[in] sortList The list to sort.
	@param[in] listSize The size of the list.
	@param[in] workSpace the list for sorting operation
	@param[in] SortFunc The Compare Function pointer.
	*/
	template<typename T>
	inline T* subMergeSort(T *sortList, int listSize, T* workSpace,CompResultType (__cdecl *SortFunc)(const void * , const void *))
	{
		if(listSize==1)
			return sortList;
		int leftSize= listSize/2;
		int rightSize= listSize-leftSize;
		T* leftSortedList= subMergeSort<T>(sortList,leftSize,workSpace,SortFunc);
		T* rightSortedList=subMergeSort<T>(sortList+leftSize, rightSize, workSpace,SortFunc);
		int trav;
		for(trav=0;trav<listSize;trav++)
		{
			if(leftSize==0)
			{
				workSpace[trav]=rightSortedList[0];
				rightSortedList++;
				rightSize--;
			}
			else if(rightSize==0)
			{
				workSpace[trav]=leftSortedList[0];
				leftSortedList++;
				leftSize--;			
			}
			else
			{
				if(SortFunc(&leftSortedList[0],&rightSortedList[0])<COMP_RESULT_EQUAL)
				{
					workSpace[trav]=leftSortedList[0];
					leftSortedList++;
					leftSize--;
				}
				else
				{
					workSpace[trav]=rightSortedList[0];
					rightSortedList++;
					rightSize--;
				}
			}
		}
		for(trav=0;trav<listSize;trav++)
		{
			sortList[trav]=workSpace[trav];
		}
		return sortList;


	}
}
#endif //__EP_MERGE_SORT_H__