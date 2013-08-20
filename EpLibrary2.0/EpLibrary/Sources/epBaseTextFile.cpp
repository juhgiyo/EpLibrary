/*! 
BaseTextFile for the EpLibrary

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
#include "epBaseTextFile.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

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
	size_t strLength=System::TcsLen(toFileString);
	if(m_encodingType==FILE_ENCODING_TYPE_ANSI)
	{	
#if defined(_UNICODE) || defined(UNICODE)
		size_t sizeNeeded= System::WideCharToMultiByte(toFileString,(char*)NULL);
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
		size_t sizeNeeded=System::MultiByteToWideChar(toFileString,(wchar_t*)NULL);
		wchar_t *wideCharToFile = EP_NEW wchar_t[sizeNeeded+1];

		System::Memset(wideCharToFile,0,(sizeNeeded+1)*sizeof(wchar_t));
		System::MultiByteToWideChar(toFileString,wideCharToFile);
		if(m_file)
			System::FWrite(toFileString,sizeof(wchar_t),sizeNeeded,m_file);
		EP_DELETE[] wideCharToFile;
#endif //defined(_UNICODE) || defined(UNICODE)
	}
}
FileEncodingType BaseTextFile::GetEncodingType()
{
	LockObj lock(m_baseTextLock);
	return m_encodingType;
}
void BaseTextFile::SetEncodingType(FileEncodingType encodingType)
{
	LockObj lock(m_baseTextLock);
	m_encodingType=encodingType;
}

bool BaseTextFile::SaveToFile(const TCHAR *filename)
{
	LockObj lock(m_baseTextLock);
	size_t strLength=System::TcsLen(filename);
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
	size_t strLength=System::TcsLen(filename);
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
	size_t strLength=System::TcsLen(filename);
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
	size_t length= System::FSize(m_file);
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
		size_t read=System::FRead(cFileBuf,sizeof(char),length,m_file);
		System::Memset(cFileBuf+read,0,(length+1-read)*sizeof(char));
		System::FClose(m_file);
		rest=System::MultiByteToWideChar(cFileBuf);
		EP_DELETE[] cFileBuf;
	}
	else if(m_encodingType==FILE_ENCODING_TYPE_UTF8)
	{
		wchar_t *wFileBuf=EP_NEW wchar_t[length+1];
		//System::Memset(tFileBuf,0,(length+1)*sizeof(wchar_t));
		size_t read=System::FRead(wFileBuf,sizeof(wchar_t),length,m_file);
		System::Memset(wFileBuf+read,0,(length+1-read)*sizeof(wchar_t));
		System::FClose(m_file);
		int stringSize=length+1;
		int stringTrav=0;
		for(;stringTrav<stringSize;stringTrav++)
		{
			if(wFileBuf[stringTrav]!=0xFEFF) // BOM
				break;
		}
		rest=wFileBuf+stringTrav;
		EP_DELETE[] wFileBuf;

	}
	else
	{
		wchar_t *wFileBuf=EP_NEW wchar_t[length/sizeof(wchar_t)+1];
		//System::Memset(wFileBuf,0,length/sizeof(wchar_t)+1);
		size_t read=System::FRead(wFileBuf,sizeof(wchar_t),length/sizeof(wchar_t),m_file);
		System::Memset(wFileBuf+read,0,((length/sizeof(wchar_t)+1)-read)*sizeof(wchar_t));
		System::FClose(m_file);
		int stringSize=length/sizeof(wchar_t)+1;
		int stringTrav=0;
		for(;stringTrav<stringSize;stringTrav++)
		{
			if(wFileBuf[stringTrav]!=0xFEFF) // BOM
				break;
		}
		rest=wFileBuf+stringTrav;
		
		EP_DELETE[] wFileBuf;
	}
	
	
#else //defined(_UNICODE) || defined(UNICODE)


	if(m_encodingType==FILE_ENCODING_TYPE_ANSI)
	{

		char *cFileBuf=EP_NEW char[length+1];
		//System::Memset(cFileBuf,0,length+1);
		size_t read=System::FRead(cFileBuf,sizeof(char),length,m_file);
		System::Memset(cFileBuf+read,0,(length+1-read)*sizeof(char));
		System::FClose(m_file);
		rest=cFileBuf;
		EP_DELETE[] cFileBuf;
	}
	else if(m_encodingType==FILE_ENCODING_TYPE_UTF8)
	{
		wchar_t *wFileBuf=EP_NEW wchar_t[length+1];
		//System::Memset(wFileBuf,0,(length+1)*sizeof(wchar_t));
		size_t read=System::FRead(wFileBuf,sizeof(wchar_t),length,m_file);
		System::Memset(wFileBuf+read,0,(length+1-read)*sizeof(wchar_t));
		System::FClose(m_file);


		int stringSize=length+1;
		int stringTrav=0;
		for(;stringTrav<stringSize;stringTrav++)
		{
			if(wFileBuf[stringTrav]!=0xFEFF) // BOM
				break;
		}

		rest=System::WideCharToMultiByte(wFileBuf+stringTrav);
		EP_DELETE[] wFileBuf;
	}
	else
	{
		wchar_t *wFileBuf=EP_NEW wchar_t[length/sizeof(wchar_t)+1];
		//System::Memset(wFileBuf,0,length/sizeof(wchar_t)+1);
		size_t read=System::FRead(wFileBuf,sizeof(wchar_t),length/sizeof(wchar_t),m_file);
		System::Memset(wFileBuf+read,0,((length/sizeof(wchar_t)+1)-read)*sizeof(wchar_t));
		System::FClose(m_file);

		int stringSize=length/sizeof(wchar_t)+1;
		int stringTrav=0;
		for(;stringTrav<stringSize;stringTrav++)
		{
			if(wFileBuf[stringTrav]!=0xFEFF) // BOM
				break;
		}
		rest=System::WideCharToMultiByte(wFileBuf+stringTrav);

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
// 		size_t length= System::FSize(m_file);
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
// 		size_t length= System::FSize(m_file);
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
// 		size_t length= System::FSize(m_file)/sizeof(wchar_t) ;
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
// 		size_t length=System::FSize(m_file)/sizeof(wchar_t) ;
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


bool BaseTextFile::GetLine(const EpTString &buf,size_t startIdx, EpTString &retLine, size_t *retEndIdx, EpTString *retRest)
{
	if(static_cast<ssize_t>(buf.length())-static_cast<ssize_t>(startIdx)<=0)
		return false;
	TCHAR splitChar=0;
	EpTString lineSTring=_T("");
	ssize_t bufTrav=static_cast<ssize_t>(startIdx);

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
