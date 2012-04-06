/*! 
@file epPatriciaTree.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date June 09, 2008
@brief Patricia Tree Interface
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

An Interface for Patricia Tree Template class.

*/
#ifndef __EP_PATRICIA_TREE_H__
#define __EP_PATRICIA_TREE_H__

#include "epFoundationLib.h"
#include "epBinarySearch.h"
#include <vector>
using namespace std;

namespace epl
{
	/*! 
	@class PatriciaTree epPatriciaTree.h
	@brief A Patricia Tree Template class.
	*/
	template<typename CharacterType, typename DataType , CharacterType Terminator=(CharacterType)0,bool isHoldingString=false, CompResultType (*CharCompareFunc)(const void *,const void *)=CompClass<CharacterType>::CompFunc>
	class PatriciaTree
	{
	public:

		/*!
		Default Constructor

		Initializes the  Patricia Tree
		@param[in] lockPolicyType The lock policy
		*/
		PatriciaTree(LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the Patricia Tree with given tree
		@param[in] b the Patricia Tree Object to copy from
		*/
		PatriciaTree(const PatriciaTree & b);

		/*!
		Default Destructor

		Destroys the Patricia Tree
		*/
		virtual ~PatriciaTree();

		/*!
		Initialize this tree to given tree
		@param[in] b the Patricia Tree structure to initialize this tree
		@return the result Patricia Tree
		*/
		PatriciaTree &operator=(const PatriciaTree & b);

		/*!
		Return the reference to data with given string if not exist insert and return the reference to data
		@param[in] str The string to return the reference to the data.
		@return the reference to the data with given string.
		*/
		DataType &operator[](const CharacterType * str);

		/*!
		Insert the string with given data to the tree
		@param[in] str The string value to insert.
		@param[in] data the data with the given string
		@return true if succeeded otherwise false
		*/
		bool Insert(const CharacterType* str,const DataType &data);

		/*!
		Remove the given string from the tree
		@param[in] str The string value to remove
		@return true if succeeded otherwise false
		*/
		bool Erase(const CharacterType* str);

		/*!
		Find the given string from the tree and return the data with the given string
		@param[in] str The string value to find
		@param[out] retData the data with the given string
		@return true if succeeded otherwise false
		*/
		bool Find(const CharacterType* str, DataType &retData) const;

		/*!
		Find the all strings starting with given string from the tree and return the string and data pairs
		@param[in] str The string value to find
		@param[out] retStrDataPairList the string and data pairs found
		@return true if succeeded otherwise false
		*/
		bool FindAll(const CharacterType* str, vector<Pair<const CharacterType*,DataType> > &retStrDataPairList) const;

		/*!
		Clear the tree
		*/
		void Clear();

	protected:

		/*! 
		@class BasePatriciaTreeNode epPatriciaTree.h
		@brief A Base Patricia Tree Node class.
		*/
		class BasePatriciaTreeNode
		{
		public:
			/*!
			Default Constructor

			Initializes the Base Patricia Tree Node
			@param[in] c the character of this node
			@param[in] isLeaf flag whether this node is a leaf or not
			*/
			BasePatriciaTreeNode(CharacterType c, bool isLeaf=false);

			/*!
			Default Copy Constructor

			Initializes the Base Patricia Tree Node with given node
			@param[in] b the Base Patricia Tree Node Object to copy from
			*/
			BasePatriciaTreeNode(const BasePatriciaTreeNode &b);

			/*!
			Default Destructor

			Destroys the Base Patricia Tree Node
			*/
			virtual ~BasePatriciaTreeNode();

			/*!
			Initialize this node to given node
			@param[in] b the Base Patricia Tree Node structure to initialize this node
			@return the result Base Patricia Tree Node
			*/
			BasePatriciaTreeNode &operator=(const BasePatriciaTreeNode & b);

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
		@class PatriciaTreeLeaf epPatriciaTree.h
		@brief A Patricia Tree Leaf class.
		*/
		class PatriciaTreeLeaf: public BasePatriciaTreeNode
		{
		public:
			/*!
			Default Constructor

			Initializes the Patricia Tree Leaf
			@param[in] str the string of this node
			@param[in] data the data of this node holding
			*/
			PatriciaTreeLeaf(const CharacterType *str,const DataType &data);

			/*!
			Default Copy Constructor

			Initializes the Patricia Tree Leaf with given leaf
			@param[in] b the Patricia Tree Leaf Object to copy from
			*/
			PatriciaTreeLeaf(const PatriciaTreeLeaf &b);

			/*!
			Default Destructor

			Destroys the Patricia Tree Leaf
			*/
			virtual ~PatriciaTreeLeaf();

			/*!
			Initialize this leaf to given leaf
			@param[in] b the Patricia Tree Leaf structure to initialize this leaf
			@return the result Patricia Tree Leaf
			*/
			PatriciaTreeLeaf &operator=(const PatriciaTreeLeaf & b);

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
		@class PatriciaTreeNode epPatriciaTree.h
		@brief A Patricia Tree Node class.
		*/
		class PatriciaTreeNode: public BasePatriciaTreeNode
		{

		public:
			/*!
			Default Constructor

			Initializes the Patricia Tree Node
			@param[in] c the character of this node
			*/
			PatriciaTreeNode(CharacterType c);

			/*!
			Default Copy Constructor

			Initializes the Patricia Tree Node with given node
			@param[in] b the Patricia Tree Node Object to copy from
			*/
			PatriciaTreeNode(const PatriciaTreeNode &b);

			/*!
			Default Destructor

			Destroys the Patricia Tree Node
			*/
			virtual ~PatriciaTreeNode();

			/*!
			Initialize this node to given node
			@param[in] b the Patricia Tree Node structure to initialize this node
			@return the result Patricia Tree Node
			*/
			PatriciaTreeNode &operator=(const PatriciaTreeNode & b);

			/*!
			Return the character list of this node holding
			@return the character list of this node
			*/
			const vector<BasePatriciaTreeNode *> &GetList() const;

			/*!
			Return the character list of this node holding
			@return the character list of this node
			*/
			vector<BasePatriciaTreeNode *> &GetList();

			/*!
			Add a new node with given character to the list of this node
			@param[in] c the character for the new node.
			@remark if the add fails then returns NULL.
			@return the pointer to the new node in the list
			*/
			BasePatriciaTreeNode *AddNode(const CharacterType c);

			/*!
			Add a new leaf with given character to the list of this node
			@param[in] str the string for the new node
			@param[in] data the data for the new node
			@remark if the add fails then returns NULL.
			@return the pointer to the new leaf in the list
			*/
			PatriciaTreeLeaf *AddLeaf(const CharacterType *str,const DataType& data);

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
			BasePatriciaTreeNode *operator[](const CharacterType &character);

		protected:
			/// character list of this node
			vector<BasePatriciaTreeNode*> m_charList;
			/// string terminator character holder
			CharacterType m_terminator;
		};

		/*!
		Actually insert the string with given data to the tree by recursive
		@param[in] root the root of the tree
		@param[in] str The string value to insert.
		@param[in] strTrav the current index of the string traversing
		@param[in] data the data with the given string
		@remark returns NULL if insertion fails
		@return Pointer to the node inserted.
		*/
		PatriciaTreeLeaf *insert(BasePatriciaTreeNode *root,const CharacterType* str,unsigned int strTrav,const DataType &data);

		/*!
		Actually remove the given string from the tree by recursive
		@param[in] root the root of the tree
		@param[in] str The string value to erase.
		@param[in] strTrav the current index of the string traversing
		@return true if succeeded otherwise false
		*/
		bool erase(BasePatriciaTreeNode *root,const CharacterType* str,unsigned int strTrav);

		/*!
		Actually find the given string from the tree by recursive and return the data
		@param[in] root the root of the tree
		@param[in] str The string value to find.
		@param[in] strTrav the current index of the string traversing
		@param[out] retData the data with the given string to return
		@remark returns NULL if find fails
		@return Pointer to the node found.
		*/
		PatriciaTreeLeaf * find(BasePatriciaTreeNode *root,const CharacterType* str,unsigned int strTrav, DataType &retData) const;

		/*!
		Actually find the strings starts with the given string from the tree by recursive
		@param[in] root the root of the tree
		@param[in] str The string value to find.
		@param[in] strTrav the current index of the string traversing
		@param[out] retStrDataPairList the string and data pair which the string starts with the given string.
		@return true if succeeded otherwise false.
		*/
		bool findAll(BasePatriciaTreeNode *root,const CharacterType* str,unsigned int strTrav, vector<Pair<const CharacterType*,DataType> > &retStrDataPairList) const;

		/*!
		Traverse the all nodes and return the all string and data pair from the tree by recursive
		@param[in] root the root of the tree
		@param[out] retStrDataPairList the string and data pair in the tree.
		@return true if succeeded otherwise false.
		*/
		void traverseAll(BasePatriciaTreeNode *root, vector<Pair<const CharacterType*,DataType> > &retStrDataPairList) const;

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
			BasePatriciaTreeNode **_b=reinterpret_cast<BasePatriciaTreeNode**>(_vB);
			return CharCompareFunc(_a,&((*_b)->GetCharacter()));
		}


		/// Root of the tree
		PatriciaTreeNode* m_root;
		/// Total number of strings in the tree
		unsigned int m_totalCount;             
		/// String Terminator holder
		CharacterType m_terminator;
		/// lock
		BaseLock *m_lock;
		/// Lock Policy
		LockPolicy m_lockPolicy;

	};




	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTree(LockPolicy lockPolicyType)
	{
		m_totalCount=0;
		m_root=EP_NEW PatriciaTreeNode(Terminator);
		m_terminator=Terminator;
		m_lockPolicy=lockPolicyType;
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

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTree(const PatriciaTree & b)
	{
		m_totalCount=b.m_totalCount;
		m_root=EP_NEW PatriciaTreeNode(Terminator);
		*m_root=*(b.m_root);
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

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::~PatriciaTree()
	{
		m_lock->Lock();
		if(m_root)
			EP_DELETE m_root;
		m_lock->Unlock();
		if(m_lock)
			EP_DELETE m_lock;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc> &PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::operator=(const PatriciaTree & b)
	{
		if(this!=&b)
		{
			LockObj lock(m_lock);
			m_totalCount=b.m_totalCount;
			if(m_root)
				EP_DELETE m_root;
			m_root=EP_NEW PatriciaTreeNode(Terminator);
			*m_root=*(b.m_root);
			m_terminator=b.m_terminator;
		}
		return *this;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	DataType &PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::operator[](const CharacterType * str)
	{

		EP_VERIFY_INVALID_ARGUMENT_W_MSG(str,"String is NULL");
		DataType retData;
		LockObj lock(m_lock);
		PatriciaTreeLeaf *foundNode=find(m_root,str,0,retData);
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

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	void PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::Clear()
	{
		LockObj lock(m_lock);
		m_totalCount=0;	
		if(m_root)
			EP_DELETE m_root;
		m_root=EP_NEW PatriciaTreeNode(Terminator);
	}


	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	bool PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::Insert(const CharacterType* str,const DataType &data)
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

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	bool PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::Erase(const CharacterType* str)
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

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	bool PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::Find(const CharacterType* str, DataType &retData) const
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

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	bool PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::FindAll(const CharacterType* str, vector<Pair<const CharacterType*,DataType> > &retStrDataPairList) const
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


	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	typename PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeLeaf *PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::insert(BasePatriciaTreeNode *root,const CharacterType* str,unsigned int strTrav,const DataType &data)
	{
		if(root->IsLeaf())
		{
			return NULL;
		}
		else
		{
			PatriciaTreeNode *rootNode=static_cast<PatriciaTreeNode*>(root);
			BasePatriciaTreeNode *currentNode=(*rootNode)[str[strTrav]];
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

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	bool PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::erase(BasePatriciaTreeNode *root,const CharacterType* str,unsigned int strTrav)
	{
		if(root->IsLeaf())
		{
			return true;
		}
		else
		{
			PatriciaTreeNode *rootNode=static_cast<PatriciaTreeNode*>(root);
			BasePatriciaTreeNode *currentNode=(*rootNode)[str[strTrav]];
			if(currentNode)
			{
				if(erase(currentNode,str,strTrav+1))
				{
					if(CharCompareFunc(&str[strTrav],&m_terminator)!=COMP_RESULT_EQUAL)
					{		
						rootNode->RemoveNode(str[strTrav+1]);
						return true;
					}
					return true;
				}
				else
					return false;
			}
			else
			{
				return false;
			}
		}
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	typename PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeLeaf *PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::find(BasePatriciaTreeNode *root,const CharacterType* str,unsigned int strTrav, DataType &retData) const
	{
		if(root->IsLeaf())
		{
			PatriciaTreeLeaf *leaf=static_cast<PatriciaTreeLeaf*>(root);
			retData=leaf->GetData();
			return leaf;
		}
		else
		{
			PatriciaTreeNode *rootNode=static_cast<PatriciaTreeNode*>(root);
			BasePatriciaTreeNode *currentNode=(*rootNode)[str[strTrav]];
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

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	bool PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::findAll(BasePatriciaTreeNode *root,const CharacterType* str,unsigned int strTrav, vector<Pair<const CharacterType*,DataType> > &retStrDataPairList) const
	{
		if(root->IsLeaf())
		{
			PatriciaTreeLeaf *leaf=static_cast<PatriciaTreeLeaf*>(root);
			Pair<const CharacterType*,DataType> newPair(leaf->GetString(),leaf->GetData());
			retStrDataPairList.push_back(newPair);
			return true;
		}
		else
		{
			PatriciaTreeNode *rootNode=static_cast<PatriciaTreeNode*>(root);
			BasePatriciaTreeNode *currentNode=(*rootNode)[str[strTrav]];
			if(currentNode)
			{
				if(findAll(currentNode,str,strTrav+1,retStrDataPairList))
				{
					if(CharCompareFunc(&str[strTrav+1],&m_terminator)==COMP_RESULT_EQUAL)
					{
						PatriciaTreeNode *travNode=static_cast<PatriciaTreeNode*>(currentNode);
						for(int trav=0;trav<travNode->GetList().size();trav++)
						{
							if(NodeCompareFunc(&m_terminator,travNode->GetList().at(trav))==COMP_RESULT_EQUAL)
								traverseAll(travNode->GetList().at(trav),retStrDataPairList);
						}
					}
					return true;
				}
				else
				{
					if(CharCompareFunc(&str[strTrav+1],&m_terminator)==COMP_RESULT_EQUAL)
					{
						PatriciaTreeNode *travNode=static_cast<PatriciaTreeNode*>(currentNode);
						for(int trav=0;trav<travNode->GetList().size();trav++)
						{
							if(NodeCompareFunc(&m_terminator,travNode->GetList().at(trav))==COMP_RESULT_EQUAL)
								traverseAll(travNode->GetList().at(trav),retStrDataPairList);
						}
						if(retStrDataPairList.size())
							return true;
					}
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}
	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	void PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::traverseAll(BasePatriciaTreeNode *root, vector<Pair<const CharacterType*,DataType> > &retStrDataPairList) const
	{
		if(root->IsLeaf())
		{
			PatriciaTreeLeaf *leaf=static_cast<PatriciaTreeLeaf*>(root);
			Pair<const CharacterType*,DataType> newPair(leaf->GetString(),leaf->GetData());
			retStrDataPairList.push_back(newPair);
			return;
		}
		else
		{
			PatriciaTreeNode *rootNode=static_cast<PatriciaTreeNode*>(root);
			for(int trav=0;trav<rootNode->GetList().size();trav++)
			{
				traverseAll(rootNode->GetList().at(trav),retStrDataPairList);
			}
		}
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::BasePatriciaTreeNode::BasePatriciaTreeNode(CharacterType c, bool isLeaf)
	{
		m_character=c;
		m_isLeaf=isLeaf;
	}
	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::BasePatriciaTreeNode::BasePatriciaTreeNode(const BasePatriciaTreeNode &b)
	{
		m_character=b.m_character;
		m_isLeaf=b.m_isLeaf;
	}
	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::BasePatriciaTreeNode::~BasePatriciaTreeNode()
	{
	}
	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	typename PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::BasePatriciaTreeNode &PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::BasePatriciaTreeNode::operator=(const BasePatriciaTreeNode & b)
	{

		if(this!=&b)
		{
			m_character=b.m_character;
			m_isLeaf=b.m_isLeaf;
		}
		return *this;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	const CharacterType &PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::BasePatriciaTreeNode::GetCharacter() const
	{
		return m_character;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	void PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::BasePatriciaTreeNode::SetCharacter(const CharacterType & c)
	{
		m_character=c;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	bool PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::BasePatriciaTreeNode::IsLeaf() const
	{
		return m_isLeaf;
	}


	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeLeaf::PatriciaTreeLeaf(const CharacterType *str,const DataType &data):BasePatriciaTreeNode(Terminator,true)
	{
		m_data=data;
		m_terminator=Terminator;
		if(isHoldingString && str!=NULL)
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

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeLeaf::PatriciaTreeLeaf(const PatriciaTreeLeaf &b):BasePatriciaTreeNode(b)
	{
		m_data=b.m_data;
		m_string=b.m_string;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeLeaf::~PatriciaTreeLeaf()
	{
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	typename PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeLeaf &PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeLeaf::operator=(const PatriciaTreeLeaf & b)
	{
		if(this!=&b)
		{
			m_data=b.m_data;
			m_string=b.m_string;
			BasePatriciaTreeNode::operator =(b);
		}
		return *this;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	const DataType &PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeLeaf::GetData() const
	{
		return m_data;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	DataType &PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeLeaf::GetData()
	{
		return m_data;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	void PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeLeaf::SetData(const DataType &data)
	{
		m_data=data;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	const CharacterType *PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeLeaf::GetString() const
	{
		if(m_string.size())
		{
			return &m_string.at(0);
		}
		return NULL;
	}


	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeNode::PatriciaTreeNode(CharacterType c):BasePatriciaTreeNode(c)
	{
		m_terminator=Terminator;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeNode::PatriciaTreeNode(const PatriciaTreeNode &b):BasePatriciaTreeNode(b)
	{
		BasePatriciaTreeNode *tmpNode=NULL;
		m_terminator=b.m_terminator;
		for(int listTrav=0;listTrav<b.m_charList.size();listTrav++)
		{
			tmpNode=b.m_charList.at(listTrav);
			if(tmpNode)
			{
				if(tmpNode->IsLeaf())
				{
					PatriciaTreeLeaf *node=EP_NEW PatriciaTreeLeaf(*static_cast<PatriciaTreeLeaf*>(tmpNode));
					*node=*static_cast<PatriciaTreeLeaf*>(tmpNode);
					m_charList.push_back(node);
				}
				else
				{
					PatriciaTreeNode *node=EP_NEW PatriciaTreeNode(*static_cast<PatriciaTreeNode*>(tmpNode));
					*node=*static_cast<PatriciaTreeNode*>(tmpNode);
					m_charList.push_back(node);
				}
			}
		}

	}

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeNode::~PatriciaTreeNode()
	{
		BasePatriciaTreeNode *tmpNode=NULL;
		for(int listTrav=0;listTrav<m_charList.size();listTrav++)
		{
			tmpNode=m_charList.at(listTrav);
			if(tmpNode)
				EP_DELETE tmpNode;
			tmpNode=NULL;
		}
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	typename PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeNode &PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeNode::operator=(const PatriciaTreeNode & b)
	{
		if(this!=&b)
		{
			m_terminator=b.m_terminator;
			BasePatriciaTreeNode *tmpNode=NULL;
			for(int listTrav=0;listTrav<b.m_charList.size();listTrav++)
			{
				tmpNode=b.m_charList.at(listTrav);
				if(tmpNode)
				{
					if(tmpNode->IsLeaf())
					{
						PatriciaTreeLeaf *node=EP_NEW PatriciaTreeLeaf(*static_cast<PatriciaTreeLeaf*>(tmpNode));
						*node=*static_cast<PatriciaTreeLeaf*>(tmpNode);
						m_charList.push_back(node);
					}
					else
					{
						PatriciaTreeNode *node=EP_NEW PatriciaTreeNode(*static_cast<PatriciaTreeNode*>(tmpNode));
						*node=*static_cast<PatriciaTreeNode*>(tmpNode);
						m_charList.push_back(node);
					}
				}
			}
			BasePatriciaTreeNode::operator =(b);
		}
		return *this;
	}


	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	const vector<typename PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::BasePatriciaTreeNode *> &PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeNode::GetList() const
	{
		return m_charList;	
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	vector<typename PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::BasePatriciaTreeNode *> &PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeNode::GetList()
	{
		return m_charList;	
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	typename PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::BasePatriciaTreeNode *PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeNode::AddNode(const CharacterType c)
	{
		if(m_charList.size())
		{
			int retIdx;
			BasePatriciaTreeNode **existNode=BinarySearch(c,&(m_charList.at(0)),m_charList.size(),NodeCompareFunc,retIdx);
			if(existNode)
				return NULL;
			else
			{
				PatriciaTreeNode *newNode=EP_NEW PatriciaTreeNode(c);		
				m_charList.insert(m_charList.begin()+retIdx,newNode);
				return newNode;
			}
		}
		else
		{
			PatriciaTreeNode *newNode=EP_NEW PatriciaTreeNode(c);
			m_charList.push_back(newNode);
			return newNode;
		}
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	typename PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeLeaf *PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeNode::AddLeaf(const CharacterType *str,const DataType& data)
	{
		if(m_charList.size())
		{
			int retIdx;
			BasePatriciaTreeNode **existNode= BinarySearch(m_terminator,&(m_charList.at(0)),m_charList.size(),NodeCompareFunc,retIdx);
			if(existNode)
				return NULL;
			else
			{
				PatriciaTreeLeaf *newNode=EP_NEW PatriciaTreeLeaf(str,data);		
				m_charList.insert(m_charList.begin()+retIdx,newNode);
				return newNode;
			}
		}
		else
		{
			PatriciaTreeLeaf *newNode=EP_NEW PatriciaTreeLeaf(str,data);		
			m_charList.push_back(newNode);
			return newNode;
		}
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	bool PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeNode::RemoveNode(CharacterType c)
	{
		if(m_charList.size())
		{
			int retIdx;
			BasePatriciaTreeNode **existNode= BinarySearch(m_terminator,&(m_charList.at(0)),m_charList.size(),PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::NodeCompareFunc,retIdx);
			if(existNode)
			{
				if((*existNode)->IsLeaf())
				{
					EP_DELETE existNode;
					m_charList.erase(m_charList.begin()+retIdx);
					return true;
				}
				else
				{
					PatriciaTreeNode *checkNode=static_cast<PatriciaTreeNode*>((*existNode));
					if(checkNode->GetList().size()==0)
					{
						EP_DELETE existNode;
						m_charList.erase(m_charList.begin()+retIdx);
						return true;
					}
				}

			}

		}
		return false;
	}

	template<typename CharacterType, typename DataType , CharacterType Terminator,bool isHoldingString, CompResultType (*CharCompareFunc)(const void *,const void *)>
	typename PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::BasePatriciaTreeNode *PatriciaTree<CharacterType,DataType,Terminator,isHoldingString,CharCompareFunc>::PatriciaTreeNode::operator[](const CharacterType &character)
	{
		if(m_charList.size())
		{
			int retIdx;
			BasePatriciaTreeNode ** retNode=BinarySearch(character,&(m_charList.at(0)),m_charList.size(),NodeCompareFunc,retIdx);
			if(retNode)
				return *retNode;
		}
		return NULL;
	}
}
#endif //__EP_PATRICIA_TREE_H__
