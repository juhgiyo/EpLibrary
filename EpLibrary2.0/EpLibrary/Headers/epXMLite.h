/*! 
@file epXMLite.h
@author Cho,Kyung Min <bro@shinbiro.com> - October 30, 2002
	edited by Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com> - August 08, 2010
			  <http://github.com/juhgiyo/eplibrary>
@date October 30, 2002
@brief XMLite Interface

@section LICENSE

The MIT License (MIT)

Copyright (c) 2008-2013 Woong Gyu La <juhgiyo@gmail.com>

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

------------------------------------------------------------------------------

Part of this software is

Copyright (c) 2002 Cho,Kyung Min <bro@shinbiro.com>

Please refer to <http://www.codeproject.com/Articles/3426/XMLite-simple-XML-parser> for the license.

@section DESCRIPTION

An Interface for XMLite Class.
*/
#ifndef __EP_XMLITE_H__
#define __EP_XMLITE_H__

#include "epLib.h"
#include "epSystem.h"
#include "epMemory.h"
#include <vector>
#include <deque>

namespace epl
{
	struct _tagXMLAttr;
	typedef _tagXMLAttr XAttr, *LPXAttr;
	typedef std::vector<LPXAttr> XAttrs;

	struct _tagXMLNode;
	typedef _tagXMLNode XNode, *LPXNode;
	typedef std::vector<LPXNode> XNodes, *LPXNodes;

	struct _tagXMLDocument;
	typedef struct _tagXMLDocument XDoc, *LPXDoc;

	// Entity Encode/Decode Support
	typedef struct EP_LIBRARY _tagXmlEntity
	{
		TCHAR m_entity;					// entity ( & " ' < > )
		TCHAR m_ref[10];					// entity reference ( &amp; &quot; etc )
		int m_ref_len;					// entity reference length
	}XENTITY,*LPXENTITY;

	typedef struct EP_LIBRARY _tagXMLEntitys : public std::vector<XENTITY>
	{
		LPXENTITY GetEntity( int entity );
		LPXENTITY GetEntity( LPTSTR entity );	
		int GetEntityCount( const TCHAR * str );
		int Ref2Entity( const TCHAR * estr, LPTSTR str, int strlen );
		int Entity2Ref( const TCHAR * str, LPTSTR estr, int estrlen );
		CString Ref2Entity( const TCHAR * estr );
		CString Entity2Ref( const TCHAR * str );	

		_tagXMLEntitys(){};
		_tagXMLEntitys( LPXENTITY entities, int count );
		static _tagXMLEntitys entityDefault;
	}XENTITYS,*LPXENTITYS;
	
	CString XRef2Entity( const TCHAR * estr );
	CString XEntity2Ref( const TCHAR * str );	

	typedef enum 
	{
		PIE_PARSE_WELFORMED	= 0,
		PIE_ALONE_NOT_CLOSED,
		PIE_NOT_CLOSED,
		PIE_NOT_NESTED,
		PIE_ATTR_NO_VALUE
	}PCODE;


	typedef struct EP_LIBRARY _tagValueParseInfo
	{

		TCHAR chXMLTagOpen;
		TCHAR chXMLTagClose;
		TCHAR chXMLTagPre;
		TCHAR chXMLEscape;	// for value field escape
		_tagValueParseInfo() { chXMLTagOpen = _T('<'); chXMLTagClose =  _T('>'); chXMLTagPre = _T('/'); chXMLEscape = _T('\\'); }

		static _tagValueParseInfo vpiDefault;
	}VALUEPARSEINFO,*LPVALUEPARSEINFO;

