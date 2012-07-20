/*! 
ServerObjectList for the EpLibrary
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
#include "epServerObjectList.h"

using namespace epl;

ServerObjectList::ServerObjectList(LockPolicy lockPolicyType)
{
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_listLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_listLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_listLock=EP_NEW NoLock();
		break;
	default:
		m_listLock=NULL;
		break;
	}
}

ServerObjectList::ServerObjectList(const ServerObjectList& b)
{
	m_objectList=b.m_objectList;
	m_lockPolicy=b.m_lockPolicy;
	switch(m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_listLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_listLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_listLock=EP_NEW NoLock();
		break;
	default:
		m_listLock=NULL;
		break;
	}
}

ServerObjectList::~ServerObjectList()
{
	Clear();
	if(m_listLock)
		EP_DELETE m_listLock;
}

void ServerObjectList::RemoveTerminated()
{
	LockObj lock(m_listLock);
	vector<BaseServerObject*>::iterator iter;
	for(iter=m_objectList.begin();iter!=m_objectList.end();)
	{
		if((*iter)->GetStatus()==Thread::THREAD_STATUS_TERMINATED)
		{
			(*iter)->ReleaseObj();
			iter=m_objectList.erase(iter);
		}
		else
			iter++;

	}
}

void ServerObjectList::Clear()
{
	LockObj lock(m_listLock);
	vector<BaseServerObject*>::iterator iter;
	for(iter=m_objectList.begin();iter!=m_objectList.end();iter++)
	{
		if(*iter)
			(*iter)->ReleaseObj();
	}
	m_objectList.clear();
}

void ServerObjectList::Push(BaseServerObject* obj)
{
	LockObj lock(m_listLock);
	if(obj)
	{
		obj->RetainObj();
		m_objectList.push_back(obj);
	}
	
}

vector<BaseServerObject*> ServerObjectList::GetList() const
{
	LockObj lock(m_listLock);
	return m_objectList;
}