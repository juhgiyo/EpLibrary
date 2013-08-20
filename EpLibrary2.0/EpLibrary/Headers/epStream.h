/*! 
@file epStream.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2010
@brief Stream Interface
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
		size_t GetStreamSize() const;

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
		virtual void SetSeek(const StreamSeekType seekType,size_t offset=0);
		
		/*!
		Get the current seek offset.
		@return the current seek offset.
		*/
		virtual size_t GetSeek() const;

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
		virtual bool WriteShorts(const short *shortList, size_t listSize);

		/*!
		Write the unsigned short values to the stream.
		@param[in] ushortList the unsigned short values to write to the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool WriteUShorts(const unsigned short *ushortList, size_t listSize);

		/*!
		Write the int values to the stream.
		@param[in] intList the int values to write to the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool WriteInts(const int *intList,size_t listSize);

		/*!
		Write the unsigned int values to the stream.
		@param[in] uintList the unsigned int values to write to the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool WriteUInts(const unsigned int *uintList,size_t listSize);

		/*!
		Write the float values to the stream.
		@param[in] floatList the float values to write to the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool WriteFloats(const float *floatList, size_t listSize);

		/*!
		Write the double values to the stream.
		@param[in] doubleList the double values to write to the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool WriteDoubles(const double *doubleList,size_t listSize);

		/*!
		Write the byte values to the stream.
		@param[in] byteList the byte values to write to the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool WriteBytes(const unsigned char* byteList,size_t listSize);

		/*!
		Write given string to the stream
		@param[in] str the string to write.
		@return true if successfully written otherwise false
		*/
		virtual bool WriteString(const char *str);

		/*!
		Write given wide string to the stream
		@param[in] str the string to write.
		@return true if successfully written otherwise false
		*/
		virtual bool WriteWString(const wchar_t *str);
		
		/*!
		Write given TString to the stream
		@param[in] str the string to write.
		@return true if successfully written otherwise false
		*/
		virtual bool WriteTString(const TCHAR *str);

		/*!
		Write given string to the stream
		@param[in] str the string to write.
		@return true if successfully written otherwise false
		*/
		virtual bool WriteString(const EpString &str);

		/*!
		Write given wide string to the stream
		@param[in] str the string to write.
		@return true if successfully written otherwise false
		*/
		virtual bool WriteWString(const EpWString &str);
		
		/*!
		Write given TString to the stream
		@param[in] str the string to write.
		@return true if successfully written otherwise false
		*/
		virtual bool WriteTString(const EpTString &str);



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
		virtual bool ReadShorts(short *retShortList, size_t listSize);

		/*!
		Read the unsigned short values from the stream.
		@param[out] retUshortList the unsigned short values read from the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool ReadUShorts(unsigned short *retUshortList, size_t listSize);

		/*!
		Read the int values from the stream.
		@param[out] retIntList the int values read from the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool ReadInts(int *retIntList, size_t listSize);

		/*!
		Read the unsigned int values from the stream.
		@param[out] retUintList the unsigned int values read from the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool ReadUInts(unsigned int *retUintList,size_t listSize);

		/*!
		Read the float values from the stream.
		@param[out] retFloatList the float values read from the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool ReadFloats(float *retFloatList, size_t listSize);

		/*!
		Read the double values from the stream.
		@param[out] retDoubleList the double values read from the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool ReadDoubles(double *retDoubleList, size_t listSize);

		/*!
		Read the byte values from the stream.
		@param[out] retByteList the byte values read from the stream
		@param[in] listSize the size of the list
		@return true if successful, otherwise false.
		*/
		virtual bool ReadBytes(unsigned char* retByteList, size_t listSize);

		/*!
		Get String from the stream
		@param[out] retString the string extracted.
		@return true if successfully extracted otherwise false
		*/
		virtual bool ReadString(EpString &retString);

		/*!
		Get Wide String from the stream
		@param[out] retString the string extracted.
		@return true if successfully extracted otherwise false
		*/
		virtual bool ReadWString(EpWString &retString);

		/*!
		Get TString from the stream
		@param[out] retString the string extracted.
		@return true if successfully extracted otherwise false
		*/
		virtual bool ReadTString(EpTString &retString);
		
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
		virtual bool write(const void *value,size_t byteSize);

		/*!
		Read the value from the stream.
		@param[in] value the value/values to read from the stream
		@param[in] byteSize the byte size of the value
		@return true if successful, otherwise false.
		*/
		virtual bool read(void *value,size_t byteSize);

		/// The actual stream buffer
		std::vector<unsigned char> m_stream;
		/// The offset for the seek
		size_t m_offset;
		/// The Stream Lock
		BaseLock *m_streamLock;
		/// Lock Policy
		LockPolicy m_lockPolicy;
		
	};
}
#endif //__EP_STREAM_H__