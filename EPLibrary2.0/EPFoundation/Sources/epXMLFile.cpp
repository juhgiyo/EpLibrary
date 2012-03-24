/*! 
XMLFile for the EpLibrary
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
#include "epXMLFile.h"
#include <queue>

using namespace epl;

#if TEST_NEW

XMLFile::XMLFile(FileEncodingType encodingType) :XNode(), BaseFile(encodingType)
{
}

XMLFile::~XMLFile()
{
	Close();
}
void XMLFile::SetNodeValue(CString nodeName, CString attrName, CString attrVal, CString nodeVal)
{
	XNodes findList=findAllNode(this,nodeName);
	XNodes::iterator findIter;
	for(findIter=findList.begin();findIter!=findList.end();findIter++)
	{
		XAttrs attrList=(*findIter)->GetAttrs(attrName.GetString());
		XAttrs::iterator attrIter;
		for(attrIter=attrList.begin();attrIter!=attrList.end();attrIter++)
		{
			if((*attrIter)->m_value==attrVal)
			{
				(*findIter)->m_value=nodeVal;
			}
		}
	}
}

void XMLFile::Clear()
{
	Close();
}
void XMLFile::writeLoop()
{
	CString toFileString;

	toFileString=GetXML();
	WriteToFile(toFileString);
	Close();
}
void XMLFile::loadFromFile(CString rest)
{
	Close();
	Load(rest.GetString());

}

XNodes XMLFile::findAllNode(XNode *node, CString nodeName)
{
	XNodes nodelist;
	std::queue<XNode*> recurseQueue;
	if(node==NULL)
		return nodelist;
	recurseQueue.push(node);
	if(node->m_name==nodeName)
		nodelist.push_back(node);
	while(!recurseQueue.empty())
	{
		XNode *procNode = recurseQueue.front();
		recurseQueue.pop();

		for(XNodes::iterator it=procNode->GetChilds().begin();it!= procNode->GetChilds().end();it++)
		{
			XNode * child = *it;
			if(child->m_name==nodeName)
				nodelist.push_back(child);
			recurseQueue.push(child);
		}
	}
	return nodelist;
}

#else //TEST_NEW
XMLFile::XMLFile(FileEncodingType encodingType) :XNode()
{
	m_encodingType=encodingType;
}

XMLFile::~XMLFile()
{
	Close();
}
void XMLFile::SetNodeValue(CString nodeName, CString attrName, CString attrVal, CString nodeVal)
{
	XNodes findList=findAllNode(this,nodeName);
	XNodes::iterator findIter;
	for(findIter=findList.begin();findIter!=findList.end();findIter++)
	{
		XAttrs attrList=(*findIter)->GetAttrs(attrName.GetString());
		XAttrs::iterator attrIter;
		for(attrIter=attrList.begin();attrIter!=attrList.end();attrIter++)
		{
			if((*attrIter)->m_value==attrVal)
			{
				(*findIter)->m_value=nodeVal;
			}
		}
	}
}

void XMLFile::Clear()
{
	Close();
}

bool XMLFile::SaveToFile(CString fileName)
{
	if(fileName.GetLength()<=0)
		return false;

	FILE *fStream;
	errno_t e;

	if(m_encodingType==FILE_ENCODING_TYPE_UTF8)
		e= _tfopen_s(&fStream,fileName.GetString(),_T("wt,ccs=UTF-8"));
	else if(m_encodingType==FILE_ENCODING_TYPE_UNICODE)
		e= _tfopen_s(&fStream,fileName.GetString(),_T("wt,ccs=UNICODE"));
	else if(m_encodingType==FILE_ENCODING_TYPE_UTF16)
		e= _tfopen_s(&fStream,fileName.GetString(),_T("wt,ccs=UTF-16LE"));
	else
		return false;
	if (e != 0) 
		return false; // failed..
	CStdioFile xmlFile(fStream);  // open the file from this stream


	if(m_encodingType==FILE_ENCODING_TYPE_UTF8||m_encodingType==FILE_ENCODING_TYPE_UTF16)
	{	
		CString toFileString;

		toFileString=GetXML();

		char *multiByteToFile = EP_NEW char[toFileString.GetLength()+1];
		
		memset(multiByteToFile,0,toFileString.GetLength()+1);
		wcstombs(multiByteToFile,toFileString.GetString(),toFileString.GetLength());

		xmlFile.Write(multiByteToFile,toFileString.GetLength());

		EP_DELETE[] multiByteToFile;

	}
	else
	{
		CString toFileString;

		toFileString=GetXML();
		xmlFile.Write(toFileString,(toFileString.GetLength())*sizeof(TCHAR));
	}
	Close();
	return true;

}

bool XMLFile::LoadFromFile(CString fileName)
{
 	Close();

	if(fileName.GetLength()<=0)
		return false;

	FILE *fStream;
	errno_t e;

	if(m_encodingType==FILE_ENCODING_TYPE_UTF8)
		e= _tfopen_s(&fStream,fileName.GetString(),_T("rt,ccs=UTF-8"));
	else if(m_encodingType==FILE_ENCODING_TYPE_UNICODE)
		e= _tfopen_s(&fStream,fileName.GetString(),_T("rt,ccs=UNICODE"));
	else if(m_encodingType==FILE_ENCODING_TYPE_UTF16)
		e= _tfopen_s(&fStream,fileName.GetString(),_T("rt,ccs=UTF-16LE"));
	else
		return false;

	if (e != 0) 
		return false; // failed..
	CStdioFile xmlFile(fStream);  // open the file from this stream

	CString xmlString;
	if(m_encodingType==FILE_ENCODING_TYPE_UTF8||m_encodingType==FILE_ENCODING_TYPE_UTF16)
	{		

		unsigned int length=(unsigned int)xmlFile.SeekToEnd()+1;
		xmlFile.SeekToBegin();

		char *cFileBuf=EP_NEW char[length];
		memset(cFileBuf,0,length);
		xmlFile.Read(cFileBuf,length);
		xmlFile.Close();

		TCHAR *tFileBuf=EP_NEW TCHAR[length];
		MultiByteToWideChar(CP_ACP, 0, cFileBuf, -1, tFileBuf,length);

		xmlString=tFileBuf;
		EP_DELETE[] cFileBuf;
		EP_DELETE[] tFileBuf;

	}
	else
	{
		unsigned int length=(unsigned int)xmlFile.GetLength();

		TCHAR *tFileBuf=EP_NEW TCHAR[length];
		memset(tFileBuf,0,sizeof(TCHAR)*length);
		xmlFile.Read(tFileBuf,length);
		xmlFile.Close();

		xmlString=tFileBuf;
		EP_DELETE[] tFileBuf;
	}
	Load(xmlString.GetString());
	return true;

}

XNodes XMLFile::findAllNode(XNode *node, CString nodeName)
{
	XNodes nodelist;
	std::queue<XNode*> recurseQueue;
	if(node==NULL)
		return nodelist;
	recurseQueue.push(node);
	if(node->m_name==nodeName)
		nodelist.push_back(node);
	while(!recurseQueue.empty())
	{
		XNode *procNode = recurseQueue.front();
		recurseQueue.pop();

		for(XNodes::iterator it=procNode->GetChilds().begin();it!= procNode->GetChilds().end();it++)
		{
			XNode * child = *it;
			if(child->m_name==nodeName)
				nodelist.push_back(child);
			recurseQueue.push(child);
		}
	}
	return nodelist;
}
#endif //TEST_NEW