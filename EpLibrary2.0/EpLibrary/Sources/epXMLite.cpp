/*! 
XMLite for the EpLibrary
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

------------------------------------------------------------------------------

Part of this software is

Copyright (c) 2002 Cho,Kyung Min <bro@shinbiro.com>

Please refer to <http://www.codeproject.com/Articles/3426/XMLite-simple-XML-parser> for the license.
*/
#include "epXMLite.h"
#include <iostream>
#include <sstream>
#include <string>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace epl;

static const TCHAR szXMLPIOpen[] = _T("<?");
static const TCHAR szXMLPIClose[] = _T("?>");
static const TCHAR szXMLCommentOpen[] = _T("<!--");
static const TCHAR szXMLCommentClose[] = _T("-->");
static const TCHAR szXMLCDATAOpen[] = _T("<![CDATA[");
static const TCHAR szXMLCDATAClose[] = _T("]]>");

static const XENTITY x_EntityTable[] = {
		{ _T('&'), _T("&amp;"), 5 } ,
		{ _T('\"'), _T("&quot;"), 6 } ,
		{ _T('\''), _T("&apos;"), 6 } ,
		{ _T('<'), _T("&lt;"), 4 } ,
		{ _T('>'), _T("&gt;"), 4 } 
	};

VALUEPARSEINFO _tagValueParseInfo::vpiDefault=VALUEPARSEINFO();
PARSEINFO _tagParseInfo::piDefault=PARSEINFO();
DISP_OPT _tagDispOption::optDefault=DISP_OPT();
XENTITYS _tagXMLEntitys::entityDefault((LPXENTITY)x_EntityTable, sizeof(x_EntityTable)/sizeof(x_EntityTable[0]) );
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//========================================================
// Name   : _tcschrs
// Desc   : same with _tcspbrk 
// Param  :
// Return :
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPTSTR _tcschrs( const TCHAR * psz, const TCHAR * pszchs )
{
	while( psz && *psz )
	{
		if( _tcschr( pszchs, *psz ) )
			return (LPTSTR)psz;
		psz++;
	}
	return NULL;
}

//========================================================
// Name   : _tcsskip
// Desc   : skip space
// Param  : 
// Return : skiped string
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPTSTR _tcsskip( const TCHAR * psz )
{
	//while( psz && *psz == ' ' && *psz == 13 && *psz == 10 ) psz++;
	while( psz && isspace(*psz) ) psz++;
		
	return (LPTSTR)psz;
}

//========================================================
// Name   : _tcsechr
// Desc   : similar with _tcschr with escape process
// Param  : escape - will be escape character
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPTSTR _tcsechr( const TCHAR * psz, int ch, int escape )
{
	LPTSTR pch = (LPTSTR)psz;

	while( pch && *pch )
	{
		if( escape != 0 && *pch == escape )
			pch++;
		else
		if( *pch == ch ) 
			return (LPTSTR)pch;
		pch++;
	}
	return pch;
}

//========================================================
// Name   : _tcselen
// Desc   : similar with _tcslen with escape process
// Param  : escape - will be escape character
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
int _tcselen( int escape, LPTSTR srt, LPTSTR end = NULL ) 
{
	int len = 0;
	LPTSTR pch = srt;
	if( end==NULL ) end = (LPTSTR)sizeof(long);
	LPTSTR prev_escape = NULL;
	while( pch && *pch && pch<end )
	{
		if( escape != 0 && *pch == escape && prev_escape == NULL )
			prev_escape = pch;
		else
		{
			prev_escape = NULL;
			len++;
		}
		pch++;
	}
	return len;
}

//========================================================
// Name   : _tcsecpy
// Desc   : similar with _tcscpy with escape process
// Param  : escape - will be escape character
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
void _tcsecpy( LPTSTR psz, int escape, LPTSTR srt, LPTSTR end = NULL )
{
	LPTSTR pch = srt;
	if( end==NULL ) end = (LPTSTR)sizeof(long);
	LPTSTR prev_escape = NULL;
	while( pch && *pch && pch<end )
	{
		if( escape != 0 && *pch == escape && prev_escape == NULL )
			prev_escape = pch;
		else
		{
			prev_escape = NULL;
			*psz++ = *pch;
		}

		pch++;
	}

	*psz = _T('\0');
}

//========================================================
// Name   : _tcsepbrk
// Desc   : similar with _tcspbrk with escape process
// Param  : escape - will be escape character
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPTSTR _tcsepbrk( const TCHAR * psz, const TCHAR * chset, int escape )
{
	LPTSTR pch = (LPTSTR)psz;
	LPTSTR prev_escape = NULL;
	while( pch && *pch )
	{
		if( escape != 0 && *pch == escape && prev_escape == NULL )
			prev_escape = pch;
		else
		{
			prev_escape = NULL;
			if( _tcschr( chset, *pch ) )
				return (LPTSTR)pch;		
		}
		pch++;
	}
	return pch;
}

//========================================================
// Name   : _tcsenicmp
// Desc   : similar with _tcsnicmp with escape process
// Param  : escape - will be escape character
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
int _tcsenicmp( const TCHAR * psz, const TCHAR * str, int len, int escape )
{
	LPTSTR pch = (LPTSTR)psz;
	LPTSTR prev_escape = NULL;
	LPTSTR des = (LPTSTR)str;
	int i = 0;
	
	while( pch && *pch && i < len )
	{
		if( escape != 0 && *pch == escape && prev_escape == NULL )
			prev_escape = pch;
		else
		{
			prev_escape = NULL;
			if( tolower(*pch) != tolower(des[i]) )
				break;
			i++;
		}
		pch ++;
	}
	
	// find
	if( i == len )
		return 0;
	if( psz[i] > des[i] )
		return 1;
	return -1;
}

