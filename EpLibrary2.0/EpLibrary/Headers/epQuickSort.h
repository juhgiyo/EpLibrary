/*! 
@file epQuickSort.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 16, 2011
@brief Quick Sort Algorithm Interface
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

An Interface for Quick Sort Algorithm Function.

*/
#ifndef __EP_QUICK_SORT_H__
#define __EP_QUICK_SORT_H__
#include "epLib.h"
#include "epInsertionSort.h"
#include "epSystem.h"
#include <stack>
using namespace std;

namespace epl
{
	/// Enumeration Type for QSort Mode
	typedef enum _qSortMode{
		/// QSort Mode using STL
		QSORT_MODE_STL=0,
		/// QSort Mode using Recursive operation
		QSORT_MODE_RECURSIVE,
		/// QSort Mode using Loop operation
		QSORT_MODE_LOOP
	}QSortMode;


	/*!
	Template Quick Sort Function

	Sort the given list with Sort Function Pointer.
	@param[in] sortList The list to sort.
	@param[in] listSize The size of the list.
	@param[in] SortFunc The Compare Function pointer.
	@param[in] mode The QSort Mode
	@param[in] minSize the minimum size for the insertion sort start
	*/
	template <typename T>
	void QuickSort (T* sortList,const size_t listSize,CompResultType (__cdecl *SortFunc)(const void * , const void *), QSortMode mode=QSORT_MODE_STL, ssize_t minSize=-1)
	{
		if(mode==QSORT_MODE_STL)
		{
			qsort(sortList,listSize,sizeof(T),(int (__cdecl *)(const void *,const void *))SortFunc);
		}
		else
		{
			if(minSize<0 || minSize>ssize_t(listSize))
				minSize=ssize_t(listSize)/2;
			if(mode==QSORT_MODE_RECURSIVE)
				subQuickSortRecursive<T>(sortList,0,listSize-1,SortFunc, minSize);
			else if(mode==QSORT_MODE_LOOP)
				subQuickSortLoop<T>(sortList,0,listSize-1,SortFunc, minSize);

		}
		
		
	}

	/*!
	Sub Function for Quick Sort with Recursive Operation.
	@param[in] sortList The list to sort.
	@param[in] low the low index of the list.
	@param[in] high the high index of the list.
	@param[in] SortFunc The Compare Function pointer.
	@param[in] minSize the minimum size of the list for insertion Sort operation
	*/
	template<typename T>
	inline void subQuickSortRecursive(T* sortList, ssize_t low, ssize_t high,CompResultType (__cdecl *SortFunc)(const void * , const void *), ssize_t minSize)
	{
		if(high>low+1)
		{
			if((high-low)+1<=minSize)
			{
				InsertionSort<T>(sortList,low,high,SortFunc);
			}
			else
			{
				ssize_t index = partitionWrapper<T>(sortList, low, high,SortFunc);
				subQuickSortRecursive<T>(sortList, low, index-1,SortFunc, minSize);
				subQuickSortRecursive<T>(sortList,index+1, high,SortFunc,minSize);
			}
		}
		else if((high==low+1)&& SortFunc(&sortList[low],&sortList[high])>COMP_RESULT_EQUAL)
		{
			SwapFunc<T>(&sortList[low],&sortList[high]);
		}
	}


	/*!
	Wrapper Function of partition function of Quick Sort with Recursive Operation.
	@param[in] sortList The list to sort.
	@param[in] low the low index of the list.
	@param[in] high the high index of the list.
	@param[in] SortFunc The Compare Function pointer.
	@return the median index
	*/
	template<typename T>
	inline ssize_t partitionWrapper(T* sortList, ssize_t low, ssize_t high,CompResultType (__cdecl *SortFunc)(const void * , const void *))
	{
		SwapFunc<T>(&sortList[low], &sortList[medianLocation<T>(sortList,low+1,high,(low+high)/2,SortFunc)]);
		ssize_t med=partition<T>(sortList, low+1, high, sortList[low],SortFunc);
		SwapFunc<T>(&sortList[low],&sortList[med]);
		return med;
	}

	/*!
	Median Locator Function for Quick Sort with Recursive Operation.
	@param[in] sortList The list to sort.
	@param[in] i the low index of the possible median.
	@param[in] j the middle index of the possible median.
	@param[in] k the high index of the possible median.
	@param[in] SortFunc The Compare Function pointer.
	@return the median index
	*/
	template<typename T>
	inline ssize_t medianLocation(T* sortList, ssize_t i, ssize_t j, ssize_t k,CompResultType (__cdecl *SortFunc)(const void * , const void *))
	{
		if (SortFunc(&sortList[i] , &sortList[j])<COMP_RESULT_GREATERTHAN)
			if (SortFunc(&sortList[j] , &sortList[k])<COMP_RESULT_GREATERTHAN)
				return j;
			else if (SortFunc(&sortList[i] , &sortList[k])<COMP_RESULT_GREATERTHAN)
				return k;
			else
				return i;
		else // sortList[j] < sortList[i]
			if (SortFunc(&sortList[i] , &sortList[k])<COMP_RESULT_GREATERTHAN)
				return i;
			else if (SortFunc(&sortList[j] , &sortList[k])<COMP_RESULT_GREATERTHAN)
				return k;
			else
				return j;
	}

