/*! 
@file epXMLFile.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief XML File Interface
@version 2.0

@section LICENSE

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
		@param[in] encodingType the encoding type for this file
		@param[in] xmlInfo the xml parsing info
		@param[in] lockPolicyType The lock policy
		*/
		XMLFile(FileEncodingType encodingType=FILE_ENCODING_TYPE_UTF16LE,XMLInfo &xmlInfo=XMLInfo::xmlDefault, LockPolicy lockPolicyType=EP_LOCK_POLICY);

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
		XMLFile & operator=(const XMLFile&b);
		

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
		Recursively find all nodes with the given node name
		@param[in] node the root of the xml
		@param[in] nodeName the node name to find the nodes
		@return the list of the nodes with the given node name
		*/
		XNodes FindAllNode(XNode *node, const TCHAR *  nodeName);

		/*!
		Recursively find all nodes with the given node name
		@param[in] node the root of the xml
		@param[in] nodeName the node name to find the nodes
		@return the list of the nodes with the given node name
		*/
		XAttrs FindAllAttr(XNode *node, const TCHAR *  attrName);

		/*!
		Recursively find all nodes with the given node name
		@param[in] nodeName the node name to find the nodes
		@return the list of the nodes with the given node name
		*/
		XNodes FindAllNode(const TCHAR *  nodeName);

		/*!
		Recursively find all nodes with the given node name
		@param[in] nodeName the node name to find the nodes
		@return the list of the nodes with the given node name
		*/
		XAttrs FindAllAttr(const TCHAR *  attrName);

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

		/*!
		Set the encoding type as given type
		@param[in] encodingType the encoding type for this file
		*/
		virtual void SetEncodingType(FileEncodingType encodingType);

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
		@param[in] node the root node
		@param[in] nodeName the node name to find the nodes
		@return the list of the nodes with the given node name
		*/
		XNodes findAllNode(XNode *node, const TCHAR *  nodeName);

		/*!
		Recursively find all attributes with the given attribute name
		@param[in] node the root node
		@param[in] attrName the attribute name to find the attributes
		@return the list of the attributes with the given attribute name
		*/
		XAttrs findAllAttr(XNode *node, const TCHAR * attrName);

		XMLInfo m_xmlInfo;

	};
}

#endif //__EP_XML_FILE_H__