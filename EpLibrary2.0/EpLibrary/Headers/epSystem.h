/*! 
@file epSystem.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
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

#include "epLib.h"
#include "epAlgorithm.h"

#include <vector>
#include <string>

#include "epMemory.h"
#include "epAssert.h"
#include "epLocale.h"

/*!
@def EP_NOTICEBOX
@brief Macro Function for Notice Box
@param[in] formatString the format of the message
*/
#if defined(_DEBUG)
#define EP_NOTICEBOX(formatString,...) epl::System::NoticeBox(__TFILE__,__TFUNCTION__,__LINE__,formatString,__VA_ARGS__)
#else
#define EP_NOTICEBOX(formatString,...) ((void)0)
#endif

#ifdef StrStr
#undef StrStr
#endif //StrStr

	/*!
	@def WAITTIME_INIFINITE
	@brief infinitely wait for the object

	Macro for infinite wait time.
	*/
#define WAITTIME_INIFINITE 0xFFFFFFFF  // Infinite timeout

	/*!
	@def WAITTIME_IGNORE
	@brief no waiting time

	Macro no waiting time.
	*/
#define WAITTIME_IGNORE    0  // Ignore waiting

namespace epl
{


	/// A type definition for EpTime Type
	typedef double EpTime;

	/// A type definition for EpFile Type
	typedef FILE EpFile ;

	/// A type definition for EpErrno Type
	typedef int EpErrno;

#ifdef  _WIN64
	typedef __int64    ssize_t;
#else
	typedef int   ssize_t;
#endif

	/*! 
	@class System epSystem.h
	@brief This is a base class for System  Class

	Implements the System Functions.
	*/
	class EP_LIBRARY System
	{

	public:

		/*!
		Return the System information.
		@return the System information..
		*/
		static SYSTEM_INFO GetSystemInfo();

		/*!
		Return the number of cores of running computer.
		@return the number of cores of running computer.
		*/
		static unsigned long GetNumberOfCores();

		/*!
		Return the time in milliseconds from first call of GetTime.
		@return the time in milliseconds from first call of GetTime.
		@remark this is not a current time like GetLocalTime or _ftime
		@remark Use DateTimeHelper::GetCurrentDateTime instead to get current time.
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
		@param[in] dstSizeInByte the maximum destination size.
		@param[in] source The source buffer to be copied.
		@param[in] srcSizeInByte the size of source to copy.
		@return the resulting buffer.
		*/
		static void* Memcpy (void* dest, size_t dstSizeInByte, const void* source, size_t srcSizeInByte);

		/*!
		Copy the source buffer to destination buffer.
		@param[in] dest The destination for copying.
		@param[in] source The source buffer to be copied.
		@param[in] srcSizeInByte the size of source to copy.
		@return the resulting buffer.
		*/
		static void* Memcpy (void* dest, const void* source, size_t srcSizeInByte);

		
		/*!
		Compare the two buffers.
		@param[in] buf1 The first buffer to compare.
		@param[in] buf2 The second buffer to compare
		@param[in] compSizeInByte the size of buffer to compare.
		@return 0 if values of both buffers are equal <br/>
		        >0 if buf1 is greater than buf2 <br/>
				<0 if buf1 is less than buf2
		*/
		static int Memcmp (void* buf1, const void* buf2, size_t compSizeInByte);

		/*!
		Set the source buffer with given value.
		@param[in] source The source to be set.
		@param[in] val The value to set.
		@param[in] srcSizeInByte the size of source.
		@return the resulting buffer.
		*/
		static void* Memset(void* source,int val,size_t srcSizeInByte);


		/*!
		Copy the current system date to a buffer.
		@param[out] buffer A pointer to a buffer which will be filled in with the formatted date string.
		@param[in] numberOfElements the Maximum number of elements in the buffer.
		@return 0 if successful, otherwise an error code.
		*/
		static EpErrno StrDate(char * buffer, size_t numberOfElements);

