/*! 
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

TinyObject for the EpLibrary
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
#include "epTinyObject.h"

namespace epl
{
	void StaticAllocator::Fragment::Init(unsigned int blockSize, unsigned char blocks)
	{
		assert(blockSize > 0);
		assert(blocks > 0);
		assert((blockSize * blocks) / blockSize == blocks);

		m_Data = EP_NEW unsigned char[blockSize * (unsigned int)blocks];
		Reset(blockSize, blocks);

	}

	void StaticAllocator::Fragment::Reset(unsigned int blockSize, unsigned char blocks)
	{
		assert(blockSize > 0);
		assert(blocks > 0);
		assert((blockSize * blocks) / blockSize == blocks);
		firstAvailableBlock=0;
		numBlocksAvailable=blocks;
		unsigned char i=0;
		unsigned char* tmp=m_Data;
		for(;i!=blocks;tmp+=blockSize)
		{
			*tmp=++i;
		}
	}
	void *StaticAllocator::Fragment::Allocate(unsigned int blockSize)
	{
		if(!numBlocksAvailable)
			return 0;
		assert((firstAvailableBlock * blockSize) / blockSize == 
			firstAvailableBlock);
		unsigned char *result= m_Data+(firstAvailableBlock*blockSize);
		firstAvailableBlock=*result;
		--numBlocksAvailable;
		return result;
	}

	void StaticAllocator::Fragment::Deallocate(void *p, unsigned int blockSize)
	{
		assert(p>=m_Data);
		unsigned char * releaseData= static_cast<unsigned char*>(p);
		assert((releaseData-m_Data)%blockSize==0);
		*releaseData=firstAvailableBlock;
		firstAvailableBlock=static_cast<unsigned char>((releaseData-m_Data)/blockSize);
		assert(firstAvailableBlock==(releaseData-m_Data)/blockSize);
		++numBlocksAvailable;
	}

	void StaticAllocator::Fragment::Release()
	{
		if (m_Data)
			EP_DELETE[] m_Data;
		m_Data=NULL;
		firstAvailableBlock=0;
		numBlocksAvailable=0;

	}


	StaticAllocator::StaticAllocator(unsigned int blockSize)
		: m_blockSize(blockSize)
		, m_allocFragment(0)
		, m_deallocFragment(0)
	{
		assert(m_blockSize > 0);

		unsigned int numBlocks = DEFAULT_FRAGMENT_SIZE / blockSize;
		if (numBlocks > UCHAR_MAX) numBlocks = UCHAR_MAX;
		else if (numBlocks == 0) numBlocks = 8 * blockSize;

		m_numBlocks = static_cast<unsigned char>(numBlocks);
		assert(m_numBlocks == numBlocks);
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
		SwapFunc<unsigned int>(&m_blockSize, &(rhs.m_blockSize));
		SwapFunc<unsigned char>(&m_numBlocks, &(rhs.m_numBlocks));
		SwapFunc<Fragments>(&m_fragments, &(rhs.m_fragments));
		SwapFunc<Fragment*>(&m_allocFragment, &(rhs.m_allocFragment));
		SwapFunc<Fragment*>(&m_deallocFragment, &(rhs.m_deallocFragment));
	}

	void *StaticAllocator::Allocate()
	{
		if(m_allocFragment==0 || m_allocFragment->numBlocksAvailable==0)
		{

			unsigned int i;
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
		assert(m_allocFragment!=0);
		assert(m_allocFragment->numBlocksAvailable>0);
		return m_allocFragment->Allocate(m_blockSize);
	}

	void StaticAllocator::Deallocate(void* p, CacheType type)
	{
		assert(!m_fragments.empty());
		assert(&(m_fragments.front()) <= m_deallocFragment);
		assert(&(m_fragments.back()) >= m_deallocFragment);

		m_deallocFragment  = VicinityFind(p);
		assert(m_deallocFragment);

		DoDeallocate(p,type);
	}
	void StaticAllocator::Release()
	{
		assert(m_fragments.empty());
		m_fragments.clear();
	}
	void StaticAllocator::Delete()
	{
		unsigned int trav;
		Fragment *tmp = NULL; 
		for(trav=0;trav<m_fragments.size();trav++)
		{
			tmp=&(m_fragments.at(trav));
			if( !tmp ) continue; 
			tmp->Release();
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
			int trav;
			for(trav=m_fragments.size()-1;trav>=0;trav--)
			{
				Fragment *tmp=&(m_fragments.at(trav));
				if(tmp->numBlocksAvailable==m_numBlocks)
				{
					tmp->Release();
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
		assert(!m_fragments.empty());
		assert(m_deallocFragment);

		unsigned int chunkLength = (unsigned int)m_numBlocks * m_blockSize;

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

	void StaticAllocator::DoDeallocate(void* p, CacheType type)
	{
		assert(m_deallocFragment->m_Data <= p);
		assert(m_deallocFragment->m_Data + m_numBlocks * m_blockSize > p);



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
				lastFragment->Release();
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



	TinyObjAllocator::TinyObjAllocator(unsigned int fragmentSize,unsigned int maxObjectSize)
		: m_lastAlloc(0), m_lastDealloc(0), m_fragmentSize(fragmentSize), m_maxObjectSize(maxObjectSize) 
	{   
	}
	TinyObjAllocator::~TinyObjAllocator()
	{
		unsigned int trav;
		for(trav=0;trav<m_pool.size();trav++)
		{
			StaticAllocator *tmp=&(m_pool.at(trav));
			tmp->Delete();
		}
		m_pool.clear();

	}

	void* TinyObjAllocator::Allocate(unsigned int numBytes)
	{
		if (numBytes > m_maxObjectSize) 
			return operator new(numBytes);

		if (m_lastAlloc && m_lastAlloc->GetBlockSize() == numBytes)
		{
			return m_lastAlloc->Allocate();
		}
		StaticAllocator *i;

		unsigned int trav;
		for(trav=0;trav<m_pool.size();trav++)
		{
			if(m_pool.at(trav).GetBlockSize()>=numBytes)
				break;
		}
		unsigned int idx=trav;
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

	void TinyObjAllocator::Deallocate(void* p, unsigned int numBytes, CacheType type)
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

		unsigned int trav;
		for(trav=0;trav<m_pool.size();trav++)
		{
			if(m_pool.at(trav).GetBlockSize()>=numBytes)
				break;
		}
		int idx=trav;

		assert(idx !=-1);
		i=&(m_pool.at(idx));
		assert(i->GetBlockSize() == numBytes);
		m_lastDealloc = i;
		m_lastDealloc->Deallocate(p,type);

		if(!(type&CACHE_TYPE_STATIC) )
		{
			m_lastDealloc->Compress(type);
			if(m_lastDealloc->IsEmpty())
			{
				m_lastDealloc->Release();
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
		int trav;
		for(trav=m_pool.size()-1;trav>=0;trav--)
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
