/*! 
BaseOutputter for the EpLibrary

The MIT License (MIT)

Copyright (c) 2008-2013 Woong Gyu La <juhgiyo@gmail.com>

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
#include "epBaseOutputter.h"
#include "epException.h"
using namespace epl;

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

DECLARE_THREAD_SAFE_CLASS(System);

BaseOutputter::OutputNode::OutputNode()
{}
BaseOutputter::OutputNode::OutputNode(const OutputNode& b)
{}
BaseOutputter::OutputNode::~OutputNode()
{}


BaseOutputter::BaseOutputter(LockPolicy lockPolicyType)
{
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_nodeListLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_nodeListLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_nodeListLock=EP_NEW NoLock();
		break;
	default:
		m_nodeListLock=NULL;
		break;
	}
}
BaseOutputter::BaseOutputter(const BaseOutputter& b)
{
	m_lockPolicy=b.m_lockPolicy;
	switch(m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_nodeListLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_nodeListLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_nodeListLock=EP_NEW NoLock();
		break;
	default:
		m_nodeListLock=NULL;
		break;
	}
	m_fileName=b.m_fileName;
	LockObj lock(b.m_nodeListLock);
	m_list=b.m_list;
}
BaseOutputter::~BaseOutputter()
{
	Clear();
	if(m_nodeListLock)
		EP_DELETE m_nodeListLock;
	m_nodeListLock=NULL;
}
BaseOutputter & BaseOutputter::operator=(const BaseOutputter&b)
{
	if(this!=&b)
	{
		Clear();
		if(m_nodeListLock)
			EP_DELETE m_nodeListLock;
		m_nodeListLock=NULL;

		m_lockPolicy=b.m_lockPolicy;
		switch(m_lockPolicy)
		{
		case LOCK_POLICY_CRITICALSECTION:
			m_nodeListLock=EP_NEW CriticalSectionEx();
			break;
		case LOCK_POLICY_MUTEX:
			m_nodeListLock=EP_NEW Mutex();
			break;
		case LOCK_POLICY_NONE:
			m_nodeListLock=EP_NEW NoLock();
			break;
		default:
			m_nodeListLock=NULL;
			break;
		}
		m_fileName=b.m_fileName;
		LockObj lock(b.m_nodeListLock);
		m_list=b.m_list;
	}
	return *this;
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

void BaseOutputter::Print() const
{
	LockObj lock(m_nodeListLock);
	std::vector<OutputNode*>::const_iterator iter;
	for(iter=m_list.begin();iter!=m_list.end();iter++)
	{
		(*iter)->Print();
	}
}
void BaseOutputter::FlushToFile()
{
	LockObj lock(m_nodeListLock);
	EpFile *file=NULL;
	System::FTOpen(file,m_fileName.c_str(),_T("at"));
	EP_ASSERT_EXPR(file,_T("Cannot open the file(%s)!"),m_fileName.c_str());
	writeToFile(file);
	System::FClose(file);
}
void BaseOutputter::SetFileName(const TCHAR *fileName)
{
	LockObj lock(m_nodeListLock);
	m_fileName=fileName;
}
void BaseOutputter::writeToFile(EpFile* const file)
{
	if(file)
	{		
		System::FTPrintf(file,_T("Log Starts...\n"));
		std::vector<OutputNode*>::iterator iter;
		for(iter=m_list.begin();iter!=m_list.end();iter++)
		{
			(*iter)->Write(file);
		}
		System::FTPrintf(file,_T("Log Ends...\n"));
	}
}