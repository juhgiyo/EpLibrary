/*! 
@file epWinProcessHelper.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief Window Processing Function Class Interface
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

An Interface for the Window Processing Operation.

*/
#ifndef __EP_WIN_PROCESS_HELPER_H__
#define __EP_WIN_PROCESS_HELPER_H__
#include "epLib.h"
#include "epSystem.h"
#include <vector>


namespace epl
{
	/*! 
	@class WinProcessHelper epWinProcessHelper.h
	@brief This is a class for Window Processing Class

	Implements the Window Processing Functions.
	*/
	class EP_LIBRARY WinProcessHelper
	{

	public:
		/*!
		Return the set of process id with given name
		@param[in] pProcessName the name of the window process
		@param[out] retSetOfPID the set of process id with given name
		*/
		static void GetProcessID(const TCHAR * pProcessName, std::vector<unsigned long>& retSetOfPID);

		/*!
		Terminate the all process with the given name
		@param[in] pProcessName the name of the window process to terminate
		@return true if terminated, otherwise false
		*/
		static bool TerminateProcess(const TCHAR * pProcessName);

		/*!
		Terminate the process with the given process ID
		@param[in] processID the id of the window process to terminate
		@return true if terminated, otherwise false
		*/
		static bool TerminateProcess(unsigned long processID);

		/*!
		Switch the focus to the process with the given process ID
		@param[in] pProcessName the name of the window process to switch
		@return true if switched, otherwise false
		*/
		static bool SwitchToProcess(const TCHAR *pProcessName);
		
		/*!
		Switch the focus to the process with the given process ID
		@param[in] processID the id of the window process to switch
		@return true if switched, otherwise false
		*/
		static bool SwitchToProcess(unsigned long processID);
		
		/*!
		Get the number of process with the given name
		@param[in] pProcessName the name of the window process
		@return the number of process with the given name
		*/
		static unsigned int GetNumberOfProcess(const TCHAR * pProcessName);
	};
}
#endif //__EP_WIN_PROCESS_HELPER_H__