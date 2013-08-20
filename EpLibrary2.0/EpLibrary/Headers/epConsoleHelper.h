/*! 
@file epConsoleHelper.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief Console Processing Function Class Interface
@version 2.0

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

An Interface for the Console Processing Operation.

*/
#ifndef __EP_CONSOLE_HELPER_H__
#define __EP_CONSOLE_HELPER_H__
#include "epLib.h"
#include "epSystem.h"

namespace epl
{
	/*! 
	@class ConsoleHelper epConsoleHelper.h
	@brief This is a class for Console Processing Class

	Implements the Console Processing Functions.
	*/
	class EP_LIBRARY ConsoleHelper
	{
	public:
		/*!
		Execute the given command to the console and return the result

		** waitStruct is ignored when isWaitForTerminate is false.
		@param[in] command the command to execute
		@param[in] isDosCommand flag whether the command is standard DOS command or not
		@param[in] isWaitForTerminate flag for waiting for process to terminate or not
		@param[in] isShowWindow flag for whether to show console window
		@param[in] useStdOutput flag for whether console to print to console window or to pipe.<br/>
		                        true to print to pipe; false to print to console.
		@param[in] priority the priority of the process executing
		@param[out] retProcessHandle the handle to the process created
		@return the result of the console command
		@remark retProcessHandle will be NULL when the function exits.
		        This can be used when isWaitForTerminate is true, and you need to terminate the process while waiting.
				Terminate the process from the other thread using the given handle pointer.
		*/
		static EpTString ExecuteConsoleCommand(const TCHAR * command, bool isDosCommand=false, bool isWaitForTerminate=true, bool isShowWindow=false,bool useStdOutput=true,  ConsolePriority priority=CONSOLE_PRIORITY_NORMAL, HANDLE *retProcessHandle=NULL);

		/*!
		Execute the given executable file
		@param[in] execFilePath the program file path to execute
		@param[in] parameters the parameter variables for executing file
		*/
		static void ExecuteProgram(const TCHAR *execFilePath, const TCHAR *parameters=NULL);
	};
}


#endif //__EP_CONSOLE_HELPER_H__