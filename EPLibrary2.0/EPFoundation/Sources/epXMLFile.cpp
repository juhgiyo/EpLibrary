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


XMLFile::XMLFile(FileEncodingType encodingType, LockPolicy lockPolicyType) :XNode(), BaseFile(encodingType)
{
}

XMLFile::XMLFile(const XMLFile& b):XNode(b),BaseFile(b)
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
