/*! 
@file epStream.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2010
@brief Stream Interface
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

An Interface for Stream Class.

*/
#ifndef __EP_STREAM_H__
#define __EP_STREAM_H__
#include "epLib.h"
#include "epCriticalSectionEx.h"
#include "epMutex.h"
#include "epNoLock.h"

namespace epl
{

	/*! 
	@class Stream epStream.h
	@brief A class for Stream.
	*/
	class EP_LIBRARY Stream
	{
	public:
		/// Enumeration for Stream Seek Type
		enum StreamSeekType{
			/// Seek to the start of the stream
			STREAM_SEEK_TYPE_SEEK_SET=0,
			/// Seek to the current seek
			STREAM_SEEK_TYPE_SEEK_CUR,
			/// Seek to the end of the stream
			STREAM_SEEK_TYPE_SEEK_END,
		};

		/*!
		Default Constructor

		Initializes the Stream
		@param[in] lockPolicyType The lock policy
		*/
		Stream(LockPolicy lockPolicyType=EP_LOCK_POLICY);
		
		/*!
		Default Copy Constructor

		Initializes the Stream
		@param[in] b the second object
		*/
		Stream(const Stream& b);

		/*!
		Default Destructor

		Destroy the Stream
		*/
		virtual ~Stream();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		Stream & operator=(const Stream&b);

		/*!
		Clear the Stream.
		*/
		void Clear();
		/*!
		Return the byte size of the stream
		@return the byte size of the stream.
		*/
		int GetStreamSize() const;

		/*!
		Return the buffer pointer of the stream
		@return the buffer pointer of the stream.
		*/
		const unsigned char *GetBuffer() const;

		/*!
		Set the seek offset. 
		@param[in] seekType The type of Seek to set
		@param[in] offset The offset from the seek type.
		*/
		virtual void SetSeek(const StreamSeekType seekType,const unsigned int offset=0);
		
		/*!
		Get the current seek offset.
		@return the current seek offset.
		*/
		virtual unsigned int GetSeek() const;

		/*!
		Write the short value to the stream.
		@param[in] value the short value to write to the stream
		@return true if successful, otherwise false.
		*/
		virtual bool WriteShort(const short value);

		/*!
		Write the unsigned short value to the stream.
		@param[in] value the unsigned short value to write to the stream
		@return true if successful, otherwise false.
		*/
		virtual bool WriteUShort(const unsigned short value);

		/*!
		Write the int value to the stream.
		@param[in] value the int value to write to the stream
		@return true if successful, otherwise false.
		*/
		virtual bool WriteInt(const int value);

		/*!
		Write the unsigned int value to the stream.
		@param[in] value the unsigned int value to write to the stream
		@return true if successful, otherwise false.
		*/
		virtual bool WriteUInt(const unsigned int value);

		/*!
		Write the float value to the stream.
		@param[in] value the float value to write to the stream
		@return true if successful, otherwise false.
		*/
		virtual bool WriteFloat(const float value);

		/*!
		Write the double value to the stream.
		@param[in] value the double value to write to the stream
		@return true if successful, otherwise false.
		*/
		virtual bool WriteDouble(const double value);

		/*!
		Write the byte value to the stream.
		@param[in] value the byte value to write to the stream
		@return true if successful, otherwise false.
		*/
		virtual bool WriteByte(const unsigned char value);
		
		/*!
		Write the short values to the stream.
		@param[in] shortList the short values to write to the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool WriteShorts(const short *shortList, const unsigned int listSize);

		/*!
		Write the unsigned short values to the stream.
		@param[in] ushortList the unsigned short values to write to the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool WriteUShorts(const unsigned short *ushortList, const unsigned int listSize);

		/*!
		Write the int values to the stream.
		@param[in] intList the int values to write to the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool WriteInts(const int *intList, const unsigned int listSize);

		/*!
		Write the unsigned int values to the stream.
		@param[in] uintList the unsigned int values to write to the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool WriteUInts(const unsigned int *uintList, const unsigned int listSize);

		/*!
		Write the float values to the stream.
		@param[in] floatList the float values to write to the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool WriteFloats(const float *floatList, const unsigned int listSize);

		/*!
		Write the double values to the stream.
		@param[in] doubleList the double values to write to the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool WriteDoubles(const double *doubleList,const unsigned int listSize);

		/*!
		Write the byte values to the stream.
		@param[in] byteList the byte values to write to the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool WriteBytes(const unsigned char* byteList,const unsigned int listSize);


		/*!
		Read the short value from the stream.
		@param[out] retVal the short value read from the stream
		@return true if successful, otherwise false.
		*/
		virtual bool ReadShort(short &retVal);