		/*!
		Copy the current time to a buffer.
		@param[out] buffer A pointer to a buffer which will be filled in with the formatted time string.
		@param[in] numberOfElements the Maximum number of elements in the buffer.
		@return 0 if successful, otherwise an error code.
		*/
		static EpErrno StrTime(char * buffer, size_t numberOfElements);

		/*!
		Copy the current time to a buffer.
		@param[out] buffer A pointer to a buffer which will be filled in with the formatted time string.
		@param[in] numberOfElements the Maximum number of elements in the buffer.
		@return 0 if successful, otherwise an error code.
		*/
		static EpErrno WStrTime(wchar_t * buffer, size_t numberOfElements);
		
				/*!
		Copy the current system date to a buffer.
		@param[out] buffer A pointer to a buffer which will be filled in with the formatted date string.
		@param[in] numberOfElements the Maximum number of elements in the buffer.
		@return 0 if successful, otherwise an error code.
		*/
		static EpErrno WStrDate(wchar_t * buffer, size_t numberOfElements);

		/*!
		Copy the current system date to a buffer.
		@param[out] buffer A pointer to a buffer which will be filled in with the formatted date string.
		@param[in] numberOfElements the Maximum number of elements in the buffer.
		@return 0 if successful, otherwise an error code.
		*/
		static EpErrno TcsDate(TCHAR * buffer, size_t numberOfElements);

		/*!
		Copy the current time to a buffer.
		@param[out] buffer A pointer to a buffer which will be filled in with the formatted time string.
		@param[in] numberOfElements the Maximum number of elements in the buffer.
		@return 0 if successful, otherwise an error code.
		*/
		static EpErrno TcsTime(TCHAR * buffer, size_t numberOfElements);


		/*!
		Perform a lowercase comparison of string1 and string2
		@param[in] str1 string1
		@param[in] str2 string2
		@return return <br/>
		        <0 if string1 less than string2 <br/>
		        0  if string1 identical to string2 <br/>
				>0 if string1 greater than string2		 
		*/
		static int StrICmp(const char * str1, const char *str2);

		/*!
		Compare string1 and string2
		@param[in] str1 string1
		@param[in] str2 string2
		@return return <br/>
		        <0 if string1 less than string2 <br/>
		        0  if string1 identical to string2 <br/>
				>0 if string1 greater than string2		 
		*/
		static int StrCmp(const char * str1, const char *str2);

		/*!
		Perform a lowercase comparison of string1 and string2
		@param[in] str1 string1
		@param[in] str2 string2
		@return return <br/>
		        <0 if string1 less than string2 <br/>
		        0  if string1 identical to string2 <br/>
				>0 if string1 greater than string2		 
		*/
		static int MbsICmp(const unsigned char * str1, const unsigned char *str2);

		/*!
		Compare string1 and string2
		@param[in] str1 string1
		@param[in] str2 string2
		@return return <br/>
		        <0 if string1 less than string2 <br/>
		        0  if string1 identical to string2 <br/>
				>0 if string1 greater than string2		 
		*/
		static int MbsCmp(const unsigned char * str1, const unsigned char *str2);

		/*!
		Perform a lowercase comparison of string1 and string2
		@param[in] str1 string1
		@param[in] str2 string2
		@return return <br/>
		        <0 if string1 less than string2 <br/>
		        0  if string1 identical to string2 <br/>
				>0 if string1 greater than string2		 
		*/
		static int WStrICmp(const wchar_t * str1, const wchar_t *str2);

		/*!
		Compare string1 and string2
		@param[in] str1 string1
		@param[in] str2 string2
		@return return <br/>
		        <0 if string1 less than string2 <br/>
		        0  if string1 identical to string2 <br/>
				>0 if string1 greater than string2		 
		*/
		static int WStrCmp(const wchar_t * str1, const wchar_t *str2);
		
		/*!
		Perform a lowercase comparison of string1 and string2
		@param[in] str1 string1
		@param[in] str2 string2
		@return return <br/>
		        <0 if string1 less than string2 <br/>
		        0  if string1 identical to string2 <br/>
				>0 if string1 greater than string2		 
		*/
		static int TcsICmp(const TCHAR * str1, const TCHAR *str2);

