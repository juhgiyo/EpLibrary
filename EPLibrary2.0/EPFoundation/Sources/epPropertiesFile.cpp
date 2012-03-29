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

PropertiesFile::PropertiesFile(FileEncodingType encodingType, LockPolicy lockPolicyType):BaseFile(encodingType,lockPolicyType)
{

}

PropertiesFile::PropertiesFile(const PropertiesFile& b):BaseFile(b)
{
	m_propertyList=b.m_propertyList;
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
