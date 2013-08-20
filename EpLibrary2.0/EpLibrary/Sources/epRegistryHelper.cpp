/*! 
RegistryHelper for the EpLibrary

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
#include "epRegistryHelper.h"
#include "epMemory.h"
#include <Shlwapi.h>

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;

bool RegistryHelper::SetRegistryData(HKEY key,const TCHAR * subKey,const TCHAR * regName,const TCHAR * regData)
{
	if(SHSetValue(key,subKey,regName,REG_SZ,regData,(System::TcsLen(regData)+1)*sizeof(TCHAR))==ERROR_SUCCESS)
		return true;
	return false;
}

bool RegistryHelper::SetRegistryData(HKEY key,const TCHAR *subKey,const TCHAR * regName,unsigned long regType,void * regData,unsigned long sizeInByte)
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

bool RegistryHelper::GetRegistryData(HKEY key,const TCHAR *subKey,const TCHAR *regName,unsigned long sizeInByte,void *retBuf, unsigned long &retSizeReadInByte, unsigned long &retRegType)
{
	unsigned long size=sizeInByte;
	if(SHGetValue(key,subKey,regName,&retRegType,retBuf,&size)==ERROR_SUCCESS)
	{
		retSizeReadInByte=size;
		return true;
	}
	return false;

}

unsigned long RegistryHelper::GetRegistryDataSize(HKEY key,const TCHAR *subKey,const TCHAR * regName)
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