	// Parse info.
	typedef struct EP_LIBRARY _tagParseInfo
	{
		bool		m_trim_value;			// [set] do trim when parse?
		bool		m_entity_value;		// [set] do convert from reference to entity? ( &lt; -> < )
		LPXENTITYS	m_entitys;			// [set] entity table for entity decode
		TCHAR		m_escape_value;		// [set] escape value (default '\0')
		bool		m_force_parse;		// [set] force parse even if xml is not welformed

		LPTSTR		m_xml;				// [get] xml source
		bool		m_erorr_occur;		// [get] is occurance of error?
		LPTSTR		m_error_pointer;		// [get] error position of xml source
		PCODE		m_error_code;			// [get] error code
		CString		m_error_string;		// [get] error string

		LPXDoc		m_doc;
		_tagParseInfo() { m_trim_value = false; m_entity_value = true; m_force_parse = false; m_entitys = &XENTITYS::entityDefault; m_xml = NULL; m_erorr_occur = false; m_error_pointer = NULL; m_error_code = PIE_PARSE_WELFORMED; m_escape_value = _T('\0'); }

		static _tagParseInfo piDefault;
	}PARSEINFO,*LPPARSEINFO;
	

	// display optional environment
	typedef struct EP_LIBRARY _tagDispOption
	{
		bool m_newline;			// newline when new tag
		bool m_reference_value;	// do convert from entity to reference ( < -> &lt; )
		TCHAR m_value_quotation_mark;	// val="" (default value quotation mark "
		LPXENTITYS	m_entitys;	// entity table for entity encode

		int m_tab_base;			// internal usage
		_tagDispOption() { m_newline = true; m_reference_value = true; m_entitys = &XENTITYS::entityDefault; m_tab_base = 0; m_value_quotation_mark = _T('"'); }
		static _tagDispOption optDefault;
	}DISP_OPT, *LPDISP_OPT;
	

	// XAttr : Attribute Implementation
	typedef struct EP_LIBRARY _tagXMLAttr
	{
		CString m_name;
		CString	m_value;
		
		_tagXMLNode*	m_parent;

		CString GetXML( LPDISP_OPT opt = &DISP_OPT::optDefault );
	}XAttr, *LPXAttr;

	typedef enum
	{
		XNODE_ELEMENT,				// general node '<element>...</element>' or <element/>
		XNODE_PI,					// <?xml version="1.0" ?>
		XNODE_COMMENT,				// <!-- comment -->
		XNODE_CDATA,				// <![CDATA[ cdata ]]>
		XNODE_DOC,					// internal virtual root
	}NODE_TYPE;

