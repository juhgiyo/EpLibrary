/*! 
@file epDelegate.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date March 08, 2012
@brief C# Style Delegate Interface
@version 2.0

@section LICENSE

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

@section DESCRIPTION

An Interface for C# Style Delegate Class.

*/
#ifndef __EP_DELEGATE_H__
#define __EP_DELEGATE_H__
#include "epLib.h"
#include "epSystem.h"
#include <vector>
#include "epCriticalSectionEx.h"
#include "epMutex.h"
#include "epNoLock.h"

using namespace std;

namespace epl
{
	/*! 
	@class Delegate epDelegate.h
	@brief A class for C# Style Delegate.
	*/
	template<typename RetType,typename ArgType=void>
	class Delegate{
	public:
		/// Function Pointer Type Definition
		typedef RetType (*FuncType) (ArgType);

		/*!
		Default Constructor

		Initializes the delegate
		@param[in] lockPolicyType The lock policy
		*/
		Delegate(LockPolicy lockPolicyType=EP_LOCK_POLICY)
		{
			m_lockPolicy=lockPolicyType;
			switch(lockPolicyType)
			{
			case LOCK_POLICY_CRITICALSECTION:
				m_delegateLock=EP_NEW CriticalSectionEx();
				break;
			case LOCK_POLICY_MUTEX:
				m_delegateLock=EP_NEW Mutex();
				break;
			case LOCK_POLICY_NONE:
				m_delegateLock=EP_NEW NoLock();
				break;
			default:
				m_delegateLock=NULL;
				break;
			}
		}

		/*!
		Default Constructor

		Initializes the delegate with given function pointer
		@param[in] func the initial function pointer
		@param[in] lockPolicyType The lock policy
		*/
		Delegate(RetType (*func)(ArgType),LockPolicy lockPolicyType=EP_LOCK_POLICY)
		{
			m_lockPolicy=lockPolicyType;
			m_funcList.push_back(func);
			switch(lockPolicyType)
			{
			case LOCK_POLICY_CRITICALSECTION:
				m_delegateLock=EP_NEW CriticalSectionEx();
				break;
			case LOCK_POLICY_MUTEX:
				m_delegateLock=EP_NEW Mutex();
				break;
			case LOCK_POLICY_NONE:
				m_delegateLock=EP_NEW NoLock();
				break;
			default:
				m_delegateLock=NULL;
				break;
			}
		}

		/*!
		Copy Constructor

		Initializes the delegate with given delegate
		@param[in] orig the delegate
		*/
		Delegate(const Delegate<RetType,ArgType> &orig)
		{
			m_lockPolicy=orig.m_lockPolicy;
			switch(m_lockPolicy)
			{
			case LOCK_POLICY_CRITICALSECTION:
				m_delegateLock=EP_NEW CriticalSectionEx();
				break;
			case LOCK_POLICY_MUTEX:
				m_delegateLock=EP_NEW Mutex();
				break;
			case LOCK_POLICY_NONE:
				m_delegateLock=EP_NEW NoLock();
				break;
			default:
				m_delegateLock=NULL;
				break;
			}
			LockObj lock(b.m_delegateLock);
			m_funcList=b.m_funcList;
		}

		/*!
		Default Destructor

		Destroy the delegate
		*/
		virtual ~Delegate()
		{
			if(m_delegateLock)
				EP_DELETE m_delegateLock;
		}	
		
		/*!
		Assignment Operator Overloading

		the Packet set as given packet b
		@param[in] b right side of packet
		@return this object
		*/
		Delegate<RetType,ArgType> & operator=(const Delegate<RetType,ArgType>&b)
		{
			if(this!=&b)
			{
				if(m_delegateLock)
				{
					EP_DELETE m_delegateLock;
					m_delegateLock=NULL;
				}
				
				m_lockPolicy=b.m_lockPolicy;
				switch(m_lockPolicy)
				{
				case LOCK_POLICY_CRITICALSECTION:
					m_delegateLock=EP_NEW CriticalSectionEx();
					break;
				case LOCK_POLICY_MUTEX:
					m_delegateLock=EP_NEW Mutex();
					break;
				case LOCK_POLICY_NONE:
					m_delegateLock=EP_NEW NoLock();
					break;
				default:
					m_delegateLock=NULL;
					break;
				}
				LockObj lock(b.m_delegateLock);
				m_funcList=b.m_funcList;
			}
			return *this;
		}