		/*!
		Compare string1 and string2
		@param[in] str1 string1
		@param[in] str2 string2
		@return return <br/>
		        <0 if string1 less than string2 <br/>
		        0  if string1 identical to string2 <br/>
				>0 if string1 greater than string2		 
		*/
		static int TcsCmp(const TCHAR * str1, const TCHAR *str2);


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
		@param[in] dstSizeInCharacter Maximum number of characters to store.
		@param[in] format The format buffer to print out.
		@return the number of characters written, not including the terminating null character, or a negative value if an output error occurs.
		*/
		static int SPrintf(char *dest,size_t dstSizeInCharacter,const char *format,...);

		/*!
		Print the given buffer to the console.
		@param[in] dest The destination location for output.
		@param[in] dstSizeInCharacter Maximum number of characters to store.
		@param[in] format The format buffer to print out.
		@param[in] args Pointer to a list of arguments.
		@return the number of characters written, not including the terminating null character, or a negative value if an output error occurs.
		*/
		static int SPrintf_V(char *dest,size_t dstSizeInCharacter,const char *format,va_list args);

				/*!
		Print the given buffer to the console.
		@param[out] retDest The destination location for output.
		@param[in] format The format buffer to print out.
		@return the number of characters written, not including the terminating null character, or a negative value if an output error occurs.
		*/
		static int SPrintf(EpString &retDest,const char *format,...);

		/*!
		Print the given buffer to the console.
		@param[out] retDest The destination location for output.
		@param[in] format The format buffer to print out.
		@param[in] args Pointer to a list of arguments.
		@return the number of characters written, not including the terminating null character, or a negative value if an output error occurs.
		*/
		static int SPrintf_V(EpString &retDest,const char *format,va_list args);
		
		
		/*!
		Print the given buffer to the console.
		@param[in] format The format buffer to print out.
		@return number of character printed, or a negative value if an error occurs
		*/
		static int WPrintf(const wchar_t * format, ... );
		
		/*!
		Print the given buffer to the console.
		@param[in] format The format buffer to print out.
		@param[in] args Pointer to a list of arguments.
		@return number of character printed, or a negative value if an error occurs
		*/
		static int WPrintf_V(const wchar_t* format, va_list args);


		/*!
		Print the given buffer to the console.
		@param[out] dest The destination location for output.
		@param[in] dstSizeInCharacter Maximum number of characters to store.
		@param[in] format The format buffer to print out.
		@return the number of characters written, not including the terminating null character, or a negative value if an output error occurs.
		*/
		static int SWPrintf(wchar_t *dest,size_t dstSizeInCharacter,const wchar_t *format,...);

		/*!
		Print the given buffer to the console.
		@param[out] dest The destination location for output.
		@param[in] dstSizeInCharacter Maximum number of characters to store.
		@param[in] format The format buffer to print out.
		@param[in] args Pointer to a list of arguments.
		@return the number of characters written, not including the terminating null character, or a negative value if an output error occurs.
		*/
		static int SWPrintf_V(wchar_t *dest,size_t dstSizeInCharacter,const wchar_t *format,va_list args);

		/*!
		Print the given buffer to the console.
		@param[out] retDest The destination location for output.
		@param[in] format The format buffer to print out.
		@return the number of characters written, not including the terminating null character, or a negative value if an output error occurs.
		*/
		static int SWPrintf(EpWString &retDest,const wchar_t *format,...);

		/*!
		Print the given buffer to the console.
		@param[out] retDest The destination location for output.
		@param[in] format The format buffer to print out.
		@param[in] args Pointer to a list of arguments.
		@return the number of characters written, not including the terminating null character, or a negative value if an output error occurs.
		*/
		static int SWPrintf_V(EpWString &retDest,const wchar_t *format,va_list args);
		


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
		@param[out] dest The destination location for output.
		@param[in] dstSizeInCharacter Maximum number of characters to store.
		@param[in] format The format buffer to print out.
		@return the number of characters written, not including the terminating null character, or a negative value if an output error occurs.
		*/
		static int STPrintf(TCHAR *dest,size_t dstSizeInCharacter,const TCHAR *format,...);

