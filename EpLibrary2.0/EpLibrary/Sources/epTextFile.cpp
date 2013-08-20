/*! 
TextFile for the EpLibrary

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
#include "epTextFile.h"
#include "epException.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;

TextFile::TextFile(FileEncodingType encodingType, LockPolicy lockPolicyType):BaseTextFile(encodingType,lockPolicyType)
{
}

TextFile::TextFile(const TextFile& b):BaseTextFile(b)
{
	LockObj lock(b.m_baseTextLock);
	m_text=b.m_text;
}

TextFile::~TextFile()
{
}
TextFile & TextFile::operator=(const TextFile&b)
{
	if(this!=&b)
	{
		BaseTextFile::operator =(b);
		LockObj lock(b.m_baseTextLock);
		m_text=b.m_text;

	}
	return *this;
}

void TextFile::SetText(const TCHAR * val)
{
	LockObj lock(m_baseTextLock);
	m_text=val;
}

EpTString TextFile::GetText() const
{
	LockObj lock(m_baseTextLock);
	return m_text;
}

void TextFile::Clear()
{
	LockObj lock(m_baseTextLock);
	m_text=_T("");
}
void TextFile::writeLoop()
{
	writeToFile(m_text.c_str());
}

void TextFile::loadFromFile(const EpTString &lines)
{
	m_text=lines;
}
