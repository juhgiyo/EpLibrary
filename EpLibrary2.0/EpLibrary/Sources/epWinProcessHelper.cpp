/*! 
WinProcessHelper for the EpLibrary

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
*/
#include "epWinProcessHelper.h"
#include <WinPerf.h>
#include <comdef.h>
#include <tchar.h>
#include <tlhelp32.h>
#include "epMemory.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;

#pragma warning(disable: 4996)

PPERF_OBJECT_TYPE FirstObject( PPERF_DATA_BLOCK PerfData )
{
    return reinterpret_cast<PPERF_OBJECT_TYPE>((PBYTE)PerfData + 
        PerfData->HeaderLength);
}

PPERF_OBJECT_TYPE NextObject( PPERF_OBJECT_TYPE PerfObj )
{
    return reinterpret_cast<PPERF_OBJECT_TYPE>((PBYTE)PerfObj + 
        PerfObj->TotalByteLength);
}

PPERF_INSTANCE_DEFINITION FirstInstance( PPERF_OBJECT_TYPE PerfObj )
{
    return reinterpret_cast<PPERF_INSTANCE_DEFINITION>((PBYTE)PerfObj + 
        PerfObj->DefinitionLength);
}

PPERF_INSTANCE_DEFINITION NextInstance( 
    PPERF_INSTANCE_DEFINITION PerfInst )
{
    PPERF_COUNTER_BLOCK PerfCntrBlk;

    PerfCntrBlk = reinterpret_cast<PPERF_COUNTER_BLOCK>((PBYTE)PerfInst + 
        PerfInst->ByteLength);

    return reinterpret_cast<PPERF_INSTANCE_DEFINITION>((PBYTE)PerfCntrBlk + 
        PerfCntrBlk->ByteLength);
}

PPERF_COUNTER_DEFINITION FirstCounter( PPERF_OBJECT_TYPE PerfObj )
{
    return reinterpret_cast<PPERF_COUNTER_DEFINITION>((PBYTE)PerfObj + 
        PerfObj->HeaderLength);
}

PPERF_COUNTER_DEFINITION NextCounter( 
    PPERF_COUNTER_DEFINITION PerfCntr )
{
    return reinterpret_cast<PPERF_COUNTER_DEFINITION>((PBYTE)PerfCntr + 
        PerfCntr->ByteLength);
}


PPERF_COUNTER_BLOCK CounterBlock(PPERF_INSTANCE_DEFINITION PerfInst)
{
	return reinterpret_cast<PPERF_COUNTER_BLOCK>((LPBYTE) PerfInst + PerfInst->ByteLength);
}

#define TOTALBYTES    64*1024
#define BYTEINCREMENT 1024

#define PROCESS_OBJECT_INDEX	230
#define PROC_ID_COUNTER			784


void WinProcessHelper::GetProcessID(const TCHAR *pProcessName, std::vector<unsigned long>& retSetOfPID)
{
    PPERF_DATA_BLOCK pPerfData = NULL;
    PPERF_OBJECT_TYPE pPerfObj;
    PPERF_INSTANCE_DEFINITION pPerfInst;
    PPERF_COUNTER_DEFINITION pPerfCntr, pCurCntr;
    PPERF_COUNTER_BLOCK PtrToCntr;
    unsigned long BufferSize = TOTALBYTES;
    unsigned long i, j;
	LONG k;

// Allocate the buffer for the performance data.

    pPerfData = reinterpret_cast<PPERF_DATA_BLOCK>(EP_Malloc( BufferSize ));

	TCHAR szKey[32];
	_stprintf(szKey,_T("%d %d"),PROCESS_OBJECT_INDEX, PROC_ID_COUNTER);
	LONG lRes;
    while( (lRes = RegQueryValueEx( HKEY_PERFORMANCE_DATA,
                               szKey,
                               NULL,
                               NULL,
                               reinterpret_cast<LPBYTE>(pPerfData),
                               &BufferSize )) == ERROR_MORE_DATA )
    {
		// Get a buffer that is big enough.

        BufferSize += BYTEINCREMENT;
        pPerfData = reinterpret_cast<PPERF_DATA_BLOCK>(EP_Realloc( pPerfData, BufferSize ));
    }

	// Get the first object type.

    pPerfObj = FirstObject( pPerfData );

	// Process all objects.

    for( i=0; i < pPerfData->NumObjectTypes; i++ )
    {

		if (pPerfObj->ObjectNameTitleIndex != PROCESS_OBJECT_INDEX)
		{
			pPerfObj = NextObject( pPerfObj );
			continue;
		}
    
		retSetOfPID.clear();

		// Get the first counter.

        pPerfCntr = FirstCounter( pPerfObj );

		// Get the first instance.

        pPerfInst = FirstInstance( pPerfObj );

		_bstr_t bstrProcessName,bstrInput;

		// Retrieve all instances.

        for( k=0; k < pPerfObj->NumInstances; k++ )
        {
			pCurCntr = pPerfCntr;
			bstrInput = pProcessName;
			bstrProcessName = reinterpret_cast<wchar_t *>(reinterpret_cast<PBYTE>(pPerfInst) + pPerfInst->NameOffset);
			if (!_tcscmp(static_cast<const TCHAR *>(bstrProcessName), static_cast<const TCHAR *>(bstrInput)))
			{
			
				// Retrieve all counters.

				for( j=0; j < pPerfObj->NumCounters; j++ )
				{
					if (pCurCntr->CounterNameTitleIndex == PROC_ID_COUNTER)
					{
						PtrToCntr = CounterBlock(pPerfInst);
						unsigned long *pdwValue = reinterpret_cast<unsigned long*>(reinterpret_cast<LPBYTE>(PtrToCntr) + pCurCntr->CounterOffset);
						retSetOfPID.push_back(*pdwValue);
						break;
					}


					// Get the next counter.

					pCurCntr = NextCounter( pCurCntr );
				}
			}

			// Get the next instance.

            pPerfInst = NextInstance( pPerfInst );
        }
    }
	EP_Free(pPerfData);
	RegCloseKey(HKEY_PERFORMANCE_DATA);
}

