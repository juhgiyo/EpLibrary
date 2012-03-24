/*! 
System for the EpLibrary
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
#include "epSystem.h"
#include "epCriticalSectionEx.h"
#include <sys/timeb.h>
using namespace epl;

#define MAX_UNSIGNED_INT ((unsigned int)(0xffffffff))
EpTime System::GetTime()
{
	static long s_lInitialSec = 0;
	static long s_lInitialUSec = 0;
	static bool s_bInitializedTime = false;
	struct timeb kTB;

	if (!s_bInitializedTime)
	{
		s_bInitializedTime = true;
		ftime(&kTB);
		s_lInitialSec = (long)kTB.time;
		s_lInitialUSec = 1000*kTB.millitm;
	}

	ftime(&kTB);
	long lCurrentSec = (long)kTB.time;
	long lCurrentUSec = 1000*kTB.millitm;
	long lDeltaSec = lCurrentSec - s_lInitialSec;
	long lDeltaUSec = lCurrentUSec -s_lInitialUSec;
	if (lDeltaUSec < 0)
	{
		lDeltaUSec += 1000000;
		lDeltaSec--;
	}

	return 0.001*(double)(1000*lDeltaSec + lDeltaUSec/1000);

	/*
	static EpTime currentTick=0;
	static unsigned int lastTickCount=0;
	unsigned int curTickCount=GetTickCount();
	EpTime diffTickCount=0;
	diffTickCount=(EpTime)curTickCount-(EpTime)lastTickCount;
	if(curTickCount<lastTickCount)
	{
		diffTickCount=(EpTime)((MAX_UNSIGNED_INT-lastTickCount)+curTickCount);
	}
	currentTick+=diffTickCount;
	return currentTick;
	*/
}

unsigned int System::GetTickCount()
{
	return ::GetTickCount();
}


errno_t System::StrDate(TCHAR * buffer, unsigned int numberOfElements)
{
	return _tstrdate_s(buffer,numberOfElements);
}

errno_t System::StrTime(TCHAR * buffer, unsigned int numberOfElements)
{
	return _tstrtime_s(buffer,numberOfElements);
}

int System::Printf(const TCHAR * format, ... )
{
	va_list args; 
	int retVal=0;
	va_start(args, format); 
	retVal=_vtprintf_s(format,args);
	va_end(args); 
	return retVal;
}

int System::Printf_V(const TCHAR* format, va_list args)
{
	return _vtprintf_s(format,args);
}


int System::SPrintf(TCHAR *dest,unsigned int sizeOfBuffer,const TCHAR *format,...)
{
	va_list args; 
	int retVal=0;
	va_start(args, format); 
	retVal=_vstprintf_s(dest,sizeOfBuffer,format,args);
	va_end(args); 
	return retVal;
}

int System::SPrintf_V(TCHAR *dest,unsigned int sizeOfBuffer,const TCHAR *format,va_list args)
{
	return _vstprintf_s(dest,sizeOfBuffer,format,args);
}

int System::StrLen(const TCHAR *format,...)
{
	va_list args; 
	int retVal; 
	va_start(args, format); 
	retVal=_vsctprintf(format,args)+1;
	va_end(args); 
	return retVal;
}

void* Memcpy (void* dest, unsigned int dstSize, const void* source, unsigned int srcSize)
{
	int err = memcpy_s(dest,dstSize,source,srcSize);
	if (err == 0)
	{
		return dest;
	}
	else
	{
		return 0;
	}
}

TCHAR* Strcat (TCHAR* dest, unsigned int dstSize, const TCHAR* source)
{
	int err = _tcscat_s(dest,dstSize,source);
	if (err == 0)
	{
		return dest;
	}
	else
	{
		return 0;
	}
}

TCHAR* Strncpy (TCHAR* dest, unsigned int dstSize, const TCHAR* source, unsigned int srcSize)
{
	int err = _tcsncpy_s(dest,dstSize,source,srcSize);
	if (err == 0)
	{
		return dest;
	}
	else
	{
		return 0;
	}
}

TCHAR* Strtok (TCHAR* token, const TCHAR* delimiters, TCHAR*& nextToken)
{
	return _tcstok_s(token,delimiters,&nextToken);
}

TCHAR *System::StrCpy(TCHAR *dest, const TCHAR*source)
{
	return _tcscpy(dest,source);
}

int System::StrLen_V(const TCHAR*format, va_list args)
{
	return _vsctprintf(format,args)+1;
}

int System::FPrintf(EpFile* const fileStream, const TCHAR* format,...)
{
	va_list args; 
	int retVal; 
	va_start(args, format); 
	retVal=_vftprintf(fileStream,format,args);
	va_end(args); 
	return retVal;

}

