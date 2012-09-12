/*! 
@file epNetworkStream.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2011
@brief Network Stream Interface
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
		void SetWriteSeek(const StreamSeekType seekType,const unsigned int offset=0);

		/*!
		Get the current write seek offset.
		@return the current write seek offset.
		*/
		unsigned int GetWriteSeek() const;

		/*!
		Set the read seek offset. 
		@param[in] seekType The type of Seek to set
		@param[in] offset The offset from the seek type.
		*/
		void SetReadSeek(const StreamSeekType seekType,const unsigned int offset=0);

		/*!
		Get the current read seek offset.
		@return the current read seek offset.
		*/
		unsigned int GetReadSeek() const;

		

		/*!
		Network Stream does not support this method!!
		(DO NOT Use This Method for Network Stream!!)
		@param[in] seekType The type of Seek to set
		@param[in] offset The offset from the seek type.
		*/
		virtual void SetSeek(const StreamSeekType seekType,const unsigned int offset=0);

		/*!
		Network Stream does not support this method!!
		(DO NOT Use This Method for Network Stream!!)
		@return the current seek offset.
		*/
		virtual unsigned int GetSeek() const;
	private:
		/*!
		Read the value from the stream.
		@param[in] value the value/values to read from the stream
		@param[in] byteSize the byte size of the value
		@return true if successful, otherwise false.
		*/
		virtual bool read(void *value,const int byteSize);

		/// Network Stream Flush Type
		NetworkStreamFlushType m_flushType;
		/// Read Seek Offset
		unsigned int m_readOffset;
	};
}

#endif //__EP_NETWORK_STREAM_H__