		/*!
		Check if the delegate is empty.
		@return true if the delegate is empty, otherwise false.
		*/
		bool IsEmpty() const
		{
			LockObj lock(m_delegateLock);
			return m_funcList.empty();
		}

		/*!
		Return the size of the delegate.
		@return the size of the delegate.
		*/
		int Size() const
		{
			LockObj lock(m_delegateLock);
			return m_funcList.size();
		}

		/*!
		Clear the delegate.
		*/
		void Clear()
		{
			LockObj lock(m_delegateLock);
			m_funcList.clear();
		}

		/*!
		Initialize the delegate with given function pointer
		@param[in] func The initial function pointer
		@return reference to this delegate
		*/
		virtual Delegate<RetType,ArgType> & operator =(RetType (*func)(ArgType))
		{
			LockObj lock(m_delegateLock);
			m_funcList.clear();
			m_funcList.push_back(func);
			return *this;
		}

		/*!
		Append the delegate with given function pointer
		@param[in] func The function pointer to append
		@return reference to this delegate
		*/
		virtual Delegate<RetType,ArgType> & operator +=(RetType (*func)(ArgType))
		{
			LockObj lock(m_delegateLock);
			m_funcList.push_back(func);
			return *this;
		}

		/*!
		Add the delegate with given function pointer
		@param[in] func The function pointer to append
		@return the delegate with the function pointers from this delegate and given function pointer
		*/
		virtual Delegate<RetType,ArgType> operator +(RetType (*func)(ArgType)) const
		{
			Delegate<RetType,ArgType> ret(*this);
			ret+=func;
			return ret;
		}

		/*!
		Append the delegate with function pointers from the given delegate
		@param[in] right The delegate to append
		@return reference to this delegate
		*/
		virtual Delegate<RetType,ArgType> & operator +=(const Delegate<RetType,ArgType> &right)
		{
			LockObj lock(m_delegateLock);
			vector<RetType (*)(ArgType)>::const_iterator iter;
			if(this==&right)
			{
				Delegate<RetType,ArgType> tempDel(right);
				for(iter=tempDel.m_funcList.begin();iter!=tempDel.m_funcList.end();iter++)
				{
					m_funcList.push_back(*iter);
				}
				return *this;
			}

			for(iter=right.m_funcList.begin();iter!=right.m_funcList.end();iter++)
			{
				m_funcList.push_back(*iter);
			}
			return *this;
		}

		/*!
		Add the delegate with function pointers from the given delegate
		@param[in] right The delegate to append
		@return the delegate with the function pointers from this delegate and given delegate
		*/
		virtual Delegate<RetType,ArgType> operator +(const Delegate<RetType,ArgType> &right) const
		{
			Delegate<RetType,ArgType> ret(*this);
			ret+=right;
			return ret;
		}

		/*!
		Remove the given function pointers from this delegate
		@param[in] func The function pointer to remove
		@return reference to this delegate
		*/
		virtual Delegate<RetType,ArgType> & operator -=(RetType (*func)(ArgType))
		{
			LockObj lock(m_delegateLock);
			vector<RetType (*)(ArgType)>::iterator iter;
			for(iter=m_funcList.begin();iter!=m_funcList.end();)
			{
				if(*iter==func)
				{
					iter=m_funcList.erase(iter);
				}
				else 
					iter++;
			}

			return *this;
		}

		/*!
		Remove given function pointer from this delegate
		@param[in] func The function pointer to remove
		@return the delegate with given function pointer extracted from this delegate
		*/
		virtual Delegate<RetType,ArgType> operator -(RetType (*func)(ArgType)) const
		{
			Delegate<RetType,ArgType> ret(*this);
			ret-=func;
			return ret;
		}

