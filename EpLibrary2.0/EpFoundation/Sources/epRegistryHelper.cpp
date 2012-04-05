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

bool RegistryHelper::SetRegistryData(HKEY key,const TCHAR * subKey,const TCHAR * regName,const TCHAR * regData)
{
	if(SHSetValue(key,subKey,regName,REG_SZ,regData,(System::TcsLen(regData)+1)*sizeof(TCHAR))==ERROR_SUCCESS)
		return true;
	return false;
}

bool RegistryHelper::SetRegistryData(HKEY key,const TCHAR *subKey,const TCHAR * regName,unsigned long regType,void * regData,unsigned int sizeInByte)
{
	if(SHSetValue(key,subKey,regName,regType,regData,sizeInByte)==ERROR_SUCCESS)
		return true;
	return false;
}

bool RegistryHelper::GetRegistryData(HKEY key,const TCHAR *subKey,const TCHAR * regName, EpTString &retString)
{
	unsigned long size;
	retString=_T("");
	SHGetValue(key,subKey,regName,NULL,NULL,&size);
	if(size)
	{
		TCHAR *buf=EP_NEW TCHAR[size];
		if(SHGetValue(key,subKey,regName,NULL,buf,&size)==ERROR_SUCCESS)
		{
			retString=buf;
			EP_DELETE[] buf;
			return true;
		}
		EP_DELETE[] buf;
	}
	return false;
}

bool RegistryHelper::GetRegistryData(HKEY key,const TCHAR *subKey,const TCHAR *regName,unsigned int sizeInByte,void *retBuf, unsigned int &retSizeReadInByte, unsigned long &retRegType)
{
	unsigned long size=sizeInByte;
	if(SHGetValue(key,subKey,regName,&retRegType,retBuf,&size)==ERROR_SUCCESS)
	{
		retSizeReadInByte=size;
		return true;
	}
	return false;

}

unsigned int RegistryHelper::GetRegistryDataSize(HKEY key,const TCHAR *subKey,const TCHAR * regName)
{
	unsigned long size;
	SHGetValue(key,subKey,regName,NULL,NULL,&size);
	return size;
}


void RegistryHelper::DeleteRegistryValue(HKEY key, const TCHAR * subkey, const TCHAR * regValue)
{
	SHDeleteValue(key,subkey,regValue);
}
void RegistryHelper::DeleteRegistryKey(HKEY key, const TCHAR * subkey)
{
	SHDeleteKey(key,subkey);
}

HKEY RegistryHelper::GetRegistryMode(const TCHAR * strRegPath)
{
	if(System::TcsStr(strRegPath,_T("HKEY_CLASSES_ROOT"))!=NULL)
	{
		return HKEY_CLASSES_ROOT;
	}
	else if(System::TcsStr(strRegPath,_T("HKEY_CURRENT_USER"))!=NULL)
	{
		return HKEY_CLASSES_ROOT;
	}
	else if(System::TcsStr(strRegPath,_T("HKEY_LOCAL_MACHINE"))!=NULL)
	{
		return HKEY_CLASSES_ROOT;
	}
	else if(System::TcsStr(strRegPath,_T("HKEY_USERS"))!=NULL)
	{
		return HKEY_CLASSES_ROOT;
	}
	return 0;
}