/*! 
@file epCmdLineOptions.h
@author Chris Losinger - 1999
	edited by Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com> - February 19, 2013
			  <http://github.com/juhgiyo/eplibrary>
@date 1999
@brief CmdLineOptions Interface

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

------------------------------------------------------------------------------

Part of this software is

CCmdLine

A utility for parsing command lines.

Copyright (C) 1999 Chris Losinger, Smaller Animals Software.
http://www.smalleranimals.com

This software is provided 'as-is', without any express
or implied warranty.  In no event will the authors be 
held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software 
for any purpose, including commercial applications, and 
to alter it and redistribute it freely, subject to the 
following restrictions:

1. The origin of this software must not be misrepresented; 
you must not claim that you wrote the original software. 
If you use this software in a product, an acknowledgment 
in the product documentation would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, 
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source 
distribution.

Please refer to <http://www.codeproject.com/Articles/1205/CCmdLine-command-line-parser> for the license.

@section DESCRIPTION

An Interface for CmdLineOptions Class.
*/
#ifndef __EP_CMDLINE_OPTIONS_H__
#define __EP_CMDLINE_OPTIONS_H__

#include "epLib.h"
#include "epLocale.h"
#include <vector>
#include <map>

using namespace std;

namespace epl
{
	/*! 
	@struct CmdArgs epCmdLineOptions.h
	@brief A CmdLine Argument List class.
	*/
	struct EP_LIBRARY CmdArgs
	{
		/// List of arguments
		vector<EpTString> m_args;
	};

	/// type definition for Option and Argument List pairs
	typedef map<EpTString,CmdArgs> CmdLineMap;

	/*! 
	@class CmdLineOptions epCmdLineOptions.h
	@brief A CmdLine Options class.
	*/
	class EP_LIBRARY CmdLineOptions:public CmdLineMap
	{
	public:
		/*!
		Default Constructor

		Initializes the CmdLine Options
		*/
		CmdLineOptions();
		
		/*!
		Default Copy Constructor

		Initializes the CmdLine Options with given CmdLine Options
		@param[in] b the CmdLine Options Object to copy from
		*/
		CmdLineOptions(const CmdLineOptions& b);

		/*!
		Default Destructor

		Destroys the CmdLine Options
		*/
		virtual ~CmdLineOptions();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		CmdLineOptions &operator=(const CmdLineOptions & b);


		/*!
		Parse the Command Line Argument with given.
		@param[in] argc the number of arguments of Command Line
		@param[in] argv the array of argument strings.
		@return the number of CmdLine Options parsed
		*/
		size_t Parse(int argc, TCHAR **argv);

		/*!
		Check if CmdLineOptions contain the given option
		@param[in] option the option string to check
		@return true if exists otherwise false
		*/
		bool HasOption(const TCHAR *option) const;

		/*!
		Get argument of given option at given index.
		@param[in] option the option string to get argument
		@param[in] idx the index of the arguments of given option
		@param[in] defaultArg the default argument string if not found 
		@return the argument string found.
		@remark if the argument does not exist then return given default argument string.
		*/
		EpTString GetArgument(const TCHAR *option,size_t idx, const TCHAR *defaultArg) const;

		/*!
		Get argument of given option at given index.
		@param[in] option the option string to get argument
		@param[in] idx the index of the arguments of given option
		@return the argument string found.
		@remark if the argument does not exist then throws exception 0.
		*/
		EpTString GetArgument(const TCHAR *option,size_t idx) const;

		/*!
		Get the number of arguments of given option
		@param[in] option the option string to get the number of arguments
		@return the number of arguments of given option
		@remark if the option does not exist then return -1
		*/
		int GetArgumentCount(const TCHAR *option) const;

	protected:
		/*!
		Check if given option is an option (starts with '-')
		@param[in] option the option string to check
		@return true if given option string is an option otherwise false.
		*/
		bool isOption(const TCHAR *option) const;



	};


}

#endif //__EP_CMDLINE_OPTIONS_H__