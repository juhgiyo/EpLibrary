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
		s_lInitialSec = static_cast<long>(kTB.time);
		s_lInitialUSec = 1000*kTB.millitm;
	}

	ftime(&kTB);
	long lCurrentSec = static_cast<long>(kTB.time);
	long lCurrentUSec = 1000*kTB.millitm;
	long lDeltaSec = lCurrentSec - s_lInitialSec;
	long lDeltaUSec = lCurrentUSec -s_lInitialUSec;
	if (lDeltaUSec < 0)
	{
		lDeltaUSec += 1000000;
		lDeltaSec--;
	}

	return 0.001*static_cast<double>(1000*lDeltaSec + lDeltaUSec/1000);

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


void* System::Memcpy (void* dest, unsigned int dstSize, const void* source, unsigned int srcSize)
{
	if(dstSize<srcSize)
		return 0;
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

void* System::Memcpy (void* dest, const void* source, unsigned int srcSize)
{
	return System::Memcpy(dest,srcSize,source,srcSize);
}
void* System::Memset(void* source,int val,unsigned int srcSize)
{
	return memset(source,val,srcSize);
}



int System::StrDate(char * buffer, unsigned int numberOfElements)
{
	return _strdate_s(buffer,numberOfElements);
}

int System::StrTime(char * buffer, unsigned int numberOfElements)
{
	return _strtime_s(buffer,numberOfElements);
}

int System::TcsDate(TCHAR * buffer, unsigned int numberOfElements)
{
	return _tstrdate_s(buffer,numberOfElements);
}

int System::TcsTime(TCHAR * buffer, unsigned int numberOfElements)
{
	return _tstrtime_s(buffer,numberOfElements);
}

int System::Printf(const char * format, ... )
{
	va_list args; 
	int retVal=0;
	va_start(args, format); 
	retVal=vprintf_s(format,args);
	va_end(args); 
	return retVal;
}

int System::Printf_V(const char* format, va_list args)
{
	return vprintf_s(format,args);
}


int System::SPrintf(char *dest,unsigned int sizeOfBuffer,const char *format,...)
{
	va_list args; 
	int retVal=0;
	va_start(args, format); 
	retVal=vsprintf_s(dest,sizeOfBuffer,format,args);
	va_end(args); 
	return retVal;
}

int System::SPrintf_V(char *dest,unsigned int sizeOfBuffer,const char *format,va_list args)
{
	return vsprintf_s(dest,sizeOfBuffer,format,args);
}

int System::TPrintf(const TCHAR * format, ... )
{
	va_list args; 
	int retVal=0;
	va_start(args, format); 
	retVal=_vtprintf_s(format,args);
	va_end(args); 
	return retVal;
}
int System::TPrintf_V(const TCHAR* format, va_list args)
{
	return _vtprintf_s(format,args);
}
int System::TSPrintf(TCHAR *dest,unsigned int sizeOfBuffer,const TCHAR *format,...)
{
	va_list args; 
	int retVal=0;
	va_start(args, format); 
	retVal=_vstprintf_s(dest,sizeOfBuffer,format,args);
	va_end(args); 
	return retVal;
}

int System::TSPrintf_V(TCHAR *dest,unsigned int sizeOfBuffer,const TCHAR *format,va_list args)
{
	return _vstprintf_s(dest,sizeOfBuffer,format,args);
}

int System::StrLen(const char *format,...)
{
	va_list args; 
	int retVal; 
	va_start(args, format); 
	retVal=_vscprintf(format,args);
	va_end(args); 
	return retVal;
}

char* System::StrCat (char* dest, unsigned int dstSize, const char* source)
{
	int err = strcat_s(dest,dstSize,source);
	if (err == 0)
	{
		return dest;
	}
	else
	{
		return 0;
	}
}

char* System::StrNCpy (char* dest, unsigned int dstSize, const char* source, unsigned int srcSize)
{
	int err = strncpy_s(dest,dstSize,source,srcSize);
	if (err == 0)
	{
		return dest;
	}
	else
	{
		return 0;
	}
}

char* System::StrTok (char* token, const char* delimiters, char*& nextToken)
{
	return strtok_s(token,delimiters,&nextToken);
}

char *System::StrCpy(char *dest, const char*source)
{
	return strcpy(dest,source);
}
const char *System::StrStr(const char *source,const char *subString)
{
	return strstr(source,subString);
}

int System::StrLen_V(const char*format, va_list args)
{
	return _vscprintf(format,args)+1;
}

int System::TcsLen(const TCHAR *format,...)
{
	va_list args; 
	int retVal; 
	va_start(args, format); 
	retVal=_vsctprintf(format,args);
	va_end(args); 
	return retVal;
}
TCHAR* System::TcsCat (TCHAR* dest, unsigned int dstSize, const TCHAR* source)
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
TCHAR* System::TcsNCpy (TCHAR* dest, unsigned int dstSize, const TCHAR* source, unsigned int srcSize)
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
TCHAR* System::TcsTok (TCHAR* token, const TCHAR* delimiters, TCHAR*& nextToken)
{
	return _tcstok_s(token,delimiters,&nextToken);
}

TCHAR *System::TcsCpy(TCHAR *dest, const TCHAR*source)
{
	return _tcscpy(dest,source);
}
const TCHAR *System::TcsStr(const TCHAR *source,const TCHAR *subString)
{
	return _tcsstr(source,subString);
}

int System::TcsLen_V(const TCHAR*format, va_list args)
{
	return _vsctprintf(format,args)+1;
}


int System::FPrintf(EpFile* const fileStream, const char* format,...)
{
	va_list args; 
	int retVal; 
	va_start(args, format); 
	retVal=vfprintf(fileStream,format,args);
	va_end(args); 
	return retVal;

}

int System::FPrintf_V(EpFile* const fileStream, const char* format,va_list args)
{
	return vfprintf(fileStream,format,args);
}

int System::TFPrintf(EpFile* const fileStream, const TCHAR* format,...)
{
	va_list args; 
	int retVal; 
	va_start(args, format); 
	retVal=_vftprintf(fileStream,format,args);
	va_end(args); 
	return retVal;

}

int System::TFPrintf_V(EpFile* const fileStream, const TCHAR* format,va_list args)
{
	return _vftprintf(fileStream,format,args);
}


int System::FOpen(EpFile *&retFileStream,const char* filename,const char * mode )
{
	return fopen_s(&retFileStream,filename,mode);
}

int System::TFOpen(EpFile *&retFileStream,const TCHAR* filename,const TCHAR * mode )
{
	return _tfopen_s(&retFileStream,filename,mode);
}


int System::FClose(EpFile * const fileStream)
{
	return fclose(fileStream);
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


int System::GetLastError()
{
	return ::GetLastError();
}
unsigned long System::FormatLastErrorMessage(TCHAR *retBuff, const unsigned int maxElementCount) 
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
	length=TcsLen_V(format,args);
	tmpString=EP_NEW TCHAR[length];
	TSPrintf_V(tmpString,length,format,args);
	fullLength=TcsLen(_T("File Name : %s\nFunction Name : %s\nLine Number : %d\n\nMessage : %s\n"),fileName,funcName,lineNum,tmpString);
	tmpSTring2=EP_NEW TCHAR[fullLength];
	TSPrintf(tmpSTring2,fullLength,_T("File Name : %s\nFunction Name : %s\nLine Number : %d\n\nMessage : %s\n"),fileName,funcName,lineNum,tmpString);
	va_end(args);
	int retVal=0;
	retVal=MessageBox(NULL,tmpSTring2,_T("Notice"),MB_OK);
	if(tmpString)
		EP_DELETE[] tmpString;
	if(tmpSTring2)
		EP_DELETE[] tmpSTring2;
	return retVal;

}

EpTString System::HexToString(const unsigned char *buff, unsigned int len)
{

	EpTString result;

	for (size_t i = 0; i < len; i++)
	{
		TCHAR c ;

		unsigned char b = buff[i] >> 4;

		if (9 >= b)
		{
			c = b + _T('0');
		}
		else
		{
			c = (b - 10) + _T('A');
		}

		result += c;

		b = buff[i] & 0x0f;

		if (9 >= b)
		{
			c = b + _T('0');
		}
		else
		{
			c = (b - 10) + _T('A');
		}

		result += c;
	}

	return result;
}


EpWString System::MultiByteToWideChar(const char *multiByteCharString, unsigned int stringLength)
{
	wchar_t *tString=EP_NEW wchar_t[stringLength+1];
	System::Memset(tString,0,sizeof(wchar_t)*stringLength);
	::MultiByteToWideChar(CP_ACP,0,multiByteCharString,-1,tString,stringLength);
	tString[stringLength]=_T('\0');
	EpWString retString=tString;
	EP_DELETE[] tString;
	return retString;
}
int System::MultiByteToWideChar(const char *multiByteCharString, unsigned int stringLength, wchar_t *retWideCharString)
{
	int result;
	wchar_t *tString=EP_NEW wchar_t[stringLength+1];
	System::Memset(tString,0,sizeof(wchar_t)*stringLength);
	result=::MultiByteToWideChar(CP_ACP,0,multiByteCharString,-1,tString,stringLength);
	tString[stringLength]=_T('\0');
	System::Memcpy(retWideCharString,tString,sizeof(wchar_t)*(stringLength+1));
	EP_DELETE[] tString;
	return result;
}

EpWString System::MultiByteToWideChar(const char *multiByteCharString)
{
	unsigned int stringLength=System::StrLen(multiByteCharString);
	wchar_t *tString=EP_NEW wchar_t[stringLength+1];
	System::Memset(tString,0,sizeof(wchar_t)*stringLength);
	::MultiByteToWideChar(CP_ACP,0,multiByteCharString,-1,tString,stringLength);
	tString[stringLength]=_T('\0');
	EpWString retString=tString;
	EP_DELETE[] tString;
	return retString;
}
int System::MultiByteToWideChar(const char *multiByteCharString, wchar_t *retWideCharString)
{
	int result;
	unsigned int stringLength=System::StrLen(multiByteCharString);
	wchar_t *tString=EP_NEW wchar_t[stringLength+1];
	System::Memset(tString,0,sizeof(wchar_t)*stringLength);
	result=::MultiByteToWideChar(CP_ACP,0,multiByteCharString,-1,tString,stringLength);
	tString[stringLength]=_T('\0');
	System::Memcpy(retWideCharString,tString,sizeof(wchar_t)*(stringLength+1));
	EP_DELETE[] tString;
	return result;
}
unsigned int System::WideCharToMultiByte(const wchar_t* wideCharString, char *retMultiByteString)
{
	unsigned int result;
	unsigned int strLength=System::TcsLen(wideCharString);
	result=wcstombs(retMultiByteString,wideCharString,strLength);
	retMultiByteString[strLength]='\0';
	return result;
}

unsigned int System::WideCharToMultiByte(const wchar_t* wideCharString, unsigned int stringLength,char *retMultiByteString)
{
	unsigned int result;
	result=wcstombs(retMultiByteString,wideCharString,stringLength);
	retMultiByteString[stringLength]='\0';
	return result;
}