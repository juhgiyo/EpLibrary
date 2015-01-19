/*! 
@file epCrypt.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
			  <http://github.com/juhgiyo/eplibrary>
@date January 16, 2013
@brief Crypt Interface
@version 1.0

@section LICENSE

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

by Ing. Georg Hasenohrl 

Please refer to <http://www.codeguru.com/cpp/misc/misc/cryptoapi/article.php/c3899/How-to-EncryptDecrypt-a-String.htm> for the license.

-------------------------------------------------------------------------------

@section DESCRIPTION

An Interface for Crypt.

*/
#ifndef __EP_CRYPT_H__
#define __EP_CRYPT_H__

#include "epLib.h"

namespace epl
{
	/*! 
	@class Crypt epCrypt.h
	@brief A class for Crypt.
	*/
	class EP_LIBRARY Crypt
	{
	public:
		/*!
		Default Constructor

		Initializes the Crypt
		*/
		Crypt();
		
		/*!
		Default Destructor

		Destroy the Crypt
		*/
		virtual ~Crypt();

		/*!
		Decrypt the given encrypted password with given key
		@param[in] szEncryptPwd the encrypted password to decrypt.
		@param[out] szPassword the decrypted password
		@param[in] szKey the key to decrypt the password
		*/
		bool DecryptString(const TCHAR* szEncryptPwd,TCHAR* szPassword,const TCHAR *szKey);

		/*!
		Encrypt the given password with given key
		@param[in] szPassword the password to encrypt.
		@param[out] szEncryptPwd the encrypted password
		@param[in] szKey the key to encrypt the password
		*/
		bool EncryptString(const TCHAR* szPassword,TCHAR* szEncryptPwd,const TCHAR *szKey);
	private:
		/*!
		Setup the crypt
		*/
		bool setupCryptoClient();
		/// Initialize flag
		bool m_bInitOK;
	};

}

#endif //__EP_CRYPT_H__