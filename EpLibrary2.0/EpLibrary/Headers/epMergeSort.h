/*! 
@file epMergeSort.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 16, 2011
@brief Merge Sort Algorithm Interface
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

An Interface for Merge Sort Algorithm Function.

*/
#ifndef __EP_MERGE_SORT_H__
#define __EP_MERGE_SORT_H__
#include "epLib.h"
#include <stack>
using namespace std;

namespace epl
{
	/// Enumeration Type for Merge Sort Mode
	typedef enum _mSortMode{
		/// MSort Mode using Recursive operation
		MSORT_MODE_RECURSIVE,
		/// MSort Mode using Loop operation
		MSORT_MODE_LOOP
	}MSortMode;

	/*!
	Template Insertion Sort Function

	Sort the given list with Sort Function Pointer.
	@param[in] sortList The list to sort.
	@param[in] listSize The size of the list.
	@param[in] SortFunc The Compare Function pointer.
	@param[in] mode the flag for recursive or loop mode
	*/
	template<typename T>
	inline void MergeSort(T *sortList, size_t listSize,CompResultType (__cdecl *SortFunc)(const void * , const void *), MSortMode mode=MSORT_MODE_LOOP)
	{ 
		if(sortList==NULL || listSize<=1)
			return;
		T* mergeSpace=reinterpret_cast<T*>(EP_Malloc(sizeof(T)*listSize));

		T* sortedList;
		if(mode==MSORT_MODE_RECURSIVE)
			sortedList=subMergeSortRecursive<T>(sortList,listSize,mergeSpace,SortFunc);
		else if(mode==MSORT_MODE_LOOP)
			sortedList=subMergeSortLoop<T>(sortList,listSize,mergeSpace,SortFunc);

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
	inline T* subMergeSortRecursive(T *sortList, size_t listSize, T* workSpace,CompResultType (__cdecl *SortFunc)(const void * , const void *))
	{
		if(listSize!=1)
		{
			size_t leftSize= listSize/2;
			size_t rightSize= listSize-leftSize;
			T* leftSortedList= subMergeSortRecursive<T>(sortList,leftSize,workSpace,SortFunc);

			T* rightSortedList=subMergeSortRecursive<T>(sortList+leftSize, rightSize, workSpace,SortFunc);

			size_t trav;
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
		}
		return sortList;


	}

	/*!
	Actual Merge Sort Operation Function.
	@param[in] sortList The list to sort.
	@param[in] listSize The size of the list.
	@param[in] workSpace the list for sorting operation
	@param[in] SortFunc The Compare Function pointer.
	*/
	template<typename T>
	inline T* subMergeSortLoop(T *sortList, size_t listSize, T* workSpace,CompResultType (__cdecl *SortFunc)(const void * , const void *))
	{
		struct SnapShotStruct
		{
			T*sortList;
			size_t listSize;
			size_t leftSize;
			size_t rightSize;
			T* leftSortedList;
			T* rightSortedList;
			int stage;
		};

		stack<SnapShotStruct> snapshotStack;
		SnapShotStruct currentSnaptshot;
		currentSnaptshot.sortList=sortList;
		currentSnaptshot.listSize=listSize;
		currentSnaptshot.leftSize=0;
		currentSnaptshot.rightSize=0;
		currentSnaptshot.leftSortedList=NULL;
		currentSnaptshot.rightSortedList=NULL;
		currentSnaptshot.stage=0;
		snapshotStack.push(currentSnaptshot);

		T* retList=sortList;
		while(!snapshotStack.empty())
		{
			
			currentSnaptshot=snapshotStack.top();
			snapshotStack.pop();
			switch(currentSnaptshot.stage)
			{
			case 0:
				if(currentSnaptshot.listSize!=1)
				{
					currentSnaptshot.leftSize= currentSnaptshot.listSize/2;
					currentSnaptshot.rightSize= currentSnaptshot.listSize-currentSnaptshot.leftSize;
					currentSnaptshot.stage=1;
					snapshotStack.push(currentSnaptshot);
					SnapShotStruct newSnapshot;
					newSnapshot.sortList=currentSnaptshot.sortList;
					newSnapshot.listSize=currentSnaptshot.leftSize;
					newSnapshot.leftSize=0;
					newSnapshot.rightSize=0;
					newSnapshot.leftSortedList=NULL;
					newSnapshot.rightSortedList=NULL;
					newSnapshot.stage=0;
					snapshotStack.push(newSnapshot);
					continue;

				}
				retList=currentSnaptshot.sortList;
				break;
			case 1:
				currentSnaptshot.stage=2;
				currentSnaptshot.leftSortedList=retList;
				snapshotStack.push(currentSnaptshot);
				SnapShotStruct newSnapshot;
				newSnapshot.sortList=currentSnaptshot.sortList+currentSnaptshot.leftSize;
				newSnapshot.listSize=currentSnaptshot.rightSize;
				newSnapshot.leftSize=0;
				newSnapshot.rightSize=0;
				newSnapshot.leftSortedList=NULL;
				newSnapshot.rightSortedList=NULL;
				newSnapshot.stage=0;
				snapshotStack.push(newSnapshot);
				break;
			case 2:
				currentSnaptshot.rightSortedList=retList;
				size_t trav;
				for(trav=0;trav<currentSnaptshot.listSize;trav++)
				{
					if(currentSnaptshot.leftSize==0)
					{
						workSpace[trav]=currentSnaptshot.rightSortedList[0];
						currentSnaptshot.rightSortedList++;
						currentSnaptshot.rightSize--;
					}
					else if(currentSnaptshot.rightSize==0)
					{
						workSpace[trav]=currentSnaptshot.leftSortedList[0];
						currentSnaptshot.leftSortedList++;
						currentSnaptshot.leftSize--;			
					}
					else
					{
						if(SortFunc(&currentSnaptshot.leftSortedList[0],&currentSnaptshot.rightSortedList[0])<COMP_RESULT_EQUAL)
						{
							workSpace[trav]=currentSnaptshot.leftSortedList[0];
							currentSnaptshot.leftSortedList++;
							currentSnaptshot.leftSize--;
						}
						else
						{
							workSpace[trav]=currentSnaptshot.rightSortedList[0];
							currentSnaptshot.rightSortedList++;
							currentSnaptshot.rightSize--;
						}
					}
				}
				for(trav=0;trav<currentSnaptshot.listSize;trav++)
				{
					currentSnaptshot.sortList[trav]=workSpace[trav];
				}
				retList=currentSnaptshot.sortList;
				break;
			}
			
		}
		return sortList;
	}
}
#endif //__EP_MERGE_SORT_H__