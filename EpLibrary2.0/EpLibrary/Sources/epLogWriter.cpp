/*! 
LogWriter for the EpWinServiceCLI
Copyright (C) 2013  Woong Gyu La <juhgiyo@gmail.com>

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
#include "epLogWriter.h"
#include "epFolderHelper.h"
using namespace epl;
#if  defined(_UNICODE) || defined(UNICODE)
LogWriter::LogWriter(LockPolicy lockPolicyType):BaseTextFile(FILE_ENCODING_TYPE_UTF16LE,lockPolicyType)
#else // defined(_UNICODE) || defined(UNICODE)
LogWriter::LogWriter(LockPolicy lockPolicyType):BaseTextFile(FILE_ENCODING_TYPE_UTF8,lockPolicyType)
#endif//  defined(_UNICODE) || defined(UNICODE)
{
	m_fileName=FolderHelper::GetModuleFileName().c_str();
	m_fileName.Replace(_T(".exe"),_T(".log"));
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_logLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_logLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_logLock=EP_NEW NoLock();
		break;
	default:
		m_logLock=NULL;
		break;
	}
}

LogWriter::~LogWriter()
{
	if(m_logLock)
		EP_DELETE m_logLock;
}

LogWriter::LogWriter(const LogWriter& b):BaseTextFile(b)
{
	m_fileName=b.m_fileName;
	m_lockPolicy=b.m_lockPolicy;
	switch(m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_logLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_logLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_logLock=EP_NEW NoLock();
		break;
	default:
		m_logLock=NULL;
		break;
	}
}

void LogWriter::WriteLog(const  TCHAR* pMsg)
{
	// write error or other information into log file
	LockObj lock(m_logLock);
	SYSTEMTIME oT;
	::GetLocalTime(&oT);
	m_logString=_T("");
	m_logString.AppendFormat(_T("%02d/%02d/%04d, %02d:%02d:%02d\n    %s\n"),oT.wMonth,oT.wDay,oT.wYear,oT.wHour,oT.wMinute,oT.wSecond,pMsg);
	AppendToFile(m_fileName);
}

void LogWriter::writeLoop()
{
	writeToFile(m_logString.GetString());
}

void LogWriter::loadFromFile(const EpTString &lines)
{
}