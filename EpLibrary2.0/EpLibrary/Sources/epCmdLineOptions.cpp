/*! 
CmdLine Options for the EpLibrary

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

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

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
size_t CmdLineOptions::Parse(int argc, TCHAR **argv)
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

EpTString CmdLineOptions::GetArgument(const TCHAR *option,size_t idx, const TCHAR *defaultArg) const
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
	return retString;
}

EpTString CmdLineOptions::GetArgument(const TCHAR *option,size_t idx) const
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
			return !Locale::IsDigit(checkSectorString.at(1));
		}
	}
	return false;
}