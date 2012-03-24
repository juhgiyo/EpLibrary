/*! 
RegistryHelper for the EpLibrary
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
#include "epRegistryHelper.h"
#include "epMemory.h"
#include <Shlwapi.h>

using namespace epl;

bool RegistryHelper::SetRegistryData(HKEY key,CString subKey,CString regName,CString regData)
{
	if(SHSetValue(key,subKey.GetString(),regName.GetString(),REG_SZ,regData.GetString(),(regData.GetLength()+1)*sizeof(TCHAR))==ERROR_SUCCESS)
		return true;
	return false;
}

bool RegistryHelper::SetRegistryData(HKEY key,CString subKey,CString regName,DWORD regType,void * regData,unsigned int sizeInByte)
{
	if(SHSetValue(key,subKey.GetString(),regName.GetString(),regType,regData,sizeInByte)==ERROR_SUCCESS)
		return true;
	return false;
}

bool RegistryHelper::GetRegistryData(HKEY key,CString subKey,CString regName, CString &retString)
{
	DWORD size;
	retString=_T("");
	SHGetValue(key,subKey.GetString(),regName.GetString(),NULL,NULL,&size);
	if(size)
	{
		TCHAR *buf=EP_NEW TCHAR[size];
		if(SHGetValue(key,subKey.GetString(),regName.GetString(),NULL,buf,&size)==ERROR_SUCCESS)
		{
			retString=buf;
			EP_DELETE[] buf;
			return true;
		}
		EP_DELETE[] buf;
	}
	return false;
}

bool RegistryHelper::GetRegistryData(HKEY key,CString subKey,CString regName,unsigned int sizeInByte,void *retBuf, unsigned int &retSizeReadInByte, DWORD &retRegType)
{
	DWORD size=sizeInByte;
	if(SHGetValue(key,subKey.GetString(),regName.GetString(),&retRegType,retBuf,&size)==ERROR_SUCCESS)
	{
		retSizeReadInByte=size;
		return true;
	}
	return false;

}

unsigned int RegistryHelper::GetRegistryDataSize(HKEY key,CString subKey,CString regName)
{
	unsigned long size;
	SHGetValue(key,subKey.GetString(),regName.GetString(),NULL,NULL,&size);
	return size;
}


void RegistryHelper::DeleteRegistryValue(HKEY key, LPCWSTR subkey, LPCWSTR regValue)
{
	SHDeleteValue(key,subkey,regValue);
}
void RegistryHelper::DeleteRegistryKey(HKEY key, LPCWSTR subkey)
{
	SHDeleteKey(key,subkey);
}

HKEY RegistryHelper::GetRegistryMode(LPCWSTR strRegPath)
{
	if(_tcsstr(strRegPath,_T("HKEY_CLASSES_ROOT"))!=NULL)
	{
		return HKEY_CLASSES_ROOT;
	}
	else if(_tcsstr(strRegPath,_T("HKEY_CURRENT_USER"))!=NULL)
	{
		return HKEY_CLASSES_ROOT;
	}
	else if(_tcsstr(strRegPath,_T("HKEY_LOCAL_MACHINE"))!=NULL)
	{
		return HKEY_CLASSES_ROOT;
	}
	else if(_tcsstr(strRegPath,_T("HKEY_USERS"))!=NULL)
	{
		return HKEY_CLASSES_ROOT;
	}
	return 0;
}