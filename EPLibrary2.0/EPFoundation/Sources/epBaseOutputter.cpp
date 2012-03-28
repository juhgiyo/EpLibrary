/*! 
BaseOutputter for the EpLibrary
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
#include "epBaseOutputter.h"

using namespace epl;

DECLARE_THREAD_SAFE_CLASS(System);

BaseOutputter::OutputNode::OutputNode()
{}
BaseOutputter::OutputNode::~OutputNode()
{}


BaseOutputter::BaseOutputter()
{
#ifdef EP_MULTIPROCESS
	m_nodeListLock=EP_NEW Mutex();
#else //EP_MULTIPROCESS
	m_nodeListLock=EP_NEW CriticalSectionEx();
#endif //EP_MULTIPROCESS
}

BaseOutputter::~BaseOutputter()
{
	Clear();
	EP_DELETE m_nodeListLock;
}


void BaseOutputter::Clear()
{
	LockObj lock(m_nodeListLock);
	std::vector<OutputNode*>::iterator iter;
	for(iter=m_list.begin();iter!=m_list.end();iter++)
	{
		EP_DELETE (*iter);
	}
	m_list.clear();
}

void BaseOutputter::Print()
{
	LockObj lock(m_nodeListLock);
	std::vector<OutputNode*>::iterator iter;
	for(iter=m_list.begin();iter!=m_list.end();iter++)
	{
		(*iter)->Print();
	}
}

void BaseOutputter::WriteToFile(EpFile* const file)
{
	if(file)
	{
		LockObj lock(m_nodeListLock);
		std::vector<OutputNode*>::iterator iter;
		for(iter=m_list.begin();iter!=m_list.end();iter++)
		{
			(*iter)->Write(file);
		}
	}
}