//========================================================
// Name   : _tcsenistr
// Desc   : similar with _tcsistr with escape process
// Param  : escape - will be escape character
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPTSTR _tcsenistr( const TCHAR * psz, const TCHAR * str, int len, int escape )
{
	LPTSTR pch = (LPTSTR)psz;
	LPTSTR prev_escape = NULL;
	LPTSTR des = (LPTSTR)str;
	int i = 0;
	
	while( pch && *pch )
	{
		if( escape != 0 && *pch == escape && prev_escape == NULL )
			prev_escape = pch;
		else
		{
			prev_escape = NULL;
			if( _tcsenicmp( pch, str, len, escape ) == 0 )
				return (LPTSTR)pch;
		}
		pch++;
	}
	return pch;
}

//========================================================
// Name   : _tcseistr
// Desc   : similar with _tcsistr with escape process
// Param  : escape - will be escape character
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPTSTR _tcseistr( const TCHAR * psz, const TCHAR * str, int escape )
{
	int len = _tcslen( str );
	return _tcsenistr( psz, str, len, escape );
}

//========================================================
// Name   : _SetString
// Desc   : put string of (psz~end) on ps string
// Param  : trim - will be trim?
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
void _SetString( LPTSTR psz, LPTSTR end, CString* ps, bool trim = FALSE, int escape = 0 )
{
	//trim
	if( trim )
	{
		while( psz && psz < end && _istspace(*psz) ) psz++;
		while( (end-1) && psz < (end-1) && _istspace(*(end-1)) ) end--;
	}
	int len = end - psz;
	if( len <= 0 ) return;
	if( escape )
	{
		len = _tcselen( escape, psz, end );
		LPTSTR pss = ps->GetBufferSetLength( len );
		_tcsecpy( pss, escape, psz, end );
	}
	else
	{
		LPTSTR pss = ps->GetBufferSetLength(len + 1 );
		 memcpy( pss, psz, len * sizeof(TCHAR) );
		pss[len] = _T('\0');
	}
}

_tagXMLNode::~_tagXMLNode()
{
	Close();
}

void _tagXMLNode::Close()
{
	int i;
	for( i = 0 ; i < m_childs.size(); i ++)
	{
		LPXNode p = m_childs[i];
		if( p )
		{
			EP_DELETE p; m_childs[i] = NULL;
		}
	}
	m_childs.clear();
	
	for( i = 0 ; i < m_attrs.size(); i ++)
	{
		LPXAttr p = m_attrs[i];
		if( p )
		{
			EP_DELETE p; m_attrs[i] = NULL;
		}
	}
	m_attrs.clear();
}
	
// attr1="value1" attr2='value2' attr3=value3 />
//                                            ^- return pointer
//========================================================
// Name   : LoadAttributes
// Desc   : loading attribute plain xml text
// Param  : pszAttrs - xml of attributes
//          pi = parser information
// Return : advanced string pointer. (error return NULL)
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPTSTR _tagXMLNode::LoadAttributes( const TCHAR * pszAttrs ,LPVALUEPARSEINFO vpi /*= &vpiDefault*/, LPPARSEINFO pi /*= &piDefault*/)
{
	LPTSTR xml = (LPTSTR)pszAttrs;

	while( xml && *xml )
	{
		if( xml = _tcsskip( xml ) )
		{
			// close tag
			if( *xml == vpi->chXMLTagClose || *xml == vpi->chXMLTagPre )
				// wel-formed tag
				return xml;

			// XML Attr Name
			TCHAR* pEnd = _tcspbrk( xml, _T(" =") );
			if( pEnd == NULL ) 
			{
				// error
				if( pi->m_erorr_occur == false ) 
				{
					pi->m_erorr_occur = true;
					pi->m_error_pointer = xml;
					pi->m_error_code = PIE_ATTR_NO_VALUE;
					pi->m_error_string.Format( _T("<%s> attribute has error "), m_name );
				}
				return NULL;
			}
			
			LPXAttr attr = EP_NEW XAttr;
			attr->m_parent = this;

			// XML Attr Name
			_SetString( xml, pEnd, &attr->m_name );
			
			// add new attribute
			m_attrs.push_back( attr );
			xml = pEnd;
			
			// XML Attr Value
			if( xml = _tcsskip( xml ) )
			{
				//if( xml = _tcschr( xml, '=' ) )
				if( *xml == _T('=') )
				{
					if( xml = _tcsskip( ++xml ) )
					{
						// if " or '
						// or none quote
						int quote = *xml;
						if( quote == _T('"') || quote == _T('\'') )
							pEnd = _tcsechr( ++xml, quote, vpi->chXMLEscape );
						else
						{
							//attr= value> 
							// none quote mode
							pEnd = _tcsepbrk( xml, _T(" >"), vpi->chXMLEscape );
						}

						bool trim = pi->m_trim_value;
						TCHAR escape = pi->m_escape_value;
						_SetString( xml, pEnd, &attr->m_value, trim, escape );
						xml = pEnd;
						// ATTRVALUE 
						if( pi->m_entity_value && pi->m_entitys )
							attr->m_value = pi->m_entitys->Ref2Entity(attr->m_value);

						if( quote == _T('"') || quote == _T('\'') )
							xml++;
					}
				}
			}
		}
	}

	// not wel-formed tag
	return NULL;
}

