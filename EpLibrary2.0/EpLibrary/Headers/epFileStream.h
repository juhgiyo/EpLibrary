/*! 
@file epFileStream.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2011
@brief File Stream Interface
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