/*! 
@file epBaseOutputter.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2011
@brief BaseOutputter Interface
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

An Interface for Outputting data Template class.

*/
#ifndef __EP_OUTPUTTER_H__
#define __EP_OUTPUTTER_H__
#include "epLib.h"
#include "epSystem.h"
#include "epThreadSafeClass.h"
#include "epCriticalSectionEx.h"
#include "epMutex.h"
#include "epNoLock.h"

namespace epl
{
	/*! 
	@class BaseOutputter epBaseOutputter.h
	@brief A Base Class for output the data.
	*/
	class EP_LIBRARY BaseOutputter
	{
	public:

		/*!
		Write the all data to the file.
		*/
		virtual void FlushToFile();
		/*!
		Set the output file name.
		@param[in] fileName the file name for the output.
		*/
		virtual void SetFileName(const TCHAR *fileName);

		/*!
		Print the data to command line.
		*/
		virtual void Print() const;

		/*!
		Clear all the data.
		*/
		virtual void Clear();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		BaseOutputter & operator=(const BaseOutputter&b)
		{
			if(this!=&b)
			{
				LockObj lock(m_nodeListLock);
				m_list=b.m_list;
				m_fileName=b.m_fileName;
			}
			return *this;
		}

	protected:


		/*! 
		@class OutputNode epBaseOutputter.h
		@brief A virtual class for OutputNode.
		*/
		class  EP_LIBRARY OutputNode
		{
		public:
			/*!
			Default Constructor
			*/
			OutputNode();
			/*!
			Default Destructor
			*/
			virtual ~OutputNode();
			/*!
			Default Copy Constructor

			Initializes the BaseClient
			@param[in] b the second object
			*/
			OutputNode(const OutputNode& b);

			/*!
			Assignment operator overloading
			@param[in] b the second object
			@return the new copied object
			*/
			OutputNode & operator=(const OutputNode&b)
			{
				return *this;
			}

			/*!
			Print the data in format,
			*/
			virtual void Print() const=0;
			/*!
			Write the data to file in format,
			@param[in] file the file to output the data.
			*/
			virtual void Write(EpFile* const file)=0;
		};

		/*!
		Default Constructor
		@param[in] lockPolicyType The lock policy
		*/
		BaseOutputter(LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the BaseClient
		@param[in] b the second object
		*/
		BaseOutputter(const BaseOutputter& b);
		/*!
		Default Destructor
		*/
		virtual ~BaseOutputter();

		/// Log List
		std::vector<OutputNode*> m_list;
		/// Lock
		BaseLock* m_nodeListLock;
		/// Lock Policy
		LockPolicy m_lockPolicy;
		/// File Name
		EpTString m_fileName;

	private:
		/*!
		Write the data to given file.
		@param[in] file the file which data will be written.
		*/
		void writeToFile(EpFile* const file);
	};
}
#endif //__EP_OUTPUTTER_H__