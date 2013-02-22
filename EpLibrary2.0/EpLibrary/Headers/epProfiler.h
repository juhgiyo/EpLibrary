/*! 
@file epProfiler.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
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
#include "epLib.h"
#include "epBaseOutputter.h"
#include "epSingletonHolder.h"

/*!
@def PROFILE_INSTANCE
@brief A Simple Macro to get the Profile Manager Instance

Macro that returns the reference of Profile Manager Instance.
*/
#define PROFILE_INSTANCE epl::SingletonHolder<epl::ProfileManager>::Instance()


/*!
@def GET_NEW_UNIQUE_NAME
@brief Simple Macro to create unique name for the profiler.

Macro that creates the unique name for the profiler and returns TCHAR *.
@return TCHAR * array holding newly created unique name of the profiler
@remark Usage: Profiler x=Profiler(GET_NEW_UNIQUE_NAME());
*/
#define GET_NEW_UNIQUE_NAME() epl::Profiler::GetNewUniqueName(__TFILE__,__TFUNCTION__,__LINE__).c_str()

/*!
@def PROFILE_THIS
@brief Simple Macro to profile the function.

Macro that profiles the function where it called.
@param[in] varName the variable name for the profile
@remark Usage: PROFILE_THIS(profile);
*/
#if  defined(_DEBUG) && defined(EP_ENABLE_PROFILE)
#define PROFILE_THIS(varName) epl::ProfileObj varName(GET_NEW_UNIQUE_NAME())
#else
#define PROFILE_THIS(varName) ((void)0)
#endif




namespace epl
{
	/*! 
	@class Profiler epProfiler.h
	@brief This is a class for handling the profiling
	*/
	class EP_LIBRARY Profiler
	{
	public:

		/*!
		Default Constructor
		*/
		Profiler();
		
		/*!
		Default Copy Constructor

		*Cannot be Used.
		*/
		Profiler(const Profiler & b);

		/*!
		Default Contructor

		@param[in] uniqueName The unique name for the profiler.
		*/
		Profiler(const TCHAR *uniqueName);
		
		/*!
		Assignment operator overloading

		@param[in] b the second object
		@return the new copied object
		*/
		Profiler &operator=(const Profiler & b);

		/*!
		Default Destructor
		*/
		virtual ~Profiler();

		/*!
		Start profiling.
		*/
		void Start();

		/*!
		Stop profiling.
		@return the profiled time in milliseconds
		*/
		unsigned __int64 Stop();

		/*!
		Return the last profiled time in milliseconds.
		@return the last profiled time in milliseconds.
		*/
		unsigned __int64 GetLastProfileTime();

		/*!
		Create Name for the Profiler and returns the name
		@param[in] fileName the file name of the caller
		@param[in] functionName the function mae of the caller
		@param[in] lineNum the line number where it called
		@return EpTString with the newly created name
		*/
		static EpTString GetNewUniqueName(TCHAR *fileName, TCHAR *functionName,unsigned int lineNum);


	private:
		friend class ProfileObj;
		/*!
		Add the last profiled time to ProfileManager
		*/
		void addLastProfileTimeToManager();
		/// The start time that profiling started
		unsigned __int64 m_startTime;
		/// The end time that profiling ended
		unsigned __int64 m_endTime;
		/// The last profile time
		unsigned __int64 m_lastProfileTime;
		/// The Profiling Name
		EpTString m_uniqueName;

	};

	/*! 
	@class ProfileObj epProfiler.h
	@brief This is a class for handling the profiling
	*/
	class EP_LIBRARY ProfileObj
	{
	public:
		/*!
		Default Contructor

		If PROFILE_THIS Macro is used the inputs are automatically generated.
		@param[in] uniqueName the Name of the profiler.
		*/
		ProfileObj(const TCHAR *uniqueName);
		
		/*!
		Assignment operator overloading

		@param[in] b the second object
		@return the new copied object
		*/
		ProfileObj &operator=(const ProfileObj & b)
		{
			return *this;
		}
		/*!
		Default Destructor
		*/
		virtual ~ProfileObj();

	private:
		/*!
		Default Constructor

		*Cannot be Used.
		*/
		ProfileObj(){EP_ASSERT(0);}

		/*!
		Default Copy Constructor

		*Cannot be Used.
		*/
		ProfileObj(const ProfileObj & b){EP_ASSERT(0);}

		/// The profiler object
		Profiler m_profiler;

	};

	
	/*! 
	@class ProfileManager epProfiler.h
	@brief A class that manages the profile data.
	*/
	class EP_LIBRARY ProfileManager:public BaseOutputter
	{
	public:
		friend class SingletonHolder<ProfileManager>;
		friend class Profiler;
		
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
		Write the all data to the file.
		*/
		virtual void FlushToFile();

	

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
		class EP_LIBRARY ProfileNode:public BaseOutputter::OutputNode
		{
		public:
			friend class ProfileManager;

			/*!
			Default Constructor
			@remark Should NOT be used
			*/
			ProfileNode(const TCHAR *uniqueName);

			/*!
			Default Copy Constructor

			Initializes the BaseClient
			@param[in] b the second object
			*/
			ProfileNode(const ProfileNode& b);

			/*!
			Default Destructor
			*/
			virtual ~ProfileNode();

			/*!
			Assignment operator overloading
			@param[in] b the second object
			@return the new copied object
			*/
			ProfileNode & operator=(const ProfileNode&b);

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
			/*!
			Default Constructor
			@remark Should NOT be used
			*/
			ProfileNode():OutputNode() 
			{
				EP_ASSERT(0);
				m_cnt=0;
				m_totalTime=0;
			}

			/// Profiling Name
			EpTString m_uniqueName;
			/// The Quantity of Profiling occurred
			int m_cnt;
			/// The Total Profiling Time elapsed.
			unsigned __int64 m_totalTime;

		};

		/*!
		Check if the Profiling Log already exists in the profiling list.

		It searches the existence using the File Name and Function Name as the keys.
		If found, it returns the reference to that object to retIter.
		@param[in] uniqueName the Name of the profiler for searching.
		@param[out] retIter The reference to the found profiling log.
		@param[out] retIdx The index of the found profiling log.
		@return true if found, otherwise false.
		*/
		bool isProfileExist(const TCHAR *uniqueName,ProfileNode * &retIter, int &retIdx );


		
		/*!
		Add the new profiling log to the profiling list.
		@param[in] uniqueName the Name for the profiler.
		@param[in] time The ellapsed time of the profiling.
		*/
		void addProfile(const TCHAR *uniqueName, const unsigned __int64 &time);

	};

}
#endif //__EP_PROFILER_H__