int System::FPrintf_V(EpFile* const fileStream, const TCHAR* format,va_list args)
{
	return _vftprintf(fileStream,format,args);
}



size_t System::FWrite(const void* buffer,unsigned int size, unsigned int count, EpFile * const fileStream)
{
	return fwrite(buffer,size,count,fileStream);
}


int System::FSize(EpFile* const fileStream)
{
	fseek (fileStream , 0 , SEEK_END);
	int fileSize = ftell (fileStream);
	rewind (fileStream);
	return fileSize;
}

int System::FRead(void *retBuff,unsigned int size, unsigned int count,EpFile * const fileStream)
{
	return fread(retBuff,size,count,fileStream);
}

errno_t System::FOpen(EpFile *&retFileStream,const TCHAR* filename,const TCHAR * mode )
{
	return _tfopen_s(&retFileStream,filename,mode);
}

int System::FClose(EpFile * const fileStream)
{
	return fclose(fileStream);
}


int System::GetLastError()
{
	return ::GetLastError();
}
DWORD System::FormatLastErrorMessage(TCHAR *retBuff, const unsigned int maxElementCount) 
{
	int err=System::GetLastError();
	return FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
		0,
		err,
		MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),
		retBuff,
		maxElementCount,
		NULL);

}

int System::NoticeBox(const TCHAR* fileName, const TCHAR* funcName, const unsigned int lineNum,const TCHAR* format,...)
{
	int length,fullLength;
	TCHAR *tmpString=NULL;
	TCHAR *tmpSTring2=NULL;
	va_list args;
	va_start(args, format);
	length=StrLen_V(format,args);
	tmpString=EP_NEW TCHAR[length];
	SPrintf_V(tmpString,length,format,args);
	fullLength=StrLen(_T("File Name : %s\nFunction Name : %s\nLine Number : %d\n\nMessage : %s\n"),fileName,funcName,lineNum,tmpString);
	tmpSTring2=EP_NEW TCHAR[fullLength];
	SPrintf(tmpSTring2,fullLength,_T("File Name : %s\nFunction Name : %s\nLine Number : %d\n\nMessage : %s\n"),fileName,funcName,lineNum,tmpString);
	va_end(args);
	int retVal=0;
	retVal=MessageBox(NULL,tmpSTring2,_T("Notice"),MB_OK);
	if(tmpString)
		EP_DELETE[] tmpString;
	if(tmpSTring2)
		EP_DELETE[] tmpSTring2;
	return retVal;

}

CString System::HexToString(const BYTE *buff, unsigned int len)
{

	CString result;

	for (size_t i = 0; i < len; i++)
	{
		BYTE c ;

		BYTE b = buff[i] >> 4;

		if (9 >= b)
		{
			c = b + '0';
		}
		else
		{
			c = (b - 10) + 'A';
		}

		result += (TCHAR)c;

		b = buff[i] & 0x0f;

		if (9 >= b)
		{
			c = b + '0';
		}
		else
		{
			c = (b - 10) + 'A';
		}

		result += (TCHAR)c;
	}

	return result;
}


CString System::MultiByteToWideChar(const char *multiByteCharString, unsigned int stringLength)
{
	TCHAR *tString=EP_NEW TCHAR[stringLength+1];
	memset(tString,0,sizeof(TCHAR)*stringLength);
	::MultiByteToWideChar(CP_ACP,0,multiByteCharString,-1,tString,stringLength);
	tString[stringLength]=_T('\0');
	CString retString=tString;
	EP_DELETE[] tString;
	return retString;
}
int System::MultiByteToWideChar(const char *multiByteCharString, unsigned int stringLength, TCHAR *retWideCharString)
{
	int result;
	TCHAR *tString=EP_NEW TCHAR[stringLength+1];
	memset(tString,0,sizeof(TCHAR)*stringLength);
	result=::MultiByteToWideChar(CP_ACP,0,multiByteCharString,-1,tString,stringLength);
	tString[stringLength]=_T('\0');
	memcpy(retWideCharString,tString,sizeof(TCHAR)*(stringLength+1));
	EP_DELETE[] tString;
	return result;
}
unsigned int System::WideCharToMultiByte(CString wideCharString, char *retMultiByteString)
{
	unsigned int result;
	result=wcstombs(retMultiByteString,wideCharString.GetString(),wideCharString.GetLength());
	retMultiByteString[wideCharString.GetLength()]='\0';
	return result;
}

unsigned int WideCharToMultiByte(const TCHAR* wideCharString, unsigned int stringLength, char *retMultiByteString)
{
	unsigned int result;
	result=wcstombs(retMultiByteString,wideCharString,stringLength);
	retMultiByteString[stringLength]='\0';
	return result;
}