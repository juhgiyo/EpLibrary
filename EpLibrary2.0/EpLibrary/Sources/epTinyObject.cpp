/*! 
TinyObject for the EpLibrary

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


------------------------------------------------------------------------------

Part of this software is

The Loki Library
Copyright (c) 2001 by Andrei Alexandrescu
This code accompanies the book:
Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design 
Patterns Applied". Copyright (c) 2001. Addison-Wesley.
Permission to use, copy, modify, distribute and sell this software for any 
purpose is hereby granted without fee, provided that the above copyright 
notice appear in all copies and that both that copyright notice and this 
permission notice appear in supporting documentation.
The author or Addison-Wesley Longman make no representations about the 
suitability of this software for any purpose. It is provided "as is" 
*/
#include "epTinyObject.h"
#include "epException.h"


namespace epl
{
	void StaticAllocator::Fragment::Init(size_t blockSize, unsigned char blocks)
	{
		EP_ASSERT(blockSize > 0);
		EP_ASSERT(blocks > 0);
		EP_ASSERT((blockSize * blocks) / blockSize == blocks);

		m_Data = EP_NEW unsigned char[blockSize * static_cast<size_t>(blocks)];
		Reset(blockSize, blocks);

	}

	void StaticAllocator::Fragment::Reset(size_t blockSize, unsigned char blocks)
	{
		EP_ASSERT(blockSize > 0);
		EP_ASSERT(blocks > 0);
		EP_ASSERT((blockSize * blocks) / blockSize == blocks);
		firstAvailableBlock=0;
		numBlocksAvailable=blocks;
		unsigned char i=0;
		unsigned char* tmp=m_Data;
		for(;i!=blocks;tmp+=blockSize)
		{
			*tmp=++i;
		}
	}
	void *StaticAllocator::Fragment::Allocate(size_t blockSize)
	{
		if(!numBlocksAvailable)
			return 0;
		EP_ASSERT((firstAvailableBlock * blockSize) / blockSize == firstAvailableBlock);
		unsigned char *result= m_Data+(firstAvailableBlock*blockSize);
		firstAvailableBlock=*result;
		--numBlocksAvailable;
		return result;
	}

	void StaticAllocator::Fragment::Deallocate(void *p, size_t blockSize)
	{
		EP_ASSERT(p>=m_Data);
		unsigned char * releaseData= reinterpret_cast<unsigned char*>(p);
		EP_ASSERT((releaseData-m_Data)%blockSize==0);
		*releaseData=firstAvailableBlock;
		firstAvailableBlock=static_cast<unsigned char>((releaseData-m_Data)/blockSize);
		EP_ASSERT(firstAvailableBlock==(releaseData-m_Data)/blockSize);
		++numBlocksAvailable;
	}

	void StaticAllocator::Fragment::Clear()
	{
		if (m_Data)
			EP_DELETE[] m_Data;
		m_Data=NULL;
		firstAvailableBlock=0;
		numBlocksAvailable=0;

	}


	StaticAllocator::StaticAllocator(size_t blockSize)
		: m_blockSize(blockSize)
		, m_allocFragment(0)
		, m_deallocFragment(0)
	{
		EP_ASSERT(m_blockSize > 0);

		size_t numBlocks = DEFAULT_FRAGMENT_SIZE / blockSize;
		if (numBlocks > UCHAR_MAX) numBlocks = UCHAR_MAX;
		else if (numBlocks == 0) numBlocks = 8 * blockSize;

		m_numBlocks = static_cast<unsigned char>(numBlocks);
		EP_ASSERT(m_numBlocks == numBlocks);
	}


	StaticAllocator::StaticAllocator(const StaticAllocator& rhs)
		: m_blockSize(rhs.m_blockSize)
		, m_numBlocks(rhs.m_numBlocks)
		, m_fragments(rhs.m_fragments)
	{
		m_allocFragment = rhs.m_allocFragment
			? &(m_fragments.front()) + (rhs.m_allocFragment - &(rhs.m_fragments.front()))
			: 0;

		m_deallocFragment = rhs.m_deallocFragment
			? &(m_fragments.front()) + (rhs.m_deallocFragment - &(rhs.m_fragments.front()))
			: 0;
	}

	StaticAllocator& StaticAllocator::operator=(const StaticAllocator& rhs)
	{
		if(this!=&rhs)
		{
			StaticAllocator copy(rhs);
			copy.Swap(*this);
		}
		return *this;
	}

	void StaticAllocator::Swap(StaticAllocator& rhs)
	{
		SwapFunc<size_t>(&m_blockSize, &(rhs.m_blockSize));
		SwapFunc<unsigned char>(&m_numBlocks, &(rhs.m_numBlocks));
		SwapFunc<Fragments>(&m_fragments, &(rhs.m_fragments));
		SwapFunc<Fragment*>(&m_allocFragment, &(rhs.m_allocFragment));
		SwapFunc<Fragment*>(&m_deallocFragment, &(rhs.m_deallocFragment));
	}

