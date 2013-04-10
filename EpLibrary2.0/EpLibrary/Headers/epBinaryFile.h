/*! 
@file epBinaryFile.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief Binary File Interface
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