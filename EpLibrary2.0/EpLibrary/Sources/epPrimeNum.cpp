/*! 
PrimeNum for the EpLibrary
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
#include "epFastSqrt.h"
#include "epPrimeNum.h"

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

