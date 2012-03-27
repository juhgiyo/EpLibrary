/*! 
@file epDynamicArray.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date August 08, 2010
@brief A Dynamic Array Data Structure Interface
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

An Interface for Dynamic Array Data Structure.

*/
#ifndef __EP_DYNAMIC_ARRAY_H__
#define __EP_DYNAMIC_ARRAY_H__

#include "epLib.h"


namespace epl
{
	/*! 
	@class DynamicArray epDynamicArray.h
	@brief A template class for Dynamic Array.
	*/
	template <typename FDATA>
	class DynamicArray
	{
		/*!
		Default Constructor

		Initializes the Dynamic Array
		*/
		DynamicArray();

		/*!
		Default Constructor

		Initializes the Dynamic Array with given array
		*/
		DynamicArray(const DynamicArray<FDATA> &dArr);

		/*!
		Default Destructor

		Destroys the Dynamic Array
		*/
		virtual ~DynamicArray();

		/*!
		Delete the given array.
		(Frees the memory)
		@param[in] dArr The dynamic array structure to delete.
		*/
		void Delete();

		/*!
		Clear all the elements in the given array.
		(Does Not Free the memory)
		@param[in] dArr The dynamic array structure to clear.
		*/
		void Clear();

		/*!
		Check if the given array is empty.
		@return true if the array is empty, false otherwise.
		*/
		bool IsEmpty() const;

		/*!
		Return the number of elements using in the given array.
		@return the number of elements  using in the given array.
		*/
		unsigned int Size() const;

		/*!
		Resize the given array to given size.
		@param[in] newSize The new size to resize.
		*/
		bool Resize(unsigned int newSize);

		/*!
		Return the element at the given index of the given array.
		@param[in] idx The index to return the element.
		@return the element at the given index.
		*/
		FDATA &At(unsigned int idx);

		/*!
		Append the element given to the dynamic array given.
		@param[in] data The data to append at the end.
		@return the result dynamic array
		*/
		DynamicArray<FDATA> &Append(const FDATA &data);

		/*!
		Append the given dynamic array to the this dynamic array.
		@param[in] dArr The dynamic array structure to append.
		@return the result dynamic array
		*/
		DynamicArray<FDATA> &Append(const DynamicArray<FDATA> &dArr);

		/*!
		Initialize this array to given array
		@param[in] b the dynamic array structure to initialize this array
		@return the result dynamic array
		*/
		DynamicArray<FDATA> &operator=(const DynamicArray<FDATA>& b);

		/*!
		Return the element at the given index of the given array.
		@param[in] idx The index to return the element.
		@return the element at the given index.
		*/
		FDATA& operator[](unsigned int idx);

		/*!
		Append the given dynamic array to the this dynamic array.
		@param[in] b The dynamic array structure to append.
		@return the result dynamic array
		*/
		DynamicArray<FDATA>& operator+=(const DynamicArray<FDATA>& b);

		/*!
		Return new array by appending the given dynamic array to the this dynamic array.
		@param[in] b The dynamic array structure to append.
		@return the new result dynamic array
		*/
		DynamicArray<FDATA> operator+(const DynamicArray<FDATA>& b) const;

		/*!
		Append the given element to the this dynamic array.
		@param[in] b The element to append.
		@return the result dynamic array
		*/
		DynamicArray<FDATA>& operator+=(const FDATA& b);

		/*!
		Return new array by appending the given element to the this dynamic array.
		@param[in] b The element to append.
		@return the new result dynamic array
		*/
		DynamicArray<FDATA> operator+(const FDATA& b) const;
	private:
		/// the actual dynamic array
		FDATA *m_head;
		/// the number of item in array
		unsigned int m_numOfElements;
		/// the actual array size
		unsigned int m_actualSize;

	};

