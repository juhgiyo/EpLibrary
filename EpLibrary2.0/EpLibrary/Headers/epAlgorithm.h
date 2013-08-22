/*! 
@file epAlgorithm.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2011
@brief Cumulative Algorithm Interface
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
		void * vA=const_cast<void *>(a);
		void * vB=const_cast<void *>(b);
		T* _a=reinterpret_cast<T*>(vA);
		T* _b=reinterpret_cast<T*>(vB);
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
		char *_a=reinterpret_cast<char*>(a);
		char *_b=reinterpret_cast<char*>(b);
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
		Pair(const T& a, const U& b)
		{
			first=a;
			second=b;
		}
		~Pair(){}
		T first;
		U second;
	};

	/// Enumerator for File Encoding
	typedef enum _fileEncodingType{
		///UTF-8
		FILE_ENCODING_TYPE_UTF8=0,
		///UTF-16 Little Endian (Default)
		FILE_ENCODING_TYPE_UTF16LE,
		///ANSI
		FILE_ENCODING_TYPE_ANSI,
		/// Enum Count
		NUM_OF_FILE_ENCODING_TYPE

	}FileEncodingType;

	/// Lock Policy Enumeration
	typedef enum _lockPolicy{
		/// Not a multi thread environment
		LOCK_POLICY_NONE=0,
		/// a multi thread environment
		LOCK_POLICY_CRITICALSECTION,
		/// a multi process environment
		LOCK_POLICY_MUTEX,
	}LockPolicy;

	/// Console Priority
	typedef enum _consolePriority{
		CONSOLE_PRIORITY_NORMAL = 0x00000020,
		CONSOLE_PRIORITY_IDLE   = 0x00000040,
		CONSOLE_PRIORITY_HIGH   = 0x00000080,
		CONSOLE_PRIORITY_REALTIME = 0x00000100,
	}ConsolePriority;

}

#endif //__EP_ALGORITHM_H__