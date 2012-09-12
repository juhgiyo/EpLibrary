/*! 
@file epLocale.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date April 17, 2011
@brief Locale Class Interface
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

An Interface for Locale Class.

*/
#ifndef __EP_LOCALE_H__
#define __EP_LOCALE_H__

#include "epLib.h"
#include <vector>
#include <string.h>
namespace epl{
	/// A type definition for EpString Type
	typedef std::string EpString;
	/// A type definition for EpWString Type
	typedef std::wstring EpWString;
	/// A type definition for EpTString Type
#if defined(_UNICODE) || defined(UNICODE)
	typedef EpWString EpTString;
#else //defined(_UNICODE) || defined(UNICODE)
	typedef EpString EpTString;
#endif //defined(_UNICODE) || defined(UNICODE)
	
	/*! 
	@class Locale epLocale.h
	@brief This is a base class for Locale  Class

	Implements the Locale Functions.
	*/
	class EP_LIBRARY Locale
	{
	public:

		/*!
		Check whether given character is a alphanumeric
		@param[in] c the character to check
		@return true if given character is a alphanumeric otherwise false
		*/
		static bool IsAlnum(TCHAR c);
		/*!
		Check whether given character is a alphabet
		@param[in] c the character to check
		@return true if given character is a alphabet otherwise false
		*/
		static bool IsAlpha(TCHAR c);
		/*!
		Check whether given character is a control
		@param[in] c the character to check
		@return true if given character is a control otherwise false
		*/
		static bool IsCntrl(TCHAR c);
		/*!
		Check whether given character is a digit
		@param[in] c the character to check
		@return true if given character is a digit otherwise false
		*/
		static bool IsDigit(TCHAR c);
		/*!
		Check whether given character is a graph
		@param[in] c the character to check
		@return true if given character is a graph otherwise false
		*/
		static bool IsGraph(TCHAR c);
		/*!
		Check whether given character is a lower character
		@param[in] c the character to check
		@return true if given character is a lower character otherwise false
		*/
		static bool IsLower(TCHAR c);
		/*!
		Check whether given character is a punctuation
		@param[in] c the character to check
		@return true if given character is a punctuation otherwise false
		*/
		static bool IsPunct(TCHAR c);
		/*!
		Check whether given character is a upper character
		@param[in] c the character to check
		@return true if given character is a upper character otherwise false
		*/
		static bool IsUpper(TCHAR c);
		/*!
		Check whether given character is a hexa-decimal
		@param[in] c the character to check
		@return true if given character is a hexa-decimal otherwise false
		*/
		static bool IsXdigit(TCHAR c);
		/*!
		Check whether given character is a space
		@param[in] c the character to check
		@return true if given character is a space otherwise false
		*/
		static bool IsSpace(TCHAR c);
		/*!
		Check whether given character is printable
		@param[in] c the character to check
		@return true if printable otherwise false
		*/
		static bool IsPrint(TCHAR c);
		/*!
		Return the small-case character of given character
		@param[in] c the character to transform to small-case character
		@return small-case character of given character
		*/
		static TCHAR ToLower(TCHAR c);
		/*!
		Return the capitalized character of given character
		@param[in] c the character to transform to capitalized character
		@return capitalized character of given character
		*/
		static TCHAR ToUpper(TCHAR c);

		/*!
		Check whether given character is a alphanumeric
		@param[in] c the character to check
		@return true if given character is a alphanumeric otherwise false
		*/
		static bool IsWAlnum(wchar_t c);
		/*!
		Check whether given character is a alphabet
		@param[in] c the character to check
		@return true if given character is a alphabet otherwise false
		*/
		static bool IsWAlpha(wchar_t c);
		/*!
		Check whether given character is a control
		@param[in] c the character to check
		@return true if given character is a control otherwise false
		*/
		static bool IsWCntrl(wchar_t c);
		/*!
		Check whether given character is a digit
		@param[in] c the character to check
		@return true if given character is a digit otherwise false
		*/
		static bool IsWDigit(wchar_t c);
		/*!
		Check whether given character is a graph
		@param[in] c the character to check
		@return true if given character is a graph otherwise false
		*/
		static bool IsWGraph(wchar_t c);
		/*!
		Check whether given character is a lower character
		@param[in] c the character to check
		@return true if given character is a lower character otherwise false
		*/
		static bool IsWLower(wchar_t c);
		/*!
		Check whether given character is a punctuation
		@param[in] c the character to check
		@return true if given character is a punctuation otherwise false
		*/
		static bool IsWPunct(wchar_t c);
		/*!
		Check whether given character is a upper character
		@param[in] c the character to check
		@return true if given character is a upper character otherwise false
		*/
		static bool IsWUpper(wchar_t c);
		/*!
		Check whether given character is a hexa-decimal
		@param[in] c the character to check
		@return true if given character is a hexa-decimal otherwise false
		*/
		static bool IsWXdigit(wchar_t c);
		/*!
		Check whether given character is a space
		@param[in] c the character to check
		@return true if given character is a space otherwise false
		*/
		static bool IsWSpace(wchar_t c);
		/*!
		Check whether given character is printable
		@param[in] c the character to check
		@return true if printable otherwise false
		*/
		static bool IsWPrint(wchar_t c);
		/*!
		Return the small-case character of given character
		@param[in] c the character to transform to small-case character
		@return small-case character of given character
		*/
		static wchar_t ToWLower(wchar_t c);
		/*!
		Return the capitalized character of given character
		@param[in] c the character to transform to capitalized character
		@return capitalized character of given character
		*/
		static wchar_t ToWUpper(wchar_t c);