	DynamicArray::DynamicArray()
	{
		m_head=NULL;
		m_numOfElements=0;
		m_actualSize=0;
	}
	DynamicArray::DynamicArray(const DynamicArray<FDATA> &dArr)
	{
		m_numOfElements=dArr.m_numOfElements;
		m_actualSize=dArr.m_actualSize;
		m_head=EP_Malloc(sizeof(FDATA)*m_actualSize);
		EP_WASSERT(m_head,_T("Allocation Failed"));
		memcpy(m_head,dArr.m_head,sizeof(FDATA)*m_actualSize);
	}

	DynamicArray::~DynamicArray()
	{
		if(m_head)
			free(m_head);
	}

	void DynamicArray::Delete()
	{
		if(m_head)
			free(m_head);
		m_head=NULL;
		m_actualSize=0;
		m_numOfElements=0;
	}

	void DynamicArray::Clear()
	{
		m_numOfElements=0;
		memset(m_head,0,sizeof(FDATA)*m_actualSize);
	}

	bool DynamicArray::IsEmpty() const
	{
		if(m_numOfElements)
			return true;
		return false;
	}

	unsigned int DynamicArray::Size() const
	{
		return m_numOfElements;
	}

	bool DynamicArray::Resize(unsigned int newSize)
	{
		if(m_actualSize>=newSize)
			return false;
		if(m_head)
		{
			m_head=EP_Realloc(m_head,newSize*sizeof(FDATA));
			m_actualSize=newSize;
		}
		else
		{
			m_head=EP_Malloc(newSize*sizeof(FDATA));
			m_actualSize=newSize;
		}
		EP_WASSERT(m_head,_T("Allocation Failed"));
		return true;
	}

	FDATA &DynamicArray::At(unsigned int idx)
	{
		if(m_numOfElements+1<idx)
		{
			Resize(idx+1);
			m_numOfElements=idx+1;
		}
		return *(m_head+idx)
	}


	DynamicArray<FDATA> &DynamicArray::Append(const FDATA &data)
	{
		if(m_actualSize<m_numOfElements+1)
		{
			Resize(m_numOfElements+1);
		}
		*(m_head+m_numOfElements)=data;
		m_numOfElements++;
	}

	DynamicArray<FDATA> &DynamicArray::Append(const DynamicArray<FDATA> &dArr)
	{
		if(m_actualSize < m_numOfElements+dArr.m_numOfElements)
		{
			Resize(m_numOfElements+dArr.m_numOfElements);
		}
		if(m_head && dArr.m_head && dArr.m_actualSize)
			memcpy(m_head+m_numOfElements,dArr.m_head,dArr.m_numOfElements*sizeof(FDATA));
		m_numOfElements+=dArr.m_numOfElements;
		return *this;
	}

	DynamicArray<FDATA> &DynamicArray::operator=(const DynamicArray<FDATA>& b)
	{
		if(this != &b)
		{
			Delete();
			m_actualSize=b.m_actualSize;
			m_numOfElements=b.m_numOfElements;
			Resize(m_actualSize);
			if(m_head&&b.m_head && b.m_actualSize)
				memcpy(m_head,b.m_head,b.m_actualSize*sizeof(FDATA));

		}
		return *this;
	}

	FDATA& DynamicArray::operator[](unsigned int idx)
	{
		return At(idx);
	}

	DynamicArray<FDATA>& DynamicArray::operator+=(const DynamicArray<FDATA>& b)
	{
		return Append(b);
	}

	DynamicArray<FDATA> DynamicArray::operator+(const DynamicArray<FDATA>& b) const
	{
		DynamicArray<FDATA> retArr;
		retArr.Append(*this);
		retArr.Append(b);
		return retArr;
	}


	DynamicArray<FDATA>& DynamicArray::operator+=(const FDATA& b)
	{
		return Append(b);
	}

	DynamicArray<FDATA> DynamicArray::operator+(const FDATA& b) const
	{
		DynamicArray<FDATA> retArr;
		retArr.Append(*this);
		retArr.Append(b);
		return retArr;				
	}
#endif //__EP_DYNAMIC_ARRAY_H__
