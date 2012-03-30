/*! 
@file epProfiler.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date April 17, 2011
@brief ProfileManager Interface
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

An Interface for the Debugging Profiler.

*/
#ifndef __EP_PROFILER_H__
#define __EP_PROFILER_H__
#include "epFoundationLib.h"
#include "epBaseOutputter.h"
#include "epSingletonHolder.h"




/*!
@def PROFILE_INSTANCE
@brief A Simple Macro to get the Profile Manager Instance

Macro that returns the reference of Profile Manager Instance.
*/
#define PROFILE_INSTANCE epl::SingletonHolder<epl::ProfileManager>::Instance()

/*!
@def PROFILE_THIS
@brief Simple Macro to profile the function.

Macro that profiles the function where it called.
*/
#if  defined(_DEBUG) && defined(ENABLE_PROFILE)
#define PROFILE_THIS epl::ProfileManager::Profiler prof=epl::ProfileManager::Profiler(__WFILE__,__WFUNCTION__)
#else
#define PROFILE_THIS ((void)0)
#endif

namespace epl
{
	/*! 
	@class ProfileManager epProfiler.h
	@brief A class that manages the profile data.
	*/
	class EP_FOUNDATION ProfileManager:public BaseOutputter
	{
	public:
		friend class SingletonHolder<ProfileManager>;

		/*!
		Add the new profiling log to the profiling list.
		@param[in] fileName the File Name for the profiling log.
		@param[in] funcName the Function Name for the profiling log.
		@param[in] time The ellapsed time of the profiling.
		*/
		void AddProfile(const TCHAR *fileName,const TCHAR * funcName, const EpTime &time);

		
		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		ProfileManager & operator=(const ProfileManager&b)
		{
			if(this!=&b)
			{
				BaseOutputter::operator =(b);
			}
			return *this;
		}

		/*! 
		@class Profiler epProfiler.h
		@brief This is a class for handling the profiling
		*/
		class EP_FOUNDATION Profiler
		{
			friend ProfileManager;
		public:
			/*!
			Default Contructor

			If PROFILE_THIS Macro is used the inputs are automatically generated.
			@param[in] fileName The file name that gets profiling.
			@param[in] funcName The function name that gets profiled.
			*/
			Profiler(const TCHAR *fileName,const TCHAR * funcName);
			
			/*!
			Default Destructor
			*/
			virtual ~Profiler();

		private:
			/// The start time that profiling started
			EpTime m_startTime;
			/// The Profiling File Name
			EpString m_fileName;
			/// The Profiling Function Name
			EpString m_funcName;

		};

	private:
		/*!
		Default Constructor
		@param[in] lockPolicyType The lock policy
		*/
		ProfileManager(LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the BaseClient
		@param[in] b the second object
		*/
		ProfileManager(const ProfileManager& b);

		/*!
		Default Destructor
		*/
		virtual ~ProfileManager();



		/*! 
		@class ProfileNode epProfiler.h
		@brief A class to hold profiling related members.
		*/
		class EP_FOUNDATION ProfileNode:public BaseOutputter::OutputNode
		{
		public:
			friend class ProfileManager;

			/*!
			Default Constructor
			*/
			ProfileNode();

			/*!
			Default Destructor
			*/
			virtual ~ProfileNode();

			/*!
			It prints the data in format,
			*/
			virtual void Print() const;

			/*!
			Write the data to file in format,
			@param[in] file the file to output the data.
			*/
			virtual void Write(EpFile* const file);


			/*!
			Compare two ProfileNode objects which given as parameter,
			and returns the CompResultType.
			@param[in] a the first ProfileNode object.
			@param[in] b the second ProfileNode object.
			@return 0 if same, 1 if greater than, -1 if less than.
			*/
			static CompResultType Compare(const void *a, const void *b);

		private:
			/// Profiling File Name
			EpString m_fileName;
			/// Profiling Function Name
			EpString m_funcName;
			/// The Quantity of Profiling occurred
			int m_cnt;
			/// The Total Profiling Time elapsed.
			EpTime m_totalTime;

		};



		/*!
		Check if the Profiling Log already exists in the profiling list.

		It searches the existence using the File Name and Function Name as the keys.
		If found, it returns the reference to that object to retIter.
		@param[in] fileName the File Name for searching.
		@param[in] funcName the Function Name for searching.
		@param[out] retIter The reference to the found profiling log.
		@param[out] retIdx The index of the found profiling log.
		@return true if found, otherwise false.
		*/
		bool isProfileExist(const TCHAR *fileName,const TCHAR * funcName,ProfileNode * &retIter, int &retIdx );

	};
}
#endif //__EP_PROFILER_H__
