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

	return static_cast<double>(1000*lDeltaSec + lDeltaUSec/1000); //*0.001 for seconds

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


void* System::Memcpy (void* dest, unsigned int dstSizeInByte, const void* source, unsigned int srcSizeInByte)
{
	if(dstSizeInByte<srcSizeInByte)
		return 0;
	int err = memcpy_s(dest,dstSizeInByte,source,srcSizeInByte);
	if (err == 0)
	{
		return dest;
	}
	else
	{
		return 0;
	}
}

void* System::Memcpy (void* dest, const void* source, unsigned int srcSizeInByte)
{
	return System::Memcpy(dest,srcSizeInByte,source,srcSizeInByte);
}
void* System::Memset(void* source,int val,unsigned int srcSizeInByte)
{
	return memset(source,val,srcSizeInByte);
}



EpErrno System::StrDate(char * buffer, unsigned int numberOfElements)
{
	return _strdate_s(buffer,numberOfElements);
}

EpErrno System::StrTime(char * buffer, unsigned int numberOfElements)
{
	return _strtime_s(buffer,numberOfElements);
}

EpErrno System::WStrDate(wchar_t * buffer, unsigned int numberOfElements)
{
	return _wstrdate_s(buffer,numberOfElements);
}

EpErrno System::WStrTime(wchar_t * buffer, unsigned int numberOfElements)
{
	return _wstrtime_s(buffer,numberOfElements);
}

EpErrno System::TcsDate(TCHAR * buffer, unsigned int numberOfElements)
{
	return _tstrdate_s(buffer,numberOfElements);
}

EpErrno System::TcsTime(TCHAR * buffer, unsigned int numberOfElements)
{
	return _tstrtime_s(buffer,numberOfElements);
}

