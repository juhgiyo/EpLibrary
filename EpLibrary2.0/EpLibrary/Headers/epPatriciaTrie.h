/*! 
@file epPatriciaTrie.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date June 09, 2008
@brief Patricia Trie Interface
@version 2.0

@section LICENSE

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

@section DESCRIPTION

An Interface for Patricia Trie Template class.

*/
#ifndef __EP_PATRICIA_TRIE_H__
#define __EP_PATRICIA_TRIE_H__

#include "epLib.h"
#include "epBinarySearch.h"
#include <vector>
using namespace std;

namespace epl
{
	/// Enumeration Type for Patricia Trie Mode
	typedef enum _patriciaTrieMode{
		/// Patricia Trie Mode using Recursive operation
		PATRICIA_TRIE_MODE_RECURSIVE,
		/// Patricia Trie Mode using Loop operation
		PATRICIA_TRIE_MODE_LOOP
	}PatriciaTrieMode;

	/*! 
	@class PatriciaTrie epPatriciaTrie.h
	@brief A Patricia Trie Template class.
	*/
	template<typename CharacterType, typename DataType , CharacterType Terminator=(CharacterType)0, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)=CompClass<CharacterType>::CompFunc>
	class PatriciaTrie
	{
	public:

		/*!
		Default Constructor

		Initializes the  Patricia Trie
		@param[in] lockPolicyType The lock policy
		@param[in] mode the flag for recursive or loop mode
		*/
		PatriciaTrie(PatriciaTrieMode mode=PATRICIA_TRIE_MODE_LOOP,LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the Patricia Trie with given trie
		@param[in] b the Patricia Trie Object to copy from
		*/
		PatriciaTrie(const PatriciaTrie & b);

		/*!
		Default Destructor

		Destroys the Patricia Trie
		*/
		virtual ~PatriciaTrie();

		/*!
		Initialize this trie to given trie
		@param[in] b the Patricia Trie structure to initialize this trie
		@return the result Patricia Trie
		*/
		PatriciaTrie &operator=(const PatriciaTrie & b);

		/*!
		Return the reference to data with given string if not exist insert and return the reference to data
		@param[in] str The string to return the reference to the data.
		@return the reference to the data with given string.
		*/
		DataType &operator[](const CharacterType * str);

		/*!
		Return the reference to data with given string and return the reference to data
		@param[in] str The string to return the reference to the data.
		@return the reference to the data with given string.
		*/
		const DataType &operator[](const CharacterType * str) const;

		/*!
		Insert the string with given data to the trie
		@param[in] str The string value to insert.
		@param[in] data the data with the given string
		@return true if succeeded otherwise false
		*/
		bool Insert(const CharacterType* str,const DataType &data);

		/*!
		Remove the given string from the trie
		@param[in] str The string value to remove
		@return true if succeeded otherwise false
		*/
		bool Erase(const CharacterType* str);

		/*!
		Find the given string from the trie and return the data with the given string
		@param[in] str The string value to find
		@param[out] retData the data with the given string
		@return true if succeeded otherwise false
		*/
		bool Find(const CharacterType* str, DataType &retData) const;

		/*!
		Find the all strings starting with given string from the trie and return the string and data pairs
		@param[in] str The string value to find
		@param[out] retStrDataPairList the string and data pairs found
		@return true if succeeded otherwise false
		*/
		bool FindAll(const CharacterType* str, vector<Pair<const CharacterType*,DataType> > &retStrDataPairList) const;

		/*!
		Clear the trie
		*/
		void Clear();

		/*!
		Check if the trie is empty
		@return true if trie is empty otherwise false
		*/
		bool IsEmpty() const;


		/*!
		return the number of element in the trie.
		@return the number of element in the trie
		*/
		unsigned int Size() const;
	protected:

		/*! 
		@class BasePatriciaTrieNode epPatriciaTrie.h
		@brief A Base Patricia Trie Node class.
		*/
		class BasePatriciaTrieNode
		{
		public:
			/*!
			Default Constructor

			Initializes the Base Patricia Trie Node
			@param[in] c the character of this node
			@param[in] isLeaf flag whether this node is a leaf or not
			*/
			BasePatriciaTrieNode(CharacterType c, bool isLeaf=false);

			/*!
			Default Copy Constructor

			Initializes the Base Patricia Trie Node with given node
			@param[in] b the Base Patricia Trie Node Object to copy from
			*/
			BasePatriciaTrieNode(const BasePatriciaTrieNode &b);

			/*!
			Default Destructor

			Destroys the Base Patricia Trie Node
			*/
			virtual ~BasePatriciaTrieNode();

			/*!
			Initialize this node to given node
			@param[in] b the Base Patricia Trie Node structure to initialize this node
			@return the result Base Patricia Trie Node
			*/
			BasePatriciaTrieNode &operator=(const BasePatriciaTrieNode & b);

			/*!
			Return this node's character
			@return the character of this node
			*/
			const CharacterType &GetCharacter() const;

			/*!
			Set the character of this node
			@param[in] c the character to set
			*/
			void SetCharacter(const CharacterType & c);

			/*!
			Return whether this node is a leaf or not
			@return true if this node is a leaf otherwise false
			*/
			bool IsLeaf() const;


		protected:
			/// the character of this node
			CharacterType m_character;
			/// flag whether this node is a leaf or not
			bool m_isLeaf;
		};

		/*! 
		@class PatriciaTrieLeaf epPatriciaTrie.h
		@brief A Patricia Trie Leaf class.
		*/
		class PatriciaTrieLeaf: public BasePatriciaTrieNode
		{
		public:
			/*!
			Default Constructor

			Initializes the Patricia Trie Leaf
			@param[in] str the string of this node
			@param[in] data the data of this node holding
			*/
			PatriciaTrieLeaf(const CharacterType *str,const DataType &data);

			/*!
			Default Copy Constructor

			Initializes the Patricia Trie Leaf with given leaf
			@param[in] b the Patricia Trie Leaf Object to copy from
			*/
			PatriciaTrieLeaf(const PatriciaTrieLeaf &b);

			/*!
			Default Destructor

			Destroys the Patricia Trie Leaf
			*/
			virtual ~PatriciaTrieLeaf();

			/*!
			Initialize this leaf to given leaf
			@param[in] b the Patricia Trie Leaf structure to initialize this leaf
			@return the result Patricia Trie Leaf
			*/
			PatriciaTrieLeaf &operator=(const PatriciaTrieLeaf & b);

			/*!
			Return this node's data
			@return the data of this node
			*/
			const DataType &GetData() const;

			/*!
			Return this node's data
			@return the data of this node
			*/
			DataType &GetData();

			/*!
			Set the data of this node
			@param[in] data the data to set
			*/
			void SetData(const DataType &data);

			/*!
			Return this node's string
			@return the string of this node
			*/
			const CharacterType *GetString() const;

		protected:
			/// data of this node
			DataType m_data;
			/// string of this node
			vector<CharacterType> m_string;
			/// string terminator character holder
			CharacterType m_terminator;
		};

		/*! 
		@class PatriciaTrieNode epPatriciaTrie.h
		@brief A Patricia Trie Node class.
		*/
		class PatriciaTrieNode: public BasePatriciaTrieNode
		{

		public:
			/*!
			Default Constructor

			Initializes the Patricia Trie Node
			@param[in] c the character of this node
			*/
			PatriciaTrieNode(CharacterType c);

			/*!
			Default Copy Constructor

			Initializes the Patricia Trie Node with given node
			@param[in] b the Patricia Trie Node Object to copy from
			*/
			PatriciaTrieNode(const PatriciaTrieNode &b);

			/*!
			Default Destructor

			Destroys the Patricia Trie Node
			*/
			virtual ~PatriciaTrieNode();

			/*!
			Initialize this node to given node
			@param[in] b the Patricia Trie Node structure to initialize this node
			@return the result Patricia Trie Node
			*/
			PatriciaTrieNode &operator=(const PatriciaTrieNode & b);

			/*!
			Return the character list of this node holding
			@return the character list of this node
			*/
			const vector<BasePatriciaTrieNode *> &GetList() const;

			/*!
			Return the character list of this node holding
			@return the character list of this node
			*/
			vector<BasePatriciaTrieNode *> &GetList();

			/*!
			Add a new node with given character to the list of this node
			@param[in] c the character for the new node.
			@remark if the add fails then returns NULL.
			@return the pointer to the new node in the list
			*/
			BasePatriciaTrieNode *AddNode(const CharacterType c);

			/*!
			Add a new leaf with given character to the list of this node
			@param[in] str the string for the new node
			@param[in] data the data for the new node
			@remark if the add fails then returns NULL.
			@return the pointer to the new leaf in the list
			*/
			PatriciaTrieLeaf *AddLeaf(const CharacterType *str,const DataType& data);

			/*!
			Remove the node with given character
			@param[in] c the character of the node to remove from this node's list
			@remark if the node with the given character contains nodes then remove fails
			@return true if succeeded otherwise false.
			*/
			bool RemoveNode(CharacterType c);

			/*!
			Return the pointer to the node with given character
			@param[in] character the character of the node to find from this node's list
			@remark if the node with the given character does not exists then return NULL.
			@return the pointer to the node with given character
			*/
			BasePatriciaTrieNode *operator[](const CharacterType &character);

			/*!
			Return the pointer to the node with given character
			@param[in] character the character of the node to find from this node's list
			@remark if the node with the given character does not exists then return NULL.
			@return the pointer to the node with given character
			*/
			const BasePatriciaTrieNode *operator[](const CharacterType &character) const;

		protected:
			/// character list of this node
			vector<BasePatriciaTrieNode*> m_charList;
			/// string terminator character holder
			CharacterType m_terminator;
		};

		/*!
		Actually insert the string with given data to the trie by recursive
		@param[in] root the root of the trie
		@param[in] str The string value to insert.
		@param[in] strTrav the current index of the string traversing
		@param[in] data the data with the given string
		@remark returns NULL if insertion fails
		@return Pointer to the node inserted.
		*/
		PatriciaTrieLeaf *insert(BasePatriciaTrieNode *root,const CharacterType* str,unsigned int strTrav,const DataType &data);

		/*!
		Actually remove the given string from the trie by recursive
		@param[in] root the root of the trie
		@param[in] str The string value to erase.
		@param[in] strTrav the current index of the string traversing
		@return true if succeeded otherwise false
		*/
		bool erase(BasePatriciaTrieNode *root,const CharacterType* str,unsigned int strTrav);

		/*!
		Actually find the given string from the trie by recursive and return the data
		@param[in] root the root of the trie
		@param[in] str The string value to find.
		@param[in] strTrav the current index of the string traversing
		@param[out] retData the data with the given string to return
		@remark returns NULL if find fails
		@return Pointer to the node found.
		*/
		PatriciaTrieLeaf * find(BasePatriciaTrieNode *root,const CharacterType* str,unsigned int strTrav, DataType &retData) const;

		/*!
		Actually find the strings starts with the given string from the trie by recursive
		@param[in] root the root of the trie
		@param[in] str The string value to find.
		@param[in] strTrav the current index of the string traversing
		@param[out] retStrDataPairList the string and data pair which the string starts with the given string.
		@return true if succeeded otherwise false.
		*/
		bool findAll(BasePatriciaTrieNode *root,const CharacterType* str,unsigned int strTrav, vector<Pair<const CharacterType*,DataType> > &retStrDataPairList) const;

		/*!
		Traverse the all nodes and return the all string and data pair from the trie by recursive
		@param[in] root the root of the trie
		@param[out] retStrDataPairList the string and data pair in the trie.
		@return true if succeeded otherwise false.
		*/
		void traverseAll(BasePatriciaTrieNode *root, vector<Pair<const CharacterType*,DataType> > &retStrDataPairList) const;


		/*!
		Actually insert the string with given data to the trie by loop
		@param[in] root the root of the trie
		@param[in] str The string value to insert.
		@param[in] strTrav the current index of the string traversing
		@param[in] data the data with the given string
		@remark returns NULL if insertion fails
		@return Pointer to the node inserted.
		*/
		PatriciaTrieLeaf *insertLoop(BasePatriciaTrieNode *root,const CharacterType* str,unsigned int strTrav,const DataType &data);

		/*!
		Actually remove the given string from the trie by loop
		@param[in] root the root of the trie
		@param[in] str The string value to erase.
		@param[in] strTrav the current index of the string traversing
		@return true if succeeded otherwise false
		*/
		bool eraseLoop(BasePatriciaTrieNode *root,const CharacterType* str,unsigned int strTrav);

		/*!
		Actually find the given string from the trie by loop and return the data
		@param[in] root the root of the trie
		@param[in] str The string value to find.
		@param[in] strTrav the current index of the string traversing
		@param[out] retData the data with the given string to return
		@remark returns NULL if find fails
		@return Pointer to the node found.
		*/
		PatriciaTrieLeaf * findLoop(BasePatriciaTrieNode *root,const CharacterType* str,unsigned int strTrav, DataType &retData) const;

		/*!
		Actually find the strings starts with the given string from the trie by loop
		@param[in] root the root of the trie
		@param[in] str The string value to find.
		@param[in] strTrav the current index of the string traversing
		@param[out] retStrDataPairList the string and data pair which the string starts with the given string.
		@return true if succeeded otherwise false.
		*/
		bool findAllLoop(BasePatriciaTrieNode *root,const CharacterType* str,unsigned int strTrav, vector<Pair<const CharacterType*,DataType> > &retStrDataPairList) const;

		/*!
		Traverse the all nodes and return the all string and data pair from the trie by loop
		@param[in] root the root of the trie
		@param[out] retStrDataPairList the string and data pair in the trie.
		@return true if succeeded otherwise false.
		*/
		void traverseAllLoop(BasePatriciaTrieNode *root, vector<Pair<const CharacterType*,DataType> > &retStrDataPairList) const;




		/*!
		Compare the given character with the character of the node
		@param[in] a the pointer to the character
		@param[in] b the pointer to the node
		@return compare result of character and character of the node.
		*/
		static CompResultType NodeCompareFunc(const void *a, const void *b)
		{
			void *_vA=const_cast<void*>(a);
			void *_vB=const_cast<void*>(b);
			CharacterType* _a=reinterpret_cast<CharacterType*>(_vA);
			BasePatriciaTrieNode **_b=reinterpret_cast<BasePatriciaTrieNode**>(_vB);
			return CharCompareFunc(_a,&((*_b)->GetCharacter()));
		}


		/// Root of the trie
		PatriciaTrieNode* m_root;
		/// Total number of strings in the trie
		unsigned int m_totalCount;             
		/// String Terminator holder
		CharacterType m_terminator;
		/// lock
		BaseLock *m_lock;
		/// Lock Policy
		LockPolicy m_lockPolicy;
		/// Patricia Trie Mode
		PatriciaTrieMode m_mode;

	};




	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrie(PatriciaTrieMode mode,LockPolicy lockPolicyType)
	{
		m_totalCount=0;
		m_root=EP_NEW PatriciaTrieNode(Terminator);
		m_terminator=Terminator;
		m_lockPolicy=lockPolicyType;
		m_mode=mode;
		switch(lockPolicyType)
		{
		case LOCK_POLICY_CRITICALSECTION:
			m_lock=EP_NEW CriticalSectionEx();
			break;
		case LOCK_POLICY_MUTEX:
			m_lock=EP_NEW Mutex();
			break;
		case LOCK_POLICY_NONE:
			m_lock=EP_NEW NoLock();
			break;
		default:
			m_lock=NULL;
		}
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrie(const PatriciaTrie & b)
	{
		m_totalCount=b.m_totalCount;
		m_root=EP_NEW PatriciaTrieNode(Terminator);
		*m_root=*(b.m_root);
		m_mode=b.m_mode;
		m_terminator=b.m_terminator;
		m_lockPolicy=b.m_lockPolicy;
		switch(m_lockPolicy)
		{
		case LOCK_POLICY_CRITICALSECTION:
			m_lock=EP_NEW CriticalSectionEx();
			break;
		case LOCK_POLICY_MUTEX:
			m_lock=EP_NEW Mutex();
			break;
		case LOCK_POLICY_NONE:
			m_lock=EP_NEW NoLock();
			break;
		default:
			m_lock=NULL;
			break;
		}

	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::~PatriciaTrie()
	{
		m_lock->Lock();
		if(m_root)
			EP_DELETE m_root;
		m_lock->Unlock();
		if(m_lock)
			EP_DELETE m_lock;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc> &PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::operator=(const PatriciaTrie & b)
	{
		if(this!=&b)
		{
			LockObj lock(m_lock);
			m_totalCount=b.m_totalCount;
			m_mode=b.m_mode;
			if(m_root)
				EP_DELETE m_root;
			m_root=EP_NEW PatriciaTrieNode(Terminator);
			*m_root=*(b.m_root);
			m_terminator=b.m_terminator;
		}
		return *this;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	DataType &PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::operator[](const CharacterType * str)
	{

		EP_VERIFY_INVALID_ARGUMENT_W_MSG(str,"String is NULL");
		DataType retData;
		LockObj lock(m_lock);
		PatriciaTrieLeaf *foundNode=find(m_root,str,0,retData);
		if(foundNode)
		{
			return foundNode->GetData();
		}
		else
		{
			foundNode=insert(m_root,str,0,retData);
			EP_VERIFY_RUNTIME_ERROR_W_MSG(foundNode,"Insert Failed");
			m_totalCount++;
			return foundNode->GetData();		
		}
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	const DataType &PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::operator[](const CharacterType * str) const
	{
		EP_VERIFY_INVALID_ARGUMENT_W_MSG(str,"String is NULL");
		DataType retData;
		LockObj lock(m_lock);
		PatriciaTrieLeaf *foundNode=find(m_root,str,0,retData);
		EP_VERIFY_OUT_OF_RANGE(foundNode);
		return foundNode->GetData();
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	void PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::Clear()
	{
		LockObj lock(m_lock);
		m_totalCount=0;	
		if(m_root)
			EP_DELETE m_root;
		m_root=EP_NEW PatriciaTrieNode(Terminator);
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	bool PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::IsEmpty() const
	{
		LockObj lock(m_lock);
		if(m_totalCount)
			return true;
		return false;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	unsigned int PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::Size() const
	{
		LockObj lock(m_lock);
		return m_totalCount;
	}


	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	bool PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::Insert(const CharacterType* str,const DataType &data)
	{
		if(str!=NULL )
		{		
			LockObj lock(m_lock);
			if(insert(m_root,str,0,data))
			{
				m_totalCount++;
				return true;		
			}
		}
		return false;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	bool PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::Erase(const CharacterType* str)
	{
		if(str!=NULL)
		{
			LockObj lock(m_lock);
			if(erase(m_root,str,0))
			{
				m_totalCount--;
				return true;
			}
		}
		return false;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	bool PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::Find(const CharacterType* str, DataType &retData) const
	{
		if(str!=NULL)
		{
			LockObj lock(m_lock);
			if(find(m_root,str,0,retData))
			{
				return true;
			}
		}
		return false;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	bool PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::FindAll(const CharacterType* str, vector<Pair<const CharacterType*,DataType> > &retStrDataPairList) const
	{
		if(str!=NULL)
		{
			LockObj lock(m_lock);
			if(findAll(m_root,str,0,retStrDataPairList))
			{
				return true;
			}
		}
		return false;
	}


	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	typename PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieLeaf *PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::insert(BasePatriciaTrieNode *root,const CharacterType* str,unsigned int strTrav,const DataType &data)
	{
		if(m_mode==PATRICIA_TRIE_MODE_RECURSIVE)
		{
			if(root->IsLeaf())
			{
				return NULL;
			}
			else
			{
				PatriciaTrieNode *rootNode=static_cast<PatriciaTrieNode*>(root);
				BasePatriciaTrieNode *currentNode=(*rootNode)[str[strTrav]];
				if(currentNode)
				{
					return insert(currentNode,str,strTrav+1,data);
				}
				else
				{
					if(CharCompareFunc(&str[strTrav],&m_terminator)==COMP_RESULT_EQUAL)
					{
						return rootNode->AddLeaf(str,data);
					}
					else
					{
						rootNode->AddNode(str[strTrav]);
						return insert((*rootNode)[str[strTrav]],str,strTrav+1,data);
					}
				}
			}
		}
		else
		{
			return insertLoop(root,str,strTrav,data);
		}
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	typename PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieLeaf *PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::insertLoop(BasePatriciaTrieNode *root,const CharacterType* str,unsigned int strTrav,const DataType &data)
	{
		struct SnapShotStruct
		{
			BasePatriciaTrieNode *root;
			unsigned int strTrav;
		};

		stack<SnapShotStruct> snapshotStack;
		SnapShotStruct currentSnaptshot;
		currentSnaptshot.root=root;
		currentSnaptshot.strTrav=strTrav;
		snapshotStack.push(currentSnaptshot);
		PatriciaTrieLeaf *retNode=NULL;
		while(!snapshotStack.empty())
		{
			currentSnaptshot=snapshotStack.top();
			snapshotStack.pop();

			if(currentSnaptshot.root->IsLeaf())
			{
				retNode= NULL;
				continue;
			}
			else
			{
				PatriciaTrieNode *rootNode=static_cast<PatriciaTrieNode*>(currentSnaptshot.root);
				BasePatriciaTrieNode *currentNode=(*rootNode)[str[currentSnaptshot.strTrav]];
				if(currentNode)
				{
					SnapShotStruct newSnapshot;
					newSnapshot.root=currentNode;
					newSnapshot.strTrav=currentSnaptshot.strTrav+1;
					snapshotStack.push(newSnapshot);
					continue;
				}
				else
				{
					if(CharCompareFunc(&str[currentSnaptshot.strTrav],&m_terminator)==COMP_RESULT_EQUAL)
					{
						retNode=rootNode->AddLeaf(str,data);
						continue;
					}
					else
					{
						rootNode->AddNode(str[currentSnaptshot.strTrav]);
						SnapShotStruct newSnapshot;
						newSnapshot.root=(*rootNode)[str[currentSnaptshot.strTrav]];
						newSnapshot.strTrav=currentSnaptshot.strTrav+1;
						snapshotStack.push(newSnapshot);
						continue;
					}
				}
			}
		}
		return retNode;
	}


	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	bool PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::erase(BasePatriciaTrieNode *root,const CharacterType* str,unsigned int strTrav)
	{
		if(m_mode==PATRICIA_TRIE_MODE_RECURSIVE)
		{
			if(root->IsLeaf())
			{
				return true;
			}
			else
			{
				PatriciaTrieNode *rootNode=static_cast<PatriciaTrieNode*>(root);
				BasePatriciaTrieNode *currentNode=(*rootNode)[str[strTrav]];
				if(currentNode)
				{
					if(erase(currentNode,str,strTrav+1))
					{
						rootNode->RemoveNode(str[strTrav]);
						return true;
					}
				}
				return false;
			}
		}
		else
		{
			return eraseLoop(root,str,strTrav);
		}
	}



	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	bool PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::eraseLoop(BasePatriciaTrieNode *root,const CharacterType* str,unsigned int strTrav)
	{
		struct SnapShotStruct
		{
			BasePatriciaTrieNode *root;
			unsigned int strTrav;
			int stage;
		};

		stack<SnapShotStruct> snapshotStack;
		SnapShotStruct currentSnaptshot;
		currentSnaptshot.root=root;
		currentSnaptshot.strTrav=strTrav;
		currentSnaptshot.stage=0;
		snapshotStack.push(currentSnaptshot);
		bool retBool=false;
		while(!snapshotStack.empty())
		{
			currentSnaptshot=snapshotStack.top();
			snapshotStack.pop();

			switch(currentSnaptshot.stage)
			{
			case 0:
				if(currentSnaptshot.root->IsLeaf())
				{
					retBool=true;
					continue;
				}
				else
				{
					PatriciaTrieNode *rootNode=static_cast<PatriciaTrieNode*>(currentSnaptshot.root);
					BasePatriciaTrieNode *currentNode=(*rootNode)[str[currentSnaptshot.strTrav]];
					if(currentNode)
					{
						currentSnaptshot.stage=1;
						snapshotStack.push(currentSnaptshot);
						SnapShotStruct newSnapshot;
						newSnapshot.root=currentNode;
						newSnapshot.stage=0;
						newSnapshot.strTrav=currentSnaptshot.strTrav+1;
						snapshotStack.push(newSnapshot);
						continue;

					}

				}
				break;
			case 1:
				if(retBool)
				{
					PatriciaTrieNode *rootNode=static_cast<PatriciaTrieNode*>(currentSnaptshot.root);
					rootNode->RemoveNode(str[currentSnaptshot.strTrav]);
					retBool=true;
					continue;
				}
				break;
			}
			retBool=false;
			continue;			
		}
		return retBool;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	typename PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieLeaf *PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::find(BasePatriciaTrieNode *root,const CharacterType* str,unsigned int strTrav, DataType &retData) const
	{
		if(m_mode==PATRICIA_TRIE_MODE_RECURSIVE)
		{
			if(root->IsLeaf())
			{
				PatriciaTrieLeaf *leaf=static_cast<PatriciaTrieLeaf*>(root);
				retData=leaf->GetData();
				return leaf;
			}
			else
			{
				PatriciaTrieNode *rootNode=static_cast<PatriciaTrieNode*>(root);
				BasePatriciaTrieNode *currentNode=(*rootNode)[str[strTrav]];
				if(currentNode)
				{
					return find(currentNode,str,strTrav+1,retData);
				}
				else
				{
					return NULL;
				}
			}
		}
		else
		{
			return findLoop(root,str,strTrav,retData);
		}
	}


	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	typename PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieLeaf *PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::findLoop(BasePatriciaTrieNode *root,const CharacterType* str,unsigned int strTrav, DataType &retData) const
	{
		struct SnapShotStruct
		{
			BasePatriciaTrieNode *root;
			unsigned int strTrav;
		};

		stack<SnapShotStruct> snapshotStack;
		SnapShotStruct currentSnaptshot;
		currentSnaptshot.root=root;
		currentSnaptshot.strTrav=strTrav;
		snapshotStack.push(currentSnaptshot);
		PatriciaTrieLeaf *retNode=NULL;

		while(!snapshotStack.empty())
		{
			currentSnaptshot=snapshotStack.top();
			snapshotStack.pop();

			if(currentSnaptshot.root->IsLeaf())
			{
				PatriciaTrieLeaf *leaf=static_cast<PatriciaTrieLeaf*>(currentSnaptshot.root);
				retData=leaf->GetData();
				retNode= leaf;
				continue;
			}
			else
			{
				PatriciaTrieNode *rootNode=static_cast<PatriciaTrieNode*>(currentSnaptshot.root);
				BasePatriciaTrieNode *currentNode=(*rootNode)[str[currentSnaptshot.strTrav]];
				if(currentNode)
				{
					SnapShotStruct newSnapshot;
					newSnapshot.root=currentNode;
					newSnapshot.strTrav=currentSnaptshot.strTrav+1;
					snapshotStack.push(newSnapshot);
					continue;
				}
				else
				{
					retNode= NULL;
					continue;
				}
			}
		}
		return retNode;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	bool PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::findAll(BasePatriciaTrieNode *root,const CharacterType* str,unsigned int strTrav, vector<Pair<const CharacterType*,DataType> > &retStrDataPairList) const
	{
		if(m_mode==PATRICIA_TRIE_MODE_RECURSIVE)
		{
			if(root->IsLeaf())
			{
				PatriciaTrieLeaf *leaf=static_cast<PatriciaTrieLeaf*>(root);
				Pair<const CharacterType*,DataType> newPair(leaf->GetString(),leaf->GetData());
				retStrDataPairList.push_back(newPair);
				return true;
			}
			else
			{
				PatriciaTrieNode *rootNode=static_cast<PatriciaTrieNode*>(root);
				BasePatriciaTrieNode *currentNode=(*rootNode)[str[strTrav]];
				if(currentNode)
				{
					findAll(currentNode,str,strTrav+1,retStrDataPairList);
				}
				if(CharCompareFunc(&str[strTrav],&m_terminator)==COMP_RESULT_EQUAL)
				{
					for(int trav=0;trav<rootNode->GetList().size();trav++)
					{
						if(NodeCompareFunc(&m_terminator,&rootNode->GetList().at(trav))!=COMP_RESULT_EQUAL)
							traverseAll(rootNode->GetList().at(trav),retStrDataPairList);
					}
					
				}
				if(retStrDataPairList.size())
					return true;
				return false;
			}
		}
		else
		{
			return findAllLoop(root,str,strTrav,retStrDataPairList);
		}
	}


	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	bool PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::findAllLoop(BasePatriciaTrieNode *root,const CharacterType* str,unsigned int strTrav, vector<Pair<const CharacterType*,DataType> > &retStrDataPairList) const
	{
		struct SnapShotStruct
		{
			BasePatriciaTrieNode *root;
			unsigned int strTrav;
			BasePatriciaTrieNode *currentNode;
			int stage;
		};

		stack<SnapShotStruct> snapshotStack;
		SnapShotStruct currentSnaptshot;
		currentSnaptshot.root=root;
		currentSnaptshot.strTrav=strTrav;
		currentSnaptshot.currentNode=NULL;
		currentSnaptshot.stage= 0;
		snapshotStack.push(currentSnaptshot);
		bool retBool=false;

		while(!snapshotStack.empty())
		{
			currentSnaptshot=snapshotStack.top();
			snapshotStack.pop();


			switch(currentSnaptshot.stage)
			{
			case 0:
				if(currentSnaptshot.root->IsLeaf())
				{
					PatriciaTrieLeaf *leaf=static_cast<PatriciaTrieLeaf*>(currentSnaptshot.root);
					Pair<const CharacterType*,DataType> newPair(leaf->GetString(),leaf->GetData());
					retStrDataPairList.push_back(newPair);
					retBool=true;
					continue;
				}
				else
				{
					PatriciaTrieNode *rootNode=static_cast<PatriciaTrieNode*>(currentSnaptshot.root);
					BasePatriciaTrieNode *currentNode=(*rootNode)[str[currentSnaptshot.strTrav]];
					if(currentNode)
					{
						currentSnaptshot.currentNode=currentNode;
						currentSnaptshot.stage=1;
						snapshotStack.push(currentSnaptshot);
						SnapShotStruct newSnapshot;
						newSnapshot.root=currentNode;
						newSnapshot.currentNode=NULL;
						newSnapshot.stage=0;
						newSnapshot.strTrav=currentSnaptshot.strTrav+1;
						snapshotStack.push(newSnapshot);
						continue;
					}

					if(CharCompareFunc(&str[currentSnaptshot.strTrav],&m_terminator)==COMP_RESULT_EQUAL)
					{
						for(int trav=0;trav<rootNode->GetList().size();trav++)
						{
							if(NodeCompareFunc(&m_terminator,&rootNode->GetList().at(trav))!=COMP_RESULT_EQUAL)
								traverseAll(rootNode->GetList().at(trav),retStrDataPairList);
						}

					}
					if(retStrDataPairList.size())
					{
						retBool= true;
						continue;
					}
					retBool= false;
					continue;
				}
				break;
			case 1:
				PatriciaTrieNode *rootNode=static_cast<PatriciaTrieNode*>(currentSnaptshot.root);
				if(CharCompareFunc(&str[currentSnaptshot.strTrav],&m_terminator)==COMP_RESULT_EQUAL)
				{
					for(int trav=0;trav<rootNode->GetList().size();trav++)
					{
						if(NodeCompareFunc(&m_terminator,&rootNode->GetList().at(trav))!=COMP_RESULT_EQUAL)
							traverseAll(rootNode->GetList().at(trav),retStrDataPairList);
					}

				}
				if(retStrDataPairList.size())
				{
					retBool= true;
					continue;
				}
				retBool= false;
				continue;
				break;
			}
	
		}
		return retBool;
	}


	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	void PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::traverseAll(BasePatriciaTrieNode *root, vector<Pair<const CharacterType*,DataType> > &retStrDataPairList) const
	{
		if(m_mode==PATRICIA_TRIE_MODE_RECURSIVE)
		{
			if(root->IsLeaf())
			{
				PatriciaTrieLeaf *leaf=static_cast<PatriciaTrieLeaf*>(root);
				Pair<const CharacterType*,DataType> newPair(leaf->GetString(),leaf->GetData());
				retStrDataPairList.push_back(newPair);
				return;
			}
			else
			{
				PatriciaTrieNode *rootNode=static_cast<PatriciaTrieNode*>(root);
				for(int trav=0;trav<rootNode->GetList().size();trav++)
				{
					traverseAll(rootNode->GetList().at(trav),retStrDataPairList);
				}
			}
		}
		else
		{
			traverseAllLoop(root,retStrDataPairList);
		}
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	void PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::traverseAllLoop(BasePatriciaTrieNode *root, vector<Pair<const CharacterType*,DataType> > &retStrDataPairList) const
	{


		struct SnapShotStruct
		{
			BasePatriciaTrieNode *root;
		};

		stack<SnapShotStruct> snapshotStack;
		SnapShotStruct currentSnaptshot;
		currentSnaptshot.root=root;
		snapshotStack.push(currentSnaptshot);
		PatriciaTrieLeaf *retBool=false;

		while(!snapshotStack.empty())
		{
			currentSnaptshot=snapshotStack.top();
			snapshotStack.pop();

			if(currentSnaptshot.root->IsLeaf())
			{
				PatriciaTrieLeaf *leaf=static_cast<PatriciaTrieLeaf*>(currentSnaptshot.root);
				Pair<const CharacterType*,DataType> newPair(leaf->GetString(),leaf->GetData());
				retStrDataPairList.push_back(newPair);
			}
			else
			{
				PatriciaTrieNode *rootNode=static_cast<PatriciaTrieNode*>(currentSnaptshot.root);
				SnapShotStruct newSnapshot;
				for(int trav=0;trav<rootNode->GetList().size();trav++)
				{
					newSnapshot.root=rootNode->GetList().at(trav);
					snapshotStack.push(newSnapshot);
				}
			}
		}
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::BasePatriciaTrieNode::BasePatriciaTrieNode(CharacterType c, bool isLeaf)
	{
		m_character=c;
		m_isLeaf=isLeaf;
	}
	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::BasePatriciaTrieNode::BasePatriciaTrieNode(const BasePatriciaTrieNode &b)
	{
		m_character=b.m_character;
		m_isLeaf=b.m_isLeaf;
	}
	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::BasePatriciaTrieNode::~BasePatriciaTrieNode()
	{
	}
	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	typename PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::BasePatriciaTrieNode &PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::BasePatriciaTrieNode::operator=(const BasePatriciaTrieNode & b)
	{

		if(this!=&b)
		{
			m_character=b.m_character;
			m_isLeaf=b.m_isLeaf;
		}
		return *this;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	const CharacterType &PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::BasePatriciaTrieNode::GetCharacter() const
	{
		return m_character;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	void PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::BasePatriciaTrieNode::SetCharacter(const CharacterType & c)
	{
		m_character=c;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	bool PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::BasePatriciaTrieNode::IsLeaf() const
	{
		return m_isLeaf;
	}


	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieLeaf::PatriciaTrieLeaf(const CharacterType *str,const DataType &data):BasePatriciaTrieNode(Terminator,true)
	{
		m_data=data;
		m_terminator=Terminator;
		if(str!=NULL)
		{
			int trav=0;
			while(CharCompareFunc(&str[trav],&m_terminator)!=COMP_RESULT_EQUAL)
			{
				m_string.push_back(str[trav]);
				trav++;
			}
			m_string.push_back(m_terminator);
		}
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieLeaf::PatriciaTrieLeaf(const PatriciaTrieLeaf &b):BasePatriciaTrieNode(b)
	{
		m_data=b.m_data;
		m_string=b.m_string;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieLeaf::~PatriciaTrieLeaf()
	{
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	typename PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieLeaf &PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieLeaf::operator=(const PatriciaTrieLeaf & b)
	{
		if(this!=&b)
		{
			BasePatriciaTrieNode::operator =(b);
			m_data=b.m_data;
			m_string=b.m_string;
		}
		return *this;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	const DataType &PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieLeaf::GetData() const
	{
		return m_data;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	DataType &PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieLeaf::GetData()
	{
		return m_data;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	void PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieLeaf::SetData(const DataType &data)
	{
		m_data=data;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	const CharacterType *PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieLeaf::GetString() const
	{
		if(m_string.size())
		{
			return &m_string.at(0);
		}
		return NULL;
	}


	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieNode::PatriciaTrieNode(CharacterType c):BasePatriciaTrieNode(c)
	{
		m_terminator=Terminator;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieNode::PatriciaTrieNode(const PatriciaTrieNode &b):BasePatriciaTrieNode(b)
	{
		BasePatriciaTrieNode *tmpNode=NULL;
		m_terminator=b.m_terminator;
		for(int listTrav=0;listTrav<b.m_charList.size();listTrav++)
		{
			tmpNode=b.m_charList.at(listTrav);
			if(tmpNode)
			{
				if(tmpNode->IsLeaf())
				{
					PatriciaTrieLeaf *node=EP_NEW PatriciaTrieLeaf(*static_cast<PatriciaTrieLeaf*>(tmpNode));
					*node=*static_cast<PatriciaTrieLeaf*>(tmpNode);
					m_charList.push_back(node);
				}
				else
				{
					PatriciaTrieNode *node=EP_NEW PatriciaTrieNode(*static_cast<PatriciaTrieNode*>(tmpNode));
					*node=*static_cast<PatriciaTrieNode*>(tmpNode);
					m_charList.push_back(node);
				}
			}
		}

	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieNode::~PatriciaTrieNode()
	{
		BasePatriciaTrieNode *tmpNode=NULL;
		for(int listTrav=0;listTrav<m_charList.size();listTrav++)
		{
			tmpNode=m_charList.at(listTrav);
			if(tmpNode)
				EP_DELETE tmpNode;
			tmpNode=NULL;
		}
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	typename PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieNode &PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieNode::operator=(const PatriciaTrieNode & b)
	{
		if(this!=&b)
		{
			BasePatriciaTrieNode::operator =(b);
			m_terminator=b.m_terminator;
			BasePatriciaTrieNode *tmpNode=NULL;
			for(int listTrav=0;listTrav<b.m_charList.size();listTrav++)
			{
				tmpNode=b.m_charList.at(listTrav);
				if(tmpNode)
				{
					if(tmpNode->IsLeaf())
					{
						PatriciaTrieLeaf *node=EP_NEW PatriciaTrieLeaf(*static_cast<PatriciaTrieLeaf*>(tmpNode));
						*node=*static_cast<PatriciaTrieLeaf*>(tmpNode);
						m_charList.push_back(node);
					}
					else
					{
						PatriciaTrieNode *node=EP_NEW PatriciaTrieNode(*static_cast<PatriciaTrieNode*>(tmpNode));
						*node=*static_cast<PatriciaTrieNode*>(tmpNode);
						m_charList.push_back(node);
					}
				}
			}
		}
		return *this;
	}


	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	const vector<typename PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::BasePatriciaTrieNode *> &PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieNode::GetList() const
	{
		return m_charList;	
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	vector<typename PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::BasePatriciaTrieNode *> &PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieNode::GetList()
	{
		return m_charList;	
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	typename PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::BasePatriciaTrieNode *PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieNode::AddNode(const CharacterType c)
	{
		if(m_charList.size())
		{
			int retIdx;
			BasePatriciaTrieNode **existNode=BinarySearch(c,&(m_charList.at(0)),m_charList.size(),NodeCompareFunc,retIdx);
			if(existNode)
				return NULL;
			else
			{
				PatriciaTrieNode *newNode=EP_NEW PatriciaTrieNode(c);		
				m_charList.insert(m_charList.begin()+retIdx,newNode);
				return newNode;
			}
		}
		else
		{
			PatriciaTrieNode *newNode=EP_NEW PatriciaTrieNode(c);
			m_charList.push_back(newNode);
			return newNode;
		}
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	typename PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieLeaf *PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieNode::AddLeaf(const CharacterType *str,const DataType& data)
	{
		if(m_charList.size())
		{
			int retIdx;
			BasePatriciaTrieNode **existNode= BinarySearch(m_terminator,&(m_charList.at(0)),m_charList.size(),NodeCompareFunc,retIdx);
			if(existNode)
				return NULL;
			else
			{
				PatriciaTrieLeaf *newNode=EP_NEW PatriciaTrieLeaf(str,data);		
				m_charList.insert(m_charList.begin()+retIdx,newNode);
				return newNode;
			}
		}
		else
		{
			PatriciaTrieLeaf *newNode=EP_NEW PatriciaTrieLeaf(str,data);		
			m_charList.push_back(newNode);
			return newNode;
		}
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	bool PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieNode::RemoveNode(CharacterType c)
	{
		if(m_charList.size())
		{
			int retIdx;
			BasePatriciaTrieNode **existNode= BinarySearch(c,&(m_charList.at(0)),m_charList.size(),PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::NodeCompareFunc,retIdx);
			if(existNode)
			{
				if((*existNode)->IsLeaf())
				{
					EP_DELETE *existNode;
					m_charList.erase(m_charList.begin()+retIdx);
					return true;
				}
				else
				{
					PatriciaTrieNode *checkNode=static_cast<PatriciaTrieNode*>((*existNode));
					if(checkNode->GetList().size()==0)
					{
						EP_DELETE *existNode;
						m_charList.erase(m_charList.begin()+retIdx);
						return true;
					}
				}

			}

		}
		return false;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	typename PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::BasePatriciaTrieNode *PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieNode::operator[](const CharacterType &character)
	{
		if(m_charList.size())
		{
			int retIdx;
			BasePatriciaTrieNode ** retNode=BinarySearch(character,&(m_charList.at(0)),m_charList.size(),NodeCompareFunc,retIdx);
			if(retNode)
				return *retNode;
		}
		return NULL;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator, CompResultType (__cdecl *CharCompareFunc)(const void *,const void *)>
	typename const PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::BasePatriciaTrieNode *PatriciaTrie<CharacterType,DataType,Terminator,CharCompareFunc>::PatriciaTrieNode::operator[](const CharacterType &character) const
	{
		if(m_charList.size())
		{
			int retIdx;
			BasePatriciaTrieNode ** retNode=BinarySearch(character,&(m_charList.at(0)),m_charList.size(),NodeCompareFunc,retIdx);
			if(retNode)
				return *retNode;
		}
		return NULL;
	}
}
#endif //__EP_PATRICIA_TRIE_H__
