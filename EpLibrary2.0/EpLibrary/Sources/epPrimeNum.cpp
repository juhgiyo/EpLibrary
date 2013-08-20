/*! 
PrimeNum for the EpLibrary

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
*/
#include "epFastSqrt.h"
#include "epPrimeNum.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;

bool PrimeNum::IsPrime(unsigned int x)
{
	if(x<=1)
		return false;
	if( x==2 || x==3)
		return true;
	if(x%2==0 || x%3==0)
		return false;
	unsigned int sqrtValue=FastSqrt::Sqrt(x);
	unsigned int trav=1;
	unsigned int multP1=(trav*6)+1;
	unsigned int multP2=(trav*6)-1;
	while(multP2<=sqrtValue)
	{
		if(x%multP1==0 || x%multP2==0)
			return false;
		trav++;
		multP1=(trav*6)+1;
		multP2=(trav*6)-1;
	}
	if(multP1<=sqrtValue && x%multP1==0)
		return false;
	return true;
}

unsigned int PrimeNum::NextPrime(unsigned int x)
{
	int multIdx=x/6;
	unsigned int check1;
	unsigned int check2;
	while(1)
	{
		check1=multIdx*6-1;
		if(check1>x && epl::PrimeNum::IsPrime(check1))
		{
			return check1;
		}
		check2=multIdx*6+1;
		if(check2>x &&epl::PrimeNum::IsPrime(check2))
			return check2;
		multIdx++;
	}
}

