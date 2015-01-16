/*! 
@file epLogWorker.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/epWinService>
@date January 16, 2013
@brief Log Worker Interface
@version 1.0

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

An Interface for Log Worker.

*/
#ifndef __EP_LOG_WORKER_H__
#define __EP_LOG_WORKER_H__

#include "epLib.h"
#include "epBaseTextFile.h"
#include "epThread.h"
#include <queue>

namespace epl
{
	/*! 
	@class LogWorker epLogWorker.h
	@brief A thread class for Writing Log.
	*/
	class EP_LIBRARY LogWorker:public BaseTextFile,protected Thread
	{
	public:
				/*!
		Default Constructor

		Initializes the Log Worker
		@param[in] fileName the filename of the log
		@param[in] encodingType the encoding type for this file
		@param[in] lockPolicyType The lock policy
		*/
		LogWorker(EpTString fileName,FileEncodingType encodingType=FILE_ENCODING_TYPE_UTF16LE, LockPolicy lockPolicyType=EP_LOCK_POLICY);
		
		/*!
		Default Copy Constructor

		Initializes the LogWorker 
		@param[in] b the second object
		*/
		LogWorker(const LogWorker& b);

		/*!
		Default Destructor

		Destroy the Log Worker
		*/
		virtual ~LogWorker();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		LogWorker & operator=(const LogWorker&b)
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
		Writer given message to the log with current time.
		@param[in] pMsg the message to print to the log file.
		*/
		void WriteLog(const TCHAR* pMsg);

	private:
		/*!
		Actual Thread Code.
		@remark Subclass should override this function for executing the thread function.
		*/
		virtual void execute();

		/*!
		Stop the writing thread
		*/
		void stop();

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
		

		/// Name of the Log File
		CString m_fileName;

		/// Log String
		CString m_logString;

		/// Log Lock
		BaseLock *m_logLock;
		/// Log Queue
		queue<CString> m_logQueue;
		/// Thread terminator
		bool m_shouldTerminate;
		/// Thread Lock
		BaseLock *m_threadLock;
		
	};
}

#endif //__EP_LOG_WORKER_H__