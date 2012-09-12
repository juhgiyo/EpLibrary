/*! 
ConsoleHelper for the EpLibrary
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
*/
#include "epConsoleHelper.h"

using namespace epl;

EpTString ReadAndHandleOutput(HANDLE hPipeRead)
{
	CHAR lpBuffer[256];
	DWORD nBytesRead;
	EpTString csOutput,csTemp;
	csOutput=_T("");
	while(TRUE)
	{
		if (!ReadFile(hPipeRead,lpBuffer,sizeof(lpBuffer),
			&nBytesRead,NULL) || !nBytesRead)
		{
			if (GetLastError() == ERROR_BROKEN_PIPE)
				break; // pipe done - normal exit path.
			else
			{
				csOutput+=_T("\n\n[Error]ConsoleHelper::ExecuteConsoleCommand:ReadFile Error\n\n"); // Something bad happened.
				break;
			}
		}

		// Display the character read on the screen.
#if defined(_UNICODE) || defined(UNICODE)
		csTemp=System::MultiByteToWideChar(lpBuffer,nBytesRead);
		csOutput+=csTemp;
#else // defined(_UNICODE) || defined(UNICODE)
		csTemp=lpBuffer;
		csOutput+=csTemp.substr(0,nBytesRead);
#endif // defined(_UNICODE) || defined(UNICODE)
	}
	return csOutput;
}

EpTString ConsoleHelper::ExecuteConsoleCommand(const TCHAR * command, bool isDosCommand, bool isWaitForTerminate, ConsolePriority priority, HANDLE *retProcessHandle)
{
	EpTString csExecute,csOutput;
	csOutput=_T("");
	if(isDosCommand)
	{
		csExecute=_T("cmd /c ");
		csExecute.append(command);
	}
	else
		csExecute=command;

	SECURITY_ATTRIBUTES secattr;
	ZeroMemory(&secattr,sizeof(secattr));
	secattr.nLength=sizeof(secattr);
	secattr.bInheritHandle=TRUE;

	HANDLE hOutputReadTmp,hOutputRead,hOutputWrite;
	HANDLE hErrorWrite;

	if(isWaitForTerminate)
	{

		if(!CreatePipe(&hOutputReadTmp,&hOutputWrite,&secattr,0))
		{
			return _T("[Error]ConsoleHelper::ExecuteConsoleCommand:Creating Pipe (hOutputWrite)");

		}

		if (!DuplicateHandle(GetCurrentProcess(),hOutputWrite,GetCurrentProcess(),&hErrorWrite,0, TRUE,DUPLICATE_SAME_ACCESS))
		{
			CloseHandle(hOutputReadTmp);
			CloseHandle(hOutputWrite);
			return _T("[Error]ConsoleHelper::ExecuteConsoleCommand:Duplicate Handle (hErrorWrite)");
		}
		if (!DuplicateHandle(GetCurrentProcess(),hOutputReadTmp,GetCurrentProcess(),&hOutputRead,0,FALSE,DUPLICATE_SAME_ACCESS))
		{
			CloseHandle(hOutputWrite);
			CloseHandle(hErrorWrite);
			CloseHandle(hOutputReadTmp);
			return _T("[Error]ConsoleHelper::ExecuteConsoleCommand:Duplicate Handle (hOutputRead)");
		}
		CloseHandle(hOutputReadTmp);
	}

	STARTUPINFO sInfo;
	ZeroMemory(&sInfo, sizeof(sInfo));
	PROCESS_INFORMATION pInfo;
	ZeroMemory(&pInfo,sizeof(pInfo));
	sInfo.cb=sizeof(sInfo);
	sInfo.wShowWindow=SW_HIDE;
	if(isWaitForTerminate)
	{
		sInfo.dwFlags=STARTF_USESTDHANDLES;
		sInfo.hStdInput=NULL;
		sInfo.hStdOutput=hOutputWrite;
		sInfo.hStdError=hErrorWrite;
	}
#if defined(_UNICODE) || defined(UNICODE)
	if(!CreateProcess(0,reinterpret_cast<LPWSTR>(const_cast<wchar_t*>(csExecute.c_str())),0,0,TRUE,priority|CREATE_NO_WINDOW,0,0,&sInfo,&pInfo))
		//if(!CreateProcess(0,reinterpret_cast<LPWSTR>(const_cast<wchar_t*>(csExecute.c_str())),0,0,TRUE,priority|CREATE_NEW_CONSOLE,0,0,&sInfo,&pInfo))
#else // defined(_UNICODE) || defined(UNICODE)
	if(!CreateProcess(0,reinterpret_cast<LPSTR>(const_cast<char*>(csExecute.c_str())),0,0,TRUE,priority|CREATE_NO_WINDOW,0,0,&sInfo,&pInfo))
		//if(!CreateProcess(0,reinterpret_cast<LPSTR>(const_cast<char*>(csExecute.c_str())),0,0,TRUE,priority|CREATE_NEW_CONSOLE,0,0,&sInfo,&pInfo))
#endif // defined(_UNICODE) || defined(UNICODE)
	{
		if(isWaitForTerminate)
		{
			CloseHandle(hOutputWrite);
			CloseHandle(hErrorWrite);
			CloseHandle(hOutputRead);
		}
		return _T("[Error]ConsoleHelper::ExecuteConsoleCommand:Creating Process");
	}
	if(retProcessHandle)
	{
		*retProcessHandle=pInfo.hProcess;
	}

	if(isWaitForTerminate)
	{
		CloseHandle(hOutputWrite);
		CloseHandle(hErrorWrite);

		csOutput=ReadAndHandleOutput(hOutputRead);
		System::WaitForSingleObject(pInfo.hProcess,WAITTIME_INIFINITE);
		CloseHandle(hOutputRead);
	}
	if(retProcessHandle)
	{
		*retProcessHandle=0;
	}
	CloseHandle(pInfo.hProcess);
	CloseHandle(pInfo.hThread);

	return csOutput;


}

void ConsoleHelper::ExecuteProgram(const TCHAR * execFilePath)
{
	ShellExecute(0,0,execFilePath,0,0,SW_SHOWNORMAL);
}