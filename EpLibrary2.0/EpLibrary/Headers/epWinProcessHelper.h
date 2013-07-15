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

	/// Process Priority Enumerator
	typedef enum _processPriority
	{
#if (_MSC_VER >=MSVC80) && (WINVER>=WINDOWS_XP) // Only for VS2005 and above and Windows XP and above
		/// Process that has priority above NORMAL_PRIORITY_CLASS <br/>
		///    but below HIGH_PRIORITY_CLASS.
		EP_PROCESS_PRIORITY_ABOVE_NORMAL_PRIORITY_CLASS=THREAD_PRIORITY_ABOVE_NORMAL,
		/// Process that has priority above IDLE_PRIORITY_CLASS<br/>
		///    but below NORMAL_PRIORITY_CLASS.
		EP_PROCESS_PRIORITY_BELOW_NORMAL_PRIORITY_CLASS=THREAD_PRIORITY_BELOW_NORMAL,
#endif //(_MSC_VER >=MSVC80) && (WINVER>=WINDOWS_XP)
		/// Process that performs time-critical tasks that must be executed<br/>
		///    immediately for it to run correctly. <br/>
		///    The threads of a high-priority class process preempt the threads of normal<br/>
		///    or idle priority class processes. <br/>
		///    An example is the Task List, which must respond quickly when called by the user,<br/>
		///    regardless of the load on the operating system. <br/>
		///    Use extreme care when using the high-priority class,<br/>
		///    because a high-priority class CPU-bound application can use nearly all available cycles.<br/>
		EP_PROCESS_PRIORITY_HIGH_PRIORITY_CLASS=HIGH_PRIORITY_CLASS,
		/// Process whose threads run only when the system is idle<br/>
		///    and are preempted by the threads of any process running in a higher priority class.<br/>
		///    The idle priority class is inherited by child processes.<br/>
		EP_PROCESS_PRIORITY_IDLE_PRIORITY_CLASS=IDLE_PRIORITY_CLASS,
		/// Process with no special scheduling needs.
		EP_PROCESS_PRIORITY_NORMAL_PRIORITY_CLASS=NORMAL_PRIORITY_CLASS ,
		/// Process that has the highest possible priority.<br/>
		///    The threads of a real-time priority class process preempt the threads of all other processes,<br/>
		///    including operating system processes performing important tasks.<br/>
		///    For example, a real-time process that executes for more than a very brief interval<br/>
		///    can cause disk caches not to flush or cause the mouse to be unresponsive.
		EP_PROCESS_PRIORITY_REALTIME_PRIORITY_CLASS=REALTIME_PRIORITY_CLASS,

		/// Returns when GetProcessPriority fails
		EP_PROCESS_PRIORITY_ERROR_RETURN=0,

#if (_MSC_VER >=MSVC90) && (WINVER>=WINDOWS_VISTA) // Only for VS2008 and above and Windows Vista and above
		/// Begin background processing mode.<br/>
		///    The system lowers the resource scheduling priorities of the process (and its threads)<br/>
		///    so that it can perform background work without significantly affecting activity in the foreground.<br/>
		///    This value can be specified only if hProcess is a handle to the current process.<br/>
		///    The function fails if the process is already in background processing mode.<br/>
		///    Windows Server 2003 and Windows XP:  This value is not supported.
		EP_PROCESS_PRIORITY_MODE_BACKGROUND_BEGIN=PROCESS_MODE_BACKGROUND_BEGIN,
		/// End background processing mode.<br/>
		///    The system restores the resource scheduling priorities of the process (and its threads)<br/>
		///    as they were before the process entered background processing mode.<br/>
		///    This value can be specified only if hProcess is a handle to the current process. <br/>
		///    The function fails if the process is not in background processing mode.<br/>
		///    Windows Server 2003 and Windows XP:  This value is not supported.
		EP_PROCESS_PRIORITY_MODE_BACKGROUND_END=PROCESS_MODE_BACKGROUND_END,
#endif //(_MSC_VER >=MSVC90) && (WINVER>=WINDOWS_VISTA)
	}ProcessPriority;

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
		Return the set of process id with given handle
		@param[in] processHandle the process handle to find id
		@return process ID of the given handle
		*/
		static unsigned long GetProcessID(HANDLE processHandle);

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
		Terminate the process with the given process Handle
		@param[in] processHandle the handle of the window process to terminate
		@return true if terminated, otherwise false
		*/
		static bool TerminateProcess(HANDLE processHandle);

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
		Switch the focus to the process with the given process Handle
		@param[in] processHandle the handle of the window process to switch
		@return true if switched, otherwise false
		*/
		static bool SwitchToProcess(HANDLE processHandle);
		
		/*!
		Get the number of process with the given name
		@param[in] pProcessName the name of the window process
		@return the number of process with the given name
		*/
		static size_t GetNumberOfProcess(const TCHAR * pProcessName);

		/*!
		Get the process handle by process name and input as parameter for function Func
		@param[in] pProcessName the name of the window process
		@param[in] Func the function pointer to get the found handle as its parameter
		*/
		static void CommandOnProcess(const TCHAR *pProcessName, void (__cdecl *Func)(HANDLE) );

		/*!
		Return the current Process Priority.
		@param[in] processHandle the process handle to get the priority
		@return the current Process Priority.
		@remark if the function fails, the return value is PROCESS_PRIORITY_ERROR_RETURN.
		*/
		static ProcessPriority GetPriority(HANDLE processHandle);

		/*!
		Set Priority of the thread
		@param[in] processHandle the process handle to set the priority
		@param[in] priority The priority of the process
		@return true if successfully set otherwise false
		*/
		static bool SetPriority(HANDLE processHandle, ProcessPriority priority);

		/*!
		Return whether current process is running under 64bit Windows
		@return true if running under 64bit Windows otherwise false
		*/
		static bool IsWow64();
	};
}
#endif //__EP_WIN_PROCESS_HELPER_H__