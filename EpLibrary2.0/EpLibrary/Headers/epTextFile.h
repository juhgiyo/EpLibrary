/*! 
@file epTextFile.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief Text File Interface
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

An Interface for the Text File Class.

*/
#ifndef __EP_TEXT_FILE_H__
#define __EP_TEXT_FILE_H__
#include "epLib.h"
#include "epSystem.h"
#include "epMemory.h"
#include "epBaseTextFile.h"

namespace epl{

	/*!
	@class TextFile epTextFile.h
	@brief A class for Text File.
	*/
	class EP_LIBRARY TextFile:public BaseTextFile{
	public:

		/*!
		Default Constructor

		Initializes the Properties File 
		@param[in] encodingType the encoding type for this file
		@param[in] lockPolicyType The lock policy
		*/
		TextFile(FileEncodingType encodingType=FILE_ENCODING_TYPE_UTF16LE, LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the PropertiesFile 
		@param[in] b the second object
		*/
		TextFile(const TextFile& b);

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		TextFile & operator=(const TextFile&b);
		

		/*!
		Default Destructor

		Destroy the Properties File 
		*/
		virtual ~TextFile();

		/*!
		Set the text with the given text
		@param[in] val the text value
		*/
		void SetText(const TCHAR * val);

		/*!
		Get the value of the textfile
		@return text value holding
		*/
		EpTString GetText() const;

	
		/*!
		Clear the text
		*/
		void Clear();

	private:

		/*!
		Loop Function that writes to the file.
		@remark Sub classes should implement this function
		*/
		virtual void writeLoop();

		/*!
		Actual Load Function that loads values from the file.
		@remark Sub classes should implement this function
		@param[in] lines the all data from the file
		*/
		virtual void loadFromFile(const EpTString &lines);

		/// The text
		EpTString m_text;
	};
}



#endif //__EP_PROPERTIES_FILE_H__