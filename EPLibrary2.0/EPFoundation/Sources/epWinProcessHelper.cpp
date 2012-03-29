/*! 
WinProcessHelper for the EpLibrary
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
#include "epWinProcessHelper.h"
#include <comdef.h>
#include <tchar.h>
using namespace epl;

#pragma warning(disable: 4996)

PPERF_OBJECT_TYPE FirstObject( PPERF_DATA_BLOCK PerfData )
{
    return( (PPERF_OBJECT_TYPE)((PBYTE)PerfData + 
        PerfData->HeaderLength) );
}

PPERF_OBJECT_TYPE NextObject( PPERF_OBJECT_TYPE PerfObj )
{
    return( (PPERF_OBJECT_TYPE)((PBYTE)PerfObj + 
        PerfObj->TotalByteLength) );
}

PPERF_INSTANCE_DEFINITION FirstInstance( PPERF_OBJECT_TYPE PerfObj )
{
    return( (PPERF_INSTANCE_DEFINITION)((PBYTE)PerfObj + 
        PerfObj->DefinitionLength) );
}

PPERF_INSTANCE_DEFINITION NextInstance( 
    PPERF_INSTANCE_DEFINITION PerfInst )
{
    PPERF_COUNTER_BLOCK PerfCntrBlk;

    PerfCntrBlk = (PPERF_COUNTER_BLOCK)((PBYTE)PerfInst + 
        PerfInst->ByteLength);

    return( (PPERF_INSTANCE_DEFINITION)((PBYTE)PerfCntrBlk + 
        PerfCntrBlk->ByteLength) );
}

PPERF_COUNTER_DEFINITION FirstCounter( PPERF_OBJECT_TYPE PerfObj )
{
    return( (PPERF_COUNTER_DEFINITION) ((PBYTE)PerfObj + 
        PerfObj->HeaderLength) );
}

PPERF_COUNTER_DEFINITION NextCounter( 
    PPERF_COUNTER_DEFINITION PerfCntr )
{
    return( (PPERF_COUNTER_DEFINITION)((PBYTE)PerfCntr + 
        PerfCntr->ByteLength) );
}


PPERF_COUNTER_BLOCK CounterBlock(PPERF_INSTANCE_DEFINITION PerfInst)
{
	return (PPERF_COUNTER_BLOCK) ((LPBYTE) PerfInst + PerfInst->ByteLength);
}

#define TOTALBYTES    64*1024
#define BYTEINCREMENT 1024

#define PROCESS_OBJECT_INDEX	230
#define PROC_ID_COUNTER			784


void WinProcessHelper::GetProcessID(LPCTSTR pProcessName, std::vector<DWORD>& retSetOfPID)
{
    PPERF_DATA_BLOCK pPerfData = NULL;
    PPERF_OBJECT_TYPE pPerfObj;
    PPERF_INSTANCE_DEFINITION pPerfInst;
    PPERF_COUNTER_DEFINITION pPerfCntr, pCurCntr;
    PPERF_COUNTER_BLOCK PtrToCntr;
    DWORD BufferSize = TOTALBYTES;
    DWORD i, j;
	LONG k;

// Allocate the buffer for the performance data.

    pPerfData = (PPERF_DATA_BLOCK) malloc( BufferSize );

	TCHAR szKey[32];
	_stprintf(szKey,_T("%d %d"),PROCESS_OBJECT_INDEX, PROC_ID_COUNTER);
	LONG lRes;
    while( (lRes = RegQueryValueEx( HKEY_PERFORMANCE_DATA,
                               szKey,
                               NULL,
                               NULL,
                               (LPBYTE) pPerfData,
                               &BufferSize )) == ERROR_MORE_DATA )
    {
		// Get a buffer that is big enough.

        BufferSize += BYTEINCREMENT;
        pPerfData = (PPERF_DATA_BLOCK) realloc( pPerfData, BufferSize );
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
			bstrProcessName = (wchar_t *)((PBYTE)pPerfInst + pPerfInst->NameOffset);
			if (!_tcscmp((LPCTSTR)bstrProcessName, (LPCTSTR) bstrInput))
			{
			
				// Retrieve all counters.

				for( j=0; j < pPerfObj->NumCounters; j++ )
				{
					if (pCurCntr->CounterNameTitleIndex == PROC_ID_COUNTER)
					{
						PtrToCntr = CounterBlock(pPerfInst);
						DWORD *pdwValue = (DWORD*)((LPBYTE) PtrToCntr + pCurCntr->CounterOffset);
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
bool WinProcessHelper::TerminateProcess(DWORD processID)
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

bool WinProcessHelper::SwitchToProcess(LPCTSTR pProcessName)
{
	std::vector<DWORD> setOfPID;
	epl::WinProcessHelper::GetProcessID(pProcessName,setOfPID);
	if(!setOfPID.empty())
	{
		HWND hwnd= ::GetTopWindow(0);

		while(hwnd)
		{
			DWORD pid;
			DWORD dwThreadID= ::GetWindowThreadProcessId(hwnd,&pid);
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
bool WinProcessHelper::SwitchToProcess(DWORD processID)
{
	HWND hwnd= ::GetTopWindow(0);

	while(hwnd)
	{
		DWORD pid;
		DWORD dwThreadID= ::GetWindowThreadProcessId(hwnd,&pid);
		if(pid==processID)
		{
			::SwitchToThisWindow(hwnd,TRUE);
			return true;
		}
		hwnd=::GetNextWindow(hwnd,GW_HWNDNEXT);
	}
	return false;
}

bool WinProcessHelper::TerminateProcess(LPCTSTR pProcessName)
{
	std::vector<DWORD> setOfPid;
	epl::WinProcessHelper::GetProcessID(pProcessName,setOfPid);
	if(!setOfPid.empty())
	{
		for(int i=0; i<(int)setOfPid.size(); i++)
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

unsigned int WinProcessHelper::GetNumberOfProcess(LPCTSTR pProcessName)
{
	std::vector<DWORD> setOfPid;
	epl::WinProcessHelper::GetProcessID(pProcessName,setOfPid);
	return setOfPid.size();
}