unsigned long WinProcessHelper::GetProcessID(HANDLE processHandle)
{
	return ::GetProcessId(processHandle);
}

bool WinProcessHelper::TerminateProcess(unsigned long processID)
{
	HANDLE pProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,processID);
	if(pProcess)
	{
		::TerminateProcess(pProcess,0);
		::CloseHandle(pProcess);
		return true;
	}
	return false;
}

bool WinProcessHelper::TerminateProcess(HANDLE processHandle)
{
	if(processHandle)
	{
		::TerminateProcess(processHandle,0);
		return true;
	}
	return false;
}

bool WinProcessHelper::SwitchToProcess(const TCHAR * pProcessName)
{
	std::vector<unsigned long> setOfPID;
	epl::WinProcessHelper::GetProcessID(pProcessName,setOfPID);
	if(!setOfPID.empty())
	{
		HWND hwnd= ::GetTopWindow(0);

		while(hwnd)
		{
			unsigned long pid;
			unsigned long dwThreadID= ::GetWindowThreadProcessId(hwnd,&pid);
			if(pid==setOfPID[0])
			{
				::SwitchToThisWindow(hwnd,TRUE);
				return true;
			}
			hwnd=::GetNextWindow(hwnd,GW_HWNDNEXT);
		}
	}
	return false;
}
bool WinProcessHelper::SwitchToProcess(unsigned long processID)
{
	HWND hwnd= ::GetTopWindow(0);

	while(hwnd)
	{
		unsigned long pid;
		unsigned long dwThreadID= ::GetWindowThreadProcessId(hwnd,&pid);
		if(pid==processID)
		{
			::SwitchToThisWindow(hwnd,TRUE);
			return true;
		}
		hwnd=::GetNextWindow(hwnd,GW_HWNDNEXT);
	}
	return false;
}

bool WinProcessHelper::SwitchToProcess(HANDLE processHandle)
{
	HWND hwnd= ::GetTopWindow(0);

	unsigned long processID=GetProcessID(processHandle);
	while(hwnd)
	{
		unsigned long pid;
		unsigned long dwThreadID= ::GetWindowThreadProcessId(hwnd,&pid);
		if(pid==processID)
		{
			::SwitchToThisWindow(hwnd,TRUE);
			return true;
		}
		hwnd=::GetNextWindow(hwnd,GW_HWNDNEXT);
	}
	return false;
}

bool WinProcessHelper::TerminateProcess(const TCHAR *pProcessName)
{
	std::vector<unsigned long> setOfPid;
	epl::WinProcessHelper::GetProcessID(pProcessName,setOfPid);
	if(!setOfPid.empty())
	{
		for(unsigned int i=0; i<setOfPid.size(); i++)
		{
			HANDLE pProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,setOfPid[i]);
			::TerminateProcess(pProcess,0);
			::CloseHandle(pProcess);
		}
	}
	else
	{
		return false;
	}
	return true;
}

size_t WinProcessHelper::GetNumberOfProcess(const TCHAR * pProcessName)
{
	std::vector<unsigned long> setOfPid;
	epl::WinProcessHelper::GetProcessID(pProcessName,setOfPid);
	return setOfPid.size();
}


void WinProcessHelper::CommandOnProcess(const TCHAR *pProcessName, void (__cdecl *Func)(HANDLE) )
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			if (_tcsicmp(entry.szExeFile, pProcessName) == 0)
			{  
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);

				Func(hProcess);

				CloseHandle(hProcess);
			}
		}
	}

	CloseHandle(snapshot);
}

ProcessPriority WinProcessHelper::GetPriority(HANDLE processHandle)
{
	ProcessPriority ret=EP_PROCESS_PRIORITY_ERROR_RETURN;
	if(processHandle!=0)
		ret=(ProcessPriority)::GetPriorityClass(processHandle);
	return ret;
}
bool WinProcessHelper::SetPriority(HANDLE processHandle, ProcessPriority priority)
{
	bool ret=false;
	if(processHandle!=0)
	{
		ret=(bool)::SetPriorityClass(processHandle,(int)priority);
	}
	return ret;
}


typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

LPFN_ISWOW64PROCESS fnIsWow64Process;

bool WinProcessHelper::IsWow64()
{
	BOOL bIsWow64 = FALSE;

	//IsWow64Process is not available on all supported versions of Windows.
	//Use GetModuleHandle to get a handle to the DLL that contains the function
	//and GetProcAddress to get a pointer to the function if available.

	fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
		GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

	if(NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
		{
			//handle error
			EP_ASSERT(0);
		}
	}
	return (bool)bIsWow64;
}