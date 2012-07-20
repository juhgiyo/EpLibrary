/*! 
@file epServerObjectList.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date July 20, 2012
@brief Server Object List Interface
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

An Interface for Server Object List.

*/
#ifndef __EP_SERVER_OBJECT_LIST_H__
#define __EP_SERVER_OBJECT_LIST_H__

#include "epFoundationLib.h"
#include "epSystem.h"
#include "epBaseServerObject.h"

#include <vector>


using namespace std;


namespace epl{

	/*! 
	@class ServerObjectList epServerObjectList.h
	@brief A class for Server Object List.
	*/
	class EP_FOUNDATION ServerObjectList{

	public:
		/*!
		Default Constructor

		Initializes the List
		@param[in] lockPolicyType The lock policy
		*/
		ServerObjectList(LockPolicy lockPolicyType=EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the List
		@param[in] b the second object
		*/
		ServerObjectList(const ServerObjectList& b);
		/*!
		Default Destructor

		Destroy the Client
		*/
		virtual ~ServerObjectList();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		ServerObjectList & operator=(const ServerObjectList&b)
		{
			if(this!=&b)
			{
				LockObj lock(m_listLock);
				m_objectList=b.m_objectList;
			}
			return *this;
		}

		/*!
		Remove all object which its thread is terminated
		@remark it also releases the object
		*/
		void RemoveTerminated();

	
		/*!
		Remove all object
		@remark it also releases the object
		*/
		void Clear();

		/*!
		Push the new object to the list
		@param[in] obj the object to push in
		*/
		void Push(BaseServerObject* obj);
		
		/*!
		Returns the list in vector
		@return the list of the objects
		*/
		vector<BaseServerObject*> GetList() const;

	private:
	
		/// list lock
		BaseLock *m_listLock;

		/// parser thread list
		vector<BaseServerObject*> m_objectList;

		/// Lock Policy
		LockPolicy m_lockPolicy;

	};
	
}


#endif //__EP_SERVER_OBJECT_LIST_H__