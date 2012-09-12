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

XMLInfo _xmlInfo::xmlDefault=XMLInfo();

XMLFile::XMLFile(XMLInfo &xmlInfo,FileEncodingType encodingType, LockPolicy lockPolicyType) :XNode(), BaseFile(encodingType,lockPolicyType)
{
	m_xmlInfo=xmlInfo;
}

XMLFile::XMLFile(const XMLFile& b):XNode(b),BaseFile(b)
{
	m_xmlInfo=b.m_xmlInfo;
}
XMLFile::~XMLFile()
{
	Close();
}
void XMLFile::SetNodeValue(const TCHAR * nodeName, const TCHAR * attrName, const TCHAR *attrVal, const TCHAR * nodeVal)
{
	LockObj lock(m_lock);
	XNodes findList=findAllNode(this,nodeName);
	XNodes::iterator findIter;
	for(findIter=findList.begin();findIter!=findList.end();findIter++)
	{
		XAttrs attrList=(*findIter)->GetAttrs(attrName);
		XAttrs::iterator attrIter;
		for(attrIter=attrList.begin();attrIter!=attrList.end();attrIter++)
		{
			if((*attrIter)->m_value.Compare(attrVal)==0)
			{
				(*findIter)->m_value=nodeVal;
			}
		}
	}
}

vector<const TCHAR *> XMLFile::GetAttrValue(const TCHAR *nodeName, const TCHAR *attrName)
{
	LockObj lock(m_lock);
	vector<const TCHAR *> retList;
	XNodes findList=findAllNode(this,nodeName);
	XNodes::iterator findIter;
	for(findIter=findList.begin();findIter!=findList.end();findIter++)
	{
		XAttrs attrList=(*findIter)->GetAttrs(attrName);
		XAttrs::iterator attrIter;
		for(attrIter=attrList.begin();attrIter!=attrList.end();attrIter++)
		{
			retList.push_back((*attrIter)->m_value.GetString());
		}
	}	
	return retList;
}

void XMLFile::SetXMLInfo(XMLInfo &info)
{
	m_xmlInfo=info;
}
XMLInfo XMLFile::GetXMLInfo()
{
	return m_xmlInfo;
}

void XMLFile::Clear()
{
	LockObj lock(m_lock);
	Close();
}
void XMLFile::writeLoop()
{
	EpTString toFileString;

	if(m_xmlInfo.m_isWriteComment)
	{
		EpTString commentToFile=_T("<?xml version=\"");
		commentToFile.append(m_xmlInfo.m_xmlVersion);
		commentToFile.append(_T("\" encoding=\""));
		commentToFile.append(m_xmlInfo.m_xmlEncoding);
		commentToFile.append(_T("\" ?>"));
		writeToFile(commentToFile.c_str());
	}

	toFileString=GetXML().GetString();
	toFileString.append(_T("\r\n"));
	writeToFile(toFileString.c_str());
}
void XMLFile::loadFromFile(EpTString lines)
{
	Close();
	VALUEPARSEINFO vpi;
	vpi.chXMLEscape=m_xmlInfo.m_escapeValue;
	Load(lines.c_str(),&vpi);

}

XNodes XMLFile::findAllNode(XNode *node, const TCHAR * nodeName)
{
	XNodes nodelist;
	std::queue<XNode*> recurseQueue;
	if(node==NULL)
		return nodelist;
	recurseQueue.push(node);
	if(node->m_name.Compare(nodeName)==0)
		nodelist.push_back(node);
	while(!recurseQueue.empty())
	{
		XNode *procNode = recurseQueue.front();
		recurseQueue.pop();

		for(XNodes::iterator it=procNode->GetChilds().begin();it!= procNode->GetChilds().end();it++)
		{
			XNode * child = *it;
			if(child->m_name.Compare(nodeName)==0)
				nodelist.push_back(child);
			recurseQueue.push(child);
		}
	}
	return nodelist;
}