// attr1="value1" attr2='value2' attr3=value3 />
//                                            ^- return pointer
//========================================================
// Name   : LoadAttributes
// Desc   : loading attribute plain xml text
// Param  : pszAttrs - xml of attributes
//          pszEnd - last string
//          pi = parser information
// Return : advanced string pointer. (error return NULL)
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2004-06-14
//========================================================
LPTSTR _tagXMLNode::LoadAttributes( const TCHAR * pszAttrs, const TCHAR * pszEnd, LPVALUEPARSEINFO vpi /*= &vpiDefault*/, LPPARSEINFO pi /*= &piDefault*/)
{
	LPTSTR xml = (LPTSTR)pszAttrs;

	while( xml && *xml )
	{
		if( xml = _tcsskip( xml ) )
		{
			// close tag
			if( xml >= pszEnd )
				// wel-formed tag
				return xml;

			// XML Attr Name
			TCHAR* pEnd = _tcspbrk( xml, _T(" =") );
			if( pEnd == NULL ) 
			{
				// error
				if( pi->m_erorr_occur == false ) 
				{
					pi->m_erorr_occur = true;
					pi->m_error_pointer = xml;
					pi->m_error_code = PIE_ATTR_NO_VALUE;
					pi->m_error_string.Format( _T("<%s> attribute has error "), m_name );
				}
				return NULL;
			}
			
			LPXAttr attr = EP_NEW XAttr;
			attr->m_parent = this;

			// XML Attr Name
			_SetString( xml, pEnd, &attr->m_name );
			
			// add new attribute
			m_attrs.push_back( attr );
			xml = pEnd;
			
			// XML Attr Value
			if( xml = _tcsskip( xml ) )
			{
				//if( xml = _tcschr( xml, '=' ) )
				if( *xml == _T('=') )
				{
					if( xml = _tcsskip( ++xml ) )
					{
						// if " or '
						// or none quote
						int quote = *xml;
						if( quote == _T('"') || quote == _T('\'') )
							pEnd = _tcsechr( ++xml, quote, vpi->chXMLEscape );
						else
						{
							//attr= value> 
							// none quote mode
							pEnd = _tcsepbrk( xml, _T(" >"),  vpi->chXMLEscape );
						}

						bool trim = pi->m_trim_value;
						TCHAR escape = pi->m_escape_value;
						_SetString( xml, pEnd, &attr->m_value, trim, escape );
						xml = pEnd;
						// ATTRVALUE 
						if( pi->m_entity_value && pi->m_entitys )
							attr->m_value = pi->m_entitys->Ref2Entity(attr->m_value);

						if( quote == _T('"') || quote == _T('\'') )
							xml++;
					}
				}
			}
		}
	}

	// not wel-formed tag
	return NULL;
}

// <?xml version="1.0"?>
//                      ^- return pointer
//========================================================
// Name   : LoadProcessingInstrunction
// Desc   : loading processing instruction
// Param  : pszXml - PI string
//          pi - parser information
// Return : advanced string pointer. (error return NULL)
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2004-06-14
//========================================================
LPTSTR _tagXMLNode::LoadProcessingInstrunction( const TCHAR * pszXml,LPVALUEPARSEINFO vpi /*= &vpiDefault*/, LPPARSEINFO pi /*= &piDefault*/)
{
	// find the end of pi
	LPTSTR end = _tcsenistr( pszXml, szXMLPIClose, sizeof(szXMLPIClose)/sizeof(TCHAR)-1, pi ? pi->m_escape_value : 0 );
	if( end == NULL )
		return NULL;

	// process pi
	if( m_doc )
	{
		LPTSTR xml = (LPTSTR)pszXml;

		LPXNode node = EP_NEW XNode;
		node->m_parent = this;
		node->m_doc = m_doc;
		node->m_type = XNODE_PI;
		
		xml += sizeof(szXMLPIOpen)/sizeof(TCHAR)-1;
		TCHAR* pTagEnd = _tcspbrk( xml, _T(" ?>") );
		_SetString( xml, pTagEnd, &node->m_name );
		xml = pTagEnd;
		
		node->LoadAttributes( xml, end, vpi, pi);

		m_doc->m_childs.push_back( node );
	}

	end += sizeof(szXMLPIClose)/sizeof(TCHAR)-1;
	return end;
}

// <!-- comment -->
//                 ^- return pointer
//========================================================
// Name   : LoadComment
// Desc   : loading comment
// Param  : pszXml - comment string
//          pi - parser information
// Return : advanced string pointer. (error return NULL)
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2004-06-14
//========================================================
LPTSTR _tagXMLNode::LoadComment( const TCHAR * pszXml, LPVALUEPARSEINFO vpi /*= &vpiDefault*/, LPPARSEINFO pi /*= &piDefault*/)
{
	// find the end of comment
	LPTSTR end = _tcsenistr( pszXml, szXMLCommentClose, sizeof(szXMLCommentClose)/sizeof(TCHAR)-1, pi ? pi->m_escape_value : 0 );
	if( end == NULL )
		return NULL;

	// process comment
	LPXNode par = m_parent;
	if( m_parent == NULL && m_doc )
		par = (LPXNode)&m_doc;
	if( par )
	{
		LPTSTR xml = (LPTSTR)pszXml;
		xml += sizeof(szXMLCommentOpen)/sizeof(TCHAR)-1;
		
		LPXNode node = EP_NEW XNode;
		node->m_parent = this;
		node->m_doc = m_doc;
		node->m_type = XNODE_COMMENT;
		node->m_name = _T("#COMMENT");
		_SetString( xml, end, &node->m_value, FALSE );

		par->m_childs.push_back( node );
	}

	end += sizeof(szXMLCommentClose)/sizeof(TCHAR)-1;
	return end;
}

// <![CDATA[ cdata ]]>
//                    ^- return pointer
//========================================================
// Name   : LoadCDATA
// Desc   : loading CDATA
// Param  : pszXml - CDATA string
//          pi - parser information
// Return : advanced string pointer. (error return NULL)
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2004-06-14
//========================================================
LPTSTR _tagXMLNode::LoadCDATA( const TCHAR * pszXml,LPVALUEPARSEINFO vpi /*= &vpiDefault*/, LPPARSEINFO pi /*= &piDefault*/)
{
	// find the end of CDATA
	LPTSTR end = _tcsenistr( pszXml, szXMLCDATAClose, sizeof(szXMLCDATAClose)/sizeof(TCHAR)-1, pi ? pi->m_escape_value : 0 );
	if( end == NULL )
		return NULL;

	// process CDATA
	LPXNode par = m_parent;
	if( m_parent == NULL && m_doc )
		par = (LPXNode)&m_doc;
	if( par )
	{
		LPTSTR xml = (LPTSTR)pszXml;
		xml += sizeof(szXMLCDATAOpen)/sizeof(TCHAR)-1;
		
		LPXNode node = EP_NEW XNode;
		node->m_parent = this;
		node->m_doc = m_doc;
		node->m_type = XNODE_CDATA;
		node->m_name = _T("#CDATA");
		_SetString( xml, end, &node->m_value, FALSE );

		par->m_childs.push_back( node );
	}

	end += sizeof(szXMLCDATAClose)/sizeof(TCHAR)-1;
	return end;
}

