/*! 
SimpleLogger for the EpLibrary

The MIT License (MIT)

Copyright (c) 2012-2013 Woong Gyu La <juhgiyo@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "epSimpleLogger.h"
#include "epException.h"
#include "epFolderHelper.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;

SimpleLogManager::SimpleLogNode::SimpleLogNode() :OutputNode()
{
	m_lineNum=0;
	System::Memset(m_dateStr,0,sizeof(TCHAR)*9);
	System::Memset(m_timeStr,0,sizeof(TCHAR)*9);
	m_userStr=_T("");
}
SimpleLogManager::SimpleLogNode::SimpleLogNode(const SimpleLogNode& b):OutputNode(b)
{
	m_fileName=b.m_fileName;
	m_funcName=b.m_funcName;
	m_lineNum=b.m_lineNum;
	m_userStr=b.m_userStr;
	System::Memcpy(m_dateStr,b.m_dateStr,sizeof(TCHAR)*9);
	System::Memcpy(m_timeStr,b.m_timeStr,sizeof(TCHAR)*9);
}
SimpleLogManager::SimpleLogNode::~SimpleLogNode()
{
}

SimpleLogManager::SimpleLogNode & SimpleLogManager::SimpleLogNode::operator=(const SimpleLogManager::SimpleLogNode&b)
{
	if(this!=&b)
	{
		BaseOutputter::OutputNode::operator =(b);
		m_fileName=b.m_fileName;
		m_funcName=b.m_funcName;
		m_lineNum=b.m_lineNum;
		m_userStr=b.m_userStr;
		System::Memcpy(m_dateStr,b.m_dateStr,sizeof(TCHAR)*9);
		System::Memcpy(m_timeStr,b.m_timeStr,sizeof(TCHAR)*9);
	}
	return *this;
}

void SimpleLogManager::SimpleLogNode::Print() const
{
	if(m_userStr.length())
		System::TPrintf(_T("%s::%s(%d) %s %s - %s\n"),m_fileName.c_str(),m_funcName.c_str(),m_lineNum,m_dateStr,m_timeStr,m_userStr.c_str());
	else
		System::TPrintf(_T("%s::%s(%d) %s %s\n"),m_fileName.c_str(),m_funcName.c_str(),m_lineNum,m_dateStr,m_timeStr);
}

void SimpleLogManager::SimpleLogNode::Write(EpFile* const file)
{
	EP_ASSERT_EXPR(file,_T("The File Pointer is NULL!"));
	if(m_userStr.length())
	{
		System::FTPrintf(file,_T("%s::%s(%d) %s %s - %s\n"),m_fileName.c_str(),m_funcName.c_str(),m_lineNum,m_dateStr,m_timeStr,m_userStr.c_str());
	}
	else
	{
		System::FTPrintf(file,_T("%s::%s(%d) %s %s\n"),m_fileName.c_str(),m_funcName.c_str(),m_lineNum,m_dateStr,m_timeStr);
	}
}


SimpleLogManager::SimpleLogManager(LockPolicy lockPolicyType):BaseOutputter(lockPolicyType)
{
	m_fileName=FolderHelper::GetModuleFileDirectory();
	m_fileName.append(_T("simplelog.dat"));
}
SimpleLogManager::SimpleLogManager(const SimpleLogManager& b):BaseOutputter(b)
{
	LockObj lock(b.m_nodeListLock);
	m_fileName=b.m_fileName;
}
SimpleLogManager::~SimpleLogManager()
{	
	FlushToFile();
}
SimpleLogManager & SimpleLogManager::operator=(const SimpleLogManager&b)
{
	if(this!=&b)
	{
		FlushToFile();

		BaseOutputter::operator =(b);
		LockObj lock(b.m_nodeListLock);
		m_fileName=b.m_fileName;
	}
	return *this;
}


void SimpleLogManager::AddSimpleLog(const TCHAR *fileName, const TCHAR *funcName,const int lineNum,const TCHAR *format,...)
{
#if  defined(_DEBUG) && defined(EP_ENABLE_LOG)
	LockObj lock(m_nodeListLock);


	SimpleLogNode *log=EP_NEW SimpleLogNode();
	log->m_fileName=fileName;
	log->m_funcName=funcName;
	log->m_lineNum=lineNum;

	va_list args; 
	int len;
	va_start(args, format); 
	len=System::TcsLen_V(format,args);
	if(len==0)
	{
		log->m_userStr=_T("");
	}
	else
	{
		System::STPrintf_V(log->m_userStr,format,args);
	}
	va_end(args); 

	System::TcsDate(log->m_dateStr,9);
	System::TcsTime(log->m_timeStr,9);
	m_list.push_back(log);
#endif// defined(_DEBUG) && defined(EP_ENABLE_LOG)
}

void SimpleLogManager::FlushToFile()
{
#if  defined(_DEBUG) && defined(EP_ENABLE_LOG)
	BaseOutputter::FlushToFile();
#endif// defined(_DEBUG) && defined(EP_ENABLE_LOG)
}

