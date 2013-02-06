/*! 
@file epXMLFile.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief XML File Interface
@version 2.0

@section LICENSE

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

@section DESCRIPTION

An Interface for the XML File Class.

*/
#ifndef __EP_XML_FILE_H__
#define __EP_XML_FILE_H__
#include "epLib.h"
#include "epSystem.h"
#include "epXMLite.h"
#include "epBaseTextFile.h"
#include <vector>

using namespace std;

namespace epl
{
	struct _tagXMLNode;
	typedef _tagXMLNode XNode;

	/*!
	@struct XMLInfo epXMLFile.h
	@brief A structure for XML parsing information.
	*/
	typedef struct EP_LIBRARY _xmlInfo
	{
		/// string for xml version
		EpTString m_xmlVersion;
		/// string for encoding type
		EpTString m_xmlEncoding;
		/// flag for writing comment or not
		bool m_isWriteComment;
		/// Escape Character for Value
		TCHAR m_escapeValue; 

		/*!
		Default Constructor

		Initializes the XML parsing info with default values 
		*/
		_xmlInfo() { m_xmlVersion = _T("1.0"); m_xmlEncoding =  _T("UTF-16"); m_isWriteComment = false;m_escapeValue=_T('\\'); }
		
		/// Default XML parsing information
		static _xmlInfo xmlDefault;
	}XMLInfo;

	/*!
	@class XMLFile epXMLFile.h
	@brief A class for XML File.
	*/
	class EP_LIBRARY XMLFile:public XNode, public BaseTextFile{
	public:
		/*!
		Default Constructor

		Initializes the XML File 
		@param[in] xmlInfo the xml parsing info
		@param[in] encodingType the encoding type for this file
		@param[in] lockPolicyType The lock policy
		*/
		XMLFile(XMLInfo &xmlInfo=XMLInfo::xmlDefault,FileEncodingType encodingType=FILE_ENCODING_TYPE_UTF16LE, LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the XML File  
		@param[in] b the second object
		*/
		XMLFile(const XMLFile& b);

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		XMLFile & operator=(const XMLFile&b)
		{
			if(this!=&b)
			{
				BaseTextFile::operator =(b);
				LockObj lock(m_lock);
				XNode::operator =(const_cast<XMLFile&>(b));
				m_xmlInfo=b.m_xmlInfo;
			}
			return *this;
		}

		/*!
		Default Destructor

		Destroy the XML File 
		*/
		virtual ~XMLFile();

		/*!
		Recursively set all nodes of the given node name with the node value given
		@param[in] nodeName the name of the node to replace
		@param[in] attrName the attribute name of the node
		@param[in] attrVal the attribute value of the node with given attribute name
		@param[in] nodeVal the value for replacement when found
		*/
		void SetNodeValue(const TCHAR * nodeName, const TCHAR * attrName, const TCHAR *attrVal, const TCHAR * nodeVal);

		/*!
		Recursively get all attribute value of nodes of the given node name with the attribute name given
		@param[in] nodeName the name of the node to find
		@param[in] attrName the attribute name of the node
		@return the list of attribute values for given node name and attribute name.
		*/
		vector<const TCHAR *> GetAttrValue(const TCHAR *nodeName, const TCHAR *attrName);
		
		/*!
		Set XML info.
		@param[in] info the XML info to set.
		*/
		void SetXMLInfo(XMLInfo &info);

		/*!
		Get current current XML info.
		@return the current XML info.
		*/
		XMLInfo GetXMLInfo();


		/*!
		Clear the list of the XML
		*/
		void Clear();

	protected:
		/*!
		Loop Function that writes to the file.
		@remark Sub classes should implement this function
		*/
		virtual void writeLoop();

		/*!
		Actual load Function that loads values from the file.
		@remark Sub classes should implement this function
		@param[in] lines the all data from the file
		*/
		virtual void loadFromFile(const EpTString &lines);

		/*!
		Recursively find all nodes with the given node name
		@param[in] node the root of the xml
		@param[in] nodeName the node name to find the nodes
		@return the list of the nodes with the given node name
		*/
		XNodes findAllNode(XNode *node, const TCHAR *  nodeName);

		XMLInfo m_xmlInfo;

	};
}

#endif //__EP_XML_FILE_H__