		/*!
		Print the given buffer to the console.
		@param[out] dest The destination location for output.
		@param[in] dstSizeInCharacter Maximum number of characters to store.
		@param[in] format The format buffer to print out.
		@param[in] args Pointer to a list of arguments.
		@return the number of characters written, not including the terminating null character, or a negative value if an output error occurs.
		*/
		static int STPrintf_V(TCHAR *dest,size_t dstSizeInCharacter,const TCHAR *format,va_list args);
		
		/*!
		Print the given buffer to the console.
		@param[out] retDest The destination location for output.
		@param[in] format The format buffer to print out.
		@return the number of characters written, not including the terminating null character, or a negative value if an output error occurs.
		*/
		static int STPrintf(EpTString &retDest,const TCHAR *format,...);

		/*!
		Print the given buffer to the console.
		@param[out] retDest The destination location for output.
		@param[in] format The format buffer to print out.
		@param[in] args Pointer to a list of arguments.
		@return the number of characters written, not including the terminating null character, or a negative value if an output error occurs.
		*/
		static int STPrintf_V(EpTString &retDest,const TCHAR *format,va_list args);

		
		
		/*!
		Return the total  number of characters of the final format string
		@param[in] format The format buffer to find the size in byte.
		@return the number of characters of the final format string
		*/
		static int StrLen(const char *format,...);

		/*!
		Concatenate the source string to destination string.
		@param[in] dest The destination for copying.
		@param[in] dstSizeInCharacter the maximum destination size.
		@param[in] source The source string to be concatenated.
		@return the resulting buffer.
		*/
		static char* StrCat (char* dest, size_t dstSizeInCharacter, const char* source);

		/*!
		Copy the source strings to destination string.
		@param[in] dest The destination for copying.
		@param[in] dstSizeInCharacter the maximum destination size.
		@param[in] source The source strings to be copied.
		@param[in] srcSizeInCharacter the number of source strings.
		@return the resulting buffer.
		*/
		static char* StrNCpy (char* dest, size_t dstSizeInCharacter, const char* source, size_t srcSizeInCharacter);

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
		static int MbsLen(const unsigned char *format,...);

		/*!
		Concatenate the source string to destination string.
		@param[in] dest The destination for copying.
		@param[in] dstSizeInCharacter the maximum destination size.
		@param[in] source The source string to be concatenated.
		@return the resulting buffer.
		*/
		static unsigned char* MbsCat (unsigned char* dest, size_t dstSizeInCharacter, const unsigned char* source);

		/*!
		Copy the source strings to destination string.
		@param[in] dest The destination for copying.
		@param[in] dstSizeInCharacter the maximum destination size.
		@param[in] source The source strings to be copied.
		@param[in] srcSizeInCharacter the number of source strings.
		@return the resulting buffer.
		*/
		static unsigned char* MbsNCpy (unsigned char* dest, size_t dstSizeInCharacter, const unsigned char* source, size_t srcSizeInCharacter);

		/*!
		Tokenize the string by delimiters.
		@param[in] token The string to tokenize.
		@param[in] delimiters the delimiter to tokenize the given string by.
		@param[in] nextToken position to store information between calls to strtok_s.
		@return the resulting buffer.
		*/
		static unsigned char* MbsTok (unsigned char* token, const unsigned char* delimiters, unsigned char*& nextToken);

		/*!
		Copy source string to destination string, and return the destination string.
		@param[out] dest The Destination String
		@param[in] source The Source String
		@return the Destination String.
		*/
		static unsigned char *MbsCpy(unsigned char *dest, const unsigned char*source);

		/*!
		Return a pointer to the first occurrence of a search string in a string.
		@param[in] source the source string to search the substring
		@param[in] subString the search string
		@return a pointer to the first occurrence of a search string in source string
		*/
		static const unsigned char *MbsStr(const unsigned char *source,const unsigned char *subString);