	void *StaticAllocator::Allocate()
	{
		if(m_allocFragment==0 || m_allocFragment->numBlocksAvailable==0)
		{

			size_t i;
			for(i=0;i<=m_fragments.size();i++)
			{
				Fragment* iter=NULL;
				if(m_fragments.size()>i)
					iter=&m_fragments.at(i);
				if(iter==NULL)
				{
					Fragment newFragment;
					newFragment.Init(m_blockSize,m_numBlocks);
					m_fragments.push_back(newFragment);
					Fragment *tmpFrag=&(m_fragments.at(m_fragments.size()-1));
					*tmpFrag=newFragment;
					m_allocFragment=&(m_fragments.back());
					m_deallocFragment=&(m_fragments.back());
					break;
				}
				if(iter->numBlocksAvailable>0)
				{
					m_allocFragment=iter;
					break;
				}
			}
		}
		EP_ASSERT(m_allocFragment!=0);
		EP_ASSERT(m_allocFragment->numBlocksAvailable>0);
		return m_allocFragment->Allocate(m_blockSize);
	}

	void StaticAllocator::Deallocate(void* p, CacheType type)
	{
		EP_ASSERT(!m_fragments.empty());
		EP_ASSERT(&(m_fragments.front()) <= m_deallocFragment);
		EP_ASSERT(&(m_fragments.back()) >= m_deallocFragment);

		m_deallocFragment  = VicinityFind(p);
		EP_ASSERT(m_deallocFragment);

		Release(p,type);
	}
	void StaticAllocator::Clear()
	{
		EP_ASSERT(m_fragments.empty());
		m_fragments.clear();
	}
	void StaticAllocator::Delete()
	{
		size_t trav;
		Fragment *tmp = NULL; 
		for(trav=0;trav<m_fragments.size();trav++)
		{
			tmp=&(m_fragments.at(trav));
			if( !tmp ) continue; 
			tmp->Clear();
		}
		m_fragments.clear();
		m_blockSize=0;
		m_numBlocks=0;
		m_allocFragment=NULL;
		m_deallocFragment=NULL;
	}
	void StaticAllocator::Compress(CacheType type)
	{
		if(type&CACHE_TYPE_FRAGMENT)
		{
			ssize_t trav;
			for(trav=static_cast<ssize_t>(m_fragments.size())-1;trav>=0;trav--)
			{
				Fragment *tmp=&(m_fragments.at(trav));
				if(tmp->numBlocksAvailable==m_numBlocks)
				{
					tmp->Clear();
					m_fragments.erase(m_fragments.begin()+trav);
				}
			}
			if(m_fragments.empty())
				m_fragments.clear();
			m_allocFragment=&(m_fragments.front());
			m_deallocFragment=&(m_fragments.back());
		}
		else
		{
			if(m_fragments.empty())
				m_fragments.clear();
		}

	}
	StaticAllocator::Fragment* StaticAllocator::VicinityFind(void* p)
	{
		EP_ASSERT(!m_fragments.empty());
		EP_ASSERT(m_deallocFragment);

		size_t chunkLength = m_numBlocks * m_blockSize;

		Fragment* lo = m_deallocFragment;
		Fragment* hi = m_deallocFragment + 1;
		Fragment* loBound = &(m_fragments.front());
		Fragment* hiBound = &(m_fragments.back()) + 1;

		if (hi == hiBound) hi = 0;

		for (;;)
		{
			if (lo)
			{
				if (p >= lo->m_Data && p < lo->m_Data + chunkLength)
				{
					return lo;
				}
				if (lo == loBound) 
					lo = 0;
				else --lo;
			}

			if (hi)
			{
				if (p >= hi->m_Data && p < hi->m_Data + chunkLength)
				{
					return hi;
				}
				if (++hi == hiBound) 
					hi = 0;
			}
		}
	}

