/*! 
@file epTextFile.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief Text File Interface
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
		TextFile & operator=(const TextFile&b)
		{
			if(this!=&b)
			{
				BaseTextFile::operator =(b);
				LockObj lock(m_baseTextLock);
				m_text=b.m_text;
				
			}
			return *this;
		}

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