		/*!
		Return the total  number of characters of the final format string
		@param[in] format The format buffer to find the size in byte.
		@param[in] args Pointer to a list of arguments.
		@return the number of characters of the final format string
		*/
		static int MbsLen_V(const unsigned char*format, va_list args);

		/*!
		Return the total  number of characters of the final format string
		@param[in] format The format buffer to find the size in byte.
		@return the number of characters of the final format string
		*/
		static int WcsLen(const wchar_t *format,...);

		/*!
		Concatenate the source string to destination string.
		@param[in] dest The destination for copying.
		@param[in] dstSizeInCharacter the maximum destination size.
		@param[in] source The source string to be concatenated.
		@return the resulting buffer.
		*/
		static wchar_t* WcsCat (wchar_t* dest, size_t dstSizeInCharacter, const wchar_t* source);

		/*!
		Copy the source strings to destination string.
		@param[in] dest The destination for copying.
		@param[in] dstSizeInCharacter the maximum destination size.
		@param[in] source The source strings to be copied.
		@param[in] srcSizeInCharacter the number of source strings.
		@return the resulting buffer.
		*/
		static wchar_t* WcsNCpy (wchar_t* dest, size_t dstSizeInCharacter, const wchar_t* source, size_t srcSizeInCharacter);

		/*!
		Tokenize the string by delimiters.
		@param[in] token The string to tokenize.
		@param[in] delimiters the delimiter to tokenize the given string by.
		@param[in] nextToken position to store information between calls to strtok_s.
		@return the resulting buffer.
		*/
		static wchar_t* WcsTok (wchar_t* token, const wchar_t* delimiters, wchar_t*& nextToken);

		/*!
		Copy source string to destination string, and return the destination string.
		@param[out] dest The Destination String
		@param[in] source The Source String
		@return the Destination String.
		*/
		static wchar_t *WcsCpy(wchar_t *dest, const wchar_t*source);

		/*!
		Return a pointer to the first occurrence of a search string in a string.
		@param[in] source the source string to search the substring
		@param[in] subString the search string
		@return a pointer to the first occurrence of a search string in source string
		*/
		static const wchar_t *WcsStr(const wchar_t *source,const wchar_t *subString);

		/*!
		Return the total  number of characters of the final format string
		@param[in] format The format buffer to find the size in byte.
		@param[in] args Pointer to a list of arguments.
		@return the number of characters of the final format string
		*/
		static int WcsLen_V(const wchar_t*format, va_list args);



		
		/*!
		Return the total  number of characters of the final format string
		@param[in] format The format buffer to find the size in byte.
		@return the number of characters of the final format string
		*/
		static int TcsLen(const TCHAR *format,...);

		/*!
		Concatenate the source string to destination string.
		@param[in] dest The destination for copying.
		@param[in] dstSizeInCharacter the maximum destination size.
		@param[in] source The source string to be concatenated.
		@return the resulting buffer.
		*/
		static TCHAR* TcsCat (TCHAR* dest, size_t dstSizeInCharacter, const TCHAR* source);

		/*!
		Copy the source strings to destination string.
		@param[in] dest The destination for copying.
		@param[in] dstSizeInCharacter the maximum destination size.
		@param[in] source The source strings to be copied.
		@param[in] srcSizeInCharacter the number of source strings.
		@return the resulting buffer.
		*/
		static TCHAR* TcsNCpy (TCHAR* dest, size_t dstSizeInCharacter, const TCHAR* source, size_t srcSizeInCharacter);

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
		static int FWPrintf(EpFile* const  fileStream, const wchar_t* format,...);
		
		/*!
		Print formatted data to a file stream.
		@param[in] fileStream Pointer to FILE structure.
		@param[in] format Format-control string.
		@param[in] args Pointer to a list of arguments.
		@return the number of bytes written. 
		*/
		static int FWPrintf_V(EpFile* const fileStream, const wchar_t* format,va_list args);
		
		
		/*!
		Print formatted data to a file stream.
		@param[in] fileStream Pointer to FILE structure.
		@param[in] format Format-control string.
		@return the number of bytes written. 
		*/
		static int FTPrintf(EpFile* const  fileStream, const TCHAR* format,...);
		
