/*! 
Endian for the EpLibrary

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