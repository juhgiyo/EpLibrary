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

using namespace epl;



bool FolderHelper::ChooseFolder(HWND hParent, LPCTSTR title, CString &retFolderPath)
{
	bool success = false;
	CString inputTitle=title;

	BROWSEINFO bi;
	::ZeroMemory(&bi, sizeof(bi));
	LPTSTR pBuffer = retFolderPath.GetBuffer(MAX_PATH);

	bi.hwndOwner = hParent;
	bi.pszDisplayName = pBuffer;
	bi.lpszTitle = inputTitle;
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

	retFolderPath.ReleaseBuffer();
	if(retFolderPath.GetAt(retFolderPath.GetLength()-1)!=_T('\\'))
	{
		retFolderPath.Append(_T("\\"));
	}
	return success;
}

void FolderHelper::removeDir(LPCTSTR strPath)  

{  

	CFileFind searchFile;  
	CString strFile;  
	BOOL bRet;  
	CString inputPath=strPath;

	if(_waccess(inputPath.GetString(),00) != 0)  
		return;  

	strFile.Append(inputPath);
	strFile.Append(_T("\\*.*"));  

	bRet = searchFile.FindFile(strFile);  

	while(bRet)  
	{  
		bRet = searchFile.FindNextFile();  
		if(searchFile.IsDots())  
			continue;  

		if(searchFile.IsDirectory())  
			removeDir(searchFile.GetFilePath());  
		else  
			DeleteFile(searchFile.GetFilePath());  
	}  
}
bool FolderHelper::CreateFolder(LPCTSTR strPath)
{
	if(SHCreateDirectory(NULL,strPath)==ERROR_SUCCESS)
		return true;
	return false;
}

void FolderHelper::DeleteFolder(LPCTSTR strPath)
{
	if(strPath==NULL)
		return;
	CString path=strPath;
	CString path2=strPath;
	int length=path.GetLength();
	int count=0;
	for(int trav=length-1;trav>=0;trav--)
	{
		count++;
		if(path.GetAt(trav)==_T(' ')||path.GetAt(trav)==_T('\0'))
			continue;
		if(path.GetAt(trav)==_T('\\'))
		{
			path.Delete(trav,count);
			break;	
		}
		else
		{
			path2.Append(_T("\\"));
			break;
		}
	}
	removeDir(path.GetString());
	RemoveDirectory(path2);
}

bool FolderHelper::IsPathExist(LPCTSTR path)
{
	return  PathFileExists(path);
}
bool FolderHelper::GetSpecialFolderPath(int csidl,bool isCreateIfNotExist,CString &retPath)
{
	TCHAR pathString[MAX_PATH]={0};
	if(SHGetSpecialFolderPath(NULL,pathString,csidl,isCreateIfNotExist))
	{
		retPath=pathString;
		return true;
	}
	return false;
}


bool FolderHelper::CopyTheFile(LPCTSTR strFromFile, LPCTSTR strToFile,bool failIfExist)
{
	if(CopyFile(strFromFile,strToFile,failIfExist))
		return true;
	return false;
}

unsigned int FolderHelper::GetActualFileLength(CFile &file)
{
	//unsigned int length= (unsigned int)file.SeekToEnd()+1;
	unsigned int length= (unsigned int)file.SeekToEnd();
	file.SeekToBegin();
	return length;
}

CString FolderHelper::GetPathOnly(CString filePath)
{
	for(int stringTrav=filePath.GetLength()-1;stringTrav>=0;stringTrav--)
	{
		if(filePath.GetAt(stringTrav)!=_T('\\'))
			filePath.Delete(stringTrav,1);
		else
			return filePath;
	}
	return filePath;
}

CString FolderHelper::GetModuleFileName()
{
	TCHAR pathName[MAX_PATH];
	memset(pathName,0,sizeof(TCHAR)*MAX_PATH);
	::GetModuleFileName(NULL, pathName,MAX_PATH);
	CString retString=pathName;
	return retString;
}

CString FolderHelper::GetModuleFileDirectory()
{
	CString retString=GetModuleFileName();
	retString=GetPathOnly(retString);
	return retString;
}