		/*!
		Print formatted data to a file stream.
		@param[in] fileStream Pointer to FILE structure.
		@param[in] format Format-control string.
		@param[in] args Pointer to a list of arguments.
		@return the number of bytes written. 
		*/
		static int FTPrintf_V(EpFile* const fileStream, const TCHAR* format,va_list args);
		
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
		static int FWOpen(EpFile *&retFileStream,const wchar_t* filename,const wchar_t * mode );
	

		/*!
		Open the file, given by file name according to the mode given.
		@param[out] retFileStream A pointer to the file pointer that will receive the pointer to the opened file.
		@param[in] filename Filename.
		@param[in] mode Type of access permitted.
		@return Zero if successful; an error code on failure. 
		*/
		static int FTOpen(EpFile *&retFileStream,const TCHAR* filename,const TCHAR * mode );
		
		/*!
		Closes the given file stream.
		@param[in] fileStream Pointer to FILE structure.
		@return 0 if the stream is successfully closed.
		*/
		static int FClose(EpFile * const fileStream);
		
		/*!
		Write the given buffer to the given file stream.
		@param[in] buffer Pointer to data to be written.
		@param[in] sizeInByte Item size in bytes.
		@param[in] count Maximum number of items to be written.
		@param[in] fileStream Pointer to FILE structure.
		@return the number of full items actually written.
		*/
		static size_t FWrite(const void* buffer,size_t sizeInByte, size_t count, EpFile * const fileStream);
		
		/*!
		Read from the given file stream and write to the given buffer.
		@param[out] retBuff Pointer to data to be read.
		@param[in] sizeInByte Item size in bytes.
		@param[in] count Maximum number of items to be read.
		@param[in] fileStream Pointer to FILE structure.
		@return the number of full items actually read.
		*/
		static size_t FRead(void *retBuff,size_t sizeInByte, size_t count,EpFile * const fileStream);

		/*!
		Return the file size in byte.
		@param[in] fileStream Pointer to FILE structure.
		@return the file size in byte.
		*/
		static long FSize(EpFile* const fileStream);
			
		/*!
		Return the last error message code
		@return the last occurred error message code.
		*/
		static unsigned long GetLastError();

		/*!
		Format the Last Error Code to String
		@param[out] retBuff the buffer to put the human-readable error message.
		@param[in] maxElementCount the number of element in the buffer
		@param[out] retErrNo the last error number
		@return if succeeds, the number of TCHARs stored in the output buffer, excluding the terminating null character otherwise zero.
		*/
		static unsigned long FormatLastErrorMessage(TCHAR *retBuff, unsigned long maxElementCount,unsigned long *retErrNo=NULL);

		/*!
		Format the Last Error Code to String
		@param[out] retString the buffer to put the human-readable error message.
		@param[out] retErrNo the last error number
		@return if succeeds, the number of TCHARs stored in the output buffer, excluding the terminating null character otherwise zero.
		*/
		static unsigned long FormatLastErrorMessage(EpWString &retString,unsigned long *retErrNo) ;
		/*!
		Format the Last Error Code to String
		@param[out] retString the buffer to put the human-readable error message.
		@param[out] retErrNo the last error number
		@return if succeeds, the number of TCHARs stored in the output buffer, excluding the terminating null character otherwise zero.
		*/
		static unsigned long FormatLastErrorMessage(EpString &retString,unsigned long *retErrNo) ;

		/*!
		Show Notice Message Box with message given including File Name, Function Name, and Line Number.
		@param[in] fileName the file name of this function called.
		@param[in] funcName the function name of this function called.
		@param[in] lineNum the line number of this function called.
		@param[in] format the format of the message
		@return if succeeds, one of the menu-item values otherwise zero.
		*/
		static int NoticeBox(const TCHAR* fileName, const TCHAR* funcName,size_t lineNum,const TCHAR* format,...);

