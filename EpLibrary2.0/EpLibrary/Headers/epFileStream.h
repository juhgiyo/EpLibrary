/*! 
@file epFileStream.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2011
@brief File Stream Interface
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

An Interface for File Stream Class.

*/
#ifndef __EP_FILE_STREAM_H__
#define __EP_FILE_STREAM_H__
#include "epLib.h"
#include "epStream.h"

namespace epl
{
	/*! 
	@class FileStream epFileStream.h
	@brief A class for File Stream.
	*/
	class EP_LIBRARY FileStream:public Stream
	{
	public:
		/*!
		Default Constructor

		Initializes the File Stream
		@param[in] fileName The File Name to load or write the stream
		@param[in] lockPolicyType The lock policy
		*/
		FileStream(const TCHAR *fileName=_T(""),LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the File Stream
		@param[in] b the second object
		*/
		FileStream(const FileStream& b);

		/*!
		Default Destructor

		Destroy the File Stream
		*/
		virtual ~FileStream();


		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		FileStream & operator=(const FileStream&b);

		/*!
		Set the file name of this stream as given string
		@param[in] fileName the name of the file to set.
		*/
		void SetFileName(const TCHAR *fileName);

		/*!
		Return the file name set for this stream
		@return the file name set for this stream.
		*/
		EpTString GetFileName() const;

		/*!
		Load the stream from the file
		@return true if successful, otherwise false.
		*/
		bool LoadStreamFromFile();

		/*!
		Write the stream to the file
		@return true if successful, otherwise false.
		*/
		bool WriteStreamToFile();

	private:
		/*!
		Write the value to the stream.
		@param[in] value the value/values to write to the stream
		@param[in] byteSize the byte size of the value
		@return true if successful, otherwise false.
		*/
		virtual bool write(const void * value,size_t byteSize);

		/*!
		Read the value from the stream.
		@param[in] value the value/values to read from the stream
		@param[in] byteSize the byte size of the value
		@return true if successful, otherwise false.
		*/
		virtual bool read(void * value,size_t byteSize);

		/// The file name to load/write the stream
		EpTString m_fileName;

	};
}
#endif //__EP_FILE_STREAM_H__