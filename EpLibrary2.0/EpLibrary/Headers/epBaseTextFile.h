/*! 
@file epBaseTextFile.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief Base Text File Interface
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

An Interface for the Base Text File Class.

*/
#ifndef __EP_BASE_TEXT_FILE_H__
#define __EP_BASE_TEXT_FILE_H__
#include "epLib.h"
#include "epSystem.h"
#include "epMemory.h"
#include <list>
#include "epCriticalSectionEx.h"
#include "epMutex.h"
#include "epNoLock.h"

using namespace std;

namespace epl{
	/*!
	@class BaseTextFile epBaseTextFile.h
	@brief A class for Base File.
	*/
	class EP_LIBRARY BaseTextFile{
	public:

		/*!
		Default Constructor

		Initializes the Base File 
		@param[in] encodingType the encoding type for this file
		@param[in] lockPolicyType The lock policy
		*/
		BaseTextFile(FileEncodingType encodingType=FILE_ENCODING_TYPE_UTF16LE, LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the Base File 
		@param[in] b the second object
		*/
		BaseTextFile(const BaseTextFile& b);

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		BaseTextFile & operator=(const BaseTextFile&b);
		
		/*!
		Default Destructor

		Destroy the Base File 
		*/
		virtual ~BaseTextFile();

		/*!
		Return the current encoding type.
		@return the current encoding type
		*/
		FileEncodingType GetEncodingType();

		/*!
		Set the encoding type as given type
		@param[in] encodingType the encoding type for this file
		*/
		virtual void SetEncodingType(FileEncodingType encodingType);

		/*!
		Save the list of the properties from the given file
		@param[in] filename the name of the file to save the list of properties
		@return true if successfully saved, otherwise false
		*/
		bool SaveToFile(const TCHAR *filename);

		/*!
		Append the list of the properties from the given file
		@param[in] filename the name of the file to append the list of properties
		@return true if successfully saved, otherwise false
		*/
		bool AppendToFile(const TCHAR *filename);
		
		/*!
		Load the list of the properties from the given file
		@param[in] filename the name of the file to load the list of properties
		@return true if successfully loaded, otherwise false
		*/
		bool LoadFromFile(const TCHAR *filename);

		/*!
		Get a single line from the given buffer
		@param[in] buf the buffer that holds all lines
		@param[in] startIdx the start index of the buf to find a line
		@param[out] retLine the first line that found in the given buffer
		@param[out] retEndIdx the end index of the buf after reading a line
		@param[out] retRest the rest of the buffer without the found line
		@return true if successfully parsed the line, otherwise false
		*/
		static bool GetLine(const EpTString &buf,size_t startIdx, EpTString &retLine, size_t *retEndIdx=NULL, EpTString *retRest=NULL);

	protected:

		/*!
		Write the given string to the file
		@param[in] toFileString the string to write to the file
		*/
		void writeToFile(const TCHAR *toFileString);

		/*!
		Loop Function that writes to the file.
		@remark Sub classes should implement this function
		*/
		virtual void writeLoop()=0;

		/*!
		Actual load Function that loads values from the file.
		@remark Sub classes should implement this function
		@param[in] lines the all data from the file
		*/
		virtual void loadFromFile(const EpTString &lines)=0;

		/// Encoding type of the file
		FileEncodingType m_encodingType;
		/// File Pointer
		EpFile *m_file;
		/// the lock
		BaseLock * m_baseTextLock;
		/// Lock Policy
		LockPolicy m_lockPolicy;
	};

}



#endif //__EP_BASE_TEXT_FILE_H__