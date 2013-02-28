/*! 
Endian for the EpLibrary
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
#include "epEndian.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;
short Endian::Swap(const short value)
{
	unsigned char b1, b2;

	b1 = value & 255;
	b2 = (value >> 8) & 255;

	return (b1 << 8) + b2;
}
unsigned short Endian::Swap(const unsigned short value)
{
	unsigned char b1, b2;

	b1 = value & 255;
	b2 = (value >> 8) & 255;

	return (b1 << 8) + b2;
}
int Endian::Swap(const int value)
{
	unsigned char b1, b2, b3, b4;

	b1 = value & 255;
	b2 = ( value >> 8 ) & 255;
	b3 = ( value>>16 ) & 255;
	b4 = ( value>>24 ) & 255;

	return (static_cast<int>(b1) << 24) + (static_cast<int>(b2) << 16) + (static_cast<int>(b3) << 8) + b4;
}
unsigned int Endian::Swap(const unsigned int value)
{
	unsigned char b1, b2, b3, b4;

	b1 = value & 255;
	b2 = ( value >> 8 ) & 255;
	b3 = ( value>>16 ) & 255;
	b4 = ( value>>24 ) & 255;

	return (static_cast<unsigned int>(b1) << 24) + (static_cast<unsigned int>(b2) << 16) + (static_cast<unsigned int>(b3) << 8) + b4;
}
float Endian::Swap(const float value)
{
	union
	{
		float f;
		unsigned char b[4];
	} dat1, dat2;

	dat1.f = value;
	dat2.b[0] = dat1.b[3];
	dat2.b[1] = dat1.b[2];
	dat2.b[2] = dat1.b[1];
	dat2.b[3] = dat1.b[0];
	return dat2.f;
}
double Endian::Swap(const double value)
{
	union
	{
		double d;
		unsigned char b[8];
	} dat1, dat2;

	dat1.d = value;
	dat2.b[0] = dat1.b[7];
	dat2.b[1] = dat1.b[6];
	dat2.b[2] = dat1.b[5];
	dat2.b[3] = dat1.b[4];
	dat2.b[4] = dat1.b[3];
	dat2.b[5] = dat1.b[2];
	dat2.b[6] = dat1.b[1];
	dat2.b[7] = dat1.b[0];
	return dat2.d;
}