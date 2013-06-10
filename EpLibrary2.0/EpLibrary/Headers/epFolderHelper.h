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

	/*! 
	@class OpenFileDialog epFolderHelper.h
	@brief This is a class for Open File Dialog

	Implements the Open File Dialog.
	*/
	class EP_LIBRARY OpenFileDialog{
	public:
		/*!
		Default Constructor

		Initializes the Open File Dialog
		@param[in] title the title of the dialog
		@param[in] defaultExt The default extension
		@param[in] defaultDir The default directory
		@param[in] filter The file extension filter
		@param[in] pParentWnd The parent window
		*/
		OpenFileDialog(TCHAR *title=NULL, TCHAR * defaultExt=NULL,TCHAR* defaultDir=NULL, TCHAR *filter=NULL,CWnd* pParentWnd = NULL );

		/*!
		Default Destructor

		Destroy the Open File Dialog
		*/
		virtual ~OpenFileDialog();

		/*! 
		Return full path and filename
		@return full path and filename
		*/
		CString GetPathName() const; 
		
		/*! 
		Return only filename
		@return filename
		*/
		CString GetFileName() const; 

		/*!
		Return only ext
		@return ext
		*/
		CString GetFileExt() const;  

		/*!
		Return file title
		@return file title
		*/
		CString GetFileTitle() const;

		/*! 
		Do modal and return Modal result
		@return Modal result
		*/
		virtual int DoModal();
	private:
		/// file dialog
		CFileDialog *m_fileDialog;
	};


	/*!
	@def DEFAULT_MULTIFILE_BUFFER_SIZE
	@brief default buffer size in byte for the Open Multi-File Dialog

	Macro for default buffer size in byte of the Open Multi-File Dialog.
	*/
	#define DEFAULT_MULTIFILE_BUFFER_SIZE 100000

	/*! 
	@class OpenMultiFileDialog epFolderHelper.h
	@brief This is a class for Open Multi-File Dialog

	Implements the Open Multi-File Dialog.
	*/
	class EP_LIBRARY OpenMultiFileDialog{
	public:
		/*!
		Default Constructor

		Initializes the Open Multi-File Dialog
		@param[in] title the title of the dialog
		@param[in] defaultExt The default extension
		@param[in] defaultDir The default directory
		@param[in] filter The file extension filter
		@param[in] pParentWnd The parent window
		@param[in] bufferSize the buffer size for multi-file list string
		*/
		OpenMultiFileDialog(TCHAR *title=NULL, TCHAR * defaultExt=NULL,TCHAR* defaultDir=NULL, TCHAR *filter=NULL,CWnd* pParentWnd = NULL , int bufferSize=DEFAULT_MULTIFILE_BUFFER_SIZE);

		/*!
		Default Destructor

		Destroy the Open Multi-File Dialog
		*/
		virtual ~OpenMultiFileDialog();

		/*! 
		Return next path name
		@return the next path name
		@remark when this function is first called, it returns the first path name.
		*/
		CString GetNextPathName(); 

		/*! 
		Rewind to the first path name
		*/
		void Rewind(); 
		
		/*! 
		Do modal and return Modal result
		@return Modal result
		*/
		virtual int DoModal();
	private:
		/// file dialog
		CFileDialog *m_fileDialog;
		/// path name list buffer
		TCHAR *m_buffer;
		/// position
		POSITION m_ps;
	};

	/*! 
	@class OpenFolderDialog epFolderHelper.h
	@brief This is a class for Open Folder Dialog

	Implements the Open Folder Dialog.
	*/
	class EP_LIBRARY OpenFolderDialog{
	public:
		/*!
		Default Constructor

		Initializes the Open Folder Dialog
		@param[in] hParent the parent window's handle
		@param[in] title the title of the dialog
		*/
		OpenFolderDialog(HWND hParent,TCHAR *title=NULL);

		/*!
		Default Destructor

		Destroy the Open Folder Dialog
		*/
		virtual ~OpenFolderDialog();

		/*! 
		Return full directory path
		@return full directory path
		*/
		CString GetPathName() const; 

		/*! 
		Do modal and return Modal result
		@return Modal result
		*/
		virtual int DoModal();
	private :
		/// folder path
		EpTString m_folderPath;
		/// browse info
		BROWSEINFO m_bi;
		/// buffer from folder path variable
		LPTSTR m_pBuffer;
	};

	/*! 
	@class SaveFileDialog epFolderHelper.h
	@brief This is a class for Save File Dialog

	Implements the Save File Dialog.
	*/
	class EP_LIBRARY SaveFileDialog{
	public:
		/*!
		Default Constructor

		Initializes the Save File Dialog
		@param[in] defaultExt The default extension
		@param[in] defaultDir The default directory
		@param[in] filter The file extension filter
		@param[in] pParentWnd The parent window
		*/
		SaveFileDialog(TCHAR * defaultExt=NULL,TCHAR* defaultDir=NULL, TCHAR *filter=NULL,CWnd* pParentWnd = NULL );
		
		/*!
		Default Destructor

		Destroy the Save File Dialog
		*/
		virtual ~SaveFileDialog();
		
		/*!
		Return full path and filename
		@return full path and filename
		*/
		CString GetPathName() const; 
		
		/*!
		Return only filename
		@return filename
		*/
		CString GetFileName() const; 

		/*! 
		Return only ext
		@return ext
		*/
		CString GetFileExt() const;  

		/*! 
		Return file title
		@return file title
		*/
		CString GetFileTitle() const;

		/*! 
		Do modal and return Modal result
		@return Modal result
		*/
		virtual int DoModal();
	private:
		/// file dialog
		CFileDialog *m_fileDialog;
	};
}

#endif //__EP_FOLDER_HELPER_H__