		/*!
		Remove the function pointers of the given delegate from this delegate
		@param[in] right The delegate to remove
		@return reference to this delegate
		*/
		virtual Delegate<RetType,ArgType> & operator -=(const Delegate<RetType,ArgType> &right)
		{
			LockObj lock(m_delegateLock);

			vector<RetType (*)(ArgType)>::const_iterator rightIter;
			vector<RetType (*)(ArgType)>::iterator iter;
			if(this==&right)
			{
				Delegate<RetType,ArgType> tempDel(right);
				for(rightIter=tempDel.m_funcList.begin();rightIter!=tempDel.m_funcList.end();rightIter++)
				{
					for(iter=m_funcList.begin();iter!=m_funcList.end();)
					{
						if(*iter==*rightIter)
						{
							iter=m_funcList.erase(iter);
						}
						else 
							iter++;
					}
				}
				return *this;
			}
			for(rightIter=right.m_funcList.begin();rightIter!=right.m_funcList.end();rightIter++)
			{
				for(iter=m_funcList.begin();iter!=m_funcList.end();)
				{
					if(*iter==*rightIter)
					{
						iter=m_funcList.erase(iter);
					}
					else 
						iter++;
				}
			}
			return *this;
		}

		/*!
		Remove the function pointers of the given delegate from this delegate 
		@param[in] right The delegate to remove
		@return the delegate with the function pointers of given delegate extracted from this delegate
		*/
		virtual Delegate<RetType,ArgType> operator -(const Delegate<RetType,ArgType> &right) const
		{
			Delegate<RetType,ArgType> ret(*this);
			ret-=right;
			return ret;
		}

		/*!
		Return the function pointer at given index
		@param[in] idx the index to return the function pointer
		@return the function pointer at given index
		*/
		virtual FuncType operator [](size_t idx) const
		{
			EP_ASSERT(idx<m_funcList.size());
			vector<RetType (*)(ArgType)>::const_iterator iter=m_funcList.begin();
			iter+=idx;
			return *iter;
		
		}

		/*!
		Execute the function pointers within this delegate with the argument given
		@param[in] arg the argument of the function pointer
		@return the return value of the last function pointer
		*/
		virtual RetType operator ()(ArgType arg)
		{
			EP_ASSERT(m_funcList.size());
			vector<RetType (*)(ArgType)>::iterator iter;
			vector<RetType (*)(ArgType)>::iterator lastIterCheck;
			for(iter=m_funcList.begin();iter!=m_funcList.end();iter++)
			{
				lastIterCheck=iter;
				lastIterCheck++;
				if(lastIterCheck==m_funcList.end())
					break;
				else
					(*iter)(arg);
			}
			return (*iter)(arg);
		}

	private:
		/// function pointer list
		vector<RetType (*)(ArgType)> m_funcList;
		/// lock
		BaseLock *m_delegateLock;
		/// Lock Policy
		LockPolicy m_lockPolicy;
	};

	/*! 
	@class Delegate epDelegate.h
	@brief A partial specialization class for C# Style Delegate with void argument.
	*/
	template<typename RetType>
	class Delegate<RetType,void>{
	public:
		/// Function Pointer Type Definition
		typedef RetType (*FuncType) (void);

		/*!
		Default Constructor

		Initializes the delegate
		@param[in] lockPolicyType The lock policy
		*/
		Delegate(LockPolicy lockPolicyType=EP_LOCK_POLICY)
		{
			m_lockPolicy=lockPolicyType;
			switch(lockPolicyType)
			{
			case LOCK_POLICY_CRITICALSECTION:
				m_delegateLock=EP_NEW CriticalSectionEx();
				break;
			case LOCK_POLICY_MUTEX:
				m_delegateLock=EP_NEW Mutex();
				break;
			case LOCK_POLICY_NONE:
				m_delegateLock=EP_NEW NoLock();
				break;
			default:
				m_delegateLock=NULL;
				break;
			}
		}

		/*!
		Default Constructor

		Initializes the delegate with given function pointer
		@param[in] func the initial function pointer
		@param[in] lockPolicyType The lock policy
		*/
		Delegate(RetType (*func)(void),LockPolicy lockPolicyType=EP_LOCK_POLICY)
		{
			m_lockPolicy=lockPolicyType;
			m_funcList.push_back(func);
			switch(lockPolicyType)
			{
			case LOCK_POLICY_CRITICALSECTION:
				m_delegateLock=EP_NEW CriticalSectionEx();
				break;
			case LOCK_POLICY_MUTEX:
				m_delegateLock=EP_NEW Mutex();
				break;
			case LOCK_POLICY_NONE:
				m_delegateLock=EP_NEW NoLock();
				break;
			default:
				m_delegateLock=NULL;
				break;
			}
		}

