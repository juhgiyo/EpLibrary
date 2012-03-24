/*! 
@file epBaseOutputter.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
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
#include "epCriticalSectionEx.h"
#include "epThreadPolicy.h"

namespace epl
{
	/*! 
	@class BaseOutputter epBaseOutputter.h
	@brief A Base Class for output the data.
	*/
	class EP_FOUNDATION BaseOutputter
	{
	public:
		/*!
		Write the data to given file.
		@param[in] file the file which data will be written.
		*/
		virtual void WriteToFile(EpFile* const file);

		/*!
		Print the data to command line.
		*/
		virtual void Print();

		/*!
		Clear all the data.
		*/
		virtual void Clear();

	protected:
		/*! 
		@class OutputNode epBaseOutputter.h
		@brief A virtual class for OutputNode.
		*/
		class  EP_FOUNDATION OutputNode
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
			Print the data in format,
			*/
			virtual void Print()=0;
			/*!
			Write the data to file in format,
			@param[in] file the file to output the data.
			*/
			virtual void Write(EpFile* const file)=0;
		};

		/*!
		Default Constructor
		*/
		BaseOutputter();
		/*!
		Default Destructor
		*/
		virtual ~BaseOutputter();

		/// Log List
		std::vector<OutputNode*> m_list;
		/// Lock
		CriticalSectionEx m_nodeListLock;
	};
}
#endif //__EP_OUTPUTTER_H__