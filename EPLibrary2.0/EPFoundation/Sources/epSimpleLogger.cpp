/*! 
SimpleLogger for the EpLibrary
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
#include "epSimpleLogger.h"

using namespace epl;

SimpleLogManager::SimpleLogNode::SimpleLogNode() :OutputNode()
{
	m_lineNum=0;
	memset(m_dateStr,0,sizeof(TCHAR)*9);
	memset(m_timeStr,0,sizeof(TCHAR)*9);
	m_userStr=NULL;
}

SimpleLogManager::SimpleLogNode::~SimpleLogNode()
{
	if(m_userStr)
		EP_DELETE[] m_userStr;
}

void SimpleLogManager::SimpleLogNode::Print() const
{
	if(m_userStr)
		System::Printf(_T("%s::%s(%d) %s %s - %s\n"),m_fileName,m_funcName,m_lineNum,m_dateStr,m_timeStr,m_userStr);
	else
		System::Printf(_T("%s::%s(%d) %s %s\n"),m_fileName,m_funcName,m_lineNum,m_dateStr,m_timeStr);
}

void SimpleLogManager::SimpleLogNode::Write(EpFile* const file)
{
	if(file)
	{
		if(m_userStr)
		{
			System::FPrintf(file,_T("%s::%s(%d) %s %s - %s\n"),m_fileName,m_funcName,m_lineNum,m_dateStr,m_timeStr,m_userStr);
		}
		else
		{
			System::FPrintf(file,_T("%s::%s(%d) %s %s\n"),m_fileName,m_funcName,m_lineNum,m_dateStr,m_timeStr);
		}
	}
	else
		EP_WASSERT(0,_T("The File Pointer is NULL!"));
}


SimpleLogManager::SimpleLogManager(LockPolicy lockPolicyType):BaseOutputter(lockPolicyType)
{
}
SimpleLogManager::SimpleLogManager(const SimpleLogManager& b):BaseOutputter(b)
{}
SimpleLogManager::~SimpleLogManager()
{	
	EpFile *file=NULL;
	System::FOpen(file,_T("simplelog.dat"),_T("wt"));
	if(file)
	{
		WriteToFile(file);
		System::FClose(file);
	}
	else
		EP_WASSERT(0,_T("Cannot Open the file(%s)!"),_T("simplelog.dat"));
}


void SimpleLogManager::AddSimpleLog(const TCHAR *fileName, const TCHAR *funcName,const int lineNum,const TCHAR *format,...)
{
	LockObj lock(m_nodeListLock);


	SimpleLogNode *log=EP_NEW SimpleLogNode();
	log->m_fileName=fileName;
	log->m_funcName=funcName;
	log->m_lineNum=lineNum;

	va_list args; 
	int len;
	va_start(args, format); 
	len=System::StrLen_V(format,args);
	if(len==1)
	{
		log->m_userStr=NULL;
	}
	else
	{
		log->m_userStr=EP_NEW TCHAR[len];
		System::SPrintf_V(log->m_userStr,len,format,args);
	}
	va_end(args); 

	System::StrDate(log->m_dateStr,9);
	System::StrTime(log->m_timeStr,9);
	m_list.push_back(log);
}


