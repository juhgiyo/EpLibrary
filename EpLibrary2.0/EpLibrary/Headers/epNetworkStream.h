/*! 
@file epNetworkStream.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2011
@brief Network Stream Interface
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

An Interface for Network Stream Class.

*/
#ifndef __EP_NETWORK_STREAM_H__
#define __EP_NETWORK_STREAM_H__
#include "epLib.h"
#include "epStream.h"

namespace epl
{
	/*! 
	@class NetworkStream epNetworkStream.h
	@brief A class for Network Stream.
	*/
	class EP_LIBRARY NetworkStream:public Stream
	{
	public:
		enum NetworkStreamFlushType{
			NETWORK_STREAM_FLUSH_TYPE_MANUAL=0,
			NETWORK_STREAM_FLUSH_TYPE_AUTO,
		};
		/*!
		Default Constructor

		Initializes the Network Stream
		@param[in] type flush type
		@param[in] lockPolicyType The lock policy
		*/
		NetworkStream(const NetworkStreamFlushType type=NETWORK_STREAM_FLUSH_TYPE_MANUAL, LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the Network Stream
		@param[in] b the second object
		*/
		NetworkStream(const NetworkStream& b);


		/*!
		Default Destructor

		Destroy the Network Stream
		*/
		virtual ~NetworkStream();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		NetworkStream & operator=(const NetworkStream&b);

		/*!
		Return the flush type of this network stream
		@return the flush type of this network stream.
		*/		
		NetworkStreamFlushType GetFlushType() const;

		/*!
		Flush the Network Stream. 
		Erases up to the read seek offset, and offsets get reset.
		*/		
		virtual void Flush();

		/*!
		Set the write seek offset. 
		@param[in] seekType The type of Seek to set
		@param[in] offset The offset from the seek type.
		*/
		void SetWriteSeek(const StreamSeekType seekType,size_t offset=0);

		/*!
		Get the current write seek offset.
		@return the current write seek offset.
		*/
		size_t GetWriteSeek() const;

		/*!
		Set the read seek offset. 
		@param[in] seekType The type of Seek to set
		@param[in] offset The offset from the seek type.
		*/
		void SetReadSeek(const StreamSeekType seekType,size_t offset=0);

		/*!
		Get the current read seek offset.
		@return the current read seek offset.
		*/
		size_t GetReadSeek() const;

		

		/*!
		Network Stream does not support this method!!
		(DO NOT Use This Method for Network Stream!!)
		@param[in] seekType The type of Seek to set
		@param[in] offset The offset from the seek type.
		*/
		virtual void SetSeek(const StreamSeekType seekType,size_t offset=0);

		/*!
		Network Stream does not support this method!!
		(DO NOT Use This Method for Network Stream!!)
		@return the current seek offset.
		*/
		virtual size_t GetSeek() const;
	private:
		/*!
		Read the value from the stream.
		@param[in] value the value/values to read from the stream
		@param[in] byteSize the byte size of the value
		@return true if successful, otherwise false.
		*/
		virtual bool read(void *value,size_t byteSize);

		/// Network Stream Flush Type
		NetworkStreamFlushType m_flushType;
		/// Read Seek Offset
		size_t m_readOffset;
	};
}

#endif //__EP_NETWORK_STREAM_H__