	/*!
	Actual partition function of Quick Sort with Recursive Operation.
	@param[in] sortList The list to sort.
	@param[in] low the low index of the list.
	@param[in] high the high index of the list.
	@param[in] pivot the pivot index of the list
	@param[in] SortFunc The Compare Function pointer.
	@return the median index
	*/
	template<typename T>
	inline ssize_t partition(T* sortList, ssize_t low, ssize_t high, T &pivot,CompResultType (__cdecl *SortFunc)(const void * , const void *))
	{
		while(high!=low)
		{
			CompResultType res1=SortFunc(&sortList[low],&pivot);
			CompResultType res2=SortFunc(&sortList[high],&pivot);
			if(res1<COMP_RESULT_GREATERTHAN)
			{
				low++;
				if(res2>COMP_RESULT_LESSTHAN && high>low)
					high--;
			}
			else if(res2>COMP_RESULT_LESSTHAN)
				high--;
			else
			{
				SwapFunc<T>(&sortList[low],&sortList[high]);
				low++;
				if(high>low)
					high--;
			}
		}

		if (SortFunc(&sortList[low], &pivot) <COMP_RESULT_EQUAL)
			return low;
		else
			return low-1;
	}

	/*!
	Sub Function for Quick Sort with Loop Operation.
	@param[in] sortList The list to sort.
	@param[in] iLow the low index of the list.
	@param[in] iHigh the high index of the list.
	@param[in] SortFunc The Compare Function pointer.
	@param[in] minSize the minimum size of the list for insertion Sort operation
	*/
	template<typename T>
	inline void subQuickSortLoop(T* sortList, ssize_t iLow, ssize_t iHigh,CompResultType (__cdecl *SortFunc)(const void *,const void *),ssize_t minSize)
	{
		struct SnapShotStruct
		{
			ssize_t low;
			ssize_t high;
		};

		stack<SnapShotStruct> snapshotStack;
		SnapShotStruct currentSnaptshot;
		currentSnaptshot.low=iLow;
		currentSnaptshot.high=iHigh;
		snapshotStack.push(currentSnaptshot);
		while(!snapshotStack.empty())
		{
			currentSnaptshot=snapshotStack.top();
			snapshotStack.pop();
			if(currentSnaptshot.high>currentSnaptshot.low+1)
			{
				if((currentSnaptshot.high-currentSnaptshot.low)+1<=minSize)
				{
					InsertionSort<T>(sortList,currentSnaptshot.low,currentSnaptshot.high,SortFunc);
				}
				else
				{
					ssize_t i=currentSnaptshot.low+1;
					ssize_t j=(currentSnaptshot.low+currentSnaptshot.high)/2;
					ssize_t k=currentSnaptshot.high;
					ssize_t medLoc;
					if (SortFunc(&sortList[i] , &sortList[j])<COMP_RESULT_GREATERTHAN)
						if (SortFunc(&sortList[j] , &sortList[k])<COMP_RESULT_GREATERTHAN)
							medLoc=j;
						else if (SortFunc(&sortList[i] , &sortList[k])<COMP_RESULT_GREATERTHAN)
							medLoc=k;
						else
							medLoc=i;
					else // sortList[j] < sortList[i]
						if (SortFunc(&sortList[i] , &sortList[k])<COMP_RESULT_GREATERTHAN)
							medLoc=i;
						else if (SortFunc(&sortList[j] , &sortList[k])<COMP_RESULT_GREATERTHAN)
							medLoc=k;
						else
							medLoc=j;

						T tmp;
						tmp=sortList[currentSnaptshot.low];
						sortList[currentSnaptshot.low]=sortList[medLoc];
						sortList[medLoc]=tmp;

						T &pivot=sortList[currentSnaptshot.low];
						ssize_t relHigh=currentSnaptshot.high;
						ssize_t relLow=currentSnaptshot.low+1;
						CompResultType res1=SortFunc(&sortList[relLow],&pivot);
						CompResultType res2=SortFunc(&sortList[relHigh],&pivot);
						while(relHigh!=relLow)
						{
							if(res1<COMP_RESULT_GREATERTHAN)
							{
								relLow++;
								res1=SortFunc(&sortList[relLow],&pivot);
								if(res2>COMP_RESULT_LESSTHAN && relHigh>relLow)
								{
									relHigh--;
									res2=SortFunc(&sortList[relHigh],&pivot);
								}
							}
							else if(res2>COMP_RESULT_LESSTHAN)
							{
								relHigh--;
								res2=SortFunc(&sortList[relHigh],&pivot);
							}
							else
							{
								tmp=sortList[relLow];
								sortList[relLow]=sortList[relHigh];
								sortList[relHigh]=tmp;
								relLow++;
								res1=SortFunc(&sortList[relLow],&pivot);
								if(relHigh>relLow)
								{
									relHigh--;
									res2=SortFunc(&sortList[relHigh],&pivot);
								}
							}
						}
						ssize_t med;
						if (SortFunc(&sortList[relLow], &pivot) < COMP_RESULT_EQUAL)
							med= relLow;
						else
							med= relLow-1;
						tmp=sortList[currentSnaptshot.low];
						sortList[currentSnaptshot.low]=sortList[med];
						sortList[med]=tmp;
						ssize_t index = med;

						SnapShotStruct leftHalf,rightHalf;
						leftHalf.low=currentSnaptshot.low;
						leftHalf.high=index-1;

						rightHalf.low=index+1;
						rightHalf.high=currentSnaptshot.high;
						snapshotStack.push(leftHalf);
						snapshotStack.push(rightHalf);
				}
			}
			else if((currentSnaptshot.high==currentSnaptshot.low+1)&& SortFunc(&sortList[currentSnaptshot.low],&sortList[currentSnaptshot.high])>COMP_RESULT_EQUAL)
			{
				T tmp;
				tmp=sortList[currentSnaptshot.low];
				sortList[currentSnaptshot.low]=sortList[currentSnaptshot.high];
				sortList[currentSnaptshot.high]=tmp;
			}
		}
		
	}

}
#endif //__EP_QUICK_SORT_H__
