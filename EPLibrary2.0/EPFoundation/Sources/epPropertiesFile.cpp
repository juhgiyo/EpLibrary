/*! 
PropertiesFile for the EpLibrary
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
#include "epPropertiesFile.h"

using namespace epl;
#if TEST_NEW
PropertiesFile::PropertiesFile(FileEncodingType encodingType):BaseFile(encodingType)
{

}

PropertiesFile::~PropertiesFile()
{
}

bool PropertiesFile::SetProperty(CString key, CString val)
{
	key.Append(_T("="));
	list<pair<CString, CString> >::iterator iter;
	for(iter=m_propertyList.begin();iter != m_propertyList.end();iter++)
	{
		if(iter->first.Compare(key)==0)
		{
			iter->second=val;
			return true;
		}
	}
	return false;
}

bool PropertiesFile::GetProperty(CString key,CString &retVal)
{
	key.Append(_T("="));
	list<pair<CString, CString> >::iterator iter;
	for(iter=m_propertyList.begin();iter != m_propertyList.end();iter++)
	{
		if(iter->first.Compare(key)==0)
		{
			retVal=iter->second;
			return true;
		}
	}
	return false;
}

bool PropertiesFile::AddProperty(CString key, CString val)
{
	key.Append(_T("="));
	list<pair<CString, CString> >::iterator iter;
	for(iter=m_propertyList.begin();iter != m_propertyList.end();iter++)
	{
		if(iter->first.Compare(key)==0)
		{
			return false;
		}
	}
	pair<CString,CString> insertPair;
	insertPair.first=key;
	insertPair.second=val;
	m_propertyList.push_back(insertPair);
	return true;
}

bool PropertiesFile::RemoveProperty(CString key)
{
	key.Append(_T("="));
	list<pair<CString, CString> >::iterator iter;
	for(iter=m_propertyList.begin();iter != m_propertyList.end();iter++)
	{
		if(iter->first.Compare(key)==0)
		{
			m_propertyList.erase(iter);
			return true;
		}
	}
	return false;
}

void PropertiesFile::Clear()
{
	m_propertyList.clear();
}
void PropertiesFile::writeLoop()
{
	list<pair<CString,CString> >::iterator iter;
	CString toFileString;
	for(iter=m_propertyList.begin();iter!=m_propertyList.end();iter++)
	{
		toFileString=_T("");
		toFileString.Append(iter->first);
		toFileString.Append(iter->second);
		toFileString.Append(_T("\r\n"));
		WriteToFile(toFileString);
	}

}
void PropertiesFile::loadFromFile(CString rest)
{
	CString line=_T("");
	while(getLine(rest,line,rest))
	{
		CString key;
		CString val;
		if(getValueKeyFromLine(line,key,val))
		{
			pair<CString,CString> inputPair;
			inputPair.first=key;
			inputPair.second=val;
			m_propertyList.push_back(inputPair);
		}
		else
		{
			pair<CString,CString> inputPair;
			inputPair.first=line;
			inputPair.second=_T("");
			m_propertyList.push_back(inputPair);
		}
	}
	
}


bool PropertiesFile::getValueKeyFromLine(CString buf, CString &retKey, CString &retVal)
{
	TCHAR splitChar=0;
	int bufTrav=0;
	if(buf.GetLength()<=0)
		return false;

	retKey=_T("");
	retVal=_T("");

	for(int testTrav=0; testTrav<buf.GetLength();testTrav++)
	{
		if(buf.GetAt(testTrav)==_T('#'))
			return false;
	}

	while(splitChar!=_T('=') && bufTrav<buf.GetLength())
	{
		splitChar=buf.GetAt(bufTrav);
		retKey.AppendChar(splitChar);
		bufTrav++;
	}
	buf.Delete(0,bufTrav);
	retVal=buf;
	return true;
}
#else //TEST_NEW
PropertiesFile::PropertiesFile(FileEncodingType encodingType)
{
	m_encodingType=encodingType;
}

PropertiesFile::~PropertiesFile()
{
}

bool PropertiesFile::SetProperty(CString key, CString val)
{
	key.Append(_T("="));
	list<pair<CString, CString> >::iterator iter;
	for(iter=m_propertyList.begin();iter != m_propertyList.end();iter++)
	{
		if(iter->first.Compare(key)==0)
		{
			iter->second=val;
			return true;
		}
	}
	return false;
}

bool PropertiesFile::GetProperty(CString key,CString &retVal)
{
	key.Append(_T("="));
	list<pair<CString, CString> >::iterator iter;
	for(iter=m_propertyList.begin();iter != m_propertyList.end();iter++)
	{
		if(iter->first.Compare(key)==0)
		{
			retVal=iter->second;
			return true;
		}
	}
	return false;
}

bool PropertiesFile::AddProperty(CString key, CString val)
{
	key.Append(_T("="));
	list<pair<CString, CString> >::iterator iter;
	for(iter=m_propertyList.begin();iter != m_propertyList.end();iter++)
	{
		if(iter->first.Compare(key)==0)
		{
			return false;
		}
	}
	pair<CString,CString> insertPair;
	insertPair.first=key;
	insertPair.second=val;
	m_propertyList.push_back(insertPair);
	return true;
}

bool PropertiesFile::RemoveProperty(CString key)
{
	key.Append(_T("="));
	list<pair<CString, CString> >::iterator iter;
	for(iter=m_propertyList.begin();iter != m_propertyList.end();iter++)
	{
		if(iter->first.Compare(key)==0)
		{
			m_propertyList.erase(iter);
			return true;
		}
	}
	return false;
}

void PropertiesFile::Clear()
{
	m_propertyList.clear();
}

bool PropertiesFile::SaveToFile(CString filename)
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


	if(m_encodingType==FILE_ENCODING_TYPE_UTF8||m_encodingType==FILE_ENCODING_TYPE_UTF16)
	{	
		CString toFileString;

		list<pair<CString,CString> >::iterator iter;
		char *multiByteToFile = EP_NEW char[toFileString.GetLength()+1];

		for(iter=m_propertyList.begin();iter!=m_propertyList.end();iter++)
		{
			toFileString=_T("");
			toFileString.Append(iter->first);
			toFileString.Append(iter->second);
			toFileString.Append(_T("\r\n"));

			memset(multiByteToFile,0,toFileString.GetLength()+1);
			wcstombs(multiByteToFile,toFileString.GetString(),toFileString.GetLength());

			propertyFile.Write(multiByteToFile,toFileString.GetLength());
		}

		EP_DELETE[] multiByteToFile;

	}
	else
	{
		CString toFileString;

		list<pair<CString,CString> >::iterator iter;
		for(iter=m_propertyList.begin();iter!=m_propertyList.end();iter++)
		{
			toFileString=_T("");
			toFileString.Append(iter->first);
			toFileString.Append(iter->second);
			toFileString.Append(_T("\r\n"));
			propertyFile.Write(toFileString,(toFileString.GetLength())*sizeof(TCHAR));
		}
	}

	propertyFile.Close();
	return true;
}


bool PropertiesFile::LoadFromFile(CString filename)
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


	CString line=_T("");
	while(getLine(rest,line,rest))
	{
		CString key;
		CString val;
		if(getValueKeyFromLine(line,key,val))
		{
			pair<CString,CString> inputPair;
			inputPair.first=key;
			inputPair.second=val;
			m_propertyList.push_back(inputPair);
		}
		else
		{
			pair<CString,CString> inputPair;
			inputPair.first=line;
			inputPair.second=_T("");
			m_propertyList.push_back(inputPair);
		}
	}
	return true;
}


bool PropertiesFile::getLine(CString buf, CString &retLine, CString &retRest)
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

bool PropertiesFile::getValueKeyFromLine(CString buf, CString &retKey, CString &retVal)
{
	TCHAR splitChar=0;
	int bufTrav=0;
	if(buf.GetLength()<=0)
		return false;

	retKey=_T("");
	retVal=_T("");

	for(int testTrav=0; testTrav<buf.GetLength();testTrav++)
	{
		if(buf.GetAt(testTrav)==_T('#'))
			return false;
	}

	while(splitChar!=_T('=') && bufTrav<buf.GetLength())
	{
		splitChar=buf.GetAt(bufTrav);
		retKey.AppendChar(splitChar);
		bufTrav++;
	}
	buf.Delete(0,bufTrav);
	retVal=buf;
	return true;
}

#endif //TEST_NEW