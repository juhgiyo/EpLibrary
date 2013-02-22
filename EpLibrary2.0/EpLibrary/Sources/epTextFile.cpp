/*! 
TextFile for the EpLibrary
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
#include "epTextFile.h"
#include "epException.h"
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