//========================================================
// Name   : LoadOtherNodes
// Desc   : internal function for loading PI/CDATA/Comment
// Param  : node - current xml node
//          pbRet - error occur
//          pszXml - CDATA string
//          pi - parser information
// Return : advanced string pointer. (error return NULL)
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2004-06-14
//========================================================
LPTSTR LoadOtherNodes( LPXNode node, bool* pbRet, const TCHAR * pszXml,LPVALUEPARSEINFO vpi /*= &vpiDefault*/, LPPARSEINFO pi /*= &piDefault*/)
{
	LPTSTR xml = (LPTSTR)pszXml;
	bool do_other_type = true;
	*pbRet = false;

	while( xml && do_other_type )
	{
		do_other_type = false;

		xml = _tcsskip( xml );
		LPTSTR prev = xml;
		// is PI( Processing Instruction ) Node?
		if( _tcsnicmp( xml, szXMLPIOpen, sizeof(szXMLPIOpen)/sizeof(TCHAR)-1 ) == 0 )
		{
			// processing instrunction parse
			// return pointer is next node of pi
			xml = node->LoadProcessingInstrunction( xml, vpi, pi);
			//if( xml == NULL )
			//	return NULL;
			// restart xml parse
		}

		if( xml != prev )
			do_other_type = true;
		xml = _tcsskip( xml );
		prev = xml;

		// is comment Node?
		if( _tcsnicmp( xml, szXMLCommentOpen, sizeof(szXMLCommentOpen)/sizeof(TCHAR)-1 ) == 0 )
		{
			// processing comment parse
			// return pointer is next node of comment
			xml = node->LoadComment( xml, vpi, pi );
			// comment node is terminal node
			if( node->m_parent && node->m_parent->m_type != XNODE_DOC 
				&& xml != prev )
			{
				*pbRet = true;
				return xml;
			}
			// restart xml parse when this node is root doc node
		}

		if( xml != prev )
			do_other_type = true;

		xml = _tcsskip( xml );
		prev = xml;
		// is CDATA Node?
		if( _tcsnicmp( xml, szXMLCDATAOpen, sizeof(szXMLCDATAOpen)/sizeof(TCHAR)-1 ) == 0 )
		{
			// processing CDATA parse
			// return pointer is next node of CDATA
			xml = node->LoadCDATA( xml, vpi, pi );
			// CDATA node is terminal node
			if( node->m_parent && node->m_parent->m_type != XNODE_DOC 
				&& xml != prev )
			{
				*pbRet = true;
				return xml;
			}
			// restart xml parse when this node is root doc node
		}

		if( xml != prev )
			do_other_type = true;
	}

	return xml;
}

// <TAG attr1="value1" attr2='value2' attr3=value3 >
// </TAG>
// or
// <TAG />
//        ^- return pointer
//========================================================
// Name   : Load
// Desc   : load xml plain text
// Param  : pszXml - plain xml text
//          pi = parser information
// Return : advanced string pointer  (error return NULL)
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPTSTR _tagXMLNode::Load( const TCHAR * pszXml, LPVALUEPARSEINFO vpi /*= &vpiDefault*/, LPPARSEINFO pi /*= &piDefault*/ )
{
	// Close it
	Close();

	LPTSTR xml = (LPTSTR)pszXml;

	xml = _tcschr( xml, vpi->chXMLTagOpen );
	if( xml == NULL )
		return NULL;

	// Close Tag
	if( *(xml+1) == vpi->chXMLTagPre ) // </Close
		return xml;

	// Load Other Node before <Tag>(pi, comment, CDATA etc)
	bool bRet = false;
	LPTSTR ret = NULL;
	ret = LoadOtherNodes( this, &bRet, xml, vpi, pi );
	if( ret != NULL ) 
		xml = ret;
	if( bRet ) 
		return xml;

	// XML Node Tag Name Open
	xml++;
	TCHAR* pTagEnd = _tcspbrk( xml, _T(" />\t\r\n") );
	_SetString( xml, pTagEnd, &m_name );
	xml = pTagEnd;
	// Generate XML Attributte List
	if( xml = LoadAttributes( xml, vpi, pi ) )
	{
		// alone tag <TAG ... />
		if( *xml == vpi->chXMLTagPre )
		{
			xml++;
			if( *xml == vpi->chXMLTagClose )
				// wel-formed tag
				return ++xml;
			else
			{
				// error: <TAG ... / >
				if( pi->m_erorr_occur == false ) 
				{
					pi->m_erorr_occur = true;
					pi->m_error_pointer = xml;
					pi->m_error_code = PIE_ALONE_NOT_CLOSED;
					pi->m_error_string = _T("Element must be closed.");
				}
				// not wel-formed tag
				return NULL;
			}
		}
		else
		// open/close tag <TAG ..> ... </TAG>
		//                        ^- current pointer
		{
			// if text value is not exist, then assign value
			//if( this->value.IsEmpty() || this->value == _T("") )
			if( XIsEmptyString( m_value ) )
			{
				// Text Value 
				TCHAR* pEnd = _tcsechr( ++xml, vpi->chXMLTagOpen, vpi->chXMLEscape );
				if( pEnd == NULL ) 
				{
					if( pi->m_erorr_occur == false ) 
					{
						pi->m_erorr_occur = true;
						pi->m_error_pointer = xml;
						pi->m_error_code = PIE_NOT_CLOSED;
						pi->m_error_string.Format(_T("%s must be closed with </%s>"), m_name );
					}
					// error cos not exist CloseTag </TAG>
					return NULL;
				}
				
				bool trim = pi->m_trim_value;
				TCHAR escape = pi->m_escape_value;
				//_SetString( xml, pEnd, &value, trim, chXMLEscape );
				_SetString( xml, pEnd, &m_value, trim, escape );

				xml = pEnd;
				// TEXTVALUE reference
				if( pi->m_entity_value && pi->m_entitys )
					m_value = pi->m_entitys->Ref2Entity(m_value);
			}

			// generate child nodes
			while( xml && *xml )
			{
				LPXNode node = EP_NEW XNode;
				node->m_parent = this;
				node->m_doc = m_doc;
				node->m_type = m_type;
				
				xml = node->Load( xml,vpi, pi );
				if( node->m_name.IsEmpty() == FALSE )
				{
					m_childs.push_back( node );

				}
				else
				{
					EP_DELETE node;
				}

				// open/close tag <TAG ..> ... </TAG>
				//                             ^- current pointer
				// CloseTag case
				if( xml && *xml && *(xml+1) && *xml == vpi->chXMLTagOpen && *(xml+1) == vpi->chXMLTagPre )
				{
					// </Close>
					xml+=2; // C
					
					if( xml = _tcsskip( xml ) )
					{
						CString closename;
						TCHAR* pEnd = _tcspbrk( xml, _T(" >") );
						if( pEnd == NULL ) 
						{
							if( pi->m_erorr_occur == false ) 
							{
								pi->m_erorr_occur = true;
								pi->m_error_pointer = xml;
								pi->m_error_code = PIE_NOT_CLOSED;
								pi->m_error_string.Format(_T("it must be closed with </%s>"), m_name );
							}
							// error
							return NULL;
						}
						_SetString( xml, pEnd, &closename );
						if( closename == this->m_name )
						{
							// wel-formed open/close
							xml = pEnd+1;
							// return '>' or ' ' after pointer
							return xml;
						}
						else
						{
							xml = pEnd+1;
							// 2004.6.15 - example <B> alone tag
							// now it can parse with attribute 'force_arse'
							if( pi->m_force_parse == false )
							{
								// not welformed open/close
								if( pi->m_erorr_occur == false ) 
								{
									pi->m_erorr_occur = true;
									pi->m_error_pointer = xml;
									pi->m_error_code = PIE_NOT_NESTED;
									pi->m_error_string.Format(_T("'<%s> ... </%s>' is not wel-formed."), m_name, closename );
								}
								return NULL;
							}
						}
					}
				}
				else	// Alone child Tag Loaded
						// else 해야하는지 말아야하는지 의심간다.
				{
					
					//if( xml && this->value.IsEmpty() && *xml !=chXMLTagOpen )
					if( xml && XIsEmptyString( m_value ) && *xml !=vpi->chXMLTagOpen )
					{
						// Text Value 
						TCHAR* pEnd = _tcsechr( xml, vpi->chXMLTagOpen, vpi->chXMLEscape );
						if( pEnd == NULL ) 
						{
							// error cos not exist CloseTag </TAG>
							if( pi->m_erorr_occur == false )  
							{
								pi->m_erorr_occur = true;
								pi->m_error_pointer = xml;
								pi->m_error_code = PIE_NOT_CLOSED;
								pi->m_error_string.Format(_T("it must be closed with </%s>"), m_name );
							}
							return NULL;
						}
						
						bool trim = pi->m_trim_value;
						TCHAR escape = pi->m_escape_value;
						//_SetString( xml, pEnd, &value, trim, chXMLEscape );
						_SetString( xml, pEnd, &m_value, trim, escape );

						xml = pEnd;
						//TEXTVALUE
						if( pi->m_entity_value && pi->m_entitys )
							m_value = pi->m_entitys->Ref2Entity(m_value);
					}
				}
			}
		}
	}

	return xml;
}

