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


void BaseFile::WriteToFile(const TCHAR *toFileString)
{
	LockObj lock(m_lock);
	unsigned int strLength=System::TcsLen(toFileString);
	if(m_encodingType==FILE_ENCODING_TYPE_UTF8||m_encodingType==FILE_ENCODING_TYPE_UTF16)
	{	
		char *multiByteToFile = EP_NEW char[strLength+1];
		
		System::Memset(multiByteToFile,0,strLength+1);
		System::WideCharToMultiByte(toFileString,multiByteToFile);

		if(m_file)
			m_file->Write(multiByteToFile,strLength);
		EP_DELETE[] multiByteToFile;
	}
	else
	{
		if(m_file)
			m_file->Write(toFileString,(strLength)*sizeof(TCHAR));
	}
}

bool BaseFile::SaveToFile(const TCHAR *filename)
{
	LockObj lock(m_lock);
	unsigned int strLength=System::TcsLen(filename);
	if(strLength<=0)
		return false;

	FILE *fStream;
	int e;

	if(m_encodingType==FILE_ENCODING_TYPE_UTF8)
		e= System::TFOpen(fStream,filename,_T("wt,ccs=UTF-8"));
	else if(m_encodingType==FILE_ENCODING_TYPE_UNICODE)
		e= System::TFOpen(fStream,filename,_T("wt,ccs=UNICODE"));
	else if(m_encodingType==FILE_ENCODING_TYPE_UTF16)
		e= System::TFOpen(fStream,filename,_T("wt,ccs=UTF-16LE"));
	else
		return false;
	if (e != 0) 
		return false; // failed..
	CStdioFile propertyFile(fStream);  // open the file from this stream
	m_file=&propertyFile;

	writeLoop();

	propertyFile.Close();
	m_file=NULL;
	return true;
}


bool BaseFile::LoadFromFile(const TCHAR *filename)
{
	LockObj lock(m_lock);
	unsigned int strLength=System::TcsLen(filename);
	if(strLength<=0)
		return false;

	FILE *fStream;
	int e;

	if(m_encodingType==FILE_ENCODING_TYPE_UTF8)
		e= System::TFOpen(fStream,filename,_T("rt,ccs=UTF-8"));
	else if(m_encodingType==FILE_ENCODING_TYPE_UNICODE)
		e= System::TFOpen(fStream,filename,_T("rt,ccs=UNICODE"));
	else if(m_encodingType==FILE_ENCODING_TYPE_UTF16)
		e= System::TFOpen(fStream,filename,_T("rt,ccs=UTF-16LE"));
	else
		return false;

	if (e != 0) 
		return false; // failed..
	CStdioFile propertyFile(fStream);  // open the file from this stream
	m_file=&propertyFile;

	EpTString rest;
	if(m_encodingType==FILE_ENCODING_TYPE_UTF8||m_encodingType==FILE_ENCODING_TYPE_UTF16)
	{		
		//Find the actual length of file
		unsigned int length= static_cast<unsigned int>(propertyFile.SeekToEnd()+1);
		propertyFile.SeekToBegin();

		char *cFileBuf=EP_NEW char[length];
		System::Memset(cFileBuf,0,length);
		propertyFile.Read(cFileBuf,length);
		propertyFile.Close();

		TCHAR *tFileBuf=EP_NEW TCHAR[length];
		MultiByteToWideChar(CP_ACP, 0, cFileBuf,-1,tFileBuf,length);

		rest=tFileBuf;
		EP_DELETE[] cFileBuf;
		EP_DELETE[] tFileBuf;
	}
	else
	{
		//Find the actual length of file
		unsigned int length=static_cast<unsigned int>( propertyFile.GetLength() );

		TCHAR *tFileBuf=EP_NEW TCHAR[length];
		System::Memset(tFileBuf,0,sizeof(TCHAR)*length);
		propertyFile.Read(tFileBuf,length);
		propertyFile.Close();

		rest=tFileBuf;
		EP_DELETE[] tFileBuf;
	}


	loadFromFile(rest);
	
	m_file=NULL;
	return true;
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

