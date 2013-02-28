/*! 
CmdLine Options for the EpLibrary
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

------------------------------------------------------------------------------

Part of this software is

CCmdLine

A utility for parsing command lines.

Copyright (C) 1999 Chris Losinger, Smaller Animals Software.
http://www.smalleranimals.com

This software is provided 'as-is', without any express
or implied warranty.  In no event will the authors be 
held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software 
for any purpose, including commercial applications, and 
to alter it and redistribute it freely, subject to the 
following restrictions:

1. The origin of this software must not be misrepresented; 
you must not claim that you wrote the original software. 
If you use this software in a product, an acknowledgment 
in the product documentation would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, 
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source 
distribution.

Please refer to <http://www.codeproject.com/Articles/1205/CCmdLine-command-line-parser> for the license.
*/

#include "epCmdLineOptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace epl;

CmdLineOptions::CmdLineOptions():CmdLineMap()
{

}
CmdLineOptions::~CmdLineOptions()
{

}
CmdLineOptions::CmdLineOptions(const CmdLineOptions& b):CmdLineMap(b)
{

}
CmdLineOptions &CmdLineOptions::operator=(const CmdLineOptions & b)
{
	if(this!=&b)
	{
		CmdLineMap::operator =(b);
	}
	return *this;
}
unsigned int CmdLineOptions::Parse(int argc, TCHAR **argv)
{
	clear();

	EpTString curSector;
	for(int argTrav=1;argTrav<argc;argTrav++)
	{
		if(isOption(argv[argTrav]))
		{
			curSector=argv[argTrav];

			CmdArgs args;
			while(argTrav+1<argc && !isOption(argv[argTrav+1]) )
			{
				args.m_args.push_back(argv[argTrav+1]);
				argTrav++;
			}
			insert(CmdLineOptions::value_type(curSector,args));
			
		}
	}
	return size();
}

bool CmdLineOptions::HasOption(const TCHAR *option) const
{
	return (find(option)!=end());
}

EpTString CmdLineOptions::GetArgument(const TCHAR *option,unsigned int idx, const TCHAR *defaultArg) const
{
	EpTString retString=_T("");
	if(defaultArg!=NULL)
		retString=defaultArg;
	try
	{
		retString=GetArgument(option,idx);
	}
	catch (...)
	{
	}
	return defaultArg;
}

EpTString CmdLineOptions::GetArgument(const TCHAR *option,unsigned int idx) const
{
	CmdLineMap::const_iterator iter;
	if((iter=find(option))!=end())
	{
		if(iter->second.m_args.size()>idx)
		{
			return iter->second.m_args.at(idx);
		}
	}
	throw (int)0;

	return EpTString(_T(""));
}

int CmdLineOptions::GetArgumentCount(const TCHAR *option) const
{
	int retCount=-1;
	CmdLineMap::const_iterator iter;
	if((iter=find(option))!=end())
	{
		retCount=(int)iter->second.m_args.size();
	}
	return retCount;

}

bool CmdLineOptions::isOption(const TCHAR *option) const
{
	if(option==NULL)
		return false;

	EpTString checkSectorString=option;
	checkSectorString=Locale::Trim(checkSectorString);

	if(checkSectorString.size()>1)
	{
		if(checkSectorString.at(0)==_T('-'))
		{
			return Locale::IsDigit(checkSectorString.at(1));
		}
	}
	return false;
}