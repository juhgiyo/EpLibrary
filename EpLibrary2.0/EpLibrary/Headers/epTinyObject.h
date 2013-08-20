/*! 
@file epTinyObject.h
@author Andrei Alexandrescu
  recreated by Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
  			   <http://github.com/juhgiyo/eplibrary>
@date April 16, 2011
@brief Tiny Object Header Interface
@version 2.0

@section LICENSE

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

@section DESCRIPTION

An Interface for Tiny Object.

*/
#ifndef __EP_TINY_OBJECT_H__
#define __EP_TINY_OBJECT_H__
#include "epLib.h"
#include "epSingletonHolder.h"
#include "epThreadSafeClass.h"


namespace epl {
	typedef int CacheType;
	enum CACHE_ENUM{
		CACHE_TYPE_FRAGMENT = 0x01,
		CACHE_TYPE_STATIC   = 0x02,
		CACHE_TYPE_POOL     = 0x04,
		CACHE_TYPE_COMPRESS = 0x08
	};

#ifndef DEFAULT_FRAGMENT_SIZE
#define DEFAULT_FRAGMENT_SIZE 4096
#endif

#ifndef MAX_TINY_OBJECT_SIZE
#define MAX_TINY_OBJECT_SIZE 64
#endif

#ifndef UCHAR_MAX
#define UCHAR_MAX     0xff      // maximum unsigned char value 
#endif 

	class EP_LIBRARY StaticAllocator
	{
	public:
		class EP_LIBRARY Fragment
		{
			friend StaticAllocator;

			void Init(size_t blockSize, unsigned char blocks);
			void *Allocate(size_t blockSize);
			void Deallocate(void *p, size_t blockSize);
			void Reset(size_t blockSize, unsigned char blocks);
			void Clear();
			unsigned char *m_Data;
			unsigned char firstAvailableBlock;
			unsigned char numBlocksAvailable;
		};

		StaticAllocator(size_t blockSize = 0);
		StaticAllocator(const StaticAllocator& rhs);
		StaticAllocator& operator=(const StaticAllocator& rhs);

		void Swap(StaticAllocator& rhs);

		void* Allocate();
		void Deallocate(void *p, CacheType type);
		void Clear();
		void Compress(CacheType type);
		void Delete();

		size_t GetBlockSize() const
		{
			return m_blockSize;
		}
		bool operator<(size_t rhs) const
		{ 
			return GetBlockSize() < rhs; 
		}

		size_t Size() const
		{
			return m_fragments.size();
		}
		bool IsEmpty() const
		{
			return m_fragments.empty();
		}
	private:
		void Release(void* p, CacheType type);
		Fragment* VicinityFind(void* p);
		size_t m_blockSize;
		unsigned char m_numBlocks;
		typedef std::vector<Fragment> Fragments;
		Fragments m_fragments;
		Fragment* m_allocFragment;
		Fragment* m_deallocFragment;
	};


	class EP_LIBRARY TinyObjAllocator
	{
	public:
		TinyObjAllocator(
			size_t fragmentSize, 
			size_t maxObjectSize);
		~TinyObjAllocator();

		void* Allocate(size_t numBytes);
		void Deallocate(void* p, size_t size, CacheType type);
		void Compress(CacheType type);


	private:
		TinyObjAllocator(const TinyObjAllocator&);
		TinyObjAllocator& operator=(const TinyObjAllocator&);

		typedef std::vector<StaticAllocator> Pool;
		Pool m_pool;
		StaticAllocator* m_lastAlloc;
		StaticAllocator* m_lastDealloc;
		size_t m_fragmentSize;
		size_t m_maxObjectSize;
	};

	template
		<
		CacheType cacheType = CACHE_TYPE_STATIC|CACHE_TYPE_POOL,
		size_t fragmentSize = DEFAULT_FRAGMENT_SIZE,
		size_t maxTinyObjectSize = MAX_TINY_OBJECT_SIZE
		>
	class EP_LIBRARY TinyObject
	{

		struct EP_LIBRARY MyTinyObjAllocator : public TinyObjAllocator
		{
			MyTinyObjAllocator() : TinyObjAllocator(fragmentSize, maxTinyObjectSize)
			{}
		};
		typedef ThreadSafeClass< MyTinyObjAllocator > MyThreadSafeClass;
	public:
		static void* operator new(size_t size)
		{
			typename MyThreadSafeClass::InstantLock lock;
#if (MAX_TINY_OBJECT_SIZE != 0) && (DEFAULT_FRAGMENT_SIZE != 0)
			return SingletonHolder<MyTinyObjAllocator>::Instance().Allocate(size);
#else
			return ::operator new(size);
#endif
		}
		static void operator delete(void* p, size_t size)
		{
			typename MyThreadSafeClass::InstantLock lock;
#if (MAX_TINY_OBJECT_SIZE != 0) && (DEFAULT_FRAGMENT_SIZE != 0)
			SingletonHolder<MyTinyObjAllocator>::Instance().Deallocate(p, size, cacheType);
#else
			::operator delete(p, size);
#endif
		}

		static void Compress()
		{
			typename MyThreadSafeClass::InstantLock lock;
#if (MAX_TINY_OBJECT_SIZE != 0) && (DEFAULT_FRAGMENT_SIZE != 0)
			SingletonHolder<MyTinyObjAllocator>::Instance().Compress(cacheType);
#else
#endif
		}
		virtual ~TinyObject() {}
	};
}

#endif //__EP_TINY_OBJECT_H__