		/*!
		Check whether given character is a alphanumeric
		@param[in] c the character to check
		@return true if given character is a alphanumeric otherwise false
		*/
		static bool IsCAlnum(char c);
		/*!
		Check whether given character is a alphabet
		@param[in] c the character to check
		@return true if given character is a alphabet otherwise false
		*/
		static bool IsCAlpha(char c);
		/*!
		Check whether given character is a control
		@param[in] c the character to check
		@return true if given character is a control otherwise false
		*/
		static bool IsCCntrl(char c);
		/*!
		Check whether given character is a digit
		@param[in] c the character to check
		@return true if given character is a digit otherwise false
		*/
		static bool IsCDigit(char c);
		/*!
		Check whether given character is a graph
		@param[in] c the character to check
		@return true if given character is a graph otherwise false
		*/
		static bool IsCGraph(char c);
		/*!
		Check whether given character is a lower character
		@param[in] c the character to check
		@return true if given character is a lower character otherwise false
		*/
		static bool IsCLower(char c);
		/*!
		Check whether given character is a punctuation
		@param[in] c the character to check
		@return true if given character is a punctuation otherwise false
		*/
		static bool IsCPunct(char c);
		/*!
		Check whether given character is a upper character
		@param[in] c the character to check
		@return true if given character is a upper character otherwise false
		*/
		static bool IsCUpper(char c);
		/*!
		Check whether given character is a hexa-decimal
		@param[in] c the character to check
		@return true if given character is a hexa-decimal otherwise false
		*/
		static bool IsCXdigit(char c);
		/*!
		Check whether given character is a space
		@param[in] c the character to check
		@return true if given character is a space otherwise false
		*/
		static bool IsCSpace(char c);
		/*!
		Check whether given character is printable
		@param[in] c the character to check
		@return true if printable otherwise false
		*/
		static 	bool IsCPrint(char c);
		/*!
		Return the small-case character of given character
		@param[in] c the character to transform to small-case character
		@return small-case character of given character
		*/
		static char ToCLower(char c);
		/*!
		Return the capitalized character of given character
		@param[in] c the character to transform to capitalized character
		@return capitalized character of given character
		*/
		static char ToCUpper(char c);


		/*!
		Trim the left size of given string
		@param[in] str the sting to trim
		@return the string trimmed
		*/
		static EpString CTrimLeft(const EpString& str);

		/*!
		Trim the right size of given string
		@param[in] str the sting to trim
		@return the string trimmed
		*/
		static EpString CTrimRight(const EpString& str);

		/*!
		Trim the given string
		@param[in] str the sting to trim
		@return the string trimmed
		*/
		static EpString CTrim(const EpString& str);

		/*!
		Trim the left size of given string
		@param[in] str the sting to trim
		@return the string trimmed
		*/
		static EpWString WTrimLeft(const EpWString& str);

		/*!
		Trim the right size of given string
		@param[in] str the sting to trim
		@return the string trimmed
		*/
		static EpWString WTrimRight(const EpWString& str);

		/*!
		Trim the given string
		@param[in] str the sting to trim
		@return the string trimmed
		*/
		static EpWString WTrim(const EpWString& str);

		/*!
		Trim the left size of given string
		@param[in] str the sting to trim
		@return the string trimmed
		*/
		static EpTString TrimLeft(const EpTString& str);

		/*!
		Trim the right size of given string
		@param[in] str the sting to trim
		@return the string trimmed
		*/
		static EpTString TrimRight(const EpTString& str);

		/*!
		Trim the given string
		@param[in] str the sting to trim
		@return the string trimmed
		*/
		static EpTString Trim(const EpTString& str);

		/*!
		Return the string that is transformed to all capitalized characters
		@param[in] str the sting to transform
		@return the string that is capitalized
		*/
		static EpString ToCUpper(const EpString& str);

		/*!
		Return the string that is transformed to all small-case characters
		@param[in] str the sting to transform
		@return the string that is all small-cased
		*/
		static EpString ToCLower(const EpString& str);

		/*!
		Return the string that is transformed to all capitalized characters
		@param[in] str the sting to transform
		@return the string that is capitalized
		*/
		static EpWString ToWUpper(const EpWString& str);

		/*!
		Return the string that is transformed to all small-case characters
		@param[in] str the sting to transform
		@return the string that is all small-cased
		*/
		static EpWString ToWLower(const EpWString& str);

		/*!
		Return the string that is transformed to all capitalized characters
		@param[in] str the sting to transform
		@return the string that is capitalized
		*/
		static EpTString ToUpper(const EpTString& str);

		
		/*!
		Return the string that is transformed to all small-case characters
		@param[in] str the sting to transform
		@return the string that is all small-cased
		*/
		static EpTString ToLower(const EpTString& str);

	};
}
#endif //__EP_LOCALE_H__