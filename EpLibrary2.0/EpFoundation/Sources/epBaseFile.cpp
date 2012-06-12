/*! 
BaseFile for the EpLibrary
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
#include "epBaseFile.h"

using namespace epl;

BaseFile::BaseFile(FileEncodingType encodingType,LockPolicy lockPolicyType)
{
	m_encodingType=encodingType;
	m_file=NULL;
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_lock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_lock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_lock=EP_NEW NoLock();
		break;
	default:
		m_lock=NULL;
		break;
	}
}
BaseFile::BaseFile(const BaseFile& b)
{
	m_encodingType=b.m_encodingType;
	m_file=b.m_file;
	m_lockPolicy=b.m_lockPolicy;
	switch(m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_lock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_lock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_lock=EP_NEW NoLock();
		break;
	default:
		m_lock=NULL;
		break;
	}
}
BaseFile::~BaseFile()
{
	if(m_lock)
		EP_DELETE m_lock;
}


void BaseFile::writeToFile(const TCHAR *toFileString)
{
	unsigned int strLength=System::TcsLen(toFileString);
	if(m_encodingType==FILE_ENCODING_TYPE_UTF8)//||m_encodingType==FILE_ENCODING_TYPE_UTF16)
	{	
#if defined(_UNICODE) || defined(UNICODE)
		char *multiByteToFile = EP_NEW char[strLength+1];

		System::Memset(multiByteToFile,0,strLength+1);
		System::UTF16ToUTF8(toFileString,multiByteToFile);
		if(m_file)
			System::FWrite(multiByteToFile,sizeof(char),strLength,m_file);
		EP_DELETE[] multiByteToFile;
#else// defined(_UNICODE) || defined(UNICODE)
		System::FWrite(toFileString,sizeof(char),strLength,m_file);
#endif// defined(_UNICODE) || defined(UNICODE)
	}
	else
	{
#if defined(_UNICODE) || defined(UNICODE)
		if(m_file)
			System::FWrite(toFileString,sizeof(wchar_t),strLength,m_file);
#else //defined(_UNICODE) || defined(UNICODE)
		wchar_t *wideCharToFile = EP_NEW wchar_t[strLength+1];

		System::Memset(wideCharToFile,0,(strLength+1)*sizeof(wchar_t));
		System::MultiByteToWideChar(toFileString,wideCharToFile);
		if(m_file)
			System::FWrite(toFileString,sizeof(wchar_t),strLength,m_file);
		EP_DELETE[] wideCharToFile;
#endif //defined(_UNICODE) || defined(UNICODE)
	}
}

bool BaseFile::SaveToFile(const TCHAR *filename)
{
	LockObj lock(m_lock);
	unsigned int strLength=System::TcsLen(filename);
	if(strLength<=0)
		return false;

	int e;

	if(m_encodingType==FILE_ENCODING_TYPE_UTF8)
		e= System::FTOpen(m_file,filename,_T("wt,ccs=UTF-8"));
// 	else if(m_encodingType==FILE_ENCODING_TYPE_UNICODE)
// 		e= System::FTOpen(m_file,filename,_T("wt,ccs=UNICODE"));
	else if(m_encodingType==FILE_ENCODING_TYPE_UTF16 || m_encodingType==FILE_ENCODING_TYPE_UNICODE)
		e= System::FTOpen(m_file,filename,_T("wt,ccs=UTF-16LE"));
	else
		return false;

	if (e != 0) 
		return false; // failed..

	writeLoop();
	System::FClose(m_file);
	m_file=NULL;
	return true;
}


bool BaseFile::LoadFromFile(const TCHAR *filename)
{
	LockObj lock(m_lock);
	unsigned int strLength=System::TcsLen(filename);
	if(strLength<=0)
		return false;

	int e;

	if(m_encodingType==FILE_ENCODING_TYPE_UTF8)
		e= System::FTOpen(m_file,filename,_T("rt,ccs=UTF-8"));
	else if(m_encodingType==FILE_ENCODING_TYPE_UNICODE)
		e= System::FTOpen(m_file,filename,_T("rt,ccs=UNICODE"));
	else if(m_encodingType==FILE_ENCODING_TYPE_UTF16)
		e= System::FTOpen(m_file,filename,_T("rt,ccs=UTF-16LE"));
	else
		return false;

	if (e != 0) 
		return false; // failed..
	EpTString rest;

#if defined(_UNICODE) || defined(UNICODE)
		//Find the actual length of file
		unsigned int length= static_cast<unsigned int>(System::FSize(m_file));

		char *cFileBuf=EP_NEW char[length+2];
		System::Memset(cFileBuf,0,length+2);
		System::FRead(cFileBuf,sizeof(char),length,m_file);
		System::FClose(m_file);

		if(m_encodingType==FILE_ENCODING_TYPE_UTF16)
		{
			rest=reinterpret_cast<wchar_t*>(cFileBuf);
		}
		else
		{
			wchar_t *tFileBuf=EP_NEW wchar_t[length+1];
			System::Memset(tFileBuf,0,sizeof(wchar_t)*(length+1));
			System::MultiByteToWideChar(cFileBuf,length,tFileBuf);
			rest=tFileBuf;
			EP_DELETE[] tFileBuf;
		}
		EP_DELETE[] cFileBuf;
		
#else //defined(_UNICODE) || defined(UNICODE)
		//Find the actual length of file
		unsigned int length= static_cast<unsigned int>(System::FSize(m_file));

		char *cFileBuf=EP_NEW char[length+2];
		System::Memset(cFileBuf,0,length+2);
		System::FRead(cFileBuf,sizeof(char),length,m_file);
		System::FClose(m_file);

		if(m_encodingType==FILE_ENCODING_TYPE_UTF16)
		{
			EpString resultString=System::WideCharToMultiByte(reinterpret_cast<wchar_t*>(cFileBuf));
			rest=resultString.c_str();
		}
		else
		{
			rest=cFileBuf;
		}
		
		EP_DELETE[] cFileBuf;
#endif //defined(_UNICODE) || defined(UNICODE)

		loadFromFile(rest);

	m_file=NULL;
	return true;

// 	if(m_encodingType==FILE_ENCODING_TYPE_UTF8||m_encodingType==FILE_ENCODING_TYPE_UTF16)
// 	{	
// #if defined(_UNICODE) || defined(UNICODE)
// 		//Find the actual length of file
// 		unsigned int length= static_cast<unsigned int>(System::FSize(m_file));
// 
// 		char *cFileBuf=EP_NEW char[length+1];
// 		System::Memset(cFileBuf,0,length+1);
// 		System::FRead(cFileBuf,sizeof(char),length,m_file);
// 		System::FClose(m_file);
// 
// 		wchar_t *tFileBuf=EP_NEW wchar_t[length+1];
// 		System::Memset(tFileBuf,0,sizeof(wchar_t)*(length+1));
// 		System::MultiByteToWideChar(cFileBuf,length,tFileBuf);
// 		rest=tFileBuf;
// 		EP_DELETE[] cFileBuf;
// 		EP_DELETE[] tFileBuf;
// #else //defined(_UNICODE) || defined(UNICODE)
// 		//Find the actual length of file
// 		unsigned int length= static_cast<unsigned int>(System::FSize(m_file));
// 
// 		char *cFileBuf=EP_NEW char[length+1];
// 		System::Memset(cFileBuf,0,length+1);
// 		System::FRead(cFileBuf,sizeof(char),length,m_file);
// 		System::FClose(m_file);
// 
// 		rest=cFileBuf;
// 		EP_DELETE[] cFileBuf;
// #endif //defined(_UNICODE) || defined(UNICODE)
// 	}
// 	else
// 	{
// #if defined(_UNICODE) || defined(UNICODE)
// 		//Find the actual length of file
// 		unsigned int length=static_cast<unsigned int>( System::FSize(m_file)/sizeof(wchar_t) );
// 
// 		wchar_t *tFileBuf=EP_NEW wchar_t[length+1];
// 		System::Memset(tFileBuf,0,sizeof(wchar_t)*(length+1));
// 		System::FRead(tFileBuf,sizeof(wchar_t),length,m_file);
// 		System::FClose(m_file);
// 
// 		rest=tFileBuf;
// 		EP_DELETE[] tFileBuf;
// #else //defined(_UNICODE) || defined(UNICODE)
// 		//Find the actual length of file
// 		unsigned int length=static_cast<unsigned int>( System::FSize(m_file)/sizeof(wchar_t) );
// 
// 		wchar_t *tFileBuf=EP_NEW wchar_t[length+1];
// 		System::Memset(tFileBuf,0,sizeof(wchar_t)*(length+1));
// 		System::FRead(tFileBuf,sizeof(TCHAR),length,m_file);
// 		System::FClose(m_file);
// 
// 		char *cFileBuf=EP_NEW char[length+1];
// 		System::Memset(cFileBuf,0,sizeof(char)*(length+1));
// 		System::WideCharToMultiByte(tFileBuf,length,cFileBuf);
// 		rest=cFileBuf;
// 		EP_DELETE[] cFileBuf;
// 		EP_DELETE[] tFileBuf;
// #endif //defined(_UNICODE) || defined(UNICODE)
// 	}
// 
// 
// 	loadFromFile(rest);
// 	
// 	m_file=NULL;
// 	return true;
}


bool BaseFile::getLine(EpTString buf, EpTString &retLine, EpTString &retRest)
{
	if(buf.length()<=0)
		return false;
	TCHAR splitChar=0;
	EpTString lineSTring=_T("");
	int bufTrav=0;

	do{
		splitChar=buf.at(bufTrav);
		if(splitChar!=_T('\r') && splitChar!=_T('\n') && splitChar!=_T('\0'))
		{
			lineSTring.append(&splitChar,1);
		}
		bufTrav++;
	}while(splitChar!=_T('\n') && splitChar!=_T('\0') && bufTrav<buf.length());
	if(bufTrav<buf.length())
		buf.erase(0,bufTrav);
	else
		buf.erase(0,buf.length());
	retRest=buf;
	retLine=lineSTring;
	return true;
}

