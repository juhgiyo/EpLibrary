/*! 
ConsoleHelper for the EpLibrary

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
*/
#include "epConsoleHelper.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

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

EpTString ConsoleHelper::ExecuteConsoleCommand(const TCHAR * command, bool isDosCommand, bool isWaitForTerminate,bool isShowWindow, bool useStdOutput, ConsolePriority priority, HANDLE *retProcessHandle)
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

	if(isWaitForTerminate && useStdOutput)
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
		
	if(isWaitForTerminate && useStdOutput)
	{
		sInfo.dwFlags=STARTF_USESTDHANDLES;
		sInfo.hStdInput=NULL;
		sInfo.hStdOutput=hOutputWrite;
		sInfo.hStdError=hErrorWrite;
	}

	DWORD creationFlag=priority;
	if(isShowWindow)
	{
		sInfo.wShowWindow=SW_SHOWNORMAL;
		sInfo.dwFlags|=STARTF_USESHOWWINDOW;
		creationFlag|=CREATE_NEW_CONSOLE;
	}
	else
	{
		sInfo.wShowWindow=SW_HIDE;
		creationFlag|=CREATE_NO_WINDOW;
	}
	

#if defined(_UNICODE) || defined(UNICODE)
	if(!CreateProcess(0,reinterpret_cast<LPWSTR>(const_cast<wchar_t*>(csExecute.c_str())),0,0,TRUE,creationFlag,0,0,&sInfo,&pInfo))
		//if(!CreateProcess(0,reinterpret_cast<LPWSTR>(const_cast<wchar_t*>(csExecute.c_str())),0,0,TRUE,priority|CREATE_NEW_CONSOLE,0,0,&sInfo,&pInfo))
#else // defined(_UNICODE) || defined(UNICODE)
	if(!CreateProcess(0,reinterpret_cast<LPSTR>(const_cast<char*>(csExecute.c_str())),0,0,TRUE,creationFlag,0,0,&sInfo,&pInfo))
		//if(!CreateProcess(0,reinterpret_cast<LPSTR>(const_cast<char*>(csExecute.c_str())),0,0,TRUE,priority|CREATE_NEW_CONSOLE,0,0,&sInfo,&pInfo))
#endif // defined(_UNICODE) || defined(UNICODE)
	{
		if(isWaitForTerminate && useStdOutput)
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
		if(useStdOutput)
		{
			CloseHandle(hOutputWrite);
			CloseHandle(hErrorWrite);
			csOutput=ReadAndHandleOutput(hOutputRead);
		}
		System::WaitForSingleObject(pInfo.hProcess,WAITTIME_INIFINITE);
		if(useStdOutput)
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

void ConsoleHelper::ExecuteProgram(const TCHAR * execFilePath, const TCHAR *parameters)
{
	ShellExecute(0,0,execFilePath,parameters,0,SW_SHOWNORMAL);
}