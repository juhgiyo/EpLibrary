/*! 
FolderHelper for the EpLibrary
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
*/
#include "epFolderHelper.h"
#include <Shlwapi.h>
#include <io.h>

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;



bool FolderHelper::ChooseFolder(HWND hParent, const TCHAR * title, EpTString &retFolderPath)
{
	bool success = false;
	BROWSEINFO bi;
	::ZeroMemory(&bi, sizeof(bi));
	retFolderPath.insert(retFolderPath.begin(),MAX_PATH,_T('\0'));
	LPTSTR pBuffer = const_cast<TCHAR*>(retFolderPath.data());

	bi.hwndOwner = hParent;
	bi.pszDisplayName = pBuffer;
	bi.lpszTitle = title;
	bi.pidlRoot = 0;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | 
		BIF_NEWDIALOGSTYLE;

	LPITEMIDLIST pItem = ::SHBrowseForFolder(&bi);
	if (pItem != NULL)
	{
		::SHGetPathFromIDList(pItem, pBuffer);
		success = true;

		CComPtr<IMalloc> pMalloc;
		if (SUCCEEDED(::SHGetMalloc(&pMalloc)))
			pMalloc->Free(pItem);
	}

	//folderPath.ReleaseBuffer();
	int length = System::TcsLen(retFolderPath.c_str());
	if(length>0 && retFolderPath.at(length-1)!=_T('\\'))
	{
		retFolderPath.at(length)=_T('\\');
	}
	return success;
}

void FolderHelper::removeDir( const TCHAR * strPath)  

{  

	CFileFind searchFile;  
	EpTString strFile;  
	long bRet;  
	EpTString inputPath=strPath;
	if(_taccess(inputPath.c_str(),00) != 0)
		return;  

	strFile.append(inputPath);
	strFile.append(_T("*.*"));  

	bRet = searchFile.FindFile(strFile.c_str());  

	while(bRet)  
	{  
		bRet = searchFile.FindNextFile();  
		if(searchFile.IsDots())  
			continue;  

		if(searchFile.IsDirectory())  
		{
			EpTString deleteFolder=searchFile.GetFilePath().GetString();
			deleteFolder.append(_T("\\"));
			removeDir(deleteFolder.c_str());  

		}
		else  
			DeleteFile(searchFile.GetFilePath());  
	}  
	RemoveDirectory(searchFile.GetFilePath());
}
bool FolderHelper::CreateFolder(const TCHAR * strPath)
{
#if defined(_UNICODE) || defined(UNICODE)
	if(SHCreateDirectory(NULL,strPath)==ERROR_SUCCESS)
#else //defined(_UNICODE) || defined(UNICODE)
	if(SHCreateDirectory(NULL,System::MultiByteToWideChar(strPath).c_str())==ERROR_SUCCESS)
#endif //defined(_UNICODE) || defined(UNICODE)
		return true;
	return false;
}

void FolderHelper::DeleteFolder(const TCHAR * strPath)
{
	if(!System::TcsLen(strPath))
		return;
	EpTString path=strPath;
	path=Locale::Trim(path);
	size_t length=path.length();
	if(path.at(length-1)!=_T('\\'))
	{
		path.append(_T("\\"));
	}
	removeDir(path.c_str());

}

bool FolderHelper::IsPathExist(const TCHAR * path)
{
	return  PathFileExists(path);
}
bool FolderHelper::GetSpecialFolderPath(int csidl,bool isCreateIfNotExist,EpTString &retPath)
{
	TCHAR pathString[MAX_PATH]={0};
	if(SHGetSpecialFolderPath(NULL,pathString,csidl,isCreateIfNotExist))
	{
		retPath=pathString;
		return true;
	}
	return false;
}


bool FolderHelper::CopyFile(const TCHAR *strFromFile, const TCHAR * strToFile,bool failIfExist)
{
	if(::CopyFile(strFromFile,strToFile,failIfExist))
		return true;
	return false;
}

size_t FolderHelper::GetActualFileLength(CFile &file)
{
	//size_t length= (unsigned int)file.SeekToEnd()+1;
	size_t length= file.SeekToEnd();
	file.SeekToBegin();
	return length;
}

EpTString FolderHelper::GetPathOnly(const TCHAR * filePath)
{
	EpTString retString=filePath;
	//size_t strLength=System::TcsLen(filePath);
	size_t strLength=retString.length();

	for(ssize_t stringTrav=static_cast<ssize_t>(strLength)-1;stringTrav>=0;stringTrav--)
	{
		if(retString.at(stringTrav)!=_T('\\'))
			retString.erase(stringTrav,1);
		else
			return retString;
	}
	return retString;
}

EpTString FolderHelper::GetFileExtension(const TCHAR *filePath)
{
	EpTString tmpString=filePath;
	//size_t strLength=System::TcsLen(filePath);
	size_t strLength=tmpString.length();
	EpTString retString=_T("");
	for(ssize_t stringTrav=static_cast<ssize_t>(strLength-1);stringTrav>=0;stringTrav--)
	{
		if(tmpString.at(stringTrav)==_T('.'))
		{
			tmpString.erase(tmpString.begin(),tmpString.begin()+(stringTrav+1));
			retString=tmpString;
			return retString;
		}
	}
	return retString;
}

EpTString FolderHelper::GetModuleFileName()
{
	TCHAR pathName[MAX_PATH];
	System::Memset(pathName,0,sizeof(TCHAR)*MAX_PATH);
	::GetModuleFileName(NULL, pathName,MAX_PATH);
	EpTString retString=pathName;
	return retString;
}

EpTString FolderHelper::GetModuleFileDirectory()
{
	EpTString retString=GetModuleFileName();
	return GetPathOnly(retString.c_str());
}