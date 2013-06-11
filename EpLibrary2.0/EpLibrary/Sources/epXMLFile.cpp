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

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;

XMLInfo _xmlInfo::xmlDefault=XMLInfo();

XMLFile::XMLFile(FileEncodingType encodingType,XMLInfo &xmlInfo, LockPolicy lockPolicyType) :XNode(), BaseTextFile(encodingType,lockPolicyType)
{
	m_xmlInfo=xmlInfo;
	if(m_encodingType==FILE_ENCODING_TYPE_UTF16LE)
		m_xmlInfo.m_xmlEncoding=_T("UTF-16");
	else
		m_xmlInfo.m_xmlEncoding=_T("UTF-8");
}

XMLFile::XMLFile(const XMLFile& b):XNode(b),BaseTextFile(b)
{
	LockObj lock(b.m_baseTextLock);
	m_xmlInfo=b.m_xmlInfo;
}
XMLFile::~XMLFile()
{
	Close();
}
XMLFile & XMLFile::operator=(const XMLFile&b)
{
	if(this!=&b)
	{
		Close();

		BaseTextFile::operator =(b);
		LockObj lock(b.m_baseTextLock);
		XNode::operator =(const_cast<XMLFile&>(b));
		m_xmlInfo=b.m_xmlInfo;
	}
	return *this;
}

void XMLFile::SetNodeValue(const TCHAR * nodeName, const TCHAR * attrName, const TCHAR *attrVal, const TCHAR * nodeVal)
{
	LockObj lock(m_baseTextLock);
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
	LockObj lock(m_baseTextLock);
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
	LockObj lock(m_baseTextLock);
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
void XMLFile::loadFromFile(const EpTString &lines)
{
	Close();
	VALUEPARSEINFO vpi;
	vpi.chXMLEscape=m_xmlInfo.m_escapeValue;
	Load(lines.c_str(),&vpi);

}

XNodes XMLFile::FindAllNode(const TCHAR *  nodeName)
{
	LockObj lock(m_baseTextLock);
	return findAllNode(this,nodeName);
}

XNodes XMLFile::FindAllNode(XNode *node, const TCHAR *  nodeName)
{
	LockObj lock(m_baseTextLock);
	return findAllNode(node,nodeName);
}

XAttrs XMLFile::FindAllAttr(const TCHAR *  attrName)
{
	LockObj lock(m_baseTextLock);
	return findAllAttr(this,attrName);
}

XAttrs XMLFile::FindAllAttr(XNode *node, const TCHAR *  attrName)
{
	LockObj lock(m_baseTextLock);
	return findAllAttr(node,attrName);
}

XAttrs XMLFile::findAllAttr(XNode *node, const TCHAR * attrName)
{
	XAttrs attrlist;
	std::queue<XNode*> recurseQueue;
	if(node==NULL)
		return attrlist;
	recurseQueue.push(node);

	while(!recurseQueue.empty())
	{
		XNode *procNode = recurseQueue.front();
		recurseQueue.pop();
		
		for(XAttrs::iterator it=procNode->m_attrs.begin();it!=procNode->m_attrs.end();it++)
		{
			if((*it)->m_name.Compare(attrName)==0)
			{
				attrlist.push_back(*it);
			}
		}

		for(XNodes::iterator it=procNode->GetChilds().begin();it!= procNode->GetChilds().end();it++)
		{
			XNode * child = *it;
			recurseQueue.push(child);
		}
	}
	return attrlist;
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

void XMLFile::SetEncodingType(FileEncodingType encodingType)
{
	LockObj lock(m_baseTextLock);
	m_encodingType=encodingType;
	if(m_encodingType==FILE_ENCODING_TYPE_UTF16LE)
		m_xmlInfo.m_xmlEncoding=_T("UTF-16");
	else
		m_xmlInfo.m_xmlEncoding=_T("UTF-8");
}
