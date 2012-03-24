/*! 
@file epAlgorithm.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date April 17, 2011
@brief Cumulative Algorithm Interface
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

An Interface for Cumulative Algorithm Function.

*/
#ifndef __EP_ALGORITHM_H__
#define __EP_ALGORITHM_H__

namespace epl
{
	/////////////////////////////////////
	//                                 //
	//       Insert Related            //
	//                                 //
	/////////////////////////////////////
	
	/// Enumerator for Insert
	typedef enum _insertStatus{
		/// Insertion is Duplicated
		INSERT_DUPLICATE =-1,
		/// Insertion is succeeded
		INSERT_SUCCESS =1,
		/// Insertion Error
		INSERT_ERROR =0
	} InsertStatus;

	/////////////////////////////////////
	//                                 //
	//       Comparison Related        //
	//                                 //
	/////////////////////////////////////


	/// Enumerator for Comparison
	typedef enum _compResult{
		/// Comparison resulted Less Than
		COMP_RESULT_LESSTHAN =-1,
		/// Comparison resulted Greater Than
		COMP_RESULT_GREATERTHAN =1,
		/// Comparison resulted Equal
		COMP_RESULT_EQUAL =0
	}CompResultType;

	/*!
	@class CompClass epAlgorithm.h
	@brief A template Compare Class for the Comparison of the objects.	
	*/
	template <typename T>
	class CompClass{
	public:
		/*!
		Default Constructor
		*/
		CompClass(){}

		/*!
		Default Destructor
		*/
		virtual ~CompClass(){}

		/*!
		Compares object a with object b

		@param[in] a the pointer to the object.
		@param[in] b the pointer to another object.
		@return the Result of Comparison
		*/
		static CompResultType CompFunc(const void *a, const void *b);
	};
	template<typename T>
	CompResultType CompClass<T>::CompFunc(const void * a, const void * b)
	{
		T* _a=(T*)a;
		T* _b=(T*)b;
		if(*_a==*_b)
			return COMP_RESULT_EQUAL;
		else if(*_a>*_b)
			return COMP_RESULT_GREATERTHAN;
		else
			return COMP_RESULT_LESSTHAN;
	}

	/*!
	Template Default Swap Function
	@param[in] a The pointer to the swap object.
	@param[in] b The pointer to the other swap object.
	*/
	template<typename T>
	inline void SwapFunc(T *a, T*b)
	{
		if(a==b)
			return;
		T tmp;
		tmp=*a;
		*a=*b;
		*b=tmp;
	}

	/*!
	Template Default Copy Function
	@param[in] a The pointer to the destination object.
	@param[in] b The pointer to the source object.
	*/
	template<typename T>
	inline void CopyFunc(T *a, T*b)
	{
		char *_a=(char*)a;
		char *_b=(char*)b;
		unsigned int width=sizeof(T);
		if ( _a != _b )
			while ( width-- ) {
				*_a++ = *_b++;
			}
	}

	/*! 
	@class Pair epAlgorithm.h
	@brief A template class for holding pair of items.
	*/
	template <typename T, typename U>
	class Pair
	{
	public:
		Pair(){}
		~Pair(){}
		T first;
		U second;
	};

	/// Enumerator for File Encoding
	typedef enum _fileEncodingType{
		///Unicode
		FILE_ENCODING_TYPE_UNICODE=0,
		///UTF-8
		FILE_ENCODING_TYPE_UTF8,
		///UTF-16
		FILE_ENCODING_TYPE_UTF16,
		/// Enum Count
		NUM_OF_FILE_ENCODING_TYPE

	}FileEncodingType;
}

#endif //__EP_ALGORITHM_H__