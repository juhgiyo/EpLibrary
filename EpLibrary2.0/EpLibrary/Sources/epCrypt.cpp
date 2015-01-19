/*! 
Crypt for the EpLibrary

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
*/

#include "epCrypt.h"
#include "epSystem.h"
#include <wincrypt.h>
#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;


Crypt::Crypt()
{
	m_bInitOK = false;
	m_bInitOK = setupCryptoClient();
}

Crypt::~Crypt()
{

}

bool Crypt::setupCryptoClient()
{
	// Ensure that the default cryptographic client is set up.	
	HCRYPTPROV hProv;
	HCRYPTKEY hKey;		
	// Attempt to acquire a handle to the default key container.
	if (!CryptAcquireContext(&hProv, NULL, MS_DEF_PROV, PROV_RSA_FULL, 0))	
	{
		// Some sort of error occured, create default key container.
		if (!CryptAcquireContext(&hProv, NULL, MS_DEF_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET))
		{	
			// Error creating key container!			
			return false;		
		}	
	}
	// Attempt to get handle to signature key.
	if (!CryptGetUserKey(hProv, AT_SIGNATURE, &hKey))	
	{
		if (GetLastError() == NTE_NO_KEY)		
		{			
			// Create signature key pair.
			if (!CryptGenKey(hProv, AT_SIGNATURE, 0, &hKey))			
			{
				// Error during CryptGenKey!				
				CryptReleaseContext(hProv, 0);
				return false;			
			}			
			else			
			{				
				CryptDestroyKey(hKey);			
			}		
		}		
		else 		
		{
			// Error during CryptGetUserKey!			
			CryptReleaseContext(hProv, 0);
			return false;		
		}	
	}	

	// Attempt to get handle to exchange key.
	if (!CryptGetUserKey(hProv,AT_KEYEXCHANGE,&hKey))	
	{
		if (GetLastError()==NTE_NO_KEY)		
		{			
			// Create key exchange key pair.
			if (!CryptGenKey(hProv,AT_KEYEXCHANGE,0,&hKey))			
			{
				// Error during CryptGenKey!				
				CryptReleaseContext(hProv, 0);
				return false;			
			}			
			else			
			{				
				CryptDestroyKey(hKey);			
			}		
		}		
		else		
		{
			// Error during CryptGetUserKey!			
			CryptReleaseContext(hProv, 0);
			return false;		
		}	
	}	

	CryptReleaseContext(hProv, 0);	
	return true;
}


bool Crypt::EncryptString(const TCHAR* szPassword,TCHAR* szEncryptPwd,const TCHAR *szKey)
{	

	// was intialize OK?
	if (!m_bInitOK)
		return false;

	bool bResult = true;	
	HKEY hRegKey = NULL;	
	HCRYPTPROV hProv = NULL;	
	HCRYPTKEY hKey = NULL;
	HCRYPTKEY hXchgKey = NULL;	
	HCRYPTHASH hHash = NULL;	
	DWORD dwLength;

	// Get handle to user default provider.
	if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0))	
	{
		// Create hash object.		
		if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))		
		{
			// Hash password string.			
			dwLength = sizeof(TCHAR)*_tcslen(szKey);
			if (CryptHashData(hHash, (BYTE *)szKey, dwLength, 0))			
			{
				// Create block cipher session key based on hash of the password.
				if (CryptDeriveKey(hProv, CALG_RC4, hHash, CRYPT_EXPORTABLE, &hKey))				
				{
					// Determine number of bytes to encrypt at a time.
					dwLength = sizeof(TCHAR)*_tcslen(szPassword);					
					// Allocate memory.
					BYTE *pbBuffer = (BYTE *)malloc(dwLength);					
					if (pbBuffer != NULL)					
					{
						memcpy(pbBuffer, szPassword, dwLength);						
						// Encrypt data
						if (CryptEncrypt(hKey, 0, TRUE, 0, pbBuffer, &dwLength, dwLength)) 						
						{
							// return encrypted string
							memcpy(szEncryptPwd, pbBuffer, dwLength);
							EpTString hexString=System::HexToString((unsigned char*)szEncryptPwd,_tcslen(szEncryptPwd));
							memcpy(szEncryptPwd,hexString.c_str(),hexString.length()*sizeof(TCHAR));
						}	
						else						
						{							
							bResult = false;						
						}						
						// Free memory
						free(pbBuffer);					
					}
					else					
					{						
						bResult = false;					
					}
					CryptDestroyKey(hKey);  // Release provider handle.				
				}				
				else				
				{
					// Error during CryptDeriveKey!					
					bResult = false;				
				}			
			}			
			else			
			{
				// Error during CryptHashData!				
				bResult = false;			
			}
			CryptDestroyHash(hHash); 
			// Destroy session key.		
		}		
		else		
		{
			// Error during CryptCreateHash!			
			bResult = false;		
		}
		CryptReleaseContext(hProv, 0);	
	}	
	return bResult;
}


bool Crypt::DecryptString(const TCHAR* szEncryptPwd,TCHAR* szPassword,const TCHAR *szKey) 
{	
	// was initalise OK?
	if (!m_bInitOK)
		return false;

	bool bResult = true;	
	HCRYPTPROV hProv = NULL;		
	HCRYPTKEY hKey = NULL;		
	HCRYPTKEY hXchgKey = NULL;
	HCRYPTHASH hHash = NULL;
	TCHAR szPasswordTemp[32] = _T("");
	DWORD dwLength;

	// Get handle to user default provider.
	if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0))		
	{
		// Create hash object.			
		if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
		{				
			// Hash password string.
			dwLength = sizeof(TCHAR)*_tcslen(szKey);
			if (CryptHashData(hHash, (BYTE *)szKey, dwLength, 0))				
			{
				// Create block cipher session key based on hash of the password.
				if (CryptDeriveKey(
					hProv, CALG_RC4, hHash, CRYPT_EXPORTABLE, &hKey))					
				{
					EpTString inputPwd=szEncryptPwd;
					unsigned char *tempPwd=new unsigned char[inputPwd.length()+sizeof(TCHAR)];
					memset(tempPwd,0,inputPwd.length()+sizeof(TCHAR));
					System::StringToHex(szEncryptPwd,tempPwd);
					TCHAR *encryptPwd=(TCHAR*)tempPwd;

					// we know the encrypted password and the length
					dwLength = sizeof(TCHAR)*_tcslen(encryptPwd);						
					// copy encrypted password to temporary TCHAR
					_tcscpy(szPasswordTemp,encryptPwd);
					if (!CryptDecrypt(
						hKey, 0, TRUE, 0, (BYTE *)szPasswordTemp, &dwLength))
						bResult = FALSE;						
					CryptDestroyKey(hKey);  // Release provider handle.					
					// copy decrypted password to outparameter
					_tcscpy(szPassword,szPasswordTemp);
					delete[] tempPwd;
				}					
				else					
				{
					// Error during CryptDeriveKey!						
					bResult = false;					
				}				
			}				
			else
			{					
				// Error during CryptHashData!					
				bResult = false;				
			}
			CryptDestroyHash(hHash); // Destroy session key.			
		}			
		else			
		{
			// Error during CryptCreateHash!				
			bResult = false;			
		}
		CryptReleaseContext(hProv, 0);		
	}		
	return bResult;
}
