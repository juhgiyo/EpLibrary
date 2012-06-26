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


WaitTimeStruct _waitTimeStruct::wtsDefault=WaitTimeStruct();

EpTString ConsoleHelper::ExecuteConsoleCommand(const TCHAR * command, bool isWaitForTerminate, int priority, WaitTimeStruct waitStruct)
{
	EpTString csExecute;
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
	sInfo.wShowWindow=SW_HIDE;
	sInfo.hStdInput=NULL;
	sInfo.hStdOutput=wPipe;
	sInfo.hStdError=wPipe;
#if defined(_UNICODE) || defined(UNICODE)
	if(!CreateProcess(0,reinterpret_cast<LPWSTR>(const_cast<wchar_t*>(csExecute.c_str())),0,0,TRUE,priority|CREATE_NO_WINDOW,0,0,&sInfo,&pInfo))
#else // defined(_UNICODE) || defined(UNICODE)
	if(!CreateProcess(0,reinterpret_cast<LPSTR>(const_cast<char*>(csExecute.c_str())),0,0,TRUE,priority|CREATE_NO_WINDOW,0,0,&sInfo,&pInfo))
#endif // defined(_UNICODE) || defined(UNICODE)
	{
		return _T("[Error]ConsoleHelper::ExecuteConsoleCommand:Creating Process");
	}
	if(isWaitForTerminate)
	{
		//WaitForSingleObject(pInfo.hProcess,INFINITE);

		// Below implementation meant to do same functionality as above function ( WaitForSingleObject(pInfo.hProcess,INFINITE) )
		// If wait for infinite like above, the child process get performance degradation, and sometimes child process gets hanged.
		// Below implementation solves the problem, however need to set relevant time for different environment. 
		unsigned long ret;
		do {     
			Sleep(waitStruct.sleepTimeMilliSec);     
			ret = WaitForSingleObject(pInfo.hProcess, waitStruct.waitTimeMilliSec); 
		} 
		while (ret == WAIT_TIMEOUT); 
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