// <?xml version='1.0'?>
// <TAG attr1="value1" attr2='value2' attr3=value3 >
// </TAG>
// or
// <TAG />
//        ^- return pointer
//========================================================
// Name   : Load
// Desc   : load xml plain text for xml document
// Param  : pszXml - plain xml text
//          pi = parser information
// Return : advanced string pointer  (error return NULL)
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPTSTR _tagXMLDocument::Load( const TCHAR * pszXml,  LPVALUEPARSEINFO vpi /*= NULL*/, LPPARSEINFO pi /*= NULL*/)
{
	LPXNode node = EP_NEW XNode;
	node->m_parent = (LPXNode)this;
	node->m_type = XNODE_ELEMENT;
	node->m_doc = this;
	LPTSTR end;
	
	if( pi == NULL )
		pi = &m_parse_info;

	if(vpi==NULL)
		vpi=&m_valueParse_info;

	if( (end = node->Load( pszXml, vpi, pi)) == NULL )
	{
		EP_DELETE node;
		return NULL;
	}

	m_childs.push_back( node );

	// Load Other Node after </Tag>(pi, comment, CDATA etc)
	LPTSTR ret;
	bool bRet = false;
	ret = LoadOtherNodes( node, &bRet, end, vpi , pi );
	if( ret != NULL ) 
		end = ret;

	return end;
}

LPXNode	_tagXMLDocument::GetRoot()
{
	XNodes::iterator it = m_childs.begin();
	for( ; it != m_childs.end() ; ++(it) )
	{
		LPXNode node = *it;
		if( node->m_type == XNODE_ELEMENT )
			return node;
	}
	return NULL;
}

//========================================================
// Name   : GetXML
// Desc   : convert plain xml text from parsed xml attirbute
// Param  :
// Return : converted plain string
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
CString _tagXMLAttr::GetXML( LPDISP_OPT opt /*= &optDefault*/ )
{
	std::basic_ostringstream<TCHAR> os;
	//os << (const TCHAR *)name << "='" << (const TCHAR *)value << "' ";
	
	os << (const TCHAR *)m_name << _T("=") << (char)opt->m_value_quotation_mark 
		<< (const TCHAR *)(opt->m_reference_value&&opt->m_entitys?opt->m_entitys->Entity2Ref(m_value):m_value) 
		<< (char)opt->m_value_quotation_mark << _T(" ");
	return os.str().c_str();
}

