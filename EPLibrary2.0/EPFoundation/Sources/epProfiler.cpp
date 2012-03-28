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

using namespace epl;

ProfileManager::ProfileNode::ProfileNode() :OutputNode()
{
	m_fileName=NULL;
	m_funcName=NULL;
	m_cnt=0;
	m_totalTime=0;
}

ProfileManager::ProfileNode::~ProfileNode()
{
}
void ProfileManager::ProfileNode::Print()
{
	System::Printf(_T("%s::%s %d ms %d ms %d\n"),m_fileName,m_funcName,m_totalTime/m_cnt,m_totalTime,m_cnt);
}

void ProfileManager::ProfileNode::Write(EpFile* const file)
{
	if(file)
	{
		System::FPrintf(file,_T("%s::%s %d ms %d ms %d\n"),m_fileName,m_funcName,m_totalTime/m_cnt,m_totalTime,m_cnt);
	}
	else
		EP_WASSERT(0,_T("The File Pointer is NULL!"));
}

CompResultType ProfileManager::ProfileNode::Compare(const void * a, const void * b)
{

	ProfileNode *_a=*(ProfileNode**)a;
	ProfileNode *_b=*(ProfileNode**)b;

	if ( _a->m_fileName >  _b->m_fileName ) return COMP_RESULT_GREATERTHAN;
	else if ( _a->m_fileName == _b->m_fileName )
	{
		if ( _a->m_funcName >  _b->m_funcName ) return COMP_RESULT_GREATERTHAN;
		else if ( _a->m_funcName ==  _b->m_funcName )return COMP_RESULT_EQUAL;
		else return COMP_RESULT_LESSTHAN;
	}
	else return COMP_RESULT_LESSTHAN;		
}


ProfileManager::Profiler::Profiler(const TCHAR *fileName,const TCHAR *funcName)
{
	m_startTime=System::GetTime();
	m_funcName=funcName;
	m_fileName=fileName;
}
ProfileManager::Profiler::~Profiler()
{
	EpTime endTime=0;
	endTime=System::GetTime();
	// Logging Comes here
	PROFILE_INSTANCE.AddProfile(m_fileName,m_funcName,endTime-m_startTime);
}



ProfileManager::ProfileManager():BaseOutputter()
{

}

ProfileManager::~ProfileManager()
{
	EpFile *file=NULL;
	System::FOpen(file,_T("profile.dat"),_T("wt"));
	if(file)
	{
		WriteToFile(file);
		System::FClose(file);
	}
	else
		EP_WASSERT(0,_T("Cannot open the file(%s)!"),_T("profile.dat"));
}
bool ProfileManager::isProfileExist(const TCHAR *fileName,const TCHAR* funcName,ProfileNode *&retIter, int &retIdx )
{
	if(!m_list.size())
		return false;
	ProfileNode profile;
	profile.m_fileName=fileName;
	profile.m_funcName=funcName;
	profile.m_cnt=1;
	profile.m_totalTime=0;

	ProfileNode *profPointer=&profile;


	ProfileNode**retData=NULL;

	if(m_list.size())
		retData=BinarySearch(profPointer,(ProfileNode**)&(m_list.at(0)),m_list.size(),ProfileNode::Compare,retIdx);
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



void ProfileManager::AddProfile(const TCHAR *fileName,const TCHAR* funcName, const EpTime &time)
{
	LockObj lock(m_nodeListLock);
	ProfileNode *existStruct=NULL;
	int retIdx=-1;
	if(isProfileExist(fileName,funcName,existStruct,retIdx) && existStruct && retIdx!=-1)
	{
		existStruct->m_totalTime+=time;
		existStruct->m_cnt++;
	}
	else
	{
		ProfileNode *profile=EP_NEW ProfileNode();
		profile->m_fileName=fileName;
		profile->m_funcName=funcName;
		profile->m_totalTime=time;
		profile->m_cnt=1;
		std::vector<OutputNode*>::iterator iter=m_list.begin()+retIdx;
		m_list.insert(iter,profile);
	}
}

