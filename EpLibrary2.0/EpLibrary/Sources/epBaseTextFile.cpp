/*! 
BaseTextFile for the EpLibrary
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
#include "epBaseTextFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace epl;

BaseTextFile::BaseTextFile(FileEncodingType encodingType,LockPolicy lockPolicyType)
{
	m_encodingType=encodingType;
	m_file=NULL;
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_baseTextLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_baseTextLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_baseTextLock=EP_NEW NoLock();
		break;
	default:
		m_baseTextLock=NULL;
		break;
	}
}
BaseTextFile::BaseTextFile(const BaseTextFile& b)
{
	m_encodingType=b.m_encodingType;
	m_file=b.m_file;
	m_lockPolicy=b.m_lockPolicy;
	switch(m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_baseTextLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_baseTextLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_baseTextLock=EP_NEW NoLock();
		break;
	default:
		m_baseTextLock=NULL;
		break;
	}
}
BaseTextFile::~BaseTextFile()
{
	if(m_baseTextLock)
		EP_DELETE m_baseTextLock;
	m_baseTextLock=NULL;
}
BaseTextFile & BaseTextFile::operator=(const BaseTextFile&b)
{
	if(this!=&b)
	{
		if(m_baseTextLock)
			EP_DELETE m_baseTextLock;
		m_baseTextLock=NULL;

		m_encodingType=b.m_encodingType;
		m_file=b.m_file;
		m_lockPolicy=b.m_lockPolicy;
		switch(m_lockPolicy)
		{
		case LOCK_POLICY_CRITICALSECTION:
			m_baseTextLock=EP_NEW CriticalSectionEx();
			break;
		case LOCK_POLICY_MUTEX:
			m_baseTextLock=EP_NEW Mutex();
			break;
		case LOCK_POLICY_NONE:
			m_baseTextLock=EP_NEW NoLock();
			break;
		default:
			m_baseTextLock=NULL;
			break;
		}
	}
	return *this;
}

void BaseTextFile::writeToFile(const TCHAR *toFileString)
{
	unsigned int strLength=System::TcsLen(toFileString);
	if(m_encodingType==FILE_ENCODING_TYPE_ANSI)
	{	
#if defined(_UNICODE) || defined(UNICODE)
		unsigned int sizeNeeded= System::WideCharToMultiByte(toFileString,(char*)NULL);
		char *multiByteToFile = EP_NEW char[sizeNeeded+1];
		System::Memset(multiByteToFile,0,sizeNeeded+1);
		System::WideCharToMultiByte(toFileString,multiByteToFile);
		if(m_file)
			System::FWrite(multiByteToFile,sizeof(char),sizeNeeded,m_file);
		EP_DELETE[] multiByteToFile;
#else// defined(_UNICODE) || defined(UNICODE)
		if(m_file)
			System::FWrite(toFileString,sizeof(char),strLength,m_file);
#endif// defined(_UNICODE) || defined(UNICODE)
	}
	else
	{
#if defined(_UNICODE) || defined(UNICODE)
		if(m_file)
			System::FWrite(toFileString,sizeof(wchar_t),strLength,m_file);
#else //defined(_UNICODE) || defined(UNICODE)
		unsigned int sizeNeeded=System::MultiByteToWideChar(toFileString,(wchar_t*)NULL);
		wchar_t *wideCharToFile = EP_NEW wchar_t[sizeNeeded+1];

		System::Memset(wideCharToFile,0,(sizeNeeded+1)*sizeof(wchar_t));
		System::MultiByteToWideChar(toFileString,wideCharToFile);
		if(m_file)
			System::FWrite(toFileString,sizeof(wchar_t),sizeNeeded,m_file);
		EP_DELETE[] wideCharToFile;
#endif //defined(_UNICODE) || defined(UNICODE)
	}
}

bool BaseTextFile::SaveToFile(const TCHAR *filename)
{
	LockObj lock(m_baseTextLock);
	unsigned int strLength=System::TcsLen(filename);
	if(strLength<=0)
		return false;

	int e;

	Mutex fileLock=Mutex(filename);
	fileLock.Lock();

	if(m_encodingType==FILE_ENCODING_TYPE_UTF8)
		e= System::FTOpen(m_file,filename,_T("wt,ccs=UTF-8"));
	else if(m_encodingType==FILE_ENCODING_TYPE_UTF16LE)
		e= System::FTOpen(m_file,filename,_T("wt,ccs=UTF-16LE"));
	else if(m_encodingType==FILE_ENCODING_TYPE_ANSI)
		e= System::FTOpen(m_file,filename,_T("wt"));
	else
	{
		fileLock.Unlock();
		return false;
	}

	if (e != 0) 
	{
		fileLock.Unlock();
		return false; // failed..
	}

	writeLoop();
	System::FClose(m_file);
	fileLock.Unlock();
	m_file=NULL;
	return true;
}

bool BaseTextFile::AppendToFile(const TCHAR *filename)
{
	LockObj lock(m_baseTextLock);
	unsigned int strLength=System::TcsLen(filename);
	if(strLength<=0)
		return false;

	int e;

	Mutex fileLock=Mutex(filename);
	fileLock.Lock();

	if(m_encodingType==FILE_ENCODING_TYPE_UTF8)
		e= System::FTOpen(m_file,filename,_T("at,ccs=UTF-8"));
	else if(m_encodingType==FILE_ENCODING_TYPE_UTF16LE)
		e= System::FTOpen(m_file,filename,_T("at,ccs=UTF-16LE"));
	else if(m_encodingType==FILE_ENCODING_TYPE_ANSI)
		e= System::FTOpen(m_file,filename,_T("at"));
	else
	{
		fileLock.Unlock();
		return false;
	}

	if (e != 0) 
	{
		fileLock.Unlock();
		return false; // failed..
	}

	writeLoop();
	System::FClose(m_file);
	fileLock.Unlock();
	m_file=NULL;
	return true;
}


bool BaseTextFile::LoadFromFile(const TCHAR *filename)
{
	LockObj lock(m_baseTextLock);
	unsigned int strLength=System::TcsLen(filename);
	if(strLength<=0)
		return false;

	int e;
	Mutex fileLock=Mutex(filename);
	fileLock.Lock();

	if(m_encodingType==FILE_ENCODING_TYPE_UTF8)
		e= System::FTOpen(m_file,filename,_T("rt,ccs=UTF-8"));
	else if(m_encodingType==FILE_ENCODING_TYPE_UTF16LE)
		e= System::FTOpen(m_file,filename,_T("rt,ccs=UTF-16LE"));
	else if(m_encodingType==FILE_ENCODING_TYPE_ANSI)
		e= System::FTOpen(m_file,filename,_T("rt"));
	else
	{
		fileLock.Unlock();
		return false;
	}
	if (e != 0) 
	{
		fileLock.Unlock();
		return false; // failed..
	}
	EpTString rest;

	//Find the actual length of file
	unsigned int length= static_cast<unsigned int>(System::FSize(m_file));
	if(length<=0)
	{
		fileLock.Unlock();
		return false;
	}

#if defined(_UNICODE) || defined(UNICODE)

	if(m_encodingType==FILE_ENCODING_TYPE_ANSI)
	{
		char *cFileBuf=EP_NEW char[length+1];
		//System::Memset(cFileBuf,0,length+1);
		int read=System::FRead(cFileBuf,sizeof(char),length,m_file);
		System::Memset(cFileBuf+read,0,(length+1-read)*sizeof(char));
		System::FClose(m_file);
		rest=System::MultiByteToWideChar(cFileBuf);
		EP_DELETE[] cFileBuf;
	}
	else if(m_encodingType==FILE_ENCODING_TYPE_UTF8)
	{
		wchar_t *wFileBuf=EP_NEW wchar_t[length+1];
		//System::Memset(tFileBuf,0,(length+1)*sizeof(wchar_t));
		int read=System::FRead(wFileBuf,sizeof(wchar_t),length,m_file);
		System::Memset(wFileBuf+read,0,(length+1-read)*sizeof(wchar_t));
		System::FClose(m_file);
		rest=wFileBuf;
		EP_DELETE[] wFileBuf;

	}
	else
	{
		wchar_t *wFileBuf=EP_NEW wchar_t[length/sizeof(wchar_t)+1];
		//System::Memset(wFileBuf,0,length/sizeof(wchar_t)+1);
		int read=System::FRead(wFileBuf,sizeof(wchar_t),length/sizeof(wchar_t),m_file);
		System::Memset(wFileBuf+read,0,((length/sizeof(wchar_t)+1)-read)*sizeof(wchar_t));
		System::FClose(m_file);
		if(wFileBuf[0]==0xFEFF) // BOM
			rest=wFileBuf+1;
		else
			rest=wFileBuf;
		EP_DELETE[] wFileBuf;
	}
	
	
#else //defined(_UNICODE) || defined(UNICODE)


	if(m_encodingType==FILE_ENCODING_TYPE_ANSI)
	{

		char *cFileBuf=EP_NEW char[length+1];
		//System::Memset(cFileBuf,0,length+1);
		int read=System::FRead(cFileBuf,sizeof(char),length,m_file);
		System::Memset(cFileBuf+read,0,(length+1-read)*sizeof(char));
		System::FClose(m_file);
		rest=cFileBuf;
		EP_DELETE[] cFileBuf;
	}
	else if(m_encodingType==FILE_ENCODING_TYPE_UTF8)
	{
		wchar_t *wFileBuf=EP_NEW wchar_t[length+1];
		//System::Memset(wFileBuf,0,(length+1)*sizeof(wchar_t));
		int read=System::FRead(wFileBuf,sizeof(wchar_t),length,m_file);
		System::Memset(wFileBuf+read,0,(length+1-read)*sizeof(wchar_t));
		System::FClose(m_file);
		rest=System::WideCharToMultiByte(wFileBuf);;
		EP_DELETE[] wFileBuf;
	}
	else
	{
		wchar_t *wFileBuf=EP_NEW wchar_t[length/sizeof(wchar_t)+1];
		//System::Memset(wFileBuf,0,length/sizeof(wchar_t)+1);
		int read=System::FRead(wFileBuf,sizeof(wchar_t),length/sizeof(wchar_t),m_file);
		System::Memset(wFileBuf+read,0,((length/sizeof(wchar_t)+1)-read)*sizeof(wchar_t));
		System::FClose(m_file);
		if(wFileBuf[0]==0xFEFF) // BOM
			rest=System::WideCharToMultiByte(wFileBuf+1);
		else
			rest=System::WideCharToMultiByte(wFileBuf);
		EP_DELETE[] wFileBuf;
		
	}
	
	
#endif //defined(_UNICODE) || defined(UNICODE)
	
	fileLock.Unlock();
	loadFromFile(rest);

	m_file=NULL;
	return true;

// 	if(m_encodingType==FILE_ENCODING_TYPE_UTF8||m_encodingType==FILE_ENCODING_TYPE_UTF16LE)
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


bool BaseTextFile::GetLine(const EpTString &buf,int startIdx, EpTString &retLine, int *retEndIdx, EpTString *retRest)
{
	if(((int)buf.length())-startIdx<=0)
		return false;
	TCHAR splitChar=0;
	EpTString lineSTring=_T("");
	int bufTrav=startIdx;

	do{
		splitChar=buf.at(bufTrav);
		if(splitChar!=_T('\r') && splitChar!=_T('\n') && splitChar!=_T('\0'))
		{
			lineSTring.append(&splitChar,1);
		}
		bufTrav++;
	}while(splitChar!=_T('\n') && splitChar!=_T('\0') && bufTrav<(int)buf.length());


	if(retEndIdx)
	{
		if(bufTrav<((int)buf.length()))
			*retEndIdx=bufTrav;
		else
			*retEndIdx=buf.length();
	}

	if(retRest)
	{
		*retRest=buf;
		if(bufTrav<(int)retRest->length())
			retRest->erase(0,bufTrav);
		else
			retRest->erase(0,retRest->length());
	}
	retLine=lineSTring;
	return true;
}