//========================================================
// Name   : GetXML
// Desc   : convert plain xml text from parsed xml node
// Param  :
// Return : converted plain string
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
CString _tagXMLNode::GetXML( LPDISP_OPT opt /*= &optDefault*/ )
{
	std::basic_ostringstream<TCHAR> os;

	// tab
	if( opt && opt->m_newline )
	{
		if( opt && opt->m_newline )
			os << _T("\r\n");
		for( int i = 0 ; i < opt->m_tab_base ; i++)
			os << _T('\t');
	}

	if( m_type == XNODE_DOC )
	{
		for( int i = 0 ; i < m_childs.size(); i++ )
			os << (const TCHAR *)m_childs[i]->GetXML( opt );
		return os.str().c_str();
	}
	else
	if( m_type == XNODE_PI )
	{
		// <?TAG
		os << szXMLPIOpen << (const TCHAR *)m_name;
		// <?TAG Attr1="Val1" 
		if( m_attrs.empty() == false ) os << _T(' ');
		for( int i = 0 ; i < m_attrs.size(); i++ )
		{
			os << (const TCHAR *)m_attrs[i]->GetXML(opt);
		}
		//?>
		os << szXMLPIClose;	
		return os.str().c_str();
	}
	else
	if( m_type == XNODE_COMMENT )
	{
		// <--comment
		os << szXMLCommentOpen << (const TCHAR *)m_value;
		//-->
		os << szXMLCommentClose;	
		return os.str().c_str();
	}
	else
	if( m_type == XNODE_CDATA )
	{
		// <--comment
		os << szXMLCDATAOpen << (const TCHAR *)m_value;
		//-->
		os << szXMLCDATAClose;	
		return os.str().c_str();
	}

	// <TAG
	os << _T('<') << (const TCHAR *)m_name;

	// <TAG Attr1="Val1" 
	if( m_attrs.empty() == false ) os << _T(' ');
	for( int i = 0 ; i < m_attrs.size(); i++ )
	{
		os << (const TCHAR *)m_attrs[i]->GetXML(opt);
	}
	
	if( m_childs.empty() && m_value.IsEmpty() )
	{
		// <TAG Attr1="Val1"/> alone tag 
		os << _T("/>");	
	}
	else
	{
		// <TAG Attr1="Val1"> and get child
		os << _T('>');
		if( opt && opt->m_newline && !m_childs.empty() )
		{
			opt->m_tab_base++;
		}

		for( int i = 0 ; i < m_childs.size(); i++ )
			os << (const TCHAR *)m_childs[i]->GetXML( opt );
		
		// Text Value
		if( m_value != _T("") )
		{
			if( opt && opt->m_newline && !m_childs.empty() )
			{
				if( opt && opt->m_newline )
					os << _T("\r\n");
				for( int i = 0 ; i < opt->m_tab_base ; i++)
					os << _T('\t');
			}
			os << (const TCHAR *)(opt->m_reference_value&&opt->m_entitys?opt->m_entitys->Entity2Ref(m_value):m_value);
		}

		// </TAG> CloseTag
		if( opt && opt->m_newline && !m_childs.empty() )
		{
			os << _T("\r\n");
			for( int i = 0 ; i < opt->m_tab_base-1 ; i++)
				os << _T('\t');
		}
		os << _T("</") << (const TCHAR *)m_name << _T('>');

		if( opt && opt->m_newline )
		{
			if( !m_childs.empty() )
				opt->m_tab_base--;
		}
	}
	
	return os.str().c_str();
}

//========================================================
// 함수명 : GetText
// 설  명 : 노드 하나를 텍스트 문자열로 반환
// 인  자 :
// 리턴값 : 변환된 문자열
//--------------------------------------------------------
// 작성자   작성일                 작성이유
// 조경민   2004-06-15
//========================================================
CString _tagXMLNode::GetText( LPDISP_OPT opt /*= &optDefault*/ )
{
	std::basic_ostringstream<TCHAR> os;

	if( m_type == XNODE_DOC )
	{
		for( int i = 0 ; i < m_childs.size(); i++ )
			os << (const TCHAR *)m_childs[i]->GetText( opt );
	}
	else
	if( m_type == XNODE_PI )
	{
		// no text
	}
	else
	if( m_type == XNODE_COMMENT )
	{
		// no text
	}
	else
	if( m_type == XNODE_CDATA )
	{
		os << (const TCHAR *)m_value;
	}
	else
	if( m_type == XNODE_ELEMENT )
	{
		if( m_childs.empty() && m_value.IsEmpty() )
		{
			// no text
		}
		else
		{
			// childs text
			for( int i = 0 ; i < m_childs.size(); i++ )
				os << (const TCHAR *)m_childs[i]->GetText();
			
			// Text Value
			os << (const TCHAR *)(opt->m_reference_value&&opt->m_entitys?opt->m_entitys->Entity2Ref(m_value):m_value);
		}
	}
	
	return os.str().c_str();
}

//========================================================
// Name   : GetAttr
// Desc   : get attribute with attribute name
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXAttr	_tagXMLNode::GetAttr( const TCHAR * attrname )
{
	for( int i = 0 ; i < m_attrs.size(); i++ )
	{
		LPXAttr attr = m_attrs[i];
		if( attr )
		{
			if( attr->m_name == attrname )
				return attr;
		}
	}
	return NULL;
}

//========================================================
// Name   : GetAttrs
// Desc   : find attributes with attribute name, return its list
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
XAttrs _tagXMLNode::GetAttrs( const TCHAR * name )
{
	XAttrs retAttrs;
	for( int i = 0 ; i < m_attrs.size(); i++ )
	{
		LPXAttr attr = m_attrs[i];
		if( attr )
		{
			if( attr->m_name == name )
				retAttrs.push_back( attr );
		}
	}
	return retAttrs;
}

//========================================================
// Name   : GetAttrValue
// Desc   : get attribute with attribute name, return its value
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
const TCHAR *	_tagXMLNode::GetAttrValue( const TCHAR * attrname )
{
	LPXAttr attr = GetAttr( attrname );
	return attr ? (const TCHAR *)attr->m_value : NULL;
}

XNodes &_tagXMLNode::GetChilds()
{
	return m_childs;
}

//========================================================
// Name   : GetChilds
// Desc   : Find childs with name and return childs list
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
XNodes _tagXMLNode::GetChilds( const TCHAR * name )
{
	XNodes nodes;
	for( int i = 0 ; i < m_childs.size(); i++ )
	{
		LPXNode node = m_childs[i];
		if( node )
		{
			if( node->m_name == name )
				nodes.push_back( node );
		}
	}
	return nodes;	
}

