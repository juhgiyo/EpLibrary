/*! 
Profiler for the EpLibrary
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
#include "epProfiler.h"
#include "epBinarySearch.h"
#include "epException.h"
#include "epFolderHelper.h"
#include "epDateTimeHelper.h"

using namespace epl;



Profiler::Profiler()
{
	m_uniqueName=_T("");
	m_startTime=0;
	m_endTime=0;
	m_lastProfileTime=0;

}

Profiler::Profiler(const Profiler & b)
{
	m_startTime=b.m_startTime;
	m_endTime=b.m_endTime;
	m_uniqueName=b.m_uniqueName;
	m_lastProfileTime=b.m_lastProfileTime;
}
Profiler::Profiler(const TCHAR *uniqueName)
{
	m_uniqueName=uniqueName;
	m_startTime=0;
	m_endTime=0;
	m_lastProfileTime=0;
}

Profiler &Profiler::operator=(const Profiler & b)
{
	if(this!=&b)
	{
		m_startTime=b.m_startTime;
		m_endTime=b.m_endTime;
		m_uniqueName=b.m_uniqueName;
		m_lastProfileTime=b.m_lastProfileTime;
	}
	return *this;
}

Profiler::~Profiler()
{
}
void Profiler::Start()
{
	m_endTime=0;
	//m_startTime=System::GetTime();
	m_startTime=DateTimeHelper::InMilliSec(DateTimeHelper::GetThreadTotalRunningTime(System::GetCurrentThread()));
}

unsigned __int64 Profiler::Stop()
{
	m_endTime=0;
	//m_endTime=System::GetTime();
	m_endTime=DateTimeHelper::InMilliSec(DateTimeHelper::GetThreadTotalRunningTime(System::GetCurrentThread()));
	EP_ASSERT_EXPR(m_endTime-m_startTime>=0,_T("Stop Function called without starting!"));
	m_lastProfileTime=m_endTime-m_startTime;
	return m_lastProfileTime;
}

unsigned __int64 Profiler::GetLastProfileTime()
{
	EP_ASSERT_EXPR(m_lastProfileTime>=0,_T("There is no last profiled time!"));
	return m_lastProfileTime;
}


void Profiler::addLastProfileTimeToManager()
{
	EP_ASSERT_EXPR(m_lastProfileTime>=0,_T("There is no last profiled time!"));
	epl::SingletonHolder<epl::ProfileManager>::Instance().addProfile(m_uniqueName.c_str(),m_lastProfileTime);
}


EpTString Profiler::GetNewUniqueName(TCHAR *fileName, TCHAR *functionName,unsigned int lineNum)
{
	EpTString uniqueProfilerName;
	System::STPrintf(uniqueProfilerName,_T("%s::%s(%d)"),fileName,functionName,lineNum);
	return uniqueProfilerName;
}



ProfileManager::ProfileNode::ProfileNode(const TCHAR *uniqueName):OutputNode() 
{
	m_uniqueName=uniqueName;
	m_cnt=0;
	m_totalTime=0;
}
ProfileManager::ProfileNode::ProfileNode(const ProfileNode& b):OutputNode(b)
{
	m_uniqueName=b.m_uniqueName;
	m_cnt=b.m_cnt;
	m_totalTime=b.m_totalTime;
}
ProfileManager::ProfileNode::~ProfileNode()
{
}

ProfileManager::ProfileNode & ProfileManager::ProfileNode::operator=(const ProfileManager::ProfileNode&b)
{
	if(this!=&b)
	{
		BaseOutputter::OutputNode::operator =(b);
		m_uniqueName=b.m_uniqueName;
		m_cnt=b.m_cnt;
		m_totalTime=b.m_totalTime;
	}
	return *this;
}

void ProfileManager::ProfileNode::Print() const
{
	System::TPrintf(_T("%s Average : %I64d ms Total : %I64d ms Call : %d\n"),m_uniqueName.c_str(),m_totalTime/m_cnt,m_totalTime,m_cnt);
}

void ProfileManager::ProfileNode::Write(EpFile* const file)
{
	EP_ASSERT_EXPR(file,_T("The File Pointer is NULL!"));
	System::FTPrintf(file,_T("%s Average : %I64d ms Total : %I64d ms Call : %d\n"),m_uniqueName.c_str(),m_totalTime/m_cnt,m_totalTime,m_cnt);
}

CompResultType ProfileManager::ProfileNode::Compare(const void * a, const void * b)
{

	ProfileNode *_a=*reinterpret_cast<ProfileNode**>(const_cast<void*>(a));
	ProfileNode *_b=*reinterpret_cast<ProfileNode**>(const_cast<void*>(b));

	int result=_a->m_uniqueName.compare(_b->m_uniqueName);
	if (result>0) return COMP_RESULT_GREATERTHAN;
	else if (result==0 ) return COMP_RESULT_EQUAL;
	else return COMP_RESULT_LESSTHAN;		
}

void ProfileManager::FlushToFile()
{
#if  defined(_DEBUG) && defined(EP_ENABLE_PROFILE)
	BaseOutputter::FlushToFile();
#endif// defined(_DEBUG) && defined(EP_ENABLE_PROFILE)
}

ProfileManager::ProfileManager(LockPolicy lockPolicyType):BaseOutputter(lockPolicyType)
{
	m_fileName=FolderHelper::GetModuleFileDirectory();
	m_fileName.append(_T("profile.dat"));
}
ProfileManager::ProfileManager(const ProfileManager& b):BaseOutputter(b)
{
	LockObj lock(b.m_nodeListLock);
	m_fileName=b.m_fileName;
}

ProfileManager::~ProfileManager()
{
	FlushToFile();
}
ProfileManager & ProfileManager::operator=(const ProfileManager&b)
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
bool ProfileManager::isProfileExist(const TCHAR *uniqueName,ProfileNode *&retIter, int &retIdx )
{
	if(!m_list.size())
		return false;
	ProfileNode profile=ProfileNode(uniqueName);
	profile.m_cnt=1;
	profile.m_totalTime=0;

	ProfileNode *profPointer=&profile;


	ProfileNode**retData=NULL;

	if(m_list.size())
		retData=BinarySearch(profPointer,reinterpret_cast<ProfileNode**>( &(m_list.at(0)) ),m_list.size(),ProfileNode::Compare,retIdx);
	if(retData)
		retIter=*retData;
	else
		retIter=NULL;
	if(retIter)
	{
		return true;
	}
	else
		return false;
}



void ProfileManager::addProfile(const TCHAR *uniqueName, const unsigned __int64 &time)
{
	LockObj lock(m_nodeListLock);
	ProfileNode *existStruct=NULL;
	int retIdx=-1;
	if(isProfileExist(uniqueName,existStruct,retIdx) && existStruct && retIdx!=-1)
	{
		existStruct->m_totalTime+=time;
		existStruct->m_cnt++;
	}
	else
	{
		ProfileNode *profile=EP_NEW ProfileNode(uniqueName);
		profile->m_totalTime=time;
		profile->m_cnt=1;
		if(m_list.size() && retIdx!=-1)
		{
			std::vector<OutputNode*>::iterator iter=m_list.begin()+retIdx;
			m_list.insert(iter,profile);
		}
		else
		{
			m_list.push_back(profile);
		}
	}
}


ProfileObj::ProfileObj(const TCHAR *uniqueName)
{
	m_profiler=Profiler(uniqueName);
	m_profiler.Start();
}


ProfileObj::~ProfileObj()
{
	m_profiler.Stop();
#if defined(_DEBUG) && defined(EP_ENABLE_PROFILE)
	m_profiler.addLastProfileTimeToManager();
#endif// defined(_DEBUG) && defined(EP_ENABLE_PROFILE)
}

