/*! 
@file epBaseTextFile.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief Base Text File Interface
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
		static bool GetLine(const EpTString &buf,int startIdx, EpTString &retLine, int *retEndIdx=NULL, EpTString *retRest=NULL);

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