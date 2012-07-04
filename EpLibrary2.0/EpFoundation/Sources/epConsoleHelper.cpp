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


EpTString ConsoleHelper::ExecuteConsoleCommand(const TCHAR * command, bool isDosCommand, bool isWaitForTerminate, ConsolePriority priority)
{
	EpTString csExecute;
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

	HANDLE rPipe, wPipe;

	if(!CreatePipe(&rPipe,&wPipe,&secattr,0))
	{
		return _T("[Error]ConsoleHelper::ExecuteConsoleCommand:Creating Pipe");
	}

	STARTUPINFO sInfo;
	ZeroMemory(&sInfo, sizeof(sInfo));
	PROCESS_INFORMATION pInfo;
	ZeroMemory(&pInfo,sizeof(pInfo));
	sInfo.cb=sizeof(sInfo);
	sInfo.dwFlags=STARTF_USESTDHANDLES;
	//sInfo.dwFlags=STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;

	sInfo.wShowWindow=SW_HIDE;
	sInfo.hStdInput=NULL;
	sInfo.hStdOutput=wPipe;
	sInfo.hStdError=wPipe;
#if defined(_UNICODE) || defined(UNICODE)
	if(!CreateProcess(0,reinterpret_cast<LPWSTR>(const_cast<wchar_t*>(csExecute.c_str())),0,0,TRUE,priority|CREATE_NO_WINDOW,0,0,&sInfo,&pInfo))
	//if(!CreateProcess(0,reinterpret_cast<LPWSTR>(const_cast<wchar_t*>(csExecute.c_str())),0,0,TRUE,priority|CREATE_NEW_CONSOLE,0,0,&sInfo,&pInfo))
#else // defined(_UNICODE) || defined(UNICODE)
	if(!CreateProcess(0,reinterpret_cast<LPSTR>(const_cast<char*>(csExecute.c_str())),0,0,TRUE,priority|CREATE_NO_WINDOW,0,0,&sInfo,&pInfo))
	//if(!CreateProcess(0,reinterpret_cast<LPSTR>(const_cast<char*>(csExecute.c_str())),0,0,TRUE,priority|CREATE_NEW_CONSOLE,0,0,&sInfo,&pInfo))
#endif // defined(_UNICODE) || defined(UNICODE)
	{
		return _T("[Error]ConsoleHelper::ExecuteConsoleCommand:Creating Process");
	}
	if(isWaitForTerminate)
	{
		WaitForSingleObject(pInfo.hProcess,INFINITE);
	}

	CloseHandle(wPipe);

	char buf[100];
	unsigned long reDword;
	EpTString m_csOutput,csTemp;
	m_csOutput=_T("");
	long res;
	unsigned long dwAvail;
	if(PeekNamedPipe(rPipe, NULL, 0, NULL, &dwAvail, NULL) && dwAvail>0)
	{
		do
		{
			memset(buf,0,sizeof(char)*100);
			res=::ReadFile(rPipe,buf,100,&reDword,0);
#if defined(_UNICODE) || defined(UNICODE)
			csTemp=System::MultiByteToWideChar(buf,reDword);
			m_csOutput+=csTemp;
#else // defined(_UNICODE) || defined(UNICODE)
			csTemp=buf;
			m_csOutput+=csTemp.substr(0,reDword);
#endif // defined(_UNICODE) || defined(UNICODE)
		}while(res);

	}
	
	CloseHandle(pInfo.hProcess);
	CloseHandle(pInfo.hThread);
	return m_csOutput;

}

void ConsoleHelper::ExecuteProgram(const TCHAR * execFilePath)
{
	ShellExecute(0,0,execFilePath,0,0,SW_SHOWNORMAL);
}