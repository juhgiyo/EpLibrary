/*! 
NoLock for the EpLibrary
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
#include "epNoLock.h"
#include "epSystem.h"
#include "epException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace epl;


NoLock::NoLock() :BaseLock()
{
}
NoLock::NoLock(const NoLock& b):BaseLock()
{}

NoLock::~NoLock()
{
}
NoLock & NoLock::operator=(const NoLock&b)
{
	return *this;
}

bool NoLock::Lock()
{
	return true;
}

long NoLock::TryLock()
{
	return 1;
}
long NoLock::TryLockFor(const unsigned int dwMilliSecond)
{
	return 1;	
}
void NoLock::Unlock()
{
}

