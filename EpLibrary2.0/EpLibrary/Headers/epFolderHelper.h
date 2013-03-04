/*! 
@file epFolderHelper.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief File System Folder Processing Function Class Interface
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

An Interface for the File System Folder Processing Operation.

*/
#ifndef __EP_FOLDER_HELPER_H__
#define __EP_FOLDER_HELPER_H__
#include "epLib.h"
#include "epSystem.h"
namespace epl
{
	/*! 
	@class FolderHelper epFolderHelper.h
	@brief This is a class for Folder Processing Class

	Implements the Folder Processing Functions.
	*/
	class EP_LIBRARY FolderHelper
	{

	public:
		/*!
		Show the Choose Folder Dialog and
		return true if successfully folder 
		is chosen with chosen folder path.
		@param[in] hParent the window handle of parent to start the choose folder dialog
		@param[in] title the title of the choose folder dialog
		@param[out] retFolderPath the chosen folder path
		@return true if the folder is successfully chosen, otherwise false
		*/
		static bool ChooseFolder(HWND hParent, const TCHAR * title, EpTString &retFolderPath);

		/*!
		Check if the given path exists
		@param[in] path the file path to check
		@return true if the folder exists, otherwise false
		*/
		static bool IsPathExist(const TCHAR * path);


		/*!
		Create given folder path from file system
		@param[in] strPath the file path to create
		@return true if the folder is created successfully, otherwise false
		*/
		static bool CreateFolder(const TCHAR * strPath);

		/*!
		Delete given folder path from file system
		@param[in] strPath the file path to delete
		@return true if the folder is deleted successfully, otherwise false
		*/
		static void DeleteFolder(const TCHAR *strPath);

		/*!
		Get Special Folder Path String
		@param[in] csidl the csidl enumeration
		@param[in] isCreateIfNotExist if true and path not exist then create the path
		@param[out] retPath the returning path string
		@return true if the folder exists, otherwise false
		*/
		static bool GetSpecialFolderPath(int csidl,bool isCreateIfNotExist,EpTString &retPath);

		/*!
		Copy the source file to the destination file
		@param[in] strFromFile the source file path
		@param[in] strToFile the destination file path
		@param[in] failIfExist if this is true and the destination file exist then fail copy
		@return true if the copied successfully, otherwise false
		*/
		static bool CopyFile(const TCHAR * strFromFile, const TCHAR *strToFile,bool failIfExist=false);

		/*!
		Return actual file length of the given file in byte size
		@param[in] file the file to find the length
		@return the actual length of the given file in byte size
		*/
		static size_t GetActualFileLength(CFile &file);

		
		/*!
		Return the full path with file name for the current executable file
		@return the full path of the current module
		*/
		static EpTString GetModuleFileName();

		/*!
		Return only the directory ending with "\" which contains the current executable file
		@return the directory ending with "\", which contains the current module
		*/
		static EpTString GetModuleFileDirectory();

		/*!
		Return only the directory of given file path with file name
		@param[in] filePath the full path of the file with file name
		@return the directory ending with "\", which contains the given file
		*/
		static EpTString GetPathOnly(const TCHAR * filePath);

		/*!
		Return only the extension of given file path
		@param[in] filePath the full path of the file with file name
		@return the extension of given file path
		*/
		static EpTString GetFileExtension(const TCHAR *filePath);

	private:
		/*!
		Recursively delete given folder path from file system
		@param[in] strPath the file path to delete
		*/
		static void removeDir(const TCHAR * strPath);
	};
}

#endif //__EP_FOLDER_HELPER_H__