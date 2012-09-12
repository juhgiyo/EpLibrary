/*! 
@file epRegistryHelper.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date October 01, 2011
@brief Registry Processing Function Class Interface
@version 2.0

@section LICENSE

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

@section DESCRIPTION

An Interface for the Registry Processing Operation.

*/
#ifndef __EP_REGISTRY_HELPER_H__
#define __EP_REGISTRY_HELPER_H__
#include "epLib.h"
#include "epSystem.h"

namespace epl
{
	/*! 
	@class RegistryHelper epRegistryHelper.h
	@brief This is a class for Registry Processing Class

	Implements the Registry Processing Functions.
	*/
	class EP_LIBRARY RegistryHelper
	{
	public:
		/*!
		Set the given registry string data to given registry name
		@param[in] key the registry mode ex. (HKEY_LOCAL_MACHINE)
		@param[in] subKey the subkey within the registry mode ex. ("SOFTWARE\\WINDOWS\\")
		@param[in] regName the name of the registry to write the data
		@param[in] regData the string data to write
		@return true if successful, otherwise false
		*/
		static bool SetRegistryData(HKEY key,const TCHAR * subKey,const TCHAR *regName,const TCHAR *regData);

		/*!
		Set the given registry data to given registry name
		@param[in] key the registry mode ex. (HKEY_LOCAL_MACHINE)
		@param[in] subKey the subkey within the registry mode ex. ("SOFTWARE\\WINDOWS\\")
		@param[in] regName the name of the registry to write the data
		@param[in] regType the type of the data ex. (REG_SZ)
		@param[in] regData the data to write
		@param[in] sizeInByte the size of data in Byte
		@return true if successful, otherwise false
		*/
		static bool SetRegistryData(HKEY key,const TCHAR *subKey,const TCHAR * regName,unsigned long regType,void * regData,unsigned int sizeInByte);

		/*!
		Get the given registry string data of given registry name
		@param[in] key the registry mode ex. (HKEY_LOCAL_MACHINE)
		@param[in] subKey the subkey within the registry mode ex. ("SOFTWARE\\WINDOWS\\")
		@param[in] regName the name of the registry to read the data
		@param[out] retString the string data read
		@return true if successful, otherwise false
		*/
		static bool GetRegistryData(HKEY key,const TCHAR * subKey,const TCHAR * regName, EpTString &retString);


		/*!
		Get the given registry data of given registry name
		@param[in] key the registry mode ex. (HKEY_LOCAL_MACHINE)
		@param[in] subKey the subkey within the registry mode ex. ("SOFTWARE\\WINDOWS\\")
		@param[in] regName the name of the registry to read the data
		@param[in] sizeInByte the size of the retBuf in Byte
		@param[out] retBuf the buffer for writing the data read
		@param[out] retSizeReadInByte the size of data read in Byte
		@param[out] retRegType the type of the data read ex. (REG_SZ)
		@return true if successful, otherwise false
		*/
		static bool GetRegistryData(HKEY key,const TCHAR * subKey,const TCHAR * regName,unsigned int sizeInByte,void *retBuf, unsigned int &retSizeReadInByte, unsigned long &retRegType);

		/*!
		Get the approximately larger size of the data in Registry
		@param[in] key the registry mode ex. (HKEY_LOCAL_MACHINE)
		@param[in] subKey the subkey within the registry mode ex. ("SOFTWARE\\WINDOWS\\")
		@param[in] regName the name of the registry to get the data size
		@return the data size of the given registry name
		*/
		static unsigned int GetRegistryDataSize(HKEY key,const TCHAR * subKey,const TCHAR * regName);

		/*!
		Delete the given registry value
		@param[in] key the registry mode
		@param[in] subkey the subkey within the registry mode
		@param[in] regValue the registry value to be deleted
		*/
		static void DeleteRegistryValue(HKEY key, const TCHAR *subkey,const TCHAR * regValue);

		/*!
		Delete the given registry key
		@param[in] key the registry mode
		@param[in] subkey the subkey within the registry mode to be deleted
		*/
		static void DeleteRegistryKey(HKEY key, const TCHAR * subkey);

		/*!
		Return the registry mode of given registry path
		@param[in] strRegPath the complete registry path
		@return the registry mode
		*/
		static HKEY GetRegistryMode(const TCHAR *strRegPath);
	};
}


#endif //__EP_REGISTRY_HELPER_H__