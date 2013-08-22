/*! 
@file epBinaryFile.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief Binary File Interface
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

An Interface for the Binary File Class.

*/
#ifndef __EP_BINARY_FILE_H__
#define __EP_BINARY_FILE_H__
#include "epLib.h"
#include "epSystem.h"
#include "epMemory.h"
#include "epCriticalSectionEx.h"
#include "epMutex.h"
#include "epNoLock.h"
#include "epStream.h"


namespace epl{
	/*!
	@class BinaryFile epBinaryFile.h
	@brief A class for Binary File.
	*/
	class EP_LIBRARY BinaryFile{
	public:

		/*!
		Default Constructor

		Initializes the Binary File 
		@param[in] lockPolicyType The lock policy
		*/
		BinaryFile( LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the Binary File 
		@param[in] b the second object
		*/
		BinaryFile(const BinaryFile& b);

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		BinaryFile & operator=(const BinaryFile&b);
		
		/*!
		Default Destructor

		Destroy the Binary File 
		*/
		virtual ~BinaryFile();

	
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
		Get the current stream
		@return the current stream
		*/
		Stream &GetStream();

		/*!
		Get the current stream as const stream
		@return the current stream
		*/
		const Stream &GetStream() const;

		/*!
		Set the stream as given stream
		@param[in] stream the stream to set
		*/
		void SetStream(const Stream &stream);


	protected:

		/// File Stream
		Stream m_stream;
		/// File Pointer
		EpFile *m_file;
		/// the lock
		BaseLock * m_baseTextLock;
		/// Lock Policy
		LockPolicy m_lockPolicy;
	};

}



#endif //__EP_BINARY_FILE_H__