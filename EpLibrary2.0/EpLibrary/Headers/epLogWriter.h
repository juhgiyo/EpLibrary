/*! 
@file epLogWriter.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/epWinService>
@date January 16, 2013
@brief Log Writer Interface
@version 1.0

@section LICENSE

Copyright (C) 2013  Woong Gyu La <juhgiyo@gmail.com>

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