		/*!
		Convert hexadecimal value of given array to EpTString.
		@param[in] buff pointer to array.
		@param[in] len length of given array.
		@return EpTString that contains hexadecimal value.
		*/
		static EpTString HexToString(const unsigned char *buff, size_t len);

		/*!
		Convert MultiByte String to WideChar String. [ANSI -> UTF-16]
		@param[in] multiByteCharString the string to convert to WideChar string.
		@param[in] stringLength length of the MultiByte string excluding the terminating NULL.
		@return EpWString that contains converted WideChar String.
		*/
		static EpWString MultiByteToWideChar(const char *multiByteCharString, int stringLength);

		/*!
		Convert MultiByte String to WideChar String. [ANSI -> UTF-16]
		@param[in] multiByteCharString the string to convert to WideChar string.
		@return EpWString that contains converted WideChar String.
		*/
		static EpWString MultiByteToWideChar(const char *multiByteCharString);

		/*!
		Convert MultiByte String to WideChar String. [ANSI -> UTF-16]
		@param[in] multiByteCharString the MultiByte string to convert to WideChar string.
		@param[in] stringLength length of the MultiByte string excluding the terminating NULL.
		@param[out] retWideCharString string that contains converted WideChar String.
		@return the result status of the conversion.
		*/
		static int MultiByteToWideChar(const char *multiByteCharString, int stringLength, wchar_t *retWideCharString);

		/*!
		Convert MultiByte String to WideChar String. [ANSI -> UTF-16]
		@param[in] multiByteCharString the MultiByte string to convert to WideChar string.
		@param[out] retWideCharString string that contains converted WideChar String.
		@return the result status of the conversion.
		*/
		static int MultiByteToWideChar(const char *multiByteCharString, wchar_t *retWideCharString);
	

		/*!
		Convert WideChar String to MultiByte String. [UTF-16 -> ANSI]
		@param[in] wideCharString the WideChar String to convert to MultiByte string.
		@param[out] retMultiByteString string that contains converted MultiByte String.
		@return the number of bytes written into the MultiByte output string, excluding the terminating NULL .
		*/
		static int  WideCharToMultiByte(const wchar_t* wideCharString, char *retMultiByteString);

		/*!
		Convert WideChar String to MultiByte String. [UTF-16 -> ANSI]
		@param[in] wideCharString the WideChar String to convert to MultiByte string.
		@param[in] stringLength length of the MultiByte string excluding the terminating NULL.
		@param[out] retMultiByteString string that contains converted MultiByte String.
		@return the number of bytes written into the MultiByte output string, excluding the terminating NULL .
		*/
		static int  WideCharToMultiByte(const wchar_t* wideCharString,int stringLength, char *retMultiByteString);


		/*!
		Convert WideChar String to MultiByte String. [UTF-16 -> ANSI]
		@param[in] wideCharString the string to convert to MultiByte string.
		@param[in] stringLength length of the MultiByte string excluding the terminating NULL.
		@return EpString that contains converted MultiByte String.
		*/
		static EpString WideCharToMultiByte(const wchar_t *wideCharString,int stringLength);

		/*!
		Convert MultiByte String to WideChar String. [UTF-16 -> ANSI]
		@param[in] wideCharString the string to convert to MultiByte string.
		@return EpString that contains converted MultiByte String.
		*/
		static EpString WideCharToMultiByte(const wchar_t *wideCharString);


		/*!
		Convert UTF-16 String to UTF-8 String. [UTF-16 -> UTF-8]
		@param[in] utf16String the UTF-16 String to convert to UTF-8 string.
		@param[out] retutf8String string that contains converted UTF-8 String.
		@return the number of bytes written into the MultiByte output string, excluding the terminating NULL .
		*/
		static int  UTF16ToUTF8(const wchar_t* utf16String, char *retutf8String);