		/*!
		Copy Constructor

		Initializes the delegate with given delegate
		@param[in] orig the delegate
		*/
		Delegate(const Delegate<RetType,void> &orig)
		{

			m_lockPolicy=orig.m_lockPolicy;
			switch(m_lockPolicy)
			{
			case LOCK_POLICY_CRITICALSECTION:
				m_delegateLock=EP_NEW CriticalSectionEx();
				break;
			case LOCK_POLICY_MUTEX:
				m_delegateLock=EP_NEW Mutex();
				break;
			case LOCK_POLICY_NONE:
				m_delegateLock=EP_NEW NoLock();
				break;
			default:
				m_delegateLock=NULL;
				break;
			}
			LockObj lock(b.m_delegateLock);
			m_funcList=orig.m_funcList;
		}

		/*!
		Default Destructor

		Destroy the delegate
		*/
		virtual ~Delegate()
		{
			if(m_delegateLock)
				EP_DELETE m_delegateLock;
		}

		/*!
		Assignment Operator Overloading

		the Delegate set as given packet b
		@param[in] b right side of packet
		@return this object
		*/
		Delegate<RetType,void> & operator=(const Delegate<RetType,void>&b)
		{
			if(this!=&b)
			{
				if(m_delegateLock)
				{
					EP_DELETE m_delegateLock;
					m_delegateLock=NULL;
				}
				
				m_lockPolicy=b.m_lockPolicy;
				switch(m_lockPolicy)
				{
				case LOCK_POLICY_CRITICALSECTION:
					m_delegateLock=EP_NEW CriticalSectionEx();
					break;
				case LOCK_POLICY_MUTEX:
					m_delegateLock=EP_NEW Mutex();
					break;
				case LOCK_POLICY_NONE:
					m_delegateLock=EP_NEW NoLock();
					break;
				default:
					m_delegateLock=NULL;
					break;
				}
				LockObj lock(b.m_delegateLock);
				m_funcList=b.m_funcList;
			}
			return *this;
		}

		/*!
		Check if the delegate is empty.
		@returns Returns true if the delegate is empty, otherwise false.
		*/
		bool IsEmpty() const
		{
			LockObj lock(m_delegateLock);
			return m_funcList.empty();
		}

		/*!
		Return the size of the delegate.
		@return the size of the delegate.
		*/
		int Size() const
		{
			LockObj lock(m_delegateLock);
			return m_funcList.size();
		}

		/*!
		Clear the delegate.
		*/
		void Clear()
		{
			LockObj lock(m_delegateLock);
			m_funcList.clear();
		}

		/*!
		Initialize the delegate with given function pointer
		@param[in] func The initial function pointer
		@return reference to this delegate
		*/
		virtual Delegate<RetType,void> & operator =(RetType (*func)(void))
		{
			LockObj lock(m_delegateLock);
			m_funcList.clear();
			m_funcList.push_back(func);
			return *this;
		}

		/*!
		Append the delegate with given function pointer
		@param[in] func The function pointer to append
		@return reference to this delegate
		*/
		virtual Delegate<RetType,void> & operator +=(RetType (*func)(void))
		{
			LockObj lock(m_delegateLock);
			m_funcList.push_back(func);
			return *this;
		}

		/*!
		Add the delegate with given function pointer
		@param[in] func The function pointer to append
		@return the delegate with the function pointers from this delegate and given function pointer
		*/
		virtual Delegate<RetType,void> operator +(RetType (*func)(void)) const
		{
			Delegate<RetType,void> ret(*this);
			ret+=func;
			return ret;
		}

		/*!
		Append the delegate with function pointers from the given delegate
		@param[in] right The delegate to append
		@return reference to this delegate
		*/
		virtual Delegate<RetType,void> & operator +=(const Delegate<RetType,void> &right)
		{
			LockObj lock(m_delegateLock);
			vector<RetType (*)(void)>::const_iterator iter;
			if(this==&right)
			{
				Delegate<RetType,void> tempDel(right);
				for(iter=tempDel.m_funcList.begin();iter!=tempDel.m_funcList.end();iter++)
				{
					m_funcList.push_back(*iter);
				}
				return *this;
			}
			
			for(iter=right.m_funcList.begin();iter!=right.m_funcList.end();iter++)
			{
				m_funcList.push_back(*iter);
			}
			return *this;
		}

		/*!
		Add the delegate with function pointers from the given delegate
		@param[in] right The delegate to append
		@return the delegate with the function pointers from this delegate and given delegate
		*/
		virtual Delegate<RetType,void> operator +(const Delegate<RetType,void> &right) const
		{
			Delegate<RetType,void> ret(*this);
			ret+=right;
			return ret;
		}