//========================================================
// Name   : GetChild
// Desc   : get child node with index
// Param  :
// Return : NULL return if no child.
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXNode _tagXMLNode::GetChild( int i )
{
	if( i >= 0 && i < m_childs.size() )
		return m_childs[i];
	return NULL;
}

//========================================================
// Name   : GetChildCount
// Desc   : get child node count
// Param  :
// Return : 0 return if no child
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-12-26
//========================================================
int	_tagXMLNode::GetChildCount()
{
	return m_childs.size();
}

//========================================================
// Name   : GetChild
// Desc   : Find child with name and return child
// Param  :
// Return : NULL return if no child.
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXNode	_tagXMLNode::GetChild( const TCHAR * name )
{
	for( int i = 0 ; i < m_childs.size(); i++ )
	{
		LPXNode node = m_childs[i];
		if( node )
		{
			if( node->m_name == name )
				return node;
		}
	}
	return NULL;
}

//========================================================
// Name   : GetChildValue
// Desc   : Find child with name and return child's value
// Param  :
// Return : NULL return if no child.
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
const TCHAR *	_tagXMLNode::GetChildValue( const TCHAR * name )
{
	LPXNode node = GetChild( name );
	return (node != NULL)? (const TCHAR *)node->m_value : NULL;
}

CString	_tagXMLNode::GetChildText( const TCHAR * name, LPDISP_OPT opt /*= &optDefault*/ )
{
	LPXNode node = GetChild( name );
	return (node != NULL)? node->GetText(opt) : _T("");
}

LPXAttr _tagXMLNode::GetChildAttr( const TCHAR * name, const TCHAR * attrname )
{
	LPXNode node = GetChild(name);
	return node ? node->GetAttr(attrname) : NULL;
}

const TCHAR * _tagXMLNode::GetChildAttrValue( const TCHAR * name, const TCHAR * attrname )
{
	LPXAttr attr = GetChildAttr( name, attrname );
	return attr ? (const TCHAR *)attr->m_value : NULL;
}

//========================================================
// Name   : Find
// Desc   : find node with tag name from it's all childs
// Param  :
// Return : NULL return if no found node.
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXNode	_tagXMLNode::Find( const TCHAR * name )
{
	XNodes::iterator it = m_childs.begin();
	for( ; it != m_childs.end(); ++(it))
	{
		LPXNode child = *it;
		if( child->m_name == name )
			return child;

		XNodes::iterator it = child->m_childs.begin();
		for( ; it != child->m_childs.end(); ++(it))
		{
			LPXNode find = child->Find( name );
			if( find != NULL )
				return find;
		}
	}

	return NULL;
}

//========================================================
// Name   : GetChildIterator
// Desc   : get child nodes iterator
// Param  :
// Return : NULL return if no childs.
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
XNodes::iterator _tagXMLNode::GetChildIterator( LPXNode node )
{
	XNodes::iterator it = m_childs.begin();
	for( ; it != m_childs.end() ; ++(it) )
	{
		if( *it == node )
			return it;
	}
	XNodes::iterator retIt;
	(*retIt)=NULL;
	return retIt;
}

//========================================================
// Name   : AppendChild
// Desc   : add node
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXNode	_tagXMLNode::AppendChild( const TCHAR * name /*= NULL*/, const TCHAR * value /*= NULL*/ )
{
	return AppendChild( CreateNode( name, value ) );
}

//========================================================
// Name   : AppendChild
// Desc   : add node
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXNode _tagXMLNode::AppendChild( LPXNode node )
{
	node->m_parent = this;
	node->m_doc = m_doc;
	m_childs.push_back( node );
	return node;
}

//========================================================
// Name   : RemoveChild
// Desc   : detach node and delete object
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
bool _tagXMLNode::RemoveChild( LPXNode node )
{
	XNodes::iterator it = GetChildIterator( node );
	if( *it )
	{
		EP_DELETE *it;
		m_childs.erase( it );
		return true;
	}
	return false;
}

//========================================================
// Name   : GetAttr
// Desc   : get attribute with index in attribute list
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXAttr _tagXMLNode::GetAttr( int i )
{
	if( i >= 0 && i < m_attrs.size() )
		return m_attrs[i];
	return NULL;
}

//========================================================
// Name   : GetAttrIterator
// Desc   : get attribute iterator
// Param  : 
// Return : std::vector<LPXAttr>::iterator
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
XAttrs::iterator _tagXMLNode::GetAttrIterator( LPXAttr attr )
{
	XAttrs::iterator it = m_attrs.begin();
	for( ; it != m_attrs.end() ; ++(it) )
	{
		if( *it == attr )
			return it;
	}
	XAttrs::iterator retIt;
	(*retIt)=NULL;
	return retIt;
}

//========================================================
// Name   : AppendAttr
// Desc   : add attribute
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXAttr _tagXMLNode::AppendAttr( LPXAttr attr )
{
	attr->m_parent = this;
	m_attrs.push_back( attr );
	return attr;
}

//========================================================
// Name   : RemoveAttr
// Desc   : detach attribute and delete object
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
bool _tagXMLNode::RemoveAttr( LPXAttr attr )
{
	XAttrs::iterator it = GetAttrIterator( attr );
	if( *it )
	{
		EP_DELETE *it;
		m_attrs.erase( it );
		return true;
	}
	return false;
}

//========================================================
// Name   : CreateNode
// Desc   : Create node object and return it
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXNode _tagXMLNode::CreateNode( const TCHAR * name /*= NULL*/, const TCHAR * value /*= NULL*/ )
{
	LPXNode node = EP_NEW XNode;
	node->m_name = name;
	node->m_value = value;
	return node;
}

//========================================================
// Name   : CreateAttr
// Desc   : create Attribute object and return it
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXAttr _tagXMLNode::CreateAttr( const TCHAR * name /*= NULL*/, const TCHAR * value /*= NULL*/ )
{
	LPXAttr attr = EP_NEW XAttr;
	attr->m_name = name;
	attr->m_value = value;
	return attr;
}

