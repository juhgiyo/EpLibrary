/*! 
@file epSystem.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date April 17, 2011
@brief System Class Interface
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

An Interface for System Class.

*/
#ifndef __EP_SYSTEM_H__
#define __EP_SYSTEM_H__

#include "epFoundationLib.h"
#include "epAlgorithm.h"

#include <assert.h>
#include <vector>
#include <string.h>

#include "epMemory.h"


/*!
@def EP_WASSERT
@brief Macro Function for assert with message
@param[in] _Expression the boolean expression to evaluate the assert
@param[in] formatString the format string for the message
*/
#if defined(_DEBUG)
#define EP_WASSERT(_Expression,formatString, ...)\
	do{\
	if(!!!(_Expression)){\
	int len=_scwprintf(formatString,__VA_ARGS__)+1;\
	wchar_t * expression=EP_NEW wchar_t[len];\
	swprintf_s(expression,len,formatString,__VA_ARGS__);\
	\
	int len2=_scwprintf(_T("%s\n\nMessage: %s"),_CRT_WIDE(#_Expression),expression )+1;\
	wchar_t * expression2=EP_NEW wchar_t[len2];\
	swprintf_s(expression2,len2,_T("%s\n\nMessage: %s"),_CRT_WIDE(#_Expression),expression);\
	\
	(void)( (_wassert(expression2,__TFILE__, __LINE__), 0) );\
	EP_DELETE[] expression;\
	EP_DELETE[] expression2;\
	}\
	}while(0)
#else
#define EP_WASSERT(_Expression,formatString, ...) ((void)0)
#endif

/*!
@def EP_ASSERT
@brief Macro Function for assert
@param[in] _Expression the boolean expression to evaluate the assert
*/
#define EP_ASSERT _ASSERT

/*!
@def EP_NOTICEBOX
@brief Macro Function for Notice Box
@param[in] formatString the format of the message
*/
#define EP_NOTICEBOX(formatString,...) epl::System::NoticeBox(__TFILE__,__TFUNCTION__,__LINE__,formatString,__VA_ARGS__)

#ifdef StrStr
#undef StrStr
#endif //StrStr

namespace epl
{
	/// A type definition for EpTString Type
	typedef std::string EpString;
	typedef std::wstring EpWString;
#ifdef _UNICODE
	typedef EpWString EpTString;
#else
	typedef EpString EpTString;
#endif

	/// A type definition for EpTime Type
	typedef double EpTime;

	/// A type definition for EpFile Type
	typedef FILE EpFile ;

	/*! 
	@class System epSystem.h
	@brief This is a base class for System  Class

	Implements the System Functions.
	*/
	class EP_FOUNDATION System
	{

	public:
		/*!
		Return the current time.
		@return the current time.
		*/
		static EpTime GetTime();

		/*!
		Return the tick time in millisec.
		@return the tick time in millisec.
		*/
		static unsigned int GetTickCount();

		/*!
		Copy the source buffer to destination buffer.
		@param[in] dest The destination for copying.
		@param[in] dstSize the maximum destination size.
		@param[in] source The source buffer to be copied.
		@param[in] srcSize the size of source to copy.
		@return the resulting buffer.
		*/
		static void* Memcpy (void* dest, unsigned int dstSize, const void* source, unsigned int srcSize);

		/*!
		Copy the source buffer to destination buffer.
		@param[in] dest The destination for copying.
		@param[in] source The source buffer to be copied.
		@param[in] srcSize the size of source to copy.
		@return the resulting buffer.
		*/
		static void* Memcpy (void* dest, const void* source, unsigned int srcSize);

		/*!
		Set the source buffer with given value.
		@param[in] source The source to be set.
		@param[in] val The value to set.
		@param[in] srcSize the size of source.
		@return the resulting buffer.
		*/
		static void* Memset(void* source,int val,unsigned int srcSize);


		/*!
		Copy the current system date to a buffer.
		@param[out] buffer A pointer to a buffer which will be filled in with the formatted date string.
		@param[in] numberOfElements the Maximum number of elements in the buffer.
		@return 0 if successful, otherwise an error code.
		*/
		static int StrDate(char * buffer, unsigned int numberOfElements);

		/*!
		Copy the current time to a buffer.
		@param[out] buffer A pointer to a buffer which will be filled in with the formatted time string.
		@param[in] numberOfElements the Maximum number of elements in the buffer.
		@return 0 if successful, otherwise an error code.
		*/
		static int StrTime(char * buffer, unsigned int numberOfElements);
		
		
		/*!
		Copy the current system date to a buffer.
		@param[out] buffer A pointer to a buffer which will be filled in with the formatted date string.
		@param[in] numberOfElements the Maximum number of elements in the buffer.
		@return 0 if successful, otherwise an error code.
		*/
		static int TcsDate(TCHAR * buffer, unsigned int numberOfElements);

		/*!
		Copy the current time to a buffer.
		@param[out] buffer A pointer to a buffer which will be filled in with the formatted time string.
		@param[in] numberOfElements the Maximum number of elements in the buffer.
		@return 0 if successful, otherwise an error code.
		*/
		static int TcsTime(TCHAR * buffer, unsigned int numberOfElements);



		/*!
		Print the given buffer to the console.
		@param[in] format The format buffer to print out.
		@return number of character printed, or a negative value if an error occurs
		*/
		static int Printf(const char * format, ... );
		
		/*!
		Print the given buffer to the console.
		@param[in] format The format buffer to print out.
		@param[in] args Pointer to a list of arguments.
		@return number of character printed, or a negative value if an error occurs
		*/
		static int Printf_V(const char* format, va_list args);


		/*!
		Print the given buffer to the console.
		@param[in] dest The destination location for output.
		@param[in] sizeOfBuffer Maximum number of characters to store.
		@param[in] format The format buffer to print out.
		@return the number of characters written, not including the terminating null character, or a negative value if an output error occurs.
		*/
		static int SPrintf(char *dest,unsigned int sizeOfBuffer,const char *format,...);

		/*!
		Print the given buffer to the console.
		@param[in] dest The destination location for output.
		@param[in] sizeOfBuffer Maximum number of characters to store.
		@param[in] format The format buffer to print out.
		@param[in] args Pointer to a list of arguments.
		@return the number of characters written, not including the terminating null character, or a negative value if an output error occurs.
		*/
		static int SPrintf_V(char *dest,unsigned int sizeOfBuffer,const char *format,va_list args);
		
				
		/*!
		Print the given buffer to the console.
		@param[in] format The format buffer to print out.
		@return number of character printed, or a negative value if an error occurs
		*/
		static int TPrintf(const TCHAR * format, ... );
		
		/*!
		Print the given buffer to the console.
		@param[in] format The format buffer to print out.
		@param[in] args Pointer to a list of arguments.
		@return number of character printed, or a negative value if an error occurs
		*/
		static int TPrintf_V(const TCHAR* format, va_list args);


		/*!
		Print the given buffer to the console.
		@param[in] dest The destination location for output.
		@param[in] sizeOfBuffer Maximum number of characters to store.
		@param[in] format The format buffer to print out.
		@return the number of characters written, not including the terminating null character, or a negative value if an output error occurs.
		*/
		static int TSPrintf(TCHAR *dest,unsigned int sizeOfBuffer,const TCHAR *format,...);

		/*!
		Print the given buffer to the console.
		@param[in] dest The destination location for output.
		@param[in] sizeOfBuffer Maximum number of characters to store.
		@param[in] format The format buffer to print out.
		@param[in] args Pointer to a list of arguments.
		@return the number of characters written, not including the terminating null character, or a negative value if an output error occurs.
		*/
		static int TSPrintf_V(TCHAR *dest,unsigned int sizeOfBuffer,const TCHAR *format,va_list args);
		
		
		
		/*!
		Return the total  number of characters of the final format string
		@param[in] format The format buffer to find the size in byte.
		@return the number of characters of the final format string
		*/
		static int StrLen(const char *format,...);



		/*!
		Concatenate the source string to destination string.
		@param[in] dest The destination for copying.
		@param[in] dstSize the maximum destination size.
		@param[in] source The source string to be concatenated.
		@return the resulting buffer.
		*/
		static char* StrCat (char* dest, unsigned int dstSize, const char* source);

		/*!
		Copy the source strings to destination string.
		@param[in] dest The destination for copying.
		@param[in] dstSize the maximum destination size.
		@param[in] source The source strings to be copied.
		@param[in] srcSize the number of source strings.
		@return the resulting buffer.
		*/
		static char* StrNCpy (char* dest, unsigned int dstSize, const char* source, unsigned int srcSize);

		/*!
		Tokenize the string by delimiters.
		@param[in] token The string to tokenize.
		@param[in] delimiters the delimiter to tokenize the given string by.
		@param[in] nextToken position to store information between calls to strtok_s.
		@return the resulting buffer.
		*/
		static char* StrTok (char* token, const char* delimiters, char*& nextToken);

		/*!
		Copy source string to destination string, and return the destination string.
		@param[out] dest The Destination String
		@param[in] source The Source String
		@return the Destination String.
		*/
		static char *StrCpy(char *dest, const char*source);

		/*!
		Return a pointer to the first occurrence of a search string in a string.
		@param[in] source the source string to search the substring
		@param[in] subString the search string
		@return a pointer to the first occurrence of a search string in source string
		*/
		static const char *StrStr(const char *source,const char *subString);

		/*!
		Return the total  number of characters of the final format string
		@param[in] format The format buffer to find the size in byte.
		@param[in] args Pointer to a list of arguments.
		@return the number of characters of the final format string
		*/
		static int StrLen_V(const char*format, va_list args);

		
		/*!
		Return the total  number of characters of the final format string
		@param[in] format The format buffer to find the size in byte.
		@return the number of characters of the final format string
		*/
		static int TcsLen(const TCHAR *format,...);



		/*!
		Concatenate the source string to destination string.
		@param[in] dest The destination for copying.
		@param[in] dstSize the maximum destination size.
		@param[in] source The source string to be concatenated.
		@return the resulting buffer.
		*/
		static TCHAR* TcsCat (TCHAR* dest, unsigned int dstSize, const TCHAR* source);

		/*!
		Copy the source strings to destination string.
		@param[in] dest The destination for copying.
		@param[in] dstSize the maximum destination size.
		@param[in] source The source strings to be copied.
		@param[in] srcSize the number of source strings.
		@return the resulting buffer.
		*/
		static TCHAR* TcsNCpy (TCHAR* dest, unsigned int dstSize, const TCHAR* source, unsigned int srcSize);

		/*!
		Tokenize the string by delimiters.
		@param[in] token The string to tokenize.
		@param[in] delimiters the delimiter to tokenize the given string by.
		@param[in] nextToken position to store information between calls to strtok_s.
		@return the resulting buffer.
		*/
		static TCHAR* TcsTok (TCHAR* token, const TCHAR* delimiters, TCHAR*& nextToken);

		/*!
		Copy source string to destination string, and return the destination string.
		@param[out] dest The Destination String
		@param[in] source The Source String
		@return the Destination String.
		*/
		static TCHAR *TcsCpy(TCHAR *dest, const TCHAR*source);

		/*!
		Return a pointer to the first occurrence of a search string in a string.
		@param[in] source the source string to search the substring
		@param[in] subString the search string
		@return a pointer to the first occurrence of a search string in source string
		*/
		static const TCHAR *TcsStr(const TCHAR *source,const TCHAR *subString);

		/*!
		Return the total  number of characters of the final format string
		@param[in] format The format buffer to find the size in byte.
		@param[in] args Pointer to a list of arguments.
		@return the number of characters of the final format string
		*/
		static int TcsLen_V(const TCHAR*format, va_list args);


		/*!
		Print formatted data to a file stream.
		@param[in] fileStream Pointer to FILE structure.
		@param[in] format Format-control string.
		@return the number of bytes written. 
		*/
		static int FPrintf(EpFile* const  fileStream, const char* format,...);
		
		/*!
		Print formatted data to a file stream.
		@param[in] fileStream Pointer to FILE structure.
		@param[in] format Format-control string.
		@param[in] args Pointer to a list of arguments.
		@return the number of bytes written. 
		*/
		static int FPrintf_V(EpFile* const fileStream, const char* format,va_list args);
		
		
		/*!
		Print formatted data to a file stream.
		@param[in] fileStream Pointer to FILE structure.
		@param[in] format Format-control string.
		@return the number of bytes written. 
		*/
		static int TFPrintf(EpFile* const  fileStream, const TCHAR* format,...);
		
		/*!
		Print formatted data to a file stream.
		@param[in] fileStream Pointer to FILE structure.
		@param[in] format Format-control string.
		@param[in] args Pointer to a list of arguments.
		@return the number of bytes written. 
		*/
		static int TFPrintf_V(EpFile* const fileStream, const TCHAR* format,va_list args);
		
		/*!
		Open the file, given by file name according to the mode given.
		@param[out] retFileStream A pointer to the file pointer that will receive the pointer to the opened file.
		@param[in] filename Filename.
		@param[in] mode Type of access permitted.
		@return Zero if successful; an error code on failure. 
		*/
		static int FOpen(EpFile *&retFileStream,const char* filename,const char * mode );
	
		/*!
		Open the file, given by file name according to the mode given.
		@param[out] retFileStream A pointer to the file pointer that will receive the pointer to the opened file.
		@param[in] filename Filename.
		@param[in] mode Type of access permitted.
		@return Zero if successful; an error code on failure. 
		*/
		static int TFOpen(EpFile *&retFileStream,const TCHAR* filename,const TCHAR * mode );
		
		/*!
		Closes the given file stream.
		@param[in] fileStream Pointer to FILE structure.
		@return 0 if the stream is successfully closed.
		*/
		static int FClose(EpFile * const fileStream);
		
		/*!
		Write the given buffer to the given file stream.
		@param[in] buffer Pointer to data to be written.
		@param[in] size Item size in bytes.
		@param[in] count Maximum number of items to be written.
		@param[in] fileStream Pointer to FILE structure.
		@return the number of full items actually written.
		*/
		static size_t FWrite(const void* buffer,unsigned int size, unsigned int count, EpFile * const fileStream);
		
		/*!
		Read from the given file stream and write to the given buffer.
		@param[out] retBuff Pointer to data to be read.
		@param[in] size Item size in bytes.
		@param[in] count Maximum number of items to be read.
		@param[in] fileStream Pointer to FILE structure.
		@return the number of full items actually read.
		*/
		static int FRead(void *retBuff,unsigned int size, unsigned int count,EpFile * const fileStream);

		/*!
		Return the file size in byte.
		@param[in] fileStream Pointer to FILE structure.
		@return the file size in byte.
		*/
		static int FSize(EpFile* const fileStream);
	
		/*!
		Return the last error message code
		@return the last occurred error message code.
		*/
		static int GetLastError();

		/*!
		Format the Last Error Code to String
		@param[out] retBuff the buffer to put the human-readable error message.
		@param[in] maxElementCount the number of element in the buffer
		@return if succeeds, the number of TCHARs stored in the output buffer, excluding the terminating null character otherwise zero.
		*/
		static unsigned long FormatLastErrorMessage(TCHAR *retBuff, const unsigned int maxElementCount);

		/*!
		Show Notice Message Box with message given including File Name, Function Name, and Line Number.
		@param[in] fileName the file name of this function called.
		@param[in] funcName the function name of this function called.
		@param[in] lineNum the line number of this function called.
		@param[in] format the format of the message
		@return if succeeds, one of the menu-item values otherwise zero.
		*/
		static int NoticeBox(const TCHAR* fileName, const TCHAR* funcName,const unsigned int lineNum,const TCHAR* format,...);

		/*!
		Convert hexadecimal value of given array to EpTString.
		@param[in] buff pointer to array.
		@param[in] len length of given array.
		@return EpTString that contains hexadecimal value.
		*/
		static EpTString HexToString(const unsigned char *buff, unsigned int len);

		/*!
		Convert MultiByte String to WideChar String.
		@param[in] multiByteCharString the string to convert to to WideChar string.
		@param[in] stringLength length of the MultiByte string excluding the terminating NULL.
		@return EpTString that contains converted WideChar String.
		*/
		static EpWString MultiByteToWideChar(const char *multiByteCharString, unsigned int stringLength);

		/*!
		Convert MultiByte String to WideChar String.
		@param[in] multiByteCharString the string to convert to to WideChar string.
		@return EpTString that contains converted WideChar String.
		*/
		static EpWString MultiByteToWideChar(const char *multiByteCharString);


		/*!
		Convert MultiByte String to WideChar String.
		@param[in] multiByteCharString the MultiByte string to convert to WideChar string.
		@param[in] stringLength length of the MultiByte string excluding the terminating NULL.
		@param[out] retWideCharString string that contains converted WideChar String.
		@return the result status of the conversion.
		*/
		static int MultiByteToWideChar(const char *multiByteCharString, unsigned int stringLength, wchar_t *retWideCharString);

				/*!
		Convert MultiByte String to WideChar String.
		@param[in] multiByteCharString the MultiByte string to convert to WideChar string.
		@param[out] retWideCharString string that contains converted WideChar String.
		@return the result status of the conversion.
		*/
		static int MultiByteToWideChar(const char *multiByteCharString, wchar_t *retWideCharString);
	
		/*!
		Convert WideChar String to MultiByte String.
		@param[in] wideCharString the WideChar String to convert to MultiByte string.
		@param[out] retMultiByteString string that contains converted MultiByte String.
		@return the number of bytes written into the MultiByte output string, excluding the terminating NULL .
		*/
		static unsigned int  WideCharToMultiByte(const wchar_t* wideCharString, char *retMultiByteString);

		/*!
		Convert WideChar String to MultiByte String.
		@param[in] wideCharString the WideChar String to convert to MultiByte string.
		@param[in] stringLength length of the MultiByte string excluding the terminating NULL.
		@param[out] retMultiByteString string that contains converted MultiByte String.
		@return the number of bytes written into the MultiByte output string, excluding the terminating NULL .
		*/
		static unsigned int  WideCharToMultiByte(const wchar_t* wideCharString,unsigned int stringLength, char *retMultiByteString);
	};

}
#endif //__EP_SYSTEM_H__