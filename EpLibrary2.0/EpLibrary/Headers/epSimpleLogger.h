/*! 
@file epSimpleLogger.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2011
@brief SimpleLogManager Interface
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

An Interface for Managing the Logs.

*/
#ifndef __EP_SIMPLE_LOGGER_H__
#define __EP_SIMPLE_LOGGER_H__
#include "epLib.h"
#include "epBaseOutputter.h"
#include "epSingletonHolder.h"



/*!
@def LOG_INSTANCE
@brief A Simple Macro to get the Log Manager Instance

Macro that returns the reference of Simple Log Manager Instance.
*/
#define LOG_INSTANCE epl::SingletonHolder<epl::SimpleLogManager>::Instance()

/*!
@def LOG_THIS
@brief Simple Macro to log simple line.

Macro that logs the line and time where it called.
*/
#if  defined(_DEBUG) && defined(EP_ENABLE_LOG)
#define LOG_THIS LOG_INSTANCE.AddSimpleLog(__TFILE__,__TFUNCTION__,__LINE__,_T(""))
#else
#define LOG_THIS ((void)0)
#endif

/*!
@def LOG_THIS_MSG
@brief Simple Macro to log simple line with msg.

Macro that logs the line, and time where it called, with user message.
*/
#if  defined(_DEBUG) && defined(EP_ENABLE_LOG)
#define LOG_THIS_MSG(inputString,...) LOG_INSTANCE.AddSimpleLog(__TFILE__,__TFUNCTION__,__LINE__,inputString,__VA_ARGS__)
#else
#define LOG_THIS_MSG(inputString,...) ((void)0)
#endif

namespace epl
{

	/*! 
	@class SimpleLogManager epSimpleLogger.h
	@brief A class that manages the logs.
	*/
	class EP_LIBRARY SimpleLogManager:public BaseOutputter
	{
	public:
		friend class SingletonHolder<SimpleLogManager>;

		/*!
		Add the new simple log to the simple log list.
		@param[in] fileName the File Name for the log.
		@param[in] funcName The Function Name for the log.
		@param[in] lineNum The Line Number for the log.
		@param[in] format The user inputted message
		*/
		void AddSimpleLog(const TCHAR *fileName, const TCHAR *funcName,const int lineNum,const TCHAR *format,...);

		/*!
		Write the all data to the file.
		*/
		virtual void FlushToFile();
		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		SimpleLogManager & operator=(const SimpleLogManager&b)
		{
			if(this!=&b)
			{
				BaseOutputter::operator =(b);
			}
			return *this;
		}
		
	private:
		/*! 
		@class SimpleLogNode epSimpleLogger.h
		@brief A class to hold simple log related members.
		*/
		class EP_LIBRARY SimpleLogNode: public BaseOutputter::OutputNode
		{
		public:

			friend class SimpleLogManager;
			/*!
			Default Constructor
			*/
			SimpleLogNode();
	
			/*!
			Default Copy Constructor

			Initializes the BaseClient
			@param[in] b the second object
			*/
			SimpleLogNode(const SimpleLogNode& b);

			/*!
			Default Destructor
			*/
			virtual ~SimpleLogNode();

			/*!
			Assignment operator overloading
			@param[in] b the second object
			@return the new copied object
			*/
			SimpleLogNode & operator=(const SimpleLogNode&b);

			/*!
			Print the data in format,
			*/
			virtual void Print() const;

			/*!
			Write the data to file in format,
			@param[in] file the file to output the data.
			*/
			virtual void Write(EpFile* const file);

		private:
			/// The name of file where the log is called.
			EpTString m_fileName;
			/// The name of function where the log is called.
			EpTString m_funcName;
			/// The line number where the log is called.
			int m_lineNum;
			/// The date when the log is called.
			TCHAR m_dateStr[9];
			/// The time when the log is called.
			TCHAR m_timeStr[9];
			/// The user string
			EpTString m_userStr;

		};


		/*!
		Default Constructor
		@param[in] lockPolicyType The lock policy
		*/
		SimpleLogManager(LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the BaseClient
		@param[in] b the second object
		*/
		SimpleLogManager(const SimpleLogManager& b);

		/*!
		Default Destructor
		*/
		virtual ~SimpleLogManager();

	};

}
#endif //__EP_SIMPLE_LOGGER_H__
