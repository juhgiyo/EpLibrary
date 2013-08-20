/*! 
PropertiesFile for the EpLibrary

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
#include "epPropertiesFile.h"
#include "epException.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;

PropertiesFile::PropertiesFile(FileEncodingType encodingType, LockPolicy lockPolicyType):BaseTextFile(encodingType,lockPolicyType)
{
	m_nullString=_T("");
}

PropertiesFile::PropertiesFile(const PropertiesFile& b):BaseTextFile(b)
{
	m_nullString=_T("");
	LockObj lock(b.m_baseTextLock);
	m_propertyList=b.m_propertyList;
}

PropertiesFile::~PropertiesFile()
{
}
PropertiesFile & PropertiesFile::operator=(const PropertiesFile&b)
{
	if(this!=&b)
	{
		BaseTextFile::operator =(b);
		m_nullString=_T("");
		LockObj lock(b.m_baseTextLock);
		m_propertyList=b.m_propertyList;

	}
	return *this;
}

bool PropertiesFile::SetProperty(const TCHAR *  key, const TCHAR * val)
{
	LockObj lock(m_baseTextLock);
	EpTString opKey=Locale::Trim(key);
	opKey.append(_T("="));
	vector<Pair<EpTString, EpTString> >::iterator iter;
	for(iter=m_propertyList.begin();iter != m_propertyList.end();iter++)
	{
		if(iter->first.compare(opKey)==0)
		{
			iter->second=Locale::Trim(val);
			return true;
		}
	}
	return false;
}

bool PropertiesFile::GetProperty(const TCHAR * key,EpTString &retVal) const
{
	LockObj lock(m_baseTextLock);
	EpTString opKey=Locale::Trim(key);
	opKey.append(_T("="));
	vector<Pair<EpTString, EpTString> >::const_iterator iter;
	for(iter=m_propertyList.begin();iter != m_propertyList.end();iter++)
	{
		if(iter->first.compare(opKey)==0)
		{
			retVal=iter->second;
			return true;
		}
	}
	return false;
}

EpTString &PropertiesFile::GetProperty(const TCHAR * key)
{
	LockObj lock(m_baseTextLock);
	EpTString opKey=Locale::Trim(key);
	opKey.append(_T("="));
	vector<Pair<EpTString, EpTString> >::iterator iter;
	for(iter=m_propertyList.begin();iter != m_propertyList.end();iter++)
	{
		if(iter->first.compare(opKey)==0)
		{
			return iter->second;
		}
	}
	EP_ASSERT_EXPR(0,_T("Given key does not exists in the list."));
	return m_nullString;
}

const EpTString &PropertiesFile::GetProperty(const TCHAR * key) const
{
	LockObj lock(m_baseTextLock);
	EpTString opKey=Locale::Trim(key);
	opKey.append(_T("="));
	vector<Pair<EpTString, EpTString> >::const_iterator iter;
	for(iter=m_propertyList.begin();iter != m_propertyList.end();iter++)
	{
		if(iter->first.compare(opKey)==0)
		{
			return iter->second;
		}
	}
	EP_ASSERT_EXPR(0,_T("Given key does not exists in the list."));
	return m_nullString;
}

bool PropertiesFile::AddProperty(const TCHAR * key, const TCHAR * val)
{
	LockObj lock(m_baseTextLock);
	EpTString opKey=Locale::Trim(key);
	opKey.append(_T("="));
	vector<Pair<EpTString, EpTString> >::iterator iter;
	for(iter=m_propertyList.begin();iter != m_propertyList.end();iter++)
	{
		if(iter->first.compare(opKey)==0)
		{
			return false;
		}
	}
	Pair<EpTString,EpTString> insertPair;
	insertPair.first=opKey;
	insertPair.second=Locale::Trim(val);
	m_propertyList.push_back(insertPair);
	return true;
}

bool PropertiesFile::RemoveProperty(const TCHAR * key)
{
	LockObj lock(m_baseTextLock);
	EpTString opKey=Locale::Trim(key);
	opKey.append(_T("="));
	vector<Pair<EpTString, EpTString> >::iterator iter;
	for(iter=m_propertyList.begin();iter != m_propertyList.end();iter++)
	{
		if(iter->first.compare(opKey)==0)
		{
			m_propertyList.erase(iter);
			return true;
		}
	}
	return false;
}

void PropertiesFile::Clear()
{
	LockObj lock(m_baseTextLock);
	m_propertyList.clear();
}
void PropertiesFile::writeLoop()
{
	vector<Pair<EpTString,EpTString> >::iterator iter;
	EpTString toFileString;
	for(iter=m_propertyList.begin();iter!=m_propertyList.end();iter++)
	{
		toFileString=iter->first;
		toFileString.append(iter->second);
		toFileString.append(_T("\n"));
		writeToFile(toFileString.c_str());
	}
}

EpTString& PropertiesFile::operator [](const TCHAR * key)
{
	LockObj lock(m_baseTextLock);
	EpTString opKey=Locale::Trim(key);
	opKey.append(_T("="));
	vector<Pair<EpTString, EpTString> >::iterator iter;
	for(iter=m_propertyList.begin();iter != m_propertyList.end();iter++)
	{
		if(iter->first.compare(opKey)==0)
		{
			return iter->second;
		}
	}
	Pair<EpTString,EpTString> insertPair;
	insertPair.first=key;
	insertPair.second=_T("");
	m_propertyList.push_back(insertPair);
	return m_propertyList.at(m_propertyList.size()-1).second;
}

const EpTString& PropertiesFile::operator [](const TCHAR * key) const
{
	LockObj lock(m_baseTextLock);
	EpTString opKey=Locale::Trim(key);
	opKey.append(_T("="));
	vector<Pair<EpTString, EpTString> >::const_iterator iter;
	for(iter=m_propertyList.begin();iter != m_propertyList.end();iter++)
	{
		if(iter->first.compare(opKey)==0)
		{
			return iter->second;
		}
	}
	EP_ASSERT(0);
	return m_nullString;
}
void PropertiesFile::loadFromFile(const EpTString &lines)
{
	m_propertyList.clear();
	EpTString line=_T("");
	size_t skipIdx=0;
	while(BaseTextFile::GetLine(lines,skipIdx,line,&skipIdx))
	{
		EpTString key;
		EpTString val;
		if(getValueKeyFromLine(line,key,val))
		{
			Pair<EpTString,EpTString> inputPair;
			inputPair.first=Locale::Trim(key);
			inputPair.second=Locale::Trim(val);
			m_propertyList.push_back(inputPair);
		}
		else
		{
			Pair<EpTString,EpTString> inputPair;
			inputPair.first=Locale::Trim(line);
			inputPair.second=_T("");
			m_propertyList.push_back(inputPair);
		}
	}
	
}


bool PropertiesFile::getValueKeyFromLine(const EpTString &buf, EpTString &retKey, EpTString &retVal)
{
	TCHAR splitChar=0;
	int bufTrav=0;
	if(buf.length()<=0)
		return false;

	retKey=_T("");
	retVal=_T("");

	for(int testTrav=0; testTrav<buf.length();testTrav++)
	{
		if(buf.at(testTrav)==_T('#'))
			return false;
	}

	while(splitChar!=_T('=') && bufTrav<buf.length())
	{
		splitChar=buf.at(bufTrav);
		retKey.append(&splitChar,1);
		bufTrav++;
	}
	retVal=buf;
	retVal.erase(0,bufTrav);
	
	return true;
}