		/*!
		Remove the given function pointers from this delegate
		@param[in] func The function pointer to remove
		@return reference to this delegate
		*/
		virtual Delegate<RetType,void> & operator -=(RetType (*func)(void))
		{
			LockObj lock(m_delegateLock);
			vector<RetType (*)(void)>::iterator iter;
			for(iter=m_funcList.begin();iter!=m_funcList.end();)
			{
				if(*iter==func)
				{
					iter=m_funcList.erase(iter);
				}
				else 
					iter++;
			}

			return *this;
		}

		/*!
		Remove given function pointer from this delegate
		@param[in] func The function pointer to remove
		@return the delegate with given function pointer extracted from this delegate
		*/
		virtual Delegate<RetType,void> operator -(RetType (*func)(void)) const
		{
			Delegate<RetType,void> ret(*this);
			ret-=func;
			return ret;
		}

		/*!
		Remove the function pointers of the given delegate from this delegate
		@param[in] right The delegate to remove
		@return reference to this delegate
		*/
		virtual Delegate<RetType,void> & operator -=(const Delegate<RetType,void> &right)
		{
			LockObj lock(m_delegateLock);

			vector<RetType (*)(void)>::const_iterator rightIter;
			vector<RetType (*)(void)>::iterator iter;
			if(this==&right)
			{
				Delegate<RetType,void> tempDel(right);
				for(rightIter=tempDel.m_funcList.begin();rightIter!=tempDel.m_funcList.end();rightIter++)
				{
					for(iter=m_funcList.begin();iter!=m_funcList.end();)
					{
						if(*iter==*rightIter)
						{
							iter=m_funcList.erase(iter);
						}
						else 
							iter++;
					}
				}
				return *this;
			}
			for(rightIter=right.m_funcList.begin();rightIter!=right.m_funcList.end();rightIter++)
			{
				for(iter=m_funcList.begin();iter!=m_funcList.end();)
				{
					if(*iter==*rightIter)
					{
						iter=m_funcList.erase(iter);
					}
					else 
						iter++;
				}
			}
			return *this;
		}

		/*!
		Remove the function pointers of the given delegate from this delegate 
		@param[in] right The delegate to remove
		@return the delegate with the function pointers of given delegate extracted from this delegate
		*/
		virtual Delegate<RetType,void> operator -(const Delegate<RetType,void> &right) const
		{
			Delegate<RetType,void> ret(*this);
			ret-=right;
			return ret;
		}

		/*!
		Return the function pointer at given index
		@param[in] idx the index to return the function pointer
		@return the function pointer at given index
		*/
		virtual FuncType operator [](size_t idx) const
		{
			EP_ASSERT(idx<m_funcList.size());
			vector<RetType (*)(void)>::const_iterator iter=m_funcList.begin();
			iter+=idx;
			return *iter;

		}

		/*!
		Execute the function pointers within this delegate
		@return the return value of the last function pointer
		*/
		virtual RetType operator ()()
		{
			EP_ASSERT(m_funcList.size());
			vector<RetType (*)(void)>::iterator iter;
			vector<RetType (*)(void)>::iterator lastIterCheck;
			for(iter=m_funcList.begin();iter!=m_funcList.end();iter++)
			{
				lastIterCheck=iter;
				lastIterCheck++;
				if(lastIterCheck==m_funcList.end())
					break;
				else
					(*iter)();
			}
			return (*iter)();
		}

	private:
		/// function pointer list
		vector<RetType (*)(void)> m_funcList;
		/// lock
		BaseLock *m_delegateLock;
		/// Lock Policy
		LockPolicy m_lockPolicy;
	};

	template<typename RetType,typename ArgType>
	Delegate<RetType,ArgType> operator +(RetType (*func)(ArgType),const Delegate<RetType,ArgType>& right)
	{
		Delegate<RetType,ArgType> ret(func);
		ret+=right;
		return ret;
	}

// 	template<typename RetType,typename ArgType>
// 	Delegate<RetType,ArgType> operator +(RetType (*func)(ArgType),RetType (*func2)(ArgType))
// 	{
// 		Delegate<RetType,ArgType> ret(func);
// 		ret+=func2;
// 		return ret;
// 	}
}


#endif //__EP_DELEGATE_H__