int System::Printf(const char * format, ... )
{
	va_list args=NULL;
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


int System::SPrintf(char *dest,unsigned int dstSizeInCharacter,const char *format,...)
{
	va_list args=NULL;
	int retVal=0;
	va_start(args, format); 
	retVal=vsprintf_s(dest,dstSizeInCharacter,format,args);
	va_end(args); 
	return retVal;
}

int System::SPrintf_V(char *dest,unsigned int dstSizeInCharacter,const char *format,va_list args)
{
	return vsprintf_s(dest,dstSizeInCharacter,format,args);
}

int System::SPrintf(EpString &retDest,const char *format,...)
{
	int length;
	char *tmpString=NULL;
	int retVal=0;
	va_list args=NULL;
	va_start(args, format);
	length=StrLen_V(format,args);
	tmpString=EP_NEW char[length+1];
	retVal=SPrintf_V(tmpString,length+1,format,args);
	va_end(args);
	retDest=tmpString;
	if(tmpString)
		EP_DELETE[] tmpString;
	return retVal;
}
int System::SPrintf_V(EpString &retDest,const char *format,va_list args)
{
	int length;
	char *tmpString=NULL;
	int retVal=0;
	length=StrLen_V(format,args);
	tmpString=EP_NEW char[length+1];
	retVal=SPrintf_V(tmpString,length+1,format,args);
	retDest=tmpString;
	if(tmpString)
		EP_DELETE[] tmpString;
	return retVal;
}

int System::WPrintf(const wchar_t * format, ... )
{
	va_list args=NULL; 
	int retVal=0;
	va_start(args, format); 
	retVal=vwprintf_s(format,args);
	va_end(args); 
	return retVal;
}

int System::WPrintf_V(const wchar_t* format, va_list args)
{
	return vwprintf_s(format,args);
}


int System::SWPrintf(wchar_t *dest,unsigned int dstSizeInCharacter,const wchar_t *format,...)
{
	va_list args=NULL;
	int retVal=0;
	va_start(args, format); 
	retVal=vswprintf_s(dest,dstSizeInCharacter,format,args);
	va_end(args); 
	return retVal;
}

int System::SWPrintf_V(wchar_t *dest,unsigned int dstSizeInCharacter,const wchar_t *format,va_list args)
{
	return vswprintf_s(dest,dstSizeInCharacter,format,args);
}
int System::SWPrintf(EpWString &retDest,const wchar_t *format,...)
{
	int length;
	wchar_t *tmpString=NULL;
	int retVal=0;
	va_list args=NULL;
	va_start(args, format);
	length=WcsLen_V(format,args);
	tmpString=EP_NEW wchar_t[length+1];
	retVal=SWPrintf_V(tmpString,length+1,format,args);
	va_end(args);
	retDest=tmpString;
	if(tmpString)
		EP_DELETE[] tmpString;
	return retVal;
}
int System::SWPrintf_V(EpWString &retDest,const wchar_t *format,va_list args)
{
	int length;
	wchar_t *tmpString=NULL;
	int retVal=0;
	length=WcsLen_V(format,args);
	tmpString=EP_NEW wchar_t[length+1];
	retVal=SWPrintf_V(tmpString,length+1,format,args);
	retDest=tmpString;
	if(tmpString)
		EP_DELETE[] tmpString;
	return retVal;
}

int System::TPrintf(const TCHAR * format, ... )
{
	va_list args=NULL;
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
int System::STPrintf(TCHAR *dest,unsigned int dstSizeInCharacter,const TCHAR *format,...)
{
	va_list args=NULL; 
	int retVal=0;
	va_start(args, format); 
	retVal=_vstprintf_s(dest,dstSizeInCharacter,format,args);
	va_end(args); 
	return retVal;
}

int System::STPrintf_V(TCHAR *dest,unsigned int dstSizeInCharacter,const TCHAR *format,va_list args)
{
	return _vstprintf_s(dest,dstSizeInCharacter,format,args);
}


int System::STPrintf(EpTString &retDest,const TCHAR *format,...)
{
	int length;
	TCHAR *tmpString=NULL;
	int retVal=0;
	va_list args=NULL;
	va_start(args, format);
	length=TcsLen_V(format,args);
	tmpString=EP_NEW TCHAR[length+1];
	retVal=STPrintf_V(tmpString,length+1,format,args);
	va_end(args);
	retDest=tmpString;
	if(tmpString)
		EP_DELETE[] tmpString;
	return retVal;
}
int System::STPrintf_V(EpTString &retDest,const TCHAR *format,va_list args)
{
	int length;
	TCHAR *tmpString=NULL;
	int retVal=0;
	length=TcsLen_V(format,args);
	tmpString=EP_NEW TCHAR[length+1];
	retVal=STPrintf_V(tmpString,length+1,format,args);
	retDest=tmpString;
	if(tmpString)
		EP_DELETE[] tmpString;
	return retVal;
}

int System::StrLen(const char *format,...)
{
	va_list args=NULL;
	int retVal; 
	va_start(args, format); 
	retVal=_vscprintf(format,args);
	va_end(args); 
	return retVal;
}

char* System::StrCat (char* dest, unsigned int dstSizeInCharacter, const char* source)
{
	int err = strcat_s(dest,dstSizeInCharacter,source);
	if (err == 0)
	{
		return dest;
	}
	else
	{
		return 0;
	}
}

char* System::StrNCpy (char* dest, unsigned int dstSizeInCharacter, const char* source, unsigned int srcSizeInCharacter)
{
	int err = strncpy_s(dest,dstSizeInCharacter,source,srcSizeInCharacter);
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
	return _vscprintf(format,args);
}



int System::MbsLen(const unsigned char *format,...)
{
	va_list args=NULL;
	int retVal; 
	va_start(args, format); 
	retVal=_vscprintf(reinterpret_cast<const char*>(format),args);
	va_end(args); 
	return retVal;
}

unsigned char* System::MbsCat (unsigned char* dest, unsigned int dstSizeInCharacter, const unsigned char* source)
{
	int err = _mbscat_s(dest,dstSizeInCharacter,source);
	if (err == 0)
	{
		return dest;
	}
	else
	{
		return 0;
	}
}

unsigned char* System::MbsNCpy (unsigned char* dest, unsigned int dstSizeInCharacter, const unsigned char* source, unsigned int srcSizeInCharacter)
{
	int err = _mbsncpy_s(dest,dstSizeInCharacter,source,srcSizeInCharacter);
	if (err == 0)
	{
		return dest;
	}
	else
	{
		return 0;
	}
}

unsigned char* System::MbsTok (unsigned char* token, const unsigned char* delimiters,unsigned char*& nextToken)
{
	return _mbstok_s(token,delimiters,&nextToken);
}

unsigned char *System::MbsCpy(unsigned char *dest, const unsigned char*source)
{
	return _mbscpy(dest,source);
}
const unsigned char *System::MbsStr(const unsigned char *source,const unsigned char *subString)
{
	return _mbsstr(source,subString);
}

int System::MbsLen_V(const unsigned char*format, va_list args)
{
	return _vscprintf(reinterpret_cast<const char*>(format),args);
}

int System::WcsLen(const wchar_t *format,...)
{
	va_list args=NULL;
	int retVal; 
	va_start(args, format); 
	retVal=_vscwprintf(format,args);
	va_end(args); 
	return retVal;
}

wchar_t* System::WcsCat (wchar_t* dest, unsigned int dstSizeInCharacter, const wchar_t* source)
{
	int err = wcscat_s(dest,dstSizeInCharacter,source);
	if (err == 0)
	{
		return dest;
	}
	else
	{
		return 0;
	}
}

wchar_t* System::WcsNCpy (wchar_t* dest, unsigned int dstSizeInCharacter, const wchar_t* source, unsigned int srcSizeInCharacter)
{
	int err = wcsncpy_s(dest,dstSizeInCharacter,source,srcSizeInCharacter);
	if (err == 0)
	{
		return dest;
	}
	else
	{
		return 0;
	}
}

wchar_t* System::WcsTok (wchar_t* token, const wchar_t* delimiters, wchar_t*& nextToken)
{
	return wcstok_s(token,delimiters,&nextToken);
}

wchar_t *System::WcsCpy(wchar_t *dest, const wchar_t*source)
{
	return wcscpy(dest,source);
}
const wchar_t *System::WcsStr(const wchar_t *source,const wchar_t *subString)
{
	return wcsstr(source,subString);
}

int System::WcsLen_V(const wchar_t*format, va_list args)
{
	return _vscwprintf(format,args);
}


int System::TcsLen(const TCHAR *format,...)
{
	va_list args=NULL; 
	int retVal; 
	va_start(args, format); 
	retVal=_vsctprintf(format,args);
	va_end(args); 
	return retVal;
}
TCHAR* System::TcsCat (TCHAR* dest, unsigned int dstSizeInCharacter, const TCHAR* source)
{
	int err = _tcscat_s(dest,dstSizeInCharacter,source);
	if (err == 0)
	{
		return dest;
	}
	else
	{
		return 0;
	}
}
TCHAR* System::TcsNCpy (TCHAR* dest, unsigned int dstSizeInCharacter, const TCHAR* source, unsigned int srcSizeInCharacter)
{
	int err = _tcsncpy_s(dest,dstSizeInCharacter,source,srcSizeInCharacter);
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
	return _vsctprintf(format,args);
}


int System::FPrintf(EpFile* const fileStream, const char* format,...)
{
	va_list args=NULL;
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

int System::FWPrintf(EpFile* const fileStream, const wchar_t* format,...)
{
	va_list args=NULL;
	int retVal; 
	va_start(args, format); 
	retVal=vfwprintf(fileStream,format,args);
	va_end(args); 
	return retVal;

}

int System::FWPrintf_V(EpFile* const fileStream, const wchar_t* format,va_list args)
{
	return vfwprintf(fileStream,format,args);
}

int System::FTPrintf(EpFile* const fileStream, const TCHAR* format,...)
{
	va_list args=NULL;
	int retVal; 
	va_start(args, format); 
	retVal=_vftprintf(fileStream,format,args);
	va_end(args); 
	return retVal;

}

int System::FTPrintf_V(EpFile* const fileStream, const TCHAR* format,va_list args)
{
	return _vftprintf(fileStream,format,args);
}


EpErrno System::FOpen(EpFile *&retFileStream,const char* filename,const char * mode )
{
	return fopen_s(&retFileStream,filename,mode);
}

EpErrno System::FWOpen(EpFile *&retFileStream,const wchar_t* filename,const wchar_t * mode )
{
	return _wfopen_s(&retFileStream,filename,mode);
}

EpErrno System::FTOpen(EpFile *&retFileStream,const TCHAR* filename,const TCHAR * mode )
{
	return _tfopen_s(&retFileStream,filename,mode);
}


int System::FClose(EpFile * const fileStream)
{
	return fclose(fileStream);
}

unsigned int System::FWrite(const void* buffer,unsigned int sizeInByte, unsigned int count, EpFile * const fileStream)
{
	return fwrite(buffer,sizeInByte,count,fileStream);
}


long System::FSize(EpFile* const fileStream)
{
	fseek (fileStream , 0 , SEEK_END);
	long fileSize = ftell (fileStream);
	rewind (fileStream);
	return fileSize;
}

unsigned int System::FRead(void *retBuff,unsigned int sizeInByte, unsigned int count,EpFile * const fileStream)
{
	return fread(retBuff,sizeInByte,count,fileStream);
}


unsigned long System::GetLastError()
{
	return ::GetLastError();
}
unsigned long System::FormatLastErrorMessage(TCHAR *retBuff, const unsigned int maxElementCount,unsigned long *retErrNo) 
{
	unsigned long err=System::GetLastError();
	if(retErrNo!=NULL)
		*retErrNo=err;

	return FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
		0,
		err,
		MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),
		retBuff,
		maxElementCount,
		NULL);

}

unsigned long System::FormatLastErrorMessage(EpString &retString,unsigned long *retErrNo) 
{
	unsigned long err=System::GetLastError();
	if(retErrNo!=NULL)
		*retErrNo=err;

	TCHAR *errMsg=NULL;
	unsigned long retVal=FormatMessage(
		// use system message tables to retrieve error text
		FORMAT_MESSAGE_FROM_SYSTEM
		// allocate buffer on local heap for error text
		|FORMAT_MESSAGE_ALLOCATE_BUFFER
		// Important! will fail otherwise, since we're not 
		// (and CANNOT) pass insertion parameters
		|FORMAT_MESSAGE_IGNORE_INSERTS,  
		NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
		err,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		errMsg,  // output 
		0, // minimum size for output buffer
		NULL);   // arguments - see note 

	if ( errMsg != NULL)
	{

#if defined(_UNICODE) || defined(UNICODE)
		retString=System::WideCharToMultiByte(errMsg);
#else// defined(_UNICODE) || defined(UNICODE)
		retString=errMsg;
#endif// defined(_UNICODE) || defined(UNICODE)

		// release memory allocated by FormatMessage()
		LocalFree(errMsg);
		errMsg = NULL;
	}
	return retVal;
}

unsigned long System::FormatLastErrorMessage(EpWString &retString,unsigned long *retErrNo) 
{
	unsigned long err=System::GetLastError();
	if(retErrNo!=NULL)
		*retErrNo=err;

	TCHAR *errMsg=NULL;
	unsigned long retVal=FormatMessage(
		// use system message tables to retrieve error text
		FORMAT_MESSAGE_FROM_SYSTEM
		// allocate buffer on local heap for error text
		|FORMAT_MESSAGE_ALLOCATE_BUFFER
		// Important! will fail otherwise, since we're not 
		// (and CANNOT) pass insertion parameters
		|FORMAT_MESSAGE_IGNORE_INSERTS,  
		NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
		err,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		errMsg,  // output 
		0, // minimum size for output buffer
		NULL);   // arguments - see note 

	if ( errMsg != NULL)
	{

#if defined(_UNICODE) || defined(UNICODE)
		retString=errMsg;
#else// defined(_UNICODE) || defined(UNICODE)
		retString=System::MultiByteToWideChar(errMsg);
#endif// defined(_UNICODE) || defined(UNICODE)

		// release memory allocated by FormatMessage()
		LocalFree(errMsg);
		errMsg = NULL;
	}
	return retVal;
}

int System::NoticeBox(const TCHAR* fileName, const TCHAR* funcName, const unsigned int lineNum,const TCHAR* format,...)
{
	int length,fullLength;
	TCHAR *tmpString=NULL;
	TCHAR *tmpSTring2=NULL;
	va_list args=NULL;
	va_start(args, format);
	length=TcsLen_V(format,args);
	tmpString=EP_NEW TCHAR[length+1];
	STPrintf_V(tmpString,length+1,format,args);
	va_end(args);
	fullLength=TcsLen(_T("File Name : %s\r\nFunction Name : %s\r\nLine Number : %d\r\n\r\nMessage : %s\r\n"),fileName,funcName,lineNum,tmpString);
	tmpSTring2=EP_NEW TCHAR[fullLength+1];
	STPrintf(tmpSTring2,fullLength+1,_T("File Name : %s\r\nFunction Name : %s\r\nLine Number : %d\r\n\r\nMessage : %s\r\n"),fileName,funcName,lineNum,tmpString);
	
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
	unsigned int sizeNeeded=::MultiByteToWideChar(CP_ACP,0,multiByteCharString,stringLength,NULL,0);
	wchar_t *tString=EP_NEW wchar_t[sizeNeeded+1];
	System::Memset(tString,0,sizeof(wchar_t)*(sizeNeeded+1));
	//mbstowcs(tString,multiByteCharString,stringLength);
	::MultiByteToWideChar(CP_ACP,0,multiByteCharString,stringLength,tString,sizeNeeded);
	tString[sizeNeeded]=_T('\0');
	EpWString retString=tString;
	EP_DELETE[] tString;
	return retString;
}

EpWString System::MultiByteToWideChar(const char *multiByteCharString)
{
	unsigned int sizeNeeded=::MultiByteToWideChar(CP_ACP,0,multiByteCharString,-1,NULL,0);
	wchar_t *tString=EP_NEW wchar_t[sizeNeeded+1];
	System::Memset(tString,0,sizeof(wchar_t)*(sizeNeeded+1));
	//mbstowcs(tString,multiByteCharString,stringLength);
	::MultiByteToWideChar(CP_ACP,0,multiByteCharString,-1,tString,sizeNeeded);
	tString[sizeNeeded]=_T('\0');
	EpWString retString=tString;
	EP_DELETE[] tString;
	return retString;
}

int System::MultiByteToWideChar(const char *multiByteCharString, unsigned int stringLength, wchar_t *retWideCharString)
{
	if(retWideCharString==NULL)
	{
		return ::MultiByteToWideChar(CP_ACP,0,multiByteCharString,stringLength,NULL,0);
	}
	int result;
	unsigned int sizeNeeded=::MultiByteToWideChar(CP_ACP,0,multiByteCharString,stringLength,NULL,0);
	//result=mbstowcs(retWideCharString,multiByteCharString,stringLength);
	result=::MultiByteToWideChar(CP_ACP,0,multiByteCharString,stringLength,retWideCharString,sizeNeeded);
	if(retWideCharString)
		retWideCharString[sizeNeeded]=_T('\0');
	return result;
}

int System::MultiByteToWideChar(const char *multiByteCharString, wchar_t *retWideCharString)
{
	if(retWideCharString==NULL)
	{
		return ::MultiByteToWideChar(CP_ACP,0,multiByteCharString,-1,NULL,0);
	}
	int result;
	unsigned int sizeNeeded=::MultiByteToWideChar(CP_ACP,0,multiByteCharString,-1,NULL,0);
	//result=mbstowcs(retWideCharString,multiByteCharString,stringLength);
	result=::MultiByteToWideChar(CP_ACP,0,multiByteCharString,-1,retWideCharString,sizeNeeded);
	if(retWideCharString)
		retWideCharString[sizeNeeded]=_T('\0');
	return result;
}


unsigned int System::WideCharToMultiByte(const wchar_t* wideCharString, char *retMultiByteString)
{
	if(retMultiByteString==NULL)
	{
		return ::WideCharToMultiByte(CP_ACP,0,wideCharString,-1,NULL,0,NULL,NULL);
	}
	unsigned int result;
	unsigned int sizeNeeded=::WideCharToMultiByte(CP_ACP,0,wideCharString,-1,NULL,0,NULL,NULL);
	//result=wcstombs(retMultiByteString,wideCharString,sizeNeeded);
	result=::WideCharToMultiByte(CP_ACP,0,wideCharString,-1,retMultiByteString,sizeNeeded,NULL,NULL);
	if(retMultiByteString)
		retMultiByteString[sizeNeeded]='\0';
	return result;
}

unsigned int System::WideCharToMultiByte(const wchar_t* wideCharString, unsigned int stringLength,char *retMultiByteString)
{
	if(retMultiByteString==NULL)
	{
		return ::WideCharToMultiByte(CP_ACP,0,wideCharString,stringLength,NULL,0,NULL,NULL);
	}
	unsigned int result;
	unsigned int sizeNeeded=::WideCharToMultiByte(CP_ACP,0,wideCharString,stringLength,NULL,0,NULL,NULL);
	//result=wcstombs(retMultiByteString,wideCharString,stringLength);
	result=::WideCharToMultiByte(CP_ACP,0,wideCharString,stringLength,retMultiByteString,sizeNeeded,NULL,NULL);
	if(retMultiByteString)
		retMultiByteString[sizeNeeded]='\0';
	return result;
}

EpString System::WideCharToMultiByte(const wchar_t *wideCharString, unsigned int stringLength)
{
	unsigned int result;
	unsigned int sizeNeeded=::WideCharToMultiByte(CP_ACP,0,wideCharString,stringLength,NULL,0,NULL,NULL);
	char *cString=EP_NEW char[sizeNeeded+1];
	System::Memset(cString,0,sizeof(char)*(sizeNeeded+1));
	//result=wcstombs(cString,wideCharString,stringLength);
	result=::WideCharToMultiByte(CP_ACP,0,wideCharString,stringLength,cString,sizeNeeded,NULL,NULL);
	cString[sizeNeeded]='\0';
	EpString retString=cString;
	EP_DELETE[] cString;
	return retString;
}

EpString System::WideCharToMultiByte(const wchar_t *wideCharString)
{
	unsigned int result;
	unsigned int sizeNeeded=::WideCharToMultiByte(CP_ACP,0,wideCharString,-1,NULL,0,NULL,NULL);
	char *cString=EP_NEW char[sizeNeeded+1];
	System::Memset(cString,0,sizeof(char)*(sizeNeeded+1));
	//result=wcstombs(cString,wideCharString,stringLength);
	result=::WideCharToMultiByte(CP_ACP,0,wideCharString,-1,cString,sizeNeeded,NULL,NULL);
	cString[sizeNeeded]='\0';
	EpString retString=cString;
	EP_DELETE[] cString;
	return retString;
}


unsigned int  System::UTF16ToUTF8(const wchar_t* utf16String, char *retutf8String)
{
	if(retutf8String==NULL)
	{
		return ::WideCharToMultiByte(CP_UTF8,0,utf16String,-1,NULL,0,NULL,NULL);
	}
	unsigned int result;
	unsigned int sizeNeeded=::WideCharToMultiByte(CP_UTF8,0,utf16String,-1,NULL,0,NULL,NULL);
	result=::WideCharToMultiByte(CP_UTF8,0,utf16String,-1,retutf8String,sizeNeeded,NULL,NULL);
	if(retutf8String)
		retutf8String[sizeNeeded]='\0';
	return result;
}
unsigned int  System::UTF16ToUTF8(const wchar_t* utf16String,unsigned int stringLength, char *retutf8String)
{
	if(retutf8String==NULL)
	{
		return ::WideCharToMultiByte(CP_UTF8,0,utf16String,stringLength,NULL,0,NULL,NULL);
	}
	unsigned int result;
	unsigned int sizeNeeded=::WideCharToMultiByte(CP_UTF8,0,utf16String,stringLength,NULL,0,NULL,NULL);
	result=::WideCharToMultiByte(CP_UTF8,0,utf16String,stringLength,retutf8String,sizeNeeded,NULL,NULL);
	if(retutf8String)
		retutf8String[sizeNeeded]='\0';
	return result;
}
EpString System::UTF16ToUTF8(const wchar_t *utf16String, unsigned int stringLength)
{
	unsigned int result;
	unsigned int sizeNeeded=::WideCharToMultiByte(CP_UTF8,0,utf16String,stringLength,NULL,0,NULL,NULL);
	char *cString=EP_NEW char[sizeNeeded+1];
	System::Memset(cString,0,sizeof(char)*(sizeNeeded+1));
	result=::WideCharToMultiByte(CP_UTF8,0,utf16String,stringLength,cString,sizeNeeded,NULL,NULL);
	cString[sizeNeeded]='\0';
	EpString retString=cString;
	EP_DELETE[] cString;
	return retString;
}
EpString System::UTF16ToUTF8(const wchar_t *utf16String)
{
	unsigned int result;
	unsigned int sizeNeeded=::WideCharToMultiByte(CP_UTF8,0,utf16String,-1,NULL,0,NULL,NULL);
	char *cString=EP_NEW char[sizeNeeded+1];
	System::Memset(cString,0,sizeof(char)*(sizeNeeded+1));
	result=::WideCharToMultiByte(CP_UTF8,0,utf16String,-1,cString,sizeNeeded,NULL,NULL);
	cString[sizeNeeded]='\0';
	EpString retString=cString;
	EP_DELETE[] cString;
	return retString;
}


EpWString System::UTF8ToUTF16(const char *utf8String, unsigned int stringLength)
{
	unsigned int sizeNeeded=::MultiByteToWideChar(CP_UTF8,0,utf8String,stringLength,NULL,0);
	wchar_t *tString=EP_NEW wchar_t[sizeNeeded+1];
	System::Memset(tString,0,sizeof(wchar_t)*(sizeNeeded+1));
	::MultiByteToWideChar(CP_UTF8,0,utf8String,stringLength,tString,sizeNeeded);
	tString[sizeNeeded]=_T('\0');
	EpWString retString=tString;
	EP_DELETE[] tString;
	return retString;
}
EpWString System::UTF8ToUTF16(const char *utf8String)
{
	unsigned int sizeNeeded=::MultiByteToWideChar(CP_UTF8,0,utf8String,-1,NULL,0);
	wchar_t *tString=EP_NEW wchar_t[sizeNeeded+1];
	System::Memset(tString,0,sizeof(wchar_t)*(sizeNeeded+1));
	::MultiByteToWideChar(CP_UTF8,0,utf8String,-1,tString,sizeNeeded);
	tString[sizeNeeded]=_T('\0');
	EpWString retString=tString;
	EP_DELETE[] tString;
	return retString;
}
int System::UTF8ToUTF16(const char *utf8String, unsigned int stringLength, wchar_t *retUtf16String)
{
	if(retUtf16String==NULL)
	{
		return ::MultiByteToWideChar(CP_UTF8,0,utf8String,stringLength,NULL,0);
	}
	int result;
	unsigned int sizeNeeded=::MultiByteToWideChar(CP_UTF8,0,utf8String,stringLength,NULL,0);
	result=::MultiByteToWideChar(CP_UTF8,0,utf8String,stringLength,retUtf16String,sizeNeeded);
	if(retUtf16String)
		retUtf16String[sizeNeeded]=_T('\0');
	return result;
}
int System::UTF8ToUTF16(const char *utf8String, wchar_t *retUtf16String)
{
	if(retUtf16String==NULL)
	{
		return ::MultiByteToWideChar(CP_UTF8,0,utf8String,-1,NULL,0);
	}
	int result;
	unsigned int sizeNeeded=::MultiByteToWideChar(CP_UTF8,0,utf8String,-1,NULL,0);
	result=::MultiByteToWideChar(CP_UTF8,0,utf8String,-1,retUtf16String,sizeNeeded);
	if(retUtf16String)
		retUtf16String[sizeNeeded]=_T('\0');
	return result;
}

bool System::IsMultiByte(byte *multiByteString, unsigned int byteLength)
{
	for(int stringTrav=0;stringTrav<byteLength;stringTrav++)
	{
		if(IsDBCSLeadByte(multiByteString[stringTrav]))
			return true;
	}
	return false;
}

void System::OutputDebugString(TCHAR * format,...)
{
	int length;
	TCHAR *tmpString=NULL;
	int retVal=0;
	va_list args=NULL;
	va_start(args, format);
	length=TcsLen_V(format,args);
	tmpString=EP_NEW TCHAR[length+1];
	retVal=STPrintf_V(tmpString,length+1,format,args);
	va_end(args);
	::OutputDebugString(tmpString);
	if(tmpString)
		EP_DELETE[] tmpString;	
}

unsigned long System::WaitForSingleObject(HANDLE handle,unsigned long milliSec)
{
	return ::WaitForSingleObject(handle,milliSec);
}

HANDLE System::GetCurrentThread()
{
	return ::GetCurrentThread();
}

long System::TerminateThread(HANDLE threadHandle, unsigned long exitCode)
{
	return ::TerminateThread(threadHandle,exitCode);
}