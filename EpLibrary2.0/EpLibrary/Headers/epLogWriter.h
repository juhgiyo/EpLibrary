/*! 
@file epLogWriter.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/epWinService>
@date January 16, 2013
@brief Log Writer Interface
@version 1.0

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

An Interface for Log Writer.

*/
#ifndef __EP_LOG_WRITER_H__
#define __EP_LOG_WRITER_H__

#include "epLib.h"
#include "epBaseTextFile.h"
#include "epSingletonHolder.h"


/*!
@def LOG_WRITER_INSTANCE
@brief Log Writer Singleton Instance

Macro for the Log Writer Singleton Instance.
*/
#define LOG_WRITER_INSTANCE epl::SingletonHolder<LogWriter>::Instance()

namespace epl
{
	/*! 
	@class LogWriter epLogWriter.h
	@brief A class for Writing Log.
	*/
	class EP_LIBRARY LogWriter:public BaseTextFile
	{
	public:
		friend SingletonHolder<LogWriter>;
		
		/*!
		Writer given message to the log with current time.
		@param[in] pMsg the message to print to the log file.
		*/
		void WriteLog(const TCHAR* pMsg);

	private:
		/*!
		Default Constructor

		Initializes the Log Writer
		@param[in] lockPolicyType The lock policy
		*/
		LogWriter(LockPolicy lockPolicyType=EP_LOCK_POLICY);
		
		/*!
		Default Copy Constructor

		Initializes the LogWriter 
		@param[in] b the second object
		*/
		LogWriter(const LogWriter& b);

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		LogWriter & operator=(const LogWriter&b)
		{
			if(this!=&b)
			{
				BaseTextFile::operator =(b);
				LockObj lock(m_logLock);
				m_fileName=b.m_fileName;
			}
			return *this;
		}

		/*!
		Loop Function that writes to the file.
		@remark Sub classes should implement this function
		*/
		virtual void writeLoop();

		/*!
		Actual load Function that loads values from the file.
		@remark Sub classes should implement this function
		@param[in] lines the all data from the file
		*/
		virtual void loadFromFile(const EpTString &lines);
		
		/*!
		Default Destructor

		Destroy the Log Writer
		*/
		virtual ~LogWriter();

		/// Name of the Log File
		CString m_fileName;

		/// Log String
		CString m_logString;

		/// Log Lock
		BaseLock *m_logLock;
		
	};
}

#endif //__EP_LOG_WRITER_H__