	void StaticAllocator::Release(void* p, CacheType type)
	{
		EP_ASSERT(m_deallocFragment->m_Data <= p);
		EP_ASSERT(m_deallocFragment->m_Data + m_numBlocks * m_blockSize > p);



		m_deallocFragment->Deallocate(p, m_blockSize);



		if(m_deallocFragment->numBlocksAvailable == m_numBlocks)
		{
			Fragment * lastFragment =&(m_fragments.back());
			SwapFunc<Fragment>(m_deallocFragment,lastFragment);
			if(type&CACHE_TYPE_FRAGMENT)
			{
				lastFragment->Reset(m_blockSize,m_numBlocks);
			}
			else
			{
				lastFragment->Clear();
				m_fragments.erase(m_fragments.begin()+(m_fragments.size()-1));
			}
			m_deallocFragment=NULL;
			if(m_fragments.size())
				m_deallocFragment = &(m_fragments.front());
			if(lastFragment==m_allocFragment)
			{
				m_allocFragment=NULL;
				if(m_fragments.size())	
					m_allocFragment=&(m_fragments.front());
			}
			return;
		}
		m_allocFragment=m_deallocFragment;

		// 	if (m_deallocFragment->numBlocksAvailable == m_numBlocks)
		// 	{
		// 		Fragment * lastFragment = m_fragments.GetBack();
		// 
		// 		if (lastFragment == m_deallocFragment)
		// 		{
		// 
		// 			if (m_fragments.Count() > 1 && 
		// 				m_deallocFragment->numBlocksAvailable == m_numBlocks)
		// 			{				
		// 				m_fragments.Erase(m_fragments.Count()-1);
		// 				lastFragment->Release();
		// 				m_allocFragment = m_deallocFragment = m_fragments.GetFront();
		// 			}
		// 			return;
		// 		}
		// 		if (lastFragment->numBlocksAvailable == m_numBlocks)
		// 		{			
		// 			m_fragments.Erase(m_fragments.Count()-1);
		// 			lastFragment->Release();
		// 			m_allocFragment = m_deallocFragment;
		// 		}
		// 		else
		// 		{
		// 			DefaultSwapFunc<Fragment>(m_deallocFragment,lastFragment);
		// 			m_allocFragment = m_fragments.GetBack();
		// 		}
		// 	}
	}



	TinyObjAllocator::TinyObjAllocator(size_t fragmentSize,size_t maxObjectSize)
		: m_lastAlloc(0), m_lastDealloc(0), m_fragmentSize(fragmentSize), m_maxObjectSize(maxObjectSize) 
	{   
	}
	TinyObjAllocator::~TinyObjAllocator()
	{
		size_t trav;
		for(trav=0;trav<m_pool.size();trav++)
		{
			StaticAllocator *tmp=&(m_pool.at(trav));
			tmp->Delete();
		}
		m_pool.clear();

	}

	void* TinyObjAllocator::Allocate(size_t numBytes)
	{
		if (numBytes > m_maxObjectSize) 
			return operator new(numBytes);

		if (m_lastAlloc && m_lastAlloc->GetBlockSize() == numBytes)
		{
			return m_lastAlloc->Allocate();
		}
		StaticAllocator *i;

		size_t trav;
		for(trav=0;trav<m_pool.size();trav++)
		{
			if(m_pool.at(trav).GetBlockSize()>=numBytes)
				break;
		}
		size_t idx=trav;
		i=NULL;
		if(m_pool.size()>idx)
			i=&(m_pool.at(idx));

		if (i ==NULL || i->GetBlockSize() != numBytes)
		{
			StaticAllocator tmp=StaticAllocator(numBytes);
			i = &(*(m_pool.insert(m_pool.begin()+idx,tmp)));
			m_lastDealloc = &(m_pool.front());
		}
		m_lastAlloc = i;
		return m_lastAlloc->Allocate();
	}

	void TinyObjAllocator::Deallocate(void* p, size_t numBytes, CacheType type)
	{
		if (numBytes > m_maxObjectSize) 
		{
			operator delete(p);
			return;
		}

		if (m_lastDealloc && m_lastDealloc->GetBlockSize() == numBytes)
		{
			m_lastDealloc->Deallocate(p,type);
			if(type&CACHE_TYPE_COMPRESS)
				Compress(type);
			return;
		}

		StaticAllocator *i = NULL;

		size_t trav;
		for(trav=0;trav<m_pool.size();trav++)
		{
			if(m_pool.at(trav).GetBlockSize()>=numBytes)
				break;
		}
		size_t idx=trav;

		i=&(m_pool.at(idx));
		EP_ASSERT(i->GetBlockSize() == numBytes);
		m_lastDealloc = i;
		m_lastDealloc->Deallocate(p,type);

		if(!(type&CACHE_TYPE_STATIC) )
		{
			m_lastDealloc->Compress(type);
			if(m_lastDealloc->IsEmpty())
			{
				m_lastDealloc->Clear();
				m_pool.erase(m_pool.begin()+idx);
				m_lastDealloc=0;
				m_lastAlloc=0;
				if(!(CACHE_TYPE_POOL&type) && m_pool.empty())
				{
					m_pool.clear();
				}
			}
		}

		if(type&CACHE_TYPE_COMPRESS)
			Compress(type);
	}

	void TinyObjAllocator::Compress(CacheType type)
	{
		ssize_t trav;
		for(trav=static_cast<ssize_t>(m_pool.size())-1;trav>=0;trav--)
		{
			StaticAllocator *tmp=&(m_pool.at(trav));
			tmp->Compress(type);
			if(m_pool.at(trav).IsEmpty())
			{
				m_pool.erase(m_pool.begin()+trav);
			}
		}
		m_lastAlloc=0;
		m_lastDealloc=0;
		if(m_pool.empty())
		{
			m_pool.clear();

		}
	}

}