		/*!
		Convert UTF-16 String to UTF-8 String. [UTF-16 -> UTF-8]
		@param[in] utf16String the UTF-16 String to convert to UTF-8 string.
		@param[in] stringLength length of the UTF-8 string excluding the terminating NULL.
		@param[out] retutf8String string that contains converted UTF-8 String.
		@return the number of bytes written into the UTF-8 output string, excluding the terminating NULL .
		*/
		static int  UTF16ToUTF8(const wchar_t* utf16String,int stringLength, char *retutf8String);


		/*!
		Convert UTF-16 String to UTF-8 String. [UTF-16 -> UTF-8]
		@param[in] utf16String the UTF-16 String to convert to UTF-8 string.
		@param[in] stringLength length of the UTF-8 string excluding the terminating NULL.
		@return EpString that contains converted UTF-8 String.
		*/
		static EpString UTF16ToUTF8(const wchar_t *utf16String, int stringLength);

		/*!
		Convert UTF-16 String to UTF-8 String. [UTF-16 -> UTF-8]
		@param[in] utf16String the UTF-16 String to convert to UTF-8 string.
		@return EpString that contains converted UTF-8 String.
		*/
		static EpString UTF16ToUTF8(const wchar_t *utf16String);


		/*!
		Convert UTF-8 String to UTF-16 String. [UTF-8 -> UTF-16]
		@param[in] utf8String the string to convert to UTF-16 string.
		@param[in] stringLength length of the UTF-8 string excluding the terminating NULL.
		@return EpWString that contains converted UTF-16 String.
		*/
		static EpWString UTF8ToUTF16(const char *utf8String, int stringLength);

		/*!
		Convert UTF-8 String to UTF-16 String. [UTF-8 -> UTF-16]
		@param[in] utf8String the UTF-8 String to convert to UTF-16 string.
		@return EpString that contains converted UTF-16 String.
		*/
		static EpWString UTF8ToUTF16(const char *utf8String);

		/*!
		Convert UTF-8 String to UTF-16 String. [UTF-8 -> UTF-16]
		@param[in] utf8String the UTF-8 string to convert to UTF-16 string.
		@param[in] stringLength length of the UTF-8 string excluding the terminating NULL.
		@param[out] retUtf16String string that contains converted UTF-16 String.
		@return the result status of the conversion.
		*/
		static int UTF8ToUTF16(const char *utf8String,int stringLength, wchar_t *retUtf16String);

		/*!
		Convert UTF-8 String to UTF-16 String. [UTF-8 -> UTF-16]
		@param[in] utf8String the UTF-8 string to convert to UTF-16 string.
		@param[out] retUtf16String string that contains converted UTF-16 String.
		@return the result status of the conversion.
		*/
		static int UTF8ToUTF16(const char *utf8String, wchar_t *retUtf16String);
	
		/*!
		Check if the string contains any multi-byte character
		@param[in] multiByteString unknown string.
		@param[out] byteLength byte size of the given string.
		@return true if string contains multi-byte character, otherwise false.
		*/
		static bool IsMultiByte(byte *multiByteString, size_t byteLength);

		/*!
		Output the debug message
		@param[in] format the string to output
		*/
		static void OutputDebugString(TCHAR * format,...);

		/*!
		Wait for a handle for given time
		@param[in] handle the handle to wait for
		@param[in] milliSec the time for waiting (WAITTIME_INIFINITE to wait forever)
		@return one of (WAIT_ABANDONED, WAIT_OBJECT_0, WAIT_TIMEOUT)
		*/
		static unsigned long WaitForSingleObject(HANDLE handle,unsigned long milliSec);

		/*!
		Return the current thread handle
		@return current thread handle
		*/
		static HANDLE GetCurrentThread();

		/*!
		Terminate the given thread with given exit code
		@param[in] threadHandle thread to terminate
		@param[in] exitCode the eixt code for the thread to exit
		@return result of the termination
		*/
		static long TerminateThread(HANDLE threadHandle, unsigned long exitCode);
	};

}
#endif //__EP_SYSTEM_H__