/*! 
FastLog for the EpLibrary

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

#include "epFastLog.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;

#define TYPE_SIGNED      2
#define TYPE_UNSIGNED    1
#define NUM_BITS_IN_BYTE 8

#define FLOAT_INPUT_MIN_VAL    1

#define UPPER 1
#define LOWER 0

unsigned int fastLog(unsigned int input, unsigned int shift_init, int bound)
{
	if(input==1)
		return 0;

	unsigned int cal_Input = input;
	unsigned int calTest;
	unsigned int finalvalue=0;
	unsigned int shifter;
	for(shifter = shift_init; shifter>0; shifter--)
	{
		calTest=(cal_Input>>shifter);
		if(calTest)
		{
			unsigned int checkAbs= calTest<<shifter;
			if(checkAbs ^ input)
			{
				finalvalue=checkAbs<<bound;
			}
			else
			{
				finalvalue=checkAbs;
			}
			break;

		}
	}
	switch(finalvalue)
	{
	case 0x00000002:
		return 1;
	case 0x00000004:
		return 2;
	case 0x00000008:
		return 3;
	case 0x00000010:
		return 4;
	case 0x00000020:
		return 5;
	case 0x00000040:
		return 6;
	case 0x00000080:
		return 7;
	case 0x00000100:
		return 8;
	case 0x00000200:
		return 9;
	case 0x00000400:
		return 10;
	case 0x00000800:
		return 11;
	case 0x00001000:
		return 12;
	case 0x00002000:
		return 13;
	case 0x00004000:
		return 14;
	case 0x00008000:
		return 15;
	case 0x00010000:
		return 16;
	case 0x00020000:
		return 17;
	case 0x00040000:
		return 18;
	case 0x00080000:
		return 19;
	case 0x00100000:
		return 20;
	case 0x00200000:
		return 21;
	case 0x00400000:
		return 22;
	case 0x00800000:
		return 23;
	case 0x01000000:
		return 24;
	case 0x02000000:
		return 25;
	case 0x04000000:
		return 26;
	case 0x08000000:
		return 27;
	case 0x10000000:
		return 28;
	case 0x20000000:
		return 29;
	case 0x40000000:
		return 30;
	case 0x80000000:
		return 31;
	default:
		return 32;
	}
}

unsigned int FastLog::FLogU(unsigned int input)
{
	if(input<FLOAT_INPUT_MIN_VAL)
		return FLOG_ERROR;
	return fastLog(input,sizeof(unsigned int)*NUM_BITS_IN_BYTE-TYPE_UNSIGNED, UPPER );
}

int FastLog::FLogU(int input)
{
	if(input<FLOAT_INPUT_MIN_VAL)
		return FLOG_ERROR;
	return static_cast<int>(fastLog(static_cast<unsigned int>(input),sizeof(int)*NUM_BITS_IN_BYTE-TYPE_SIGNED, UPPER));
}

unsigned short FastLog::FLogU(unsigned short input)
{
	if(input<FLOAT_INPUT_MIN_VAL)
		return FLOG_ERROR;
	return static_cast<unsigned short>(fastLog(static_cast<unsigned int>(input), sizeof(unsigned short)*NUM_BITS_IN_BYTE-TYPE_UNSIGNED, UPPER) );
}

short FastLog::FLogU(short input)
{
	if(input<FLOAT_INPUT_MIN_VAL)
		return FLOG_ERROR;
	return static_cast<short>( fastLog(static_cast<unsigned int>(input), sizeof(short)*NUM_BITS_IN_BYTE-TYPE_SIGNED, UPPER) );
}


unsigned int FastLog::FLogL(unsigned int input)
{
	if(input<FLOAT_INPUT_MIN_VAL)
		return FLOG_ERROR;
	return fastLog(input,sizeof(unsigned int)*NUM_BITS_IN_BYTE-TYPE_UNSIGNED, LOWER );
}

int FastLog::FLogL(int input)
{
	if(input<FLOAT_INPUT_MIN_VAL)
		return FLOG_ERROR;
	return static_cast<int>( fastLog(static_cast<unsigned int>(input),sizeof(int)*NUM_BITS_IN_BYTE-TYPE_SIGNED, LOWER) );
}

unsigned short FastLog::FLogL(unsigned short input)
{
	if(input<FLOAT_INPUT_MIN_VAL)
		return FLOG_ERROR;
	return static_cast<unsigned short>( fastLog(static_cast<unsigned int>(input), sizeof(unsigned short)*NUM_BITS_IN_BYTE-TYPE_UNSIGNED, LOWER) );
}

short FastLog::FLogL(short input)
{
	if(input<FLOAT_INPUT_MIN_VAL)
		return FLOG_ERROR;
	return static_cast<short>( fastLog(static_cast<unsigned int>(input), sizeof(short)*NUM_BITS_IN_BYTE-TYPE_SIGNED, LOWER) );
}