	// XMLNode structure
	typedef struct EP_LIBRARY _tagXMLNode
	{
		// name and value
		CString m_name;
		CString	m_value;

		// internal variables
		LPXNode	m_parent;		// parent node
		XNodes	m_childs;		// child node
		XAttrs	m_attrs;		// attributes
		NODE_TYPE m_type;		// node type 
		LPXDoc	m_doc;		// document

		// Load/Save XML
		LPTSTR	Load( const TCHAR * pszXml, LPVALUEPARSEINFO vpi = &VALUEPARSEINFO::vpiDefault, LPPARSEINFO pi = &PARSEINFO::piDefault );
		CString GetXML( LPDISP_OPT opt = &DISP_OPT::optDefault );
		CString GetText( LPDISP_OPT opt = &DISP_OPT::optDefault );

		// internal load functions
		LPTSTR	LoadAttributes( const TCHAR * pszAttrs,LPVALUEPARSEINFO vpi = &VALUEPARSEINFO::vpiDefault, LPPARSEINFO pi = &PARSEINFO::piDefault);
		LPTSTR	LoadAttributes( const TCHAR * pszAttrs, const TCHAR * pszEnd, LPVALUEPARSEINFO vpi = &VALUEPARSEINFO::vpiDefault, LPPARSEINFO pi = &PARSEINFO::piDefault );
		LPTSTR	LoadProcessingInstrunction( const TCHAR * pszXml, LPVALUEPARSEINFO vpi = &VALUEPARSEINFO::vpiDefault, LPPARSEINFO pi = &PARSEINFO::piDefault);
		LPTSTR	LoadComment( const TCHAR * pszXml, LPVALUEPARSEINFO vpi = &VALUEPARSEINFO::vpiDefault, LPPARSEINFO pi = &PARSEINFO::piDefault ); 
		LPTSTR	LoadCDATA( const TCHAR * pszXml, LPVALUEPARSEINFO vpi = &VALUEPARSEINFO::vpiDefault, LPPARSEINFO pi = &PARSEINFO::piDefault ); 

		// in own attribute list
		LPXAttr	GetAttr( const TCHAR * attrname ); 
		const TCHAR *	GetAttrValue( const TCHAR * attrname ); 
		XAttrs	GetAttrs( const TCHAR * name ); 

		// in one level child nodes
		LPXNode	GetChild( const TCHAR * name ); 
		const TCHAR *	GetChildValue( const TCHAR * name ); 
		CString	GetChildText( const TCHAR * name, LPDISP_OPT opt = &DISP_OPT::optDefault );
		XNodes	GetChilds( const TCHAR * name ); 
		XNodes	&GetChilds(); 

		LPXAttr GetChildAttr( const TCHAR * name, const TCHAR * attrname );
		const TCHAR * GetChildAttrValue( const TCHAR * name, const TCHAR * attrname );
		
		// search node
		LPXNode	Find( const TCHAR * name );

		// modify DOM 
		size_t		GetChildCount();
		LPXNode GetChild( int i );
		XNodes::iterator GetChildIterator( LPXNode node );
		LPXNode CreateNode( const TCHAR * name = NULL, const TCHAR * value = NULL );
		LPXNode	AppendChild( const TCHAR * name = NULL, const TCHAR * value = NULL );
		LPXNode	AppendChild( LPXNode node );
		bool	RemoveChild( LPXNode node );
		LPXNode DetachChild( LPXNode node );

		// node/branch copy
		void	CopyNode( LPXNode node );
		void	CopyBranch( LPXNode branch );
		void	_CopyBranch( LPXNode node );
		LPXNode	AppendChildBranch( LPXNode node );

		// modify attribute
		LPXAttr GetAttr( int i );
		XAttrs::iterator GetAttrIterator( LPXAttr node );
		LPXAttr CreateAttr( const TCHAR * anem = NULL, const TCHAR * value = NULL );
		LPXAttr AppendAttr( const TCHAR * name = NULL, const TCHAR * value = NULL );
		LPXAttr	AppendAttr( LPXAttr attr );
		bool	RemoveAttr( LPXAttr attr );
		LPXAttr DetachAttr( LPXAttr attr );

		// operator overloads
		LPXNode operator [] ( int i ) { return GetChild(i); }
		XNode& operator = ( XNode& node ) { CopyBranch(&node); return *this; }

		_tagXMLNode() { m_parent = NULL; m_doc = NULL; m_type = XNODE_ELEMENT; }
		~_tagXMLNode();

		void Close();
	}XNode, *LPXNode;

	// XMLDocument structure
	typedef struct EP_LIBRARY _tagXMLDocument : public XNode
	{
		PARSEINFO	m_parse_info;
		VALUEPARSEINFO m_valueParse_info;

		_tagXMLDocument() { m_parent = NULL; m_doc = this; m_type = XNODE_DOC; }
		
		LPTSTR	Load( const TCHAR * pszXml, LPVALUEPARSEINFO vpi = NULL,LPPARSEINFO pi = NULL);
		LPXNode	GetRoot();

	}XDoc, *LPXDoc;

	// Helper Funtion
	inline long XStr2Int( const TCHAR * str, long default_value = 0 )
	{
		return ( str && *str ) ? _ttol(str) : default_value;
	}

	inline bool XIsEmptyString( const TCHAR * str )
	{
		CString s(str);
		s.TrimLeft();
		s.TrimRight();

		return ( s.IsEmpty() || s == _T("") );
	}
}
#endif //__EP_XMLITE_H__
