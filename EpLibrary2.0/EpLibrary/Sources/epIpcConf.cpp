/*! 
IpcConf for the EpLibrary
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
#include "epIpcConf.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;




PipeWriteElem::PipeWriteElem(epl::LockPolicy lockPolicyType):SmartObject(lockPolicyType)
{
	m_dataSize=0;
	m_data=NULL;
}
PipeWriteElem::PipeWriteElem(unsigned int dataSize,epl::LockPolicy lockPolicyType):SmartObject(lockPolicyType)
{
	m_dataSize=dataSize;
	m_data=EP_NEW char[m_dataSize];
}
PipeWriteElem::~PipeWriteElem()
{
	if(m_data)
		EP_DELETE m_data;
}