//========================================================
// Name   : AppendAttr
// Desc   : add attribute
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXAttr _tagXMLNode::AppendAttr( const TCHAR * name /*= NULL*/, const TCHAR * value /*= NULL*/ )
{
	return AppendAttr( CreateAttr( name, value ) );
}

//========================================================
// Name   : DetachChild
// Desc   : no delete object, just detach in list
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXNode _tagXMLNode::DetachChild( LPXNode node )
{
	XNodes::iterator it = GetChildIterator( node );
	if( *it )
	{
		m_childs.erase( it );
		return node;
	}
	return NULL;
}

//========================================================
// Name   : DetachAttr
// Desc   : no delete object, just detach in list
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXAttr _tagXMLNode::DetachAttr( LPXAttr attr )
{
	XAttrs::iterator it = GetAttrIterator( attr );
	if( *it )
	{
		m_attrs.erase( it );
		return attr;
	}
	return NULL;
}

//========================================================
// Name   : CopyNode
// Desc   : copy current level node with own attributes
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
void _tagXMLNode::CopyNode( LPXNode node )
{
	Close();

	m_doc = node->m_doc;
	m_parent = node->m_parent;
	m_name = node->m_name;
	m_value = node->m_value;
	m_type = node->m_type;

	// copy attributes
	for( int i = 0 ; i < node->m_attrs.size(); i++)
	{
		LPXAttr attr = node->m_attrs[i];
		if( attr )
			AppendAttr( attr->m_name, attr->m_value );
	}
}

//========================================================
// Name   : _CopyBranch
// Desc   : recursive internal copy branch 
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
void _tagXMLNode::_CopyBranch( LPXNode node )
{
	CopyNode( node );

	for( int i = 0 ; i < node->m_childs.size(); i++)
	{
		LPXNode child = node->m_childs[i];
		if( child )
		{
			LPXNode mychild = EP_NEW XNode;
			mychild->CopyNode( child );
			AppendChild( mychild );

			mychild->_CopyBranch( child );
		}
	}
}

//========================================================
// Name   : AppendChildBranch
// Desc   : add child branch ( deep-copy )
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXNode	_tagXMLNode::AppendChildBranch( LPXNode node )
{
	LPXNode child = EP_NEW XNode;
	child->CopyBranch( node );

	return AppendChild( child );
}

//========================================================
// Name   : CopyBranch
// Desc   : copy branch ( deep-copy )
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
void _tagXMLNode::CopyBranch( LPXNode branch )
{
	Close();
	
	_CopyBranch( branch );
}


_tagXMLEntitys::_tagXMLEntitys( LPXENTITY entities, int count )
{
	for( int i = 0; i < count; i++)
		push_back( entities[i] );
}

LPXENTITY _tagXMLEntitys::GetEntity( int entity )
{
	for( int i = 0 ; i < size(); i ++ )
	{
		if( at(i).m_entity == entity )
			return LPXENTITY(&at(i));
	}
	return NULL;
}

LPXENTITY _tagXMLEntitys::GetEntity( LPTSTR entity )
{
	for( int i = 0 ; i < size(); i ++ )
	{
		LPTSTR ref = (LPTSTR)at(i).m_ref;
		LPTSTR ps = entity;
		while( ref && *ref )
			if( *ref++ != *ps++ )
				break;
		if( ref && !*ref )	// found!
			return LPXENTITY(&at(i));
	}
	return NULL;
}

int _tagXMLEntitys::GetEntityCount( const TCHAR * str )
{
	int nCount = 0;
	LPTSTR ps = (LPTSTR)str;
	while( ps && *ps )
		if( GetEntity( *ps++ ) ) nCount ++;
	return nCount;
}

int _tagXMLEntitys::Ref2Entity( const TCHAR * estr, LPTSTR str, int strlen )
{
	LPTSTR pes = (LPTSTR)estr;
	LPTSTR ps = str;
	LPTSTR ps_end = ps+strlen;
	while( pes && *pes && ps < ps_end )
	{
		LPXENTITY ent = GetEntity( pes );
		if( ent )
		{
			// copy entity meanning char
			*ps = ent->m_entity;
			pes += ent->m_ref_len;
		}
		else
			*ps = *pes++;	// default character copy
		ps++;
	}
	*ps = _T('\0');
	
	// total copied characters
	return ps-str;	
}

int _tagXMLEntitys::Entity2Ref( const TCHAR * str, LPTSTR estr, int estrlen )
{
	LPTSTR ps = (LPTSTR)str;
	LPTSTR pes = (LPTSTR)estr;
	LPTSTR pes_end = pes+estrlen;
	while( ps && *ps && pes < pes_end )
	{
		LPXENTITY ent = GetEntity( *ps );
		if( ent )
		{
			// copy entity string
			LPTSTR ref = (LPTSTR)ent->m_ref;
			while( ref && *ref )
				*pes++ = *ref++;
		}
		else
			*pes++ = *ps;	// default character copy
		ps++;
	}
	*pes = _T('\0');
	
	// total copied characters
	return pes-estr;
}

CString _tagXMLEntitys::Ref2Entity( const TCHAR * estr )
{
	CString es;
	if( estr )
	{
		int len = _tcslen(estr);
		LPTSTR esbuf = es.GetBufferSetLength( len+1 );
		if( esbuf )
			Ref2Entity( estr, esbuf, len );
	}
	return es;
}

CString _tagXMLEntitys::Entity2Ref( const TCHAR * str )
{
	CString s;
	if( str )
	{
		int nEntityCount = GetEntityCount(str);
		if( nEntityCount == 0 )
			return CString(str);
		int len = _tcslen(str) + nEntityCount*10 ;
		LPTSTR sbuf = s.GetBufferSetLength( len+1 );
		if( sbuf )
			Entity2Ref( str, sbuf, len );
	}
	return s;
}

CString XRef2Entity( const TCHAR * estr )
{
	return epl::XENTITYS::entityDefault.Ref2Entity( estr );
}

CString XEntity2Ref( const TCHAR * str )
{
	return epl::XENTITYS::entityDefault.Entity2Ref( str );
}