		/*!
		Read the unsigned short value from the stream.
		@param[out] retVal the unsigned short value read from the stream
		@return true if successful, otherwise false.
		*/
		virtual bool ReadUShort(unsigned short &retVal);

		/*!
		Read the int value from the stream.
		@param[out] retVal the int value read from the stream
		@return true if successful, otherwise false.
		*/
		virtual bool ReadInt(int &retVal);

		/*!
		Read the unsigned int value from the stream.
		@param[out] retVal the unsigned int value read from the stream
		@return true if successful, otherwise false.
		*/
		virtual bool ReadUInt(unsigned int &retVal);

		/*!
		Read the float value from the stream.
		@param[out] retVal the float value read from the stream
		@return true if successful, otherwise false.
		*/
		virtual bool ReadFloat(float &retVal);

		/*!
		Read the double value from the stream.
		@param[out] retVal the double value read from the stream
		@return true if successful, otherwise false.
		*/
		virtual bool ReadDouble(double &retVal);

		/*!
		Read the byte value from the stream.
		@param[out] retVal the byte value read from the stream
		@return true if successful, otherwise false.
		*/
		virtual bool ReadByte(unsigned char &retVal);

		/*!
		Read the short values from the stream.
		@param[out] retShortList the short values read from the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool ReadShorts(short *retShortList, const unsigned int listSize);

		/*!
		Read the unsigned short values from the stream.
		@param[out] retUshortList the unsigned short values read from the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool ReadUShorts(unsigned short *retUshortList, const unsigned int listSize);

		/*!
		Read the int values from the stream.
		@param[out] retIntList the int values read from the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool ReadInts(int *retIntList, const unsigned int listSize);

		/*!
		Read the unsigned int values from the stream.
		@param[out] retUintList the unsigned int values read from the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool ReadUInts(unsigned int *retUintList, const unsigned int listSize);

		/*!
		Read the float values from the stream.
		@param[out] retFloatList the float values read from the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool ReadFloats(float *retFloatList, const unsigned int listSize);

		/*!
		Read the double values from the stream.
		@param[out] retDoubleList the double values read from the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool ReadDoubles(double *retDoubleList, const unsigned int listSize);

		/*!
		Read the byte values from the stream.
		@param[out] retByteList the byte values read from the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool ReadBytes(unsigned char* retByteList, const unsigned int listSize);
		
		/*!
		Write the current stream to the given file
		@param[in] fileName the name of the file to write to.
		@return true if successful, otherwise false.
		*/
		bool WriteStreamToFile(const TCHAR *fileName);

	protected:
		/*!
		Write the value to the stream.
		@param[in] value the value/values to write to the stream
		@param[in] byteSize the byte size of the value
		@return true if successful, otherwise false.
		*/
		virtual bool write(const void *value,const int byteSize);

		/*!
		Read the value from the stream.
		@param[in] value the value/values to read from the stream
		@param[in] byteSize the byte size of the value
		@return true if successful, otherwise false.
		*/
		virtual bool read(void *value,const int byteSize);

		/// The actual stream buffer
		std::vector<unsigned char> m_stream;
		/// The offset for the seek
		unsigned int m_offset;
		/// The Stream Lock
		BaseLock *m_streamLock;
		/// Lock Policy
		LockPolicy m_lockPolicy;
		
	};
}
#endif //__EP_STREAM_H__