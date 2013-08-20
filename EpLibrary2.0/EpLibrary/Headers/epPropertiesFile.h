/*! 
@file epPropertiesFile.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief Properties File Interface
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

An Interface for the Properties File Class.

*/
#ifndef __EP_PROPERTIES_FILE_H__
#define __EP_PROPERTIES_FILE_H__
#include "epLib.h"
#include "epSystem.h"
#include "epMemory.h"
#include "epBaseTextFile.h"
#include <list>

using namespace std;

namespace epl{

	/*!
	@class PropertiesFile epPropertiesFile.h
	@brief A class for Peroperties File.
	*/
	class EP_LIBRARY PropertiesFile:public BaseTextFile{
	public:

		/*!
		Default Constructor

		Initializes the Properties File 
		@param[in] encodingType the encoding type for this file
		@param[in] lockPolicyType The lock policy
		*/
		PropertiesFile(FileEncodingType encodingType=FILE_ENCODING_TYPE_UTF16LE, LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the PropertiesFile 
		@param[in] b the second object
		*/
		PropertiesFile(const PropertiesFile& b);

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		PropertiesFile & operator=(const PropertiesFile&b);
		

		/*!
		Default Destructor

		Destroy the Properties File 
		*/
		virtual ~PropertiesFile();

		/*!
		Set the property with the given key with the value given
		@param[in] key the key of the property to change the value
		@param[in] val the value to change the property
		@return true if changed, otherwise false
		*/
		bool SetProperty(const TCHAR * key, const TCHAR * val);

		/*!
		Get the value of the property with the given key
		@param[in] key the key of the property to get the value
		@param[in] retVal the value of the property of given key
		@return true if found, otherwise false
		*/
		bool GetProperty(const TCHAR * key,EpTString &retVal) const;

		/*!
		Get the value of the property with the given key
		@param[in] key the key of the property to get the value
		@remark raises exception when key does not exists 
		@return the value of the property of given key
		*/
		EpTString &GetProperty(const TCHAR * key);

		/*!
		Get the value of the property with the given key
		@param[in] key the key of the property to get the value
		@remark raises exception when key does not exists 
		@return the value of the property of given key
		*/
		const EpTString &GetProperty(const TCHAR * key) const;

		/*!
		Add new property with the given key and value
		@param[in] key the key of the property to add
		@param[in] val the value of the new property
		@return true if successfully added, otherwise false
		*/
		bool AddProperty(const TCHAR * key, const TCHAR *val);

		/*!
		Remove the property with the given key
		@param[in] key the key of the property to remove
		@return true if successfully removed, otherwise false
		*/
		bool RemoveProperty(const TCHAR * key);

		/*!
		Clear the list of the properties
		*/
		void Clear();

		/*!
		If given key exists, then return the value, and 
		if given key does not exist, then create key and 
		return the reference to empty value.
		@param[in] key the key of the property to find/create
		@return value of the given key.
		*/
		EpTString& operator [](const TCHAR * key);

		/*!
		If given key exists, then return the value, and
		return the reference to empty value.
		@param[in] key the key of the property to find/create
		@return value of the given key.
		*/
		const EpTString& operator [](const TCHAR * key) const;

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

		/*!
		Parse the key and value from the line buffer
		@param[in] buf the buffer that holds a line
		@param[out] retKey the key part of the given line
		@param[out] retVal the value part of the given line
		@return true if successfully parsed the key and value, otherwise false
		*/
		bool getValueKeyFromLine(const EpTString &buf, EpTString &retKey, EpTString &retVal);

		/// The list of the properties
		vector<Pair<EpTString,EpTString> > m_propertyList;

		/// Null String
		EpTString m_nullString;
	};
}



#endif //__EP_PROPERTIES_FILE_H__