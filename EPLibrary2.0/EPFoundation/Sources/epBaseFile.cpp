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

BaseFile::BaseFile(FileEncodingType encodingType)
{
	m_encodingType=encodingType;
	m_file=NULL;
}

BaseFile::~BaseFile()
{
}


void BaseFile::WriteToFile(CString toFileString)
{
	if(m_encodingType==FILE_ENCODING_TYPE_UTF8||m_encodingType==FILE_ENCODING_TYPE_UTF16)
	{	

		char *multiByteToFile = EP_NEW char[toFileString.GetLength()+1];

		memset(multiByteToFile,0,toFileString.GetLength()+1);
		wcstombs(multiByteToFile,toFileString.GetString(),toFileString.GetLength());

		if(m_file)
			m_file->Write(multiByteToFile,toFileString.GetLength());
		EP_DELETE[] multiByteToFile;
	}
	else
	{
		if(m_file)
			m_file->Write(toFileString,(toFileString.GetLength())*sizeof(TCHAR));
	}
}

bool BaseFile::SaveToFile(CString filename)
{

	if(filename.GetLength()<=0)
		return false;

	FILE *fStream;
	errno_t e;

	if(m_encodingType==FILE_ENCODING_TYPE_UTF8)
		e= _tfopen_s(&fStream,filename.GetString(),_T("wt,ccs=UTF-8"));
	else if(m_encodingType==FILE_ENCODING_TYPE_UNICODE)
		e= _tfopen_s(&fStream,filename.GetString(),_T("wt,ccs=UNICODE"));
	else if(m_encodingType==FILE_ENCODING_TYPE_UTF16)
		e= _tfopen_s(&fStream,filename.GetString(),_T("wt,ccs=UTF-16LE"));
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


bool BaseFile::LoadFromFile(CString filename)
{
	if(filename.GetLength()<=0)
		return false;

	FILE *fStream;
	errno_t e;

	if(m_encodingType==FILE_ENCODING_TYPE_UTF8)
		e= _tfopen_s(&fStream,filename.GetString(),_T("rt,ccs=UTF-8"));
	else if(m_encodingType==FILE_ENCODING_TYPE_UNICODE)
		e= _tfopen_s(&fStream,filename.GetString(),_T("rt,ccs=UNICODE"));
	else if(m_encodingType==FILE_ENCODING_TYPE_UTF16)
		e= _tfopen_s(&fStream,filename.GetString(),_T("rt,ccs=UTF-16LE"));
	else
		return false;

	if (e != 0) 
		return false; // failed..
	CStdioFile propertyFile(fStream);  // open the file from this stream
	m_file=&propertyFile;

	CString rest;
	if(m_encodingType==FILE_ENCODING_TYPE_UTF8||m_encodingType==FILE_ENCODING_TYPE_UTF16)
	{		
		//Find the actual length of file
		unsigned int length= (unsigned int)propertyFile.SeekToEnd()+1;
		propertyFile.SeekToBegin();

		char *cFileBuf=EP_NEW char[length];
		memset(cFileBuf,0,length);
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
		unsigned int length=(unsigned int)propertyFile.GetLength();

		TCHAR *tFileBuf=EP_NEW TCHAR[length];
		memset(tFileBuf,0,sizeof(TCHAR)*length);
		propertyFile.Read(tFileBuf,length);
		propertyFile.Close();

		rest=tFileBuf;
		EP_DELETE[] tFileBuf;
	}


	loadFromFile(rest);
	
	m_file=NULL;
	return true;
}


bool BaseFile::getLine(CString buf, CString &retLine, CString &retRest)
{
	if(buf.GetLength()<=0)
		return false;
	TCHAR splitChar=0;
	CString lineSTring=_T("");
	int bufTrav=0;

	do{
		splitChar=buf.GetAt(bufTrav);
		if(splitChar!=_T('\r') && splitChar!=_T('\n') && splitChar!=_T('\0'))
		{
			lineSTring.AppendChar(splitChar);
		}
		bufTrav++;
	}while(splitChar!=_T('\n') && splitChar!=_T('\0') && bufTrav<buf.GetLength());
	if(bufTrav<buf.GetLength())
		buf.Delete(0,bufTrav);
	else
		buf.Delete(0,buf.GetLength());
	retRest=buf;
	retLine=lineSTring;
	return true;
}

