/*! 
Random for the EpLibrary
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
#include "epRandom.h"
#include <stdlib.h>
#include <time.h>
#include "epSystem.h"
#include "epException.h"
using namespace epl;

int Random::GetRandom()
{
	return rand();
}
int Random::GetRandom(int startNum, int endNum)
{
	EP_ASSERT(endNum>=startNum);
	return rand()%(endNum-startNum+1)+startNum;
}
Random::Random()
{
 srand(time(NULL));
}
Random::~Random()
{

}