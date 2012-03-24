/*! 
@file epXMLite.h
@author Cho,Kyung Min: bro@shinbiro.com
	edited by Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date August 08, 2010
@brief A XMLite Interface

@section LICENSE

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
	typedef struct EP_FOUNDATION _tagXmlEntity
	{
		TCHAR m_entity;					// entity ( & " ' < > )
		TCHAR m_ref[10];					// entity reference ( &amp; &quot; etc )
		int m_ref_len;					// entity reference length
	}XENTITY,*LPXENTITY;

	typedef struct EP_FOUNDATION _tagXMLEntitys : public std::vector<XENTITY>
	{
		LPXENTITY GetEntity( int entity );
		LPXENTITY GetEntity( LPTSTR entity );	
		int GetEntityCount( LPCTSTR str );
		int Ref2Entity( LPCTSTR estr, LPTSTR str, int strlen );
		int Entity2Ref( LPCTSTR str, LPTSTR estr, int estrlen );
		CString Ref2Entity( LPCTSTR estr );
		CString Entity2Ref( LPCTSTR str );	

		_tagXMLEntitys(){};
		_tagXMLEntitys( LPXENTITY entities, int count );
		static _tagXMLEntitys entityDefault;
	}XENTITYS,*LPXENTITYS;
	
	CString XRef2Entity( LPCTSTR estr );
	CString XEntity2Ref( LPCTSTR str );	

	typedef enum 
	{
		PIE_PARSE_WELFORMED	= 0,
		PIE_ALONE_NOT_CLOSED,
		PIE_NOT_CLOSED,
		PIE_NOT_NESTED,
		PIE_ATTR_NO_VALUE
	}PCODE;

	// Parse info.
	typedef struct EP_FOUNDATION _tagParseInfo
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
	typedef struct EP_FOUNDATION _tagDispOption
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
	typedef struct EP_FOUNDATION _tagXMLAttr
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
	typedef struct EP_FOUNDATION _tagXMLNode
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
		LPTSTR	Load( LPCTSTR pszXml, LPPARSEINFO pi = &PARSEINFO::piDefault );
		CString GetXML( LPDISP_OPT opt = &DISP_OPT::optDefault );
		CString GetText( LPDISP_OPT opt = &DISP_OPT::optDefault );

		// internal load functions
		LPTSTR	LoadAttributes( LPCTSTR pszAttrs, LPPARSEINFO pi = &PARSEINFO::piDefault );
		LPTSTR	LoadAttributes( LPCTSTR pszAttrs, LPCTSTR pszEnd, LPPARSEINFO pi = &PARSEINFO::piDefault );
		LPTSTR	LoadProcessingInstrunction( LPCTSTR pszXml, LPPARSEINFO pi = &PARSEINFO::piDefault );
		LPTSTR	LoadComment( LPCTSTR pszXml, LPPARSEINFO pi = &PARSEINFO::piDefault ); 
		LPTSTR	LoadCDATA( LPCTSTR pszXml, LPPARSEINFO pi = &PARSEINFO::piDefault ); 

		// in own attribute list
		LPXAttr	GetAttr( LPCTSTR attrname ); 
		LPCTSTR	GetAttrValue( LPCTSTR attrname ); 
		XAttrs	GetAttrs( LPCTSTR name ); 

		// in one level child nodes
		LPXNode	GetChild( LPCTSTR name ); 
		LPCTSTR	GetChildValue( LPCTSTR name ); 
		CString	GetChildText( LPCTSTR name, LPDISP_OPT opt = &DISP_OPT::optDefault );
		XNodes	GetChilds( LPCTSTR name ); 
		XNodes	&GetChilds(); 

		LPXAttr GetChildAttr( LPCTSTR name, LPCTSTR attrname );
		LPCTSTR GetChildAttrValue( LPCTSTR name, LPCTSTR attrname );
		
		// search node
		LPXNode	Find( LPCTSTR name );

		// modify DOM 
		int		GetChildCount();
		LPXNode GetChild( int i );
		XNodes::iterator GetChildIterator( LPXNode node );
		LPXNode CreateNode( LPCTSTR name = NULL, LPCTSTR value = NULL );
		LPXNode	AppendChild( LPCTSTR name = NULL, LPCTSTR value = NULL );
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
		LPXAttr CreateAttr( LPCTSTR anem = NULL, LPCTSTR value = NULL );
		LPXAttr AppendAttr( LPCTSTR name = NULL, LPCTSTR value = NULL );
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
	typedef struct EP_FOUNDATION _tagXMLDocument : public XNode
	{
		PARSEINFO	m_parse_info;

		_tagXMLDocument() { m_parent = NULL; m_doc = this; m_type = XNODE_DOC; }
		
		LPTSTR	Load( LPCTSTR pszXml, LPPARSEINFO pi = NULL );
		LPXNode	GetRoot();

	}XDoc, *LPXDoc;

	// Helper Funtion
	inline long XStr2Int( LPCTSTR str, long default_value = 0 )
	{
		return ( str && *str ) ? _ttol(str) : default_value;
	}

	inline bool XIsEmptyString( LPCTSTR str )
	{
		CString s(str);
		s.TrimLeft();
		s.TrimRight();

		return ( s.IsEmpty() || s == _T("") );
	}
}
#endif //__EP_XMLITE_H__
