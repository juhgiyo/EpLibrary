/*! 
@file epBaseOutputter.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2011
@brief BaseOutputter Interface
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
		BaseOutputter & operator=(const BaseOutputter&b);
		

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